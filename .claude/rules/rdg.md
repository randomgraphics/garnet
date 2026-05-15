---
paths:
  - "src/core/rdg/**"
  - "src/inc/garnet/rdg/**"
  - "src/inc/garnet/GNrdg.h"
  - "src/inc/garnet/GNrdg2.h"
---

# RDG Module

Render Dependency Graph. The primary actively-developed rendering abstraction. Namespace: `GN::rdg` (v1) and `GN::rdg2` (v2). Always include `GNrdg.h` or `GNrdg2.h` — never include sub-headers directly.

## Two Generations

| | V1 (`GN::rdg`) | V2 (`GN::rdg2`) |
|---|---|---|
| Public include | `GNrdg.h` | `GNrdg2.h` (also pulls in `GNrdg.h` and `GNgpu2.h`) |
| Source | `src/core/rdg/1/` | `src/core/rdg/2/` |
| Public headers | `src/inc/garnet/rdg/1/` | `src/inc/garnet/rdg/2/` |
| Status | Stable, Vulkan backend | WIP, Vulkan-only proof-of-concept |
| GPU layer | Self-contained (`GpuContext`, `Texture`, etc. in `artifacts.h`) | Delegates to `GN::gpu2` |

## RDG V1 — 4-Level Architecture

### Level 1 — Dependency Graph (`rdg/1/dependency-graph.h`)

Pure task-graph engine; no GPU semantics.

| Type | Role |
|---|---|
| `Artifact` | Base of everything: `RefCounter` + `RuntimeType` + `name` |
| `Arguments` | Parameters for one action invocation; must implement `addToReadWriteList()` |
| `Action` | Unit of work; implement `prepare()` + `execute()` |
| `Workflow` | Ordered sequence of `(Action, Arguments)` tasks; move-only |
| `RenderGraph` | Creates workflows; `submit()` executes them in topological order |
| `Submission` | Handle to a submitted batch; query `isFinished()` / `result()` |

Dependency rule: task A depends on task B if A is newer and they share a read/write artifact conflict (write→read, read→write, or write→write).

### Level 2 — GPU IR (`rdg/1/artifacts.h`, `rdg/1/actions.h`)

API-agnostic GPU resource and action types.

**Artifacts (resources):**
- `GpuContext` — wraps D3D12/Vulkan device; `GpuContext::create({.api="vulkan"})`
- `Backbuffer` — swapchain; must be prepared (`PrepareBackbuffer`) before drawing and presented (`PresentBackbuffer`) after
- `Texture` — 2D/3D/cube texture; `Texture::create()` / `Texture::load()`
- `PersistentBuffer` — device-local, lives across submissions
- `TransientBuffer` — host-visible, one-shot per submission; allocate via `TransientArena::allocate()`
- `GpuResourceView` — view into any resource (image view, buffer view, or sampler)
- `RenderTarget` — set of color + depth/stencil targets with blend/depth/stencil/viewport state

**Actions:**
- `ClearRenderTarget` — must precede any draw to the same render target
- `PrepareBackbuffer` / `PresentBackbuffer` — frame boundary actions; always used in pairs
- `GpuDraw` — rasterization draw; provides VS/HS/DS/GS/PS shaders + `GpuGeometry` + resource table
- `GpuCompute` — compute dispatch
- `GpuCopy` — `BufferToBuffer` or `BufferToImage` (uses `TransientBuffer` as source for uploads)
- `SetupRenderStates` — sets fill mode, cull mode, front face

### Level 3 — Pipeline Library (`rdg/1/pipeline.h`, `rdg/1/physical.h`, `rdg/1/transform.h`)

Rendering effects and subgraph builders. Key type: `SubGraph` — a self-contained bundle of workflows submittable to a `RenderGraph`.

Key effects: `SharedShaderConstants`, `PbrShading`, `SkyBox`.

### Level 4 — Scene (`rdg/1/scene.h`)

Placeholder only. Not implemented.

## RDG V2 — Open Graph (`GN::rdg2`)

Ground-up redesign. Generic DAG executor with no baked-in GPU semantics; GPU work is layered on top via `GN::gpu2`.

### Graph Engine Types (`rdg/2/open-graph.h`)

| Type | Role |
|---|---|
| `Entity` | Base of everything: `RefCounter` + `RuntimeType` + 128-bit `id` + `name` |
| `Action` | Unit of work; implement `execute(Arguments*)`; or use `Action::createFromLambda()` |
| `Arguments` | Parameters passed to an action |
| `Token` (opaque) | Synchronization handle; satisfied when its associated condition is met |
| `Node` (opaque) | Scheduled execution slot; lifecycle: Blocked → Ready → Running → FinishedAction → Completed |
| `Artifact` (opaque) | Versioned named payload; published and waited on by nodes |
| `Graph` | Executor; `Graph::create()` returns the concrete single-worker implementation |

**Node lifecycle:**
- Add via `graph->addNode(NodeDesc{...}.setAction(...).dependsOn(token))`
- Nodes become Ready when all dependency tokens are satisfied
- Single-worker pump executes the highest-priority ready node
- `manualComplete = true` → node stays in FinishedAction until `graph->satisfyNode(node)` is called externally (for GPU fence waits)
- `waitForIdle()` returns `BUSY` when called from inside an executing node (re-entrancy guard)

**Artifact versioning:**
- `NeverOverflowingCounter` — 128-bit counter; `OOO()` = "next published version", `ONE()` = "published at least once"
- `graph->getArtifactVersionToken(artifact, OOO())` → token satisfied on next publish
- `graph->publishArtifact(artifact, std::any{value})` → increments version, satisfies waiting tokens

**Scheduling:** `SchedulingClass` (CRITICAL < NORMAL < BACKGROUND < IDLE) → `priority` int → insertion order.

### RDG V2 Supplemental Types

- `rdg/2/physical2.h` — type aliases from v1 (`Nanoseconds`, `Candela`, `IntensityRGB`, etc.)
- `rdg/2/transform2.h` — transform/camera types (imported from v1 during transition)
- `rdg/2/shared-shader-constants2.h` — `SharedShaderConstants` for v2

## Object Model Conventions (both V1 and V2)

- Use `AutoRef<T>` exclusively; raw owning pointers are prohibited.
- Type casting: `RuntimeType::cast<T>(ptr)`, never `dynamic_cast`.
- Register with `GN_REGISTER_RUNTIME_TYPE(DirectParentClass)` — must be the direct parent, not a grandparent.
- Factory: `T::create(name, params)` returns `AutoRef<T>`; returns empty ref on failure, never throws.

## V1 vs V2 GPU Resource Naming

V1 (`GN::rdg`) and V2 (`GN::gpu2`, used by `GN::rdg2`) have parallel but distinct types with similar names:

| Concept | V1 type | V2 type |
|---|---|---|
| GPU device | `rdg::GpuContext` | `gpu2::GpuContext` |
| Texture | `rdg::Texture` | `gpu2::Texture` |
| Buffer | `rdg::PersistentBuffer` / `rdg::TransientBuffer` | `gpu2::Buffer` |
| Swapchain | `rdg::Backbuffer` | `gpu2::Swapchain` |
| Resource view | `rdg::GpuResourceView` | `gpu2::GpuResourceView` |
| Render target | `rdg::RenderTarget` | `gpu2::RasterTarget` |
| Draw call | `rdg::GpuDraw` (Action) | `gpu2::GpuRaster` (records multiple draws) |

Do not mix v1 and v2 GPU resource types.

## Branching Workflow

`feature/rdg/main` is the staging branch for gpu2 and rdg development — treat it as this module's "main".

### Day-to-day development

- **Feature work**: branch off `feature/rdg/main` as `wip/gpu2/<topic>` (gpu2 work) or `wip/rdg2/<topic>` (rdg2 work). Develop there; squash-merge back to `feature/rdg/main` when done; delete the wip branch.
- **Small fixes**: commit directly to `feature/rdg/main`.
- Never branch wip branches off `master` for gpu2/rdg work.

### Submodule discipline

- Submodules we own or actively modify (e.g. `rapid-vulkan`, other `rapid-*` repos) must live on their own `wip/<topic>` branch during development — never on their `main`/`master` directly.
- `garnet master` must only ever reference a submodule's `main`/`master` commit, never a wip commit. This invariant must hold at all times on `master`.

### Promoting feature/rdg/main → garnet master

1. For every modified submodule, open a PR and land all changes to that submodule's own `main`/`master` first.
2. Update `feature/rdg/main`'s submodule pointers to the new `main`/`master` commits and verify everything builds and tests pass.
3. Open a PR from `feature/rdg/main` → `master`. **Do not delete `feature/rdg/main` after the PR lands** — it is a permanent staging branch.
4. After the PR lands, do a one-way merge from `master` → `feature/rdg/main` to pull in any master-side changes (CI fixups, version bumps, etc.) and keep the staging branch fully in sync.

## File Layout

| Location | Role |
|---|---|
| `src/inc/garnet/GNrdg.h` | V1 public monolithic include |
| `src/inc/garnet/GNrdg2.h` | V2 public monolithic include (also includes GNrdg.h + GNgpu2.h) |
| `src/inc/garnet/rdg/1/dependency-graph.h` | Level 1: task graph engine |
| `src/inc/garnet/rdg/1/artifacts.h` | Level 2: GPU resource types |
| `src/inc/garnet/rdg/1/actions.h` | Level 2: GPU action types |
| `src/inc/garnet/rdg/1/physical.h` | Level 3: physical units, timing types |
| `src/inc/garnet/rdg/1/transform.h` | Level 3: transform, camera, projection |
| `src/inc/garnet/rdg/1/pipeline.h` | Level 3: SubGraph, PbrShading, SkyBox, SharedShaderConstants |
| `src/inc/garnet/rdg/1/scene.h` | Level 4: placeholder |
| `src/inc/garnet/rdg/2/open-graph.h` | V2 graph engine: Entity, Action, Graph, Token, Artifact, Node |
| `src/inc/garnet/rdg/2/physical2.h` | V2 physical/timing aliases (imported from v1) |
| `src/inc/garnet/rdg/2/transform2.h` | V2 transform aliases (imported from v1) |
| `src/inc/garnet/rdg/2/shared-shader-constants2.h` | V2 shared shader constants |
| `src/core/rdg/1/` | V1 implementation (Vulkan backend + common) |
| `src/core/rdg/2/open-graph.cpp` | V2 graph engine implementation |
| `src/core/rdg/2/shared-shader-constants2.cpp` | V2 shared constants implementation |

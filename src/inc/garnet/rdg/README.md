# Garnet RDG — Render Dependency Graph

This document describes the design philosophy and four-level architecture of the
`GN::rdg` module. All headers in this directory implement one of these four levels.
Understanding the level of each type is the key to using the module correctly and
extending it cleanly.

> **Note:** This design is actively evolving. Decisions documented here represent
> current best thinking, not final truth. If something seems wrong or has a better
> solution, question it.

---

## The Four Levels

```
┌─────────────────────────────────────────────────────────┐
│  Level 4 — Scene Management        (scene.h)            │  ← future / placeholder
├─────────────────────────────────────────────────────────┤
│  Level 3 — Pipeline / Effect Library (pipeline.h, ...)  │  ← rendering effects, subgraph builders
├─────────────────────────────────────────────────────────┤
│  Level 2 — GPU Render Graph IR  (artifacts.h, actions.h)│  ← thin GPU abstraction, generic workflows
├─────────────────────────────────────────────────────────┤
│  Level 1 — Dependency Graph    (dependency-graph.h)     │  ← pure task graph, no GPU semantics
└─────────────────────────────────────────────────────────┘
```

Each level depends only on levels below it. Code in `src/core/rdg/` (the backend)
may implement any level, but the public headers in `src/inc/garnet/rdg/` must respect
the layering: no Level 3 concept may appear in a Level 2 header, and no GPU-API-
specific type (`VkBuffer`, `ID3D12Resource`, `MTLBuffer`, etc.) may appear in any
public header at any level.

---

## Level 1 — Dependency Graph (`dependency-graph.h`)

**What it is:** A pure, generic task-graph engine. Zero GPU semantics. Could in
principle be used to schedule any asynchronous workload.

**Key types:**
- `Artifact` — the base class of every named, database-tracked object in the graph.
- `Action` — holds logic for one operation; declares its read/write artifact set.
- `Arguments` — one-time-use parameter bundle passed to an `Action` at execution.
  Each concrete subclass declares which artifacts it reads or writes via
  `addToReadWriteList()`.
- `Workflow` — an ordered sequence of `(Action, Arguments)` tasks.
  Tasks within a workflow run in strict sequential order.
- `RenderGraph` — creates workflows; submits them for execution in topological
  dependency order (based on artifact read/write declarations).
- `Submission` — represents an in-flight or completed batch of workflows.

**Dependency rule:** Task A depends on task B if A is newer than B (later in
submission order) and their artifact sets overlap in a read-after-write,
write-after-read, or write-after-write pattern.

**Conventions at this level:**
- No GPU, shader, or rendering concept of any kind.
- Name/sequence lookup and deduplication are the responsibility of higher-level layers (e.g. scene management), not RDG.
- `Arguments` objects are single-use: create, pass to `RenderGraph`, discard.

---

## Level 2 — GPU Render Graph IR (`artifacts.h`, `actions.h`)

**What it is:** A thin, API-agnostic abstraction layer over GPU operations. It is
the "intermediate representation" (IR) of the GPU pipeline: common enough that
most GPU workflows can be expressed through it, but free of any higher-level
scene, effect, or pipeline policy.

> Think of this level as a portable command encoding API, not an effect system.
> Any rendering effect *can* be built on top of it, but a complex effect *may*
> bypass it for performance if needed.

**Key artifacts** (`artifacts.h`):
- `GpuContext` — a GPU device handle (Vulkan instance/device, D3D12 device, etc.).
- `Buffer` — a GPU buffer. Created with a size and usage flags; content is set
  separately via an upload action (see below).
- `Texture` — a 2D/3D/cube GPU texture.
- `Backbuffer` — the swapchain surface for presentation.
- `GpuResourceGroup` — a group of shader-bindable resources (maps to a Vulkan
  descriptor set, D3D12 root descriptor table, etc.).
- `RenderTarget` — describes color and depth/stencil outputs for a draw operation.

**Key actions** (`actions.h`):
- `ClearRenderTarget` — explicitly clears a render target.  Normally not needed:
  the first `GpuDraw` to a render target clears it automatically.  Use this only
  when there are no draw commands at all, or to clear mid-pass.
- `PrepareBackbuffer` / `PresentBackbuffer` — swapchain lifecycle.
- `GpuDraw` — generic indexed/instanced draw call with shader binaries and resource
  bindings. The building block for all render effects.
- `GpuCompute` — generic dispatch for compute shaders.
- `GpuBufferUpload` — uploads CPU data into a GPU buffer (legacy ring-slot model).
  See *Dynamic Data Upload* below.
- `CopyBuffer` — stateless buffer-to-buffer copy.  The preferred building block
  for CPU→GPU uploads when combined with a `TransientBuffer`.
- `CopyBufferToImage` — stateless buffer-to-image copy (stub, future work).

**Naming note:** These files may be renamed `gpu-artifacts.h` / `gpu-actions.h` in
a future refactor to make the level boundary visually obvious.

**Rules at this level:**
- No scene, material, effect, or rendering-strategy concepts.
- No API-specific types in public headers (`VkBuffer`, `ID3D12Resource`, etc.).
- The interface expresses *what* to do, not *how* the backend does it.

### Dynamic Data Upload — Level 2 Design

GPU buffers holding per-frame data (camera matrices, light positions, per-draw
transforms, etc.) must be updated by the CPU every frame, without stalling the CPU
waiting for the GPU to finish the previous frame. `GpuBufferUpload` provides three
upload mechanisms with different trade-offs. **The choice of mechanism belongs to
Level 3** — Level 2 just defines them and documents the trade-offs.

#### Mechanism A: BLOB

```
CPU data  ──copy──▶  owned blob  ──copy──▶  GPU buffer
             (at action-build time)   (at execute time)
```

- CPU data is copied into an owned blob when the action's arguments are built.
  The original pointer is not retained.
- The blob is kept alive until the GPU finishes reading it.
- The GPU buffer is written from the blob at execute time (a second copy).
- **Pros:** Data lifetime is fully automatic. Caller can discard the source
  immediately after building the arguments. Simple to reason about.
- **Cons:** Two copies — potentially wasteful for large or frequently-updated data.
- **Best for:** Small, infrequently changing data where simplicity matters more
  than throughput.

#### Mechanism B: HOST_MAP (host-mapped direct write)

```
CPU data  ──write──▶  host-visible GPU buffer   (read directly by GPU)
             (1 copy, or 0 with write-callback)
```

- The backend allocates a ring of N host-visible GPU buffers.
- At execute time, the backend maps the next available slot and either:
  - calls `memcpy(slot, data, size)` (raw-pointer path: 1 copy), or
  - calls `writeFn(slot, capacity)` (callback path: 0 copies if the user writes
    directly into the mapped memory).
- The slot is ring-managed: before reuse, the backend verifies the GPU is no
  longer reading it (via a per-slot GPU-completion token, internal to the backend).
- **Pros:** Minimum CPU-side copies. Write-callback path is zero-copy. Simple
  execution model — no GPU-side copy command needed.
- **Cons:** Host-visible memory (PCI-E aperture or shared system memory) may be
  slower for GPU reads than device-local memory, especially for large buffers or
  memory-bandwidth-bound shaders.
- **Best for:** Small, frequently-updated data (e.g., per-frame UBOs) where GPU
  read bandwidth is not the bottleneck.

#### Mechanism C: STAGED (staging + device-local)

```
CPU data ──write──▶ host-visible staging  ──GPU copy──▶  device-local GPU buffer
           (1 copy, or 0 with callback)        (1 GPU copy)
```

- The backend maintains a ring of N host-visible *staging* buffers and a single
  device-local destination buffer.
- CPU writes into the staging slot (1 copy or 0 with callback, same as HOST_MAP).
- A GPU buffer-copy command transfers data from staging to device-local at execute
  time. A memory barrier ensures the copy completes before shader reads.
- The staging slot is protected by a per-slot GPU-completion token until the copy
  command finishes.
- **Pros:** Device-local memory delivers the fastest GPU read bandwidth. Works on
  platforms where host-visible memory has no coherence or is uncached.
- **Cons:** Two copies total (CPU→staging, staging→GPU-local). Requires recording
  a copy command in the command buffer. The destination buffer is not available
  for shader reads until the copy barrier resolves.
- **Best for:** Large buffers that change occasionally and are read by many shaders
  or in memory-bandwidth-bound passes.

#### Ring-Slot Lifetime Management

All three mechanisms share the same slot-lifecycle model for safe CPU/GPU
pipelining (implemented internally in the backend):

```
before write:  wait if slot's GPU-completion token is still pending (rare)
               release slot's keepAlive list (drop old data)
write:         copy/callback into slot
after submit:  record GPU-completion token into slot
               add any transient resources to slot's keepAlive list
```

The `keepAlive` list holds any transient resource (staging buffer, data blob) that
must remain valid until the GPU completes the slot. Resources are released lazily
at the start of the *next* use of that slot — after the token confirms completion.

This model is the foundation for:
- **Ring-buffered UBOs** (HOST_MAP, N=2 slots) — per-frame camera/lights.
- **Staged uploads** (STAGED, N=2 staging slots) — per-frame mesh data updates.
- **Dynamic-offset sub-allocation** (`GpuUploadPool`, future) — one large ring
  buffer sub-allocated per draw call, each draw uses a dynamic buffer offset.

### Preferred Model: Transient Buffers + Copy Actions

The above `GpuBufferUpload` mechanisms remain functional, but the **preferred**
approach for new code is the *transient buffer + copy action* model.  It cleanly
separates the per-submission lifetime (transient buffer) from the stateless
transfer operation (copy action) and requires zero user-side lifetime management.

```
Caller thread                            Submission worker
─────────────                            ──────────────────
1. arena = TransientArena::create(db, name, {.context = gpu}); tb = arena->allocate(N, name)
2. mapped = tb->map(); write(mapped.data, data, N); mapped goes out of scope (unmap)
3. tb is ready                             ─►  4. CopyBuffer::execute(tb → dst)
5. tb ref can be dropped                 6. GPU copy completes
```

Key properties:
- **TransientBuffer** (`artifacts.h`) — per-submission, one-shot, host-visible
  buffer allocated from a grow-only ring-buffer pool.  Use `map()` to get a
  `Mapped` accessor (data, size); write on the caller's thread, release by
  letting it go out of scope or calling `unmap()`, then pass buffer to GPU actions.
- **CopyBuffer** (`actions.h`) — a stateless Level 2 action that records
  `vkCmdCopyBuffer`.  The source can be either a regular `Buffer` artifact or a
  `TransientBuffer`.
- **CopyBufferToImage** (`actions.h`) — stub for future texture uploads.
- Actions stay **stateless**; per-task context is stored in `TaskInfo::context`
  (an `AutoRef<RefCounter>` whose lifetime is tied to the owning submission).
- **Per-draw dynamic offsets:** Multiple small payloads can be packed into a
  single `TransientBuffer` and copied in one shot to a device-local storage or
  uniform buffer.  Each draw then uses a dynamic offset to select its region.

---

## Level 3 — Pipeline / Effect Library (`pipeline.h`, `physical.h`, ...)

**What it is:** A library of reusable rendering effects and pipeline components.
This is where policy lives: how GPU resources are organized, how effects are
composed, and what "correct rendering" means for this module.

**Contrast with Level 2:** Level 2 says "here is a draw call with these shader
resources." Level 3 says "here is a PBR-shaded mesh with this material and these
lights, rendered according to our binding layout policy."

### Resource Binding Contract (all Level 3 effects must follow this)

All effects in this layer share the same descriptor set layout so they can
interoperate:

```
Set 0 — SharedShaderConstants (per-frame globals: camera, lights)
    Binding 0: GlobalCameraUBO    (view/proj matrices, camera position, etc.)
    Binding 1: DirectLightingUBO  (light array, count)

Set 1 — Per-effect/material data (textures, material parameters)
    Layout is effect-specific; each effect declares its own Set 1 layout.

Set 2 — Per-draw-call data (model matrix, etc.) — future
    Backend manages a ring buffer; each draw uses a dynamic offset.

Push constants — small, hot-path immediates (≤ 128 bytes) — transitional.
```

This contract allows effects to be mixed in the same frame: any effect can read
Set 0 from the same `SharedShaderConstants` instance, regardless of its own
material or per-draw data.

### Subgraph Builders

Level 3 objects are **subgraph builders**: they expose a `build()` method that
produces one or more `Workflow`s ready to be submitted via `RenderGraph`.
Choose one of two patterns based on the builder's purpose:

#### Pattern A — Stateless builder

All data is passed directly to `build()` as parameters. The builder itself holds
no mutable frame state. This makes it inherently thread-safe and easy to reason
about: calling `build()` twice with the same parameters produces the same result.

```cpp
// Good for effects whose per-frame data arrives naturally as build() arguments.
class PbrShadingEffect {
public:
    struct BuildParams {
        AutoRef<Buffer>      vertexBuffer;
        AutoRef<Buffer>      indexBuffer;
        AutoRef<RenderTarget> renderTarget;
        // ... material, transform, etc.
    };

    // No setters. All data is in BuildParams. Thread-safe.
    Workflow * build(RenderGraph & rg, const BuildParams & p);
};
```

Use Pattern A when the builder is purely structural — it wires together data
that the caller already owns — and carries no persistent logical identity between
frames.

#### Pattern B — Stateful builder

The builder is also a state container. `set*()` methods update internal CPU-side
state (no GPU touch). `build()` snapshots the current state and produces the
corresponding workflows.

```cpp
// Good for builders that accumulate state across multiple set() calls,
// or whose state persists and changes incrementally between frames.
class SharedShaderConstants {
public:
    // Setters update CPU state only; may be called in any order before build().
    void setFrameInformation(const FrameInformation &);
    void setViewInformation(const ViewInformation &);
    void setDirectLightingInformation(const DirectLightingInformation &);

    // Snapshots current state; creates upload workflow(s).
    Workflow * build(RenderGraph & rg);
};
```

Use Pattern B when the builder owns logical state that accumulates over time or
is naturally separated from the build step (e.g., camera state set once per
frame, lighting state set at scene load, etc.).

The choice of upload mechanism (BLOB / HOST_MAP / STAGED), ring-slot count, and
any other Level 2 configuration detail is made **inside `build()`**, hidden from
the app. The app only deals with semantic concepts (camera, lights, mesh).

### Current Level 3 Types

- `SharedShaderConstants` — per-frame globals subgraph builder (Set 0 data).
- `PbrShading` — physically-based rendering effect (Set 1 material).
- `SkyBox` — sky/environment rendering.

---

## Level 4 — Scene Management (`scene.h`)

**Status:** Placeholder only. Not yet designed or implemented.

**Intended purpose:** High-level scene graph management: frustum culling, LOD
selection, material sorting, draw-call batching, occlusion queries, etc.

**Open question:** Level 4 may belong in a separate `engine` module rather than
in `rdg`. Using Level 3 effects as building blocks and composing them at the
engine level might be cleaner than embedding scene management here.

---

## File Map

```
src/inc/garnet/rdg/
  README.md              ← you are here
  dependency-graph.h     ← Level 1: Artifact, Action, Arguments, Workflow, RenderGraph
  artifacts.h            ← Level 2: GpuContext, Buffer, Texture, GpuResourceGroup, ...
  actions.h              ← Level 2: GpuDraw, GpuCompute, GpuBufferUpload, ...
  pipeline.h             ← Level 3: SharedShaderConstants, PbrShading, ...
  physical.h             ← Level 3: transform/physics helpers for rendering
  transform.h            ← Level 3: spatial transform types for rendering
  scene.h                ← Level 4: placeholder
```

**Potential future rename:** `artifacts.h` → `gpu-artifacts.h` and
`actions.h` → `gpu-actions.h`, to make the Level 2 GPU boundary visually obvious.
Not done yet to avoid disrupting existing includes.

---

## API Boundary Rules

These rules are enforced by code review; violations must be justified:

| Rule | Rationale |
|------|-----------|
| No backend types in any public header (`VkBuffer`, `vk::*`, `rapid_vulkan::*`, `ID3D12*`, `MTL*`) | Backends are interchangeable; leaking backend types makes the abstraction a lie |
| No Level N+1 concept in a Level N header | Each level must be independently usable |
| Level 2 expresses *mechanism*, not *policy* | Policy (frequency, quality, strategy) belongs in Level 3 |
| `Arguments` objects are single-use | They carry a snapshot of state for one task execution |
| `build()` snapshots CPU state; setters are pure CPU | No GPU resource touched until `build()` returns a workflow |

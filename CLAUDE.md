# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Garnet 3D is a cross-platform C++17 graphics engine supporting D3D11, OpenGL/ES, Vulkan (WIP), and D3D12 (WIP). It provides a unified GPU abstraction layer with a modern render dependency graph (RDG) architecture.

## Environment Setup

Before building, initialize the shell environment (provides `b`, `cit`, and other aliases):

Most of common scritps has a .cmd variant on Windows, such as setupenv.cmd, build.cmd, cit.cmd and etc.

```bash
setupenv.sh
```

## Build Commands

```bash
env/bin/build.py d        # Debug build
env/bin/build.py d        # Debug build
env/bin/build.py p        # Profile (RelWithDebInfo) build
env/bin/build.py r        # Release build
env/bin/build.py c        # Clean all build directories
env/bin/build.py -c d     # CMake configure only (no build)
env/bin/build.py -C d     # Build only (skip configure)
env/bin/build.py -a d     # Android build
env/bin/build.py --clang d  # Use Clang on Linux
env/bin/build.py --help   # full help
```

Output goes to `build/<variant>/bin/` and `build/<variant>/lib/`.

## Testing

```bash
env/bin/cit.py        # Full suite: lint + unit tests + internal tests + sample smoke tests
env/bin/cit.py -l     # Lint/style check only
env/bin/cit.py -u     # Unit tests only
env/bin/cit.py -i     # Internal tests only
env/bin/cit.py -t     # All tests, skip lint
```

Individual test executables after build:
- `build/<variant>/bin/GNtest-unit-tests` — unit tests that depends only on public interface of garnet library. will be depricated and replaced by GNtest-internal
- `build/<variant>/bin/GNtest-internal` — comprehensive internal unit tests

## Code Formatting

Clang-format 22.1.0+ is bundled in `env/bin/clang-format/`. 160-column limit, 4-space indent, LLVM base style.

```bash
env/bin/format-all-sources.py        # Format all tracked files
env/bin/format-all-sources.py -n     # Dry-run (report violations only)
env/bin/format-all-sources.py -d     # Format only files changed from master
```

## Code Comments

Add a reasonable amount of comments to code. Comments should mainly explain **why** the code is written the way it is — the logic, the reasoning, the assumptions, the constraints, the non-obvious tradeoffs.

Do **not** write comments that just narrate **what** the code does — well-named identifiers already convey that.

Guidelines:
- Add a comment when the *reason* for an approach is non-obvious: invariants, why a particular algorithm/order/check is needed, performance assumptions, hardware/API quirks being worked around, etc.
- Skip comments that merely restate the code (e.g., `// increment counter` above `++counter`).
- Keep comments concise — usually one short line. Multi-line only when the rationale genuinely needs it.
- Do not reference task numbers, PR numbers, or "added for X" — explain the enduring reason, not the transient circumstance.

This rule overrides the default "write no comments" stance.

## Architecture

### Module Map

| Module | Headers | Source | Description |
|--------|---------|--------|-------------|
| Base | `GNbase.h` | `src/core/base/` | Memory, logging, threading, data structures |
| Graphics | `GNgfx.h` | `src/core/gfx/` | Images, pixel formats, sprites, fonts |
| GPU v1 | (via GNgfx.h) | `src/core/gpu/` | OpenGL backend (stable, but slated for retirement) |
| GPU v2 | (via GNgfx.h) | `src/core/gpu2/` | Modern GPU abstraction (D3D12 + Vulkan WIP). Go-to GPU model for future code. |
| RDG | `GNrdg.h` | `src/core/rdg/` | Render dependency graph. The one that is being actively developed. |
| Engine | `GNengine.h` | `src/core/engine/` | Entity-component, meshes, scene objects. Works with GPU v1 only. |
| Input | `GNinput.h` | `src/core/input/` | Keyboard, mouse, gamepad |
| D3D utils | `GNd3d.h` | `src/core/d3d/` | DirectX COM helpers, state tracking |
| Utilities | `GNutil.h` | `src/core/util/` | Strings, XML, asset database |

### RDG — 4-Level Hierarchy (V1)

The Render Dependency Graph (`src/core/rdg/`) is the primary rendering abstraction, organized into four levels:

1. **Dependency Graph** (`dependency-graph.h`) — Pure task-graph engine; key types: `Artifact`, `Action`, `Arguments`, `Workflow`, `RenderGraph`, `Submission`
2. **GPU IR** (`artifacts.h`, `actions.h`) — API-agnostic GPU semantics; key types: `Buffer`, `Texture`, `GpuDraw`, `GpuBufferUpload`, `GpuKernelLaunch`
3. **Pipeline Library** (`pipeline.h`, `physical.h`, `transform.h`) — Rendering effects and subgraph builders; key types: `SharedShaderConstants`, `PbrShading`, `SkyBox`
4. **Scene** (`scene.h`) — High-level scene graph (placeholder, in development)

### RDG V2 — Open Graph (work in progress)

A ground-up redesign living in `src/core/rdg/2/` (public headers: `src/inc/garnet/rdg/2/`). Currently Vulkan-only proof-of-concept; D3D12/Metal backends are stubs.

**Core concept**: A generic DAG executor with no render-specific semantics baked into the graph engine itself. GPU abstractions are layered on top as ordinary graph nodes.

**Key files:**
- `open-graph.h` / `open-graph.cpp` — graph engine (~1000 lines, single-worker)
- `gpu.h` — `GpuContext`, `Texture`, `GpuShader`, `GpuCommandBuffer`, `GpuResourceView`
- `raster.h` — `RenderTarget`, `RenderGeometry`, `RenderStates`, `GpuRaster`
- `swapchain.h` — `Swapchain`
- `vk-*.cpp/h` — Vulkan backend implementations

**Graph engine types** (`open-graph.h`):

| Type | Role |
|------|------|
| `Entity` | Reference-counted base; every graph object has an ID and name |
| `Action` | Unit of work; subclass and implement `execute(Arguments*)` |
| `Token` | Synchronization handle; satisfied when a node completes |
| `Node` | Wraps an `Action` with state + priority; lifecycle: Blocked → Ready → Running → FinishedAction → Completed |
| `Artifact` | Versioned named payload; nodes publish to it and wait on a specific version |
| `Graph` | Executor; `OpenGraphImpl` is the concrete single-worker implementation |

**Node execution model:**
- Nodes become `Ready` when all their `Token` dependencies are satisfied
- The single-worker pump pops the highest-priority ready node, releases the mutex, runs the action, then re-acquires
- `manualComplete = true` keeps a node in `FinishedAction` until `satisfyNode()` is called externally (for GPU fence waits, async I/O)
- `waitForIdle()` returns `BUSY` if called from within a node's action (re-entrancy guard via `thread_local`)

**Priority ordering**: scheduling class (lower = higher priority) → priority int → insertion order.

**Artifact versioning**: `Artifact` uses a 128-bit never-overflowing counter. A node can wait for `OOO()` ("out-of-order" / next version), which is satisfied the next time any node publishes to that artifact.

**GPU layer** (`gpu.h`, `raster.h`):
- `GpuContext::create(api)` — factory that resolves to the Vulkan/D3D12/Metal implementation
- `Texture` — GPU resource with descriptor (format, width, height, mips, array faces); Vulkan impl tracks per-subresource layout/access/stage for barrier generation
- `GpuShader::compile(spirv)` — SPIR-V shader module
- `GpuRaster` — records draws (no Vulkan calls yet); `seal()` produces a `Payload` that records into a `VkCommandBuffer` on submit, building `VkGraphicsPipeline` on the fly
- `GpuCommandBuffer` — records and submits work; optional `Fence` for CPU-side completion tracking

**Safety patterns in the implementation:**
- FOURCC runtime type tags on opaque handles detect use-after-free and type confusion
- Destructors overwrite the tag with a "BADBEEF" sentinel for dangling-pointer detection

**Known missing pieces** (stubs that log a warning and return `{}`):
- D3D12 and Metal factory implementations
- `GpuResourceTable` descriptor binding
- Indexed draws and vertex buffer binding in the raster pass
- `GpuShader::load()` from file paths

### Feature Detection

`src/core/features.h` (generated from `features.h.in`) defines compile-time flags:
`GN_BUILD_HAS_OGL`, `GN_BUILD_HAS_D3D11`, `GN_BUILD_HAS_D3D12`, `GN_BUILD_HAS_VULKAN`, etc.
Use these guards for API-specific code paths.

### Conventions

- **Enum values**: All enum values must be `ALL_CAPS` (e.g., `GRAPHICS`, `ASYNC_COMPUTE`). Never use `PascalCase` or `camelCase` for enum values.
- **Namespaces**: `GN::` (public), `GN::<module>::` (e.g., `GN::gfx::`, `GN::rdg::`), `GN::internal::` (private)
- **Export macro**: `GN_API` marks symbols exported from the shared library
- **Public headers are monolithic**: Always include the top-level module header (`GNrdg.h`, `GNgfx.h`, etc.) as a unit. Never include sub-headers directly (e.g., `rdg/1/artifacts.h`). Sub-header paths are private implementation detail and may change.
- **PCH**: Each major subsystem has `pch.h`/`pch.cpp` — include module-local PCH rather than global headers
- **Math**: GLM (column-major matrices); headers in `src/3rdparty/glm/`
- **String formatting**: `fmt` library (in `src/3rdparty/fmt/`)
- **Compiler flags**: Warnings-as-errors (`/WX` on MSVC, `-Werror` on GCC/Clang)

### Adding a New GPU Backend

GPU v2 is the go-to GPU model for future code; v1 (`src/core/gpu/`) will retire eventually.

New backends go under `src/core/gpu2/<api>/`. Follow the D3D12 implementation pattern: add a feature flag in `features.h.in`, guard with `#if GN_BUILD_HAS_<API>`, and register the new target in `src/core/CMakeLists.txt`.

## Superpowers Docs Location

All superpowers-generated documents (plans, specs, designs) must be saved under `agent/superpower/`:

| Type | Path |
|---|---|
| Implementation plans | `agent/superpower/plans/YYYY-MM-DD-<topic>.md` |
| Design specs | `agent/superpower/specs/YYYY-MM-DD-<topic>-design.md` |

Do **not** use `docs/superpowers/` — that path is not used in this project.

## Module Rule Files

Per-module guidance lives in `.claude/rules/` as path-scoped rule files. Each file has a YAML frontmatter `paths:` field listing glob patterns; Claude loads the file automatically when editing any matching file.

| Rule file | Covers |
|---|---|
| `.claude/rules/gpu2.md` | `src/core/gpu2/**`, `src/inc/garnet/gpu2/**`, `src/inc/garnet/GNgpu2.h` |
| `.claude/rules/rdg.md` | `src/core/rdg/**`, `src/inc/garnet/rdg/**`, `src/inc/garnet/GNrdg.h`, `src/inc/garnet/GNrdg2.h` |

**When to create a new rule file:** Add `.claude/rules/<module>.md` whenever a module has non-obvious conventions, a complex internal architecture, or known stubs/pitfalls that Claude would otherwise have to rediscover each session. Keep rule files focused on the WHY and non-obvious patterns — file layout, object model, key invariants — not on things Claude can derive by reading the code.

**Template for a new module rule file:**

```markdown
---
paths:
  - "src/core/<module>/**"
  - "src/inc/garnet/<module>/**"
  - "src/inc/garnet/GN<Module>.h"
---

# <Module> Module

One-line description. Namespace. Status (stable / WIP / deprecated).

## File Layout
...

## Object Model / Key Patterns
...

## Conventions Specific to This Module
...

## Known Stubs / Missing Pieces
...
```

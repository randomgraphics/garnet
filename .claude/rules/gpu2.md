---
paths:
  - "src/core/gpu2/**"
  - "src/inc/garnet/gpu2/**"
  - "src/inc/garnet/GNgpu2.h"
---

# gpu2 Module

GPU abstraction layer v2. Namespace: `GN::gpu2`. Active Vulkan backend; D3D12 and Metal are stubs. This module is being superseded by the RDG module (`src/core/rdg/`) and will eventually be removed.

## File Layout

| Location | Role |
|---|---|
| `src/inc/garnet/GNgpu2.h` | Public monolithic include — **always include this, never sub-headers** |
| `src/inc/garnet/gpu2/gpu2.h` | Core types: `RootEntity`, `GpuContext`, `Texture`, `Buffer`, `Sampler`, `GpuShader`, `GpuResourceView` |
| `src/inc/garnet/gpu2/raster.h` | `RasterState`, `RasterTarget`, `RasterGeometry`, `GpuRaster` |
| `src/inc/garnet/gpu2/swapchain.h` | `Swapchain` |
| `src/core/gpu2/gpu-context.h` | Internal: `GpuContextCommon2` (base for all backends) |
| `src/core/gpu2/vk-gpu-context.h` | `GpuContextVulkan2` — owns rapid-vulkan `Instance` + `Device` |
| `src/core/gpu2/vk-texture.h` | `TextureVulkanBase` — shared base for owned textures and swapchain backbuffers |
| `src/core/gpu2/vk-buffer.h` | `BufferVulkan` |
| `src/core/gpu2/vk-buffer-state.h` | `BufferStateVulkan` — whole-buffer Vulkan access/stage tracking |
| `src/core/gpu2/vk-gpu-image-state.h` | `TextureGpuImageState` — per-subresource layout/access/stage tracking |
| `src/core/gpu2/vk-gpu-resource-state-tracker.h` | `GpuResourceStateTrackerVulkan` — batches pre-pass image+buffer barriers |
| `src/core/gpu2/vk-gpu-raster.h/cpp` | `GpuRasterVulkan` — records draws, `seal()` → `VkCommandBuffer` |
| `src/core/gpu2/vk-gpu-shader.h/cpp` | SPIR-V shader module wrapper |
| `src/core/gpu2/vk-gpu-command-buffer.h/cpp` | `GpuCommandBufferVulkan` — submission + fence tracking |
| `src/core/gpu2/vk-swapchain.h/cpp` | `SwapchainVulkan` |
| `src/core/gpu2/vk-format-utils.h/cpp` | `gfx::img::PixelFormat` ↔ `VkFormat` helpers |
| `src/core/gpu2/gpu-factories-stub.cpp` | D3D12 / Metal factory stubs (log warning, return `{}`) |

## Object Model

All GPU objects inherit from `RootEntity`, which provides:
- `RefCounter` — use `AutoRef<T>` exclusively; raw owning pointers are prohibited.
- `RuntimeType` — use `RuntimeType::cast<T>()`, not `dynamic_cast`. Register with `GN_REGISTER_RUNTIME_TYPE(ParentClass)`.
- `uint64_t id` — process-unique, assigned at construction.
- `StrA name` — human-readable, non-unique.

Inheritance chain for concrete types:
```
RootEntity
  GpuContext → GpuContextCommon2 → GpuContextVulkan2
  Texture    → TextureVulkanBase → TextureVulkan2 (owned)
                                 → TextureSwapchainVulkan2 (swapchain backbuffer)
  Buffer     → BufferVulkan
  GpuShader  → GpuShaderVulkan
  GpuRaster  → GpuRasterVulkan
  GpuPayload → (Vulkan sealed payload, internal)
  Swapchain  → SwapchainVulkan
```

## Key Patterns

**Factory method** — every public type uses a static `create()` / `load()` that returns `AutoRef<T>` (never throws; returns empty ref on failure):
```cpp
auto gpu  = GpuContext::create("main", {.api = "vulkan"});
auto tex  = Texture::create("rt", {.context = gpu, .descriptor = ...});
auto buf  = Buffer::create("vb", {.context = gpu, .size = 1024});
auto sh   = GpuShader::create({.context = gpu, .binary = spv, .size = sz});
auto rast = GpuRaster::create({.gpu = gpu, .target = rt});
```

**Render loop** — per frame:
```
swapchain.prepare()          → Frame{view, ready}
GpuRaster::create(target)   → record draws via raster.draw(...)
raster.seal()                → GpuPayload
GpuContext::submit({work, waitForGpu=ready, onComplete=...})
swapchain.present(payload)
GpuContext::pump()           → fire onComplete callbacks
```

**RasterState** — two construction modes:
- `RasterState{}` — all fields `std::nullopt`; used in `DrawParameters` (absent = inherit from `RasterTarget::states`).
- `RasterState{RasterState::WithDefaults{}}` — every field set to its logical default; used as the baseline in `RasterTarget::states`.
- Draw-level overrides are **persistent** within a `GpuRaster` pass until overridden again.

**Barrier management** (`GpuResourceStateTrackerVulkan`):
1. Register all textures and resource bindings via `add*()` methods.
2. Call `emitPrePassBarriers(cb)` before `beginRendering`.
3. After GPU completion, call `flushStatesToResources()` to write final states back.
- Texture state is per-subresource (`TextureGpuImageState`, indexed by mip × arrayLayer).
- Buffer state is whole-buffer (`BufferStateVulkan`).

**`GpuPayload`** is the currency for GPU-side ordering: `submit().waitForGpu` takes payloads; `Swapchain::Frame::ready` and `Swapchain::present()` also use payloads.

## Conventions Specific to This Module

- Vulkan types live behind `vk::` (Vulkan-Hpp). `rv::` is the `rapid-vulkan` namespace aliased to `GN::gpu2::rv`.
- Include `vk-gpu-context.h` **before** any other header that might include `rapid-vulkan.h` in the same TU (ODR risk; enforced by a `#error` guard in that header).
- Internal impl files include `<garnet/GNgpu2.h>` at the top, then the `vk-*.h` private headers.
- `GN_REGISTER_RUNTIME_TYPE` must pass the **direct** parent class, not a grandparent.
- Stub backends in `gpu-factories-stub.cpp` log a warning and return `{}` — do not remove stubs; add the real impl alongside them when implementing a new backend.

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

## Known Stubs / Missing Pieces

- D3D12 and Metal `GpuContext`, `Texture`, `Buffer`, `Swapchain` factories — all in `gpu-factories-stub.cpp`.
- `GpuResourceTable` descriptor binding is not fully wired in the Vulkan raster pass.
- Indexed draws and vertex buffer binding in `GpuRasterVulkan` are partially implemented.
- `GpuShader::load()` (from file path) is a stub.
- PSO cache/factory — `GpuRasterVulkan` rebuilds the pipeline each frame; see `TODO.md`.
- Transient buffer/texture arena — see `TODO.md`.

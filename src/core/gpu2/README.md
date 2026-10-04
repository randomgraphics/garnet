# GPU2 Core Module Architecture & Bindless Design

`GN::gpu2` (public include `<garnet/GNgpu2.h>`) is the active modern GPU abstraction layer for Garnet. It abstracts low-level graphics APIs (Vulkan active; D3D12 and Metal stubs) into an intuitive, high-performance C++ interface for rendering, compute, and memory transfers.

---

## 1. Core Architectural Concepts

### The Single Currency: `GpuPayload`
To the caller, all unsubmitted GPU work—regardless of whether it represents rasterization, compute, memory copy, bound rendering, or bindless rendering—is represented by an opaque `AutoRef<GpuPayload>`.
- The caller records work into a recorder object and calls `seal()`.
- `seal()` returns an `AutoRef<GpuPayload>`, finalizing the recording and sealing internal state.
- The caller submits the payload to the GPU via `GpuContext::submit(SubmitParameters("...").appendWork(payload))` or simply drops it if no longer needed.
- Under the hood, `GpuPayload` subclasses manage native command buffers and keep all dependent resources (geometry, textures, constant buffers, transient memory) alive until GPU execution completes (`onGpuComplete()`).

### Coexistence of Rendering Paradigms
GPU2 provides two distinct raster paths tailored for different workloads:
1. **`GN::gpu2::GpuRaster`**: The traditional bound-rendering path. Safe, convenient, handles per-draw `GpuResourceTable` binding and hazard tracking automatically.
2. **`GN::gpu2::bindless::Raster`**: The high-performance bindless rendering path. Completely bypasses descriptor set thrashing and per-draw table scanning, enabling tens of thousands of unique draws at maximum hardware throughput.

---

## 2. Bindless Rendering Architecture (`GN::gpu2::bindless`)

The bindless subsystem lives entirely within `namespace GN::gpu2::bindless` and is declared in `<garnet/gpu2/bindless.h>` (included via `<garnet/GNgpu2.h>`).

### A. The "Read-Ready" Invariant (Zero Per-Draw Barrier Overhead)
In traditional rendering, recorders recursively scan and hash every draw's resource table to discover layout transitions and hazards. This causes heavy CPU overhead on large scenes.

The bindless subsystem eliminates per-draw scanning by establishing a universal invariant:
- **Default State**: All textures in the bindless heap reside in `VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL`, and all buffers reside in "read-ready" state.
- **Readers (Draw Calls)**: Draw calls are strictly readers of textures and buffers. Because concurrent reads have zero hazards in Vulkan, draws emit **zero barriers**, perform **zero tracking**, and scan **zero tables**.
- **Writers (Render Targets, Compute UAVs, Transfers)**: Any operation that modifies a resource is explicitly known at the pass/operation level:
  - At pass start (`beginRendering`): Transitions attachments from their read-ready state to the required write layout (e.g. `COLOR_ATTACHMENT_OPTIMAL` or `DEPTH_ATTACHMENT_OPTIMAL`).
  - At pass end (`endRendering`): Automatically transitions attachments back to `SHADER_READ_ONLY_OPTIMAL` with write-cache flushes.
  - Compute storage images and transfer destination buffers similarly emit cache flushes upon operation completion.

Layout and barrier management is completely automated and invisible to the user.

---

### B. Persistent Global Descriptor Heap (`bindless::DescriptorHeap`)
Traditional descriptor set management allocates and updates `VkDescriptorSet`s frequently. `DescriptorHeap` replaces this with a persistent, long-lived descriptor array:
- Created once and persists across passes and frames.
- Built on Vulkan 1.2 `VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT` and `VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT`.
- Unbounded descriptor arrays (e.g., `sampler2D u_textures[]`, storage images, buffers) managed via a thread-safe slot allocator (bump counter + free list).
- Provides stable `uint32_t` indices for shaders to consume.
- Supports in-place `update(slot, view)`: allows background streaming threads to swap a fallback 1x1 placeholder texture for a loaded 4K texture without changing material constants or rebinding shaders.
- Bound **once** at the start of the pass. Per-frame descriptor writes drop to zero for static/reused resources.

---

### C. Bindless Raster Recorder (`bindless::Raster`)

#### Plain POD Configuration
Callers describe their pass requirements using a plain POD `CreateParameters` struct without touching native pipeline layout objects:

```cpp
namespace GN::gpu2::bindless {

struct Raster::CreateParameters {
    AutoRef<GpuContext>     gpu;
    const RasterTarget *    target           = nullptr;
    AutoRef<DescriptorHeap> heap;
    uint32_t                heapSetIndex     = 0;   ///< Set index for the bindless heap (e.g. Set 0, Set 1)
    GpuResourceTable        passResources;          ///< Optional pass-wide resources (e.g. Set 0 Camera UBO)
    uint32_t                pushConstantSize = 128; ///< Max push constant bytes (default 128)
};
```

#### Fast Conflict Check
`Raster::create()` validates that `passResources` does not define bindings for the set index reserved for `heapSetIndex`. If both claim the same set index, it fails fast with a descriptive error.

#### Internal Pipeline Layout Caching
`gpu2` hashes the POD configuration `(heapSetIndex, passResources layout, pushConstantSize)` and reuses or builds the `VkPipelineLayout` internally. The caller has zero pipeline layout boilerplate.

#### Lean `DrawParameters`
Draws do not carry a `GpuResourceTable`. They specify only:
- Shaders (`vs`, `ps`)
- Raster state overrides (`states`)
- Geometry (`geometry`)
- Push constants (`immediates`: transforms, material BDA addresses, bindless texture IDs)

#### Parameter Buffer Retention
`bindless::Raster::retainResource(AutoRef<RCRT64> resource)` allows higher-level modules (`fx2`) to attach dynamic per-draw parameter buffers, material buffers, or textures to the recorder. The sealed `GpuPayload` holds these references until GPU execution completes.

#### Direct Vulkan Recording & Dynamic Rendering
Bypasses `rv::Drawable` and descriptor pools entirely:
- Emits native Vulkan 1.3 dynamic rendering (`vkCmdBeginRendering` / `vkCmdEndRendering`) without requiring `VkRenderPass` or `VkFramebuffer` handles.
- **`VkBindlessPipelineLayoutCache`**: Thread-safe layout cache that hashes the POD configuration `(heapSetIndex, passResources layout, pushConstantSize)`. Generates dummy empty descriptor set layouts for any set indices between 0 and `heapSetIndex` that are not consumed by `passResources`, ensuring the unified layout is always valid.
- **`VkBindlessPsoCache`**: Thread-safe PSO cache generating dynamic rendering graphics pipelines (`VkPipeline`) with deduplication across shaders, vertex layouts, raster state, and attachment formats.
- At pass start: Binds pass resources and the bindless descriptor set.
- Per draw: Emits direct `vkCmdPushConstants`, `vkCmdBindVertexBuffers`, `vkCmdBindIndexBuffer`, and `vkCmdDrawIndexed` with consecutive state deduplication.
- At pass end: `ctx.batchTracker->restoreAttachmentToShaderReadOnly()` transitions attachments to `VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL` with write-to-read barrier execution and synchronization with CPU tracker state.

---

### D. Lock-Free Concurrency Hierarchy

1. **Level 1: Inter-Pass Concurrency (Coarse-Grained)**:
   - Multiple passes (e.g., Shadow Pass, G-Buffer, Main Lighting) record concurrently on separate threads.
   - Each thread records into its own primary command buffer / payload.
   - The submit thread sequences the resulting payloads into `gpu->submit()`.

2. **Level 2: Intra-Pass Concurrency (Fine-Grained)**:
   - When a single pass contains tens of thousands of draws, worker threads record into thread-local draw streams.
   - Recording is 100% lock-free: worker threads only read numeric descriptor indices and device addresses, and scene resources are strictly read-only.
   - The pass aggregates the streams into a single sealed `GpuPayload`.

---

### E. Future Extensibility to Copy and Compute (`bindless::CnC`)
The bindless model extends naturally to compute and memory operations:
- `DescriptorHeap` allocates storage image (UAV) and storage buffer indices via `GpuResourceView`.
- Compute dispatches take push constants (BDA addresses, storage indices) without a `GpuResourceTable`.
- Operations automatically flush memory/caches before sealing, preserving the universal read-ready invariant.

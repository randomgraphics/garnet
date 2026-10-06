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

The heap owns one persistent descriptor set containing a material SSBO followed
by five typed resource arrays.
For a configured base `bindingIndex`, Vulkan uses:

| Binding offset | Descriptor type |
|---|---|
| 0 | Material storage buffer (one descriptor) |
| 1 | Sampled texture, without sampler |
| 2 | Storage texture |
| 3 | Uniform buffer |
| 4 | Storage buffer |
| 5 | Sampler |

`DescriptorIndex` packs a 28-bit slot, a 3-bit type, and a high validity bit.
Allocated handles have bit 31 set; zero/default-initialized handles are invalid.
Update/free reject untagged handles even if their type/slot names an active entry.
Shaders still receive only the slot, so this CPU encoding does not change bindings.

Combined texture/sampler descriptors are not supported by this heap. A shader can
sample a texture with any registered sampler, or load texels without a sampler.
All arrays have `capacity` entries and share one thread-safe slot allocator; the
capacity limits total active descriptors across the five types.

`allocate(type, view)` returns a packed `DescriptorIndex`. Its `type` identifies
the binding offset, and its `slot` is the shader array index. Allocation rejects
views that disagree with the requested descriptor kind, including sampled versus
storage image views and uniform versus storage buffer views. Updates preserve
the allocated type. Batch update/free remain available; batch allocation is deferred.
The heap retains each complete view until it is replaced or freed.

The set uses Vulkan update-after-bind and partially-bound arrays, and is bound
once per pass. CPU operations are thread-safe, but callers must not replace or
free slots referenced by recorded or in-flight GPU work. Writable resource hazards
must still be declared through the recorder's existing pass resources.

---

### B.1. Built-in Material Buffer

The first implementation of this extension is a fixed-capacity mini heap inside
`bindless::DescriptorHeap`. The growth design below is a subsequent step. The heap will own both its existing descriptor arrays and a material buffer.
The buffer is a generic store for relatively long-lived, read-only, non-uniform
GPU records. "Material" names its common use; gpu2 does not know the contents of
any record, shader schema, FX2 kernel type, or descriptor indices packed inside.
Kernels and other callers define and interpret those bytes.

#### Opaque variable-sized chunks

The heap allocates relatively long-lived, read-only, non-uniform GPU records.
`allocateMaterial(size, alignment)` returns an opaque `MaterialToken`; alignment
must be a nonzero power of two. `materialView(token)` returns the buffer range
with its offset/size, allowing a caller-owned CnC to upload bytes. Allocation
itself creates no upload work. `freeMaterial(token)` immediately returns the
range to the allocator; the caller must delay freeing until all consumers finish.
Stale/foreign tokens have no effect. Material chunks do not consume typed-array
slots, and gpu2 never interprets their contents.

`CreateParameters::materialCapacity` is the current fixed byte budget (default
4 MiB). Invalid size/alignment or exhaustion returns `INVALID_MATERIAL_TOKEN`.
The current implementation fails explicitly instead of growing or relocating
storage. Recorded work can retain the heap or captured buffer views; retaining a
view does not prevent an explicit free from reusing its bytes.

#### Future TODO: growth

Add doubling growth with caller-recorded GPU buffer copies, preserving every
live token's offset/size and versioning the heap descriptor set with its backing
buffer. No blocking readback or persistent CPU copy of material contents is
needed. Maximum capacity/failure policy remains explicit. This growth step is
not implemented by the initial fixed-capacity allocator.

#### Allocation implementation

One long-lived storage buffer plus a free-list allocator over aligned,
variable-sized ranges, with adjacent free ranges coalesced. Tokens are dense
identifiers; the token-to-offset mapping is private. Growth preserves byte offsets
by design, so tokens survive it. Uploads and the growth copy are recorded on the
caller's CnC. The arena keeps no CPU shadow of its contents.

#### Descriptor and pipeline integration

The material SSBO is part of the heap's own descriptor set, alongside its five
typed descriptor arrays. It uses `bindingIndex + 0`; with the FX2 heap at set 0
and base binding zero, this is set 0, binding 0. The five arrays move to offsets
1-5 while DescriptorType values remain 0-4. Binding 0 is one storage-buffer
descriptor, not another indexed resource array. The heap provides one
descriptor set and its set layout, covering all six bindings; Raster/CnC combine
this set layout with other sets and push constants into the pipeline layout.
The implemented heap layout contains these six bindings.

When growth is implemented, subsequent recordings capture the new material buffer together
with its heap descriptor-set version. Already recorded/in-flight work must retain
the old buffer and matching descriptor-set version. Do not rewrite a material
binding still used by earlier work; retaining just the old buffer is insufficient
if its descriptor now points elsewhere. The caller executes uploads/growth copies
before consumers. `free(token)` immediately reclaims the chunk and does not
schedule deferred reclamation; delaying that call until consumers finish is the
caller's responsibility.

This buffer is distinct from a mapped CPU-to-GPU streaming pool for per-draw
arguments and from versioned shared-uniform storage. FX2 SSC shares the heap;
it does not implement a second material allocator.

#### Required verification

Verify byte-exact GPU readback, mixed sizes/alignment, free-list reuse/coalescing,
capacity failure, immediate free, captured-buffer lifetime, and caller uploads with
no CPU shadow, readback, implicit submission, or blocking transfer. Run relevant
heap and pass-resource tests under Vulkan validation. FX2 integration tests
verify consumption of heap-owned records, not another allocation implementation.

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
    GpuResourceTable        passResources;          ///< Optional pass-wide resources (e.g. Set 1 Camera UBO)
    uint32_t                pushConstantSize = 128; ///< Max push constant bytes (default 128)
};
```

#### Descriptor Heap Set Takeover and Fast Conflict Check
The descriptor heap and related data take over the entire set reserved for `heapSetIndex`. `Raster::create()` validates that `passResources` does not define bindings in the same set as where the descriptor heap is located; if pass resources conflict with the heap set, creation will fail.

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

#### Future TODO: Interleaved Bind-Based Draws in `bindless::Raster`
Currently, issuing bind-based draws requires closing the `bindless::Raster` pass and beginning a separate `GpuRaster` pass against the same render target (typically with `loadColor = true`). This incurs pass split overhead (`vkCmdEndRendering` / `vkCmdBeginRendering` transitions).
- **Goal**: Allow `bindless::Raster` to optionally accept bind-based draws within the same render pass.
- **Draw-Level Resource Table**: Support an optional `const GpuResourceTable *` or dedicated draw overload in `bindless::Raster`.
- **Fast-Path Preservation**: Pure bindless draws must continue to incur zero descriptor compilation and zero hazard scanning.
- **Dynamic Descriptor Sets**: When a draw supplies bound resources, descriptor sets for non-heap sets can be allocated from a pass-level or frame-level pool and bound on-demand, without invalidating the persistent bindless heap set.
- **Hazard & Lifetime Tracking**: Retain bound resources for the duration of the pass, keeping CPU overhead isolated only to draws opting into traditional binding.

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

## Material mini-heap verification checkpoint

The fixed-capacity implementation and six-binding layout are verified on Windows
Debug/Vulkan. `python.exe env/bin/build.py -C d` passed. With
`VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation`,
`python.exe env/bin/cit.py -i "[bindless]"` passed 31,930 assertions in 20 cases,
and `-i "[gpu2],[fx2]"` passed 86,775 assertions in 108 cases. The bindless sample
`t --frames 3 --draws 1000` passed its pixel check. No Vulkan validation diagnostics
were found. `python.exe env/bin/cit.py -l` and `git diff --check` passed.
Doubling growth and descriptor-set version retention are not implemented yet.
Evidence logs: `build/fx2-bindless/mini-heap-*.log` (local build artifacts).

### Bindless sample material usage

`GNsample-gpu2-bindless` prepares 100,000 reusable 32-byte material chunks
(3,200,000 bytes, fitting the default 4 MiB). Cube and gem pipelines share the
sample-private std430 schema: color tint, texture slot, shininess, and UV scale.
The fresh heap packs these chunks contiguously; the sample records one batched
initialization upload and submits it before the first raster consumer.

Every draw passes the material index alongside its transform and frame light/time
values. Both vertex shaders read the material SSBO at set 0/binding 0. Push
constants shrink from 128 to 100 bytes; no material allocation or material upload
occurs in the frame loop. The CPU initialization array is released after upload
recording. The heap retains the records for the sample lifetime. Buffer growth
remains a future TODO and is not a prerequisite for the fixed-capacity sample.

Sample material checkpoint: the targeted Debug build and lint pass. With Vulkan
validation enabled, 20 bindless cases pass 31,933 assertions (including the default
4 MiB boundary). Three-frame headless sample runs at 1,000 and 100,000 draws pass
pixel checks (66,576 and 664,438 non-background pixels) with no Vulkan diagnostics.
Logs: `build/fx2-bindless/material-sample-{build,tests,1000,100000,lint}.log`.

Descriptor-index checkpoint: bit 31 is the validity tag, type occupies bits 28-30,
and the slot occupies bits 0-27. Zero/default handles are invalid. Targeted Debug
build, lint, 31,955 assertions/20 bindless cases and the three-frame 1,000-draw
sample pass with Vulkan validation enabled and no diagnostics.

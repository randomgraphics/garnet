# FX2 typed rendering kernels

FX2 (`GN::fx2`, public include `garnet/GNfx2.h`) provides graph-independent GPU
rendering effects over gpu2. It has no E2 or RDG2 dependency and no geometry,
surface, or asset ownership layer. Callers import/generate data, create buffers
and textures, prepare uploads, and control submission/presentation.

## Kernel contract

`Kernel` provides effect identity and an explicit raster/compute execution class.
Each derived effect defines strongly typed inputs and recording operations.
There is no string-based parameter map or central effect registry; applications
can derive their own kernels without changing FX2 or E2.

| Kernel | Recording operation |
| --- | --- |
| `UnlitKernel` | Appends a triangle draw to the caller's raster |
| `PbrKernel` | Appends a metallic/roughness lighting draw |
| `LambertianKernel` | Appends a diffuse lighting draw |
| `CelKernel` | Appends cel shading and an optional outline draw |
| `SkyboxKernel` | Appends the background draw using SSC environment bindings |
| `RasterGaussianBlurKernel` / `ComputeGaussianBlurKernel` | Produce horizontal and vertical filtering work |
| `RasterMipmapKernel` / `ComputeMipmapKernel` | Produce ordered work for successive preallocated mip levels |

Raster and compute implementations are different kernel types. Higher-level
code selects one based on capabilities and policy; FX2 does not silently fall
back to another implementation. See `image-kernels.h` for current image-format,
radius, extent, and level restrictions.

```cpp
auto effect = fx2::UnlitKernel::create(gpu);
fx2::UnlitKernel::Inputs input;
input.geometry = vertexAndIndexBindings; // gpu2::RasterGeometry, not an FX2 owner
input.color = {0.8f, 0.2f, 0.1f, 1};
input.worldFromObject = transform;
if (!effect || !effect->record(raster, shared.set0Resources, input)) return false;
```

Position uses vertex location 0, normal 1, UV 2, tangent 3, and color 4.
Only enabled features require their corresponding optional attributes. Lit
kernels require position and normal; normal mapping additionally needs UV and
tangent with handedness. Kernel validation rejects unsupported input layouts.
GPU resources must belong to the kernel's creation device.

Ordinary raster effects inherit the caller's depth, culling, blending, viewport,
and scissor policy. Documented algorithm-required exceptions are local to draws
or passes: skybox uses far-background depth/culling, cel outlines reverse culling,
and image-generation passes require full output coverage and replacement writes.
Transparent draw sorting and raster target blending remain caller responsibilities.
Unlit bypasses lighting/exposure; lit effects preserve the shared lighting,
exposure, and tone-mapping conventions. Moving tone mapping out is separate work.

## Uploads and lifetime

Reuse one kernel across many invocations and append ordinary triangle draws to
one raster. Recording copies immediate CPU values and retains referenced GPU
resources. It does not decode files, compile shaders, submit, or wait for the GPU.
Retaining a resource keeps its allocation alive; callers still order writes to
its contents against consumers.

Lit kernel creation records fallback-texture initialization into the supplied
`GpuCnC`; submit that work before any kernel use. Lit recording writes private
constants into distinct per-invocation buffers through `prerequisiteUploads`.
Seal and schedule those uploads before the raster payload. Do not discard
initialization work and then use the kernel. Pooling private parameter allocations
is a possible optimization; there is no reusable material/asset object to manage.

Image kernels append ordered payloads to a work array. Keep that ordering and
schedule input producers before it, output consumers afterward. Intermediates
are retained by the work. FX2 records the effect's passes while gpu2 handles
backend resource barriers. Image work consuming raster output belongs after
that raster, never in a blanket upload batch before all draws.

## Shared shader constants

SSC retains one scene/camera GPU buffer pair per instance. A snapshot captures
CPU values into upload work; it does not allocate an independent GPU version.

```text
upload A -> every consumer of A -> upload B -> every consumer of B
```

Uploading A and B first and then drawing both makes both draws observe B. An
upload cannot occur between draws inside one physical raster pass: use pass
boundaries or separate SSC instances for distinct views. Preserve initialization
payloads when discarding a snapshot; SSC transfers pending initialization work
once and does not automatically return it again. Mutations of public `set0` and
snapshot creation must be caller-synchronized.

Set 0 is shared across effects: binding 0 scene/frame/lights, binding 1 camera,
and bindings 2–5 skybox/irradiance/prefiltered environment/BRDF lookup. Set 1 holds
kernel-private data. CPU parameters are packed into the shader ABI internally;
callers should not include private packing headers. SSC owns shared data and
environment preparation; `SkyboxKernel` owns background rendering.

## Web and mobile compatibility

The interfaces keep execution strategy explicit and use portable baseline
algorithms: two-direction Gaussian blur and successive mip reductions. A raster
path can target GLES 3.0/WebGL 2-class devices; compute requires appropriate native
or WebGPU support. That direction is not a claim that those gpu2 backends exist.
Current implementations and shader binaries use Vulkan; other backends, WGSL/GLSL
ES shader lowering, uniform lowering for native push constants, and device testing
remain platform work. Check actual formats/usages and limits, not OS names alone.

Generated image effects filter color channels including alpha independently;
premultiply colors before filtering when transparent-edge semantics require it.
Normal-map reduction and depth pyramids need distinct algorithms. Callers allocate
mip storage up front; mip generation does not resize textures. Image algorithm
verification is tracked in `agent/completed/2026-10-03-214942-FX2_KERNEL_REFACTOR.txt`.

## ImGui backend

`fx2::ImGuiBackend` owns a Dear ImGui context and records into the caller's
`gpu2::GpuRaster`. The caller owns the native event loop. Call `newFrame()`, build
widgets, `render()`, then `record(raster, uploads)`. Submit returned uploads before
the raster. On recording failure, discard the raster. Callers select target alpha
blending; the backend sets UI depth/culling/scissor state per draw.

Record before starting the next ImGui frame or modifying registered textures.
Payloads retain their buffers/textures until completion. The backend disables
`imgui.ini` persistence by default. An E2 application can wrap it in a
`VisualOverlay`; standalone tools use it directly without E2.

`GNsample-fx2-unlit` demonstrates direct kernel recording (append `t` for a
headless run). The mesh viewer uses public typed effects and owns import,
navigation, GPU resource preparation, and frame submission. No tool should
include FX2's private shader headers or recreate effect parameter packing.

## Bindless design: separate `fx2::bindless` implementation

The second implementation attempt adds new classes in `GN::fx2::bindless`.
Existing `GN::fx2` classes, shader contracts, and callers remain intact. The first
attempt is retained in a stash as reference; it is not the implementation baseline.
The resumable plan is [FX2_BINDLESS_DEVICE.txt](../../../agent/FX2_BINDLESS_DEVICE.txt).

Implement one small, verifiable step at a time. New bindless classes use the existing
`gpu2::bindless::DescriptorHeap`, `Raster`, and `CnC` facilities. Applications opt
into the new path explicitly; wholesale caller migration and legacy API removal
are outside this attempt. Keep `gpu2::GpuContext` named as-is.

### GPU resource layout

The following Vulkan layout is private to the bindless implementation. Applications
provide typed resources and values, not descriptor tables, binding numbers, arena
offsets, or shader ABI structures. Other backends may lower the same contracts
differently.

| Set | Binding | Resource | Scope |
| --- | --- | --- | --- |
| 0 | 0 | Material SSBO built into the gpu2 descriptor heap | Captured heap/buffer |
| 0 | 1 | Sampled-image descriptor array | SSC-owned shared heap |
| 0 | 2 | Storage-image descriptor array | SSC-owned shared heap |
| 0 | 3 | Uniform-buffer descriptor array | SSC-owned shared heap |
| 0 | 4 | Storage-buffer descriptor array | SSC-owned shared heap |
| 0 | 5 | Sampler descriptor array | SSC-owned shared heap |
| 1 | 0 | CPU-to-GPU streaming SSBO | Long-lived mapped buffer pool; ranges recycled after consumption |
| 1 | 1 | Shared-shader-constant (SSC) uniform buffer | Captured shared-state version |
| 2+ | As needed | Optional additional pass resources | Explicit extension point |

Set 0 uses the gpu2 heap with base binding zero: binding 0 is its material SSBO,
and bindings 1-5 are its typed arrays. Set 1 belongs entirely to SSC and holds
exactly its two stores: the streaming pool at binding 0 and the versioned uniform
at binding 1. Texture views register separately from samplers; thousands of
textures may share a few samplers. Shader sampling uses both indices; texel fetch
needs only a texture index.

The SSC buffer combines scene, frame, lighting, camera, and active-sky selection
in one private ABI at set 1, binding 1. There is no separate camera binding.
Each shared state retains an independently aligned allocation and captures its
buffer range for consuming passes. Pool offsets and sizes must obey GPU uniform
buffer alignment and range limits.

SSC's uniform management is generic: it allocates, versions, uploads, and recycles
byte ranges, and knows only each record's total size. The concrete combined-uniform
layout is defined outside SSC as an independent structure that callers pack into
bytes before recording an update. The same management therefore serves any uniform
format, not only the FX2 scene layout.

### SSC-owned storage

A long-lived `fx2::bindless::SharedShaderConstants` (SSC) is the owner of the GPU
data shared by the new kernels. There is no separate `fx2::bindless::Device` class.
SSC implements exactly two stores and shares one heap:

| Store | Set / binding | Entry lifetime | Transfer mechanism |
| --- | --- | --- | --- |
| Streaming pool | 1 / 0 | One-time use, until the consuming payload releases it | None: CPU writes a mapped range |
| Versioned shared-state uniforms | 1 / 1 | One `recordUpdate` version, until its last lease releases | Caller-supplied CnC |
| Shared descriptor heap, material SSBO included | 0 / 0–5 | gpu2-defined | gpu2-defined |

Long-lived, read-only, non-uniform records — materials included — are **not** an
SSC store. The gpu2 descriptor heap owns that material buffer, as described in the
[gpu2 design](../gpu2/README.md#b1-built-in-material-buffer), and FX2 does not
implement a second material allocator. The heap performs descriptor allocation and
registration; SSC retains and shares it rather than introducing a second descriptor
manager, and also owns shared fallback resources.

Across its two stores SSC is responsible for GPU backing buffers, allocation,
upload batching, versioning, resource retention, and safe recycling.

Kernels define typed material inputs and private shader schemas, retain their
texture/sampler dependencies, and consume opaque chunks from the gpu2 heap-owned
material buffer. SSC shares the heap; its captured descriptor set includes the material buffer. Material
allocation, free, upload, and growth contracts are defined in the gpu2 README.
Ordinary per-draw values can still be encoded as kernel-defined push constants;
buffer-backed draw parameters use a long-lived CPU-mapped streaming buffer pool. Applications
explicitly submit the
SSC-prepared producer work before its consumers. Unlit, PBR, Lambertian, Cel, and Sky schemas are FX2-specific interpretations of
opaque heap-managed chunks. They share one material-buffer binding and can reuse
a prepared record across draws without per-draw material uploads. FX2 retains
referenced registrations and delays freeing chunks until consumers finish.

Each kernel controls its complete push-constant layout. Ordinary transforms,
flags, and optional material references use push constants; there is no universal
packet or mandatory material-address field. Buffer-backed draw parameters use
a long-lived mappable buffer pool for streaming CPU data to the GPU. When the
backend and memory allocation support it, keep pool buffers persistently mapped
for their lifetime to avoid repeated map/unmap calls. Otherwise map as needed;
map/unmap is not the mechanism for synchronizing reuse with GPU consumers. CPU
recording writes an aligned range directly,
and the shader reads it as an SSBO at set 1, binding 1 using a per-draw offset
encoded in the kernel's push constants. No CnC upload or CnC ownership is needed.
The buffer pool persists across draws and frames. Individual data ranges are
retained by consuming raster/compute work until GPU completion or discard, then
recycled within the pool. Do not create or destroy a GPU buffer per draw. Recorded or in-flight ranges must not be
overwritten. Flush non-coherent mapped ranges and establish host-write visibility
before shader consumption. The caller may release source CPU input after it is
copied into the mapped pool. The pool is shared by kernels and holds streamed
data such as per-draw arguments; its lifetime is independent of any one record
or CnC upload. Material/uniform storage has its own reuse policy. Geometry uses ordinary vertex/index bindings.

New raster/compute recorders capture the heap and common buffers at pass scope.
Kernel and material switches do not construct per-draw/per-dispatch resource
tables or switch material-buffer bindings. App-defined kernels must have a typed
extension path without exposing private descriptor assembly.

### Shared states, sky selection, and lifetime

New `fx2::bindless::SharedShaderConstants::recordUpdate(typedParameters)` returns
an opaque retained shared state and access to its update payload. It does not
change the existing FX2 SSC API. Each update captures its own combined SSC bytes.
Two updates from one SSC must support this ordering with distinct results:

```text
upload A -> upload B -> consumers B -> consumers A
```

The state, upload work, unsealed recordings, and all consuming payloads retain
allocation leases. Recycle storage only after the final lease releases; upload
completion alone is insufficient. Capture backing storage through growth, or
fail explicitly at a documented capacity, without invalidating recorded work.
Never overwrite/free a descriptor slot still referenced by recorded or in-flight
work.

Sky materials contain background, diffuse, specular, and BRDF lookup resources
plus sampler references and lighting parameters. The SSC selects one sky material;
skybox and lit kernels read the same selection. Preserve existing exposure,
luminance, ambient-floor, and no-sky behavior.

### Uploads and hazards

Applications submit initialization/material producers and SSC updates before
their consumers. FX2 records work without implicitly submitting, presenting, or
waiting. A pass returns its own work and does not duplicate producers shared by
multiple passes. Dropped initialization must be retryable or cause explicit
uninitialized-use rejection. Recorders copy immediate CPU input and retain GPU
resources through completion or cancellation.

Bindless indices do not describe resource hazards. Track sampled/storage views,
geometry, attachments, transfers, and compute writes explicitly. Reject unsupported
attachment feedback before appending work. Preserve blur intermediates, in-place
semantics, odd edges, per-mip dependencies, and raster/compute visibility.

### Delivery and verification

Build the new SSC shared-resource owner and heap integration first, then heap-owned material-buffer integration, combined
SSC versions, a minimal pass/unlit path, sky/lit kernels, the mapped CPU-to-GPU streaming pool,
image kernels, and an independent ImGui backend. Add an opt-in bindless sample;
existing samples, tools, and E2/RDG2 paths continue to use their existing APIs.
Verify each step before starting the next. Final checks cover GPU readback,
descriptor/allocation lifetime, cancellation, validation-clean samples, and a
reproducible 10,000-draw recording-cost comparison. Previous stash test results
are historical evidence, not verification of the second attempt.

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
| 0 | gpu2-defined | gpu2 bindless descriptor heap, material buffer included | Shared; gpu2-owned |
| 1 | 0 | CPU-to-GPU streaming SSBO | SSC streaming pool; ranges recycled after consumption |
| 1 | 1 | Shared-shader-constant (SSC) uniform buffer | Captured shared-state version |
| 2+ | As needed | Optional additional pass resources | Explicit extension point |

Set 0's descriptor and material contracts belong to gpu2, as documented in
[gpu2 design](../gpu2/README.md). Callers share the heap directly with
Raster/CnC; SSC neither owns nor requires it. Texture and sampler descriptors
remain separate so many textures can share a few samplers.

Set 1 belongs entirely to SSC and is not part of the heap. It is supplied through
`Raster`/`CnC` `CreateParameters::passResources`, indexed by absolute set number as
`passResources[set][binding][arrayIndex]`. `passResources[heapSetIndex]` must be
empty or recorder creation is rejected. Binding number equals the slot array index,
and empty slots are skipped by both the pipeline-layout builder and the descriptor
writer, so the two stores can be delivered independently. Descriptor type derives
from `slot[0].bufferView.type`: `STORAGE` becomes a storage buffer, anything else a
uniform buffer. Binding 0 must therefore be `BufferView::STORAGE` and binding 1
`BufferView::UNIFORM`.

Environment resources have no dedicated bindings. A sky is an ordinary gpu2 heap
material chunk; its textures register in the heap like any other sampled texture,
and the SSC uniform records only which chunk is selected.

### SSC-owned storage

A long-lived `fx2::bindless::SharedShaderConstants` (SSC) owns the uniform and
streaming stores shared by new kernels. There is no separate
`fx2::bindless::Device` class. SSC is created with a GPU context and has no
descriptor-heap dependency.

| Store | Set / binding | Entry lifetime | Transfer mechanism |
| --- | --- | --- | --- |
| Streaming pool | 1 / 0 | One-time use, until the consuming payload releases it | None: CPU writes a mapped range |
| Versioned shared-state uniforms | 1 / 1 | One recorded version, until its last lease releases | Caller-supplied CnC |

Long-lived, read-only, non-uniform records, including materials, are not an
SSC store. The gpu2 descriptor heap owns the material buffer, as described in
[gpu2 design](../gpu2/README.md#b1-built-in-material-buffer). Callers manage
that heap separately and provide it directly to Raster/CnC. SSC exposes no
heap accessor or material allocator. Across its two stores SSC is responsible
for GPU backing buffers, allocation, upload batching, versioning, resource
retention, and safe recycling.

#### Versioned uniform store

SSC's uniform management is generic: it allocates, versions, uploads, and
recycles opaque byte ranges. The standard `SharedUniforms` schema is defined in
the public bindless SSC header, without making the storage implementation
interpret it. One 1,024-byte std140 UBO contains frame counter/duration, a fixed
array of 16 legacy-compatible direct-light records with an active count, all
camera/view fields, and an active sky-material index. There is no separate
camera or light buffer. Environment texture bindings and calibration fields
are absent; those belong to sky materials. `NO_SKY_MATERIAL` is UINT32_MAX;
index zero remains valid. Callers fill the struct and upload all its bytes with
`recordUniformUpdate()`.

Each update captures its own bytes in a newly allocated, independently aligned
range, so two updates are distinct GPU bytes rather than two writes into one
buffer. That is what makes update order independent of draw order:

```text
upload A -> upload B -> consumers B -> consumers A
```

Range offsets and sizes must obey GPU uniform buffer alignment and range limits.
Multiple passes share one state without duplicating producer work. The existing
`fx2::SharedShaderConstants` and its snapshot API are unchanged.

#### Streaming pool

The pool holds one-time-use per-draw data that a kernel wants the GPU to read but
that is too large for push constants. It is backed by one or a few long-lived
mappable buffers reused across draws and frames; no buffer is created or destroyed
per draw, and no CnC transfer is involved. Allocation takes only a size and returns
a CPU-writable block — an identifier plus a view over the mapped range — which the
caller fills. The consuming raster or CnC payload owns the release and calls it on
completion or discard, recycling the range within the pool. Shaders read the pool
as an SSBO at set 1, binding 0 through a per-draw offset encoded in the kernel's
push constants.

When the backend and memory allocation support it, keep pool buffers persistently
mapped for their lifetime to avoid repeated map/unmap calls; otherwise map as
needed. Map/unmap is not the mechanism for synchronizing reuse with GPU consumers:
recorded or in-flight ranges must not be overwritten, and that is enforced by
deferring release to payload completion rather than by mapping state. Flush
non-coherent mapped ranges and establish host-write visibility before shader
consumption. The caller may release its source CPU input once it has been copied
into the mapped range.

#### Kernels and push constants

Kernels define their typed inputs and private shader schemas, retain their
texture/sampler dependencies, and consume opaque chunks from the gpu2 heap-owned
material buffer. They do not own separate arenas or independently manage uploads of
shared records. Unlit, PBR, Lambertian, Cel, and Sky schemas are FX2-specific
interpretations of those opaque chunks; they share one material binding and reuse a
prepared record across draws without per-draw material uploads. FX2 retains
referenced registrations and delays freeing chunks until consumers finish.

Each kernel controls its complete push-constant layout: ordinary transforms, flags,
and optional material references use push constants, with no universal packet or
mandatory material-address field. Vertex and index buffers remain ordinary geometry
bindings. New raster/compute recorders capture the heap and common buffers at pass
scope; kernel and material switches do not construct per-draw/per-dispatch resource
tables or switch material-buffer bindings. App-defined kernels must have a typed
extension path without exposing private descriptor assembly. Applications explicitly
submit SSC-prepared producer work before its consumers.

### Lifetime and leases

Recording an update returns an opaque retained shared state plus access to its
update payload; it does not change the existing FX2 SSC API. The state, its upload
work, unsealed recordings, and all consuming payloads retain a lease on the ranges
they reference. Recycle storage only after the final lease releases; upload
completion alone is insufficient, because a consumer may still be recorded or in
flight. Capture backing storage through growth, or fail explicitly at a documented
capacity, without invalidating recorded work. Never overwrite or free a descriptor
slot still referenced by recorded or in-flight work.

Sky materials are ordinary gpu2 heap material chunks containing background,
diffuse, specular, and BRDF lookup resources plus sampler references and lighting
parameters; their textures register in the shared heap like any other sampled
texture. The SSC uniform records only which sky material is selected, as a heap
material token, so choosing a different sky changes state rather than rebinding
textures. Skybox and lit kernels read that same selection. Preserve existing
exposure, luminance, ambient-floor, and no-sky behavior.

### Uploads and hazards

Applications submit initialization/material producers and SSC updates before
their consumers. FX2 records work without implicitly submitting, presenting, or
waiting. A pass returns its own work and does not duplicate producers shared by
multiple passes. Dropped initialization must be retryable or cause explicit
uninitialized-use rejection. Recorders copy immediate CPU input and retain GPU
resources through completion or cancellation.

Follow gpu2's read-ready invariant: raster readers perform no per-draw table
scanning, resource tracking, or barriers. Writers declare destinations at
operation/pass scope and restore read-ready state after completion. Schedule
writers before readers. Do not sample an active render attachment or mutate
heap descriptors still used by recorded/in-flight consumers.

### Delivery and verification

Build the SSC framework skeleton first, then its versioned uniform store and its
mapped CPU-to-GPU streaming pool, then the combined scene uniform and its typed
packing. Material storage comes from the gpu2 heap and needs no FX2 implementation
step. Later work — a minimal pass/unlit path, sky and lit kernels, image kernels,
an independent ImGui backend, and an opt-in bindless sample — is tracked in the
assignment plan. Existing samples, tools, and E2/RDG2 paths continue to use their
existing APIs. Verify each step before starting the next. Final checks cover GPU
readback, descriptor/allocation lifetime, cancellation, validation-clean samples,
and a reproducible 10,000-draw recording-cost comparison. Previous stash test
results are historical evidence, not verification of the second attempt.

### Simple bindless kernels

`fx2::bindless::UnlitKernel` and `LambertianKernel` use only SSC's public
`UniformState` contract and the standard `SharedUniforms` schema in the SSC
header. Upload the bytes with `recordUniformUpdate()`, then use
`sharedUniformResources(state)` as the caller-owned Raster's pass resources.
Recording a draw retains the state lease until payload completion/discard.
The producer is submitted once before all consumers; kernels never submit it.

Create a kernel with a gpu2 heap, then create immutable materials from that
kernel. Material parameters use `GpuResourceView` and `Sampler` directly; FX2
allocates the required heap descriptors and releases them with material
lifetime. Each material retains its kernel, so the heap and built-in fallback
resources remain valid. The kernel exposes default material parameters, which
callers can copy and customize. The heap is bound at set 0 with bindingIndex 0.
Optional color maps modulate the input color. Lambertian also accepts a
tangent-space normal map, deriving its tangent frame from world-position/UV
derivatives. UV location 2 is needed only when a map is enabled; normal
location 1 is needed for Lambertian.
Unlit bypasses lighting/exposure. Lambertian handles up to 16 direct lights, inverse-transpose normals, back faces, and exposure/Reinhard.
Both support emissive and alpha cutoff, inherit raster policy, and use 128
vertex-stage push bytes. These draws need no streaming allocation.

Readers follow gpu2's read-ready invariant and add no per-draw resource
tracking. Callers schedule writers first and avoid attachment feedback. FX2
retains material descriptor slots through recorded/in-flight draws. SSC has no
heap dependency. Its current implementation is still a dummy, so this exercise
has compile/link verification only; runtime rendering has not been tested.

The standard direct-light ABI matches legacy FX2's three vec4 light records.
Lambertian handles directional and point attenuation/range; SPOT currently
uses point attenuation, matching the legacy Lambertian shader. There is no
ambient or environment texture contribution until sky-material sampling is
implemented. Camera exposure remains in the shared UBO.

SharedUniforms physical order is frame/sky header, camera fields, then direct
lighting. numLights is at byte 244 and lights[16] starts at byte 256. The
combined UBO remains 1024 bytes, with a fixed maximum of 16 direct lights.

The simple-unlit sample now exercises the public bindless API flow at compile
level: heap/SSC/kernel creation, shared uniform upload, pass resource bindings,
two draws, producer/consumer sealing, submission, and presentation. Its 4x4
RGBA8 black/white checker is uploaded once and supplied as a resource view to
the unlit material; its nearest sampler is supplied directly as well.
Position/UV vertices feed the textured unlit variant.
No private SSC or shader ABI headers are included by the sample. This is a
compile-only exercise: dummy SSC still returns an empty uniform state, so no
runtime image or descriptor-heap behavior has been verified yet.

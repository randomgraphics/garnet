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

## TODO: Bindless Resource Management (GpuRaster & GpuCnC)

To support rendering thousands of unique items at high frame rates (>400 FPS)
without CPU-side descriptor thrashing or per-draw allocation overhead, FX2 and
gpu2 will evolve toward a **bindless resource architecture** managed by `GpuRaster`
and `GpuCnC`:

### Architecture & Direction

1. **Pass-Level Bindless in `GpuRaster`**:
   - `GpuRaster` already aggregates every texture and buffer active within a pass
     during resource collection (`collectPassResources()`).
   - Rather than assembling per-draw `GpuResourceTable` descriptors and compiling
     unique descriptor sets per item, `GpuRaster` will manage a single pass-wide
     bindless descriptor set (e.g. Set 1: unbounded descriptor arrays for textures
     and material storage buffers).
   - Individual draws push 32-bit indices (`materialId`, `textureId`, `worldMatrix`)
     via push constants / immediates, reducing CPU descriptor management per draw
     to zero.

2. **Bindless for Compute in `GpuCnC`**:
   - Compute pipelines (`GpuCnC::recordCompute`) will share the same bindless
     descriptor model (`VK_PIPELINE_BIND_POINT_COMPUTE`).
   - Dispatches index inputs/outputs dynamically via push constants, eliminating
     per-dispatch `GpuResourceTable` and `DrawPack` compilation.

3. **Optimized Transfers in `GpuCnC`**:
   - Copy and upload operations (`recordUploadBuffer`, `recordCopyBufferToBuffer`)
     are fixed-function DMA operations that do not use descriptors.
   - Migrate upload staging from individual `Buffer` allocations to a persistent,
     mapped ring staging buffer.
   - Updating buffer contents via `GpuCnC` transfers does not invalidate bindless
     descriptors (handles remain unchanged); only standard pipeline barriers
     (`TRANSFER_WRITE` -> `SHADER_READ`) are emitted.

### Action Items

- [ ] **GpuRaster pass-wide bindless descriptor set**: Build and bind a single
      bindless descriptor set per raster pass using `VK_EXT_descriptor_indexing`.
- [ ] **FX2 material parameter storage buffer**: Replace per-draw `Buffer::create`
      for `ModelMaterialUBO` with a shared material SSBO or dynamic uniform buffer
      suballocation indexed by `materialId`.
- [ ] **Shader ABI & push constant indexing**: Update lit kernels to look up textures
      and material parameters through push constant indices.
- [ ] **GpuCnC compute bindless support**: Enable compute dispatches to consume
      bindless descriptor sets without building ad-hoc resource tables.
- [ ] **GpuCnC persistent staging ring allocator**: Replace per-upload staging
      buffer creations with a persistent host-visible ring buffer.
- [ ] **Backend allocation cleanup**: Eliminate red-black tree allocations (`std::set`)
      in rapid-vulkan `DrawPack::Dependencies` and pool descriptor snapshot blobs.


# RDG2 SharedShaderConstants — Design Spec

**Date:** 2026-05-15  
**Status:** Approved

---

## Overview

Implement `GN::rdg2::SharedShaderConstants` — the v2 equivalent of the v1 pipeline-library class of the same name. It holds per-frame scene constants (camera, lighting, environment) on the CPU, snapshots them each frame into GPU-optimal device-local UBO buffers via an async staging-copy path, and exposes a `GpuResourceSet` that render nodes bind as descriptor set 0.

---

## Motivation

`render-to-cube2.cpp` currently hardcodes the camera transform inside `CubeDraw::addFrameNode()`. SSC provides a clean, reusable home for all per-frame shader constants so render nodes can consume them without duplicating camera/lighting math.

---

## Public Interface (header changes)

File: `src/inc/garnet/rdg/2/shared-shader-constants2.h`

### `EnvironmentLightingConstants` — replace alias with native struct

The existing alias (`= GN::rdg::SharedShaderConstants::EnvironmentLightingInformation`) uses `rdg::Texture`. Replace it with a native struct using `gpu2::Texture`:

```cpp
struct EnvironmentLightingConstants {
    AutoRef<GN::gpu2::Texture> skyboxCubemap;
    AutoRef<GN::gpu2::Texture> irradianceMap;
    AutoRef<GN::gpu2::Texture> prefilteredEnvMap;
    AutoRef<GN::gpu2::Texture> brdfLut;
    float                      environmentRadianceScale = 1.f;
};
```

`FrameConstants` and `DirectLight` keep their existing v1 aliases (no GPU types involved).

### `set0` — public, directly writable

`SharedShaderConstants::set0` is the sole source of per-frame data. App code writes to it directly:

```cpp
ssc->set0.camera.cameraPosition    = eye;
ssc->set0.camera.cameraFov         = Degree(60.f);
ssc->set0.environmentLighting.brdfLut = myLut;
ssc->set0.directLighting.append(light);
```

No separate setter methods.

---

## GPU Resource Layout (set 0)

| Binding | Type    | Content                                           | Default at `create()` |
|---------|---------|---------------------------------------------------|------------------------|
| 0       | UBO     | SceneUBO: frame + direct lighting + env params    | uploaded each snapshot |
| 1       | UBO     | CameraUBO: view/proj matrices + camera position   | uploaded each snapshot |
| 2       | Texture | `set0.environmentLighting.skyboxCubemap`          | 1×1 black RGBA8 cubemap |
| 3       | Texture | `set0.environmentLighting.irradianceMap`          | 1×1 black RGBA8 cubemap |
| 4       | Texture | `set0.environmentLighting.prefilteredEnvMap`      | 1×1 black RGBA8 cubemap |
| 5       | Texture | `set0.environmentLighting.brdfLut`                | 1×1 white RGBA8 2D |

---

## UBO Structs (new v2 headers)

New headers under `src/core/rdg/2/vk-shaders/`. Dual C++/GLSL, std140.

### `scene-ubo.h` — binding 0

```
frameCounter              uint   offset   0   (4 bytes)
frameDurationMs           float  offset   4   (4 bytes)
environmentRadianceScale  float  offset   8   (4 bytes)
numLights                 uint   offset  12   (4 bytes)
lights                    DirectLightData[16]  offset 16  (768 bytes)
Total: 784 bytes
```

`DirectLightData` (48 bytes each): `positionOrDir` vec4, `colorAndRange` vec4, `coneAngles` vec4 — same layout as v1's `direct-lighting-ubo.h`.

### `camera-ubo.h` — binding 1

```
viewMatrix        mat4   offset   0   (64 bytes)
projMatrix        mat4   offset  64   (64 bytes)
viewProjMatrix    mat4   offset 128   (64 bytes)
cameraPosition    vec4   offset 192   (16 bytes)
renderTargetSize  vec2   offset 208   ( 8 bytes)
nearPlane         float  offset 216   ( 4 bytes)
farPlane          float  offset 220   ( 4 bytes)
Total: 224 bytes
```

No `frameCounter`/`frameDurationMs` in the camera UBO (those live in SceneUBO). V1's `GlobalCameraUBO` is unchanged.

---

## Implementation — `SharedShaderConstants2Impl`

File: `src/core/rdg/2/shared-shader-constants2.cpp`

### Persistent members (lifetime = SSC object)

```
mGpu                  AutoRef<gpu2::GpuContext>
mGraph                AutoRef<Graph>
mSceneBuffer          AutoRef<gpu2::Buffer>   // device-local, 784 bytes, binding 0
mCameraBuffer         AutoRef<gpu2::Buffer>   // device-local, 224 bytes, binding 1
mFallbackCubemap      AutoRef<gpu2::Texture>  // 1×1 black RGBA8, faces=6
mFallbackBrdfLut      AutoRef<gpu2::Texture>  // 1×1 white RGBA8, faces=1
mContentMap           mutable std::unordered_map<TokenPtr, AutoRef<Content>>
mMutex                mutable std::mutex      // guards mContentMap
```

`init()` creates the two device-local buffers and two fallback textures (via `Texture::create` + `setContent` — one-time static upload, acceptable for init). It also populates `set0.environmentLighting.*` with the fallback textures so bindings 2–5 are always valid.

### `takeSnapshot() const`

1. Lock `mMutex`; copy `set0` into a local `Set0Parameters snapshot`; unlock.
2. Create `AutoRef<Content> content`.
3. Pre-build `content->set0Resources` (6 slots):
   - Slot 0: UBO buffer view → `mSceneBuffer`
   - Slot 1: UBO buffer view → `mCameraBuffer`
   - Slots 2–5: image views from `snapshot.environmentLighting.*`
4. Add graph node; node action:
   a. Create 2 mappable staging buffers (scene: 784 B, camera: 224 B).
   b. Pack `SceneUBO` from `snapshot` (frame counters + lights + env scale); write via `staging->map()` / memcpy.
   c. Pack `CameraUBO` from `snapshot` (matrices via GLM, render-target size from `snapshot.renderTarget`); write via `staging->map()` / memcpy.
   d. Create `GpuCnC`; record `copyBufferToBuffer` for both pairs (staging → device-local).
   e. `cnc->seal()` → `content->set0Payload`. The payload must hold refs to the staging buffers internally to keep them alive until the GPU copy completes. If inspection of `GpuCnC`'s Vulkan implementation shows it does not retain source buffer refs, store the staging buffers directly in `Content` (add two `AutoRef<gpu2::Buffer>` fields) as a fallback.
5. Get node completion token `t`.
6. Lock `mMutex`; insert `{t → content}` into `mContentMap`; unlock.
7. Return `t`.

**Token semantics:** satisfied after CPU-side recording completes (GPU copy is submitted but may still be in flight). Dependent render nodes must chain `content->set0Payload` into their `gpuContext->submit()` as a `waitFor` dependency.

### `getContent(TokenPtr t) const`

Lock `mMutex`; look up `mContentMap[t]`; return content (empty ref if not found).

### Camera matrix packing

Same as v1: `viewMatrix = inverse(translate(pos) * mat4_cast(orientation))`, `projMatrix = perspectiveRH_ZO(fov, aspect, near, far)` with Y-flip for Vulkan NDC, `viewProjMatrix = proj * view`. Render-target size deduced from first valid color target or depth target in `snapshot.renderTarget`.

---

## Changes to `render-to-cube2.cpp`

### `SharedCtx` — add SSC

```cpp
struct SharedCtx {
    AutoRef<Graph>                     graph;
    AutoRef<GpuContext>                gpu;
    AutoRef<rdg2::SharedShaderConstants> ssc;
    uint32_t                           width, height;
};
```

### `main()` — create SSC, set up camera each frame

After graph creation:
```cpp
auto ssc = rdg2::SharedShaderConstants::create({.gpu = gpuContext, .graph = graph});
```

Per frame, replace the hardcoded `view`/`proj` computation in `CubeDraw` with writes to `ssc->set0.camera`:
```cpp
ssc->set0.camera.cameraPosition    = glm::vec3(0.f, 0.f, 3.f);
// derive orientation from lookAt: quat_cast(mat3(inverse(lookAtRH(eye, target, up))))
ssc->set0.camera.cameraOrientation = glm::quat_cast(glm::mat3(glm::inverse(glm::lookAtRH(eye, target, up))));
ssc->set0.camera.cameraFov         = Degree(45.f);
ssc->set0.camera.aspectRatio       = (float)windowWidth / windowHeight;
ssc->set0.camera.nearPlane         = 0.1f;
ssc->set0.camera.farPlane          = 100.f;
ssc->set0.renderTarget.setColorTarget(0, frame.view);

TokenPtr sscToken = ssc->takeSnapshot();
```

### `CubeDraw::addFrameNode()` — consume SSC

- Adds `sscToken` as a dependency.
- Inside the action: `ssc->getContent(sscToken)` → `content`.
- `resources` table: set 0 = `content->set0Resources`, set 1 (slot 0) = cubemap view.
- Model matrix passed as push constant (unchanged).
- GPU submit: `.waitFor(content->set0Payload)`.

### `cube-draw.vert` shader update

- Read `view` and `proj` from camera UBO at `set=0, binding=1`.
- Keep `model` matrix as a push constant (64 bytes).
- Compute `gl_Position = proj * view * model * vec4(pos, 1.0)`.
- Cubemap sampler moves from `set=0, binding=0` to `set=1, binding=0` in the fragment shader.

---

## Files touched

| File | Change |
|---|---|
| `src/inc/garnet/rdg/2/shared-shader-constants2.h` | Replace `EnvironmentLightingConstants` alias with native struct |
| `src/core/rdg/2/shared-shader-constants2.cpp` | Full implementation of `SharedShaderConstants2Impl` |
| `src/core/rdg/2/vk-shaders/scene-ubo.h` | New: SceneUBO struct (dual C++/GLSL) |
| `src/core/rdg/2/vk-shaders/camera-ubo.h` | New: CameraUBO struct (dual C++/GLSL) |
| `src/sample/rdg/cube-draw.vert` | Read view/proj from camera UBO; model stays push constant |
| `src/sample/rdg/cube-draw.frag` | Move cubemap sampler to set=1, binding=0 |
| `src/sample/rdg/render-to-cube2.cpp` | Integrate SSC; remove hardcoded camera math from CubeDraw |

SPIR-V headers (`cube-draw-vert.spv.h`, `cube-draw-frag.spv.h`) are regenerated as part of the build.

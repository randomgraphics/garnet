# RDG2 Simple Effects: PbrShading, SkyBox, and SSC Env Texture Artifact Model

**Date:** 2026-05-16  
**Status:** Approved for implementation

---

## Overview

Introduce two new rdg2 pipeline-level effects — `GN::rdg2::PbrShading` and `GN::rdg2::SkyBox` — declared in a new public header `simple-effect.h`. Simultaneously refactor `SharedShaderConstants` to replace the direct `set0.environmentLighting` texture assignment with the same artifact+token async-loading model used by `PbrShading`.

The unifying principle: **every file-backed GPU asset is a graph artifact**. The artifact is published immediately at creation time with procedural defaults (so rendering is never blocked), and a worker thread publishes a new version when real disk content is ready.

---

## Motivation

`draw-mesh2.cpp` currently wires shaders, textures, geometry, and SSC manually into the graph — 600+ lines of boilerplate. The new effects encapsulate that boilerplate behind a clean API:

```cpp
auto ssc    = SharedShaderConstants::create({gpu, graph});
auto pbr    = PbrShading::create({gpu, graph});
auto skybox = SkyBox::create({gpu, graph});

auto [envArtifact, envReady] = ssc->loadEnvTextures({.skyboxPath = ..., .simulateSlowLoading = true});
auto [helmetArtifact, helmetReady] = pbr->load({.gltfPath = ..., .albedoPath = ..., .simulateSlowLoading = true});

// render loop — works from frame 1; defaults visible until async IO completes
```

---

## Files

| Action | Path |
|--------|------|
| **New** | `src/inc/garnet/rdg/2/simple-effect.h` |
| **New** | `src/core/rdg/2/simple-effect.cpp` |
| **New** | `src/core/rdg/2/vk-shaders/pbr.vert` |
| **New** | `src/core/rdg/2/vk-shaders/pbr.frag` |
| **New** | `src/core/rdg/2/vk-shaders/skybox.vert` |
| **New** | `src/core/rdg/2/vk-shaders/skybox.frag` |
| **New** | `src/sample/rdg/simple-pbr2.cpp` |
| **Modify** | `src/inc/garnet/GNrdg2.h` — add `#include "rdg/2/simple-effect.h"` |
| **Modify** | `src/inc/garnet/rdg/2/shared-shader-constants2.h` — replace env lighting fields with artifact model |
| **Modify** | `src/core/rdg/2/shared-shader-constants2.cpp` — impl of `loadEnvTextures()` |
| **Modify** | `src/sample/rdg/CMakeLists.txt` — new sample target + v2 shader compile rules |
| **Modify** | `src/core/rdg/2/CMakeLists.txt` — compile v2 shaders, build `simple-effect.cpp` |

---

## Part 1 — SharedShaderConstants changes

### Removed

`Set0Parameters::EnvironmentLightingConstants` and `set0.environmentLighting` are removed. Direct texture assignment (`ssc->set0.environmentLighting.skyboxCubemap = tex`) is no longer supported.

### Added: `EnvTextureSet`

Plain data struct that becomes the artifact payload for environment textures:

```cpp
/// Artifact payload for environment/IBL textures.
/// Published immediately at loadEnvTextures() with procedural blue-sky defaults;
/// updated asynchronously when real DDS files finish loading.
struct EnvTextureSet {
    AutoRef<gpu2::Texture> skyboxCubemap;     ///< env cubemap for SkyBox pass
    AutoRef<gpu2::Texture> irradianceMap;     ///< diffuse IBL (pre-convolved)
    AutoRef<gpu2::Texture> prefilteredEnvMap; ///< specular IBL (mip-mapped)
    AutoRef<gpu2::Texture> brdfLut;           ///< split-sum BRDF LUT
    float environmentRadianceScale = 1.f;    ///< linear HDR multiplier
};
```

**Default content (v1, published synchronously):**
- `skyboxCubemap` — 6-face cubemap, all faces sky-blue (R=135 G=206 B=235 A=255 ≈ `#87CEEB`)
- `irradianceMap` — 6-face cubemap, muted blue (R=34 G=68 B=102 A=255) — soft ambient from sky
- `prefilteredEnvMap` — same muted blue as irradiance (single mip sufficient for 1×1 fallback)
- `brdfLut` — 1×1 RGBA8 (255, 255, 0, 0) ≈ scale=1, bias=1 (neutral split-sum approximation)
- `environmentRadianceScale = 1.f`

### Added: `loadEnvTextures()`

```cpp
struct EnvLoadParameters {
    StrA skyboxPath;
    StrA irradiancePath;
    StrA prefilteredPath;
    StrA brdfLutPath;
    float environmentRadianceScale = 1.f;
    bool  simulateSlowLoading = false; ///< sleep ~5 s on worker before publishing
};
struct EnvLoadResult {
    ArtifactPtr artifact; ///< EnvTextureSet; v1 = blue-sky defaults, v2 = loaded
    TokenPtr    ready;    ///< satisfied when async IO completes and v2 is published
};
GN_API EnvLoadResult loadEnvTextures(const EnvLoadParameters &);
```

### `takeSnapshot()` change

`takeSnapshot()` now calls `mGraph->getTypedArtifactContent<EnvTextureSet>(mEnvArtifact)` internally to fetch the latest env textures, copies them into `Content`, and forwards them to `buildSet0Resources()`. The `Content` struct gains an `EnvTextureSet envTextures` field for this snapshot.

`environmentRadianceScale` moves from `EnvironmentLightingConstants` into `EnvTextureSet` and is read from the artifact snapshot when packing `SceneUBO`.

### Async-IO flow for env textures

Default 1×1 cubemap/LUT textures (blue-sky defaults, v1) are created with `setContent()` — they are tiny and created once at startup, not on any hot path.

Real asset loading uses the staging-buffer pattern so the graph worker is never blocked:

```
ssc->loadEnvTextures(params):
  1. Creates mEnvArtifact
  2. Builds default EnvTextureSet (1×1 procedural textures via setContent) → publishArtifact v1
  3. ready = graph->getArtifactVersionToken(artifact, OOO())  [satisfied on v2 publish]
  4. Spawns std::thread:
       a. Read 4 DDS files from disk into CPU memory (std::vector<uint8_t> per file)
          [if simulateSlowLoading: sleep ~5 s HERE, after file read, inside staging write]
       b. For each texture:
            Create mappable staging Buffer (size = pixel byte count)
            staging->map() → memcpy pixel data → unmap()
            Create device-local Texture (permanent resource)
       c. Create GpuCnC; record copyBufferToImage() for each texture
       d. payload = cnc->seal()
       e. Build realEnvTextureSet with the device-local Textures
       f. gpu->submit({.work = payload, .onComplete = [graph, artifact, realEnvTextureSet]() {
              graph->publishArtifact(artifact, realEnvTextureSet);  // v2, satisfies ready
          }})
       g. Worker thread exits; staging buffers kept alive by GpuCnC payload until GPU copy done

Main/render loop calls gpu->pump() each frame → fires onComplete when GPU fence signals

takeSnapshot() each frame:
  reads latest EnvTextureSet from mEnvArtifact via getArtifactContent()
  → v1 (blue-sky defaults) until onComplete fires; v2+ thereafter
```

---

## Part 2 — `simple-effect.h` API

### `PbrAsset`

```cpp
/// Graph artifact payload for one renderable PBR object: material textures + mesh geometry.
/// Default (v1, published synchronously in load()): white-matte 1×1 textures + procedural 1×1×1 cube.
/// Async-loaded version (v2): real textures from DDS files + real mesh from GLTF.
struct PbrAsset {
    AutoRef<gpu2::Texture>  albedo;      ///< sRGB base color   — default 1×1 white (255,255,255)
    AutoRef<gpu2::Texture>  normal;      ///< tangent-space N   — default 1×1 flat  (128,128,255)
    AutoRef<gpu2::Texture>  emissive;    ///< emissive radiance — default 1×1 black  (0,0,0)
    AutoRef<gpu2::Texture>  occlusion;   ///< ambient occlusion — default 1×1 white  (255,255,255)
    AutoRef<gpu2::Texture>  metalRough;  ///< metallic(B)+rough(G) — default (0,128,0): non-metal, medium-rough
    gpu2::RasterGeometry    geometry;    ///< mesh — default: unit cube (12 triangles, interleaved pos+norm+uv)
};
```

**Default cube geometry:** Unit cube centred at origin (−0.5…+0.5 on each axis), 24 vertices (4 per face, with per-face normals and UVs), 36 indices (12 triangles). Interleaved layout: `vec3 pos | vec3 normal | vec2 uv` = 32 bytes/vertex.

### `SkyBox`

```cpp
struct SkyBox : Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

    struct CreateParameters {
        AutoRef<gpu2::GpuContext> gpu;
        AutoRef<Graph>            graph;
    };

    /// Satisfied once skybox shaders are compiled (one-time graph node).
    /// Render nodes must dependsOn(skybox->readyToken()) before calling getDrawParams().
    GN_API TokenPtr readyToken() const;

    /// Build a DrawParameters for one fullscreen skybox pass.
    /// Must be called inside a graph node action.
    /// @param sscContent  SSC snapshot for this frame; provides set0 (camera + env textures).
    GN_API gpu2::GpuRaster::DrawParameters getDrawParams(
        AutoRef<const SharedShaderConstants::Content> sscContent) const;

    GN_API static AutoRef<SkyBox> create(const CreateParameters &);

protected:
    using Entity::Entity;
};
```

`SkyBox` internally compiles `skybox.vert` + `skybox.frag` as a one-time graph node. The draw uses a 3-vertex fullscreen triangle (no VBO), depth pinned to 1.0, `LESS_EQUAL` depth test.

### `PbrShading`

```cpp
struct PbrShading : Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

    struct CreateParameters {
        AutoRef<gpu2::GpuContext> gpu;
        AutoRef<Graph>            graph;
    };

    struct LoadParameters {
        StrA gltfPath;                        ///< mesh geometry source
        StrA albedoPath, normalPath,
             emissivePath, occlusionPath,
             metalRoughPath;                  ///< texture DDS paths; empty = keep default
        /// When true, inject a ~5 s sleep during staging-buffer writes to simulate slow disk I/O.
        /// The sleep happens on the worker thread; the graph worker is never blocked.
        bool simulateSlowLoading = false;
    };
    struct LoadResult {
        ArtifactPtr artifact; ///< PbrAsset; v1 = defaults, v2 = loaded content
        TokenPtr    ready;    ///< satisfied when async IO finishes and v2 is published
    };

    /// Satisfied once PBR shaders are compiled (one-time graph node).
    GN_API TokenPtr shadersReadyToken() const;

    /// Returns immediately. Publishes default PbrAsset (v1) synchronously,
    /// then spawns a worker thread that loads real content and publishes v2.
    /// Can be called multiple times — each call returns an independent artifact.
    GN_API LoadResult load(const LoadParameters &);

    /// Build DrawParameters for one PBR mesh draw. Must be called inside a graph node action.
    /// Reads latest PbrAsset from artifact; works from frame 1 (defaults always available).
    /// @param sscContent    SSC snapshot; provides set0 resources (camera + IBL).
    /// @param assetArtifact ArtifactPtr from a prior load() call.
    /// @param worldTransform Model-to-world matrix; normalTransform is derived internally.
    GN_API gpu2::GpuRaster::DrawParameters getDrawParams(
        AutoRef<const SharedShaderConstants::Content> sscContent,
        ArtifactPtr                                   assetArtifact,
        const glm::mat4 &                             worldTransform) const;

    GN_API static AutoRef<PbrShading> create(const CreateParameters &);

protected:
    using Entity::Entity;
};
```

**Async-IO flow for PbrShading:**

Default 1×1 textures and the procedural cube (v1) are created with `setContent()` / direct buffer writes — tiny, one-shot, not on any hot path.

Real file loading uses the staging-buffer pattern:

```
pbr->load(params):
  1. Creates a new ArtifactPtr
  2. Builds default PbrAsset (1×1 textures + procedural cube via setContent) → publishArtifact v1
  3. ready = graph->getArtifactVersionToken(artifact, OOO())
  4. Spawns std::thread:
       a. Read GLTF .json + .bin; decode vertex/index data into CPU vectors
          Read each non-empty DDS path into CPU memory
          [if simulateSlowLoading: sleep ~5 s HERE, during staging writes]
       b. For geometry:
            Create mappable staging Buffer (vertex bytes + index bytes)
            map() → write interleaved vertex data + index data → unmap()
            Create device-local vertex Buffer and index Buffer (permanent)
       c. For each non-empty texture path:
            Create mappable staging Buffer (pixel byte count)
            map() → memcpy pixel data → unmap()
            Create device-local Texture (permanent)
       d. Create GpuCnC:
            copyBufferToBuffer() for vertex + index data
            copyBufferToImage() for each texture
       e. payload = cnc->seal()
       f. Build realPbrAsset with device-local Buffers + Textures (not yet GPU-readable)
       g. gpu->submit({.work = payload, .onComplete = [graph, artifact, realPbrAsset]() {
              graph->publishArtifact(artifact, realPbrAsset);  // GPU copy done, v2 published
          }})
       h. Worker thread exits; staging buffers kept alive inside sealed payload until GPU done

Main/render loop calls gpu->pump() each frame → fires onComplete → artifact v2 published
```

**`getDrawParams()` internals:**
- Calls `graph->getTypedArtifactContent<PbrAsset>(assetArtifact)` → latest asset
- Set 0: `sscContent->set0Resources` (camera + IBL from SSC)
- Set 1: 5 texture views from `asset.albedo/normal/emissive/occlusion/metalRough`
- Push constants: `mat4 worldTransform | mat4 normalTransform` (128 bytes)
- `DrawParameters.geometry = asset.geometry`
- Cull mode: `CULL_BACK`, front face: `FRONT_CCW`

---

## Part 3 — Shader Layout (v2 UBO bindings)

Both effects use the existing v2 UBO headers (`scene-ubo.h`, `camera-ubo.h`):

| Set | Binding | Resource |
|-----|---------|----------|
| 0 | 0 | `SceneUBO` (lights, env radiance scale) |
| 0 | 1 | `CameraUBO` (view, proj, camera pos) |
| 0 | 2 | `sscSkyboxCubemap` (samplerCube) |
| 0 | 3 | `sscIrradianceMap` (samplerCube) |
| 0 | 4 | `sscPrefilteredEnvMap` (samplerCube) |
| 0 | 5 | `sscBrdfLut` (sampler2D) |
| 1 | 0–4 | PBR material textures (albedo, normal, emissive, occlusion, metalRough) |

Push constants (PBR vert only): `mat4 worldTransform` + `mat4 normalTransform` = 128 bytes.

**`pbr.vert`** — adapted from v1 `rdg/1/vk-shaders/pbr.vert`; replaces `global-camera-ubo.h` with `camera-ubo.h`. Push constant expanded to 128 bytes (`mat4 worldTransform | mat4 normalTransform`); normalTransform is pre-computed on CPU in `getDrawParams()` instead of in-shader (avoids per-vertex matrix inverse).  
**`pbr.frag`** — adapted from v1 `rdg/1/vk-shaders/pbr.frag`; replaces v1 include chain with `scene-ubo.h`. Adds set 1 bindings for occlusion (binding 3) and metalRough (binding 4) beyond the 3 in v1.  
**`skybox.vert`** — adapted from v1 `rdg/1/vk-shaders/skybox.vert`; replaces `global-camera-ubo.h` with `camera-ubo.h`.  
**`skybox.frag`** — adapted from v1 `rdg/1/vk-shaders/skybox.frag`; replaces include chain with `scene-ubo.h`.

---

## Part 4 — `simple-pbr2.cpp`

Demonstrates async loading: blue-sky default → real IBL; white-matte cube → real DamagedHelmet. Both transitions happen live while the render loop is running.

```cpp
// One-time setup
auto ssc    = SharedShaderConstants::create({gpu, graph});
auto pbr    = PbrShading::create({gpu, graph});
auto skybox = SkyBox::create({gpu, graph});

auto [envArtifact, envReady] = ssc->loadEnvTextures({
    .skyboxPath     = "media::envmap/bad-salzbrunn-walking-hall/skybox.dds",
    .irradiancePath = "media::envmap/bad-salzbrunn-walking-hall/irradiance.dds",
    .prefilteredPath= "media::envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
    .brdfLutPath    = "media::envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
    .environmentRadianceScale = 3500.f,
    .simulateSlowLoading = true,   // see blue sky, then real env kicks in after ~5 s
});

auto [helmetArtifact, helmetReady] = pbr->load({
    .gltfPath      = "media::pbr/DamagedHelmet/DamagedHelmet.gltf",
    .albedoPath    = "media::pbr/DamagedHelmet/albedo.dds",
    .normalPath    = "media::pbr/DamagedHelmet/normal.dds",
    .emissivePath  = "media::pbr/DamagedHelmet/emissive.dds",
    .occlusionPath = "media::pbr/DamagedHelmet/occlusion.dds",
    .metalRoughPath= "media::pbr/DamagedHelmet/metallic_roughness.dds",
    .simulateSlowLoading = true,   // see unit cube, then real helmet kicks in after ~5 s
});

// Render loop
while (running) {
    // ... update camera in ssc->set0 ...
    ssc->set0.renderTarget.setColorTarget(0, frame.view);
    TokenPtr sscSnapshot = ssc->takeSnapshot();

    AutoRef<GpuPayload> colorWork;
    graph->addNode(NodeDesc("color pass")
        .dependsOn(pbr->shadersReadyToken())
        .dependsOn(skybox->readyToken())
        .dependsOn(sscSnapshot)
        .setAction(Action::createFromLambda("draw", [&]() {
            auto sc = ssc->getContent(sscSnapshot);
            auto r  = GpuRaster::create({.gpu = gpu, .target = sc->set0Parameters.renderTarget});
            r->draw(skybox->getDrawParams(sc));
            r->draw(pbr->getDrawParams(sc, helmetArtifact, helmetWorldMat));
            colorWork = r->seal();
        }), nullptr));

    graph->waitForToken(graph->getNodeCompletionToken(colorPassNode));
    gpu->submit({colorWork, waitFor=frame.ready});
    swapchain->present(*colorWork);

    // Fire onComplete callbacks for completed GPU work (including texture/geometry upload completions).
    // This is what triggers artifact v2 publication once the staging→device GPU copies finish.
    gpu->pump();
}
```

Orbit camera and lighting setup are identical to `draw-mesh2.cpp`.

---

## Build System

**`src/core/rdg/2/CMakeLists.txt`** — add:
- Custom commands to compile the 4 new GLSL shaders to SPIR-V headers (output into `${CMAKE_CURRENT_BINARY_DIR}`)
- Add `simple-effect.cpp` to `GNcore` sources
- Include `${CMAKE_CURRENT_BINARY_DIR}` in `GNcore`'s private include directories

**`src/sample/rdg/CMakeLists.txt`** — add:
```cmake
add_executable(GNsample-rdg2-simple-pbr simple-pbr2.cpp)
target_link_libraries(GNsample-rdg2-simple-pbr GNcore)
```
(No per-sample shader compile step needed — v2 shaders compile as part of GNcore.)

---

## GLTF Parsing

`simple-effect.cpp` needs to parse GLTF for geometry loading. GNcore has no existing v2 GLTF helper. The minimal inline JSON + binary reader from `draw-mesh2.cpp` (anonymous namespace, function `loadGltfGeometryV2`) is promoted into `simple-effect.cpp` as a private helper `loadGltfGeometry()`. It handles interleaved `POSITION + NORMAL + TEXCOORD_0` with `uint16` indices, which covers the DamagedHelmet and most test meshes. This is adequate for the demo; a production-grade GLTF loader (via assimp) is a separate concern.

---

## Constraints and Non-Goals

- `Buffer::setContent()` is used **only** for procedural 1×1 default textures and the unit-cube geometry created at startup. It must never be used for real file-loaded assets — those always go through the staging-buffer → `GpuCnC` → `submit(onComplete)` path.
- `Texture::load()` (gpu2) is not used anywhere in this feature; it stalls the CPU and is test-only.
- Staging buffers are created as mappable (`Buffer::CreateParameters::mappable = true`) and are kept alive inside the sealed `GpuPayload` until the GPU copy completes.
- `simulateSlowLoading` adds a `std::this_thread::sleep_for(5s)` on the worker thread before `publishArtifact()`. It is a debug-only feature; the sleep must come after I/O is complete (so real files are ready when published, just delayed).
- The `simple-effect.h` effects are Vulkan-only. D3D12/Metal will return empty `DrawParameters` via the existing stub pattern.
- No test coverage added in this iteration; the visual output of `simple-pbr2.cpp` serves as the integration test.

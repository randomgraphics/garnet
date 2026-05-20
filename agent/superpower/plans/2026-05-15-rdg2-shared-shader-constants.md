# RDG2 SharedShaderConstants Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement `GN::rdg2::SharedShaderConstants` with async GPU UBO upload via staging buffers, and wire it into `render-to-cube2` so camera is driven through SSC instead of hardcoded inline math.

**Architecture:** Two device-local UBO buffers (SceneUBO = frame+lighting+env, CameraUBO = matrices) are uploaded each frame via `GpuCnC::copyBufferToBuffer` from per-snapshot mappable staging buffers. `takeSnapshot()` adds a graph node, returns its completion token; callers chain `content->set0Payload` as a GPU-side `waitFor` in their submit.

**Tech Stack:** C++17, `GN::gpu2` (Buffer, Texture, GpuCnC), `GN::rdg2` (Graph, Entity, Token), GLM (matrices/quaternions), rapid-image (fallback texture init), Catch2 (tests), GLSL 450 (shaders compiled via `compile_glsl_shader.py`)

---

## File Map

| File | Action | Purpose |
|---|---|---|
| `src/core/rdg/2/vk-shaders/scene-ubo.h` | Create | SceneUBO struct, dual C++/GLSL, std140 |
| `src/core/rdg/2/vk-shaders/camera-ubo.h` | Create | CameraUBO struct, dual C++/GLSL, std140 |
| `src/inc/garnet/rdg/2/shared-shader-constants2.h` | Modify | Replace `EnvironmentLightingConstants` alias with native `gpu2::Texture` struct |
| `src/core/rdg/2/shared-shader-constants2.cpp` | Modify | Full `SharedShaderConstants2Impl` implementation |
| `src/core/rdg/2/test/shared-shader-constants2-test.cpp` | Create | Catch2 unit tests (auto-picked up by `GNtest-internal`) |
| `src/sample/rdg/cube-draw.vert` | Modify | Read view/proj from camera UBO (set=0, binding=1); model stays push constant |
| `src/sample/rdg/cube-draw.frag` | Modify | Move `samplerCube` to set=1, binding=0 |
| `src/sample/rdg/render-to-cube2.cpp` | Modify | Add SSC to SharedCtx; drive camera via `ssc->set0.camera`; consume `set0Resources` and `set0Payload` |

---

## Task 1: UBO struct headers

**Files:**
- Create: `src/core/rdg/2/vk-shaders/scene-ubo.h`
- Create: `src/core/rdg/2/vk-shaders/camera-ubo.h`

- [ ] **Create `src/core/rdg/2/vk-shaders/scene-ubo.h`**

```cpp
// Shared between C++17 and GLSL 4.5+.
//
// C++ usage: #include "vk-shaders/scene-ubo.h"
// GLSL usage: declare UBO inline (see cube-draw.vert for the pattern).
//
// std140 layout (all offsets in bytes):
//   frameCounter             uint  offset   0
//   frameDurationMs          float offset   4
//   environmentRadianceScale float offset   8
//   numLights                uint  offset  12
//   lights[]                 DirectLightData[16]  offset 16  (16 × 48 = 768 bytes)
//   Total: 784 bytes
#pragma once

// MAX_SCENE_LIGHTS must match the array size declared in any GLSL shader that
// includes this UBO layout.
#define MAX_SCENE_LIGHTS 16

// Light type tags stored in DirectLightData::positionOrDir.w
#define SCENE_LIGHT_TYPE_POINT       0
#define SCENE_LIGHT_TYPE_SPOT        1
#define SCENE_LIGHT_TYPE_DIRECTIONAL 2

#ifdef __cplusplus
    #include <cstdint>
    #include <glm/vec4.hpp>
    #define vec4 glm::vec4
    #define uint uint32_t
    #define float float
#endif

// One direct light entry (48 bytes, std140-compatible).
struct DirectLightData {
    vec4 positionOrDir; // xyz = world-space pos (point/spot) or direction (directional), w = type
    vec4 colorAndRange; // xyz = linear RGB × luminous intensity, w = range (0 for directional)
    vec4 coneAngles;    // x = cos(innerConeAngle), y = cos(outerConeAngle), zw = unused
};

struct SceneUBO {
    uint  frameCounter;
    float frameDurationMs;
    float environmentRadianceScale;
    uint  numLights;
    DirectLightData lights[MAX_SCENE_LIGHTS];
};

#ifdef __cplusplus
    #undef vec4
    #undef uint
    #undef float

    #include <cstddef>
    static_assert(sizeof(DirectLightData) == 48, "DirectLightData must be 48 bytes");
    static_assert(sizeof(SceneUBO) == 784,       "SceneUBO must be 784 bytes");
    static_assert(offsetof(SceneUBO, frameCounter)             ==   0);
    static_assert(offsetof(SceneUBO, frameDurationMs)          ==   4);
    static_assert(offsetof(SceneUBO, environmentRadianceScale) ==   8);
    static_assert(offsetof(SceneUBO, numLights)                ==  12);
    static_assert(offsetof(SceneUBO, lights)                   ==  16);
#endif
```

- [ ] **Create `src/core/rdg/2/vk-shaders/camera-ubo.h`**

```cpp
// Shared between C++17 and GLSL 4.5+.
//
// C++ usage: #include "vk-shaders/camera-ubo.h"
// GLSL usage: declare UBO inline (see cube-draw.vert for the pattern).
//
// std140 layout (all offsets in bytes):
//   viewMatrix       mat4  offset   0  (64 bytes)
//   projMatrix       mat4  offset  64  (64 bytes)
//   viewProjMatrix   mat4  offset 128  (64 bytes)
//   cameraPosition   vec4  offset 192  (16 bytes)  xyz = world pos, w = 1
//   renderTargetSize vec2  offset 208  ( 8 bytes)  width, height in pixels
//   nearPlane        float offset 216  ( 4 bytes)
//   farPlane         float offset 220  ( 4 bytes)
//   Total: 224 bytes
#pragma once

#ifdef __cplusplus
    #include <glm/mat4x4.hpp>
    #include <glm/vec2.hpp>
    #include <glm/vec4.hpp>
    #define mat4 glm::mat4
    #define vec4 glm::vec4
    #define vec2 glm::vec2
#endif

struct CameraUBO {
    mat4  viewMatrix;       // world → view
    mat4  projMatrix;       // view → clip (Vulkan: Y-flipped, [0,1] depth)
    mat4  viewProjMatrix;   // projMatrix * viewMatrix
    vec4  cameraPosition;   // xyz = camera world-space position, w = 1
    vec2  renderTargetSize; // width, height in pixels
    float nearPlane;
    float farPlane;
};

#ifdef __cplusplus
    #undef mat4
    #undef vec4
    #undef vec2

    #include <cstddef>
    static_assert(sizeof(CameraUBO) == 224,                            "CameraUBO must be 224 bytes");
    static_assert(offsetof(CameraUBO, viewMatrix)       ==   0);
    static_assert(offsetof(CameraUBO, projMatrix)       ==  64);
    static_assert(offsetof(CameraUBO, viewProjMatrix)   == 128);
    static_assert(offsetof(CameraUBO, cameraPosition)   == 192);
    static_assert(offsetof(CameraUBO, renderTargetSize) == 208);
    static_assert(offsetof(CameraUBO, nearPlane)        == 216);
    static_assert(offsetof(CameraUBO, farPlane)         == 220);
#endif
```

- [ ] **Verify static_asserts compile cleanly**

```bash
python3 env/bin/build.py d 2>&1 | grep -E "scene-ubo|camera-ubo|static_assert|error"
```

Expected: no errors or static_assert failures.

- [ ] **Commit**

```bash
git add src/core/rdg/2/vk-shaders/
git commit -m "feat/rdg2: add SceneUBO and CameraUBO v2 shader constant headers"
```

---

## Task 2: Update `shared-shader-constants2.h`

**Files:**
- Modify: `src/inc/garnet/rdg/2/shared-shader-constants2.h`

- [ ] **Replace the `EnvironmentLightingConstants` alias with a native struct**

Find the line:
```cpp
using EnvironmentLightingConstants = GN::rdg::SharedShaderConstants::EnvironmentLightingInformation;
```

Replace it with:
```cpp
/// Environment lighting textures and uniform scale. Fields default to fallback
/// 1×1 textures created at SSC construction; assign real textures to replace them.
struct EnvironmentLightingConstants {
    AutoRef<GN::gpu2::Texture> skyboxCubemap;              ///< env cubemap rendered as background
    AutoRef<GN::gpu2::Texture> irradianceMap;              ///< pre-convolved diffuse-IBL cubemap
    AutoRef<GN::gpu2::Texture> prefilteredEnvMap;          ///< mip-mapped specular-IBL cubemap
    AutoRef<GN::gpu2::Texture> brdfLut;                    ///< split-sum BRDF LUT
    float                      environmentRadianceScale = 1.f;
};
```

- [ ] **Build to verify the header change compiles**

```bash
python3 env/bin/build.py d 2>&1 | grep -E "error:|shared-shader-constants2"
```

Expected: build succeeds (or the only errors are in the stub `.cpp` which uses the old alias — fix those in Task 4).

- [ ] **Commit**

```bash
git add src/inc/garnet/rdg/2/shared-shader-constants2.h
git commit -m "feat/rdg2: replace EnvironmentLightingConstants alias with native gpu2::Texture struct"
```

---

## Task 3: Write failing SSC unit tests

**Files:**
- Create: `src/core/rdg/2/test/shared-shader-constants2-test.cpp`

`GNtest-internal` auto-discovers any `*.cpp` under a `test/` directory — no CMakeLists change needed.

- [ ] **Create the test file**

```cpp
// Tests for GN::rdg2::SharedShaderConstants.
// Requires a Vulkan GPU context; each test skips gracefully if unavailable.

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg2.h>

using namespace GN;
using namespace GN::rdg2;
using namespace GN::gpu2;

static const GpuContext::Verbosity kGpuVerbosity = GpuContext::Verbosity::SILENCE;

static AutoRef<GpuContext> makeGpu(const char * name) {
    return GpuContext::create(name, GpuContext::CreateParameters {.howToPrintDeviceCaps = kGpuVerbosity});
}

TEST_CASE("rdg2::SharedShaderConstants: create() returns non-null with valid params", "[rdg2][ssc][gpu]") {
    auto gpu   = makeGpu("ssc_create_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);
}

TEST_CASE("rdg2::SharedShaderConstants: fallback env textures are non-null after create", "[rdg2][ssc][gpu]") {
    auto gpu   = makeGpu("ssc_fallback_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);
    CHECK(ssc->set0.environmentLighting.skyboxCubemap);
    CHECK(ssc->set0.environmentLighting.irradianceMap);
    CHECK(ssc->set0.environmentLighting.prefilteredEnvMap);
    CHECK(ssc->set0.environmentLighting.brdfLut);
}

TEST_CASE("rdg2::SharedShaderConstants: takeSnapshot() returns non-null token", "[rdg2][ssc][gpu]") {
    auto gpu   = makeGpu("ssc_snap_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    TokenPtr token = ssc->takeSnapshot();
    REQUIRE(token != nullptr);
}

TEST_CASE("rdg2::SharedShaderConstants: getContent() returns valid content after snapshot", "[rdg2][ssc][gpu]") {
    auto gpu   = makeGpu("ssc_content_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    TokenPtr token = ssc->takeSnapshot();
    REQUIRE(token);

    // Block until the upload node completes.
    graph->waitForToken(token);

    auto content = ssc->getContent(token);
    REQUIRE(content);
    // set0Resources must have 6 bindings: scene UBO, camera UBO, 4 env textures.
    CHECK(content->set0Resources.size() == 6u);
    // Payload must be non-null — it carries the GPU copy command.
    CHECK(content->set0Payload);
}

TEST_CASE("rdg2::SharedShaderConstants: two snapshots evict stale map entries", "[rdg2][ssc][gpu]") {
    auto gpu   = makeGpu("ssc_evict_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    // Frame 1
    TokenPtr t1 = ssc->takeSnapshot();
    REQUIRE(t1);
    graph->waitForToken(t1);
    auto c1 = ssc->getContent(t1);
    REQUIRE(c1);

    // Frame 2 — takeSnapshot() should evict the satisfied token from frame 1.
    ssc->set0.frameConstants.frameCounter = 2;
    TokenPtr t2 = ssc->takeSnapshot();
    REQUIRE(t2);
    graph->waitForToken(t2);

    // t1's entry was evicted; getContent must return empty for it.
    auto c1again = ssc->getContent(t1);
    CHECK(!c1again);

    // t2's content is still accessible.
    auto c2 = ssc->getContent(t2);
    CHECK(c2);
}
```

- [ ] **Attempt to build — expect compile error or test failures (stub is incomplete)**

```bash
python3 env/bin/build.py d 2>&1 | grep -E "error:|FAILED|ssc"
```

Expected: compile errors in the stub (undefined `name`, wrong constructor) OR tests fail at runtime. This confirms the tests are exercising real behaviour.

- [ ] **Commit failing tests**

```bash
git add src/core/rdg/2/test/shared-shader-constants2-test.cpp
git commit -m "test/rdg2: add failing unit tests for SharedShaderConstants"
```

---

## Task 4: Implement `SharedShaderConstants2Impl`

**Files:**
- Modify: `src/core/rdg/2/shared-shader-constants2.cpp`

Replace the entire file with the following implementation.

- [ ] **Write the implementation**

```cpp
#include <garnet/GNrdg2.h>
#include "vk-shaders/scene-ubo.h"
#include "vk-shaders/camera-ubo.h"

#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::inverse
#include <glm/gtc/quaternion.hpp>        // glm::mat4_cast
#include <glm/ext/matrix_clip_space.hpp> // glm::perspectiveRH_ZO

#include <mutex>
#include <unordered_map>
#include <algorithm>

static GN::Logger * sLogger = GN::getLogger("GN.rdg2");

namespace GN::rdg2 {

// ---------------------------------------------------------------------------
// Helper: deduce render-target pixel dimensions from the first valid attachment.
// Falls back to 1×1 when no attachment is set (avoids a zero-size projection).
// ---------------------------------------------------------------------------
static std::pair<uint32_t, uint32_t> getRenderTargetSize(const gpu2::RasterTarget & rt) {
    for (const auto & c : rt.colorTargets) {
        if (auto tex = c.target.texture()) return {tex->descriptor().width, tex->descriptor().height};
    }
    if (auto tex = rt.depthStencilTarget.texture()) return {tex->descriptor().width, tex->descriptor().height};
    return {1u, 1u};
}

// ---------------------------------------------------------------------------
// Helper: create a 1×1 RGBA8 texture and upload a single pixel.
// setContent() is intentionally used here: this is one-time static init, not
// per-frame work (see gpu2.md rule on setContent() usage).
// ---------------------------------------------------------------------------
static AutoRef<gpu2::Texture> make1x1Texture(AutoRef<gpu2::GpuContext> gpu, const StrA & name,
                                              uint8_t r, uint8_t g, uint8_t b, uint8_t a,
                                              uint32_t faces) {
    gpu2::Texture::Descriptor desc;
    desc.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM())
        .setDimensions(1, 1)
        .setFaces(faces)
        .setLevels(1);
    auto tex = gpu2::Texture::create(name, {.context = gpu, .descriptor = desc});
    if (!tex) GN_UNLIKELY return {};

    // Build an Image whose layout matches the texture descriptor.
    gfx::img::ImageDesc imgDesc;
    if (faces == 6)
        imgDesc.setCube(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM(), 1, 1);
    else
        imgDesc.set2D(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM(), 1, 1, 1);

    gfx::img::Image img(imgDesc);
    for (size_t f = 0; f < faces; ++f) {
        uint8_t * p = img.at({.face = f}, 0, 0);
        p[0] = r; p[1] = g; p[2] = b; p[3] = a;
    }
    if (!tex->setContent(img)) GN_UNLIKELY {
        GN_ERROR(sLogger)("make1x1Texture: setContent failed for '{}'", name);
        return {};
    }
    return tex;
}

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

class SharedShaderConstants2Impl : public SharedShaderConstants {
    AutoRef<gpu2::GpuContext>  mGpu;
    AutoRef<Graph>             mGraph;
    AutoRef<gpu2::Buffer>      mSceneBuffer;   // device-local, 784 bytes, binding 0
    AutoRef<gpu2::Buffer>      mCameraBuffer;  // device-local, 224 bytes, binding 1
    AutoRef<gpu2::Texture>     mFallbackCubemap; // 1×1 black RGBA8, faces=6
    AutoRef<gpu2::Texture>     mFallbackBrdfLut; // 1×1 white RGBA8, faces=1

    mutable std::mutex mMutex;
    mutable std::unordered_map<TokenPtr, AutoRef<Content>> mContentMap;

public:
    GN_REGISTER_RUNTIME_TYPE(SharedShaderConstants);

    SharedShaderConstants2Impl(): SharedShaderConstants(TYPE_INFO(), "SharedShaderConstants2") {}

    bool init(const CreateParameters & params) {
        if (!params.gpu || !params.graph) {
            GN_ERROR(sLogger)("SharedShaderConstants2: invalid create parameters");
            return false;
        }
        mGpu   = params.gpu;
        mGraph = params.graph;

        mSceneBuffer = gpu2::Buffer::create("ssc2.scene_ubo",
            {.context = mGpu, .size = sizeof(SceneUBO)});
        mCameraBuffer = gpu2::Buffer::create("ssc2.camera_ubo",
            {.context = mGpu, .size = sizeof(CameraUBO)});
        if (!mSceneBuffer || !mCameraBuffer) GN_UNLIKELY {
            GN_ERROR(sLogger)("SharedShaderConstants2: failed to create UBO buffers");
            return false;
        }

        mFallbackCubemap = make1x1Texture(mGpu, "ssc2.fallback_cubemap", 0, 0, 0, 255, 6);
        mFallbackBrdfLut = make1x1Texture(mGpu, "ssc2.fallback_brdf_lut", 255, 255, 255, 255, 1);
        if (!mFallbackCubemap || !mFallbackBrdfLut) GN_UNLIKELY {
            GN_ERROR(sLogger)("SharedShaderConstants2: failed to create fallback textures");
            return false;
        }

        // Initialise env lighting fields with fallback textures so set0 is always valid.
        set0.environmentLighting.skyboxCubemap     = mFallbackCubemap;
        set0.environmentLighting.irradianceMap     = mFallbackCubemap;
        set0.environmentLighting.prefilteredEnvMap = mFallbackCubemap;
        set0.environmentLighting.brdfLut           = mFallbackBrdfLut;

        return true;
    }

    TokenPtr takeSnapshot() const override {
        // Evict stale entries whose upload tokens are already satisfied.
        // By the time a new snapshot is requested, all prior render nodes have
        // completed and called getContent(), so their entries are safe to drop.
        {
            std::lock_guard<std::mutex> lock(mMutex);
            for (auto it = mContentMap.begin(); it != mContentMap.end(); ) {
                if (mGraph->waitForToken(it->first) == Graph::WaitResult::IDLE)
                    it = mContentMap.erase(it);
                else
                    ++it;
            }
        }

        // Snapshot current CPU state under the lock so concurrent set0 writes
        // do not race with the copy. (Single-threaded callers can skip the lock,
        // but we guard anyway for correctness.)
        Set0Parameters snap;
        {
            std::lock_guard<std::mutex> lock(mMutex);
            snap = set0;
        }

        // Pre-build set0Resources using the device-local buffer views and the
        // texture refs from the snapshot. This is purely CPU work and does not
        // depend on the staging upload completing.
        auto content = AutoRef<Content>(new Content());
        content->set0Parameters = snap;
        buildSet0Resources(snap, content->set0Resources);

        // Add graph node that packs UBO data and issues the GPU copy.
        NodePtr node = mGraph->addNode(
            NodeDesc("ssc2 upload")
                .setAction(Action::createFromLambda("pack + upload scene/camera UBOs",
                    [this, snap, content]() {
                        uploadSnapshot(snap, content);
                    }), nullptr));

        TokenPtr token = mGraph->getNodeCompletionToken(node);
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mContentMap[token] = content;
        }
        return token;
    }

    AutoRef<Content> getContent(TokenPtr token) const override {
        std::lock_guard<std::mutex> lock(mMutex);
        auto it = mContentMap.find(token);
        if (it == mContentMap.end()) return {};
        return it->second;
    }

private:
    // Build the set0Resources array from device-local buffers and env texture refs.
    void buildSet0Resources(const Set0Parameters & snap, gpu2::GpuResourceSet & out) const {
        out.resize(6);
        // Binding 0: SceneUBO
        out[0].resize(1);
        out[0][0].resource = mSceneBuffer;
        out[0][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM)
                 .setBufferViewOffset(0)
                 .setBufferViewSize(sizeof(SceneUBO));
        // Binding 1: CameraUBO
        out[1].resize(1);
        out[1][0].resource = mCameraBuffer;
        out[1][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM)
                 .setBufferViewOffset(0)
                 .setBufferViewSize(sizeof(CameraUBO));
        // Bindings 2-5: env textures (fall back to 1×1 defaults if the field is null)
        auto bindTex = [&](uint32_t slot, const AutoRef<gpu2::Texture> & tex,
                           const AutoRef<gpu2::Texture> & fallback) {
            out[slot].resize(1);
            out[slot][0].resource = tex ? tex : fallback;
            out[slot][0].setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        };
        const auto & env = snap.environmentLighting;
        bindTex(2, env.skyboxCubemap,     mFallbackCubemap);
        bindTex(3, env.irradianceMap,     mFallbackCubemap);
        bindTex(4, env.prefilteredEnvMap, mFallbackCubemap);
        bindTex(5, env.brdfLut,           mFallbackBrdfLut);
    }

    // Pack UBO structs from snapshot, write to mappable staging buffers,
    // record GPU copies into device-local buffers, seal → set0Payload.
    // The GpuCnC payload internally holds AutoRef<Buffer> to the staging
    // buffers (see StoredBufferToBuffer in vk-gpu-cnc.cpp), keeping them
    // alive until the GPU copy completes and the payload is released.
    void uploadSnapshot(const Set0Parameters & snap, const AutoRef<Content> & content) const {
        // --- Pack SceneUBO ---
        SceneUBO scene {};
        scene.frameCounter             = snap.frameConstants.frameCounter;
        scene.frameDurationMs          = (float)((double)snap.frameConstants.frameDuration.count() * 1e-3); // µs → ms
        scene.environmentRadianceScale = snap.environmentLighting.environmentRadianceScale;

        const size_t numLights = std::min((size_t)MAX_SCENE_LIGHTS, snap.directLighting.size());
        scene.numLights = (uint32_t)numLights;
        for (uint32_t i = 0; i < numLights; ++i) {
            const DirectLight & src = snap.directLighting[i];
            DirectLightData &   dst = scene.lights[i];
            switch (src.type) {
            case DirectLight::POINT: {
                dst.positionOrDir = glm::vec4(glm::vec3(src.point.position), (float)SCENE_LIGHT_TYPE_POINT);
                float iv          = src.point.intensity.intensity.value;
                dst.colorAndRange = glm::vec4(src.point.intensity.r * iv,
                                              src.point.intensity.g * iv,
                                              src.point.intensity.b * iv,
                                              src.point.range);
                dst.coneAngles    = glm::vec4(0.f);
                break;
            }
            case DirectLight::SPOT: {
                dst.positionOrDir = glm::vec4(glm::vec3(src.spot.position), (float)SCENE_LIGHT_TYPE_SPOT);
                float iv          = src.spot.intensity.intensity.value;
                dst.colorAndRange = glm::vec4(src.spot.intensity.r * iv,
                                              src.spot.intensity.g * iv,
                                              src.spot.intensity.b * iv,
                                              src.spot.range);
                dst.coneAngles    = glm::vec4(src.spot.cosInnerConeAngle, src.spot.cosOuterConeAngle, 0.f, 0.f);
                break;
            }
            case DirectLight::DIRECTIONAL: {
                glm::vec3 dir     = glm::mat3_cast(src.directional.orientation) * glm::vec3(0.f, 0.f, -1.f);
                dst.positionOrDir = glm::vec4(dir, (float)SCENE_LIGHT_TYPE_DIRECTIONAL);
                float iv          = src.directional.irradiance.irradiance.value;
                dst.colorAndRange = glm::vec4(src.directional.irradiance.r * iv,
                                              src.directional.irradiance.g * iv,
                                              src.directional.irradiance.b * iv,
                                              0.f);
                dst.coneAngles    = glm::vec4(0.f);
                break;
            }
            }
        }

        // --- Pack CameraUBO ---
        CameraUBO cam {};
        {
            const glm::vec3 pos      = snap.camera.cameraPosition;
            glm::mat4 camToWorld = glm::translate(glm::mat4(1.f), pos)
                                 * glm::mat4_cast(snap.camera.cameraOrientation);
            cam.viewMatrix     = glm::inverse(camToWorld);
            cam.projMatrix     = glm::perspectiveRH_ZO(snap.camera.cameraFov.value,
                                                       snap.camera.aspectRatio,
                                                       snap.camera.nearPlane,
                                                       snap.camera.farPlane);
            cam.projMatrix[1][1] *= -1.f; // Vulkan Y-flip
            cam.viewProjMatrix = cam.projMatrix * cam.viewMatrix;
            cam.cameraPosition = glm::vec4(pos, 1.f);
            auto [w, h]        = getRenderTargetSize(snap.renderTarget);
            cam.renderTargetSize = glm::vec2((float)w, (float)h);
            cam.nearPlane      = snap.camera.nearPlane;
            cam.farPlane       = snap.camera.farPlane;
        }

        // --- Create staging buffers and write packed data ---
        auto stagingScene = gpu2::Buffer::create("ssc2.staging_scene",
            {.context = mGpu, .size = sizeof(SceneUBO), .mappable = true});
        auto stagingCam   = gpu2::Buffer::create("ssc2.staging_cam",
            {.context = mGpu, .size = sizeof(CameraUBO), .mappable = true});
        if (!stagingScene || !stagingCam) GN_UNLIKELY {
            GN_ERROR(sLogger)("SharedShaderConstants2: failed to create staging buffers");
            return;
        }
        {
            auto m = stagingScene->map();
            if (!m.data()) GN_UNLIKELY { GN_ERROR(sLogger)("ssc2: scene staging map failed"); return; }
            memcpy(m.data(), &scene, sizeof(scene));
        }
        {
            auto m = stagingCam->map();
            if (!m.data()) GN_UNLIKELY { GN_ERROR(sLogger)("ssc2: camera staging map failed"); return; }
            memcpy(m.data(), &cam, sizeof(cam));
        }

        // --- Record GPU copies and seal ---
        auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
        if (!cnc) GN_UNLIKELY { GN_ERROR(sLogger)("ssc2: GpuCnC create failed"); return; }
        cnc->copyBufferToBuffer({.src = stagingScene, .dst = mSceneBuffer,  .size = sizeof(SceneUBO)});
        cnc->copyBufferToBuffer({.src = stagingCam,   .dst = mCameraBuffer, .size = sizeof(CameraUBO)});
        content->set0Payload = cnc->seal();
        // GpuCncPayloadVulkan stores AutoRef<Buffer> for each src/dst in its op list,
        // so stagingScene and stagingCam remain alive until set0Payload is released.
    }
};

// ---------------------------------------------------------------------------
// Factory
// ---------------------------------------------------------------------------

GN_API AutoRef<SharedShaderConstants> SharedShaderConstants::create(const CreateParameters & params) {
    auto impl = AutoRef<SharedShaderConstants2Impl>(new SharedShaderConstants2Impl());
    if (!impl->init(params)) {
        GN_ERROR(sLogger)("SharedShaderConstants::create failed");
        return {};
    }
    return impl;
}

} // namespace GN::rdg2
```

- [ ] **Build**

```bash
python3 env/bin/build.py d 2>&1 | tail -20
```

Expected: build succeeds with no errors.

- [ ] **Run SSC unit tests**

```bash
./build/debug/bin/GNtest-internal "[rdg2][ssc]" -v
```

Expected: all 5 SSC tests PASS (or skip if no Vulkan GPU available).

- [ ] **Commit**

```bash
git add src/core/rdg/2/shared-shader-constants2.cpp
git commit -m "feat/rdg2: implement SharedShaderConstants with async GpuCnC UBO upload"
```

---

## Task 5: Update `cube-draw.vert`

**Files:**
- Modify: `src/sample/rdg/cube-draw.vert`

The shader currently receives a full MVP matrix as a push constant and computes `gl_Position = mvp * pos`. After this change it reads view+proj from the camera UBO (set=0, binding=1) and keeps only the model matrix as a push constant.

- [ ] **Replace `cube-draw.vert` with**

```glsl
#version 450

// Camera UBO — set 0, binding 1.
// Keep field order and types in sync with src/core/rdg/2/vk-shaders/camera-ubo.h.
layout(std140, set = 0, binding = 1) uniform CameraBlock {
    mat4  viewMatrix;
    mat4  projMatrix;
    mat4  viewProjMatrix;
    vec4  cameraPosition;
    vec2  renderTargetSize;
    float nearPlane;
    float farPlane;
} u_camera;

// Model matrix only — view and proj come from the UBO above.
layout(push_constant) uniform PC { mat4 model; } pc;

layout(location = 0) in  vec3 inPosition;
layout(location = 1) in  float inFaceIndex; // unused; kept so the VBO layout does not change

layout(location = 0) out vec3 outDirection; // object-space position → cubemap sampling direction

void main() {
    gl_Position  = u_camera.projMatrix * u_camera.viewMatrix * pc.model * vec4(inPosition, 1.0);
    outDirection = inPosition; // cube centered at origin; vertex position = sample direction
}
```

- [ ] **Commit (shaders rebuild on next build)**

```bash
git add src/sample/rdg/cube-draw.vert
git commit -m "feat/rdg2: cube-draw.vert reads view/proj from camera UBO; model stays push constant"
```

---

## Task 6: Update `cube-draw.frag`

**Files:**
- Modify: `src/sample/rdg/cube-draw.frag`

The cubemap sampler moves from set=0,binding=0 to set=1,binding=0 to make room for SSC's set 0.

- [ ] **Replace `cube-draw.frag` with**

```glsl
#version 450

// Cubemap is now set=1, binding=0 (set 0 is reserved for SSC resources).
layout(set = 1, binding = 0) uniform samplerCube uCubemap;

layout(location = 0) in  vec3 inDirection;
layout(location = 0) out vec4 outColor;

void main() { outColor = texture(uCubemap, inDirection); }
```

- [ ] **Commit**

```bash
git add src/sample/rdg/cube-draw.frag
git commit -m "feat/rdg2: cube-draw.frag moves samplerCube to set=1,binding=0 for SSC set-0 layout"
```

---

## Task 7: Integrate SSC into `render-to-cube2.cpp`

**Files:**
- Modify: `src/sample/rdg/render-to-cube2.cpp`

Changes in this task:
1. Add `ssc` to `SharedCtx`
2. Create SSC in `main()` after graph creation
3. Set camera per-frame via `ssc->set0.camera`
4. `CubeDraw::addFrameNode()` takes SSC + SSC token; consumes `set0Resources`; push-constant becomes model-only
5. GPU submit chains `sscContent->set0Payload`

- [ ] **Add `ssc` to `SharedCtx`**

Find:
```cpp
struct SharedCtx {
    AutoRef<Graph>      graph;
    AutoRef<GpuContext> gpu;
    uint32_t            width, height;
};
```

Replace with:
```cpp
struct SharedCtx {
    AutoRef<Graph>                         graph;
    AutoRef<GpuContext>                    gpu;
    AutoRef<rdg2::SharedShaderConstants>   ssc;
    uint32_t                               width, height;
};
```

- [ ] **Update `CubeDraw::addFrameNode()` signature and body**

Find the existing `addFrameNode` declaration:
```cpp
NodePtr addFrameNode(const TokenPtr & cubemapToken, const AutoRef<Texture> & cubemap, const GpuResourceView & swapView, float elapsed,
                     AutoRef<GpuPayload> & outPayload) {
```

Replace the whole method with:
```cpp
NodePtr addFrameNode(const TokenPtr & cubemapToken, const TokenPtr & sscToken,
                     const AutoRef<Texture> & cubemap, const GpuResourceView & swapView,
                     float elapsed, AutoRef<GpuPayload> & outPayload) {
    return mCtx.graph->addNode(
        NodeDesc("draw cube")
            .setAction(Action::createFromLambda(
                           "render rotating cube",
                           [this, cubemap, swapView, elapsed, sscToken, &outPayload]() {
                               auto vs = mCtx.graph->getTypedArtifactContent<AutoRef<GpuShader>>(mVsArtifact);
                               auto ps = mCtx.graph->getTypedArtifactContent<AutoRef<GpuShader>>(mPsArtifact);
                               if (!vs || !ps) {
                                   GN_ERROR(sLogger)("CubeDraw: missing shaders");
                                   return;
                               }

                               auto sscContent = mCtx.ssc->getContent(sscToken);
                               if (!sscContent) {
                                   GN_ERROR(sLogger)("CubeDraw: SSC content unavailable");
                                   return;
                               }

                               const glm::mat4 model = glm::rotate(glm::mat4(1.f), elapsed * 0.5f,
                                                           glm::normalize(glm::vec3(0.5f, 1.0f, 0.3f)));

                               GpuResourceView depthView;
                               depthView.resource = mDepthTex;

                               GpuRaster::CreateParameters rcp;
                               rcp.gpu = mCtx.gpu;
                               rcp.target.colorTargets.append(RasterTarget::ColorTarget {.target = swapView});
                               rcp.target.setDepthStencilTarget(depthView);
                               rcp.target.setClearColor(0.05f, 0.05f, 0.1f, 1.0f);
                               rcp.target.setClearDepth(1.0f);
                               rcp.target.states.setCullMode(RasterState::CULL_NONE);
                               rcp.target.states.setDepthState(RasterState::DepthState {
                                   .func = RasterState::Compare::LESS, .write = true});

                               RasterGeometry geom;
                               geom.format.attributes.append(
                                   RasterGeometry::VertexAttribute {.location = 0, .offset = 0,  .format = RasterGeometry::AttributeFormat::F32_3});
                               geom.format.attributes.append(
                                   RasterGeometry::VertexAttribute {.location = 1, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_1});
                               geom.vertices.append(RasterGeometry::GeometryBuffer {.buffer = mVb, .offset = 0, .stride = kVertexStride});
                               geom.indices    = RasterGeometry::GeometryBuffer {.buffer = mIb, .offset = 0, .stride = sizeof(uint16_t)};
                               geom.indexCount = kIndexCount;

                               // set=0 → SSC resources (camera + scene UBOs + env textures)
                               // set=1, binding=0 → assembled cubemap
                               GpuResourceView cubeView;
                               cubeView.resource = cubemap;
                               GpuResourceSet cubeSet;
                               cubeSet.resize(1);
                               cubeSet[0].resize(1);
                               cubeSet[0][0] = cubeView;

                               GpuResourceTable resources;
                               resources.append(sscContent->set0Resources); // set 0
                               resources.append(cubeSet);                   // set 1

                               GpuRaster::DrawParameters dp;
                               dp.vs        = vs;
                               dp.ps        = ps;
                               dp.geometry  = geom;
                               dp.resources = resources;
                               // Push constant: model matrix only (view+proj come from camera UBO)
                               dp.immediates = gpu2::ArrayProxy<uint8_t>(
                                   reinterpret_cast<uint8_t *>(&const_cast<glm::mat4 &>(model)), sizeof(model));

                               auto rast = GpuRaster::create(rcp);
                               rast->draw(dp);
                               outPayload = rast->seal();
                           }),
                       nullptr)
            .dependsOn(cubemapToken)
            .dependsOn(sscToken)
            .dependsOn(mVsReady)
            .dependsOn(mPsReady));
}
```

- [ ] **Create SSC in `main()` after graph creation**

Find in `main()`:
```cpp
SharedCtx ctx {.graph = graph, .gpu = gpuContext, .width = windowWidth, .height = windowHeight};
```

Replace with:
```cpp
auto ssc = rdg2::SharedShaderConstants::create({.gpu = gpuContext, .graph = graph});
if (!ssc) return -1;
SharedCtx ctx {.graph = graph, .gpu = gpuContext, .ssc = ssc, .width = windowWidth, .height = windowHeight};
```

- [ ] **Drive camera via SSC and snapshot each frame**

Find in the render loop (inside `while`), before the cubemap renderer call:
```cpp
const float elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - startTime).count();

std::vector<AutoRef<GpuPayload>> cubemapPayloads;
```

Insert after `elapsed` and before `cubemapPayloads`:
```cpp
// Camera: fixed position looking at the cube from +Z.
// Extract orientation quaternion from the lookAt view matrix so SSC's
// UBO packing (translate × mat4_cast(orientation) → view) is consistent.
{
    static const glm::vec3 kEye(0.f, 0.f, 3.f), kTarget(0.f), kUp(0.f, 1.f, 0.f);
    glm::mat4 camToWorld = glm::inverse(glm::lookAtRH(kEye, kTarget, kUp));
    ssc->set0.camera.cameraPosition    = kEye;
    ssc->set0.camera.cameraOrientation = glm::quat_cast(glm::mat3(camToWorld));
    ssc->set0.camera.cameraFov         = Degree(45.f);
    ssc->set0.camera.aspectRatio       = (float)windowWidth / (float)windowHeight;
    ssc->set0.camera.nearPlane         = 0.1f;
    ssc->set0.camera.farPlane          = 100.f;
    ssc->set0.frameConstants.frameCounter = (uint32_t)frameCounter;
    ssc->set0.renderTarget.setColorTarget(0, frame.view);
}
TokenPtr sscToken = ssc->takeSnapshot();
```

- [ ] **Update `CubeDraw::addFrameNode()` call site** to pass `sscToken`

Find:
```cpp
NodePtr presentNode = cubeDraw.addFrameNode(cubemapThisFrame, cubemapRenderer.cubemap(), frame.view, elapsed, presentPayload);
```

Replace with:
```cpp
NodePtr presentNode = cubeDraw.addFrameNode(cubemapThisFrame, sscToken,
                                            cubemapRenderer.cubemap(), frame.view, elapsed, presentPayload);
```

- [ ] **Chain `set0Payload` in the GPU submit**

Find:
```cpp
GpuContext::SubmitParameters sp(StrA::format("frame {}", frameCounter));
if (cubemapRenderer.prevPayload) sp.waitFor(cubemapRenderer.prevPayload);
sp.waitFor(frame.ready);
sp.appendWorks(cubemapPayloads);
sp.appendWork(presentPayload);
gpuContext->submit(sp);
```

Replace with:
```cpp
// Retrieve SSC content to chain the UBO upload as a GPU-side dependency.
auto sscContent = ssc->getContent(sscToken);
GpuContext::SubmitParameters sp(StrA::format("frame {}", frameCounter));
if (cubemapRenderer.prevPayload) sp.waitFor(cubemapRenderer.prevPayload);
sp.waitFor(frame.ready);
if (sscContent && sscContent->set0Payload) sp.waitFor(sscContent->set0Payload);
sp.appendWorks(cubemapPayloads);
sp.appendWork(presentPayload);
gpuContext->submit(sp);
```

- [ ] **Also add `ssc->set0.renderTarget` default colour slot before the loop**

The `RasterTarget` inside `ssc->set0.renderTarget` needs at least one `ColorTarget` slot so `setColorTarget(0, ...)` doesn't index out of bounds. Find in `main()`, after SSC creation and before the render loop:

```cpp
// Pre-allocate colour slot 0 so per-frame setColorTarget(0, ...) is an update not an append.
ssc->set0.renderTarget.colorTargets.append(RasterTarget::ColorTarget {});
```

- [ ] **Commit**

```bash
git add src/sample/rdg/render-to-cube2.cpp
git commit -m "feat/rdg2: integrate SharedShaderConstants into render-to-cube2; camera driven via ssc->set0"
```

---

## Task 8: Build and smoke-test

- [ ] **Full debug build (recompiles shaders + C++)**

```bash
python3 env/bin/build.py d 2>&1 | tail -30
```

Expected: zero errors. Shader recompilation runs automatically via the CMake custom commands for `cube-draw.vert` and `cube-draw.frag`.

- [ ] **Run SSC unit tests**

```bash
./build/debug/bin/GNtest-internal "[rdg2][ssc]" -v
```

Expected: all 5 tests PASS (or SKIP if no Vulkan device).

- [ ] **Smoke-test the render-to-cube2 sample in headless mode**

```bash
./build/debug/bin/GNsample-rdg2-render-to-cube t 2>&1 | tail -20
```

Expected: 10 frames complete, cubemap readback logs a non-empty pixel. No Vulkan validation errors, no crashes.

- [ ] **Run full CI suite**

```bash
python3 env/bin/cit.py -t 2>&1 | tail -30
```

Expected: all tests pass.

- [ ] **Commit if any lint/format fixes were needed**

```bash
python3 env/bin/format-all-sources.py -d
git add -p
git commit -m "style: clang-format rdg2 SSC and render-to-cube2 changes"
```

# RDG2 Simple Effects Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement `GN::rdg2::PbrShading`, `GN::rdg2::SkyBox`, refactor `SharedShaderConstants` env textures to the artifact model, and create `simple-pbr2.cpp` demo. `setContent()`, `Texture::load()`, `waitForIdle()`, and any `gpu->submit()` outside the frame render loop are **banned from all production code**.

**Architecture:** Every async GPU work item — texture upload, buffer upload, UBO update — is encapsulated as a sealed `GpuPayload` embedded in a versioned artifact's content. Effects, helpers, asset loaders, and graph nodes **never call `gpu->submit()` or block waiting on GPU**. All GPU submission happens in exactly one place: the frame render loop. CPU never waits on GPU completion except at the frame boundary (swapchain image acquisition fence), maximizing CPU–GPU parallelism.

The render loop drives all GPU submission each frame:

1. **Frame prepare:** `ssc->takeSnapshot()` schedules a graph node that packs UBO data into a staging buffer and seals a **frame-prepare payload** (`set0Payload`). The draw node depends on the snapshot token (pure CPU-side ordering; no GPU wait).
2. **Artifact dependencies:** The draw node depends on the version tokens of every artifact it consumes. Tokens are satisfied when the artifact's current version is published — synchronously for v1 defaults, from a worker thread for v2 real assets. The graph node action is pure CPU work (command recording); waiting on it does not block the CPU on GPU.
3. **Payload collection:** After the draw node completes, collect all payloads in strict order:
   - **Frame-prepare payload first** (`set0Payload`) — UBO data written before any draw reads it.
   - **Artifact upload payloads next** — deduplicated by content-pointer equality so each version uploads exactly once across frames.
   - **Render work last** — Vulkan pipeline barriers inside the single command buffer enforce copy-before-draw ordering.
4. **Single submit + present:** `gpu->submit()` is called once with all payloads, then `swapchain->present()`. The CPU immediately starts the next frame's CPU work (update camera, take snapshot, record draws) while the GPU executes the submitted command buffer.

Default textures and geometry are CPU-staged (one `GpuCnC`, one `cnc->seal()`) and published as artifact v1 synchronously. File-loaded v2 is published directly from a worker thread — no `gpu->submit()` in the thread. The render loop works from frame 1 with defaults while v2 loads in the background.

**Tech Stack:** C++17, Vulkan (via rapid-vulkan), glm, rapid-image (`gfx::img::Image`), `GN::gpu2`, `GN::rdg2`

---

## File Map

| Action | Path |
|--------|------|
| Modify | `src/inc/garnet/gpu2/gpu2.h` — add `Buffer::StagedTexture` + `loadTextureToStagingBuffer()` |
| Modify | `src/inc/garnet/gpu2/cnc.h` — add `GpuCnC::copyBufferToImage(StagedTexture, Texture)` overload |
| New | `src/core/gpu2/gpu-texture-staging.cpp` — impl of `loadTextureToStagingBuffer` |
| Modify | `src/inc/garnet/rdg/2/shared-shader-constants2.h` — replace env lighting with artifact model |
| Modify | `src/core/rdg/2/shared-shader-constants2.cpp` — async env loading, staging-only defaults |
| New | `src/core/rdg/2/vk-shaders/skybox.vert` |
| New | `src/core/rdg/2/vk-shaders/skybox.frag` |
| New | `src/core/rdg/2/vk-shaders/pbr.vert` |
| New | `src/core/rdg/2/vk-shaders/pbr.frag` |
| New | `src/core/rdg/2/CMakeLists.txt` — compile 4 shaders to SPIR-V headers |
| Modify | `src/core/rdg/CMakeLists.txt` — add `add_subdirectory(2)` |
| Modify | `src/core/CMakeLists.txt` — depend on rdg2 shaders, set compile flag for simple-effect.cpp |
| New | `src/inc/garnet/rdg/2/simple-effect.h` |
| Modify | `src/inc/garnet/GNrdg2.h` — include simple-effect.h |
| New | `src/core/rdg/2/simple-effect.cpp` |
| New | `src/sample/rdg/simple-pbr2.cpp` |
| Modify | `src/sample/rdg/CMakeLists.txt` — new sample target |

---

## Task 0: Add `Buffer::loadTextureToStagingBuffer()` to gpu2

**Purpose:** Thread-safe utility that loads a texture file into a host-visible staging buffer via `map()`/`unmap()`. Returns the staging buffer, a `Texture::Descriptor`, and copy regions. No GPU commands issued. This is the recommended production way to load textures.

**Files:**
- Modify: `src/inc/garnet/gpu2/gpu2.h` (inside `struct Buffer`, after `readContent`)
- Modify: `src/inc/garnet/gpu2/cnc.h` (inside `struct GpuCnC`, convenience overload)
- Create: `src/core/gpu2/gpu-texture-staging.cpp`

- [ ] **Step 1: Add `StagedTexture` struct and method declaration to `Buffer` in `gpu2.h`**

Find the end of `struct Buffer` (just before `protected:`) and insert:

```cpp
    /// Result of loading a texture image file into a CPU-visible staging buffer.
    /// Pass to GpuCnC::copyBufferToImage() and Texture::create(); keep staging alive
    /// until the GPU copy payload completes.
    struct StagedTexture {
        struct Region {
            uint32_t          mip            = 0;
            uint32_t          face           = 0;
            Vector3<uint32_t> imageOffset    = {};
            Vector3<uint32_t> imageExtent    = {};
            uint64_t          bufferOffset   = 0;
            uint32_t          bufferRowLength = 0; ///< 0 = tight (same as imageExtent.x)
            uint32_t          bufferHeight   = 0;  ///< 0 = tight (same as imageExtent.y)
        };
        AutoRef<Buffer>        staging;    ///< host-visible; keep alive until GPU copy completes
        Texture::Descriptor    descriptor; ///< use with Texture::create() for the GPU-side texture
        ArrayContainer<Region> regions;    ///< one entry per face×mip
        bool empty() const { return !staging; }
    };

    /// Load a texture image file into a CPU-visible staging buffer. No GPU operations.
    /// Thread-safe; call from any worker thread without affecting the render loop.
    /// Uses rapid-image to decode; supports DDS, KTX, and common formats.
    /// @return StagedTexture with empty()==true on failure.
    static GN_API StagedTexture loadTextureToStagingBuffer(
        const StrA & name, AutoRef<GpuContext> context, const StrA & path);
```

- [ ] **Step 2: Add convenience overload to `GpuCnC` in `cnc.h`**

Inside `struct GpuCnC`, after the existing `copyBufferToImage(const BufferToImage &)` declaration, add:

```cpp
    /// Convenience: upload all planes from a StagedTexture into dst.
    /// Converts Buffer::StagedTexture::Region → GpuCnC::Region internally.
    void copyBufferToImage(const Buffer::StagedTexture & staged, AutoRef<Texture> dst) {
        ArrayContainer<Region> regions;
        for (const auto & r : staged.regions)
            regions.append(Region{
                .imageSubresource = {r.mip, r.face},
                .imageOffset      = r.imageOffset,
                .imageExtent      = r.imageExtent,
                .bufferOffset     = r.bufferOffset,
                .bufferRowLength  = r.bufferRowLength,
                .bufferHeight     = r.bufferHeight,
            });
        copyBufferToImage({.src = staged.staging, .dst = std::move(dst), .regions = regions});
    }
```

- [ ] **Step 3: Create `src/core/gpu2/gpu-texture-staging.cpp`**

```cpp
#include <garnet/GNgpu2.h>
#include <garnet/base/filesys.h>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2");

namespace GN::gpu2 {

Buffer::StagedTexture Buffer::loadTextureToStagingBuffer(
    const StrA & name, AutoRef<GpuContext> context, const StrA & path)
{
    if (!context) {
        GN_ERROR(sLogger)("loadTextureToStagingBuffer: null context, name='{}'", name);
        return {};
    }

    StrA absPath = path;
    if (!GN::fs::isAbsPath(absPath))
        absPath = GN::fs::resolvePath(GN::fs::getCurrentDir(), absPath);

    auto fp = GN::fs::openFile(absPath, std::ios::in | std::ios::binary);
    if (!fp) {
        GN_ERROR(sLogger)("loadTextureToStagingBuffer: cannot open '{}', name='{}'", absPath, name);
        return {};
    }

    gfx::img::Image image = gfx::img::Image::load(fp->input(), absPath.c_str());
    if (image.empty()) {
        GN_ERROR(sLogger)("loadTextureToStagingBuffer: failed to decode '{}', name='{}'", absPath, name);
        return {};
    }

    // Derive GPU texture descriptor from the decoded image.
    gfx::img::PlaneCoord p0 {};
    Texture::Descriptor desc;
    desc.format  = image.format(p0);
    desc.width   = image.width(p0);
    desc.height  = image.height(p0);
    desc.depth   = image.depth(p0);
    desc.faces   = (uint32_t) image.desc().faces;
    desc.levels  = image.desc().levels ? (uint32_t) image.desc().levels : 1u;
    desc.samples = 1;
    if (desc.width == 0 || desc.height == 0) {
        GN_ERROR(sLogger)("loadTextureToStagingBuffer: zero-size image '{}', name='{}'", absPath, name);
        return {};
    }

    // Copy the whole image (all faces × mips) into one host-visible staging buffer.
    // image.size() accounts for all planes including inter-plane alignment gaps.
    // image.offset(pc) gives each plane's byte offset within this layout.
    // rapid-image does not add row padding within a plane, so bufferRowLength=0 (tight) is correct.
    auto staging = Buffer::create(name + "_stg",
        {.context = context, .size = image.size(), .mappable = true});
    if (!staging) {
        GN_ERROR(sLogger)("loadTextureToStagingBuffer: staging buffer alloc failed, name='{}'", name);
        return {};
    }

    {
        auto m = staging->map();
        if (!m.data()) {
            GN_ERROR(sLogger)("loadTextureToStagingBuffer: map failed, name='{}'", name);
            return {};
        }
        memcpy(m.data(), image.data(), (size_t) image.size());
        // m unmaps on scope exit (RAII)
    }

    // Build one copy region per face × mip.
    ArrayContainer<StagedTexture::Region> regions;
    for (uint32_t f = 0; f < desc.faces; ++f) {
        for (uint32_t l = 0; l < desc.levels; ++l) {
            gfx::img::PlaneCoord pc {0, f, l};
            uint32_t w = image.width(pc);
            uint32_t h = image.height(pc);
            if (w == 0 || h == 0) continue;
            StagedTexture::Region r;
            r.mip          = l;
            r.face         = f;
            r.imageOffset  = {0, 0, 0};
            r.imageExtent  = {w, h, 1u};
            r.bufferOffset = (uint64_t) image.offset(pc);
            regions.append(r);
        }
    }

    return {std::move(staging), desc, std::move(regions)};
}

} // namespace GN::gpu2
```

- [ ] **Step 4: Build and verify**

```bash
python3 env/bin/build.py d 2>&1 | tail -20
```

Expected: compiles cleanly. `gpu-texture-staging.cpp` appears in build output.

---

## Task 1: Modify `SharedShaderConstants` header

Remove `EnvironmentLightingConstants` and `set0.environmentLighting`. Add `EnvTextureSet` artifact model.

**File:** `src/inc/garnet/rdg/2/shared-shader-constants2.h`

- [ ] **Step 1: Replace the header content**

Replace the entire file contents with:

```cpp
#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg/2/shared-shader-constants2.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

namespace GN::rdg2 {

struct SharedShaderConstants : public Entity {

    using FrameConstants = GN::rdg::SharedShaderConstants::FrameInformation;
    using DirectLight    = GN::rdg::SharedShaderConstants::DirectLight;

    struct CameraConstants {
        Location    cameraPosition    = {0, 0, 0};
        Orientation cameraOrientation = {1.f, 0.f, 0.f, 0.f};
        Radian      cameraFov         = Degree(60.f);
        float       aspectRatio       = 16.f / 9.f;
        float       nearPlane         = 0.01f;
        float       farPlane          = 10000.f;
    };

    /// Artifact payload for environment/IBL textures. Published immediately with
    /// procedural blue-sky defaults (v1); updated asynchronously from DDS files (v2+).
    /// The uploadPayload carries the staging→device copy work for this version.
    /// The render loop includes it in the frame submission exactly once (on version change).
    struct EnvTextureSet {
        AutoRef<GN::gpu2::Texture> skyboxCubemap;
        AutoRef<GN::gpu2::Texture> irradianceMap;
        AutoRef<GN::gpu2::Texture> prefilteredEnvMap;
        AutoRef<GN::gpu2::Texture> brdfLut;
        float environmentRadianceScale = 1.f;
        /// Staging→device copy for this version. Include in the frame's submit exactly once.
        AutoRef<GN::gpu2::GpuPayload> uploadPayload;
    };

    struct Set0Parameters {
        FrameConstants         frameConstants;
        GN::gpu2::RasterTarget renderTarget;
        CameraConstants        camera;
        DynaArray<DirectLight> directLighting;
    };

    struct Content : RefCounter {
        Set0Parameters            set0Parameters;
        EnvTextureSet             envTextures;      ///< snapshot of env artifact at takeSnapshot() time
        GN::gpu2::GpuResourceSet  set0Resources;
        /// Per-frame UBO upload (always new each snapshot). Submit every frame.
        AutoRef<GN::gpu2::GpuPayload> set0Payload;
        /// One-time fallback texture upload (set once at SSC init). Submit only on first frame.
        AutoRef<GN::gpu2::GpuPayload> fallbackInitPayload;
    };

    Set0Parameters set0;

    virtual TokenPtr takeSnapshot() const = 0;

    virtual AutoRef<const Content> getContent(TokenPtr) const = 0;

    struct EnvLoadParameters {
        StrA skyboxPath;
        StrA irradiancePath;
        StrA prefilteredPath;
        StrA brdfLutPath;
        float environmentRadianceScale = 1.f;
        bool  simulateSlowLoading = false;
    };
    struct EnvLoadResult {
        ArtifactPtr artifact; ///< EnvTextureSet; v1=blue-sky defaults, v2+=loaded
        TokenPtr    ready;    ///< satisfied when v1 is published (CPU staging done; upload pending)
    };
    /// Publish blue-sky defaults (with uploadPayload) immediately, then load real DDS files async.
    GN_API virtual EnvLoadResult loadEnvTextures(const EnvLoadParameters &) = 0;

    struct CreateParameters {
        AutoRef<GN::gpu2::GpuContext> gpu;
        AutoRef<Graph>                graph;
    };
    GN_API static AutoRef<SharedShaderConstants> create(const CreateParameters & params);

private:
    using Entity::Entity;
};

}; // namespace GN::rdg2
```

- [ ] **Step 2: Build to catch compilation errors from the API change**

```bash
python3 env/bin/build.py d 2>&1 | grep -E "error:|warning:" | head -30
```

Expected: errors in `shared-shader-constants2.cpp` (implementation not updated yet) and callers. Other files should be clean.

---

## Task 2: Update `SharedShaderConstants` implementation

**File:** `src/core/rdg/2/shared-shader-constants2.cpp`

Replace the entire file:

- [ ] **Step 1: Write new implementation**

```cpp
#include <garnet/GNrdg2.h>
#include "vk-shaders/scene-ubo.h"
#include "vk-shaders/camera-ubo.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <mutex>
#include <thread>
#include <unordered_map>
#include <algorithm>

static GN::Logger * sLogger = GN::getLogger("GN.rdg2");

namespace GN::rdg2 {

static std::pair<uint32_t, uint32_t> getRenderTargetSize(const gpu2::RasterTarget & rt) {
    for (const auto & c : rt.colorTargets) {
        if (auto tex = c.target.texture()) return {tex->descriptor().width, tex->descriptor().height};
    }
    if (auto tex = rt.depthStencilTarget.texture()) return {tex->descriptor().width, tex->descriptor().height};
    return {1u, 1u};
}

/// Create a 1×1 solid-color texture and record the staging→device copy into cnc.
/// Caller is responsible for sealing the CnC to get a combined payload.
/// The staging buffer is captured by the CnC and kept alive inside the sealed payload.
static AutoRef<gpu2::Texture> make1x1Texture(
    AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc,
    const StrA & name, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint32_t faces)
{
    gpu2::Texture::Descriptor desc;
    desc.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(faces).setLevels(1);
    auto tex = gpu2::Texture::create(name, {.context = gpu, .descriptor = desc});
    if (!tex) GN_UNLIKELY return {};

    const uint64_t stagingSize = 4ULL * faces;
    auto staging = gpu2::Buffer::create(name + "_stg",
        {.context = gpu, .size = stagingSize, .mappable = true});
    if (!staging) GN_UNLIKELY return {};

    {
        auto m = staging->map();
        if (!m.data()) GN_UNLIKELY return {};
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t f = 0; f < faces; ++f) {
            p[f*4+0]=r; p[f*4+1]=g; p[f*4+2]=b; p[f*4+3]=a;
        }
    }

    gpu2::ArrayContainer<gpu2::GpuCnC::Region> regions;
    for (uint32_t f = 0; f < faces; ++f) {
        gpu2::GpuCnC::Region reg;
        reg.imageSubresource = {0, f};
        reg.imageOffset      = {0, 0, 0};
        reg.imageExtent      = {1, 1, 1};
        reg.bufferOffset     = f * 4ULL;
        regions.append(reg);
    }
    cnc.copyBufferToImage({.src = staging, .dst = tex, .regions = regions});
    return tex;  // staging reference held by cnc until seal()
}

/// Load a texture file into a device-local texture, recording the copy into cnc.
/// Returns the texture (not yet GPU-readable until the caller submits cnc).
static AutoRef<gpu2::Texture> stageTextureLoad(
    const StrA & name, AutoRef<gpu2::GpuContext> gpu,
    const StrA & path, gpu2::GpuCnC & cnc)
{
    auto staged = gpu2::Buffer::loadTextureToStagingBuffer(name, gpu, path);
    if (staged.empty()) return {};
    auto tex = gpu2::Texture::create(name, {.context = gpu, .descriptor = staged.descriptor});
    if (!tex) return {};
    cnc.copyBufferToImage(staged, tex);
    return tex;
}

class SharedShaderConstants2Impl : public SharedShaderConstants {
    AutoRef<gpu2::GpuContext> mGpu;
    AutoRef<Graph>            mGraph;
    AutoRef<gpu2::Buffer>     mSceneBuffer;
    AutoRef<gpu2::Buffer>     mCameraBuffer;
    // Fallback textures bound to set0 bindings 2–5 when no env artifact is available.
    AutoRef<gpu2::Texture>    mFallbackCubemap;
    AutoRef<gpu2::Texture>    mFallbackBrdfLut;
    // Combined payload for the fallback textures; render loop submits once on first frame.
    AutoRef<gpu2::GpuPayload> mFallbackInitPayload;
    // Set by loadEnvTextures(); null means use fallbacks every frame.
    ArtifactPtr               mEnvArtifact = nullptr;

    mutable std::mutex                                      mMutex;
    mutable std::unordered_map<TokenPtr, AutoRef<Content>>  mContentMap;

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

        mSceneBuffer  = gpu2::Buffer::create("ssc2.scene_ubo",  {.context = mGpu, .size = sizeof(shader::SceneUBO)});
        mCameraBuffer = gpu2::Buffer::create("ssc2.camera_ubo", {.context = mGpu, .size = sizeof(shader::CameraUBO)});
        if (!mSceneBuffer || !mCameraBuffer) GN_UNLIKELY {
            GN_ERROR(sLogger)("SharedShaderConstants2: failed to create UBO buffers");
            return false;
        }

        // Build all fallback textures with a single CnC → one combined payload.
        // The render loop includes mFallbackInitPayload in the first frame's submission.
        // No gpu->submit() here — the render loop does the only submit.
        {
            gpu2::Texture::Descriptor cubeDesc, brdfDesc;
            cubeDesc.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1,1).setFaces(6).setLevels(1);
            brdfDesc.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1,1).setFaces(1).setLevels(1);
            mFallbackCubemap = gpu2::Texture::create("ssc2.fallback_cubemap", {.context=mGpu, .descriptor=cubeDesc});
            mFallbackBrdfLut = gpu2::Texture::create("ssc2.fallback_brdf_lut",{.context=mGpu, .descriptor=brdfDesc});
            if (!mFallbackCubemap || !mFallbackBrdfLut) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstants2: failed to create fallback textures");
                return false;
            }

            // 6 faces × 4 bytes for cubemap + 1 face × 4 bytes for brdfLut
            const uint64_t stgSize = 6 * 4 + 1 * 4;
            auto staging = gpu2::Buffer::create("ssc2.fallback_stg",
                {.context=mGpu, .size=stgSize, .mappable=true});
            if (!staging) GN_UNLIKELY return false;
            {
                auto m = staging->map();
                if (!m.data()) GN_UNLIKELY return false;
                auto * p = static_cast<uint8_t*>(m.data());
                // Cubemap: black  (0,0,0,255) for all 6 faces
                for (int f = 0; f < 6; ++f) { p[f*4]=0; p[f*4+1]=0; p[f*4+2]=0; p[f*4+3]=255; }
                // BrdfLut: white (255,255,255,255) for 1 face, starting at offset 24
                p[24]=255; p[25]=255; p[26]=255; p[27]=255;
            }

            auto cnc = gpu2::GpuCnC::create({.gpu=mGpu});
            if (!cnc) GN_UNLIKELY return false;
            for (uint32_t f = 0; f < 6; ++f)
                cnc->copyBufferToImage({.src=staging, .dst=mFallbackCubemap, .regions={
                    {{0,f},{0,0,0},{1,1,1},f*4ULL}}});
            cnc->copyBufferToImage({.src=staging, .dst=mFallbackBrdfLut, .regions={
                {{0,0},{0,0,0},{1,1,1},24ULL}}});
            mFallbackInitPayload = cnc->seal();  // staging kept alive inside payload
        }

        return true;
    }

    EnvLoadResult loadEnvTextures(const EnvLoadParameters & params) override {
        auto artifact = mGraph->createArtifact("ssc2.env_textures");
        // OOO() is satisfied when v1 is published (synchronously below — CPU staging is enough).
        auto ready = mGraph->getArtifactVersionToken(artifact, OOO());

        // CPU: alloc staging + memcpy for blue-sky defaults. One CnC → one combined payload.
        // All four stagings are captured by the CnC and kept alive inside the sealed payload.
        auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
        if (!cnc) GN_UNLIKELY return {};
        auto skyCubemap  = make1x1Texture(mGpu, *cnc, "ssc2.sky_default",       135, 206, 235, 255, 6);
        auto irradiance  = make1x1Texture(mGpu, *cnc, "ssc2.irr_default",        34,  68, 102, 255, 6);
        auto prefiltered = make1x1Texture(mGpu, *cnc, "ssc2.prefilter_default",  34,  68, 102, 255, 6);
        auto defBrdf     = make1x1Texture(mGpu, *cnc, "ssc2.brdf_default",      255, 255,   0,   0, 1);

        EnvTextureSet defaults;
        defaults.skyboxCubemap            = skyCubemap;
        defaults.irradianceMap            = irradiance;
        defaults.prefilteredEnvMap        = prefiltered;
        defaults.brdfLut                  = defBrdf;
        defaults.environmentRadianceScale = params.environmentRadianceScale;
        defaults.uploadPayload            = cnc->seal(); // all four stagings kept alive inside payload

        // Publish v1 synchronously. The textures' staging buffers live inside uploadPayload.
        // The render loop will submit uploadPayload on the first frame it encounters this version.
        mGraph->publishArtifact(artifact, std::any{std::move(defaults)});

        // Store so takeSnapshot() can read the latest env version each frame.
        {
            std::lock_guard<std::mutex> lk(mMutex);
            mEnvArtifact = artifact;
        }

        // Async: worker thread does IO + staging. Publishes v2 with new uploadPayload.
        // NO gpu->submit() here — the render loop handles all GPU submission.
        float scale   = params.environmentRadianceScale;
        bool simulate = params.simulateSlowLoading;
        std::thread([=, gpu = mGpu, graph = mGraph,
                     skyPath    = params.skyboxPath,
                     irrPath    = params.irradiancePath,
                     prefilPath = params.prefilteredPath,
                     brdfPath   = params.brdfLutPath]() mutable
        {
            auto cnc = gpu2::GpuCnC::create({.gpu = gpu});
            if (!cnc) return;

            auto skyTex    = stageTextureLoad("env.skybox",      gpu, skyPath,    *cnc);
            auto irrTex    = stageTextureLoad("env.irradiance",  gpu, irrPath,    *cnc);
            auto prefilTex = stageTextureLoad("env.prefiltered", gpu, prefilPath, *cnc);
            auto brdfTex   = stageTextureLoad("env.brdf_lut",    gpu, brdfPath,   *cnc);

            if (simulate) std::this_thread::sleep_for(std::chrono::seconds(5));

            EnvTextureSet real;
            real.skyboxCubemap            = skyTex    ? skyTex    : AutoRef<gpu2::Texture>{};
            real.irradianceMap            = irrTex    ? irrTex    : AutoRef<gpu2::Texture>{};
            real.prefilteredEnvMap        = prefilTex ? prefilTex : AutoRef<gpu2::Texture>{};
            real.brdfLut                  = brdfTex   ? brdfTex   : AutoRef<gpu2::Texture>{};
            real.environmentRadianceScale = scale;
            real.uploadPayload            = cnc->seal(); // null if no textures loaded

            // Publish v2 directly — NO gpu->submit(). The render loop submits all payloads.
            graph->publishArtifact(artifact, std::any{std::move(real)});
        }).detach();

        return {artifact, ready};
    }

    TokenPtr takeSnapshot() const override {
        {
            std::lock_guard<std::mutex> lock(mMutex);
            for (auto it = mContentMap.begin(); it != mContentMap.end();) {
                if (mGraph->waitForToken(it->first) == Graph::WaitResult::IDLE)
                    it = mContentMap.erase(it);
                else
                    ++it;
            }
        }

        auto content = AutoRef<Content>(new Content());
        {
            std::lock_guard<std::mutex> lock(mMutex);
            content->set0Parameters = set0;
            if (mEnvArtifact) {
                auto env = mGraph->getTypedArtifactContent<EnvTextureSet>(mEnvArtifact);
                if (env) content->envTextures = *env;
            }
        }
        // Provide fallback payload so the render loop can submit it exactly once (first frame).
        // Same pointer every snapshot; render loop deduplicates by AutoRef pointer equality.
        content->fallbackInitPayload = mFallbackInitPayload;
        buildSet0Resources(content->set0Parameters, content->envTextures, content->set0Resources);

        NodePtr node = mGraph->addNode(
            NodeDesc("ssc2 upload")
                .setAction(Action::createFromLambda("pack+upload UBOs",
                    [this, content]() { uploadSnapshot(content); }), nullptr));
        if (!node) GN_UNLIKELY {
            GN_ERROR(sLogger)("SharedShaderConstants2: addNode failed");
            return {};
        }

        TokenPtr token = mGraph->getNodeCompletionToken(node);
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mContentMap[token] = content;
        }
        return token;
    }

    AutoRef<const Content> getContent(TokenPtr token) const override {
        std::lock_guard<std::mutex> lock(mMutex);
        auto it = mContentMap.find(token);
        if (it == mContentMap.end()) return {};
        return it->second;
    }

private:
    void buildSet0Resources(const Set0Parameters & snap, const EnvTextureSet & env,
                            gpu2::GpuResourceSet & out) const {
        out.resize(6);

        out[0].resize(1);
        out[0][0].resource = mSceneBuffer;
        out[0][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM)
                 .setBufferViewOffset(0).setBufferViewSize(sizeof(shader::SceneUBO));

        out[1].resize(1);
        out[1][0].resource = mCameraBuffer;
        out[1][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM)
                 .setBufferViewOffset(0).setBufferViewSize(sizeof(shader::CameraUBO));

        auto bindTex = [&](uint32_t slot, const AutoRef<gpu2::Texture> & tex,
                           const AutoRef<gpu2::Texture> & fallback) {
            out[slot].resize(1);
            out[slot][0].resource = tex ? tex : fallback;
            out[slot][0].setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        };

        bindTex(2, env.skyboxCubemap,     mFallbackCubemap);
        bindTex(3, env.irradianceMap,     mFallbackCubemap);
        bindTex(4, env.prefilteredEnvMap, mFallbackCubemap);
        bindTex(5, env.brdfLut,           mFallbackBrdfLut);
    }

    void uploadSnapshot(const AutoRef<Content> & content) const {
        const Set0Parameters & snap = content->set0Parameters;

        shader::SceneUBO scene {};
        scene.frameCounter             = snap.frameConstants.frameCounter;
        scene.frameDurationMs          = (float)((double)snap.frameConstants.frameDuration.count() * 1e-3);
        scene.environmentRadianceScale = content->envTextures.environmentRadianceScale;

        const size_t numLights = std::min((size_t)shader::MAX_SCENE_LIGHTS, snap.directLighting.size());
        scene.numLights = (uint32_t)numLights;
        for (uint32_t i = 0; i < numLights; ++i) {
            const DirectLight & src = snap.directLighting[i];
            shader::DirectLightData & dst = scene.lights[i];
            switch (src.type) {
            case DirectLight::POINT: {
                dst.positionOrDir = glm::vec4(glm::vec3(src.point.position), (float)DirectLight::POINT);
                float iv = src.point.intensity.intensity.value;
                dst.colorAndRange = glm::vec4(src.point.intensity.r*iv, src.point.intensity.g*iv, src.point.intensity.b*iv, src.point.range);
                dst.coneAngles = glm::vec4(0.f); break;
            }
            case DirectLight::SPOT: {
                dst.positionOrDir = glm::vec4(glm::vec3(src.spot.position), (float)DirectLight::SPOT);
                float iv = src.spot.intensity.intensity.value;
                dst.colorAndRange = glm::vec4(src.spot.intensity.r*iv, src.spot.intensity.g*iv, src.spot.intensity.b*iv, src.spot.range);
                dst.coneAngles = glm::vec4(src.spot.cosInnerConeAngle, src.spot.cosOuterConeAngle, 0.f, 0.f); break;
            }
            case DirectLight::DIRECTIONAL: {
                glm::vec3 dir = glm::mat3_cast(src.directional.orientation) * glm::vec3(0.f, 0.f, -1.f);
                dst.positionOrDir = glm::vec4(dir, (float)DirectLight::DIRECTIONAL);
                float iv = src.directional.irradiance.irradiance.value;
                dst.colorAndRange = glm::vec4(src.directional.irradiance.r*iv, src.directional.irradiance.g*iv, src.directional.irradiance.b*iv, 0.f);
                dst.coneAngles = glm::vec4(0.f); break;
            }
            }
        }

        shader::CameraUBO cam {};
        {
            const glm::vec3 pos = snap.camera.cameraPosition;
            glm::mat4 camToWorld = glm::translate(glm::mat4(1.f), pos) * glm::mat4_cast(snap.camera.cameraOrientation);
            cam.viewMatrix     = glm::inverse(camToWorld);
            cam.projMatrix     = glm::perspectiveRH_ZO(snap.camera.cameraFov.value, snap.camera.aspectRatio, snap.camera.nearPlane, snap.camera.farPlane);
            cam.projMatrix[1][1] *= -1.f; // Vulkan Y-inversion
            cam.projViewMatrix = cam.projMatrix * cam.viewMatrix;
            cam.cameraPosition = glm::vec4(pos, 1.f);
            auto [w, h]        = getRenderTargetSize(snap.renderTarget);
            cam.renderTargetSize = glm::vec2((float)w, (float)h);
            cam.nearPlane      = snap.camera.nearPlane;
            cam.farPlane       = snap.camera.farPlane;
        }

        auto stagingScene = gpu2::Buffer::create("ssc2.staging_scene", {.context = mGpu, .size = sizeof(shader::SceneUBO), .mappable = true});
        auto stagingCam   = gpu2::Buffer::create("ssc2.staging_cam",   {.context = mGpu, .size = sizeof(shader::CameraUBO), .mappable = true});
        if (!stagingScene || !stagingCam) GN_UNLIKELY return;

        { auto m = stagingScene->map(); if (!m.data()) GN_UNLIKELY return; memcpy(m.data(), &scene, sizeof(scene)); }
        { auto m = stagingCam->map();   if (!m.data()) GN_UNLIKELY return; memcpy(m.data(), &cam,   sizeof(cam));   }

        auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
        if (!cnc) GN_UNLIKELY return;
        cnc->copyBufferToBuffer({.src = stagingScene, .dst = mSceneBuffer, .size = sizeof(shader::SceneUBO)});
        cnc->copyBufferToBuffer({.src = stagingCam,   .dst = mCameraBuffer, .size = sizeof(shader::CameraUBO)});
        content->set0Payload = cnc->seal();
    }
};

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

- [ ] **Step 2: Build**

```bash
python3 env/bin/build.py d 2>&1 | grep -E "error:" | head -20
```

Expected: SSC2 compiles. Remaining errors in `simple-effect.cpp` (not yet created).

---

## Task 3: Create v2 skybox shaders

**Files:** `src/core/rdg/2/vk-shaders/skybox.vert` and `skybox.frag`

- [ ] **Step 1: Create `skybox.vert`**

```glsl
#version 450
#extension GL_GOOGLE_include_directive : require

#include "camera-ubo.h"

layout(location = 0) out vec3 v_dir;

// Full-screen triangle; no VBO needed.
const vec2 kPos[3] = vec2[](vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0));

void main() {
    vec2 ndc = kPos[gl_VertexIndex];

    // Unproject NDC to world-space direction using the v2 camera UBO.
    mat4 invProj    = inverse(u_camera.projMatrix);
    mat3 invViewRot = transpose(mat3(u_camera.viewMatrix)); // rotation inverse = transpose

    vec4 viewDir = invProj * vec4(ndc, 1.0, 1.0);
    viewDir.z    = -1.0;

    v_dir = invViewRot * viewDir.xyz;

    // Depth = 1.0 so skybox is always behind geometry (LESS_EQUAL depth test).
    gl_Position = vec4(ndc, 1.0, 1.0);
}
```

- [ ] **Step 2: Create `skybox.frag`**

```glsl
#version 450
#extension GL_GOOGLE_include_directive : require

#include "scene-ubo.h"

layout(location = 0) in  vec3 v_dir;
layout(location = 0) out vec4 o_color;

vec3 gn_tonemap(vec3 radiance) {
    const float exposure = 0.002;
    vec3 c = radiance * exposure;
    c = c / (c + vec3(1.0));
    return c;
}

void main() {
    vec3 raw = texture(sscSkyboxCubemap, v_dir).rgb * u_scene.environmentRadianceScale;
    o_color  = vec4(gn_tonemap(raw), 1.0);
}
```

---

## Task 4: Create v2 PBR shaders

**Files:** `src/core/rdg/2/vk-shaders/pbr.vert` and `pbr.frag`

- [ ] **Step 1: Create `pbr.vert`**

```glsl
#version 450
#extension GL_GOOGLE_include_directive : require

#include "camera-ubo.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexCoord;

// 128-byte push constant: worldTransform + normalTransform (pre-computed on CPU).
layout(push_constant, std430) uniform PC {
    mat4 worldTransform;
    mat4 normalTransform; // transpose(inverse(worldTransform)); pre-computed to avoid per-vertex mat inverse
} pc;

void main() {
    vec4 worldPos = pc.worldTransform * vec4(inPosition, 1.0);
    outWorldPos   = worldPos.xyz;
    outNormal     = mat3(pc.normalTransform) * inNormal;
    outTexCoord   = inTexCoord;
    gl_Position   = u_camera.projViewMatrix * worldPos;
}
```

- [ ] **Step 2: Create `pbr.frag`**

```glsl
#version 450
#extension GL_GOOGLE_include_directive : require

#include "scene-ubo.h"
#include "camera-ubo.h"

// Set 1: PBR material textures
layout(set = 1, binding = 0) uniform sampler2D u_albedo;
layout(set = 1, binding = 1) uniform sampler2D u_normal;
layout(set = 1, binding = 2) uniform sampler2D u_emissive;
layout(set = 1, binding = 3) uniform sampler2D u_occlusion;
layout(set = 1, binding = 4) uniform sampler2D u_metalRough;

layout(location = 0) in  vec3 inWorldPos;
layout(location = 1) in  vec3 inNormal;
layout(location = 2) in  vec2 inTexCoord;
layout(location = 0) out vec4 outColor;

const float PI = 3.14159265359;

mat3 buildTBN(vec3 N, vec3 worldPos, vec2 uv) {
    vec3 dpdx  = dFdx(worldPos); vec3 dpdy  = dFdy(worldPos);
    vec2 duvdx = dFdx(uv);       vec2 duvdy = dFdy(uv);
    vec3 T = normalize(dpdx * duvdy.t - dpdy * duvdx.t);
    vec3 B = normalize(dpdy * duvdx.s - dpdx * duvdy.s);
    return mat3(T, B, N);
}

vec3 fresnelSchlick(float cos, vec3 F0) { return F0 + (1.0-F0)*pow(clamp(1.0-cos,0.0,1.0),5.0); }
vec3 fresnelSchlickR(float cos, vec3 F0, float r) { return F0+(max(vec3(1.0-r),F0)-F0)*pow(clamp(1.0-cos,0.0,1.0),5.0); }

float distGGX(vec3 N, vec3 H, float r) {
    float a=r*r, a2=a*a, NdH=max(dot(N,H),0.0), NdH2=NdH*NdH;
    float d=NdH2*(a2-1.0)+1.0; d=PI*d*d;
    return (d>0.0)?(a2/d):0.0;
}
float geoSGGX(float NdX, float k) { return NdX/(NdX*(1.0-k)+k); }
float geoSmith(vec3 N, vec3 V, vec3 L, float r) {
    float k=r*r*0.5;
    return geoSGGX(max(dot(N,V),0.0),k)*geoSGGX(max(dot(N,L),0.0),k);
}
vec3 tonemap(vec3 r) { const float e=0.002; r*=e; return r/(r+vec3(1.0)); }

void main() {
    vec3  baseColor = texture(u_albedo,    inTexCoord).rgb;
    vec3  arm       = texture(u_metalRough,inTexCoord).rgb;
    float metallic  = arm.b;
    float roughness = max(arm.g, 0.04);
    float ao        = texture(u_occlusion, inTexCoord).r;
    vec3  emissive  = texture(u_emissive,  inTexCoord).rgb;

    vec3 N = normalize(inNormal);
    vec3 Ns = texture(u_normal, inTexCoord).rgb * 2.0 - 1.0;
    N  = normalize(buildTBN(N, inWorldPos, inTexCoord) * Ns);

    vec3 V  = normalize(u_camera.cameraPosition.xyz - inWorldPos);
    vec3 F0 = mix(vec3(0.04), baseColor, metallic);

    vec3 Lo = vec3(0.0);
    for (uint i = 0u; i < u_scene.numLights && i < uint(MAX_SCENE_LIGHTS); ++i) {
        DirectLightData light = u_scene.lights[i];
        vec3 L = (uint(light.positionOrDir.w) == uint(SCENE_LIGHT_TYPE_DIRECTIONAL))
            ? normalize(-light.positionOrDir.xyz)
            : normalize(light.positionOrDir.xyz - inWorldPos);
        vec3 H = normalize(V+L);
        float NdL = max(dot(N,L),0.0), NdV = max(dot(N,V),1e-5);
        float D=distGGX(N,H,roughness), G=geoSmith(N,V,L,roughness);
        vec3  F=fresnelSchlick(max(dot(H,V),0.0),F0);
        vec3  spec=(D*G*F)/(4.0*NdV*NdL+1e-5);
        vec3  kD=(vec3(1.0)-F)*(1.0-metallic);
        Lo += (kD*baseColor/PI + spec) * light.colorAndRange.rgb * NdL;
    }

    // IBL ambient
    float NdV = max(dot(N,V),0.0);
    vec3  Fr  = fresnelSchlickR(NdV, F0, roughness);
    vec3  kDi = (vec3(1.0)-Fr)*(1.0-metallic);
    float envScale = u_scene.environmentRadianceScale;
    vec3  irr  = texture(sscIrradianceMap, N).rgb * envScale;
    vec3  R    = reflect(-V, N);
    const float MAX_LOD = 4.0;
    vec3  pref = textureLod(sscPrefilteredEnvMap, R, roughness*MAX_LOD).rgb * envScale;
    vec2  brdf = texture(sscBrdfLut, vec2(NdV, roughness)).rg;
    Lo += kDi*baseColor*irr*ao + pref*(Fr*brdf.x+brdf.y)*ao;

    if (u_scene.numLights == 0u) Lo += baseColor * 0.03;

    Lo += emissive;
    outColor = vec4(tonemap(Lo), 1.0);
}
```

---

## Task 5: Build system — v2 shader compilation

**Files:**
- Create: `src/core/rdg/2/CMakeLists.txt`
- Modify: `src/core/rdg/CMakeLists.txt`
- Modify: `src/core/CMakeLists.txt`

- [ ] **Step 1: Create `src/core/rdg/2/CMakeLists.txt`**

```cmake
if(VULKAN_FOUND)
    find_package(Python3 REQUIRED)
    set(_compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)
    set(_shader_dir   ${CMAKE_CURRENT_SOURCE_DIR}/vk-shaders)

    set(_pbr_vert_h    ${CMAKE_CURRENT_BINARY_DIR}/pbr-vert.spv.h)
    set(_pbr_frag_h    ${CMAKE_CURRENT_BINARY_DIR}/pbr-frag.spv.h)
    set(_sky_vert_h    ${CMAKE_CURRENT_BINARY_DIR}/skybox-vert.spv.h)
    set(_sky_frag_h    ${CMAKE_CURRENT_BINARY_DIR}/skybox-frag.spv.h)

    file(GLOB _shader_headers "${_shader_dir}/*.h")

    add_custom_command(
        OUTPUT "${_pbr_vert_h}" "${_pbr_frag_h}"
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_shader_dir}/pbr.vert ${_shader_dir}/pbr.frag
                --out-dir . -I ${_shader_dir}
        DEPENDS ${_compile_tool} ${_shader_dir}/pbr.vert ${_shader_dir}/pbr.frag ${_shader_headers}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Compiling rdg2 PBR GLSL to SPIR-V"
    )
    add_custom_command(
        OUTPUT "${_sky_vert_h}" "${_sky_frag_h}"
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_shader_dir}/skybox.vert ${_shader_dir}/skybox.frag
                --out-dir . -I ${_shader_dir}
        DEPENDS ${_compile_tool} ${_shader_dir}/skybox.vert ${_shader_dir}/skybox.frag ${_shader_headers}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Compiling rdg2 Skybox GLSL to SPIR-V"
    )

    add_custom_target(GNcore-rdg2-shaders DEPENDS
        ${_pbr_vert_h} ${_pbr_frag_h} ${_sky_vert_h} ${_sky_frag_h})
endif()
```

- [ ] **Step 2: Modify `src/core/rdg/CMakeLists.txt`**

Add `add_subdirectory(2)` after the existing `add_subdirectory(1)`:

```cmake
add_subdirectory(1)
add_subdirectory(2)
```

- [ ] **Step 3: Modify `src/core/CMakeLists.txt`**

After `GN_add_source_folder(rdg_sources rdg/2)` (line 86), insert:

```cmake
# simple-effect.cpp includes SPIR-V headers generated into the rdg/2 binary dir.
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/rdg/2/simple-effect.cpp
    PROPERTIES COMPILE_FLAGS "-I${CMAKE_CURRENT_BINARY_DIR}/rdg/2")
```

After the `if(TARGET GNcore-rdg-shaders)` block (around line 120), add:

```cmake
if(TARGET GNcore-rdg2-shaders)
    add_dependencies(GNcore-objects GNcore-rdg2-shaders)
endif()
```

- [ ] **Step 4: Configure and verify**

```bash
python3 env/bin/build.py -c d 2>&1 | tail -5
```

Expected: CMake configures without errors.

---

## Task 6: Create `simple-effect.h`

**File:** `src/inc/garnet/rdg/2/simple-effect.h`

- [ ] **Step 1: Create the header**

```cpp
#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg/2/simple-effect.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

namespace GN::rdg2 {

/// Artifact payload for one renderable PBR object: material textures + mesh geometry.
/// v1 (synchronous default): white-matte 1×1 textures + unit cube, published immediately.
/// v2 (async-loaded): real textures from DDS files + real mesh from GLTF.
/// uploadPayload carries the staging→device copy work; the render loop includes it in the
/// frame submission exactly once per version (dedup by comparing AutoRef pointer).
struct PbrAsset {
    AutoRef<gpu2::Texture> albedo;      ///< sRGB base color         — default 1×1 white  (255,255,255)
    AutoRef<gpu2::Texture> normal;      ///< tangent-space normal     — default 1×1 flat   (128,128,255)
    AutoRef<gpu2::Texture> emissive;    ///< emissive radiance        — default 1×1 black  (0,0,0)
    AutoRef<gpu2::Texture> occlusion;   ///< ambient occlusion        — default 1×1 white  (255,255,255)
    AutoRef<gpu2::Texture> metalRough;  ///< metallic(B)+roughness(G) — default (0,128,0)
    gpu2::RasterGeometry   geometry;    ///< mesh — default: unit cube (24 verts, 36 idx, pos+norm+uv)
    /// Staging→device copy for this version. Render loop submits once per version change.
    AutoRef<gpu2::GpuPayload> uploadPayload;
};

/// Renders a fullscreen skybox using env cubemap from SharedShaderConstants set0.
struct SkyBox : Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

    struct CreateParameters {
        AutoRef<gpu2::GpuContext> gpu;
        AutoRef<Graph>            graph;
    };

    /// Satisfied once skybox shaders are compiled. Render nodes must dependsOn() this.
    GN_API TokenPtr readyToken() const;

    /// Build DrawParameters for one fullscreen skybox pass.
    /// Must be called inside a graph node action.
    GN_API gpu2::GpuRaster::DrawParameters getDrawParams(
        AutoRef<const SharedShaderConstants::Content> sscContent) const;

    GN_API static AutoRef<SkyBox> create(const CreateParameters &);

protected:
    using Entity::Entity;
};

/// Renders PBR-shaded meshes with IBL from SharedShaderConstants.
struct PbrShading : Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

    struct CreateParameters {
        AutoRef<gpu2::GpuContext> gpu;
        AutoRef<Graph>            graph;
    };

    struct LoadParameters {
        StrA gltfPath;                 ///< mesh geometry source (empty = keep default unit cube)
        StrA albedoPath, normalPath, emissivePath, occlusionPath, metalRoughPath;
        bool simulateSlowLoading = false; ///< sleep ~5 s on worker during staging writes
    };
    struct LoadResult {
        ArtifactPtr artifact; ///< PbrAsset; v1=defaults, v2+=loaded
        TokenPtr    ready;    ///< satisfied when async IO and GPU upload complete
    };

    /// Satisfied once PBR shaders are compiled. Render nodes must dependsOn() this.
    GN_API TokenPtr shadersReadyToken() const;

    /// Returns immediately. Publishes default PbrAsset (v1) synchronously, starts async IO.
    /// Call multiple times for multiple independent objects.
    GN_API LoadResult load(const LoadParameters &);

    /// Build DrawParameters for one PBR mesh draw. Must be called inside a graph node action.
    /// Always works (v1 defaults visible from frame 1); v2 kicks in after async IO.
    GN_API gpu2::GpuRaster::DrawParameters getDrawParams(
        AutoRef<const SharedShaderConstants::Content> sscContent,
        ArtifactPtr                                   assetArtifact,
        const glm::mat4 &                             worldTransform) const;

    GN_API static AutoRef<PbrShading> create(const CreateParameters &);

protected:
    using Entity::Entity;
};

} // namespace GN::rdg2
```

---

## Task 7: Modify `GNrdg2.h`

**File:** `src/inc/garnet/GNrdg2.h`

- [ ] **Step 1: Add include**

Find the line `#include "rdg/2/shared-shader-constants2.h"` and add after it:

```cpp
#include "rdg/2/simple-effect.h"
```

---

## Task 8: Create `simple-effect.cpp`

**File:** `src/core/rdg/2/simple-effect.cpp`

This is the largest file. It includes compiled SPIR-V bytes via generated headers.

- [ ] **Step 1: Write the implementation**

```cpp
#include <garnet/GNrdg2.h>
#include "pbr-vert.spv.h"
#include "pbr-frag.spv.h"
#include "skybox-vert.spv.h"
#include "skybox-frag.spv.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <thread>
#include <cstring>

static GN::Logger * sLogger = GN::getLogger("GN.rdg2");

namespace GN::rdg2 {

// ─────────────────────────────────────────────────────────────────────────────
// Internal helpers — CPU-only, no GPU submit
// ─────────────────────────────────────────────────────────────────────────────

/// Records a 1×1 solid-color texture copy into cnc. Caller seals the CnC for a combined payload.
/// Staging buffer is captured by cnc and kept alive inside the sealed payload.
static AutoRef<gpu2::Texture> make1x1Tex(
    AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc,
    const StrA & name, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint32_t faces)
{
    gpu2::Texture::Descriptor desc;
    desc.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM())
        .setDimensions(1, 1).setFaces(faces).setLevels(1);
    auto tex = gpu2::Texture::create(name, {.context = gpu, .descriptor = desc});
    if (!tex) GN_UNLIKELY return {};

    const uint64_t stgSz = 4ULL * faces;
    auto stg = gpu2::Buffer::create(name + "_stg", {.context = gpu, .size = stgSz, .mappable = true});
    if (!stg) GN_UNLIKELY return {};

    {
        auto m = stg->map();
        if (!m.data()) GN_UNLIKELY return {};
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t f = 0; f < faces; ++f) {
            p[f*4+0]=r; p[f*4+1]=g; p[f*4+2]=b; p[f*4+3]=a;
        }
    }

    gpu2::ArrayContainer<gpu2::GpuCnC::Region> regions;
    for (uint32_t f = 0; f < faces; ++f)
        regions.append(gpu2::GpuCnC::Region{
            .imageSubresource = {0, f}, .imageOffset = {0,0,0},
            .imageExtent = {1,1,1}, .bufferOffset = f * 4ULL});
    cnc.copyBufferToImage({.src=stg, .dst=tex, .regions=regions});
    return tex;  // staging reference held by cnc until seal()
}

/// Records staging→device copies for a unit cube into cnc. Returns {vb, ib}. No GPU submit.
/// 24 verts (4 per face, interleaved pos+normal+uv = 32 bytes), 36 uint16 indices.
static std::pair<AutoRef<gpu2::Buffer>, AutoRef<gpu2::Buffer>>
makeUnitCubeStaged(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc)
{
    struct Vert { float px,py,pz, nx,ny,nz, u,v; };
    static const Vert verts[24] = {
        // +Z face (normal 0,0,1)
        {-0.5f,-0.5f, 0.5f, 0,0,1, 0,1}, { 0.5f,-0.5f, 0.5f, 0,0,1, 1,1},
        { 0.5f, 0.5f, 0.5f, 0,0,1, 1,0}, {-0.5f, 0.5f, 0.5f, 0,0,1, 0,0},
        // -Z face (normal 0,0,-1)
        { 0.5f,-0.5f,-0.5f, 0,0,-1, 0,1}, {-0.5f,-0.5f,-0.5f, 0,0,-1, 1,1},
        {-0.5f, 0.5f,-0.5f, 0,0,-1, 1,0}, { 0.5f, 0.5f,-0.5f, 0,0,-1, 0,0},
        // +X
        { 0.5f,-0.5f, 0.5f, 1,0,0, 0,1}, { 0.5f,-0.5f,-0.5f, 1,0,0, 1,1},
        { 0.5f, 0.5f,-0.5f, 1,0,0, 1,0}, { 0.5f, 0.5f, 0.5f, 1,0,0, 0,0},
        // -X
        {-0.5f,-0.5f,-0.5f,-1,0,0, 0,1}, {-0.5f,-0.5f, 0.5f,-1,0,0, 1,1},
        {-0.5f, 0.5f, 0.5f,-1,0,0, 1,0}, {-0.5f, 0.5f,-0.5f,-1,0,0, 0,0},
        // +Y
        {-0.5f, 0.5f, 0.5f, 0,1,0, 0,1}, { 0.5f, 0.5f, 0.5f, 0,1,0, 1,1},
        { 0.5f, 0.5f,-0.5f, 0,1,0, 1,0}, {-0.5f, 0.5f,-0.5f, 0,1,0, 0,0},
        // -Y
        {-0.5f,-0.5f,-0.5f, 0,-1,0, 0,1}, { 0.5f,-0.5f,-0.5f, 0,-1,0, 1,1},
        { 0.5f,-0.5f, 0.5f, 0,-1,0, 1,0}, {-0.5f,-0.5f, 0.5f, 0,-1,0, 0,0},
    };
    static const uint16_t idx[36] = {
        0,1,2, 0,2,3,   4,5,6, 4,6,7,   8,9,10, 8,10,11,
        12,13,14, 12,14,15,  16,17,18, 16,18,19,  20,21,22, 20,22,23,
    };

    const uint64_t vbSz = sizeof(verts), ibSz = sizeof(idx);
    auto stgVB = gpu2::Buffer::create("cube_stg_vb", {.context=gpu, .size=vbSz, .mappable=true});
    auto stgIB = gpu2::Buffer::create("cube_stg_ib", {.context=gpu, .size=ibSz, .mappable=true});
    if (!stgVB || !stgIB) GN_UNLIKELY return {};

    { auto m=stgVB->map(); if (!m.data()) return {}; memcpy(m.data(), verts, vbSz); }
    { auto m=stgIB->map(); if (!m.data()) return {}; memcpy(m.data(), idx,   ibSz); }

    auto vb = gpu2::Buffer::create("cube_vb", {.context=gpu, .size=vbSz});
    auto ib = gpu2::Buffer::create("cube_ib", {.context=gpu, .size=ibSz});
    if (!vb || !ib) GN_UNLIKELY return {};

    cnc.copyBufferToBuffer({.src=stgVB, .dst=vb, .size=vbSz});
    cnc.copyBufferToBuffer({.src=stgIB, .dst=ib, .size=ibSz});
    return {vb, ib};  // stagings held by cnc until caller calls seal()
}

static gpu2::RasterGeometry::VertexFormat stdVertexFormat() {
    gpu2::RasterGeometry::VertexFormat fmt;
    fmt.attributes.append({.location=0, .binding=0, .offset=0,  .format=gpu2::RasterGeometry::AttributeFormat::F32_3});
    fmt.attributes.append({.location=1, .binding=0, .offset=12, .format=gpu2::RasterGeometry::AttributeFormat::F32_3});
    fmt.attributes.append({.location=2, .binding=0, .offset=24, .format=gpu2::RasterGeometry::AttributeFormat::F32_2});
    return fmt;
}

// ─────────────────────────────────────────────────────────────────────────────
// SkyBox
// ─────────────────────────────────────────────────────────────────────────────

class SkyBoxImpl : public SkyBox {
    AutoRef<gpu2::GpuContext> mGpu;
    AutoRef<Graph>            mGraph;
    AutoRef<gpu2::GpuShader>  mVS, mPS;
    TokenPtr                  mReady = nullptr;

public:
    GN_REGISTER_RUNTIME_TYPE(SkyBox);

    SkyBoxImpl(): SkyBox(TYPE_INFO(), "SkyBox") {}

    bool init(const CreateParameters & p) {
        if (!p.gpu || !p.graph) return false;
        mGpu = p.gpu; mGraph = p.graph;

        // Shader compilation (vkCreateShaderModule) is synchronous and has no GPU payload;
        // wrap in a graph node so readyToken() gates render nodes on compilation finishing.
        auto node = mGraph->addNode(NodeDesc("skybox.compile")
            .setAction(Action::createFromLambda("compile skybox shaders", [this]() {
                mVS = gpu2::GpuShader::create({.context=mGpu, .name="skybox.vert",
                    .binary=kSkyboxVertSpv, .size=sizeof(kSkyboxVertSpv)});
                mPS = gpu2::GpuShader::create({.context=mGpu, .name="skybox.frag",
                    .binary=kSkyboxFragSpv, .size=sizeof(kSkyboxFragSpv)});
            }), nullptr));
        if (!node) return false;
        mReady = mGraph->getNodeCompletionToken(node);
        return true;
    }

    TokenPtr readyToken() const override { return mReady; }

    gpu2::GpuRaster::DrawParameters getDrawParams(
        AutoRef<const SharedShaderConstants::Content> sscContent) const override
    {
        gpu2::GpuRaster::DrawParameters dp;
        dp.vs = mVS;
        dp.ps = mPS;
        dp.states.depthTestFunc = gpu2::RasterState::DepthTestFunc::LESS_EQUAL;
        dp.states.cullMode      = gpu2::RasterState::CullMode::NONE;
        dp.geometry.vertexCount = 3;  // fullscreen triangle, no VBO
        dp.resources.resize(1);
        dp.resources[0] = sscContent->set0Resources;
        return dp;
    }
};

GN_API AutoRef<SkyBox> SkyBox::create(const CreateParameters & p) {
    auto impl = AutoRef<SkyBoxImpl>(new SkyBoxImpl());
    if (!impl->init(p)) {
        GN_ERROR(sLogger)("SkyBox::create failed");
        return {};
    }
    return impl;
}

// ─────────────────────────────────────────────────────────────────────────────
// PbrShading
// ─────────────────────────────────────────────────────────────────────────────

class PbrShadingImpl : public PbrShading {
    AutoRef<gpu2::GpuContext> mGpu;
    AutoRef<Graph>            mGraph;
    AutoRef<gpu2::GpuShader>  mVS, mPS;
    TokenPtr                  mShadersReady = nullptr;

public:
    GN_REGISTER_RUNTIME_TYPE(PbrShading);

    PbrShadingImpl(): PbrShading(TYPE_INFO(), "PbrShading") {}

    bool init(const CreateParameters & p) {
        if (!p.gpu || !p.graph) return false;
        mGpu = p.gpu; mGraph = p.graph;

        auto node = mGraph->addNode(NodeDesc("pbr.compile")
            .setAction(Action::createFromLambda("compile PBR shaders", [this]() {
                mVS = gpu2::GpuShader::create({.context=mGpu, .name="pbr.vert",
                    .binary=kPbrVertSpv, .size=sizeof(kPbrVertSpv)});
                mPS = gpu2::GpuShader::create({.context=mGpu, .name="pbr.frag",
                    .binary=kPbrFragSpv, .size=sizeof(kPbrFragSpv)});
            }), nullptr));
        if (!node) return false;
        mShadersReady = mGraph->getNodeCompletionToken(node);
        return true;
    }

    TokenPtr shadersReadyToken() const override { return mShadersReady; }

    LoadResult load(const LoadParameters & params) override {
        auto artifact = mGraph->createArtifact("pbr_asset");
        // OOO() is satisfied when v1 is published synchronously below.
        auto ready = mGraph->getArtifactVersionToken(artifact, OOO());

        // CPU work: alloc staging buffers + memcpy for all defaults. One CnC → one payload.
        // All six stagings are captured by the CnC and kept alive inside the sealed payload.
        auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
        if (!cnc) GN_UNLIKELY return {};

        auto albedo     = make1x1Tex(mGpu, *cnc, "pbr.albedo_def",     255, 255, 255, 255, 1);
        auto normal     = make1x1Tex(mGpu, *cnc, "pbr.normal_def",     128, 128, 255, 255, 1);
        auto emissive   = make1x1Tex(mGpu, *cnc, "pbr.emissive_def",     0,   0,   0, 255, 1);
        auto occlusion  = make1x1Tex(mGpu, *cnc, "pbr.occlusion_def",  255, 255, 255, 255, 1);
        auto metalRough = make1x1Tex(mGpu, *cnc, "pbr.metalrough_def",   0, 128,   0, 255, 1);
        auto [vb, ib]   = makeUnitCubeStaged(mGpu, *cnc);

        PbrAsset defaults;
        defaults.albedo     = albedo;
        defaults.normal     = normal;
        defaults.emissive   = emissive;
        defaults.occlusion  = occlusion;
        defaults.metalRough = metalRough;
        if (vb && ib) {
            defaults.geometry.format      = stdVertexFormat();
            defaults.geometry.vertices.append({.buffer=vb, .offset=0, .stride=32});
            defaults.geometry.vertexCount = 24;
            defaults.geometry.indices     = {.buffer=ib, .offset=0, .stride=sizeof(uint16_t)};
            defaults.geometry.indexCount  = 36;
        }
        defaults.uploadPayload = cnc->seal(); // all six stagings kept alive inside payload

        // Publish v1 synchronously. Staging buffers stay alive inside uploadPayload.
        mGraph->publishArtifact(artifact, std::any{defaults});

        // Async: worker thread handles slow IO + staging for real assets, publishes v2.
        // NO gpu->submit() in the thread — it just builds payloads and publishes the artifact.
        bool simulate = params.simulateSlowLoading;
        std::thread([=, gpu=mGpu, graph=mGraph,
                     albedoPath    = params.albedoPath,
                     normalPath    = params.normalPath,
                     emissivePath  = params.emissivePath,
                     occlusionPath = params.occlusionPath,
                     mrPath        = params.metalRoughPath,
                     defAsset      = defaults]() mutable
        {
            auto cnc = gpu2::GpuCnC::create({.gpu = gpu});
            if (!cnc) return;

            // Start from defaults so any missing texture reuses v1 GPU resources.
            PbrAsset real = defAsset;
            real.uploadPayload = nullptr; // will be set below if any new texture is loaded

            auto tryLoad = [&](const StrA & path, const StrA & name,
                               AutoRef<gpu2::Texture> & outTex) {
                if (path.empty()) return;
                auto stg = gpu2::Buffer::loadTextureToStagingBuffer(name, gpu, path);
                if (stg.empty()) return;
                auto tex = gpu2::Texture::create(name, {.context=gpu, .descriptor=stg.descriptor});
                if (!tex) return;
                cnc->copyBufferToImage(stg, tex);
                outTex = tex;
            };
            tryLoad(albedoPath,    "pbr.albedo",     real.albedo);
            tryLoad(normalPath,    "pbr.normal",     real.normal);
            tryLoad(emissivePath,  "pbr.emissive",   real.emissive);
            tryLoad(occlusionPath, "pbr.occlusion",  real.occlusion);
            tryLoad(mrPath,        "pbr.metalrough", real.metalRough);

            if (simulate) std::this_thread::sleep_for(std::chrono::seconds(5));

            real.uploadPayload = cnc->seal(); // null if no new textures loaded

            // Publish v2 directly — NO gpu->submit(). Render loop handles all submission.
            graph->publishArtifact(artifact, std::any{std::move(real)});
        }).detach();

        return {artifact, ready};
    }

    gpu2::GpuRaster::DrawParameters getDrawParams(
        AutoRef<const SharedShaderConstants::Content> sscContent,
        ArtifactPtr assetArtifact,
        const glm::mat4 & worldTransform) const override
    {
        auto asset = mGraph->getTypedArtifactContent<PbrAsset>(assetArtifact);

        gpu2::GpuRaster::DrawParameters dp;
        dp.vs = mVS;
        dp.ps = mPS;
        dp.states.cullMode      = gpu2::RasterState::CullMode::BACK;
        dp.states.frontFace     = gpu2::RasterState::FrontFace::CCW;
        dp.states.depthTestFunc = gpu2::RasterState::DepthTestFunc::LESS;
        dp.states.depthWrite    = true;

        // 128-byte push constant: worldTransform | normalTransform (pre-computed on CPU)
        glm::mat4 normalTransform = glm::transpose(glm::inverse(worldTransform));
        struct PC { glm::mat4 world, normal; };
        PC pc{worldTransform, normalTransform};
        dp.immediates = ArrayProxy<const uint8_t>(
            reinterpret_cast<const uint8_t *>(&pc), sizeof(pc));

        dp.resources.resize(2);
        dp.resources[0] = sscContent->set0Resources;

        auto & set1 = dp.resources[1];
        set1.resize(5);
        auto bindTex = [&](uint32_t slot, const AutoRef<gpu2::Texture> & tex) {
            set1[slot].resize(1);
            set1[slot][0].resource = tex;
            set1[slot][0].setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        };
        if (asset) {
            bindTex(0, asset->albedo);
            bindTex(1, asset->normal);
            bindTex(2, asset->emissive);
            bindTex(3, asset->occlusion);
            bindTex(4, asset->metalRough);
            dp.geometry = asset->geometry;
        }

        return dp;
    }
};

GN_API AutoRef<PbrShading> PbrShading::create(const CreateParameters & p) {
    auto impl = AutoRef<PbrShadingImpl>(new PbrShadingImpl());
    if (!impl->init(p)) {
        GN_ERROR(sLogger)("PbrShading::create failed");
        return {};
    }
    return impl;
}

// ─────────────────────────────────────────────────────────────────────────────
// GN_API forwarding stubs (bridge base class to impl)
// ─────────────────────────────────────────────────────────────────────────────

GN_API TokenPtr SkyBox::readyToken() const {
    return static_cast<const SkyBoxImpl *>(this)->readyToken();
}
GN_API gpu2::GpuRaster::DrawParameters SkyBox::getDrawParams(
    AutoRef<const SharedShaderConstants::Content> sc) const {
    return static_cast<const SkyBoxImpl *>(this)->getDrawParams(std::move(sc));
}

GN_API TokenPtr PbrShading::shadersReadyToken() const {
    return static_cast<const PbrShadingImpl *>(this)->shadersReadyToken();
}
GN_API PbrShading::LoadResult PbrShading::load(const LoadParameters & p) {
    return static_cast<PbrShadingImpl *>(this)->load(p);
}
GN_API gpu2::GpuRaster::DrawParameters PbrShading::getDrawParams(
    AutoRef<const SharedShaderConstants::Content> sc, ArtifactPtr art,
    const glm::mat4 & wt) const {
    return static_cast<const PbrShadingImpl *>(this)->getDrawParams(std::move(sc), art, wt);
}

} // namespace GN::rdg2
```

- [ ] **Step 2: Build**

```bash
python3 env/bin/build.py d 2>&1 | grep -E "error:" | head -20
```

Expected: `simple-effect.cpp` compiles. Resolve any type/method name errors.

---

## Task 9: Create `simple-pbr2.cpp`

**File:** `src/sample/rdg/simple-pbr2.cpp`

- [ ] **Step 1: Write the sample**

```cpp
#include <garnet/GNrdg2.h>
#include <garnet/GNinput.h>
#include <garnet/GNengine.h> // for SimpleApp / window helpers

// Orbit camera state
struct OrbitCamera {
    float yaw = 0.f, pitch = 0.3f, distance = 3.f;
    glm::vec3 target = {0,0,0};

    glm::vec3 position() const {
        return target + glm::vec3(
            distance * cosf(pitch) * sinf(yaw),
            distance * sinf(pitch),
            distance * cosf(pitch) * cosf(yaw));
    }
    glm::quat orientation() const {
        return glm::quat(glm::vec3(-pitch, yaw, 0));
    }
};

int main() {
    using namespace GN;
    using namespace GN::gpu2;
    using namespace GN::rdg2;

    // --- GPU + Graph setup ---
    auto gpu   = GpuContext::create("main", {.api = "vulkan"});
    if (!gpu) return 1;
    auto graph = Graph::create("main");

    auto ssc    = SharedShaderConstants::create({gpu, graph});
    auto pbr    = PbrShading::create({gpu, graph});
    auto skybox = SkyBox::create({gpu, graph});
    if (!ssc || !pbr || !skybox) return 1;

    // --- Async asset loading (simulate slow IO so the blue-sky default and white cube are visible) ---
    auto [envArtifact, envReady] = ssc->loadEnvTextures({
        .skyboxPath      = "media::envmap/default/skybox.dds",
        .irradiancePath  = "media::envmap/default/irradiance.dds",
        .prefilteredPath = "media::envmap/default/prefiltered.dds",
        .brdfLutPath     = "media::envmap/default/brdf_lut.dds",
        .environmentRadianceScale = 3500.f,
        .simulateSlowLoading = true,
    });

    auto [helmetArtifact, helmetReady] = pbr->load({
        .albedoPath     = "media::pbr/DamagedHelmet/albedo.dds",
        .normalPath     = "media::pbr/DamagedHelmet/normal.dds",
        .emissivePath   = "media::pbr/DamagedHelmet/emissive.dds",
        .occlusionPath  = "media::pbr/DamagedHelmet/occlusion.dds",
        .metalRoughPath = "media::pbr/DamagedHelmet/metallic_roughness.dds",
        .simulateSlowLoading = true,
    });

    // --- Window + swapchain ---
    // (Use the same window/swapchain setup pattern as simple-triangle2.cpp)
    // ... (omitted for brevity — see simple-triangle2.cpp for the full pattern)

    OrbitCamera camera;
    glm::mat4 helmetMat = glm::mat4(1.f);
    bool running = true;

    // Per-artifact dedup: submit an artifact's uploadPayload only when its content pointer
    // changes (= new version published). Content objects are immutable once published, so
    // pointer equality is a reliable version-identity check across frames.
    AutoRef<GpuPayload>      lastFallbackPayload; // fired exactly once (first frame)
    AutoRef<const EnvTextureSet> lastEnvContent;
    AutoRef<const PbrAsset>      lastHelmetContent;

    while (running) {
        // Acquire next swapchain image. This is the ONLY point where the CPU may stall
        // briefly — waiting for the swapchain fence from N frames ago, not for last frame.
        // ... swapchain->acquireNextImage(...) ...

        // Update camera state for this frame.
        ssc->set0.camera.cameraPosition    = camera.position();
        ssc->set0.camera.cameraOrientation = camera.orientation();
        ssc->set0.camera.aspectRatio       = 16.f / 9.f;
        // ... set render target from current swapchain image ...

        // takeSnapshot() schedules a CPU-only graph node that packs UBO data into a staging
        // buffer and seals the frame-prepare payload. Pure CPU work — no GPU wait.
        TokenPtr sscToken = ssc->takeSnapshot();

        // The draw node is also pure CPU work: it records draw commands into GpuRaster and
        // seals them into a GpuPayload. No GPU submission happens inside this action.
        AutoRef<GpuPayload> renderWork;
        auto drawNode = graph->addNode(NodeDesc("color pass")
            .dependsOn(pbr->shadersReadyToken())
            .dependsOn(skybox->readyToken())
            .dependsOn(sscToken)
            .dependsOn(envReady)     // satisfied when env v1 is published (CPU staging done)
            .dependsOn(helmetReady)  // satisfied when PBR v1 is published (CPU staging done)
            .setAction(Action::createFromLambda("draw", [&]() {
                auto sc = ssc->getContent(sscToken);
                auto r  = GpuRaster::create({.gpu=gpu, .target=sc->set0Parameters.renderTarget});
                r->draw(skybox->getDrawParams(sc));
                r->draw(pbr->getDrawParams(sc, helmetArtifact, helmetMat));
                renderWork = r->seal(); // CPU-only; no gpu->submit() here
            }), nullptr));

        // Wait on the graph node's CPU action only — this does NOT block on GPU completion.
        graph->waitForToken(graph->getNodeCompletionToken(drawNode));

        // ─── Single frame submission ───────────────────────────────────────────
        // Collect all payloads produced this frame. Every effect, loader, and helper has
        // already returned its GPU work as a GpuPayload embedded in an artifact. Here we
        // gather them all and issue the single gpu->submit() for the entire frame.
        auto sscContent    = ssc->getContent(sscToken);
        auto envContent    = graph->getTypedArtifactContent<EnvTextureSet>(envArtifact);
        auto helmetContent = graph->getTypedArtifactContent<PbrAsset>(helmetArtifact);

        GpuContext::SubmitParameters submit("frame");

        // 1. Frame-prepare payload FIRST — UBO data must be written before any draw reads it.
        if (sscContent->set0Payload) submit.appendWork(sscContent->set0Payload);

        // 2. Artifact upload payloads — deduplicated by payload pointer so each version
        //    uploads exactly once. A new pointer means a new artifact version was published.
        if (sscContent->fallbackInitPayload.get() != lastFallbackPayload.get()) {
            submit.appendWork(sscContent->fallbackInitPayload);
            lastFallbackPayload = sscContent->fallbackInitPayload;
        }
        if (envContent.get() != lastEnvContent.get()) {
            if (envContent && envContent->uploadPayload) submit.appendWork(envContent->uploadPayload);
            lastEnvContent = envContent;
        }
        if (helmetContent.get() != lastHelmetContent.get()) {
            if (helmetContent && helmetContent->uploadPayload) submit.appendWork(helmetContent->uploadPayload);
            lastHelmetContent = helmetContent;
        }

        // 3. Render work last — barriers inside the command buffer guarantee the uploads
        //    above complete before the draws that sample those textures.
        if (renderWork) submit.appendWork(renderWork);

        // The single gpu->submit() for the entire frame. Non-blocking: the GPU processes
        // the command buffer while the CPU immediately loops to the next frame.
        gpu->submit(submit);

        // Present after submit. The swapchain fence (checked on next acquireNextImage)
        // is the only CPU–GPU sync point; the CPU does not wait here.
        swapchain->present(currentImage);
    }

    // Drain the GPU before AutoRef destructors release Vulkan resources.
    // Only acceptable here — application exit path, not a per-frame operation.
    gpu->waitForIdle();
    return 0;
}
```

**Note:** Window/swapchain boilerplate follows the same pattern as `simple-triangle2.cpp` (`src/sample/rdg/simple-triangle2.cpp`). Copy those sections for the final implementation.

---

## Task 10: Modify sample `CMakeLists.txt`

**File:** `src/sample/rdg/CMakeLists.txt`

- [ ] **Step 1: Add the new sample target**

Append to the file:

```cmake
add_executable(GNsample-rdg2-simple-pbr simple-pbr2.cpp)
target_link_libraries(GNsample-rdg2-simple-pbr GNcore)
```

No per-sample shader compile step — v2 shaders compile as part of GNcore via `GNcore-rdg2-shaders`.

---

## Task 11: Build and verify

- [ ] **Step 1: Full build**

```bash
python3 env/bin/build.py d 2>&1 | tail -30
```

Expected: zero errors. All targets compile including `GNsample-rdg2-simple-pbr`.

- [ ] **Step 2: Verify SPIR-V headers were generated**

```bash
ls build/darwin.gcc.d/src/core/rdg/2/pbr-vert.spv.h \
      build/darwin.gcc.d/src/core/rdg/2/pbr-frag.spv.h \
      build/darwin.gcc.d/src/core/rdg/2/skybox-vert.spv.h \
      build/darwin.gcc.d/src/core/rdg/2/skybox-frag.spv.h
```

Expected: all four files present.

- [ ] **Step 3: Run the sample (manual smoke test)**

```bash
./build/darwin.gcc.d/bin/GNsample-rdg2-simple-pbr
```

Expected:
- Window opens; blue-sky background + white unit cube visible immediately (v1 defaults)
- After ~5 seconds: real env textures replace blue sky; real PBR textures replace white cube (v2)
- No validation errors in Vulkan debug output

- [ ] **Step 4: Commit**

```bash
git add src/inc/garnet/gpu2/gpu2.h \
        src/inc/garnet/gpu2/cnc.h \
        src/core/gpu2/gpu-texture-staging.cpp \
        src/inc/garnet/rdg/2/shared-shader-constants2.h \
        src/core/rdg/2/shared-shader-constants2.cpp \
        src/core/rdg/2/vk-shaders/skybox.vert \
        src/core/rdg/2/vk-shaders/skybox.frag \
        src/core/rdg/2/vk-shaders/pbr.vert \
        src/core/rdg/2/vk-shaders/pbr.frag \
        src/core/rdg/2/CMakeLists.txt \
        src/core/rdg/CMakeLists.txt \
        src/core/CMakeLists.txt \
        src/inc/garnet/rdg/2/simple-effect.h \
        src/inc/garnet/GNrdg2.h \
        src/core/rdg/2/simple-effect.cpp \
        src/sample/rdg/simple-pbr2.cpp \
        src/sample/rdg/CMakeLists.txt
git commit -m "feat: add rdg2 PbrShading, SkyBox, SSC env artifact model, and simple-pbr2 demo"
```

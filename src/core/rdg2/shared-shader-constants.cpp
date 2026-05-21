#include "pch.h"
#include "vk-shaders/scene-ubo.h"
#include "vk-shaders/camera-ubo.h"
#include "skybox-vert.spv.h"
#include "skybox-frag.spv.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <mutex>
#include <thread>
#include <unordered_map>
#include <algorithm>

static GN::Logger * sLogger = GN::getLogger("GN.rdg2");

namespace GN::rdg2 {

// ─── Internal env texture bundle ────────────────────────────────────────────
// Not in the public header; published through a local RDG2 entity wrapper.
struct EnvTextureSet {
    AutoRef<gpu2::Texture>    skyboxCubemap;
    AutoRef<gpu2::Texture>    irradianceMap;
    AutoRef<gpu2::Texture>    prefilteredEnvMap;
    AutoRef<gpu2::Texture>    brdfLut;
    AutoRef<gpu2::GpuPayload> uploadPayload;
};

struct EnvTextureSetContent final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    EnvTextureSet textures;

    explicit EnvTextureSetContent(EnvTextureSet value): Entity(TYPE_INFO(), "EnvTextureSetContent"), textures(std::move(value)) {}
};

struct SscContent final : public SharedShaderConstants::Content {
    GN_REGISTER_RUNTIME_TYPE(Content);

    SscContent(): Content(TYPE_INFO(), "SharedShaderConstants::Content") {}
};

// ─── Helpers ─────────────────────────────────────────────────────────────────

/// Create a 1×1 solid-color texture and record the staging→device copy into cnc.
/// The staging buffer is captured by the CnC and kept alive inside the sealed payload.
static AutoRef<gpu2::Texture> make1x1Texture(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc, const StrA & name, uint8_t r, uint8_t g, uint8_t b, uint8_t a,
                                             uint32_t faces) {
    gpu2::Texture::Descriptor desc;
    desc.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(faces).setLevels(1);
    auto tex = gpu2::Texture::create(name, {.context = gpu, .descriptor = desc});
    if (!tex) GN_UNLIKELY return {};

    const uint64_t stagingSize = 4ULL * faces;
    auto           staging     = gpu2::Buffer::create(name + "_stg", {.context = gpu, .size = stagingSize, .mappable = true});
    if (!staging) GN_UNLIKELY return {};

    {
        auto m = staging->map();
        if (!m.data()) GN_UNLIKELY return {};
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t f = 0; f < faces; ++f) {
            p[f * 4 + 0] = r;
            p[f * 4 + 1] = g;
            p[f * 4 + 2] = b;
            p[f * 4 + 3] = a;
        }
    }

    gpu2::ArrayContainer<gpu2::GpuCnC::Region> regions;
    for (uint32_t f = 0; f < faces; ++f) {
        gpu2::GpuCnC::Region reg;
        reg.mip          = 0;
        reg.face         = f;
        reg.imageOffset  = {0, 0, 0};
        reg.imageExtent  = {1, 1, 1};
        reg.bufferOffset = f * 4ULL;
        regions.append(reg);
    }
    cnc.copyBufferToImage({.src = staging, .dst = tex, .regions = regions});
    return tex; // staging reference held by cnc until seal()
}

// ─── SharedShaderConstants2Impl ──────────────────────────────────────────────

class SharedShaderConstants2Impl : public SharedShaderConstants {
    AutoRef<gpu2::GpuContext> mGpu;
    GraphPtr                  mGraph;
    AutoRef<gpu2::Buffer>     mSceneBuffer;
    AutoRef<gpu2::Buffer>     mCameraBuffer;

    // Skybox shaders compiled once at init; referenced by every getSkyboxDrawParams() call.
    AutoRef<gpu2::GpuShader> mSkyboxVS, mSkyboxPS;

    // Fallback textures used in buildSet0Resources() when env slots are null.
    AutoRef<gpu2::Texture> mFallbackCubemap;
    AutoRef<gpu2::Texture> mFallbackBrdfLut;
    // Upload for the fallback textures; appended to set0Payloads on the first snapshot only.
    AutoRef<gpu2::GpuPayload> mFallbackInitPayload;

    ArtifactPtr mContentArtifact = nullptr;

    // Internal env artifact. Created at init() with 1×1 blue-sky defaults (v1).
    // Async IO triggered by triggerEnvLoad() publishes v2+ on success.
    ArtifactPtr               mEnvArtifact = nullptr;
    EnvTextureSet             mEnvDefaults;    // blue-sky refs saved before v1 move
    AutoRef<gpu2::GpuPayload> mLastEnvPayload; // dedup: only append upload on version change
    EnvLightingParameters     mLastEnvParams;  // detect user-side path changes

    mutable std::mutex mMutex;

public:
    GN_REGISTER_RUNTIME_TYPE(SharedShaderConstants);

    SharedShaderConstants2Impl(): SharedShaderConstants(TYPE_INFO(), "SharedShaderConstants2") {}

    bool init(const CreateParameters & params) {
        if (!params.gpu || !params.graph) {
            GN_ERROR(sLogger)("SharedShaderConstants2: invalid create parameters");
            return false;
        }
        mGpu             = params.gpu;
        mGraph           = params.graph;
        mContentArtifact = mGraph->createArtifact("ssc2.content");

        mSceneBuffer  = gpu2::Buffer::create("ssc2.scene_ubo", {.context = mGpu, .size = sizeof(shader::SceneUBO)});
        mCameraBuffer = gpu2::Buffer::create("ssc2.camera_ubo", {.context = mGpu, .size = sizeof(shader::CameraUBO)});
        if (!mSceneBuffer || !mCameraBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstants2: failed to create UBO buffers");
                return false;
            }

        mSkyboxVS = gpu2::GpuShader::create({.context = mGpu, .name = "skybox.vert", .binary = kSkyboxVertSpv, .size = sizeof(kSkyboxVertSpv)});
        mSkyboxPS = gpu2::GpuShader::create({.context = mGpu, .name = "skybox.frag", .binary = kSkyboxFragSpv, .size = sizeof(kSkyboxFragSpv)});
        if (!mSkyboxVS || !mSkyboxPS) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstants2: failed to compile skybox shaders");
                return false;
            }

        // Fallback textures (black cubemap + white BRDF LUT) for when env slots are null.
        {
            auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
            if (!cnc) GN_UNLIKELY return false;
            mFallbackCubemap = make1x1Texture(mGpu, *cnc, "ssc2.fallback_cubemap", 0, 0, 0, 255, 6);
            mFallbackBrdfLut = make1x1Texture(mGpu, *cnc, "ssc2.fallback_brdf_lut", 255, 255, 255, 255, 1);
            if (!mFallbackCubemap || !mFallbackBrdfLut) GN_UNLIKELY {
                    GN_ERROR(sLogger)("SharedShaderConstants2: failed to create fallback textures");
                    return false;
                }
            mFallbackInitPayload = cnc->seal();
        }

        // Create env artifact and publish 1×1 blue-sky defaults as v1.
        // triggerEnvLoad() publishes v2+ when the caller sets non-empty paths in set0.envLighting.
        {
            auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
            if (!cnc) GN_UNLIKELY return false;
            auto skyCubemap  = make1x1Texture(mGpu, *cnc, "ssc2.sky_default", 135, 206, 235, 255, 6);
            auto irradiance  = make1x1Texture(mGpu, *cnc, "ssc2.irr_default", 34, 68, 102, 255, 6);
            auto prefiltered = make1x1Texture(mGpu, *cnc, "ssc2.prefilter_default", 34, 68, 102, 255, 6);
            auto defBrdf     = make1x1Texture(mGpu, *cnc, "ssc2.brdf_default", 255, 255, 0, 0, 1);

            // Save refs so triggerEnvLoad() can fall back to them per-slot on load failure.
            mEnvDefaults.skyboxCubemap     = skyCubemap;
            mEnvDefaults.irradianceMap     = irradiance;
            mEnvDefaults.prefilteredEnvMap = prefiltered;
            mEnvDefaults.brdfLut           = defBrdf;

            mEnvArtifact = mGraph->createArtifact("ssc2.env_textures");

            EnvTextureSet v1 = mEnvDefaults;
            v1.uploadPayload = cnc->seal();
            mGraph->publishArtifact(mEnvArtifact, AutoRef<EnvTextureSetContent>(new EnvTextureSetContent(std::move(v1))));
        }

        return true;
    }

    VersionedArtifact takeSnapshot() const override {
        TokenPtr version = mGraph->getArtifactVersionToken(mContentArtifact, NeverOverflowingCounter::OOO());
        auto     content = AutoRef<SscContent>(new SscContent());
        {
            std::lock_guard<std::mutex> lock(mMutex);
            content->set0Parameters = set0;

            // Check for path changes and start async env loading if needed.
            bool pathsChanged = set0.envLighting.skyboxPath != mLastEnvParams.skyboxPath || set0.envLighting.irradiancePath != mLastEnvParams.irradiancePath ||
                                set0.envLighting.prefilteredPath != mLastEnvParams.prefilteredPath ||
                                set0.envLighting.brdfLutPath != mLastEnvParams.brdfLutPath;
            if (pathsChanged) {
                const_cast<SharedShaderConstants2Impl *>(this)->triggerEnvLoad(set0.envLighting);
                const_cast<SharedShaderConstants2Impl *>(this)->mLastEnvParams = set0.envLighting;
            }

            // Read current env version; append upload payload once per version change.
            auto envContent = mGraph->getTypedArtifactContent<EnvTextureSetContent>(mEnvArtifact);
            if (!envContent) GN_UNLIKELY return {};
            EnvTextureSet envSnap = envContent->textures;
            if (envSnap.uploadPayload.get() != mLastEnvPayload.get()) {
                if (envSnap.uploadPayload) content->set0Payloads.append(envSnap.uploadPayload);
                const_cast<SharedShaderConstants2Impl *>(this)->mLastEnvPayload = envSnap.uploadPayload;
            }

            // Include fallback upload only on the very first snapshot, then clear.
            if (mFallbackInitPayload) {
                content->set0Payloads.append(mFallbackInitPayload);
                const_cast<SharedShaderConstants2Impl *>(this)->mFallbackInitPayload = {};
            }

            buildSet0Resources(envSnap, content->set0Resources);
        }

        NodePtr node = mGraph->addNode(NodeDesc("ssc2 upload")
                                           .setAction(Action::createFromLambda("pack+upload UBOs",
                                                                               [this, content]() {
                                                                                   uploadSnapshot(content);
                                                                                   mGraph->publishArtifact(mContentArtifact, content);
                                                                               }),
                                                      nullptr));
        if (!node) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstants2: addNode failed");
                return {};
            }

        return {mContentArtifact, version};
    }

    AutoRef<const Content> getContent(const ArtifactPtr & artifact) const override { return mGraph->getTypedArtifactContent<Content>(artifact); }

    gpu2::GpuRaster::DrawParameters getSkyboxDrawParams(const GN::gpu2::GpuResourceSet & set0Resources) const override {
        gpu2::GpuRaster::DrawParameters dp;
        dp.vs                   = mSkyboxVS;
        dp.ps                   = mSkyboxPS;
        dp.states.depthState    = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::LESS_EQUAL, false};
        dp.states.cullMode      = gpu2::RasterState::CULL_NONE;
        dp.geometry.vertexCount = 3; // fullscreen triangle, no VBO
        dp.resources.resize(1);
        dp.resources[0] = set0Resources;
        return dp;
    }

private:
    // Start async IO for env textures. Publishes v2+ to mEnvArtifact when any file loads.
    // Called only when set0.envLighting paths change, under mMutex.
    void triggerEnvLoad(const EnvLightingParameters & params) {
        bool anyPath = !params.skyboxPath.empty() || !params.irradiancePath.empty() || !params.prefilteredPath.empty() || !params.brdfLutPath.empty();
        if (!anyPath) return;

        EnvTextureSet defEnv = mEnvDefaults;

        std::thread([=, gpu = mGpu, graph = mGraph, artifact = mEnvArtifact, skyPath = params.skyboxPath, irrPath = params.irradiancePath,
                     prefilPath = params.prefilteredPath, brdfPath = params.brdfLutPath, simulate = params.simulateSlowLoading,
                     defEnv = std::move(defEnv)]() mutable {
            auto cnc = gpu2::GpuCnC::create({.gpu = gpu});
            if (!cnc) return;

            EnvTextureSet real = defEnv;
            real.uploadPayload = {};

            auto tryLoad = [&](const StrA & path, const StrA & texName, AutoRef<gpu2::Texture> & outTex) {
                if (path.empty()) return;
                auto stg = gpu2::Buffer::loadTextureToStagingBuffer(texName, gpu, path);
                if (stg.empty()) return;
                auto tex = gpu2::Texture::create(texName, {.context = gpu, .descriptor = stg.descriptor});
                if (!tex) return;
                cnc->copyBufferToImage(stg, tex);
                outTex = tex;
            };
            tryLoad(skyPath, "env.skybox", real.skyboxCubemap);
            tryLoad(irrPath, "env.irradiance", real.irradianceMap);
            tryLoad(prefilPath, "env.prefiltered", real.prefilteredEnvMap);
            tryLoad(brdfPath, "env.brdf_lut", real.brdfLut);

            auto payload = cnc->seal();
            if (!payload) return; // nothing loaded; keep v1 blue-sky defaults

            real.uploadPayload = std::move(payload);
            if (simulate) std::this_thread::sleep_for(std::chrono::seconds(5));
            graph->publishArtifact(artifact, AutoRef<EnvTextureSetContent>(new EnvTextureSetContent(std::move(real))));
        }).detach();
    }

    void buildSet0Resources(const EnvTextureSet & env, gpu2::GpuResourceSet & out) const {
        out.resize(6);

        out[0].resize(1);
        out[0][0].resource = mSceneBuffer;
        out[0][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(shader::SceneUBO));

        out[1].resize(1);
        out[1][0].resource = mCameraBuffer;
        out[1][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(shader::CameraUBO));

        auto bindTex = [&](uint32_t slot, const AutoRef<gpu2::Texture> & tex, const AutoRef<gpu2::Texture> & fallback) {
            out[slot].resize(1);
            out[slot][0].resource = tex ? tex : fallback;
            out[slot][0].setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        };

        bindTex(2, env.skyboxCubemap, mFallbackCubemap);
        bindTex(3, env.irradianceMap, mFallbackCubemap);
        bindTex(4, env.prefilteredEnvMap, mFallbackCubemap);
        bindTex(5, env.brdfLut, mFallbackBrdfLut);
    }

    void uploadSnapshot(const AutoRef<Content> & content) const {
        const Set0Parameters & snap = content->set0Parameters;

        shader::SceneUBO scene {};
        scene.frameCounter             = snap.frameConstants.frameCounter;
        scene.frameDurationMs          = (float) ((double) snap.frameConstants.frameDuration.count() * 1e-3);
        scene.environmentRadianceScale = snap.envLighting.environmentRadianceScale;

        const size_t numLights = std::min((size_t) shader::MAX_SCENE_LIGHTS, snap.directLighting.size());
        scene.numLights        = (uint32_t) numLights;
        for (uint32_t i = 0; i < numLights; ++i) {
            const DirectLight &       src = snap.directLighting[i];
            shader::DirectLightData & dst = scene.lights[i];
            switch (src.type) {
            case DirectLight::POINT: {
                dst.positionOrDir = glm::vec4(glm::vec3(src.point.position), (float) DirectLight::POINT);
                float iv          = src.point.intensity.intensity.value;
                dst.colorAndRange = glm::vec4(src.point.intensity.r * iv, src.point.intensity.g * iv, src.point.intensity.b * iv, src.point.range);
                dst.coneAngles    = glm::vec4(0.f);
                break;
            }
            case DirectLight::SPOT: {
                dst.positionOrDir = glm::vec4(glm::vec3(src.spot.position), (float) DirectLight::SPOT);
                float iv          = src.spot.intensity.intensity.value;
                dst.colorAndRange = glm::vec4(src.spot.intensity.r * iv, src.spot.intensity.g * iv, src.spot.intensity.b * iv, src.spot.range);
                dst.coneAngles    = glm::vec4(src.spot.cosInnerConeAngle, src.spot.cosOuterConeAngle, 0.f, 0.f);
                break;
            }
            case DirectLight::DIRECTIONAL: {
                glm::vec3 dir     = glm::mat3_cast(src.directional.orientation) * glm::vec3(0.f, 0.f, -1.f);
                dst.positionOrDir = glm::vec4(dir, (float) DirectLight::DIRECTIONAL);
                float iv          = src.directional.irradiance.irradiance.value;
                dst.colorAndRange = glm::vec4(src.directional.irradiance.r * iv, src.directional.irradiance.g * iv, src.directional.irradiance.b * iv, 0.f);
                dst.coneAngles    = glm::vec4(0.f);
                break;
            }
            }
        }

        shader::CameraUBO cam {};
        {
            const glm::vec3 pos        = snap.camera.cameraPosition;
            glm::mat4       camToWorld = glm::translate(glm::mat4(1.f), pos) * glm::mat4_cast(snap.camera.cameraOrientation);
            cam.viewMatrix             = glm::inverse(camToWorld);
            cam.projMatrix = glm::perspectiveRH_ZO(snap.camera.cameraFov.value, snap.camera.aspectRatio, snap.camera.nearPlane, snap.camera.farPlane);
            cam.projMatrix[1][1] *= -1.f; // Vulkan Y-inversion
            cam.projViewMatrix   = cam.projMatrix * cam.viewMatrix;
            cam.cameraPosition   = glm::vec4(pos, 1.f);
            cam.renderTargetSize = glm::vec2((float) snap.camera.viewWidthInPixel, (float) snap.camera.viewHeightInPixel);
            cam.nearPlane        = snap.camera.nearPlane;
            cam.farPlane         = snap.camera.farPlane;
        }

        auto stagingScene = gpu2::Buffer::create("ssc2.staging_scene", {.context = mGpu, .size = sizeof(shader::SceneUBO), .mappable = true});
        auto stagingCam   = gpu2::Buffer::create("ssc2.staging_cam", {.context = mGpu, .size = sizeof(shader::CameraUBO), .mappable = true});
        if (!stagingScene || !stagingCam) GN_UNLIKELY return;

        {
            auto m = stagingScene->map();
            if (!m.data()) GN_UNLIKELY return;
            memcpy(m.data(), &scene, sizeof(scene));
        }
        {
            auto m = stagingCam->map();
            if (!m.data()) GN_UNLIKELY return;
            memcpy(m.data(), &cam, sizeof(cam));
        }

        auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
        if (!cnc) GN_UNLIKELY return;
        cnc->copyBufferToBuffer({.src = stagingScene, .dst = mSceneBuffer, .size = sizeof(shader::SceneUBO)});
        cnc->copyBufferToBuffer({.src = stagingCam, .dst = mCameraBuffer, .size = sizeof(shader::CameraUBO)});
        content->set0Payloads.append(cnc->seal());
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

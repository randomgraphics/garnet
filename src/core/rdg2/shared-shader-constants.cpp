#include "pch.h"
#include "vk-shaders/scene-ubo.h"
#include "vk-shaders/camera-ubo.h"
#include "skybox-vert.spv.h"
#include "skybox-frag.spv.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <mutex>
#include <algorithm>

static GN::Logger * sLogger = GN::getLogger("GN.rdg2");

namespace GN::rdg2 {

struct EnvTextureSet {
    AutoRef<gpu2::Texture>    skyboxCubemap;
    AutoRef<gpu2::Texture>    irradianceMap;
    AutoRef<gpu2::Texture>    prefilteredEnvMap;
    AutoRef<gpu2::Texture>    brdfLut;
    AutoRef<gpu2::GpuPayload> uploadPayload;
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

    DynaArray<gpu2::GpuCnC::Region> regions;
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
    AutoRef<gpu2::Buffer>     mSceneBuffer;
    AutoRef<gpu2::Buffer>     mCameraBuffer;

    // Skybox shaders compiled once at init; referenced by every getSkyboxDrawParams() call.
    AutoRef<gpu2::GpuShader> mSkyboxVS, mSkyboxPS;

    // Fallback textures used in buildSet0Resources() when env slots are null.
    AutoRef<gpu2::Texture> mFallbackCubemap;
    AutoRef<gpu2::Texture> mFallbackBrdfLut;
    // Upload for the fallback textures; appended to set0Payloads on the first snapshot only.
    mutable AutoRef<gpu2::GpuPayload> mFallbackInitPayload;

    mutable EnvTextureSet             mEnvKnownGood;
    mutable AutoRef<gpu2::GpuPayload> mPendingEnvUploadPayload;
    mutable EnvLightingParameters     mLastEnvParams; // detect user-side path changes

    mutable std::mutex mMutex;

public:
    GN_REGISTER_RUNTIME_TYPE(SharedShaderConstants);

    SharedShaderConstants2Impl(): SharedShaderConstants(TYPE_INFO(), "SharedShaderConstants2") {}

    bool init(const CreateParameters & params) {
        if (!params.gpu) {
            GN_ERROR(sLogger, "SharedShaderConstants2: invalid create parameters");
            return false;
        }
        mGpu = params.gpu;

        mSceneBuffer  = gpu2::Buffer::create("ssc2.scene_ubo", {.context = mGpu, .size = sizeof(shader::SceneUBO)});
        mCameraBuffer = gpu2::Buffer::create("ssc2.camera_ubo", {.context = mGpu, .size = sizeof(shader::CameraUBO)});
        if (!mSceneBuffer || !mCameraBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger, "SharedShaderConstants2: failed to create UBO buffers");
                return false;
            }

        mSkyboxVS = gpu2::GpuShader::create({.context = mGpu, .name = "skybox.vert", .binary = kSkyboxVertSpv, .size = sizeof(kSkyboxVertSpv)});
        mSkyboxPS = gpu2::GpuShader::create({.context = mGpu, .name = "skybox.frag", .binary = kSkyboxFragSpv, .size = sizeof(kSkyboxFragSpv)});
        if (!mSkyboxVS || !mSkyboxPS) GN_UNLIKELY {
                GN_ERROR(sLogger, "SharedShaderConstants2: failed to compile skybox shaders");
                return false;
            }

        // Fallback textures (black cubemap + white BRDF LUT) for when env slots are null.
        {
            auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
            if (!cnc) GN_UNLIKELY return false;
            mFallbackCubemap = make1x1Texture(mGpu, *cnc, "ssc2.fallback_cubemap", 0, 0, 0, 255, 6);
            mFallbackBrdfLut = make1x1Texture(mGpu, *cnc, "ssc2.fallback_brdf_lut", 255, 255, 255, 255, 1);
            if (!mFallbackCubemap || !mFallbackBrdfLut) GN_UNLIKELY {
                    GN_ERROR(sLogger, "SharedShaderConstants2: failed to create fallback textures");
                    return false;
                }
            mFallbackInitPayload = cnc->seal();
        }

        // Create 1x1 blue-sky defaults. Path changes replace these cached pointers only after a load succeeds.
        {
            auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
            if (!cnc) GN_UNLIKELY return false;
            auto skyCubemap  = make1x1Texture(mGpu, *cnc, "ssc2.sky_default", 135, 206, 235, 255, 6);
            auto irradiance  = make1x1Texture(mGpu, *cnc, "ssc2.irr_default", 34, 68, 102, 255, 6);
            auto prefiltered = make1x1Texture(mGpu, *cnc, "ssc2.prefilter_default", 34, 68, 102, 255, 6);
            auto defBrdf     = make1x1Texture(mGpu, *cnc, "ssc2.brdf_default", 255, 255, 0, 0, 1);

            mEnvKnownGood.skyboxCubemap     = skyCubemap;
            mEnvKnownGood.irradianceMap     = irradiance;
            mEnvKnownGood.prefilteredEnvMap = prefiltered;
            mEnvKnownGood.brdfLut           = defBrdf;
            mEnvKnownGood.uploadPayload     = cnc->seal();
            mPendingEnvUploadPayload        = mEnvKnownGood.uploadPayload;
        }

        return true;
    }

    Snapshot takeSnapshot() const override {
        Snapshot       snapshot;
        Set0Parameters parameters;
        {
            std::lock_guard<std::mutex> lock(mMutex);
            parameters = set0;

            // Check for path changes and synchronously update cached known-good env textures on successful load.
            bool pathsChanged = set0.envLighting.skyboxPath != mLastEnvParams.skyboxPath || set0.envLighting.irradiancePath != mLastEnvParams.irradiancePath ||
                                set0.envLighting.prefilteredPath != mLastEnvParams.prefilteredPath ||
                                set0.envLighting.brdfLutPath != mLastEnvParams.brdfLutPath;
            if (pathsChanged) {
                loadEnvTextures_(set0.envLighting);
                mLastEnvParams = set0.envLighting;
            }

            if (mPendingEnvUploadPayload) {
                snapshot.set0Payloads.append(mPendingEnvUploadPayload);
                mPendingEnvUploadPayload = {};
            }

            // Include fallback upload only on the very first snapshot, then clear.
            if (mFallbackInitPayload) {
                snapshot.set0Payloads.append(mFallbackInitPayload);
                mFallbackInitPayload = {};
            }

            buildSet0Resources(mEnvKnownGood, snapshot.set0Resources);
        }

        uploadSnapshot(parameters, snapshot);
        return snapshot;
    }

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
    // Called only when set0.envLighting paths change, under mMutex.
    void loadEnvTextures_(const EnvLightingParameters & params) const {
        bool anyPath = !params.skyboxPath.empty() || !params.irradiancePath.empty() || !params.prefilteredPath.empty() || !params.brdfLutPath.empty();
        if (!anyPath) return;

        auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
        if (!cnc) return;

        EnvTextureSet real       = mEnvKnownGood;
        real.uploadPayload       = {};
        bool loadedAnyEnvTexture = false;

        auto tryLoad = [&](const StrA & path, const StrA & texName, AutoRef<gpu2::Texture> & outTex) {
            if (path.empty()) return;
            auto stg = gpu2::Buffer::loadTextureToStagingBuffer(texName, mGpu, path);
            if (stg.empty()) return;
            auto tex = gpu2::Texture::create(texName, {.context = mGpu, .descriptor = stg.descriptor});
            if (!tex) return;
            cnc->copyBufferToImage(stg, tex);
            outTex              = tex;
            loadedAnyEnvTexture = true;
        };
        tryLoad(params.skyboxPath, "env.skybox", real.skyboxCubemap);
        tryLoad(params.irradiancePath, "env.irradiance", real.irradianceMap);
        tryLoad(params.prefilteredPath, "env.prefiltered", real.prefilteredEnvMap);
        tryLoad(params.brdfLutPath, "env.brdf_lut", real.brdfLut);

        if (!loadedAnyEnvTexture) return;
        auto payload = cnc->seal();
        if (!payload) return;

        real.uploadPayload       = payload;
        mEnvKnownGood            = std::move(real);
        mPendingEnvUploadPayload = payload;
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

    void uploadSnapshot(const Set0Parameters & snap, Snapshot & snapshot) const {
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
        snapshot.set0Payloads.append(cnc->seal());
    }
};

GN_API AutoRef<SharedShaderConstants> SharedShaderConstants::create(const CreateParameters & params) {
    auto impl = AutoRef<SharedShaderConstants2Impl>(new SharedShaderConstants2Impl());
    if (!impl->init(params)) {
        GN_ERROR(sLogger, "SharedShaderConstants::create failed");
        return {};
    }
    return impl;
}

} // namespace GN::rdg2

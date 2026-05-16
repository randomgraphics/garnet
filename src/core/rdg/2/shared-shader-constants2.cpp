#include <garnet/GNrdg2.h>
#include "vk-shaders/scene-ubo.h"
#include "vk-shaders/camera-ubo.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <mutex>
#include <unordered_map>
#include <algorithm>

static GN::Logger * sLogger = GN::getLogger("GN.rdg2");

namespace GN::rdg2 {

// Deduce render-target pixel dimensions from the first valid attachment.
// Falls back to 1×1 when no attachment is set (avoids a zero-size projection).
static std::pair<uint32_t, uint32_t> getRenderTargetSize(const gpu2::RasterTarget & rt) {
    for (const auto & c : rt.colorTargets) {
        if (auto tex = c.target.texture()) return {tex->descriptor().width, tex->descriptor().height};
    }
    if (auto tex = rt.depthStencilTarget.texture()) return {tex->descriptor().width, tex->descriptor().height};
    return {1u, 1u};
}

// Create a solid-color 1×1 texture. Used for fallback env-map slots so shaders always
// have a valid binding even when the caller hasn't provided real IBL textures.
static AutoRef<gpu2::Texture> make1x1Texture(AutoRef<gpu2::GpuContext> gpu, const StrA & name, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint32_t faces) {
    gpu2::Texture::Descriptor desc;
    desc.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(faces).setLevels(1);
    auto tex = gpu2::Texture::create(name, {.context = gpu, .descriptor = desc});
    if (!tex) GN_UNLIKELY return {};

    gfx::img::ImageDesc imgDesc;
    if (faces == 6)
        imgDesc.setCube(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM(), 1, 1);
    else
        imgDesc.set2D(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM(), 1, 1, 1);

    gfx::img::Image img(imgDesc);
    for (size_t f = 0; f < faces; ++f) {
        uint8_t * p = img.at(gfx::img::PlaneCoord {0, f, 0}, 0, 0);
        p[0]        = r;
        p[1]        = g;
        p[2]        = b;
        p[3]        = a;
    }
    if (!tex->setContent(img)) GN_UNLIKELY {
            GN_ERROR(sLogger)("make1x1Texture: setContent failed for '{}'", name);
            return {};
        }
    return tex;
}

class SharedShaderConstants2Impl : public SharedShaderConstants {
    AutoRef<gpu2::GpuContext> mGpu;
    AutoRef<Graph>            mGraph;
    AutoRef<gpu2::Buffer>     mSceneBuffer;
    AutoRef<gpu2::Buffer>     mCameraBuffer;
    AutoRef<gpu2::Texture>    mFallbackCubemap;
    AutoRef<gpu2::Texture>    mFallbackBrdfLut;

    mutable std::mutex                                     mMutex;
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

        mSceneBuffer  = gpu2::Buffer::create("ssc2.scene_ubo", {.context = mGpu, .size = sizeof(shader::SceneUBO)});
        mCameraBuffer = gpu2::Buffer::create("ssc2.camera_ubo", {.context = mGpu, .size = sizeof(shader::CameraUBO)});
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

        // Populate fallbacks so callers that never set IBL textures still get a valid binding.
        set0.environmentLighting.skyboxCubemap     = mFallbackCubemap;
        set0.environmentLighting.irradianceMap     = mFallbackCubemap;
        set0.environmentLighting.prefilteredEnvMap = mFallbackCubemap;
        set0.environmentLighting.brdfLut           = mFallbackBrdfLut;

        return true;
    }

    TokenPtr takeSnapshot() const override {
        // Evict entries whose upload tokens are already satisfied. Safe to drop because
        // the single-worker graph and the caller's waitForToken(renderNode) guarantee
        // that all render nodes have consumed getContent() before the next takeSnapshot()
        // is called, so no live ref to the evicted Content can remain outside this map.
        {
            std::lock_guard<std::mutex> lock(mMutex);
            for (auto it = mContentMap.begin(); it != mContentMap.end();) {
                if (mGraph->waitForToken(it->first) == Graph::WaitResult::IDLE)
                    it = mContentMap.erase(it);
                else
                    ++it;
            }
        }

        // Allocate content first so we can copy set0 directly into it under the lock,
        // avoiding the extra copy that a local Set0Parameters variable would require.
        auto content = AutoRef<Content>(new Content());
        {
            std::lock_guard<std::mutex> lock(mMutex);
            content->set0Parameters = set0;
        }
        buildSet0Resources(content->set0Parameters, content->set0Resources);

        // Lambda captures only content (not a redundant snap copy); uploadSnapshot reads
        // content->set0Parameters directly.
        NodePtr node = mGraph->addNode(
            NodeDesc("ssc2 upload")
                .setAction(Action::createFromLambda("pack + upload scene/camera UBOs", [this, content]() { uploadSnapshot(content); }), nullptr));
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
        auto                        it = mContentMap.find(token);
        if (it == mContentMap.end()) return {};
        return it->second;
    }

private:
    // Populate the GpuResourceSet that shaders will bind as set 0:
    //   binding 0 — scene UBO
    //   binding 1 — camera UBO
    //   bindings 2-5 — env textures (skybox, irradiance, prefiltered, BRDF LUT)
    void buildSet0Resources(const Set0Parameters & snap, gpu2::GpuResourceSet & out) const {
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

        const auto & env = snap.environmentLighting;
        bindTex(2, env.skyboxCubemap, mFallbackCubemap);
        bindTex(3, env.irradianceMap, mFallbackCubemap);
        bindTex(4, env.prefilteredEnvMap, mFallbackCubemap);
        bindTex(5, env.brdfLut, mFallbackBrdfLut);
    }

    void uploadSnapshot(const AutoRef<Content> & content) const {
        const Set0Parameters & snap = content->set0Parameters;

        // --- Pack SceneUBO ---
        shader::SceneUBO scene {};
        scene.frameCounter             = snap.frameConstants.frameCounter;
        scene.frameDurationMs          = (float) ((double) snap.frameConstants.frameDuration.count() * 1e-3);
        scene.environmentRadianceScale = snap.environmentLighting.environmentRadianceScale;

        const size_t numLights = std::min((size_t) shader::MAX_SCENE_LIGHTS, (size_t) snap.directLighting.size());
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
                // Rotate the canonical -Z forward vector by the orientation quaternion to get the light direction.
                glm::vec3 dir     = glm::mat3_cast(src.directional.orientation) * glm::vec3(0.f, 0.f, -1.f);
                dst.positionOrDir = glm::vec4(dir, (float) DirectLight::DIRECTIONAL);
                float iv          = src.directional.irradiance.irradiance.value;
                dst.colorAndRange = glm::vec4(src.directional.irradiance.r * iv, src.directional.irradiance.g * iv, src.directional.irradiance.b * iv, 0.f);
                dst.coneAngles    = glm::vec4(0.f);
                break;
            }
            }
        }

        // --- Pack CameraUBO ---
        shader::CameraUBO cam {};
        {
            const glm::vec3 pos        = snap.camera.cameraPosition;
            glm::mat4       camToWorld = glm::translate(glm::mat4(1.f), pos) * glm::mat4_cast(snap.camera.cameraOrientation);
            cam.viewMatrix             = glm::inverse(camToWorld);
            cam.projMatrix = glm::perspectiveRH_ZO(snap.camera.cameraFov.value, snap.camera.aspectRatio, snap.camera.nearPlane, snap.camera.farPlane);
            // Vulkan clip-space has Y pointing down; negate proj[1][1] to match the convention expected by shaders.
            cam.projMatrix[1][1] *= -1.f;
            cam.projViewMatrix   = cam.projMatrix * cam.viewMatrix;
            cam.cameraPosition   = glm::vec4(pos, 1.f);
            auto [w, h]          = getRenderTargetSize(snap.renderTarget);
            cam.renderTargetSize = glm::vec2((float) w, (float) h);
            cam.nearPlane        = snap.camera.nearPlane;
            cam.farPlane         = snap.camera.farPlane;
        }

        // --- Create per-upload staging buffers and write packed data ---
        // Mappable (host-visible) buffers are slower for the GPU, but ideal for one-shot CPU→GPU uploads.
        auto stagingScene = gpu2::Buffer::create("ssc2.staging_scene", {.context = mGpu, .size = sizeof(shader::SceneUBO), .mappable = true});
        auto stagingCam   = gpu2::Buffer::create("ssc2.staging_cam", {.context = mGpu, .size = sizeof(shader::CameraUBO), .mappable = true});
        if (!stagingScene || !stagingCam) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstants2: failed to create staging buffers");
                return;
            }

        // Use braces to scope each Mapped object so it unmaps before the GPU copy.
        {
            auto m = stagingScene->map();
            if (!m.data()) GN_UNLIKELY {
                    GN_ERROR(sLogger)("ssc2: scene staging map failed");
                    return;
                }
            memcpy(m.data(), &scene, sizeof(scene));
        }
        {
            auto m = stagingCam->map();
            if (!m.data()) GN_UNLIKELY {
                    GN_ERROR(sLogger)("ssc2: camera staging map failed");
                    return;
                }
            memcpy(m.data(), &cam, sizeof(cam));
        }

        // --- Record GPU copies and seal ---
        // GpuCncPayloadVulkan stores AutoRef<Buffer> for each src op, so staging buffers
        // remain alive until the payload is released (after GPU execution).
        auto cnc = gpu2::GpuCnC::create({.gpu = mGpu});
        if (!cnc) GN_UNLIKELY {
                GN_ERROR(sLogger)("ssc2: GpuCnC create failed");
                return;
            }
        cnc->copyBufferToBuffer({.src = stagingScene, .dst = mSceneBuffer, .size = sizeof(shader::SceneUBO)});
        cnc->copyBufferToBuffer({.src = stagingCam, .dst = mCameraBuffer, .size = sizeof(shader::CameraUBO)});
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

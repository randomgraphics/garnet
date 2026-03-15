#include "pch.h"
#include <garnet/GNrdg.h>
#include "gpu-context.h"
#include "vk-gpu-context.h"
#include "vk-shaders/global-camera-ubo.h"
#include "vk-shaders/direct-lighting-ubo.h"
#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::inverse
#include <glm/gtc/quaternion.hpp>        // glm::mat4_cast
#include <glm/ext/matrix_clip_space.hpp> // glm::perspectiveRH_ZO

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// SharedShaderConstantsVulkan — uses GpuResourceTable (Set 0 views) + PersistentBuffer.
// =============================================================================
//
// Set 0 layout: binding 0 = GlobalCameraUBO, binding 1 = DirectLightingUBO.
// Effect builders get Set 0 via getSet0Resources() and assign to draw args' resources[0].
// draw args' resources[0][0] and resources[0][1]; backend binds via Drawable/DrawPack.
//
// Deduce render target dimensions from the first available color or depth attachment.
static std::pair<uint32_t, uint32_t> getRenderTargetSize(const RenderTarget * rt) {
    if (!rt) return {1u, 1u};
    for (const auto & c : rt->colors) {
        if (auto tex = c.target.texture()) return {tex->descriptor().width, tex->descriptor().height};
        if (auto bb = c.target.backbuffer()) return {bb->descriptor().width, bb->descriptor().height};
    }
    if (auto tex = rt->depthStencilTarget.texture()) return {tex->descriptor().width, tex->descriptor().height};
    return {1u, 1u};
}

class SharedShaderConstantsVulkan : public SharedShaderConstants {
    AutoRef<GpuContext>       mGpu;
    FrameInformation          mFrame;
    ViewInformation           mView;
    DirectLightingInformation mLighting;

    AutoRef<TransientArena>   mArena;          ///< Arena for per-frame transient UBO staging
    AutoRef<GpuCopy>          mCopyAction;     ///< Stateless buffer-to-buffer copy (reused for camera + lighting)
    AutoRef<PersistentBuffer> mCameraBuffer;   ///< Set 0 binding 0: GlobalCameraUBO
    AutoRef<PersistentBuffer> mLightingBuffer; ///< Set 0 binding 1: DirectLightingUBO

    Set0ResourceSet mLastSet0; ///< Last built Set 0 resource set (for getSet0Resources()).

    GlobalCameraUBO   mPendingCamera   = {};
    DirectLightingUBO mPendingLighting = {};

    void initGpuResources() {
        mArena = TransientArena::create(StrA::format("{}.arena", name), TransientArena::CreateParameters {.context = mGpu});
        if (!mArena) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create transient arena");
                return;
            }

        mCopyAction = GpuCopy::create(StrA::format("{}.copy_ubo", name), GpuCopy::CreateParameters {.gpu = mGpu});
        if (!mCopyAction) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create copy action");
                return;
            }

        PersistentBuffer::CreateParameters bufParams;
        bufParams.context = mGpu;
        bufParams.size    = sizeof(GlobalCameraUBO);
        mCameraBuffer     = PersistentBuffer::create(StrA::format("{}.camera_ubo", name), bufParams);
        if (!mCameraBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create camera UBO");
                return;
            }

        bufParams.size  = sizeof(DirectLightingUBO);
        mLightingBuffer = PersistentBuffer::create(StrA::format("{}.lighting_ubo", name), bufParams);
        if (!mLightingBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create lighting UBO");
                return;
            }
    }

    void packCameraUbo() {
        mPendingCamera             = {};
        const glm::vec3 camPos     = mView.cameraPosition;
        glm::mat4       camToWorld = glm::translate(glm::mat4(1.f), camPos) * glm::mat4_cast(mView.cameraOrientation);
        mPendingCamera.viewMatrix  = glm::inverse(camToWorld);

        mPendingCamera.projMatrix = glm::perspectiveRH_ZO(mView.cameraFov.value, mView.aspectRatio, mView.nearPlane, mView.farPlane);
        mPendingCamera.projMatrix[1][1] *= -1.f;

        mPendingCamera.viewProjMatrix   = mPendingCamera.projMatrix * mPendingCamera.viewMatrix;
        mPendingCamera.cameraPosition   = glm::vec4(camPos, 1.f);
        auto [rtW, rtH]                 = getRenderTargetSize(mView.renderTarget.get());
        mPendingCamera.renderTargetSize = glm::vec2((float) rtW, (float) rtH);
        mPendingCamera.nearPlane        = mView.nearPlane;
        mPendingCamera.farPlane         = mView.farPlane;
        mPendingCamera.frameCounter     = mFrame.frameCounter;
        mPendingCamera.frameDurationMs  = (float) ((double) mFrame.frameDuration.count() * 1e-3); // µs → ms
    }

    void packLightingUbo() {
        mPendingLighting = {};

        const auto &   lights      = mLighting.lights;
        const uint32_t numLights   = (uint32_t) std::min((size_t) MAX_DIRECT_LIGHTS, lights.size());
        mPendingLighting.numLights = numLights;

        for (uint32_t i = 0; i < numLights; ++i) {
            const DirectLight & src = lights[i];
            DirectLightData &   dst = mPendingLighting.lights[i];

            switch (src.type) {
            case DirectLight::POINT: {
                glm::vec3 pos     = src.point.position;
                dst.positionOrDir = glm::vec4(pos, (float) LIGHT_TYPE_POINT);
                float intensity   = src.point.intensity.intensity.value;
                dst.colorAndRange =
                    glm::vec4(src.point.intensity.r * intensity, src.point.intensity.g * intensity, src.point.intensity.b * intensity, src.point.range);
                dst.coneAngles = glm::vec4(0.f, 0.f, 0.f, 0.f);
                break;
            }
            case DirectLight::SPOT: {
                glm::vec3 pos     = src.spot.position;
                dst.positionOrDir = glm::vec4(pos, (float) LIGHT_TYPE_SPOT);
                float intensity   = src.spot.intensity.intensity.value;
                dst.colorAndRange =
                    glm::vec4(src.spot.intensity.r * intensity, src.spot.intensity.g * intensity, src.spot.intensity.b * intensity, src.spot.range);
                dst.coneAngles = glm::vec4(src.spot.cosInnerConeAngle, src.spot.cosOuterConeAngle, 0.f, 0.f);
                break;
            }
            case DirectLight::DIRECTIONAL: {
                glm::vec3 dir     = glm::mat3_cast(src.directional.orientation) * glm::vec3(0.f, 0.f, -1.f);
                dst.positionOrDir = glm::vec4(dir, (float) LIGHT_TYPE_DIRECTIONAL);
                float irradiance  = src.directional.irradiance.irradiance.value;
                dst.colorAndRange = glm::vec4(src.directional.irradiance.r * irradiance, src.directional.irradiance.g * irradiance,
                                              src.directional.irradiance.b * irradiance, 0.f);
                dst.coneAngles    = glm::vec4(0.f, 0.f, 0.f, 0.f);
                break;
            }
            }
        }
    }

public:
    SharedShaderConstantsVulkan(const StrA & name, AutoRef<GpuContext> gpu): SharedShaderConstants(TYPE_INFO(), name), mGpu(std::move(gpu)) {
        initGpuResources();
    }

    GpuContext & gpu() const override { return *mGpu; }

    void setFrameInformation(const FrameInformation & v) override {
        mFrame = v;
        packCameraUbo(); // frame counter + duration live in GlobalCameraUBO
    }

    void setViewInformation(const ViewInformation & v) override {
        mView = v;
        packCameraUbo();
    }

    void setDirectLightingInformation(const DirectLightingInformation & v) override {
        mLighting = v;
        packLightingUbo();
    }

    const FrameInformation &          getFrameInformation() const override { return mFrame; }
    const ViewInformation &           getViewInformation() const override { return mView; }
    const DirectLightingInformation & getDirectLightingInformation() const override { return mLighting; }

    SubGraph build(RenderGraph & rg) override {
        SubGraph sg(rg, StrA::format("{}.upload", name));
        if (!mArena || !mCopyAction || !mCameraBuffer || !mLightingBuffer) return sg;

        // Allocate transient staging buffers and write CPU snapshot (no setContent; use arena + copy).
        AutoRef<TransientBuffer> tbCamera   = mArena->allocate(sizeof(GlobalCameraUBO), "camera");
        AutoRef<TransientBuffer> tbLighting = mArena->allocate(sizeof(DirectLightingUBO), "lighting");
        if (!tbCamera || !tbLighting) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan::build: transient allocate failed");
                return sg;
            }
        {
            auto m = tbCamera->map();
            if (!m.data() || m.size() < sizeof(GlobalCameraUBO)) GN_UNLIKELY {
                    GN_ERROR(sLogger)("SharedShaderConstantsVulkan::build: camera map failed");
                    return sg;
                }
            memcpy(m.data(), &mPendingCamera, sizeof(mPendingCamera));
        }
        {
            auto m = tbLighting->map();
            if (!m.data() || m.size() < sizeof(DirectLightingUBO)) GN_UNLIKELY {
                    GN_ERROR(sLogger)("SharedShaderConstantsVulkan::build: lighting map failed");
                    return sg;
                }
            memcpy(m.data(), &mPendingLighting, sizeof(mPendingLighting));
        }

        // Copy transient → persistent; workflow keeps transient refs alive until submit completes.
        auto wf         = rg.createWorkflow(sg.name);
        auto camArgs    = AutoRef<GpuCopy::BufferToBuffer>(new GpuCopy::BufferToBuffer());
        camArgs->src    = tbCamera;
        camArgs->dst    = mCameraBuffer;
        camArgs->size   = sizeof(GlobalCameraUBO);
        auto lightArgs  = AutoRef<GpuCopy::BufferToBuffer>(new GpuCopy::BufferToBuffer());
        lightArgs->src  = tbLighting;
        lightArgs->dst  = mLightingBuffer;
        lightArgs->size = sizeof(DirectLightingUBO);
        wf.appendTask("camera", mCopyAction, std::move(camArgs));
        wf.appendTask("lighting", mCopyAction, std::move(lightArgs));
        sg.workflows.append(std::move(wf));

        // Build Set 0 resource set (camera + lighting; expandable later) for effects to use as resources[0].
        mLastSet0.resize(2);
        mLastSet0[0].resize(1);
        if (mCameraBuffer)
            mLastSet0[0][0] = GpuResourceView {}
                                  .setArtifact(mCameraBuffer)
                                  .setBufferViewType(GpuResourceView::BufferView::Type::UNIFORM)
                                  .setBufferViewOffset(0)
                                  .setBufferViewSize(sizeof(GlobalCameraUBO));
        mLastSet0[1].resize(1);
        if (mLightingBuffer)
            mLastSet0[1][0] = GpuResourceView {}
                                  .setArtifact(mLightingBuffer)
                                  .setBufferViewType(GpuResourceView::BufferView::Type::UNIFORM)
                                  .setBufferViewOffset(0)
                                  .setBufferViewSize(sizeof(DirectLightingUBO));

        return sg;
    }

    const Set0ResourceSet & getSet0Resources() const override { return mLastSet0; }
};

// =============================================================================
// SharedShaderConstants::create() - API-neutral dispatch
// =============================================================================

GN_API AutoRef<SharedShaderConstants> SharedShaderConstants::create(const StrA & name, const CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("SharedShaderConstants::create: gpu is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan: {
        return AutoRef<SharedShaderConstants>(new SharedShaderConstantsVulkan(name, params.gpu));
    }
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("SharedShaderConstants::create: D3D12 backend not implemented");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("SharedShaderConstants::create: Metal backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger)("SharedShaderConstants::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg

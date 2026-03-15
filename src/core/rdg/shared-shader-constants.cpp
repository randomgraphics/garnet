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
// SharedShaderConstantsVulkan — commented out for now; to be restored later.
// =============================================================================
#if 0
// SharedShaderConstantsVulkan
//
// Vulkan GPU resource layout for Set 0 (shared per-frame constants):
//   Binding 0: GlobalCameraUBO   (std140, vertex + fragment)
//   Binding 1: DirectLightingUBO (std140, vertex + fragment)
//
// CPU/GPU sync: UBO buffers use host-coherent memory and are updated
// synchronously before draw submission.  Proper frame-in-flight ring
// buffering is deferred to a later phase.
//
// Deduce render target dimensions from the first available color or depth
// attachment.  Returns {1, 1} when no attachment is present.
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

    AutoRef<PersistentBuffer>  mCameraUniforms;   ///< uniform buffer for camera info
    AutoRef<PersistentBuffer>  mLightingUinforms; ///< uniform buffer for lighting info
    AutoRef<GpuResourceGroup>  mSet0Group;        ///< Descriptor set for Set 0 (camera + lighting)

    GlobalCameraUBO   mPendingCamera   = {}; ///< CPU-side snapshot built by set*Information()
    DirectLightingUBO mPendingLighting = {}; ///< CPU-side snapshot built by set*Information()

    void initGpuResources() {
        GpuBufferUpload::CreateParameters uploadParams;
        uploadParams.gpu       = mGpu;
        uploadParams.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
        uploadParams.ringSlots = 2;

        uploadParams.size = sizeof(GlobalCameraUBO);
        mCameraUpload     = GpuBufferUpload::create(database, StrA::format("{}.camera_upload", name), uploadParams);
        if (!mCameraUpload) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create camera upload action");
                return;
            }

        uploadParams.size = sizeof(DirectLightingUBO);
        mLightingUpload   = GpuBufferUpload::create(database, StrA::format("{}.lighting_upload", name), uploadParams);
        if (!mLightingUpload) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create lighting upload action");
                return;
            }

        // Set 0 descriptor group: slot 0 = camera UBO, slot 1 = lighting UBO
        const GpuShaderStageFlags          vsAndPs(static_cast<uint32_t>(GpuShaderStageBits::VERTEX) | static_cast<uint32_t>(GpuShaderStageBits::PIXEL));
        GpuResourceGroup::CreateParameters grpParams;
        grpParams.context = mGpu;
        grpParams.slots   = {
            GpuResourceGroup::SlotDescription {.type = GpuResourceGroup::SlotDescription::UNIFORM_BUFFER, .count = 1, .stages = vsAndPs},
            GpuResourceGroup::SlotDescription {.type = GpuResourceGroup::SlotDescription::UNIFORM_BUFFER, .count = 1, .stages = vsAndPs},
        };
        mSet0Group = GpuResourceGroup::create(database, StrA::format("{}.set0", name), grpParams);
        if (!mSet0Group) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create Set 0 resource group");
                return;
            }

        // Point descriptors at slot 0 of each upload action (initial state before first build()).
        auto cameraView = mCameraUpload->currentBufferView();
        mSet0Group->setResourceViews(0, 0, SafeArrayAccessor<const GpuResourceView>(&cameraView, 1));

        auto lightingView = mLightingUpload->currentBufferView();
        mSet0Group->setResourceViews(1, 0, SafeArrayAccessor<const GpuResourceView>(&lightingView, 1));
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
    SharedShaderConstantsVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContext> gpu)
        : SharedShaderConstants(db, TYPE_INFO(), name), mGpu(std::move(gpu)) {
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

    Workflow * build(RenderGraph & rg) override {
        if (!mCameraUpload || !mLightingUpload || !mSet0Group) return nullptr;

        // Pre-bind descriptors to the slots that will be written by execute().
        // nextBufferView() predicts which ring slot execute() will write to next,
        // ensuring the descriptor set and the upload stay in sync.
        auto nextCameraView = mCameraUpload->nextBufferView();
        if (!nextCameraView.empty()) mSet0Group->setResourceViews(0, 0, SafeArrayAccessor<const GpuResourceView>(&nextCameraView, 1));

        auto nextLightingView = mLightingUpload->nextBufferView();
        if (!nextLightingView.empty()) mSet0Group->setResourceViews(1, 0, SafeArrayAccessor<const GpuResourceView>(&nextLightingView, 1));

        auto wf = rg.createWorkflow(StrA::format("{}.upload", name));
        wf->tasks.append(Workflow::Task("camera", mCameraUpload, GpuBufferUpload::A::make(&mPendingCamera, sizeof(mPendingCamera))));
        wf->tasks.append(Workflow::Task("lighting", mLightingUpload, GpuBufferUpload::A::make(&mPendingLighting, sizeof(mPendingLighting))));
        return wf;
    }

    /// Returns the Set 0 resource group (camera + lighting UBOs).
    /// Intended for backend use only (e.g. PbrShadingVulkan).
    GpuResourceGroup * getSet0Group() const { return mSet0Group.get(); }
};
#endif

// =============================================================================
// SharedShaderConstants::create() - API-neutral dispatch
// =============================================================================

GN_API AutoRef<SharedShaderConstants> SharedShaderConstants::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    (void) db;
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("SharedShaderConstants::create: gpu is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        GN_ERROR(sLogger)("SharedShaderConstants::create: Vulkan backend (SharedShaderConstantsVulkan) temporarily disabled, name='{}'", name);
        return {};
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

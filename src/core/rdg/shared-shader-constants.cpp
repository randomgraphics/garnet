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
// SharedShaderConstantsVulkan
//
// Vulkan GPU resource layout for Set 0 (shared per-frame constants):
//   Binding 0: GlobalCameraUBO   (std140, vertex + fragment)
//   Binding 1: DirectLightingUBO (std140, vertex + fragment)
//
// CPU/GPU sync: UBO buffers use host-coherent memory and are updated
// synchronously before draw submission.  Proper frame-in-flight ring
// buffering is deferred to a later phase.
// =============================================================================

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

    AutoRef<Buffer>           mCameraBuffer;   ///< GPU buffer for GlobalCameraUBO (Set 0, binding 0)
    AutoRef<Buffer>           mLightingBuffer; ///< GPU buffer for DirectLightingUBO (Set 0, binding 1)
    AutoRef<GpuResourceGroup> mSet0Group;      ///< Descriptor set for Set 0 (camera + lighting)

    void initGpuResources() {
        // Camera UBO buffer
        mCameraBuffer =
            Buffer::create(database, StrA::format("{}.camera_ubo", name), Buffer::CreateParameters {.context = mGpu, .size = sizeof(GlobalCameraUBO)});
        if (!mCameraBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create camera UBO buffer");
                return;
            }

        // Lighting UBO buffer
        mLightingBuffer =
            Buffer::create(database, StrA::format("{}.lighting_ubo", name), Buffer::CreateParameters {.context = mGpu, .size = sizeof(DirectLightingUBO)});
        if (!mLightingBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstantsVulkan: failed to create lighting UBO buffer");
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

        // Bind camera buffer to slot 0, lighting buffer to slot 1
        GpuResourceView cameraView;
        cameraView.setArtifact(mCameraBuffer)
            .setBufferView(GpuResourceView::BufferView {.type = GpuResourceView::BufferView::UNIFORM, .offset = 0, .size = sizeof(GlobalCameraUBO)});
        mSet0Group->setResourceViews(0, 0, SafeArrayAccessor<const GpuResourceView>(&cameraView, 1));

        GpuResourceView lightingView;
        lightingView.setArtifact(mLightingBuffer)
            .setBufferView(GpuResourceView::BufferView {.type = GpuResourceView::BufferView::UNIFORM, .offset = 0, .size = sizeof(DirectLightingUBO)});
        mSet0Group->setResourceViews(1, 0, SafeArrayAccessor<const GpuResourceView>(&lightingView, 1));
    }

    void uploadCameraUbo() {
        if (!mCameraBuffer) return;
        GlobalCameraUBO ubo {};

        // Compute view matrix: camera world → view = inverse(camera TRS)
        const glm::vec3 camPos     = mView.cameraPosition; // Location is glm::vec3 in meters
        glm::mat4       camToWorld = glm::translate(glm::mat4(1.f), camPos) * glm::mat4_cast(mView.cameraOrientation);
        ubo.viewMatrix             = glm::inverse(camToWorld);

        // Compute projection matrix (RH, [0,1] depth, Vulkan Y-down flip)
        ubo.projMatrix = glm::perspectiveRH_ZO(mView.cameraFov.value, mView.aspectRatio, mView.nearPlane, mView.farPlane);
        ubo.projMatrix[1][1] *= -1.f;

        ubo.viewProjMatrix   = ubo.projMatrix * ubo.viewMatrix;
        ubo.cameraPosition   = glm::vec4(camPos, 1.f);
        auto [rtW, rtH]      = getRenderTargetSize(mView.renderTarget.get());
        ubo.renderTargetSize = glm::vec2((float) rtW, (float) rtH);
        ubo.nearPlane        = mView.nearPlane;
        ubo.farPlane         = mView.farPlane;
        ubo.frameCounter     = mFrame.frameCounter;
        ubo.frameDurationMs  = (float) ((double) mFrame.frameDuration.count() * 1e-3); // µs → ms

        mCameraBuffer->setContent(&ubo, sizeof(ubo));
    }

    void uploadLightingUbo() {
        if (!mLightingBuffer) return;
        DirectLightingUBO ubo {};

        const auto &   lights    = mLighting.lights;
        const uint32_t numLights = (uint32_t) std::min((size_t) MAX_DIRECT_LIGHTS, lights.size());
        ubo.numLights            = numLights;

        for (uint32_t i = 0; i < numLights; ++i) {
            const DirectLight & src = lights[i];
            DirectLightData &   dst = ubo.lights[i];

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
                // Direction derived from orientation: camera looks down -Z in its local space
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

        mLightingBuffer->setContent(&ubo, sizeof(ubo));
    }

public:
    SharedShaderConstantsVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContext> gpu)
        : SharedShaderConstants(db, TYPE_INFO(), name), mGpu(std::move(gpu)) {
        initGpuResources();
    }

    GpuContext & gpu() const override { return *mGpu; }

    void setFrameInformation(const FrameInformation & v) override {
        mFrame = v;
        uploadCameraUbo(); // frame counter + duration live in GlobalCameraUBO
    }

    void setViewInformation(const ViewInformation & v) override {
        mView = v;
        uploadCameraUbo();
    }

    void setDirectLightingInformation(const DirectLightingInformation & v) override {
        mLighting = v;
        uploadLightingUbo();
    }

    const FrameInformation &          getFrameInformation() const override { return mFrame; }
    const ViewInformation &           getViewInformation() const override { return mView; }
    const DirectLightingInformation & getDirectLightingInformation() const override { return mLighting; }

    /// Returns the Set 0 resource group (camera + lighting UBOs).
    /// Intended for backend use only (e.g. PbrShadingVulkan).
    GpuResourceGroup * getSet0Group() const { return mSet0Group.get(); }
};

// =============================================================================
// SharedShaderConstants::create() - API-neutral dispatch
// =============================================================================

GN_API AutoRef<SharedShaderConstants> SharedShaderConstants::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("SharedShaderConstants::create: gpu is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan: {
        auto * p = new SharedShaderConstantsVulkan(db, name, params.gpu);
        if (p->sequence == 0) GN_UNLIKELY {
                GN_ERROR(sLogger)("SharedShaderConstants::create: duplicate type+name, name='{}'", name);
                delete p;
                return {};
            }
        return AutoRef<SharedShaderConstants>(p);
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

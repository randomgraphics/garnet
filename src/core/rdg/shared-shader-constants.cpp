#include "pch.h"
#include <garnet/GNrdg.h>
#include "gpu-context.h"
#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// SharedShaderConstantsImpl - stores logical data (Task 4.2)
// =============================================================================

class SharedShaderConstantsImpl : public SharedShaderConstants {
    AutoRef<GpuContext>       mGpu;
    FrameInformation          mFrame;
    ViewInformation           mView;
    DirectLightingInformation mLighting;

public:
    SharedShaderConstantsImpl(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContext> gpu)
        : SharedShaderConstants(db, TYPE_ID, TYPE_NAME, name), mGpu(std::move(gpu)) {}

    GpuContext & gpu() const override { return *mGpu; }

    void setFrameInformation(const FrameInformation & v) override { mFrame = v; }
    void setViewInformation(const ViewInformation & v) override { mView = v; }
    void setDirectLightingInformation(const DirectLightingInformation & v) override { mLighting = v; }

    const FrameInformation &          getFrameInformation() const override { return mFrame; }
    const ViewInformation &           getViewInformation() const override { return mView; }
    const DirectLightingInformation & getDirectLightingInformation() const override { return mLighting; }
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
        auto * p = new SharedShaderConstantsImpl(db, name, params.gpu);
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

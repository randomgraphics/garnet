#include "pch.h"
#include "vk-gpu-context.h"
#include "vk-texture.h"
#include "skybox-vert.spv.h"
#include "skybox-frag.spv.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// SkyBoxVulkan
// =============================================================================

class SkyBoxVulkan : public SkyBox {
    AutoRef<GpuContext>      mGpu;
    mutable AutoRef<GpuDraw> mDrawAction;

    void ensureDrawAction() const {
        if (mDrawAction) return;
        GpuDraw::CreateParameters cp;
        cp.context = mGpu;
        cp.vs      = {.binary = (void *) kSkyboxVertSpv, .size = kSkyboxVertSpvSize * sizeof(unsigned int), .entry = "main"};
        cp.ps      = {.binary = (void *) kSkyboxFragSpv, .size = kSkyboxFragSpvSize * sizeof(unsigned int), .entry = "main"};
        StrA drawName = name.empty() ? "skybox_draw"_s : StrA::format("skybox_draw_{}", name);
        mDrawAction   = GpuDraw::create(drawName, cp);
        GN_REQUIRE(mDrawAction, fmt::format("Failed to create SkyBox draw action, name='{}'", drawName));
    }

public:
    SkyBoxVulkan(const StrA & n, AutoRef<GpuContext> gpu): SkyBox(TYPE_INFO(), n), mGpu(std::move(gpu)) {}

    GpuContext & gpu() const override { return *mGpu; }

    SubGraph build(const BuildParameters & params) override {
        if (!params.renderGraph) GN_UNLIKELY {
                SubGraph sg;
                sg.builtResult = Action::ExecutionResult::PASSED;
                return sg;
            }

        Workflow workflow = params.renderGraph->createWorkflow("Skybox");
        if (!workflow) GN_UNLIKELY {
                SubGraph sg(*params.renderGraph, "Skybox");
                sg.builtResult = Action::ExecutionResult::FAILED;
                return sg;
            }

        ensureDrawAction();
        if (!mDrawAction) GN_UNLIKELY {
                SubGraph sg(*params.renderGraph, "Skybox");
                sg.builtResult = Action::ExecutionResult::FAILED;
                return sg;
            }

        auto drawArgs = AutoRef<GpuDraw::A>(new GpuDraw::A());

        // Render target from SharedShaderConstants.ViewInformation.
        if (params.sharedShaderConstants) drawArgs->renderTarget = params.sharedShaderConstants->getViewInformation().renderTarget;

        // Set 0: shared camera + lighting + environment bindings from SSC.
        if (params.sharedShaderConstants) {
            const auto & set0 = params.sharedShaderConstants->getSet0Resources();
            if (!set0.empty()) {
                drawArgs->resources.resize(1);
                drawArgs->resources[0] = set0;
            }
        }

        // Full-screen triangle: 3 vertices, no vertex buffer. Depth write disabled by depth = 1.0 in vertex shader.
        drawArgs->geometry.vertexCount = 3;

        workflow.appendTask("Skybox draw", AutoRef<Action>(mDrawAction), std::move(drawArgs));

        SubGraph sg(*params.renderGraph, "Skybox");
        sg.workflows.append(std::move(workflow));
        sg.builtResult = Action::ExecutionResult::PASSED;
        return sg;
    }
};

// =============================================================================
// SkyBox::create()
// =============================================================================

GN_API AutoRef<SkyBox> SkyBox::create(const StrA & name, const CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("SkyBox::create: gpu is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return AutoRef<SkyBox>(new SkyBoxVulkan(name, params.gpu));
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("SkyBox::create: D3D12 backend not implemented");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("SkyBox::create: Metal backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger)("SkyBox::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg

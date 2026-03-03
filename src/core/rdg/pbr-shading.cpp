#include "pch.h"
#include <garnet/GNrdg.h>
#include "gpu-context.h"
#include "vk-gpu-context.h"
#include "pbr-vert.spv.h"
#include "pbr-frag.spv.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// SubGraph::submit()
// =============================================================================

AutoRef<Submission> SubGraph::submit() {
    if (!graph) return {};
    auto sub = graph->submit(RenderGraph::SubmitParameters {.workflows = SafeArrayAccessor<Workflow *>(workflows.data(), workflows.size()), .name = name});
    workflows.clear();
    return sub;
}

// =============================================================================
// PbrShadingVulkan - stub implementation
// =============================================================================

class PbrShadingVulkan : public PbrShading {
    AutoRef<GpuContext> mGpu;

public:
    PbrShadingVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContext> gpu): PbrShading(db, TYPE_ID, TYPE_NAME, name), mGpu(std::move(gpu)) {}

    GpuContext & gpu() const override { return *mGpu; }

    SubGraph build(const BuildParameters & params) override {
        if (!params.renderGraph) {
            SubGraph sg;
            sg.builtResult = Action::ExecutionResult::PASSED;
            return sg;
        }
        Workflow * workflow = params.renderGraph->createWorkflow("Pbr");
        if (!workflow) GN_UNLIKELY {
                SubGraph sg;
                sg.builtResult = Action::ExecutionResult::FAILED;
                return sg;
            }
        // Create GpuDraw action with PBR vertex and fragment shaders
        GpuDraw::CreateParameters drawCp;
        drawCp.context  = mGpu;
        drawCp.vs       = {.binary = (void *) kPbrVertSpv, .size = kPbrVertSpvSize * sizeof(unsigned int), .entry = "main"};
        drawCp.ps       = {.binary = (void *) kPbrFragSpv, .size = kPbrFragSpvSize * sizeof(unsigned int), .entry = "main"};
        StrA drawName   = StrA::format("pbr_draw_{}", (unsigned long) sequence);
        auto drawAction = GpuDraw::create(database, drawName, drawCp);
        if (!drawAction) GN_UNLIKELY {
                SubGraph sg(*params.renderGraph, "Pbr");
                sg.builtResult = Action::ExecutionResult::FAILED;
                params.renderGraph->dropWorkflow(workflow);
                return sg;
            }
        auto drawArgs            = AutoRef<GpuDraw::A>(new GpuDraw::A());
        drawArgs->geometry.value = params.geometry;
        // Push constants: model (64 bytes) + viewProj (64 bytes), column-major for GLSL.
        const Matrix44f & model    = params.modelToWorld.matrix();
        const Matrix44f & viewProj = params.sharedShaderConstants ? params.sharedShaderConstants->getViewInformation().worldToClip : params.worldToClip;
        drawArgs->pushConstantData.resize(128);
        float * pc = reinterpret_cast<float *>(drawArgs->pushConstantData.data());
        for (int col = 0; col < 4; ++col)
            for (int row = 0; row < 4; ++row) pc[col * 4 + row] = model[row][col];
        pc += 16;
        for (int col = 0; col < 4; ++col)
            for (int row = 0; row < 4; ++row) pc[col * 4 + row] = viewProj[row][col];
        workflow->appendTask("PBR draw", std::move(drawAction), std::move(drawArgs));
        SubGraph sg(*params.renderGraph, "Pbr");
        sg.workflows.append(workflow);
        sg.builtResult = Action::ExecutionResult::PASSED;
        return sg;
    }
};

// =============================================================================
// PbrShading::create() - API-neutral dispatch
// =============================================================================

GN_API AutoRef<PbrShading> PbrShading::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("PbrShading::create: gpu is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan: {
        auto * p = new PbrShadingVulkan(db, name, params.gpu);
        if (p->sequence == 0) GN_UNLIKELY {
                GN_ERROR(sLogger)("PbrShading::create: duplicate type+name, name='{}'", name);
                delete p;
                return {};
            }
        return AutoRef<PbrShading>(p);
    }
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("PbrShading::create: D3D12 backend not implemented");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("PbrShading::create: Metal backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger)("PbrShading::create: unknown API, name='{}'", name);
        return {};
    }
}

// =============================================================================
// PbrMaterialImpl - minimal Material implementation (Task 4.1)
// =============================================================================

class PbrMaterialImpl : public PbrShading::Material {
    AutoRef<GpuContext> mGpu;

public:
    PbrMaterialImpl(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContext> gpu)
        : PbrShading::Material(db, TYPE_ID, TYPE_NAME, name), mGpu(std::move(gpu)) {}
};

// =============================================================================
// Material::load() - load from GN::File (minimal)
// =============================================================================

GN_API AutoRef<PbrShading::Material> PbrShading::Material::load(ArtifactDatabase & db, const StrA & name, const LoadParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("PbrShading::Material::load: gpu is null");
            return {};
        }
    if (!params.source) GN_UNLIKELY {
            GN_ERROR(sLogger)("PbrShading::Material::load: source is null");
            return {};
        }
    if (!params.source->readable()) GN_UNLIKELY {
            GN_ERROR(sLogger)("PbrShading::Material::load: source is not readable");
            return {};
        }
    // Minimal load: no format parsing yet; create material artifact for use in BuildParameters.
    auto * p = new PbrMaterialImpl(db, name, params.gpu);
    if (p->sequence == 0) GN_UNLIKELY {
            GN_ERROR(sLogger)("PbrShading::Material::load: duplicate type+name, name='{}'", name);
            delete p;
            return {};
        }
    return AutoRef<PbrShading::Material>(p);
}

} // namespace GN::rdg

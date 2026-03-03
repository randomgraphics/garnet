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
        // modelToWorld: wire in Task 3.2 via uniforms; for now geometry only
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
// Material::load() - stub (File-based API)
// =============================================================================

GN_API AutoRef<PbrShading::Material> PbrShading::Material::load(ArtifactDatabase & db, const StrA & name, const LoadParameters & params) {
    (void) db;
    (void) name;
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("PbrShading::Material::load: gpu is null");
            return {};
        }
    if (!params.source) GN_UNLIKELY {
            GN_ERROR(sLogger)("PbrShading::Material::load: source is null");
            return {};
        }
    // Stub: no full loading yet; return nullptr so callers can pass a File * for future use.
    return {};
}

} // namespace GN::rdg

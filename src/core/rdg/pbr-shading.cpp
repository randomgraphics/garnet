#include "pch.h"
#include <garnet/GNrdg.h>
#include <garnet/base/filesys.h>
#include "gpu-context.h"
#include "vk-gpu-context.h"
#include "pbr-vert.spv.h"
#include "pbr-frag.spv.h"
#include <sstream>
#include <string>

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
    AutoRef<GpuContext>      mGpu;
    mutable AutoRef<GpuDraw> mDrawAction; ///< Created once, reused for each build with different arguments.

    void ensureDrawAction() const {
        if (mDrawAction) return;
        GpuDraw::CreateParameters drawCp;
        drawCp.context = mGpu;
        drawCp.vs      = {.binary = (void *) kPbrVertSpv, .size = kPbrVertSpvSize * sizeof(unsigned int), .entry = "main"};
        drawCp.ps      = {.binary = (void *) kPbrFragSpv, .size = kPbrFragSpvSize * sizeof(unsigned int), .entry = "main"};
        StrA drawName  = StrA::format("pbr_draw_{}", (unsigned long) sequence);
        mDrawAction    = GpuDraw::create(database, drawName, drawCp);
        GN_REQUIRE(mDrawAction, fmt::format("Failed to create PBR draw action, name='{}'", drawName));
    }

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
        ensureDrawAction();
        if (!mDrawAction) GN_UNLIKELY {
                SubGraph sg(*params.renderGraph, "Pbr");
                sg.builtResult = Action::ExecutionResult::FAILED;
                params.renderGraph->dropWorkflow(workflow);
                return sg;
            }
        // Build arguments from params; the action is reused.
        auto drawArgs      = AutoRef<GpuDraw::A>(new GpuDraw::A());
        drawArgs->geometry = params.geometry;
        // Push constants: model (64 bytes) + viewProj (64 bytes), column-major for GLSL.
        const Matrix44f & model    = params.modelToWorld.matrix();
        const Matrix44f & viewProj = params.sharedShaderConstants ? params.sharedShaderConstants->getViewInformation().worldToClip : params.worldToClip;
        drawArgs->constants.resize(128);
        float * pc = reinterpret_cast<float *>(drawArgs->constants.data());
        for (int col = 0; col < 4; ++col)
            for (int row = 0; row < 4; ++row) pc[col * 4 + row] = model[row][col];
        pc += 16;
        for (int col = 0; col < 4; ++col)
            for (int row = 0; row < 4; ++row) pc[col * 4 + row] = viewProj[row][col];
        workflow->appendTask("PBR draw", AutoRef<Action>(mDrawAction), std::move(drawArgs));
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
// PbrMaterialImpl - Material with optional PBR textures from file (Task 4.1)
// =============================================================================

class PbrMaterialImpl : public PbrShading::Material {
    AutoRef<GpuContext> mGpu;
    AutoRef<Texture>    mBaseColorTexture;
    AutoRef<Texture>    mMetallicRoughnessTexture;
    AutoRef<Texture>    mNormalTexture;

public:
    PbrMaterialImpl(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContext> gpu)
        : PbrShading::Material(db, TYPE_ID, TYPE_NAME, name), mGpu(std::move(gpu)) {}

    GpuContext & gpu() const override { return *mGpu; }
    Texture *    getBaseColorTexture() const override { return mBaseColorTexture.get(); }
    Texture *    metallicRoughnessTexture() const { return mMetallicRoughnessTexture.get(); }
    Texture *    normalTexture() const { return mNormalTexture.get(); }

    void setBaseColorTexture(AutoRef<Texture> t) { mBaseColorTexture = std::move(t); }
    void setMetallicRoughnessTexture(AutoRef<Texture> t) { mMetallicRoughnessTexture = std::move(t); }
    void setNormalTexture(AutoRef<Texture> t) { mNormalTexture = std::move(t); }
};

// Parse a single line "key=value", trim whitespace. Returns true if line had key=value.
static bool parseMaterialLine(const std::string & line, std::string & key, std::string & value) {
    size_t eq = line.find('=');
    if (eq == std::string::npos) return false;
    key       = line.substr(0, eq);
    value     = line.substr(eq + 1);
    auto trim = [](std::string & s) {
        while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) s.erase(0, 1);
        while (!s.empty() && (s.back() == ' ' || s.back() == '\t')) s.pop_back();
    };
    trim(key);
    trim(value);
    return !key.empty();
}

// =============================================================================
// Material::load() - load from GN::File (key=value format; loads textures)
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
    auto * p = new PbrMaterialImpl(db, name, params.gpu);
    if (p->sequence == 0) GN_UNLIKELY {
            GN_ERROR(sLogger)("PbrShading::Material::load: duplicate type+name, name='{}'", name);
            delete p;
            return {};
        }

    // Determine the base path to resolve relative texture paths.
    // If not provided, the the loader will try using the source file's directory as the base path, if it has one.
    // If the source file does not have a path associated with it (like a memory file), then the texture path
    // in the material file will be resolved as relative to the current working directory.
    StrA basePath = params.basePath;
    if (basePath.empty()) {
        basePath = GN::fs::dirName(params.source->name());
        if (basePath.empty()) {
            GN_WARN(sLogger)
            ("PbrShading::Material::load: no base path provided and source file has no directory. "
             "The loader will try to resolve relative texture paths as relative to the current working directory, "
             "which could yeild undetermined result.");
            basePath = GN::fs::getCurrentDir();
        }
    }

    std::istream & in = params.source->input();
    std::string    s;
    while (std::getline(in, s)) {
        while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.pop_back();
        if (s.empty() || (s.size() >= 1 && s[0] == '#')) continue;
        std::string key, value;
        if (!parseMaterialLine(s, key, value)) continue;

        StrA             valueA(value.c_str());
        StrA             texPath = GN::fs::isAbsPath(valueA) ? valueA : GN::fs::resolvePath(basePath, valueA);
        AutoRef<Texture> tex     = Texture::load(db, Texture::LoadParameters {.context = params.gpu, .filename = texPath});
        if (!tex) {
            GN_ERROR(sLogger)("PbrShading::Material::load: failed to load texture '{}' for key '{}'", texPath, key);
            continue;
        }
        if (key == "baseColorTexture")
            p->setBaseColorTexture(std::move(tex));
        else if (key == "metallicRoughnessTexture" || key == "armTexture")
            p->setMetallicRoughnessTexture(std::move(tex));
        else if (key == "normalTexture" || key == "normalTexture_gl")
            p->setNormalTexture(std::move(tex));
        else if (key == "normalTexture_dx" && !p->normalTexture())
            p->setNormalTexture(std::move(tex));
    }
    return AutoRef<PbrShading::Material>(p);
}

} // namespace GN::rdg

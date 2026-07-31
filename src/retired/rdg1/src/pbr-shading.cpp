#include "pch.h"
#include "vk-gpu-context.h"
#include "vk-texture.h"
#include "pbr-vert.spv.h"
#include "pbr-frag.spv.h"
#include <sstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::inverse
#include <glm/gtc/quaternion.hpp>        // glm::mat4_cast
#include <glm/ext/matrix_clip_space.hpp> // glm::perspectiveRH_ZO

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// PbrShadingVulkan - stub implementation
// =============================================================================

class PbrShadingVulkan : public PbrShading {
    AutoRef<GpuContext>      mGpu;
    mutable AutoRef<GpuDraw> mDrawAction;               ///< Created once, reused for each build with different arguments.
    mutable AutoRef<Texture> mDefaultBaseColor;         ///< 1x1 white for missing base color
    mutable AutoRef<Texture> mDefaultMetallicRoughness; ///< 1x1 (128,128,0) for missing ARM
    mutable AutoRef<Texture> mDefaultNormal;            ///< 1x1 (128,128,255) for missing normal

    Texture * defaultBaseColor() const {
        if (!mDefaultBaseColor) mDefaultBaseColor = createDefault1x1Texture(mGpu, "pbr_default_base", 255, 255, 255, 255);
        return mDefaultBaseColor.get();
    }
    Texture * defaultMetallicRoughness() const {
        if (!mDefaultMetallicRoughness) mDefaultMetallicRoughness = createDefault1x1Texture(mGpu, "pbr_default_arm", 128, 128, 0, 255);
        return mDefaultMetallicRoughness.get();
    }
    Texture * defaultNormal() const {
        if (!mDefaultNormal) mDefaultNormal = createDefault1x1Texture(mGpu, "pbr_default_normal", 128, 128, 255, 255);
        return mDefaultNormal.get();
    }

    void ensureDrawAction() const {
        if (mDrawAction) return;
        GpuDraw::CreateParameters drawCp;
        drawCp.context = mGpu;
        drawCp.vs      = {.binary = (void *) kPbrVertSpv, .size = kPbrVertSpvSize * sizeof(unsigned int), .entry = "main"};
        drawCp.ps      = {.binary = (void *) kPbrFragSpv, .size = kPbrFragSpvSize * sizeof(unsigned int), .entry = "main"};
        StrA drawName  = name.empty() ? "pbr_draw"_s : StrA::format("pbr_draw_{}", name);
        mDrawAction    = GpuDraw::create(drawName, drawCp);
        GN_REQUIRE(mDrawAction, fmt::format("Failed to create PBR draw action, name='{}'", drawName));
    }

public:
    PbrShadingVulkan(const StrA & name, AutoRef<GpuContext> gpu): PbrShading(TYPE_INFO(), name), mGpu(std::move(gpu)) {}

    GpuContext & gpu() const override { return *mGpu; }

    SubGraph build(const BuildParameters & params) override {
        if (!params.renderGraph) {
            SubGraph sg;
            sg.builtResult = Action::ExecutionResult::PASSED;
            return sg;
        }
        Workflow workflow = params.renderGraph->createWorkflow("Pbr");
        if (!workflow) GN_UNLIKELY {
                SubGraph sg;
                sg.builtResult = Action::ExecutionResult::FAILED;
                return sg;
            }
        ensureDrawAction();
        if (!mDrawAction) GN_UNLIKELY {
                SubGraph sg(*params.renderGraph, "Pbr");
                sg.builtResult = Action::ExecutionResult::FAILED;
                return sg;
            }
        // Build arguments from params; the action is reused.
        auto drawArgs      = AutoRef<GpuDraw::A>(new GpuDraw::A());
        drawArgs->geometry = params.geometry;

        // Render target comes from SharedShaderConstants.ViewInformation.
        if (params.sharedShaderConstants) drawArgs->renderTarget = params.sharedShaderConstants->getViewInformation().renderTarget;

        // Set 0: use shared resource set from SharedShaderConstants (camera + lighting; expandable).
        if (params.sharedShaderConstants) {
            const auto & set0 = params.sharedShaderConstants->getSet0Resources();
            if (!set0.empty()) {
                drawArgs->resources.resize(1);
                drawArgs->resources[0] = set0;
            }
        }

        // Set 1: PBR material textures (binding 0 = base color, 1 = metallic-roughness/ARM, 2 = normal). Always bind; use material or defaults.
        auto addTex = [&](GpuShaderAction::GraphicsResourceSet & set, size_t binding, Texture * tex) {
            Texture * t = tex ? tex : (binding == 0 ? defaultBaseColor() : (binding == 1 ? defaultMetallicRoughness() : defaultNormal()));
            if (!t) return;
            GpuResourceView v;
            v.setArtifact(AutoRef<Artifact>(t));
            v.setSubresourceExtent(GpuResourceView::SubresourceExtent {1, 1});
            if (set.size() <= binding) set.resize(binding + 1);
            set[binding].append(std::move(v));
        };
        GpuShaderAction::GraphicsResourceSet materialSet;
        materialSet.resize(3);
        Texture * baseColor = params.material ? params.material->getBaseColorTexture() : nullptr;
        Texture * arm       = params.material ? params.material->getMetallicRoughnessTexture() : nullptr;
        Texture * normal    = params.material ? params.material->getNormalTexture() : nullptr;
        addTex(materialSet, 0, baseColor);
        addTex(materialSet, 1, arm);
        addTex(materialSet, 2, normal);
        if (drawArgs->resources.size() < 2) drawArgs->resources.resize(2);
        drawArgs->resources[1] = std::move(materialSet);

        // Push constants: model matrix only (64 bytes). viewProj now comes from Set 0 UBO.
        // GLM stores mat4 in column-major order, matching GLSL layout, so memcpy directly.
        const glm::mat4 model = glm::translate(glm::mat4(1.f), params.locationInWorldSpace) * glm::mat4_cast(params.orientationInWorldSpace);
        drawArgs->immediates.resize(64);
        memcpy(drawArgs->immediates.data(), glm::value_ptr(model), 64);

        workflow.appendTask("PBR draw", AutoRef<Action>(mDrawAction), std::move(drawArgs));
        SubGraph sg(*params.renderGraph, "Pbr");
        sg.workflows.append(std::move(workflow));
        sg.builtResult = Action::ExecutionResult::PASSED;
        return sg;
    }
};

// =============================================================================
// PbrShading::create() - API-neutral dispatch
// =============================================================================

GN_API AutoRef<PbrShading> PbrShading::create(const StrA & name, const CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger, "PbrShading::create: gpu is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan: {
        return AutoRef<PbrShading>(new PbrShadingVulkan(name, params.gpu));
    }
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger, "PbrShading::create: D3D12 backend not implemented");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger, "PbrShading::create: Metal backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "PbrShading::create: unknown API, name='{}'", name);
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
    PbrMaterialImpl(const StrA & name, AutoRef<GpuContext> gpu): PbrShading::Material(TYPE_INFO(), name), mGpu(std::move(gpu)) {}

    GpuContext & gpu() const override { return *mGpu; }
    Texture *    getBaseColorTexture() const override { return mBaseColorTexture.get(); }
    Texture *    getMetallicRoughnessTexture() const override { return mMetallicRoughnessTexture.get(); }
    Texture *    getNormalTexture() const override { return mNormalTexture.get(); }

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

GN_API AutoRef<PbrShading::Material> PbrShading::Material::load(const StrA & name, const LoadParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger, "PbrShading::Material::load: gpu is null");
            return {};
        }
    if (!params.source) GN_UNLIKELY {
            GN_ERROR(sLogger, "PbrShading::Material::load: source is null");
            return {};
        }
    if (!params.source->readable()) GN_UNLIKELY {
            GN_ERROR(sLogger, "PbrShading::Material::load: source is not readable");
            return {};
        }
    auto * p = new PbrMaterialImpl(name, params.gpu);

    // Determine the base path to resolve relative texture paths.
    // If not provided, the the loader will try using the source file's directory as the base path, if it has one.
    // If the source file does not have a path associated with it (like a memory file), then the texture path
    // in the material file will be resolved as relative to the current working directory.
    StrA basePath = params.basePath;
    if (basePath.empty()) {
        basePath = GN::fs::dirName(params.source->name());
        if (basePath.empty()) {
            GN_WARN(sLogger, "PbrShading::Material::load: no base path provided and source file has no directory. "
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
        AutoRef<Texture> tex     = Texture::load(Texture::LoadParameters {.context = params.gpu, .filename = texPath});
        if (!tex) {
            GN_ERROR(sLogger, "PbrShading::Material::load: failed to load texture '{}' for key '{}'", texPath, key);
            continue;
        }
        if (key == "baseColorTexture")
            p->setBaseColorTexture(std::move(tex));
        else if (key == "metallicRoughnessTexture" || key == "armTexture")
            p->setMetallicRoughnessTexture(std::move(tex));
        else if (key == "normalTexture" || key == "normalTexture_gl")
            p->setNormalTexture(std::move(tex));
        else if (key == "normalTexture_dx" && !p->getNormalTexture())
            p->setNormalTexture(std::move(tex));
    }
    return AutoRef<PbrShading::Material>(p);
}

} // namespace GN::rdg

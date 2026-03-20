#pragma once

#include <garnet/rdg/rtti.h>
#include <glm/mat4x4.hpp>
#include <mutex>

namespace GN::rdg {

// /// Represents one light source in the scene.
// struct SimpleLight : public Artifact {
//     GN_API static const uint64_t         TYPE_ID;
//     inline static constexpr const char * TYPE_NAME = "SimpleLight";

//     enum Type : int {
//         POINT,
//         SPOT,
//         DIRECTIONAL,
//     } type;

//     struct SpotLight {
//         Location     location          = {};
//         Orientation  orientation       = {};
//         IntensityRGB intensity         = {1.0f, 1.0f, 1.0f, {1.0f}};
//         WorldUnit    range             = {};
//         float        cosInnerConeAngle = 1.0f;
//         float        cosOuterConeAngle = 1.0f;
//     } spot;

//     struct PointLight {
//         Location     location;
//         IntensityRGB intensity;
//         float        range = 0.0f;
//     } point;

//     struct DirectionalLight {
//         Orientation   orientation;
//         IrradianceRGB irradiance;
//     } directional;

//     bool active        = true;
//     bool castingShadow = false;
// };

/// Self-contained list of workflows (and their tasks and arguments) that can be
/// submitted to a RenderGraph to render the effect to screen or to a render target.
struct GN_API SubGraph {
    GN_NO_COPY(SubGraph); // not copyable

    RenderGraph *           graph       = {};
    StrA                    name        = {};
    Action::ExecutionResult builtResult = Action::ExecutionResult::PASSED;
    DynaArray<Workflow>     workflows;

    SubGraph() = default;

    SubGraph(RenderGraph & graph, const StrA & name): graph(&graph), name(name) {}

    ~SubGraph() { drop(); }

    // move constructor
    SubGraph(SubGraph && other) noexcept: graph(other.graph), name(other.name), builtResult(other.builtResult), workflows(std::move(other.workflows)) {
        other.graph = nullptr;
        other.name.clear();
        other.builtResult = Action::ExecutionResult::PASSED;
        GN_ASSERT(other.workflows.empty());
    }

    // move assignment
    SubGraph & operator=(SubGraph && other) noexcept {
        if (this == &other) return *this;
        drop(); // drop current graph, if any.
        graph       = other.graph;
        name        = other.name;
        builtResult = other.builtResult;
        workflows   = std::move(other.workflows);
        other.graph = nullptr;
        other.name.clear();
        other.builtResult = Action::ExecutionResult::PASSED;
        GN_ASSERT(other.workflows.empty());
        return *this;
    }

    /// Submit this subgraph for execution on the given render graph.
    /// The workflows are submitted and then cleared from this SubGraph.
    /// \return The submission object; valid until execution completes.
    AutoRef<Submission> submit() {
        if (!graph) return {};
        auto sub = graph->submit(RenderGraph::SubmitParameters {.workflows = SafeArrayAccessor<Workflow>(workflows.data(), workflows.size()), .name = name});
        workflows.clear();
        return sub;
    }

    /// Drop this subgraph without executing. All workflows are dropped and cleared from this SubGraph.
    SubGraph & drop() {
        workflows.clear();
        builtResult = Action::ExecutionResult::DROPPED;
        return *this;
    }
};

/// A container for shader constants that are shared by other effects.
/// Holds logical data (frame, view, lights, etc.); the API-specific implementation
/// organises and uploads this data to GPU resources. The public interface does not
/// expose any information about how the backend organises or binds these resources.
struct SharedShaderConstants : public GpuResource {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(GpuResource);

    /// Logical frame data. Implementation maps this to GPU resources as needed.
    struct FrameInformation {
        uint32_t     frameCounter  = 0;
        Microseconds frameDuration = {};
    };

    /// Logical view/camera data. Implementation maps this to GPU resources as needed.
    struct ViewInformation {
        Location              cameraPosition    = {0, 0, 0};            ///< camera position in world space
        Orientation           cameraOrientation = {1.f, 0.f, 0.f, 0.f}; ///< camera orientation (world space)
        Radian                cameraFov         = Degree(60.f);         ///< vertical field of view
        float                 aspectRatio       = 16.f / 9.f;           ///< viewport width / height
        float                 nearPlane         = 0.01f;                ///< near clip plane in meters
        float                 farPlane          = 10000.f;              ///< far clip plane in meters
        AutoRef<RenderTarget> renderTarget;                             ///< current render target; render target size is deduced from it
    };

    /// One direct light with physically correct photometric terms (see physical.h).
    struct DirectLight {
        enum Type : int { POINT, SPOT, DIRECTIONAL } type = POINT;

        /// Point light: position, luminous intensity [cd], range in meters.
        struct Point {
            Location     position  = {0, 0, 0};
            IntensityRGB intensity = {1.0f, 1.0f, 1.0f, {1.0f}};
            Distance     range     = 0;
        } point;

        /// Spot light: position, orientation, luminous intensity [cd], range, cone angles.
        struct Spot {
            Location     position          = {0, 0, 0};
            Orientation  orientation       = {0, 0, 0, 1.0f};
            IntensityRGB intensity         = {1.0f, 1.0f, 1.0f, {1.0f}};
            Distance     range             = 0;
            float        cosInnerConeAngle = 1.0f;
            float        cosOuterConeAngle = 1.0f;
        } spot;

        /// Directional light: orientation, irradiance [lx].
        struct Directional {
            Orientation   orientation = {0, 0, 0, 1.0f};
            IrradianceRGB irradiance  = {1.0f, 1.0f, 1.0f, {1.0f}};
        } directional;
    };

    /// Logical direct lighting data. Implementation maps to GPU resources as needed.
    struct DirectLightingInformation {
        DynaArray<DirectLight> lights;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Resource set for Set 0: bindings (camera UBO, lighting UBO, expandable).
    /// Same shape as one set in GpuResourceTable: set0[bindingIndex][arrayIndex] = GpuResourceView.
    using Set0ResourceSet = GpuShaderAction::GraphicsResourceSet;

    /// Environment / image-based lighting. Supplies scene-level textures used by both the skybox
    /// renderer and the IBL diffuse/specular terms in PBR shading.
    struct EnvironmentLightingInformation {
        AutoRef<Texture> skyboxCubemap;     ///< environment cubemap rendered as background
        AutoRef<Texture> irradianceMap;     ///< pre-convolved diffuse-IBL cubemap (hemisphere integral)
        AutoRef<Texture> prefilteredEnvMap; ///< mip-mapped specular-IBL cubemap (roughness → mip level)
        AutoRef<Texture> brdfLut;           ///< split-sum BRDF LUT: NdotV×roughness → (scale, bias)
    };

    virtual void setFrameInformation(const FrameInformation &)                             = 0;
    virtual void setViewInformation(const ViewInformation &)                               = 0;
    virtual void setDirectLightingInformation(const DirectLightingInformation &)           = 0;
    virtual void setEnvironmentLightingInformation(const EnvironmentLightingInformation &) = 0;

    virtual const FrameInformation &              getFrameInformation() const              = 0;
    virtual const ViewInformation &               getViewInformation() const               = 0;
    virtual const DirectLightingInformation &     getDirectLightingInformation() const     = 0;
    virtual const EnvironmentLightingInformation & getEnvironmentLightingInformation() const = 0;
    /// Last built Set 0 resource set (valid after build() has been called). Effects use this as set #0 in the resource table.
    virtual const Set0ResourceSet & getSet0Resources() const = 0;

    /// Snapshot current CPU state; upload to GPU. Returns a SubGraph whose workflows perform the upload. Call once per frame; submit that SubGraph (or its
    /// workflows) before draw workflows. Effects get set0 via getSet0Resources().
    virtual SubGraph build(RenderGraph & rg) = 0;

    static GN_API AutoRef<SharedShaderConstants> create(const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

// /// Represents lighting information of the surrounding world.
// /// Can be, but not limited to, skybox, reflection probes, global illumination etc.
// struct EnvironmentalLighting : public RenderGraphBuilder {
//     GN_API static const uint64_t         TYPE_ID;
//     inline static constexpr const char * TYPE_NAME = "EnvironmentalLighting";

//     /// Sky lighting. If skybox texture is not provided, use the ambient as fallback.
//     /// \todo: no need to put them here. move them to backend implementation.
//     AutoRef<Texture> skybox;
//     IrradianceRGB    ambient = {1.0f, 1.0f, 1.0f, {300.0f}}; /// default to 300 lux (normal office lighting).

//     // TODO: more environment map types, like irradiance map, prefiltered map, etc.

//     struct CreateParameters {
//         AutoRef<GpuContext> gpu;
//         /// TODO: add more parameters for different environment map types.
//     };

//     struct LoadParameters {
//         AutoRef<GpuContext> gpu;
//         StrA                fileName;
//     };

//     /// Create a new blank (full black) environment resource.
//     static GN_API AutoRef<EnvironmentalLighting> create(const StrA & name, const CreateParameters & params);

//     /// Load a Pbr environment resource from external file.
//     static GN_API AutoRef<EnvironmentalLighting> load(const StrA & name, const LoadParameters & params);

// protected:
//     using RenderGraphBuilder::RenderGraphBuilder;
// };

// /// Store shadow related information for all active lights, to be used
// /// to determine whether a surface point is in shadow or not.
// struct ShadowVisibility : public RenderGraphBuilder {
//     GN_API static const uint64_t         TYPE_ID;
//     inline static constexpr const char * TYPE_NAME = "ShadowVisibility";

//     struct CreateParameters {
//         // tbd
//     };

//     static GN_API AutoRef<ShadowVisibility> create(const StrA & name, const CreateParameters & params);

// protected:
//     using RenderGraphBuilder::RenderGraphBuilder;
// };

// struct SimpleForwardShadingPipeline : public Artifact {
//     struct CreateParameters {
//         RenderGraph * renderGraph = nullptr;
//     };

//     /// Direct lighting information. For now, is just a list of active lights.
//     /// Later can be expanded to a dedicated DirectLighting artifact and workflows.
//     DynaArray<SimpleLight> lights;

//     /// The stage to render shadow maps
//     DynaArray<Workflow *>     shadowPass;
//     AutoRef<ShadowVisibility> shadowVisibility;

//     /// The state to render/update environment maps.
//     DynaArray<Workflow *>          environmentalPass;
//     AutoRef<EnvironmentalLighting> environmentalLighting;

//     /// \todo The stage to render depth prepass (maybe motion vector as well)

//     /// The main stage for forward color pass.
//     DynaArray<Workflow *> mainColorPass;

//     /// \todo The stage to perform post processing: tone mapping, anti-aliasing, etc.

//     /// \todo UI stage is rendered last after main scene is completely done.
//     /// This allows main scene and UI to be rendered with different resolution or even frequency.
//     /// Also, UI is usually rendered in LDR, no post processing (like anti-aliasing) is needed.

//     /// Submit the accumulated workflows for execution. Clear out all workflow arrays.
//     virtual AutoRef<Submission> submit() = 0;

//     static GN_API AutoRef<SimpleForwardShadingPipeline> create(const StrA & name, const CreateParameters & params);

// protected:
//     using Artifact::Artifact;
// };

struct PbrShading : public GpuResource {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(GpuResource);

    /// Represents a PBR material resource.
    /// Backend is free to determine the best way to implements it, such as texture set, material parameter values, etc.
    struct Material : public GpuResource {
        GN_API GN_RDG_REGISTER_RUNTIME_TYPE(GpuResource);

        struct LoadParameters {
            AutoRef<GpuContext> gpu;

            /// The source file to load the material from, provding an non-null readable stream
            /// (disk, memory blob, or embedded)
            AutoRef<GN::File> source;

            /// Optional base path to resolve relative texture paths (e.g. "media::materials/")
            /// If not provided, the the loader will try using the source file's directory as the base path, if it has one.
            /// If the source file does not have a path associated with it (like a memory file), then the texture path
            /// in the material file will be resolved as relative to the current working directory.
            StrA basePath = {};
        };

        static GN_API AutoRef<Material> load(const StrA & name, const LoadParameters & params);

        /// Optional: return base color texture if loaded from file. Default returns nullptr.
        virtual Texture * getBaseColorTexture() const { return nullptr; }

        /// Optional: return metallic-roughness (or ARM) texture if loaded. Default returns nullptr.
        virtual Texture * getMetallicRoughnessTexture() const { return nullptr; }

        /// Optional: return normal map texture if loaded. Default returns nullptr.
        virtual Texture * getNormalTexture() const { return nullptr; }

    protected:
        using GpuResource::GpuResource;
    };

    struct BuildParameters {
        RenderGraph *                        renderGraph = {};
        AutoRef<const SharedShaderConstants> sharedShaderConstants;
        AutoRef<Material>                    material;
        GpuDraw::GpuGeometry                 geometry;
        Location                             locationInWorldSpace    = {0, 0, 0};
        Orientation                          orientationInWorldSpace = ZERO_ROTATION;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Add task graphs into the workflow to render a PBR object.
    virtual SubGraph build(const BuildParameters & params) = 0;

    static GN_API AutoRef<PbrShading> create(const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

struct SkyBox : public GpuResource {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(GpuResource);

    struct BuildParameters {
        AutoRef<const SharedShaderConstants> sharedShaderConstants;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Add task graphs into the workflow to render a sky box.
    virtual SubGraph build(const BuildParameters & params);

    static GN_API AutoRef<SkyBox> create(const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

} // namespace GN::rdg
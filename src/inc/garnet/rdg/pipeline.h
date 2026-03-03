#pragma once

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

/// A container for shader constants that are shared by other effects.
/// Holds logical data (frame, view, lights, etc.); the API-specific implementation
/// organizes and uploads this data to uniform buffers and textures. The public
/// interface does not expose GPU layout.
struct SharedShaderConstants : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "SharedShaderConstants";

    /// Logical frame data. Implementation maps this to GPU resources as needed.
    struct FrameInformation {
        uint32_t     frameCounter = 0;
        Microseconds frameDuration {};
    };

    /// Logical view/camera data. Implementation maps this to GPU resources as needed.
    struct ViewInformation {
        Matrix44f             worldToClip   = Matrix44f::sIdentity();
        Location              cameraPosition = {}; ///< camera position in world space
        AutoRef<RenderTarget> renderTarget;
    };

    /// One direct light with physically correct photometric terms (see physical.h).
    struct DirectLight {
        enum Type : int { POINT, SPOT, DIRECTIONAL } type = POINT;

        /// Point light: position, luminous intensity [cd], range in world units.
        struct Point {
            Location     position  = {};
            IntensityRGB intensity = {1.0f, 1.0f, 1.0f, {1.0f}};
            WorldUnit    range     = {};
        } point;

        /// Spot light: position, orientation, luminous intensity [cd], range, cone angles.
        struct Spot {
            Location     position          = {};
            Orientation  orientation       = {};
            IntensityRGB intensity         = {1.0f, 1.0f, 1.0f, {1.0f}};
            WorldUnit    range             = {};
            float        cosInnerConeAngle = 1.0f;
            float        cosOuterConeAngle = 1.0f;
        } spot;

        /// Directional light: orientation, irradiance [lx].
        struct Directional {
            Orientation   orientation = {};
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

    virtual void setFrameInformation(const FrameInformation &)                   = 0;
    virtual void setViewInformation(const ViewInformation &)                     = 0;
    virtual void setDirectLightingInformation(const DirectLightingInformation &) = 0;

    virtual const FrameInformation &         getFrameInformation() const         = 0;
    virtual const ViewInformation &           getViewInformation() const          = 0;
    virtual const DirectLightingInformation & getDirectLightingInformation() const = 0;

    static GN_API AutoRef<SharedShaderConstants> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

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
//     static GN_API AutoRef<EnvironmentalLighting> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

//     /// Load a Pbr environment resource from external file.
//     static GN_API AutoRef<EnvironmentalLighting> load(ArtifactDatabase & db, const StrA & name, const LoadParameters & params);

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

//     static GN_API AutoRef<ShadowVisibility> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

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

//     static GN_API AutoRef<SimpleForwardShadingPipeline> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

// protected:
//     using Artifact::Artifact;
// };

/// Self-contained list of workflows (and their tasks and arguments) that can be
/// submitted to a RenderGraph to render the effect to screen or to a render target.
struct GN_API SubGraph {
    GN_NO_COPY(SubGraph); // not copyable

    RenderGraph *           graph       = {};
    StrA                    name        = {};
    Action::ExecutionResult builtResult = Action::ExecutionResult::PASSED;
    DynaArray<Workflow *>   workflows;

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
    AutoRef<Submission> submit();

    /// Drop this subgraph without executing. All workflows are dropped and cleared from this SubGraph.
    SubGraph & drop() {
        if (!graph) return *this;
        for (auto * w : workflows) { graph->dropWorkflow(w); }
        workflows.clear();
        builtResult = Action::ExecutionResult::DROPPED;
        return *this;
    }
};

struct PbrShading : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "PbrShading";

    /// Represents a PBR material resource.
    /// Backend is free to determine the best way to implements it, such as texture set, material parameter values, etc.
    struct Material : public GpuResource {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "PbrMaterial";

        struct LoadParameters {
            AutoRef<GpuContext> gpu;
            GN::File *          source  = {}; ///< non-null readable stream (disk, memory blob, or embedded)
            StrA                basePath = {}; ///< optional base path to resolve relative texture paths (e.g. "media::materials/")
        };

        static GN_API AutoRef<Material> load(ArtifactDatabase & db, const StrA & name, const LoadParameters & params);

        /// Optional: return base color texture if loaded from file. Default returns nullptr.
        virtual Texture * getBaseColorTexture() const { return nullptr; }

    protected:
        using GpuResource::GpuResource;
    };

    struct BuildParameters {
        RenderGraph *                  renderGraph = {};
        AutoRef<SharedShaderConstants> sharedShaderConstants;
        AutoRef<Material>              material;
        GpuGeometry                    geometry;
        AffineTransform                modelToWorld;
        /// World-to-clip (view-projection) matrix. Used for push constants when SharedShaderConstants view is not yet available.
        Matrix44f worldToClip = Matrix44f::sIdentity();
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Add task graphs into the workflow to render a PBR object.
    virtual SubGraph build(const BuildParameters & params) = 0;

    static GN_API AutoRef<PbrShading> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

struct SkyBox : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "SkyBox";

    struct BuildParameters {
        AutoRef<SharedShaderConstants> sharedShaderConstants;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Add task graphs into the workflow to render a sky box.
    virtual SubGraph build(const BuildParameters & params);

    static GN_API AutoRef<SkyBox> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

} // namespace GN::rdg
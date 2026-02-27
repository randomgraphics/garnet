#pragma once

namespace GN::rdg {

/// Represents one light source in the scene.
struct SimpleLight : public Artifact {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "SimpleLight";

    enum Type : int {
        POINT,
        SPOT,
        DIRECTIONAL,
    } type;

    struct SpotLight {
        Location     location          = {};
        Orientation  orientation       = {};
        IntensityRGB intensity         = {1.0f, 1.0f, 1.0f, 1.0f};
        WorldUnit    range             = {};
        float        cosInnerConeAngle = 1.0f;
        float        cosOuterConeAngle = 1.0f;
    } spot;

    struct PointLight {
        Location     location;
        IntensityRGB intensity;
        float        range = 0.0f;
    } point;

    struct DirectionalLight {
        Orientation   orientation;
        IrradianceRGB irradiance;
    } directional;

    bool active = true;
    bool castingShadow = false;
};

/// Base class of all render graph builders.
struct RenderGraphBuilder : public Artifact {
    /// \todo add public interface later.
protected:
    using Artifact::Artifact;
};

/// Represents lighing information of the surrounding world.
/// Can be, but not limited to, skybox, reflection probes, global illumination etc.
struct EnviromentalLighting : public RenderGraphBuilder {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "EnviromentalLighting";

    /// Sky lighting. If skybox texture is not provided, use the ambient as fallback.
    /// \todo: no need to put them here. move them to backend implementation.
    AutoRef<Texture> skybox;
    IrradianceRGB    ambient = {1.0f, 1.0f, 1.0f, 300.0f}; /// default to 300 lux (normal office lighting).

    // TODO: more environment map types, like irradiance map, prefiltered map, etc.

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
        /// TODO: add more parameters for different environment map types.
    };

    struct LoadParameters {
        AutoRef<GpuContext> gpu;
        StrA                fileName;
    };

    /// Create a new blank (full black) environment resource.
    static GN_API AutoRef<EnvironmentMap> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

    /// Load a Pbr environment resource from external file.
    static GN_API AutoRef<EnvironmentMap> load(ArtifactDatabase & db, const StrA & name, const LoadParameters & params);

protected:
    using RenderGraphBuilder::RenderGraphBuilder;
};

// struct DirectLighing : public RenderGraphBuilder {
//     GN_API static const uint64_t         TYPE_ID;
//     inline static constexpr const char * TYPE_NAME = "DirectLighting";

//     struct CreateParameters {
//         // tbd
//     };

// protected:
//     using RenderGraphBuilder::RenderGraphBuilder;
// };


/// Store shadow relate information for all active lights, to be used
/// to determine whether a surface point is in shadow or not.
struct ShadowVisbility : public RenderGraphBuilder {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "ShadowVisibility";

    struct CreateParameters {
        // tbd
    };

    static GN_API AutoRef<ShadowVisbility> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using RenderGraphBuilder::RenderGraphBuilder;
};

// struct EffectContext : public Artifact {
//     GN_API static const uint64_t         TYPE_ID;
//     inline static constexpr const char * TYPE_NAME = "EffectContext";

//     // // Frame specific information
//     // uint64_t    frameCounter  = 0;              // A monotonically increasing counter of the frame.
//     // Nanoseconds frameDuration = Nanoseconds(0); // duration of the last known good frame in nanoseconds.
//     // Nanoseconds engineUptime  = Nanoseconds(0); // total run time since the engine started in nanoseconds.

//     // // Caemra information
//     // WorldToClipTransform worldToClip;

//     // // Lighting information
//     // DynaArray<SimpleLight>    lights;             /// lists off available/significant/visible lights for the current view.
//     // AutoRef<ShadowVisibility> shadowVisibility;   /// the shadow visibility information for the current view.
//     // AutoRef<EnvironmentMap>   environmentMap;     /// the environment map to be used for the current view.

//     struct CreateParameters {
//         RenderGraph renderGraph;
// };


struct SimpleForwardShadingPipeline : public Artifact {
    struct CreateParameters {
        RenderGraph renderGraph;
    };

    /// Direct lighint information. For now, is just a ist of active lights.
    /// Later can be expanded to a dedicated DirectLighing artifact and workflows.
    DynaArray<SimpleLight> lights;

    /// The stage to render shadow maps
    DynaArray<Workflow*> shadowPass = nullptr;
    AutoRef<ShadowVisibility> shadowVisibility;

    /// The state to render/update environment maps.
    DynaArray<Workflow*> environmentalPass = nullptr;
    AutoRef<EnvironmentalLighting> environmentalLighting;

    /// \todo The stage to render depth prepass (maybe motion vector as well)

    /// The main stage for forward color pass.
    DynaArray<Workflow*> mainColorPass;

    /// \todo The stage to perform post processing: tone mapping, anti-aliasing, etc.

    /// \todo UI stage is rendered last after main scene is completely done.
    /// This allows main scene and UI to be rendered with different resolution or even frequency.
    /// Also, UI is usually rendered in LDR, no post processing (like anti-aliasing) is needed.

    /// Submit the accumulated workflows for execution. Clear out all workflow arrays.
    virtual AutoRef<Submission> submit() = 0;

    static GN_API AutoRef<SimpleForwardShadingPipeline> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Artifact::Artifact;
};

struct PbrShading : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "PbrShading";

    /// Represents a PBR material resource.
    /// Backend is free to determine the best way to implements it, such as texture set, material parameter values, etc.
    struct Material : public GpuResource {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "PbrTextureSet";

        struct LoadParameters {
            AutoRef<GpuContext> gpu;
            StrA                fileName;
        };

        static GN_API AutoRef<Material> load(ArtifactDatabase & db, const StrA & name, const LoadParameters & params);

    protected:
        using GpuResource::GpuResource;
    };

    struct BuildParameters {
        AutoRef<Material>    material;
        AutoRef<GpuGeometry> geometry;
        AffineTransform      modelToWorld;
        WorldToClipTransform worldToClip;
        AutoRef<EnvironmentalLighting> env;
        AutoRef<ShadowVisibility> shadow;
        RenderTarget         renderTarget;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Add task graphs into the workflow to render a PBR object.
    virtual Action::ExecutionResult build(const BuildParameters & params) = 0;

    static GN_API AutoRef<PbrShading> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

struct SkyBox : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "SkyBox";

    struct BuildParameters {
        RenderTarget rt;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Add task graphs into the workflow to render a sky box.
    virtual Action::ExecutionResult build(Workflow & workflow, const EffectContext & context, const BuildParameters & params);

    static GN_API AutoRef<SkyBox> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

} // namespace GN::rdg
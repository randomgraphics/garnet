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
        IntensityRGB intensity         = {1.0f, 1.0f, 1.0f, {1.0f}};
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

    bool active        = true;
    bool castingShadow = false;
};

/// Base class of all render graph builders.
struct RenderGraphBuilder : public Artifact {
    /// \todo add public interface later.
protected:
    using Artifact::Artifact;
};

/// A container for shader constants that are shared by other effects.
/// For example, the frame related information, the camera information, light information and etc.
struct SharedShaderConstants : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "SharedShaderConstants";

    struct FrameInformation {
        uint32_t     frameCounter;
        Microseconds frameDuration;
    };

    struct ViewInformation {
        WorldToClipTransformChain transforms;
        AutoRef<RenderTarget>     renderTarget;
    };

    struct LightInformation {};

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    static GN_API AutoRef<SharedShaderConstants> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);
};

/// Represents lighting information of the surrounding world.
/// Can be, but not limited to, skybox, reflection probes, global illumination etc.
struct EnvironmentalLighting : public RenderGraphBuilder {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "EnvironmentalLighting";

    /// Sky lighting. If skybox texture is not provided, use the ambient as fallback.
    /// \todo: no need to put them here. move them to backend implementation.
    AutoRef<Texture> skybox;
    IrradianceRGB    ambient = {1.0f, 1.0f, 1.0f, {300.0f}}; /// default to 300 lux (normal office lighting).

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
    static GN_API AutoRef<EnvironmentalLighting> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

    /// Load a Pbr environment resource from external file.
    static GN_API AutoRef<EnvironmentalLighting> load(ArtifactDatabase & db, const StrA & name, const LoadParameters & params);

protected:
    using RenderGraphBuilder::RenderGraphBuilder;
};

/// Store shadow related information for all active lights, to be used
/// to determine whether a surface point is in shadow or not.
struct ShadowVisibility : public RenderGraphBuilder {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "ShadowVisibility";

    struct CreateParameters {
        // tbd
    };

    static GN_API AutoRef<ShadowVisibility> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using RenderGraphBuilder::RenderGraphBuilder;
};

struct SimpleForwardShadingPipeline : public Artifact {
    struct CreateParameters {
        RenderGraph * renderGraph = nullptr;
    };

    /// Direct lighting information. For now, is just a list of active lights.
    /// Later can be expanded to a dedicated DirectLighting artifact and workflows.
    DynaArray<SimpleLight> lights;

    /// The stage to render shadow maps
    DynaArray<Workflow *>     shadowPass;
    AutoRef<ShadowVisibility> shadowVisibility;

    /// The state to render/update environment maps.
    DynaArray<Workflow *>          environmentalPass;
    AutoRef<EnvironmentalLighting> environmentalLighting;

    /// \todo The stage to render depth prepass (maybe motion vector as well)

    /// The main stage for forward color pass.
    DynaArray<Workflow *> mainColorPass;

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
        AutoRef<Material>              material;
        GpuGeometry                    geometry;
        AffineTransform                modelToWorld;
        WorldToClipTransformChain      worldToClip;
        AutoRef<EnvironmentalLighting> env;
        AutoRef<ShadowVisibility>      shadow;
        AutoRef<RenderTarget>          renderTarget;
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
        AutoRef<RenderTarget> rt;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Add task graphs into the workflow to render a sky box.
    virtual Action::ExecutionResult build(Workflow & workflow, const BuildParameters & params);

    static GN_API AutoRef<SkyBox> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

} // namespace GN::rdg
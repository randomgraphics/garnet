#pragma once

namespace GN::rdg {

struct FrameInfo {
    /// Core frame info
    uint64_t    frameCounter  = 0; // A monotonically increasing counter of the frame.
    Nanoseconds frameDuration = Nanoseconds(0); // duration of the last known good frame in nanoseconds.
    Nanoseconds engineUptime  = Nanoseconds(0); // total run time since the engine started in nanoseconds.
};

struct CameraInfo {
    Matrix44f viewProjMatrix;
    // Matrix44f viewMatrix;
    // Matrix44f projMatrix;
    // Vector4f  cameraPosition;
    // Vector4f  cameraDirection;
    // Vector4f  cameraUp;
    // Vector4f  cameraRight;
    // float     cameraFov;
    // float     cameraAspectRatio;
    // float     cameraNearPlane;
    // float     cameraFarPlane;
    // float     cameraViewportWidth;
    // float     cameraViewportHeight;
    // float     cameraViewportX;
    // float     cameraViewportY;
};

/// Represents commonly used lighting information.
struct LightingInfo {
    /// Represents one light source in the scene.
    struct Light {
        enum Type : int {
            POINT,
            SPOT,
            DIRECTIONAL,
        };

        struct SpotLight {
            Location     location = {};
            Orientation  orientation = {};
            IntensityRGB intensity = {1.0f, 1.0f, 1.0f, 1.0f};
            WorldUnit    range = {};
            float        cosInnerConeAngle = 1.0f;
            float        cosOuterConeAngle = 1.0f;
        } spot;

        struct PointLight {
            Location     location;
            IntensityRGB intensity;
            float range = 0.0f;
        } point;

        struct DirectionalLight {
            Orientation  orientation;
            IrradianceRGB irradiance;
        } directional;

        /// Type of the light source.
        Type type;

        /// Variant of the light source. The order of the variant must match the Type enum. So you can check the type by calling light.index() directly.
        std::variant<SpotLight, PointLight, DirectionalLight> light;

        // TODO: add shadow information.
    };

    DynaArray<Light> lights;
};

/// Represents commonly used environment resources.
struct EnvironmentMap : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "Environment";

    IrradianceRGB    ambient = { 1.0f, 1.0f, 1.0f, 300.0f }; // default to 300 lux (normal office lighting).
    AutoRef<Texture> skybox;

    // TODO: more environment map types, like irradiance map, prefiltered map, etc.

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    struct LoadParameters {
        AutoRef<GpuContext> gpu;
        StrA                fileName;
    };

    /// Create a new blank (full black) environment resource.
    static GN_API AutoRef<EnvironmentMap> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

    /// Load a Pbr environment resource from external file.
    static GN_API AutoRef<EnvironmentMap> load(ArtifactDatabase & db, const StrA & name, const LoadParameters & params);
};

struct GN_API EffectContext {
    FrameInfo      frame;
    CameraInfo     camera;
    LightingInfo   lighting;
    EnvironmentMap env;
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
        GpuGeometry          geometry;
        RenderTarget         renderTarget;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Add task graphs into the workflow to render a PBR object.
    virtual Action::ExecutionResult build(Workflow & workflow, const EffectContext & context, const BuildParameters & params) = 0;

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
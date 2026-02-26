#pragma once

namespace GN::rdg {

struct PbrShading {
    /// PbrMaterial represents a PBR material resource.
    /// Backend is free to determine the best way to represent the material. Texture set, material parameter values, etc.
    struct Material : public GpuResource {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "PbrTextureSet";

        struct CreateParameters {
            AutoRef<GpuContext> gpu;
        };

        struct LoadParameters {
            AutoRef<GpuContext> gpu;
            StrA                fileName;
        };

        static GN_API AutoRef<Material> load(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

    protected:
        using GpuResource::GpuResource;
    };

    /// PbrEnvironment represents a PBR environment resource.
    /// Bacnend is free to determine the best way to represent the environment. Cubemap, latlong map, along with
    /// any other auxiliary data (like exposure, tone mapping, etc.) to define this environment.
    struct Environment : public GpuResource {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "PbrEnvironment";

        struct CreateParameters {
            AutoRef<GpuContext> gpu;
        };

        struct LoadParameters {
            AutoRef<GpuContext> gpu;
            StrA                fileName;
        };

        /// Create a new blank (full black) environment resource.
        static GN_API AutoRef<Environment> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

        /// Load a Pbr environment resource from external file.
        static GN_API AutoRef<Environment> load(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

    };


    struct Inputs {
        AutoRef<Material>    material;
        AutoRef<Environment> environment;

        /// \todo find a way to encapsulate these matrices into another concetp to allow
        /// each backend to customize it properly.
        /// For now, just use the standard matrix for simplicity.
        Matrix44f modelToWorld, worldToView, viewToClip;
    };

    struct Outputs {
        RenderTarget rt;
    };

    static GN_API Action::ExecutionResult build(const Inputs & inputs, Outputs & outputs);
};

} // namespace GN::rdg
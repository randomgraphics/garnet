#pragma once

namespace GN::rdg {

struct ShadowMapping {
    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    virtual ~ShadowMapping() = default;

    static GN_API ShadowMapping * create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    ShadowMapping() = default;
};

struct PbrShading {
    struct Inputs {
        AutoRef<GpuContext> gpu;

        /// Albedo map.
        TextureView albedo;

        /// Normal map.
        TextureView normal;

        /// Roughness-ambient-metallic map.
        /// Red channel: roughness
        /// Green channel: metallic
        /// Blue channel: optional ambient occlusion.
        TextureView ram;

        /// Separated occlusion map. Use a separate occlusion map because it is often in much lower resolution than the other maps.
        TextureView ao;

        /// Environment map.
        TextureView environment;

        /// Model-to-world matrix.
        Matrix44f modelMatrix;

        /// World-to-view matrix.
        Matrix44f viewMatrix;

        /// View-to-clip matrix.
        Matrix44f projMatrix;
    };

    struct Outputs {
        RenderTarget rt;
    };

    static GN_API Action::ExecutionResult build(const Inputs & inputs, Outputs & outputs);
};

} // namespace GN::rdg
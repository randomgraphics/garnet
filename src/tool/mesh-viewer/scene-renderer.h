#pragma once

#include "model-scene.h"
#include <garnet/GNfx2.h>

namespace GN::viewer {

/// Application-owned materials and imported geometry shared by scene instances.
struct RenderModel {
    struct Material {
        Shading                                    shading = Shading::PBR;
        AutoRef<fx2::bindless::PbrMaterial>        pbr;
        AutoRef<fx2::bindless::LambertianMaterial> lambertian;
        AutoRef<fx2::bindless::UnlitMaterial>      unlit;
        bool                                       doubleSided = false;
    };

    struct Instance {
        gpu2::RasterGeometry geometry;
        uint32_t             material  = 0;
        glm::mat4            transform = glm::mat4(1);
    };

    DynaArray<Material> materials;
    DynaArray<Instance> instances;

    bool prepare(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & geometryUploads, gpu2::bindless::CnC & materialUploads, AutoRef<fx2::bindless::PbrKernel> pbr,
                 AutoRef<fx2::bindless::LambertianKernel> lambertian, AutoRef<fx2::bindless::UnlitKernel> unlit, const ModelScene & scene, Shading shading,
                 const glm::mat4 & modelTransform);
};

/// Shared bindless scene renderer for windowed presentation and headless snapshots.
///
/// Cel shading has no bindless kernel yet: `--surface cel` is rejected by the option parser and
/// imported cel materials fall back to PBR. Add a bindless cel kernel field here when it lands.
struct SceneRenderer {
    AutoRef<gpu2::bindless::DescriptorHeap>       heap;
    AutoRef<fx2::bindless::SharedShaderConstants> ssc;
    AutoRef<fx2::bindless::PbrKernel>             pbr;
    AutoRef<fx2::bindless::LambertianKernel>      lambertian;
    AutoRef<fx2::bindless::UnlitKernel>           unlit;
    AutoRef<fx2::bindless::SkyKernel>             sky;
    AutoRef<fx2::bindless::SkyMaterial>           skyMaterial;
    RenderModel                                   model, bounds, axes;

    /// One frame's camera, viewport, and exposure inputs for the shared uniform block.
    struct FrameParameters {
        glm::vec3 eye         = glm::vec3(0);          ///< World-space camera position.
        glm::quat orientation = glm::quat(1, 0, 0, 0); ///< Camera-to-world rotation.
        uint32_t  width = 1, height = 1;               ///< Render target size in pixels.
        float     nearPlane = 0.01f, farPlane = 10000.f;
        float     fovDegrees      = 45.f;   ///< Vertical field of view for the projection.
        float     exposure        = 0.002f; ///< Linear radiance multiplier applied before tone mapping.
        float     frameDurationMs = 0.f;
        uint32_t  frame           = 0;
    };

    /// Create the descriptor heap, kernels, sky material, and geometry, then upload and wait.
    /// \p environmentLuminance calibrates the environment maps in nits and is baked into the
    /// immutable sky material, so it cannot change after this call.
    bool prepare(AutoRef<gpu2::GpuContext> gpu, const ModelScene & source, Shading shading, float environmentLuminance);

    /// Pack one frame's shared uniforms and record their upload on \p uploads. Submit the sealed
    /// recorder before every consuming payload; each consumer retains the returned state.
    AutoRef<fx2::bindless::SharedShaderConstants::UniformState> updateUniforms(gpu2::bindless::CnC & uploads, const FrameParameters & frame);

    /// Draws record() appends for the given debug toggles, for raster preallocation.
    size_t drawCount(bool showBounds, bool showAxes) const;

    bool record(gpu2::bindless::Raster & raster, const AutoRef<fx2::bindless::SharedShaderConstants::UniformState> & state, bool showBounds,
                bool showAxes) const;

private:
    bool recordModel(const RenderModel & source, gpu2::bindless::Raster & raster,
                     const AutoRef<fx2::bindless::SharedShaderConstants::UniformState> & state) const;
};

} // namespace GN::viewer

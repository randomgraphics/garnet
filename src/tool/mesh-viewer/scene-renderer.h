#pragma once
#include "model-scene.h"
#include <garnet/GNfx2.h>

namespace GN::viewer {
// Application-owned resources and imported values, shared by all instances.
struct RenderModel {
    struct Material {
        Shading                shading;
        fx2::PbrKernel::Inputs parameters;
        bool                   doubleSided = false;
    };
    struct Instance {
        gpu2::RasterGeometry geometry;
        uint32_t             material;
        glm::mat4            transform;
    };
    DynaArray<Material> materials;
    DynaArray<Instance> instances;
    bool                prepare(AutoRef<gpu2::GpuContext>, gpu2::GpuCnC &, const ModelScene &, Shading, const glm::mat4 &);
};

// One scene-recording path for both windowed presentation and headless snapshots.
struct SceneRenderer {
    AutoRef<fx2::SharedShaderConstants> ssc;
    AutoRef<fx2::PbrKernel>             pbr;
    AutoRef<fx2::CelKernel>             cel;
    AutoRef<fx2::LambertianKernel>      lambertian;
    AutoRef<fx2::UnlitKernel>           unlit;
    AutoRef<fx2::SkyboxKernel>          skybox;
    RenderModel                         model, bounds, axes;
    AutoRef<gpu2::GpuPayload>           initialization;
    bool                                prepare(AutoRef<gpu2::GpuContext>, const ModelScene &, Shading, float environmentLuminance);
    bool                                record(gpu2::GpuRaster &, gpu2::GpuCnC &, const gpu2::GpuResourceSet &, bool showBounds, bool showAxes) const;

private:
    bool recordModel(const RenderModel &, gpu2::GpuRaster &, gpu2::GpuCnC &, const gpu2::GpuResourceSet &) const;
};
} // namespace GN::viewer

#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif
#include <glm/mat4x4.hpp>
namespace GN::fx2 {
/// Immediate values and GPU bindings shared by lit raster effects. No CPU geometry ownership.
struct LitKernelInputs {
    /// Position xyz at 0, normal xyz at 1; UV xy at 2 when mapped, tangent xyzw at 3 for normal mapping, color rgba at 4 when enabled.
    gpu2::RasterGeometry geometry;
    /// Per-invocation caller overrides layered over the raster baseline.
    gpu2::RasterState states;
    glm::mat4         worldFromObject = glm::mat4(1);
    glm::vec4         color           = glm::vec4(1);
    glm::vec3         emissive        = glm::vec3(0);
    /// Reject alpha below this threshold; zero disables rejection.
    float alphaCutoff = 0;
    /// Force output alpha to one after alpha rejection.
    bool opaque         = true;
    bool useVertexColor = false;
    /// Optional sampled 2D views; absent maps use white and flat-normal defaults.
    gpu2::GpuResourceView colorMap, normalMap;
};
/// Metallic/roughness PBR raster effect. Inherits caller raster state.
struct PbrKernel : Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(Kernel);
    struct Inputs : LitKernelInputs {
        float                 metallic = 0, roughness = 1;
        gpu2::GpuResourceView emissiveMap, occlusionMap, metalRoughMap;
    };
    /// Records fallback texture initialization. Submit initialization before any use; no submission or wait occurs here.
    GN_API static AutoRef<PbrKernel> create(AutoRef<gpu2::GpuContext>, gpu2::GpuCnC & initialization);
    /// Appends draws and copies private values into prerequisite uploads. Schedule uploads before this raster.
    /// Shared bindings follow SSC's single-version ordering. Resources must belong to this kernel's device.
    /// False appends no draws. Private parameter buffers are unique to each invocation.
    virtual bool record(gpu2::GpuRaster &, gpu2::GpuCnC & prerequisiteUploads, const gpu2::GpuResourceSet & shared, const Inputs &) const = 0;

protected:
    using Kernel::Kernel;
};
/// Diffuse Lambertian raster effect. Inherits caller raster state.
struct LambertianKernel : Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(Kernel);
    struct Inputs : LitKernelInputs {
        float ambientIntensity = 1;
    };
    /// Records fallback texture initialization. Submit initialization before any use; no submission or wait occurs here.
    GN_API static AutoRef<LambertianKernel> create(AutoRef<gpu2::GpuContext>, gpu2::GpuCnC & initialization);
    /// Appends draws and copies private values into prerequisite uploads. Schedule uploads before this raster.
    /// Shared bindings follow SSC's single-version ordering. Resources must belong to this kernel's device.
    /// False appends no draws. Private parameter buffers are unique to each invocation.
    virtual bool record(gpu2::GpuRaster &, gpu2::GpuCnC & prerequisiteUploads, const gpu2::GpuResourceSet & shared, const Inputs &) const = 0;

protected:
    using Kernel::Kernel;
};
/// Typed Cel raster effect. Inherits caller raster state; Cel outline overrides culling and depth.
struct CelKernel : Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(Kernel);
    struct Inputs : LitKernelInputs {
        gpu2::GpuResourceView emissiveMap, occlusionMap;
        float                 shadowThreshold     = 0.5f;
        float                 shadowFeather       = 0.02f;
        float                 deepShadowThreshold = 0.25f;
        float                 deepShadowFeather   = 0.02f;
        glm::vec3             shadowTint          = glm::vec3(0.6f, 0.65f, 0.75f);
        glm::vec3             deepShadowTint      = glm::vec3(0.4f, 0.42f, 0.52f);
        float                 specularThreshold   = 0.7f;
        float                 specularShininess   = 32;
        float                 specularIntensity   = 1;
        float                 rimIntensity        = 0.5f;
        float                 rimThreshold        = 0.6f;
        float                 rimFeather          = 0.05f;
        glm::vec3             rimTint             = glm::vec3(1);
        float                 outlineWidth        = 0.003f;
        glm::vec4             outlineColor        = glm::vec4(0.15f, 0.12f, 0.15f, 1);
    };
    /// Records fallback texture initialization. Submit initialization before any use; no submission or wait occurs here.
    GN_API static AutoRef<CelKernel> create(AutoRef<gpu2::GpuContext>, gpu2::GpuCnC & initialization);
    /// Appends draws and copies private values into prerequisite uploads. Schedule uploads before this raster.
    /// Shared bindings follow SSC's single-version ordering. Resources must belong to this kernel's device.
    /// False appends no draws. Private parameter buffers are unique to each invocation.
    virtual bool record(gpu2::GpuRaster &, gpu2::GpuCnC & prerequisiteUploads, const gpu2::GpuResourceSet & shared, const Inputs &) const = 0;

protected:
    using Kernel::Kernel;
};
} // namespace GN::fx2

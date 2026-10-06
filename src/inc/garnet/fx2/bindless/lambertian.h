#pragma once
#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>

namespace GN::fx2::bindless {

/// Immutable Lambertian material retaining its kernel, stable values, and heap allocations.
struct LambertianMaterial : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Fixed shader input locations used by the Lambertian kernel.
    static constexpr uint32_t POSITION_LOCATION = 0;
    static constexpr uint32_t NORMAL_LOCATION   = 1;
    static constexpr uint32_t TEXCOORD_LOCATION = 2;

    /// Stable material data; GPU packing is private to the kernel implementation.
    struct Parameters {
        glm::vec4              color             = glm::vec4(1);
        glm::vec3              emissive          = glm::vec3(0);
        float                  diffuseMultiplier = 1.0f;
        gpu2::GpuResourceView  colorMap;
        gpu2::GpuResourceView  normalMap;
        AutoRef<gpu2::Sampler> sampler; ///< leave empty to use built-in default sampler.
    };

    /// Per-draw geometry, state, and transform for Lambertian.
    struct DrawParameters : CommonDrawParameters {
        glm::mat4 object2WorldTransform = glm::mat4(1);
    };

    virtual bool record(const DrawParameters &) const = 0;

protected:
    using RCRT64::RCRT64;
};

/// Lambertian diffuse raster kernel.
struct LambertianKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    virtual LambertianMaterial::Parameters defaultMaterialParameters() const = 0;
    virtual AutoRef<LambertianMaterial>    createMaterial(gpu2::bindless::CnC & initialization, const LambertianMaterial::Parameters &) const = 0;

    GN_API static AutoRef<LambertianKernel> create(gpu2::bindless::DescriptorHeap & heap, gpu2::bindless::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2::bindless

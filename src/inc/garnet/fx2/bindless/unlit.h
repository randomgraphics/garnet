#pragma once
#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>

namespace GN::fx2::bindless {

/// Common raster draw inputs shared by bindless raster materials.
struct CommonDrawParameters {
    gpu2::bindless::Raster &                     raster;
    AutoRef<SharedShaderConstants::UniformState> ssc;
    const gpu2::RasterGeometry &                 geometry;
    const gpu2::RasterState *                    states = nullptr;
};

/// Immutable unlit material retaining its kernel, stable values, and heap allocations.
struct UnlitMaterial : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Fixed shader input locations used by the unlit kernel.
    static constexpr uint32_t POSITION_LOCATION = 0;
    static constexpr uint32_t TEXCOORD_LOCATION = 2;

    /// Stable material data; GPU packing is private to the kernel implementation.
    struct Parameters {
        glm::vec4              color    = glm::vec4(1);
        glm::vec3              emissive = glm::vec3(0);
        gpu2::GpuResourceView  colorMap; ///< Optional sampled 2D view; empty uses the built-in white texture.
        AutoRef<gpu2::Sampler> sampler;  ///< Empty uses the built-in sampler.
    };

    /// Per-draw geometry, state, and transform for Unlit.
    struct DrawParameters : CommonDrawParameters {
        glm::mat4 object2WorldTransform = glm::mat4(1);
    };

    /// Append a draw and retain this material and uniform state until payload cleanup.
    virtual bool record(const DrawParameters &) const = 0;

protected:
    using RCRT64::RCRT64;
};

/// Unlit raster kernel. It owns its heap reference, shared shaders, and fallback resources.
/// Materials created by this kernel use the same heap and retain the kernel.
struct UnlitKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    virtual UnlitMaterial::Parameters defaultMaterialParameters() const = 0;
    virtual AutoRef<UnlitMaterial>    createMaterial(gpu2::bindless::CnC & initialization, const UnlitMaterial::Parameters &) const = 0;

    GN_API static AutoRef<UnlitKernel> create(gpu2::bindless::DescriptorHeap & heap, gpu2::bindless::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2::bindless

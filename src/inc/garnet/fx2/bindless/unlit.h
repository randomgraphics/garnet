#pragma once
#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>

namespace GN::fx2 {

/// Common raster draw inputs shared by bindless raster materials.
struct CommonDrawParameters {
    ::GN::gpu2::Raster &                         raster;
    AutoRef<SharedShaderConstants::UniformState> ssc;
    const gpu2::RasterGeometry &                 geometry;
    const gpu2::RasterState *                    states = nullptr;
};

/// Immutable unlit material retaining stable values and heap allocations.
struct UnlitMaterial : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Fixed shader input locations used by the unlit kernel.
    static constexpr uint32_t POSITION_LOCATION = 0;
    static constexpr uint32_t TEXCOORD_LOCATION = 2;

    /// Stable material data; GPU packing is private to the kernel implementation.
    struct Parameters {
        glm::vec4              color    = glm::vec4(1);
        glm::vec3              emissive = glm::vec3(0);
        /// Apply SSC exposure and Reinhard mapping after composition; disabled by default.
        bool                   toneMapWithExposure = false;
        gpu2::GpuResourceView  colorMap; ///< Optional sampled 2D view; empty uses the built-in white texture.
        AutoRef<gpu2::Sampler> sampler;  ///< Empty uses the built-in sampler.
    };

    /// Per-draw geometry, state, and transform for Unlit.
    struct DrawParameters : CommonDrawParameters {
        glm::mat4 object2WorldTransform = glm::mat4(1);
    };

    /// Create heap-backed material data independent of a rendering kernel.
    GN_API static AutoRef<UnlitMaterial> create(::GN::gpu2::DescriptorHeap & heap, ::GN::gpu2::CnC & initialization, const Parameters & parameters);

protected:
    using RCRT64::RCRT64;
};

/// Unlit raster kernel that records draws using heap-compatible materials.
struct UnlitKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    virtual bool record(const UnlitMaterial & material, const UnlitMaterial::DrawParameters & parameters) const = 0;

    GN_API static AutoRef<UnlitKernel> create(::GN::gpu2::DescriptorHeap & heap, ::GN::gpu2::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2

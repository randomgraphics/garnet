#pragma once
#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace GN::fx2 {

/// Immutable Cel (anime/NPR) material retaining stable values and heap allocations.
struct CelMaterial : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Fixed shader input locations used by the Cel kernel.
    static constexpr uint32_t POSITION_LOCATION = 0;
    static constexpr uint32_t NORMAL_LOCATION   = 1;
    static constexpr uint32_t TEXCOORD_LOCATION = 2;

    /// Stable material data; GPU packing is private to the kernel implementation.
    struct Parameters {
        glm::vec4              color               = glm::vec4(1.0f);
        glm::vec3              emissive            = glm::vec3(0.0f);
        float                  shadowThreshold     = 0.5f;
        float                  shadowFeather       = 0.02f;
        float                  deepShadowThreshold = 0.25f;
        float                  deepShadowFeather   = 0.02f;
        glm::vec3              shadowTint          = glm::vec3(0.6f, 0.65f, 0.75f);
        glm::vec3              deepShadowTint      = glm::vec3(0.4f, 0.42f, 0.52f);
        float                  specularThreshold   = 0.7f;
        float                  specularShininess   = 32.0f;
        float                  specularIntensity   = 1.0f;
        float                  rimThreshold        = 0.6f;
        float                  rimFeather          = 0.05f;
        float                  rimIntensity        = 0.5f;
        glm::vec3              rimTint             = glm::vec3(1.0f);
        float                  outlineWidth        = 0.003f;
        glm::vec4              outlineColor        = glm::vec4(0.15f, 0.12f, 0.15f, 1.0f);
        gpu2::GpuResourceView  colorMap;
        gpu2::GpuResourceView  normalMap;
        AutoRef<gpu2::Sampler> sampler; ///< leave empty to use built-in default sampler.
    };

    /// Per-draw geometry, state, and transform for Cel shading.
    struct DrawParameters : CommonDrawParameters {
        glm::mat4 object2WorldTransform = glm::mat4(1.0f);
        bool      renderOutline         = true; ///< Record inverted-hull silhouette outline draw when true and outlineWidth > 0.
    };

    /// Create heap-backed material data independent of a rendering kernel.
    GN_API static AutoRef<CelMaterial> create(::GN::gpu2::DescriptorHeap & heap, ::GN::gpu2::CnC & initialization, const Parameters & parameters);

protected:
    using RCRT64::RCRT64;
};

/// Cel (NPR) raster kernel that records draws using heap-compatible materials.
struct CelKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    virtual bool record(const CelMaterial & material, const CelMaterial::DrawParameters & parameters) const = 0;

    GN_API static AutoRef<CelKernel> create(::GN::gpu2::DescriptorHeap & heap, ::GN::gpu2::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2

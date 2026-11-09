#pragma once
#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

namespace GN::fx2 {

/// Immutable sky / environment material retaining stable values and heap allocations.
struct SkyMaterial : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Stable material parameters; GPU packing is private to the kernel implementation.
    struct Parameters {
        /// Cubemap texture used to render the background skybox.
        gpu2::GpuResourceView skyboxMap;
        /// Diffuse irradiance cubemap used for IBL.
        gpu2::GpuResourceView irradianceMap;
        /// Specular reflection prefiltered cubemap used for IBL.
        gpu2::GpuResourceView prefilteredMap;
        /// 2D split-sum BRDF lookup texture.
        gpu2::GpuResourceView brdfLut;
        /// Sampler for cubemaps (leave empty to use built-in linear clamp sampler).
        AutoRef<gpu2::Sampler> sampler;
        /// Sampler for 2D LUT (leave empty to use built-in linear clamp sampler).
        AutoRef<gpu2::Sampler> lutSampler;
        /// Multiplier converting linear environment RGB to nits. Default is 1.0f.
        float luminanceScale = 1.0f;
        /// Debug ambient floor in nits. Default is 0.0f.
        float ambientFloor = 0.0f;
    };

    /// Draw inputs for skybox rendering.
    struct DrawParameters {
        ::GN::gpu2::Raster &                         raster;
        AutoRef<SharedShaderConstants::UniformState> ssc;
        const gpu2::RasterState *                    states = nullptr;
    };

    /// Create heap-backed environment data independent of a rendering kernel.
    GN_API static AutoRef<SkyMaterial> create(::GN::gpu2::DescriptorHeap & heap, ::GN::gpu2::CnC & initialization, const Parameters & parameters);

protected:
    using RCRT64::RCRT64;
};

/// Skybox and environment lighting raster kernel.
struct SkyKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    virtual bool record(const SkyMaterial & material, const SkyMaterial::DrawParameters & parameters) const = 0;

    GN_API static AutoRef<SkyKernel> create(::GN::gpu2::DescriptorHeap & heap, ::GN::gpu2::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2

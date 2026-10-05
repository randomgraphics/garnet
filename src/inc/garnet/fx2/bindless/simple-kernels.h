#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>

namespace GN::fx2::bindless {

/// Build pass bindings from a captured SharedUniforms state; invalid views yield an empty table.
/// Supply the table when creating the caller's raster. It retains the buffer, not the uniform
/// lease; each material draw retains that lease until payload cleanup.
GN_API gpu2::GpuResourceTable sharedUniformResources(const AutoRef<SharedShaderConstants::UniformState> & state);

struct CommonDrawParameter {
    gpu2::bindless::Raster & raster;
    AutoRef<SharedShaderConstants::UniformState> ssc;
    gpu2::RasterGeometry geometry; ///< Position at location 0; UV at location 2 when a custom map is used.
    gpu2::RasterState    states;
};

/// Immutable unlit material retaining its kernel, stable values, and heap allocations.
struct UnlitMaterial : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Stable material data; GPU packing is private to the kernel implementation.
    struct Parameters {
        glm::vec4              color       = glm::vec4(1);
        glm::vec3              emissive    = glm::vec3(0);
        float                  alphaCutoff = 0;
        gpu2::GpuResourceView  colorMap; ///< Optional sampled 2D view; empty uses the built-in white texture.
        AutoRef<gpu2::Sampler> sampler;  ///< Empty uses the built-in sampler.
    };

    /// Per-draw geometry and transform; GPU representation remains private.
    struct DrawParameters : CommonDrawParameter {
        glm::mat4 object2WorldTransform = glm::mat4(1);
    };

    /// Append a draw and retain this material and uniform state until payload cleanup.
    /// Raster must use sharedUniformResources(state), this kernel's heap at set 0, and the
    /// default immediate-data capacity. All resources must share the creation GPU.
    virtual bool record(const DrawParameters &) const = 0;

protected:
    using RCRT64::RCRT64;
};

/// Unlit raster kernel. It owns its heap reference, shared shaders, and fallback resources.
/// Materials created by this kernel use the same heap and retain the kernel.
struct UnlitKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Return user-facing defaults, including the kernel's private fallback resources.
    virtual UnlitMaterial::Parameters defaultMaterialParameters() const = 0;

    /// Create immutable material data and record its upload. Producer retains the material
    /// through upload completion or discard; all GPU resources must belong to this kernel's GPU.
    virtual AutoRef<UnlitMaterial> createMaterial(gpu2::bindless::CnC & initialization, const UnlitMaterial::Parameters & parameters) const = 0;

    /// Create kernel shader/fallback state and record fallback uploads in the caller's producer.
    /// The heap reference must be non-null; the kernel retains it after creation.
    GN_API static AutoRef<UnlitKernel> create(gpu2::bindless::DescriptorHeap & heap, gpu2::bindless::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

/// Immutable Lambertian material retaining its kernel, stable values, and heap allocations.
struct LambertianMaterial : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Stable material data; GPU packing is private to the kernel implementation.
    struct Parameters {
        glm::vec4              color       = glm::vec4(1);
        glm::vec3              emissive    = glm::vec3(0);
        float                  alphaCutoff = 0; ///< Alpha rejection threshold in [0,1].
        gpu2::GpuResourceView  colorMap;        ///< Optional sampled 2D view; empty uses built-in white texture.
        gpu2::GpuResourceView  normalMap;       ///< Optional tangent-space normal map; empty uses built-in flat normal.
        AutoRef<gpu2::Sampler> sampler;         ///< Optional sampler shared by both maps; empty uses built-in sampler.
        float                  diffuseMultiplier = 1;    ///< Nonnegative diffuse multiplier; emissive is unaffected.
        bool                   opaque            = true; ///< Force surviving fragment alpha to one.
    };

    /// Per-draw geometry and transform; GPU representation remains private.
    struct DrawParameters : CommonDrawParameter {
        glm::mat4 object2WorldTransform = glm::mat4(1);
    };

    /// Append a draw and retain this material and uniform state until payload cleanup.
    virtual bool record(const DrawParameters &) const = 0;

protected:
    using RCRT64::RCRT64;
};

/// Lambertian raster kernel with legacy direct lights, normal mapping, exposure, and Reinhard.
struct LambertianKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Return user-facing defaults, including this kernel's fallback resources.
    virtual LambertianMaterial::Parameters defaultMaterialParameters() const = 0;
    /// Create immutable material data and record its upload. The producer retains the material
    /// through completion/discard; all GPU resources must belong to this kernel's GPU.
    virtual AutoRef<LambertianMaterial> createMaterial(gpu2::bindless::CnC & initialization, const LambertianMaterial::Parameters & parameters) const = 0;

    /// Create shared shader/fallback state and record fallback uploads in the caller's producer.
    GN_API static AutoRef<LambertianKernel> create(gpu2::bindless::DescriptorHeap & heap, gpu2::bindless::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2::bindless

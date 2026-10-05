#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>

namespace GN::fx2::bindless {

/// Build pass bindings from a captured SharedUniforms state. Returns an empty table
/// for an invalid uniform view. Set 0 remains empty for the caller's heap.
/// Set Raster::CreateParameters::passResources before creating the recorder. The table
/// retains the buffer only; keep state alive until all consumers complete or discard.
GN_API gpu2::GpuResourceTable sharedUniformResources(const AutoRef<SharedShaderConstants::UniformState> & state);

/// Immediate per-draw values. Texture descriptors are allocated separately by the caller in the gpu2 heap.
struct SimpleMeshInputs {
    gpu2::RasterGeometry geometry; ///< Position at 0; Lambertian also needs normal at 1; float16/float32 xyz.
    /// Optional sampled 2D descriptor; zero selects white. Requires UV at location 2.
    gpu2::bindless::DescriptorHeap::DescriptorIndex colorMap = {};
    /// Required with either map; shared by color and normal sampling.
    gpu2::bindless::DescriptorHeap::DescriptorIndex sampler = {};
    gpu2::RasterState                               states; ///< Caller policy with per-draw overrides; kernels add no state overrides.
    glm::mat4                                       worldFromObject = glm::mat4(1);
    glm::vec4                                       color           = glm::vec4(1);
    glm::vec3                                       emissive        = glm::vec3(0);
    float                                           alphaCutoff     = 0; ///< Reject fragments below this alpha, in [0,1].
};

/// Color/texture raster effect that bypasses lighting, exposure, and tone mapping.
struct UnlitKernel : fx2::Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(fx2::Kernel);
    using Inputs = SimpleMeshInputs;
    /// Create shaders without submitting work; returns an empty ref on failure.
    GN_API static AutoRef<UnlitKernel> create(AutoRef<gpu2::GpuContext> gpu);
    /// Append a draw and retain state until completion/discard. Raster must use
    /// sharedUniformResources(state), heap set 0 with bindingIndex 0, and at least 128 immediate bytes.
    /// All resources must share the creation GPU; schedule the state's producer first.
    /// Descriptor indices must belong to that heap and remain allocated/unchanged until
    /// every recorded/in-flight consumer completes or is discarded. Follow gpu2's read-ready
    /// invariant: submit writers before readers; never sample an active render attachment.
    /// False appends no draw. No SSC internals or shared uploads are accessed.
    virtual bool record(gpu2::bindless::Raster &, AutoRef<SharedShaderConstants::UniformState> state, const Inputs &) const = 0;

protected:
    using fx2::Kernel::Kernel;
};

/// Lambertian diffuse with legacy FX2 direct-light records, inverse-transpose
/// normals, back-face normal flipping, exposure, and Reinhard tone mapping.
struct LambertianKernel : fx2::Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(fx2::Kernel);
    struct Inputs : SimpleMeshInputs {
        /// Optional tangent-space normal map; requires UV at 2. Tangents are derived from
        /// world-position/UV derivatives, so no tangent vertex attribute is required.
        gpu2::bindless::DescriptorHeap::DescriptorIndex normalMap         = {};
        float                                           diffuseMultiplier = 1;    ///< Nonnegative diffuse multiplier; emissive is unaffected.
        bool                                            opaque            = true; ///< Force surviving fragment alpha to one.
    };
    /// Create shaders without submitting work; returns an empty ref on failure.
    GN_API static AutoRef<LambertianKernel> create(AutoRef<gpu2::GpuContext> gpu);
    /// Same pass/lifetime contract as UnlitKernel::record; world linear transform must be invertible.
    virtual bool record(gpu2::bindless::Raster &, AutoRef<SharedShaderConstants::UniformState> state, const Inputs &) const = 0;

protected:
    using fx2::Kernel::Kernel;
};

} // namespace GN::fx2::bindless

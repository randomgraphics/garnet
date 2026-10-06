#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

namespace GN::fx2::bindless {

struct SkyMaterial;

/// Immutable metallic/roughness material with a PBR-specific parameter contract.
struct PbrMaterial : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    struct Parameters {
        /// Linear base reflectance and alpha; alpha is preserved when opaque is false.
        glm::vec4 baseColor = glm::vec4(1);
        glm::vec3 emissive  = glm::vec3(0);
        /// Metallic and perceptual roughness factors. Roughness is clamped to [0.04, 1] in shading.
        float metallic  = 0;
        float roughness = 1;
        /// Multiplier for sampled tangent-space normals and strength of the occlusion map.
        float normalScale       = 1;
        float occlusionStrength = 1;
        bool  opaque            = true;
        /// Optional base-color, tangent-space normal, emissive, occlusion, and metallic/roughness maps.
        gpu2::GpuResourceView  baseColorMap, normalMap, emissiveMap, occlusionMap, metalRoughMap;
        AutoRef<gpu2::Sampler> sampler; ///< leave empty to use built-in default sampler.
    };

    /// Fixed shader input locations used by the PBR kernel.
    static constexpr uint32_t POSITION_LOCATION = 0;
    static constexpr uint32_t NORMAL_LOCATION   = 1;
    static constexpr uint32_t TEXCOORD_LOCATION = 2;

    /// Per-draw geometry, state, and transform for PBR.
    struct DrawParameters : CommonDrawParameters {
        glm::mat4            object2WorldTransform = glm::mat4(1);
        AutoRef<SkyMaterial> skyMaterial;
    };

    /// Append this material's draw. Position, normal, and UV attributes are required.
    virtual bool record(const DrawParameters &) const = 0;

protected:
    using RCRT64::RCRT64;
};

/// Metallic/roughness PBR raster kernel with direct lights and typed materials.
struct PbrKernel : RCRT64 {
    GN_API                           GN_REGISTER_RUNTIME_TYPE(RCRT64);
    virtual PbrMaterial::Parameters  defaultMaterialParameters() const                                                                      = 0;
    virtual AutoRef<PbrMaterial>     createMaterial(gpu2::bindless::CnC & initialization, const PbrMaterial::Parameters & parameters) const = 0;
    GN_API static AutoRef<PbrKernel> create(gpu2::bindless::DescriptorHeap & heap, gpu2::bindless::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2::bindless

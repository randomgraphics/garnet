#if !defined(__GN_INSIDE_FX2_H__)
    #error "Do not include <garnet/fx2/cel-shading.h> directly. Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace GN::fx2 {

/// Stylized cel / non-photorealistic (NPR) anime shading state and draw generator for ModelAsset instances.
///
/// CelModelShading produces a Genshin Impact / modern anime aesthetic:
/// - Quantized multi-band cartoon diffuse ramps with custom cool/warm shadow tints.
/// - Crisp stepped cartoon specular highlights.
/// - Fresnel anime rim lighting biased toward silhouette edges.
/// - Inverted-hull mesh outline passes with clip-space aspect-ratio-corrected normal extrusion.
///
/// It operates on unchanged ModelAsset instances, allowing any loaded model to switch between
/// photorealistic PBR (ModelShading) and stylized anime cel shading (CelModelShading).
struct CelModelShading {
    CelModelShading() = delete;

    /// Art-directed tuning parameters for anime cel shading.
    struct Config {
        /// Primary shadow boundary threshold in Half-Lambert space [0..1].
        /// Default 0.5f separates lit surfaces from half-shadow bands.
        float shadowThreshold = 0.5f;

        /// Softness / feather width of the primary shadow boundary.
        /// Small values (0.01 - 0.03) produce crisp anime boundaries. Default 0.02f.
        float shadowFeather = 0.02f;

        /// Secondary (deep shadow / crease) threshold in Half-Lambert space [0..1].
        /// Default 0.25f defines core shadows and occluded crevices.
        float deepShadowThreshold = 0.25f;

        /// Softness / feather width of the deep shadow boundary. Default 0.02f.
        float deepShadowFeather = 0.02f;

        /// Color tint multiplier applied to base color in primary shadow areas.
        /// Default (0.6f, 0.65f, 0.75f) provides a stylized anime cool-shadow shift.
        glm::vec3 shadowTint = glm::vec3(0.6f, 0.65f, 0.75f);

        /// Color tint multiplier applied in deep shadow areas.
        /// Default (0.4f, 0.42f, 0.52f).
        glm::vec3 deepShadowTint = glm::vec3(0.4f, 0.42f, 0.52f);

        /// Specular threshold in Blinn-Phong lobe [0..1].
        /// Exponentiation lobe above this threshold is clipped to a solid cartoon highlight. Default 0.7f.
        float specularThreshold = 0.7f;

        /// Specular shininess exponent. Default 32.0f.
        float specularShininess = 32.0f;

        /// Specular highlight intensity multiplier. Default 1.0f.
        float specularIntensity = 1.0f;

        /// Rim lighting intensity multiplier. Default 0.5f.
        float rimIntensity = 0.5f;

        /// Rim lighting Fresnel threshold [0..1]. Default 0.6f.
        float rimThreshold = 0.6f;

        /// Rim lighting feather width. Default 0.05f.
        float rimFeather = 0.05f;

        /// Tint color of the rim highlight. Default (1.0f, 1.0f, 1.0f).
        glm::vec3 rimTint = glm::vec3(1.0f);

        /// Inverted-hull silhouette outline extrusion width in clip-space units.
        /// Setting to 0.0f disables outline draws. Default 0.003f.
        float outlineWidth = 0.003f;

        /// Inverted-hull silhouette outline RGBA color.
        /// Default dark charcoal/sepia anime ink: (0.15f, 0.12f, 0.15f, 1.0f).
        glm::vec4 outlineColor = glm::vec4(0.15f, 0.12f, 0.15f, 1.0f);
    };

    /// Opaque shared GPU resources (shaders, parameter buffers, fallback textures) for cel shading.
    struct Asset : RCRT64 {
        GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

        /// Initialization payload for shaders and resources. Submit once before first use.
        virtual AutoRef<gpu2::GpuPayload> uploadPayload() const = 0;

        /// Update runtime configuration parameters on the GPU.
        virtual void updateConfig(const Config & config) = 0;

        /// Current configuration settings.
        virtual const Config & config() const = 0;

    protected:
        using RCRT64::RCRT64;
    };

    /// Create shared cel shading GPU resources with default tuning.
    GN_API static AutoRef<Asset> create(AutoRef<gpu2::GpuContext> gpu);

    /// Create shared cel shading GPU resources with custom tuning.
    GN_API static AutoRef<Asset> create(AutoRef<gpu2::GpuContext> gpu, const Config & config);

    /// Build one primitive surface draw parameter set.
    /// Uses backface culling (CULL_BACK) and the anime multi-band surface shader.
    GN_API static gpu2::GpuRaster::DrawParameters getDrawParams(const SharedShaderConstants::Snapshot & sscSnapshot, AutoRef<const Asset> shading,
                                                                AutoRef<const ModelAsset> model, uint32_t primitiveIndex, const glm::mat4 & worldTransform);

    /// Build one primitive inverted-hull outline draw parameter set.
    /// Uses frontface culling (CULL_FRONT) to render extruded backfaces forming a silhouette outline.
    /// Returns empty DrawParameters if outlineWidth <= 0.0f or if the primitive is not outlineable.
    GN_API static gpu2::GpuRaster::DrawParameters getOutlineDrawParams(const SharedShaderConstants::Snapshot & sscSnapshot, AutoRef<const Asset> shading,
                                                                       AutoRef<const ModelAsset> model, uint32_t primitiveIndex,
                                                                       const glm::mat4 & worldTransform);
};

} // namespace GN::fx2

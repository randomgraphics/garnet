#if !defined(__GN_INSIDE_FX2_H__)
    #error "Do not include <garnet/fx2/pbr.h> directly. Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>

namespace GN::fx2 {

/// Namespace struct grouping PBR asset types and operations.
/// All members are static — PbrShading is never instantiated.
struct PbrShading {
    PbrShading() = delete;

    struct LoadParameters {
        StrA gltfPath;
        StrA albedoPath, normalPath, emissivePath, occlusionPath, metalRoughPath;
    };

    /// Opaque loaded PBR asset. Implementation keeps shaders, material textures, and mesh data private.
    struct Asset : RCRT64 {
        GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

        AutoRef<gpu2::GpuPayload> gpuPayload;

    protected:
        using RCRT64::RCRT64;
    };

    /// Load one PBR asset synchronously. Missing files fall back per-slot to built-in defaults.
    static GN_API AutoRef<Asset> load(AutoRef<gpu2::GpuContext> gpu, const LoadParameters & params);

    /// Build DrawParameters for one PBR mesh draw.
    /// The asset's gpuPayload must be submitted before the first draw that uses it.
    static GN_API gpu2::GpuRaster::DrawParameters getDrawParams(const SharedShaderConstants::Snapshot & sscSnapshot, AutoRef<const Asset> pbrAsset,
                                                                const glm::mat4 & worldTransform);
};

} // namespace GN::fx2

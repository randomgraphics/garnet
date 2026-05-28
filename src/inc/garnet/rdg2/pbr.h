#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/pbr.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

namespace GN::rdg2 {

/// Namespace struct grouping PBR asset types and operations.
/// All members are static — PbrShading is never instantiated.
struct PbrShading {
    PbrShading() = delete;

    struct LoadParameters {
        StrA gltfPath;
        StrA albedoPath, normalPath, emissivePath, occlusionPath, metalRoughPath;
    };

    /// Opaque loaded PBR asset. Implementation keeps shaders, material textures, and mesh data private.
    struct Asset : Entity {
        GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

        AutoRef<gpu2::GpuPayload> gpuPayload;

    protected:
        using Entity::Entity;
    };

    /// Load one PBR asset synchronously. Missing files fall back per-slot to built-in defaults.
    static GN_API AutoRef<Asset> load(AutoRef<gpu2::GpuContext> gpu, const LoadParameters & params);

    /// Build DrawParameters for one PBR mesh draw.
    /// The asset's gpuPayload must be submitted before the first draw that uses it.
    static GN_API gpu2::GpuRaster::DrawParameters getDrawParams(const SharedShaderConstants::Snapshot & sscSnapshot, AutoRef<const Asset> pbrAsset,
                                                                const glm::mat4 & worldTransform);
};

} // namespace GN::rdg2

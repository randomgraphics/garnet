#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/simple-effect.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

namespace GN::rdg2 {

/// Namespace struct grouping PBR asset types and operations.
/// All members are static — PbrShading is never instantiated.
struct PbrShading {
    PbrShading() = delete;

    struct LoadParameters {
        StrA gltfPath;
        StrA albedoPath, normalPath, emissivePath, occlusionPath, metalRoughPath;
        bool simulateSlowLoading = false; ///< sleep ~5 s on worker during staging writes
    };

    /// Public content of the loaded PBR asset.
    struct Content : Entity {
        GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

        AutoRef<gpu2::GpuPayload> gpuPayload;

    protected:
        using Entity::Entity;
    };

    /// Start loading a PBR asset asynchronously.
    /// Compiles shaders and publishes a default opaque asset (if it is the very fist time to load this particular asset)
    /// before returning.
    /// Async IO loads real textures/geometry and publishes v2 when done.
    /// Call multiple times to create independent per-object artifacts.
    static GN_API VersionedArtifact load(AutoRef<gpu2::GpuContext> gpu, AutoRef<Graph> graph, const LoadParameters & params);

    /// Get the latest public content of the PBR asset artifact.
    static GN_API AutoRef<Content> getContent(AutoRef<Graph> graph, ArtifactPtr assetArtifact);

    /// Build DrawParameters for one PBR mesh draw.
    /// Must be called after the artifact's ready token is satisfied.
    static GN_API gpu2::GpuRaster::DrawParameters getDrawParams(AutoRef<const SharedShaderConstants::Content> sscContent, AutoRef<const Content> pbrContent,
                                                                const glm::mat4 & worldTransform);
};

} // namespace GN::rdg2

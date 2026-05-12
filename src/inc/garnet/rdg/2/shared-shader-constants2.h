#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg/2/open-graph.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

namespace GN::rdg2 {

struct SharedShaderConstants2 : public Entity {

    using FrameConstants               = GN::rdg::SharedShaderConstants::FrameInformation;
    using DirectLight                  = GN::rdg::SharedShaderConstants::DirectLight;
    using EnvironmentLightingConstants = GN::rdg::SharedShaderConstants::EnvironmentLightingInformation;

    struct CameraConstants {
        Location    cameraPosition    = {0, 0, 0};            ///< camera position in world space
        Orientation cameraOrientation = {1.f, 0.f, 0.f, 0.f}; ///< camera orientation (world space)
        Radian      cameraFov         = Degree(60.f);         ///< vertical field of view
        float       aspectRatio       = 16.f / 9.f;           ///< viewport width / height
        float       nearPlane         = 0.01f;                ///< near clip plane in meters
        float       farPlane          = 10000.f;              ///< far clip plane in meters
    };

    struct Set0Parameters {
        FrameConstants               frameConstants;
        GN::gpu2::RasterTarget       renderTarget;
        CameraConstants              camera;
        DynaArray<DirectLight>       directLighting;
        EnvironmentLightingConstants environmentLighting;
    };

    struct Content : RefCounter {
        Set0Parameters set0Parameters; ///< A copy of the set0 parameters when the snapshot is taken. It can be retrieved later inside a Node action via
                                       ///< SharedShaderConstants2::getContent.
        GN::gpu2::GpuResourceSet set0Resources; ///< GPU resource set matching the set0 parameters. It can be used directly in shaders without rebinding, and it
                                                ///< is updated every frame to match the current set0 parameters. It can also be retrieved later inside a Node
                                                ///< action via SharedShaderConstants2::getContent.
    };

    Set0Parameters set0;

    // Create a token that represents the current snapshot of set0's content. It can be used later to retrieve
    // this snapshot inside a Node action.
    virtual TokenPtr takeSnapshot() const = 0;

    // Retrieve the content of set0 when to token is created.
    // This method should only be called inside an Node action. Calling it from outside of a Node action is considered undefined behavior.
    virtual AutoRef<Content> getContent(TokenPtr) const = 0;

    struct CreateParameters {
        AutoRef<GN::gpu2::GpuContext> gpu;
        AutoRef<Graph>                graph;
    };
    GN_API static AutoRef<SharedShaderConstants2> create(const CreateParameters & params);

private:
    using Entity::Entity; // inherit constructor
};

}; // namespace GN::rdg2
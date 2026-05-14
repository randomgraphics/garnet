#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg/2/open-graph.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

namespace GN::rdg2 {

struct SharedShaderConstants : public Entity {

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
        /// A copy of the set0 parameters when the snapshot is taken. It can be retrieved later inside a Node action via
        /// SharedShaderConstants::getContent.
        Set0Parameters set0Parameters;

        /// GPU resource set matching the set0 parameters. It can be used directly in shaders without rebinding, and it
        /// is updated every frame to match the current set0 parameters. It can also be retrieved later inside a Node
        /// action via SharedShaderConstants::getContent.
        GN::gpu2::GpuResourceSet set0Resources;

        /// The GPU payload. If it not empty, it must be sumitted to GPU to actually update
        /// set0Resources to match the value of set0Parameters.
        AutoRef<GN::gpu2::GpuPayload> set0Payload;
    };

    /// The CPU accessible and modifiable set0 parameters. Can be read and write
    /// at any give time. But the change won't affect GPU resources, until snapshot
    /// is taken, content is get and GPU workload is submitted.
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
    GN_API static AutoRef<SharedShaderConstants> create(const CreateParameters & params);

private:
    using Entity::Entity; // inherit constructor
};

}; // namespace GN::rdg2
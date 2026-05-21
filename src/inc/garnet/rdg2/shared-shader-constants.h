#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/shared-shader-constants.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

namespace GN::rdg2 {

struct VersionedArtifact {
    ArtifactPtr artifact;
    TokenPtr    version;
};

struct SharedShaderConstants : public Entity {
    struct FrameConstants {
        int          frameCounter  = 0;
        Microseconds frameDuration = {};
    };

    struct DirectLight {
        enum Type : int { POINT, SPOT, DIRECTIONAL } type = POINT;

        struct Point {
            Location     position  = {0, 0, 0};
            IntensityRGB intensity = {1.0f, 1.0f, 1.0f, {1.0f}};
            Distance     range     = 0;
        } point;

        struct Spot {
            Location     position          = {0, 0, 0};
            Orientation  orientation       = {0, 0, 0, 1.0f};
            IntensityRGB intensity         = {1.0f, 1.0f, 1.0f, {1.0f}};
            Distance     range             = 0;
            float        cosInnerConeAngle = 1.0f;
            float        cosOuterConeAngle = 1.0f;
        } spot;

        struct Directional {
            Orientation   orientation = {0, 0, 0, 1.0f};
            IrradianceRGB irradiance  = {1.0f, 1.0f, 1.0f, {1.0f}};
        } directional;
    };

    struct CameraConstants {
        Location    cameraPosition    = {0, 0, 0};
        Orientation cameraOrientation = {1.f, 0.f, 0.f, 0.f};
        Radian      cameraFov         = Degree(60.f);
        float       aspectRatio       = 16.f / 9.f;
        Distance    nearPlane         = 0.01f;
        Distance    farPlane          = 10000.f;
    };

    struct EnvLightingParameters {
        StrA  skyboxPath;
        StrA  irradiancePath;
        StrA  prefilteredPath;
        StrA  brdfLutPath;
        float environmentRadianceScale = 1.f;
        bool  simulateSlowLoading      = false;
    };

    struct Set0Parameters {
        FrameConstants                  frameConstants;
        AutoRef<GN::gpu2::RasterTarget> renderTarget;
        CameraConstants                 camera;
        DynaArray<DirectLight>          directLighting;
        EnvLightingParameters           envLighting;
    };

    struct Content : Entity {
        GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

        Set0Parameters           set0Parameters;
        GN::gpu2::GpuResourceSet set0Resources;
        /// All GPU work for this snapshot: any one-time inits (first frame only) followed by
        /// the per-frame UBO upload Submit all entries every frame.
        ArrayContainer<AutoRef<GN::gpu2::GpuPayload>> set0Payloads;

    protected:
        using Entity::Entity;
    };

    Set0Parameters set0;

    virtual VersionedArtifact takeSnapshot() const = 0;

    /// Get the latest content of the ssc artifact.
    virtual AutoRef<const Content> getContent(const ArtifactPtr &) const = 0;

    /// Build DrawParameters for a fullscreen skybox pass using the snapshot's set0 resources.
    virtual GN::gpu2::GpuRaster::DrawParameters getSkyboxDrawParams(const GN::gpu2::GpuResourceSet &) const = 0;

    struct CreateParameters {
        AutoRef<GN::gpu2::GpuContext> gpu;
        GraphPtr                      graph;
    };
    GN_API static AutoRef<SharedShaderConstants> create(const CreateParameters & params);

private:
    using Entity::Entity;
};

}; // namespace GN::rdg2

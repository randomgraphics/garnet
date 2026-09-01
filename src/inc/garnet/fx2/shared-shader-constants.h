#if !defined(__GN_INSIDE_FX2_H__)
    #error "Do not include <garnet/fx2/shared-shader-constants.h> directly. Include <garnet/GNfx2.h> instead."
#endif

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <chrono>

namespace GN::fx2 {

using Microseconds = std::chrono::microseconds;

/// Position in float meters. fx2 works entirely in render space: positions are nominally
/// camera-relative so float precision holds in large worlds. It is the caller's job (e.g. e2)
/// to rebase exact world coordinates against the camera before handing values down; small
/// self-contained scenes may pass absolute positions since the math is identical.
using Location    = glm::vec3;
using Orientation = glm::quat;
using Distance    = float; ///< distance in meters (float)

struct SharedShaderConstants : public RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

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
        Radian      cameraFov         = ArcDegree(60.f);
        float       aspectRatio       = 16.f / 9.f;
        Distance    nearPlane         = 0.01f;
        Distance    farPlane          = 10000.f;
        uint32_t    viewWidthInPixel  = 1;
        uint32_t    viewHeightInPixel = 1;
    };

    struct EnvLightingParameters {
        StrA  skyboxPath;
        StrA  irradiancePath;
        StrA  prefilteredPath;
        StrA  brdfLutPath;
        float environmentRadianceScale = 1.f;
    };

    struct Set0Parameters {
        FrameConstants         frameConstants;
        CameraConstants        camera;
        DynaArray<DirectLight> directLighting;
        EnvLightingParameters  envLighting;
    };

    struct Snapshot {
        GN::gpu2::GpuResourceSet set0Resources;
        /// All GPU work for this snapshot: any one-time inits (first frame only) followed by
        /// the per-frame UBO upload. Submit all entries every frame.
        DynaArray<AutoRef<GN::gpu2::GpuPayload>> set0Payloads;
    };

    Set0Parameters set0;

    virtual Snapshot takeSnapshot() const = 0;

    /// Build DrawParameters for a fullscreen skybox pass using the snapshot's set0 resources.
    virtual GN::gpu2::GpuRaster::DrawParameters getSkyboxDrawParams(const GN::gpu2::GpuResourceSet &) const = 0;

    struct CreateParameters {
        AutoRef<GN::gpu2::GpuContext> gpu;
    };
    GN_API static AutoRef<SharedShaderConstants> create(const CreateParameters & params);

private:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2

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
        /// Nonnegative linear multiplier applied to lit model RGB before Reinhard tone mapping.
        /// For lighting calibrated in nits, 0.002 maps a neutral 500-nit value to 0.5; use 1 / L
        /// to map reference luminance L to 0.5. This is camera exposure, separate from environment
        /// calibration. Use 1 for no exposure scaling or 0 for black. Unlit visualization bypasses it.
        float exposure = 0.002f;
    };

    struct EnvLightingParameters {
        StrA skyboxPath;
        StrA irradiancePath;
        StrA prefilteredPath;
        StrA brdfLutPath;
        /// Converts linear environment-map RGB to the scene's photometric scale, separately from camera exposure.
        /// Set to target luminance / source luminance for a known reference: a neutral source texel of 1
        /// becomes this many cd/m^2 (nits). Use 1 for maps already calibrated in nits; without a known
        /// reference, this is only a brightness adjustment and does not establish physical units.
        /// Use a nonnegative value (0 disables sampled environment lighting). Skybox and derived diffuse/specular
        /// maps must share the source scale; diffuse maps must store illuminance / pi, not raw lux.
        float environmentLuminanceScale = 1.f;
        /// Debug-only minimum per RGB channel for lit model diffuse/specular environment samples, in nits
        /// on the calibrated scene scale (the diffuse map stores illuminance / pi). Default 0 disables it.
        /// If a model is unexpectedly black, temporarily set this to 5.f and increase as needed to inspect
        /// visibility while diagnosing missing/dark environment maps or an incorrect luminance scale.
        /// Applied with max(sample, floor) AFTER environmentLuminanceScale, so it also works when that
        /// scale is 0; brighter channels remain unchanged. It is not added to final color or the skybox.
        /// Material response, AO, camera exposure and tone mapping still apply: zero exposure, black
        /// materials, invalid normals or other rendering faults can still hide the model. Uniform light
        /// provides little shape shading. This is a diagnostic aid, not physical lighting or a guaranteed
        /// visibility mode; restore 0 after debugging. Unlit visualization bypasses it.
        float environmentAmbientFloor = 0.f;
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

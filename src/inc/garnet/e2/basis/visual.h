#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/basis/visual.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2::basis {

/// Renders caller-provided Tableaux with fixed white-model lighting.
/// Create with a Platform for presentation, or without one for headless readback.
struct Visual : Being {
    GN_E2_DEFINE_A_BEING(Being);

    using MeshId    = Assets::MeshId;
    using TextureId = Assets::TextureId;

    /// Simple pure data representation of a camera / observer.
    struct Camera {
        /// Camera position in world unit.
        WorldVector3 position = {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()};

        /// Identity by default: GLM's default quaternion constructor does not initialize its components.
        Rotation        orientation = {1.f, 0.f, 0.f, 0.f};
        WorldCoordinate nearPlane   = WorldCoordinate(0, 1);     ///< Near clip distance in world units.
        WorldCoordinate farPlane    = WorldCoordinate(0, 10000); ///< Far clip distance in world units.

        /// Set to positive for perspective camera, 0 for orthogonal camera.
        /// Values outside [0, 180] are invalid and will be clamped back into valid range.
        float fovYInDegree = 60.0f;

        /// Nonnegative linear exposure before lit-surface tone mapping. Unlit surfaces bypass it.
        /// The default maps a neutral 500-nit input to half the display's linear range.
        float exposure = 0.002f;
    };

    /// Transform information for an object in the scene.
    struct Transform {
        WorldVector3 position    = {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()};
        Rotation     orientation = {1.f, 0.f, 0.f, 0.f};
        glm::vec3    scale       = {1.f, 1.f, 1.f};
    };

    /// Mesh instance rendered with fixed white-model lighting.
    struct Object {
        Assets::MeshId meshId = 0;
        Transform      transform;
    };

    /// Reserved environment inputs; the current white-model renderer ignores these.
    struct Environment {
        TextureId skyCubeMap                = nullptr;
        TextureId irradiancePath            = nullptr;
        TextureId prefilteredPath           = nullptr;
        TextureId brdfLutPath               = nullptr;
        float     environmentLuminanceScale = 1.0f;
        bool      visible                   = true;
    };

    /// Pure data snapshot containing full information used by Visual to render the entire scene.
    struct Tableau {
        /// Physical size of one world unit; object scales are mesh multipliers.
        PhysicalScale     scale = PhysicalScale::METER();
        Camera            camera;
        Environment       environment;
        DynaArray<Object> objects;

        /// Linear RGBA clear color; encoded to the render target's color space.
        gpu2::RasterTarget::ClearColorValue clearColor = {{0.05f, 0.06f, 0.09f, 1.0f}};

        /// Depth clear value in [0, 1].
        float clearDepth = 1.0f;
    };

    /// Parameters for creating a Visual service.
    struct CreateParameters {
        Universe &    universe;
        Ref<Platform> platform;    ///< Null selects headless rendering.
        Ref<Assets>   assets = {}; ///< Optional; an internal Assets service is created if null.
    };

    /// The universe this visual service belongs to.
    virtual Universe & universe() const = 0;

    /// Borrow the host Platform. Requires creation with a non-null platform.
    virtual Platform & platform() const = 0;
    virtual Assets &   assets() const   = 0;

    /// GPU context owned by this visual service, for compatible extension renderers.
    virtual gpu2::GpuContext & gpu() const = 0;

    /// Render opaque white models with fixed diffuse lighting and present the frame.
    /// Meshes require position and normal attributes; environment textures are currently ignored.
    virtual void renderFrame(const Tableau &) = 0;

    /// Read the last successfully rendered headless frame. Blocks CPU/GPU; intended
    /// for snapshots and diagnostics. Returns an empty image when the most recent
    /// render failed or no frame has been rendered.
    virtual gfx::img::Image readbackFrame() const = 0;

    GN_API static Ref<Visual> create(const CreateParameters &);
};

} // namespace GN::e2::basis

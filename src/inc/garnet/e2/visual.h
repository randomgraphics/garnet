#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/visual.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

struct VisualDomain;
struct VisualTableau;

/// Represents a visual observer of the world.
struct Camera : Being {
    GN_E2_DEFINE_A_BEING(Being);

    struct Desc {
        WorldVector3    position;
        Rotation        orientation;
        LocalCoordinate nearPlane; ///< clip distances are camera-relative, hence local
        LocalCoordinate farPlane;

        /// Set to positive for perspective camera, 0 for orthogonal camera.
        /// Values outside [0, 180] are invalid and will be clamped back into valid range.
        float fovYInDegree = 60.0f;
    };

    /// Mutable observer description. May be updated at any time, e.g. to move or re-orient
    /// the camera; the value is read when a world captures a visual moment.
    Desc desc;

    struct CreateParameters {
        /// The visual domain this camera observes through. The camera takes its owning
        /// universe from this domain, so it must not be null.
        Ref<VisualDomain> domain;
    };

    GN_API static Ref<Camera> create(const CreateParameters &);
};

struct VisualDomain : Being {
    GN_E2_DEFINE_A_BEING(Being);

    struct CreateParameters {
        Universe &           universe;
        Ref<OperatingDomain> os;
    };

    /// The universe this domain belongs to.
    virtual Universe & universe() const = 0;

    /// GPU context owned by this visual domain, for compatible extension renderers.
    virtual AutoRef<gpu2::GpuContext> gpu() const = 0;

    /// Render an opaque snapshot. Its contents and organization belong to the tableau.
    virtual void render(Ref<VisualTableau>) = 0;

    /// Read the last successfully rendered headless frame. Blocks CPU/GPU; intended
    /// for snapshots and diagnostics. Returns an empty image for windowed domains
    /// or when the most recent render failed or no frame has been rendered.
    virtual gfx::img::Image readbackFrame() const = 0;

    GN_API static Ref<VisualDomain> create(const CreateParameters &);
};

/// One captured item that knows how to record its own rendering work.
/// Moments hold rendering state; a VisualTableau owns the snapshot's collection.
struct VisualMoment : Being {
    GN_E2_DEFINE_A_BEING(Being);

    /// Borrowed services for one recording call; the context has no reference counting
    /// or object identity. Its raster exposes target dimensions, viewport, and scissor.
    struct RenderContext : RuntimeType {
        // RuntimeType is the unregistered root of the runtime-type system.
        GN_API GN_REGISTER_RUNTIME_TYPE();

        /// The active frame raster. Draws retain referenced resources through submission.
        virtual gpu2::GpuRaster & raster() const = 0;

        /// Prepared FX2 shared shader bindings for this moment, valid for this call.
        /// The domain already schedules their uploads; do not submit them again.
        /// Custom moments use the first scene's constants, or defaults if no scene exists,
        /// including the tableau's selected environment lighting when present.
        virtual const fx2::SharedShaderConstants::Snapshot & ssc() const = 0;

        /// Schedule GPU work before the recorded draws. Do not submit directly.
        virtual void upload(AutoRef<gpu2::GpuPayload>) = 0;

    protected:
        using RuntimeType::RuntimeType;
        ~RenderContext() = default; // borrowed, never deleted through this interface
    };

    /// Record this item's rendering; false abandons the frame. The context is valid
    /// only for this call. Keep captured state unchanged until domain rendering returns.
    virtual bool record(RenderContext &) const = 0;
};

/// An opaque collection of visual moments for one captured scene or snapshot.
/// Storage, relationships, traversal, and scheduling machinery are implementation details.
/// Regular moments render in insertion order, followed by environments, then overlays.
struct VisualTableau : Being {
    GN_E2_DEFINE_A_BEING(Being);

    struct SnapshotParameters {
        Ref<VisualDomain>      domain;
        ArrayView<Ref<Camera>> cameras;
        UnitOfTime             expectedRenderTimeShift = {};
    };

    /// Create an empty tableau; worlds normally create populated ones via snapshot().
    GN_API static Ref<VisualTableau> create(Universe &);

    /// Include an additional renderable item. Complete composition before rendering,
    /// and do not modify the tableau or its moments until render() returns.
    virtual void add(Ref<VisualMoment>) = 0;
};

/// A screen-space visual moment, implemented by overlay renderers such as UI2.
/// All overlays render after regular moments and environments, from larger Z (far)
/// to smaller Z (near). Relative rendering order for overlays with equal Z is unspecified.
struct VisualOverlay : VisualMoment {
    GN_E2_DEFINE_A_BEING(VisualMoment);

    /// Logical Z used to order overlays, independent of GPU depth coordinates.
    virtual int32_t zOrder() const = 0;

    /// Set logical Z before rendering; keep it unchanged until render() returns.
    virtual void setZOrder(int32_t z) = 0;
};

/// Frame environment task: supplies shared image-based lighting resources and draws
/// the skybox after regular moments and before overlays. Relative rendering order
/// among environments is unspecified. Skyboxes use the first scene camera; scene
/// lighting uses one environment's resources, with unspecified selection if several exist.
/// Keep this moment alive across frames to reuse its GPU resources. Omit it for no environment.
struct VisualEnvironment : VisualMoment {
    GN_E2_DEFINE_A_BEING(VisualMoment);

    struct Desc {
        StrA  skyboxPath;
        StrA  irradiancePath;
        StrA  prefilteredPath;
        StrA  brdfLutPath;
        /// Scene-constant scale that maps environment map source values to calibrated scene luminance.
        /// Multiply sampled environment RGB by this to express values in scene-local nits.
        float environmentLuminanceScale = 1.0f;
    };

    struct CreateParameters {
        Universe &                universe;
        AutoRef<gpu2::GpuContext> gpu;
        Desc                      description;
    };

    /// Create a reusable environment for the supplied GPU; returns empty on failure.
    GN_API static Ref<VisualEnvironment> create(const CreateParameters &);
};

} // namespace GN::e2

#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/visual.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

struct VisualDomain;

/// Optional screen-space contribution appended after world geometry in the E2 raster pass.
struct VisualOverlay : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Append gpu2 draws and return a transient upload payload that must execute first.
    /// Set `ok` false when recording failed and the entire frame should be abandoned.
    virtual AutoRef<gpu2::GpuPayload> record(gpu2::GpuRaster & raster, bool & ok) = 0;

protected:
    using RCRT64::RCRT64;
};

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

/// A visual snapshot consumed by the graphics domain for rendering.
struct VisualMoment : Being {
    GN_E2_DEFINE_A_BEING(Being);

    struct CaptureParameters {
        Ref<VisualDomain>      domain;
        ArrayView<Ref<Camera>> cameras;
        UnitOfTime             expectedRenderTimeShift = {};
    };

protected:
    VisualMoment(const RuntimeType::TypeInfo & type, int64_t id, const StrA & name): Being(type, id, name) {}
};

struct VisualDomain : Being {
    GN_E2_DEFINE_A_BEING(Being);

    struct Reset {
        uint32_t screenWidthInPixels;
        uint32_t screenHeightInPixels;
    };

    struct CreateParameters {
        Universe &           universe;
        Ref<OperatingDomain> os;
    };

    /// The universe this domain belongs to.
    virtual Universe & universe() const = 0;

    /// GPU context owned by this visual domain, for compatible extension renderers.
    virtual AutoRef<gpu2::GpuContext> gpu() const = 0;

    /// Application-selected image-based lighting resources used for subsequent frames.
    struct Environment {
        StrA  skyboxPath;
        StrA  irradiancePath;
        StrA  prefilteredPath;
        StrA  brdfLutPath;
        float radianceScale = 1.0f;
    };

    /// Change skybox and image-based lighting resources for subsequent snapshots.
    virtual void setEnvironment(const Environment &) = 0;

    /// Set or clear the screen-space overlay appended to subsequent frames.
    virtual void setOverlay(AutoRef<VisualOverlay> overlay) = 0;

    virtual void render(Ref<VisualMoment>) = 0;

    /// Read the last successfully rendered headless frame. Blocks CPU/GPU; intended
    /// for snapshots and diagnostics. Returns an empty image for windowed domains
    /// or when the most recent render failed or no frame has been rendered.
    virtual gfx::img::Image readbackFrame() const = 0;

    GN_API static Ref<VisualDomain> create(const CreateParameters &);
};

} // namespace GN::e2

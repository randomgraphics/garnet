#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/visual.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

struct VisualDomain;

/// Represents a visual observer of the world.
struct Camera : Thing {
    GN_E2_DEFINE_A_THING(Thing);

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
struct VisualMoment : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct CaptureParameters {
        Ref<VisualDomain>      domain;
        ArrayView<Ref<Camera>> cameras;
        UnitOfTime             expectedRenderTimeShift = {};
    };

    /// Physical size of one world unit for all lengths carried by this moment. Lengths stay in
    /// integer world units until the visual domain converts them, so the absolute-to-camera-
    /// relative rebasing happens in exact integer space before scaling.
    const PhysicalScale scale;

protected:
    VisualMoment(const RuntimeType::TypeInfo & type, UniqueIdentifier id, const StrA & name, PhysicalScale scale_): Thing(type, id, name), scale(scale_) {}
};

struct VisualDomain : Thing {
    GN_E2_DEFINE_A_THING(Thing);

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

    virtual void render(Ref<VisualMoment>) = 0;

    GN_API static Ref<VisualDomain> create(const CreateParameters &);
};

} // namespace GN::e2

#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/visual.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

struct VisualDomain;

/// Prepresents an visual observer of the world.
struct Camera : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct Desc {
        WorldPosition position;
        Rotation      orientation;
        UnitOfLength  nearPlane;
        UnitOfLength  farPlane;

        /// Set to positive for perspective camera, 0 for orthogonal camera.
        /// Anything out side of [0, 180] are invalid and will be clamped back into valid range.
        float fovYInDegree = 60.0f;
    };

    struct CreateParameters {
        Ref<VisualDomain> domain;
    };

    GN_API Ref<Camera> create(const CreateParameters &);
};

/// A visual snapshot of something. consumed by graphics domain for graphics rendering.
struct VisualMoment : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct CaptureParameters {
        Ref<VisualDomain>      domain;
        ArrayView<Ref<Camera>> cameras;
        UnitOfTime             expectedRenderTimeShift = {};
    };
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

    virtual void render(Ref<VisualMoment>) = 0;

    GN_API Ref<VisualDomain> create(const CreateParameters &);
};

} // namespace GN::e2

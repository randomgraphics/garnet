#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/world.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

// /// An audio snapshot of something, consumed by audio domain to play sound & music.
// struct AudioMoment : Thing {
//     GN_E2_DEFINE_A_THING(Thing);
// };

/// The main class that represents a presence in the world that could affect the state of the world and could
/// interact with other forms.
struct Form : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    /// update this form's internal state. called by the world, usually with an fixed interval.
    virtual bool update() = 0;

    /// capture the visual part of a form for rendering.
    virtual Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) = 0;

    // /// update the audio part of the form.
    // virtual Ref<AudioMoment> captureAudioMoment() = 0;
};

/// This represents a continuously evolving game world with diffent form of things living in it.
struct World : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct CreateParameters {
        Universe & universe;
    };

    /// The main entry point (game-loop) of this world.
    virtual void run() = 0;

    /// Add new actors to the world. Can be called from any thread.
    virtual void populate(ArrayView<Ref<Form>>) = 0;

    /// Briefly freeze the world, snap a visual moment, then continue. Can be called from any thread.
    virtual Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) = 0;

    /// Create a new empty world with nothing in it.
    GN_API Ref<World> create(const CreateParameters &);
};

} // namespace GN::e2

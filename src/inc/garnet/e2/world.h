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
    virtual void update() {}

    /// capture the visual part of a form for rendering. The defalt implementation returns nullptr, which means
    /// this form is not renderable.
    virtual Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) { return {}; }

    // /// update the audio part of the form.
    // virtual Ref<AudioMoment> captureAudioMoment() = 0;
};

/// This represents a continuously evolving game world with diffent form of things living in it.
struct World : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct CreateParameters {
        Universe & universe;
        double     metersPerUnit = 1.0; ///< physical size of one WorldLength unit, in meters.
    };

    /// Physical size of one WorldLength unit in this world, in meters. Constant for the world's lifetime.
    const double metersPerUnit;

    /// Conversion between this world's length unit and physical units, bound to the world's scale.
    ///@{
    float       toMeters(WorldLength v) const { return v.toMeters(metersPerUnit); }
    glm::vec3   toMeters(const WorldVector3 & p) const { return {toMeters(p.x), toMeters(p.y), toMeters(p.z)}; }
    float       toCentimeters(WorldLength v) const { return v.toCentimeters(metersPerUnit); }
    WorldLength fromMeters(float meters) const { return WorldLength::fromMeters(meters, metersPerUnit); }
    WorldLength fromCentimeters(float cm) const { return WorldLength::fromCentimeters(cm, metersPerUnit); }
    ///@}

    /// The main entry point (game-loop) of this world. Blocks the calling thread until stop()
    /// is called. The world imposes no threading policy of its own; callers that want the
    /// simulation to evolve concurrently should invoke run() on a thread they own.
    virtual void run() = 0;

    /// Signal the game loop to exit. Can be called from any thread; run() returns shortly after.
    virtual void stop() = 0;

    /// Add new actors to the world. Can be called from any thread.
    virtual void populate(ArrayView<Ref<Form>>) = 0;

    /// Briefly freeze the world, snap a visual moment, then continue. Can be called from any thread.
    virtual Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) = 0;

protected:
    World(const RuntimeType::TypeInfo & type, UniqueIdentifier id, const StrA & name, double metersPerUnit_)
        : Thing(type, id, name), metersPerUnit(metersPerUnit_) {
        // zero/negative/NaN scale is a programming error; NaN fails the > comparison too.
        GN_ASSERT(metersPerUnit > 0.0);
    }
};

} // namespace GN::e2

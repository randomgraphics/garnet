#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/world.h> directly. Include <garnet/GNengine2.h> instead."
#endif

#include <functional>

namespace GN::e2 {

// /// An audio snapshot of something, consumed by audio domain to play sound & music.
// struct AudioMoment : Thing {
//     GN_E2_DEFINE_A_THING(Thing);
// };

struct World;

/// The main class that represents a presence in the world. A form may be atomic or composed
/// from child forms; the world owns only root forms and propagates world membership through
/// the form tree.
struct Form : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    /// The world this form currently belongs to, or null when not attached to any world. A form can be
    /// attached to at most one world at a time; attachment is managed by the world's populate().
    World * world() const { return mWorld; }

    /// The parent form this form is currently attached to, or null when this is a root form.
    Form * parent() const { return mParent; }

    /// Child forms directly attached to this form.
    ArrayView<Ref<Form>> children() { return {mChildren.data(), mChildren.size()}; }

    /// Child forms directly attached to this form.
    ArrayView<const Ref<Form>> children() const { return {mChildren.data(), mChildren.size()}; }

    /// Attach a child form. Fails if the child is null, already parented, belongs to a different world,
    /// or would create a cycle. When this form is already in a world, the child joins the same world.
    bool attach(Ref<Form> child) {
        if (!child) GN_UNLIKELY return false;
        if (child.get() == this) GN_UNLIKELY return false;
        if (child->mParent) GN_UNLIKELY return false;
        if (child->mWorld && child->mWorld != mWorld) GN_UNLIKELY return false;
        for (auto * p = mParent; p; p = p->mParent) {
            if (p == child.get()) GN_UNLIKELY return false;
        }
        child->mParent = this;
        child->setWorldRecursive(mWorld);
        mChildren.append(child);
        return true;
    }

    /// Enter a world as a root form. Fails if this form already belongs to a world or is attached
    /// below another form. World implementations call this before storing a populated root form.
    bool enterWorld(World & world) {
        if (mWorld) GN_UNLIKELY return false;
        if (mParent) GN_UNLIKELY return false;
        setWorldRecursive(&world);
        return true;
    }

    /// Leave a world entered by this root form. Clears world membership for the whole form tree.
    void leaveWorld(World & world) {
        if (mWorld == &world) setWorldRecursive(nullptr);
    }

    /// Update this form's internal state. Called by the world, usually with a fixed interval.
    virtual void update() {}

private:
    World *              mWorld  = nullptr;
    Form *               mParent = nullptr;
    ArrayBody<Ref<Form>> mChildren;

    void setWorldRecursive(World * world) {
        mWorld = world;
        for (auto & child : mChildren) child->setWorldRecursive(world);
    }
};

/// A form with visible state that can contribute to a self-contained visual snapshot.
struct VisualForm : Form {
    GN_E2_DEFINE_A_THING(Form);

    virtual Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) = 0;
};

/// A reusable recipe that casts a fresh form tree from a root-form factory and child molds.
struct Mold : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct CreateParameters {
        Universe & universe;
        StrA       name;
    };

    using Factory = std::function<Ref<Form>(const CreateParameters &)>;

    GN_API static Ref<Mold> create(Universe & universe, const StrA & name, Factory factory);

    /// Add a child mold to this recipe. Fails if the child is null or would create a recipe cycle.
    virtual bool add(Ref<Mold> child, const StrA & childName) = 0;

    /// Cast a new form tree. Returns null if the root factory or any child factory fails.
    virtual Ref<Form> cast(Universe & universe, const StrA & formName) const = 0;
};

/// This represents a continuously evolving game world with different forms living in it.
struct World : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct CreateParameters {
        Universe & universe;
        double     metersPerUnit = 1.0; ///< physical size of one WorldLength unit, in meters.
    };

    /// Physical size of one WorldLength unit in this world, in meters. Constant for the world's lifetime.
    const double metersPerUnit;

    const WorldLength ONE_METER = WorldLength::fromMeters(1.0f, metersPerUnit);

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

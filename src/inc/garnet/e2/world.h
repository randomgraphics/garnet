#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/world.h> directly. Include <garnet/GNengine2.h> instead."
#endif

#include <functional>

namespace GN::e2 {

// /// An audio snapshot of something, consumed by audio domain to play sound & music.
// struct AudioMoment : RCRT64 {
//     GN_E2_DEFINE_PUBLIC_ABSTRACT_TYPE(RCRT64);
// };

struct World;
struct Form;

/// A unit of state and behavior attached to a form. A form composes its capabilities from a
/// flat list of facets (visual, audio, physics, gameplay, ...). Facets are the extension
/// point of the simulation: aspects that a form has are expressed by attaching facets, not
/// by subclassing Form. A facet never forms a hierarchy of its own — tree structure belongs
/// to forms.
struct Facet : RCRT64 {
    GN_E2_DEFINE_PUBLIC_ABSTRACT_TYPE(RCRT64);

    /// The form this facet is attached to, or null when detached. A facet belongs to at most
    /// one form at a time; the back-pointer is maintained by Form::addFacet() and cleared by
    /// the owning form's destructor.
    Form * form() const { return mForm; }

    /// Live one simulation moment: advance this facet's internal state by one step.
    /// Called by the owning form's live(), on the world's simulation cadence.
    virtual void live() {}

    /// Called when this facet starts living in a world: either the owning form enters the
    /// world, or the facet is added to a form that already lives in one. Invoked by the
    /// owning form; not meant to be called directly. form()->world() is already set when
    /// the call arrives.
    virtual void enterWorld(World &) {}

    /// Called when this facet stops living in a world: the owning form leaves the world or
    /// is destroyed while in one. Invoked by the owning form; not meant to be called
    /// directly. form() and its world are still valid during the call.
    virtual void leaveWorld(World &) {}

private:
    friend struct Form; // the owning form maintains the back-pointer
    Form * mForm = nullptr;
};

/// A facet with visible state that can contribute to a self-contained visual snapshot.
struct VisualFacet : Facet {
    GN_E2_DEFINE_PUBLIC_ABSTRACT_TYPE(Facet);

    virtual Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) = 0;
};

/// The main class that represents a presence in the world. A form owns the structural side
/// of the simulation: the parent/child hierarchy, the spatial transform, and a flat list of
/// facets that supply its state and behavior. A form may be atomic or composed from child
/// forms; the world owns only root forms and propagates world membership through the form
/// tree. The interface is sealed: forms are created with create() (or cast from molds) and
/// gain capabilities by attaching facets; the concrete implementation lives in the engine.
struct Form : RCRT64 {
    GN_E2_DEFINE_PUBLIC_ABSTRACT_TYPE(RCRT64);

    /// Create a new empty form: no parent, no children, no facets, identity transform.
    /// Capabilities are added by attaching facets.
    GN_API static Ref<Form> create(Universe & universe, const StrA & name);

    /// The world this form currently belongs to, or null when not attached to any world. A form can be
    /// attached to at most one world at a time; attachment is managed by the world's populate().
    virtual World * world() const = 0;

    /// The parent form this form is currently attached to, or null when this is a root form.
    virtual Form * parent() const = 0;

    /// Child forms directly attached to this form.
    virtual ArrayView<Ref<Form>> children() = 0;

    /// Child forms directly attached to this form.
    virtual ArrayView<const Ref<Form>> children() const = 0;

    /// Attach a child form. Fails if the child is null, not created by this engine, already parented,
    /// belongs to a different world, or would create a cycle. When this form is already in a world,
    /// the child joins the same world.
    virtual bool attach(Ref<Form> child) = 0;

    /// Position relative to the parent form (world-relative for root forms), in world units.
    ///@{
    virtual const WorldVector3 & position() const                  = 0;
    virtual void                 setPosition(const WorldVector3 &) = 0;
    ///@}

    /// Orientation relative to the parent form (world-relative for root forms).
    ///@{
    virtual const Rotation & rotation() const              = 0;
    virtual void             setRotation(const Rotation &) = 0;
    ///@}

    /// This form's transform composed with all ancestors, i.e. in world space. Defined once for
    /// all implementations in terms of parent() and the local transform.
    ///@{
    GN_API WorldVector3 worldPosition() const;
    GN_API Rotation     worldRotation() const;
    ///@}

    /// Facets attached to this form, in attach order.
    virtual ArrayView<Ref<Facet>> facets() = 0;

    /// Facets attached to this form, in attach order.
    virtual ArrayView<const Ref<Facet>> facets() const = 0;

    /// Attach a facet. Fails if the facet is null or already attached to a form. When this
    /// form already lives in a world, the facet is told it enters that world.
    virtual bool addFacet(Ref<Facet> facet) = 0;

    /// Enter a world as a root form. Fails if this form already belongs to a world or is attached
    /// below another form. World implementations call this before storing a populated root form.
    virtual bool enterWorld(World & world) = 0;

    /// Leave a world entered by this root form. Clears world membership for the whole form tree.
    virtual void leaveWorld(World & world) = 0;

    /// Live one simulation moment: lets each facet live, in attach order. Called by the
    /// world, usually with a fixed interval; the world's tree traversal covers child
    /// forms, not this method.
    virtual void live() = 0;

protected:
    /// Facet befriends only this base class and friendship does not inherit, so the engine's
    /// concrete implementation maintains a facet's owner back-pointer through this helper.
    static void setFacetOwner(Facet & facet, Form * owner) { facet.mForm = owner; }
};

/// A reusable recipe that casts a fresh form tree from a root-form factory and child molds.
struct Mold : RCRT64 {
    GN_E2_DEFINE_PUBLIC_ABSTRACT_TYPE(RCRT64);

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
struct World : RCRT64 {
    GN_E2_DEFINE_PUBLIC_ABSTRACT_TYPE(RCRT64);

    struct CreateParameters {
        Universe &    universe;
        PhysicalScale scale = PhysicalScale::NANOMETER(); ///< physical size of one world unit.
    };

    /// The universe this world belongs to.
    Universe & universe;

    /// Physical size of one world unit in this world; all physical-unit conversion goes through
    /// it. Constant for the world's lifetime.
    const PhysicalScale scale;

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
    World(const RuntimeType::TypeInfo & type, int64_t id, const StrA & name, const CreateParameters & cp)
        : RCRT64(type, id, name), universe(cp.universe), scale(cp.scale) {}
};

} // namespace GN::e2

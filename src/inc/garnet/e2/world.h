#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Include <garnet/GNengine2.h> instead."
#endif
#include <functional>

namespace GN::e2 {

/// Universe-scoped identity; zero is invalid. Do not transfer raw IDs between Universes.
using FormId = int64_t;
struct PrimeView;

/// Polymorphic versioned data. Every concrete subclass must clone its complete dynamic type.
/// Copies must own mutable payloads; immutable resources may be shared between versions.
/// Derive a Facet's nested Value or a world-state type, then submit copies through Slate.
struct FacetValue : RefCounter, RuntimeType {

    // These mixins provide ownership and RTTI without an instance identity.
    GN_API GN_REGISTER_RUNTIME_TYPE();

    /// Make an independent copy of the complete dynamic value, including all derived fields.
    virtual Ref<FacetValue> clone() const = 0;

protected:
    explicit FacetValue(const RuntimeType::TypeInfo & type): RuntimeType(type) {}
    FacetValue(const FacetValue & other): RuntimeType(other.typeInfo()) {}
};

/// First-class capability identified by its owning Form and concrete type, with no instance ID/name.
/// Its immutable configuration and behavior are separate from tick values.
/// A subclass declares nested Value, overrides valueType(), and clones its complete dynamic type.
/// Register a prototype with World and attach instances/initial values through a Mold or Slate.
struct Facet : RefCounter, RuntimeType {
    GN_API GN_REGISTER_RUNTIME_TYPE();

    /// Accepted Value base type; concrete values may derive from it.
    virtual const RuntimeType::TypeInfo & valueType() const = 0;

    /// Required capabilities, checked against final composition with RuntimeType inheritance.
    virtual DynaArray<const RuntimeType::TypeInfo *> requirements() const { return {}; }

    /// Domain invariants over the complete candidate. No mutation or external side effects.
    virtual bool       validate(const PrimeView &, FormId, const FacetValue &) const { return true; }
    virtual Ref<Facet> clone() const = 0;

protected:
    explicit Facet(const RuntimeType::TypeInfo & type): RuntimeType(type) {}
    Facet(const Facet & other): RuntimeType(other.typeInfo()) {}
};

/// Extensible individual identity. Composition is resolved in an explicit Prime version.
/// Obtain committed Forms from PrimeView; use a Mold factory when custom Form methods are needed.
struct Form : Being {
    GN_E2_DEFINE_A_BEING(Being);
    GN_API static Ref<Form> create(FormId, const StrA & name = "form");
    virtual FormId          formId() const = 0;
    template<typename F>
    Ref<const F> getFacet(const PrimeView &) const;
};

/// Immutable published object world; retaining a view retains its Form, Facet and value references.
/// Storage uses only the Facet/FacetValue base contracts; typed accessors serve external callers.
/// Acquire through World::primeSnapshot() and retain the view while using borrowed values.
struct PrimeView : Being {
    GN_E2_DEFINE_A_BEING(Being);
    virtual uint64_t              tick() const                                       = 0;
    virtual Ref<const Form>       form(FormId) const                                 = 0;
    virtual Ref<const Facet>      facet(FormId, const RuntimeType::TypeInfo &) const = 0;
    virtual Ref<const FacetValue> value(FormId, const RuntimeType::TypeInfo &) const = 0;
    virtual Ref<const FacetValue> state(const RuntimeType::TypeInfo &) const         = 0;

    /// Match exact concrete capability types; an empty list returns every committed Form.
    virtual DynaArray<FormId> query(const DynaArray<const RuntimeType::TypeInfo *> &) const = 0;
    template<typename F>
    Ref<const F> getFacet(FormId id) const {
        return RuntimeType::cast<const F>(facet(id, F::TYPE_INFO()));
    }

    /// Borrow a value for this view's lifetime. Exact type wins; ambiguous derived matches return null.
    template<typename F>
    const typename F::Value * get(FormId id) const {
        return RuntimeType::cast<const typename F::Value>(value(id, F::TYPE_INFO()).get());
    }
    template<typename T>
    const T * state() const {
        return RuntimeType::cast<const T>(state(T::TYPE_INFO()).get());
    }
    template<typename... F>
    DynaArray<FormId> query() const {
        return query({&F::TYPE_INFO()...});
    }
};

template<typename F>
Ref<const F> Form::getFacet(const PrimeView & view) const {
    return view.template getFacet<F>(formId());
}

using Prime = PrimeView;

/// Pairs a capability with compatible initial data for a Mold.
/// Construct from owning references; Mold/Slate snapshot both objects on ingress.
struct FacetBinding {

    /// Capability prototype. Mold and World clone it so builder aliases cannot mutate attached capabilities.
    Ref<const Facet> facet;

    /// Initial value compatible with facet->valueType(); cloned independently for each created Form.
    Ref<const FacetValue> value;
};

/// Reusable Form creation recipe, supplied to World::createForm() or Slate::create().
/// Build with initial FacetBindings; an optional factory creates custom Form identities using the reserved ID.
struct Mold : Being {
    GN_E2_DEFINE_A_BEING(Being);
    using FormFactory = std::function<Ref<Form>(FormId)>;
    GN_API static Ref<Mold>                 create(Universe &, const StrA &, DynaArray<FacetBinding>, FormFactory = {});
    virtual const DynaArray<FacetBinding> & facets() const           = 0;
    virtual Ref<Form>                       createForm(FormId) const = 0;
};

/// Mutation workspace borrowed from LawContext during a tick.
/// Use it to propose owned value/structural changes; they become visible only on successful publication.
/// It is never a second state read source. All operations validate caller ownership.
struct Slate {
    virtual ~Slate()                    = default;
    virtual FormId create(const Mold &) = 0;

    /// Deletes only the explicit Form; deletion dominates its value writes.
    virtual void destroy(FormId)                                                = 0;
    virtual void set(FormId, const RuntimeType::TypeInfo &, const FacetValue &) = 0;
    virtual void add(FormId, const Facet &, const FacetValue &)                 = 0;
    virtual void remove(FormId, const RuntimeType::TypeInfo &)                  = 0;
    virtual void setState(const FacetValue &)                                   = 0;
    template<typename F>
    void set(FormId id, const typename F::Value & value) {
        set(id, F::TYPE_INFO(), value);
    }
    template<typename F>
    void remove(FormId id) {
        remove(id, F::TYPE_INFO());
    }
};

/// Receiver-defined directed request. Concrete subclasses clone their complete dynamic type.
/// Declare its type in the receiving Law contract; submit through World or send from LawContext.
struct Intent : Being {
    GN_E2_DEFINE_A_BEING(Being);
    virtual Ref<Intent> clone() const = 0;

protected:
    explicit Intent(const RuntimeType::TypeInfo & type, const StrA & name = "intent"): Being(type, 0, name) {}
    Intent(const Intent & other): Being(other.typeInfo(), 0, "intent") {}
};

/// Producer-defined observational output; it has no simulation mutation authority.
/// Declare its type in the producing Law contract and emit through LawContext; presentation consumes World::events().
struct Event : Being {
    GN_E2_DEFINE_A_BEING(Being);
    virtual Ref<Event> clone() const = 0;

protected:
    explicit Event(const RuntimeType::TypeInfo & type, const StrA & name = "event"): Being(type, 0, name) {}
    Event(const Event & other): Being(other.typeInfo(), 0, "event") {}
};

/// Published notification with cursor metadata. Read through World::events() and cast its payload to the declared Event type.
struct PublishedEvent {

    /// Committed simulation tick that produced this notification.
    uint64_t tick = 0;

    /// World-wide publication sequence used by events() cursors; independent of rendering frequency.
    uint64_t sequence = 0;

    /// Immutable cloned notification; retaining the reference survives event-buffer eviction.
    Ref<const Event> payload;
};

/// Registration contract returned by Law::contract(); lists its write authority and message types.
/// Registration order is execution order; all laws read the same tick-start Prime.
/// Writes and simulation queries match exact concrete types. A subclass supplies its own Law.
struct LawContract {

    /// Diagnostic label for this registered domain.
    StrA name;

    /// Exact Facet or world-state types this Law may mutate; each type has one writer.
    DynaArray<const RuntimeType::TypeInfo *> writes;

    /// Exact Intent types routed exclusively to this Law. TypeInfo pointers must remain valid for World's lifetime.
    DynaArray<const RuntimeType::TypeInfo *> intents;

    /// Grants structural operations; existing Facet additions/removals still require writes ownership.
    bool structural = false;

    /// Exact Event types this Law may emit; delivery follows successful publication, with no simulation causality.
    DynaArray<const RuntimeType::TypeInfo *> events = {};
};

/// Per-invocation inputs and output channels passed to Law::tick().
/// Read prime, stage changes through slate, and send/emit messages; do not retain borrowed members after tick returns.
struct LawContext {

    /// Borrowed immutable tick-start state shared by every Law in this tick.
    const PrimeView & prime;

    /// Borrowed write-only workspace; valid only during this Law invocation.
    Slate & slate;

    /// Positive simulated duration in shared fiz nanosecond units, independent of presentation frame time.
    UnitOfTime dt;

    /// Borrowed requests routed to this invocation, in delivery order; self/backward sends arrive next tick.
    const DynaArray<Ref<const Intent>> & intents;
    LawContext(const PrimeView & p, Slate & s, UnitOfTime d, const DynaArray<Ref<const Intent>> & i): prime(p), slate(s), dt(d), intents(i) {}
    virtual ~LawContext()             = default;
    virtual void send(const Intent &) = 0;
    virtual void emit(const Event &)  = 0;
};

/// Extensible coarse simulation domain. Causal mutable state belongs in Prime.
/// Override contract() and tick(), then register with World::addLaw(). Runtime registration remains open.
/// Each tick reads one shared Prime version and proposes changes through LawContext.
struct Law : Being {
    GN_E2_DEFINE_A_BEING(Being);
    virtual LawContract contract() const   = 0;
    virtual void        tick(LawContext &) = 0;

protected:
    explicit Law(const RuntimeType::TypeInfo & type, const StrA & name = "law"): Being(type, 0, name) {}
};

/// Pure candidate cleanup for the registered capability after structural deletion.
using DeletionCleanup = std::function<Ref<FacetValue>(FormId, const FacetValue &, const DynaArray<FormId> &)>;

/// Runtime owning publication, ordered Laws and message routing.
/// Registration remains open; each tick captures a stable registry and input batch.
/// Readers independently retain primeSnapshot() views; producers submit declared Intents.
struct World : Being {
    GN_E2_DEFINE_A_BEING(Being);
    GN_API static Ref<World> create(Universe &, const StrA & name = "world", size_t eventCapacity = 4096);

    /// Register a concrete capability at any time; optional cleanup repairs references after deletion.
    virtual bool registerFacet(const Facet &, DeletionCleanup = {}) = 0;

    /// Register a concrete world-level FacetValue type at any time.
    virtual bool registerState(const RuntimeType::TypeInfo &) = 0;
    template<typename T>
    bool registerState() {
        return registerState(T::TYPE_INFO());
    }

    /// Append an owner/receiver in execution order; duplicate concrete ownership is rejected.
    /// Late Laws activate after the publication that installs their captured initial data.
    /// Their queued Intents wait until activation. Registered writes and unique receivers are checked immediately.
    virtual bool addLaw(Ref<Law>) = 0;

    /// Reserve a Form identity and clone the recipe. Before the first tick, publishes into Prime 0.
    /// Later calls queue creation for the next uncaptured tick; failure returns zero.
    virtual FormId createForm(const Mold &) = 0;

    /// Attach a new capability, never overwrite one. Bootstrap publishes immediately; later calls queue it.
    /// Value/type/composition validity is checked before publication; queued acceptance is not completion.
    virtual bool addFacet(FormId, const Facet &, const FacetValue &) = 0;

    /// Initialize a missing registered world-state value. Never overwrites existing/pending state.
    /// Bootstrap publishes immediately; later calls queue initialization at a tick boundary.
    virtual bool initializeState(const FacetValue &) = 0;

    /// Advance one positive nanosecond duration. Failed ticks keep Prime unchanged and permanently halt.
    virtual bool           tick(UnitOfTime dt)    = 0;
    virtual bool           halted() const         = 0;
    virtual StrA           error() const          = 0;
    virtual Ref<PrimeView> primeSnapshot() const  = 0;
    virtual bool           submit(const Intent &) = 0;

    /// Bounded presentation stream. Cursor advances past returned events; old events may be dropped.
    virtual DynaArray<PublishedEvent> events(uint64_t & cursor) const = 0;
};

} // namespace GN::e2

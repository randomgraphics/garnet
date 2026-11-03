#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Include <garnet/GNengine2.h> instead."
#endif
namespace GN::e2 {

/// Actual world-space metric motion, independent of Transform parent frames. The current AABB solver requires zero angular velocity.
/// Attach alongside TransformFacet and BodyFacet to opt a demo body into DynamicsLaw integration.
struct MotionFacet : Facet {
    GN_API GN_REGISTER_RUNTIME_TYPE(Facet);

    /// Versioned payload; derived values must override clone to preserve their dynamic type.
    struct Value : FacetValue {
        GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(const RuntimeType::TypeInfo & type = TYPE_INFO()): FacetValue(type) {}
        Ref<FacetValue> clone() const override {
            if (typeInfo().id != TYPE_INFO().id) return {};
            return Ref<FacetValue>(new Value(*this));
        }
        glm::dvec3 linearVelocity {0};  ///< Actual world-space velocity in metres per second, not a requested target.
        glm::dvec3 angularVelocity {0}; ///< World-axis angular velocity in radians per second; this demo requires zero.
    };

    explicit MotionFacet(const RuntimeType::TypeInfo & type = TYPE_INFO()): Facet(type) {}

    /// Motion requires a pose; its velocity reference frame remains world-space.
    DynaArray<const RuntimeType::TypeInfo *> requirements() const override { return {&TransformFacet::TYPE_INFO()}; }
    const RuntimeType::TypeInfo &            valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                               clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Facet>(new MotionFacet(*this));
    }

    /// Read this capability's value for an individual in an explicit committed version.
    const Value * read(const Form & form, const PrimeView & view) const { return view.get<MotionFacet>(form.formId()); }
};

/// Axis-aligned box collision capability, separate from visual geometry.
/// Attach with TransformFacet and ContactFacet; DynamicsLaw treats bodies without MotionFacet as static.
struct BodyFacet : Facet {
    GN_API GN_REGISTER_RUNTIME_TYPE(Facet);

    /// Versioned payload; derived values must override clone to preserve their dynamic type.
    struct Value : FacetValue {
        GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(const RuntimeType::TypeInfo & type = TYPE_INFO()): FacetValue(type) {}
        Ref<FacetValue> clone() const override {
            if (typeInfo().id != TYPE_INFO().id) return {};
            return Ref<FacetValue>(new Value(*this));
        }
        glm::dvec3 halfExtent {0.25};  ///< Positive half-size of the collision box in metres on each axis.
        double     mass        = 1;    ///< Positive mass in kilograms. MotionFacet presence, not mass, selects dynamic integration.
        double     friction    = 0.05; ///< Nonnegative dimensionless contact-friction coefficient.
        double     restitution = 0.1;  ///< Bounce coefficient in [0, 1]: zero is inelastic, one is fully elastic.
    };

    explicit BodyFacet(const RuntimeType::TypeInfo & type = TYPE_INFO()): Facet(type) {}
    const RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                    clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Facet>(new BodyFacet(*this));
    }

    /// Read this capability's value for an individual in an explicit committed version.
    const Value * read(const Form & form, const PrimeView & view) const { return view.get<BodyFacet>(form.formId()); }
};

/// Final tick contact partners. Empty means no contact.
/// Attach to every demo body; DynamicsLaw writes the set and deletion cleanup removes vanished partners.
struct ContactFacet : Facet {
    GN_API GN_REGISTER_RUNTIME_TYPE(Facet);

    /// Versioned payload; derived values must override clone to preserve their dynamic type.
    struct Value : FacetValue {
        GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(const RuntimeType::TypeInfo & type = TYPE_INFO()): FacetValue(type) {}
        Ref<FacetValue> clone() const override {
            if (typeInfo().id != TYPE_INFO().id) return {};
            return Ref<FacetValue>(new Value(*this));
        }
        DynaArray<FormId> touching; ///< Sorted final-tick contact partners; empty means no contact. Updated symmetrically by DynamicsLaw.
    };

    explicit ContactFacet(const RuntimeType::TypeInfo & type = TYPE_INFO()): Facet(type) {}
    const RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                    clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Facet>(new ContactFacet(*this));
    }

    /// Read this capability's value for an individual in an explicit committed version.
    const Value * read(const Form & form, const PrimeView & view) const { return view.get<ContactFacet>(form.formId()); }
};

/// Counts contact beginnings sampled at published tick boundaries.
/// Optionally attach to a body to retain cumulative statistics; read the published Value for UI or app logic.
struct CollisionStatsFacet : Facet {
    GN_API GN_REGISTER_RUNTIME_TYPE(Facet);

    /// Versioned payload; derived values must override clone to preserve their dynamic type.
    struct Value : FacetValue {
        GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(const RuntimeType::TypeInfo & type = TYPE_INFO()): FacetValue(type) {}
        Ref<FacetValue> clone() const override {
            if (typeInfo().id != TYPE_INFO().id) return {};
            return Ref<FacetValue>(new Value(*this));
        }
        uint64_t count = 0; ///< Cumulative new contact partners relative to the preceding tick; resting contact does not increment it.
    };

    explicit CollisionStatsFacet(const RuntimeType::TypeInfo & type = TYPE_INFO()): Facet(type) {}
    const RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                    clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Facet>(new CollisionStatsFacet(*this));
    }

    /// Read this capability's value for an individual in an explicit committed version.
    const Value * read(const Form & form, const PrimeView & view) const { return view.get<CollisionStatsFacet>(form.formId()); }
};

/// Marks individuals included in LifetimeLaw's population cap.
/// Generated boxes include it; requestDestroy() submits a deletion intent without changing a snapshot.
struct LifetimeFacet : Facet {
    GN_API GN_REGISTER_RUNTIME_TYPE(Facet);

    /// Versioned payload; derived values must override clone to preserve their dynamic type.
    struct Value : FacetValue {
        GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(const RuntimeType::TypeInfo & type = TYPE_INFO()): FacetValue(type) {}
        Ref<FacetValue> clone() const override {
            if (typeInfo().id != TYPE_INFO().id) return {};
            return Ref<FacetValue>(new Value(*this));
        }
    };

    explicit LifetimeFacet(const RuntimeType::TypeInfo & type = TYPE_INFO()): Facet(type) {}
    const RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                    clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Facet>(new LifetimeFacet(*this));
    }

    /// Read this capability's value for an individual in an explicit committed version.
    const Value * read(const Form & form, const PrimeView & view) const { return view.get<LifetimeFacet>(form.formId()); }

    /// Request deletion through LifetimeLaw without mutating the observed version.
    GN_API bool requestDestroy(World &, const Form &) const;
};

/// Generator progress and PRNG state owned by LifetimeLaw, committed in Prime.
/// Register as world state and initialize before the Law first runs; LifetimeLaw then advances it through Slate.
struct LifetimeState : FacetValue {
    GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
    LifetimeState(): FacetValue(TYPE_INFO()) {}
    Ref<FacetValue> clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<FacetValue>(new LifetimeState(*this));
    }
    double   elapsed = 0;                     ///< Unconsumed simulation seconds toward the next spawn; discarded when population is full.
    uint64_t random  = 0x6a09e667f3bcc909ULL; ///< Nonzero PRNG state; seed at initialization, then let LifetimeLaw advance it.
};

/// Receiver-defined structural operation consumed by LifetimeLaw.
/// Submit to World or send through LawContext; processed deletions appear in published Form membership.
struct DestroyFormIntent : Intent {
    GN_API GN_REGISTER_RUNTIME_TYPE(Intent);
    explicit DestroyFormIntent(FormId target = 0): Intent(TYPE_INFO()), form(target) {}
    Ref<Intent> clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Intent>(new DestroyFormIntent(*this));
    }
    FormId form; ///< Form requested for deletion; children are not implicitly deleted.
};

/// Configuration passed to createDynamicsLaw(); copied when the Law is created.
/// Numerical limits describe the translation-only AABB demo solver, not a general rigid-body API.
struct DynamicsOptions {
    PhysicalScale scale = PhysicalScale::NANOMETER(); ///< Metres per integer world unit; use the same scale for simulation and rendering.
    WorldVector3  origin {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(),
                          WorldCoordinate::ZERO()}; ///< Absolute anchor for the local metric simulation/spawn region.
    glm::dvec3    gravity {0, -9.81, 0};            ///< World-space acceleration in metres per second squared.
    double        destroyBelowY    = -15;           ///< Y threshold in metres relative to origin; lower dynamic bodies request LifetimeLaw deletion.
    double        contactTolerance = 0.002;         ///< Contact persistence tolerance in metres, used when collecting final-tick partners.
};

/// Configuration passed to createLifetimeLaw(); copied when the Law is created.
/// Use the same scale/origin as DynamicsOptions. Spawn limits refer to simulation time; horizontal speed
/// is uniform in [0, maximumHorizontalSpeed].
struct LifetimeOptions {
    PhysicalScale scale = PhysicalScale::NANOMETER(); ///< Metres per integer world unit; use the same scale for simulation and rendering.
    WorldVector3  origin {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(),
                          WorldCoordinate::ZERO()}; ///< Absolute anchor for the local metric simulation/spawn region.
    double        spawnPerSecond         = 100;     ///< Positive generation rate per simulation second, subject to the population cap.
    size_t        maximumPopulation      = 1024;    ///< Maximum Forms carrying LifetimeFacet; zero disables generation.
    double        maximumHorizontalSpeed = 0.5;     ///< Initial XZ speed is sampled uniformly from zero to this value in metres per second.
    glm::dvec3    spawnMinimum {-4.2, 4, -4.2};     ///< Lower spawn-region corner in metres relative to origin.
    glm::dvec3    spawnMaximum {4.2, 8, 4.2};       ///< Upper spawn-region corner; each component must be at least the corresponding minimum.
    bool          includeGNfizCollider = false; ///< Attach PhysicsFacet to generated boxes for the optional GNfiz Law.
};

/// Register capability types and the contact-deletion cleanup. Call before adding the dynamics/lifetime Laws.
GN_API bool registerDynamicsFacets(World &);

/// Stateless per-tick local solver; owns Transform, Motion, Contact, and CollisionStats.
/// Uses conservative world AABB bounds for transformed boxes and translation-only response; it may
/// report proxy contacts for rotated/scaled bodies. Static colliders beneath moving ancestors require
/// compound/kinematic simulation and are rejected. Replace this Law where tighter shape fidelity is needed.
GN_API Ref<Law> createDynamicsLaw(const DynamicsOptions & = {});

/// Creates and deletes forms; initialize LifetimeState before the Law first runs.
GN_API Ref<Law> createLifetimeLaw(Universe &, const LifetimeOptions & = {});

/// Ground box centered half a metre below origin, with a finite eight-metre square top at y=0.
GN_API Ref<Mold> createGroundMold(Universe &, PhysicalScale = PhysicalScale::NANOMETER(),
                                  const WorldVector3 & origin = {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()},
                                  bool includeGNfizCollider = false);

/// Complete dynamic box template. The current AABB solver permits arbitrary Transform orientation/scale
/// as a conservative proxy and requires zero angular velocity.
GN_API Ref<Mold> createBoxMold(Universe &, const TransformFacet::Value &, const MotionFacet::Value &, const BodyFacet::Value & = BodyFacet::Value(),
                               const VisualFacet::Value & = VisualFacet::Value(), bool includeGNfizCollider = false);

} // namespace GN::e2

#pragma once
#if !defined(__GN_INSIDE_FIZ_H__)
    #error "Do not include <garnet/fiz/solid.h> directly. Include <garnet/GNfiz.h> instead."
#endif

namespace GN::fiz {

struct Solid;

/// Raycast intersection query result.
struct RaycastHit {
    bool           hit      = false;
    Scalar         distance = 0.0f;
    Vector3        position = {0.0f, 0.0f, 0.0f};
    Vector3        normal   = {0.0f, 1.0f, 0.0f};
    AutoRef<Solid> solid;
};

/// Construction descriptor for instantiating a rigid solid.
struct SolidDesc {
    AutoRef<Hull>  hull;
    Temper         temper;
    Transform      transform;
    Vector3        linearVelocity  = {0.0f, 0.0f, 0.0f};
    Vector3        angularVelocity = {0.0f, 0.0f, 0.0f};
    MotionType     motionType      = MotionType::DYNAMIC;
    CollisionLayer layer           = CollisionLayer::MOVING;
    Scalar         massOverride    = 0.0f; ///< If 0.0, mass is computed as volume * temper.density.
    bool           allowSleep      = true;
    bool           ccd             = false; ///< Continuous collision detection for high-speed tunneling prevention.
    uint64_t       entityId        = 0;     ///< Entity ID used for deterministic contact and island sorting.
};

/// Rigid body instance governed by 6-DOF Newtonian mechanics and contact constraints.
struct Solid : public Being {
    GN_FIZ_DEFINE_A_BEING(Being);

    virtual Transform transform() const                                               = 0;
    virtual void      setTransform(const Transform & transform, bool activate = true) = 0;

    virtual Vector3    position() const    = 0;
    virtual Quaternion orientation() const = 0;

    virtual Vector3 linearVelocity() const                      = 0;
    virtual void    setLinearVelocity(const Vector3 & velocity) = 0;

    virtual Vector3 angularVelocity() const                             = 0;
    virtual void    setAngularVelocity(const Vector3 & angularVelocity) = 0;

    virtual void applyForce(const Vector3 & force)            = 0;
    virtual void applyImpulse(const Vector3 & impulse)        = 0;
    virtual void applyTorque(const Vector3 & torque)          = 0;
    virtual void applyAngularImpulse(const Vector3 & impulse) = 0;

    virtual MotionType motionType() const             = 0;
    virtual void       setMotionType(MotionType type) = 0;

    virtual const Temper & temper() const                   = 0;
    virtual void           setTemper(const Temper & temper) = 0;

    virtual const Hull * hull() const = 0;
    virtual Box          aabb() const = 0;

    virtual bool isActive() const = 0;
    virtual void activate()       = 0;
    virtual void deactivate()     = 0;

    virtual uint64_t entityId() const = 0;
    virtual Scalar   mass() const     = 0;
};

/// Construction descriptor for SolidEngine.
struct SolidEngineDesc {
    Vector3        gravity               = {0.0f, -9.81f, 0.0f};
    uint32_t       maxBodies             = 65536;
    uint32_t       maxBodyPairs          = 65536;
    uint32_t       maxContactConstraints = 65536;
    uint32_t       numWorkerThreads      = 0; ///< 0 = auto-detect hardware concurrency.
    SimulationMode simulationMode        = SimulationMode::DISSIPATIVE;
};

/// Simulation engine managing rigid bodies, spatial acceleration, and constraint resolution.
struct SolidEngine : public Being {
    GN_FIZ_DEFINE_A_BEING(Being);

    static GN_API AutoRef<SolidEngine> create(const SolidEngineDesc & desc = {});

    virtual AutoRef<Solid> createSolid(const SolidDesc & desc) = 0;
    virtual void           removeSolid(Solid * solid)          = 0;

    /// Advances the simulation by discrete time dt.
    /// Supports negative dt for time-reversible conservative systems.
    virtual void step(UnitOfTime dt) = 0;

    virtual void    setGravity(const Vector3 & gravity) = 0;
    virtual Vector3 gravity() const                     = 0;

    virtual bool raycast(const Vector3 & origin, const Vector3 & direction, Scalar maxDistance, RaycastHit & outHit) const = 0;

    virtual size_t bodyCount() const       = 0;
    virtual size_t activeBodyCount() const = 0;

    virtual void           setSimulationMode(SimulationMode mode) = 0;
    virtual SimulationMode simulationMode() const                 = 0;
};

} // namespace GN::fiz

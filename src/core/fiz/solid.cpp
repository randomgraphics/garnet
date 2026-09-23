#include <garnet/GNfiz.h>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/JobSystemSingleThreaded.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

JPH_SUPPRESS_WARNINGS

#include <cmath>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace GN::fiz {

static void JoltTrace(const char * inFMT, ...) {
    va_list list;
    va_start(list, inFMT);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFMT, list);
    va_end(list);
    fprintf(stderr, "[JoltTrace] %s\n", buffer);
    fflush(stderr);
}

#ifdef JPH_ENABLE_ASSERTS
static bool JoltAssertFailed(const char * inExpression, const char * inMessage, const char * inFile, JPH::uint inLine) {
    fprintf(stderr, "\n[JoltAssert] %s (%s) at %s:%u\n", inExpression, inMessage ? inMessage : "", inFile, inLine);
    fflush(stderr);
    return false; // return false to avoid __debugbreak()
}
#endif

// Global Jolt initialization
static std::once_flag sJoltInitOnce;
static void           ensureJoltInitialized() {
    std::call_once(sJoltInitOnce, []() {
        JPH::RegisterDefaultAllocator();
        JPH::Trace = JoltTrace;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = JoltAssertFailed;)
        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();
    });
}

// Broadphase and object layers mapping
namespace Layers {
static constexpr JPH::ObjectLayer NON_MOVING = 0;
static constexpr JPH::ObjectLayer MOVING     = 1;
static constexpr JPH::ObjectLayer TRIGGER    = 2;
static constexpr JPH::ObjectLayer DEBRIS     = 3;
static constexpr JPH::ObjectLayer NUM_LAYERS = 4;
} // namespace Layers

namespace BroadPhaseLayers {
static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
static constexpr JPH::BroadPhaseLayer MOVING(1);
static constexpr JPH::BroadPhaseLayer TRIGGER(2);
static constexpr JPH::BroadPhaseLayer DEBRIS(3);
static constexpr JPH::uint            NUM_LAYERS(4);
} // namespace BroadPhaseLayers

class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
public:
    BPLayerInterfaceImpl() {
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING]     = BroadPhaseLayers::MOVING;
        mObjectToBroadPhase[Layers::TRIGGER]    = BroadPhaseLayers::TRIGGER;
        mObjectToBroadPhase[Layers::DEBRIS]     = BroadPhaseLayers::DEBRIS;
    }

    JPH::uint GetNumBroadPhaseLayers() const override { return BroadPhaseLayers::NUM_LAYERS; }

    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
        if (inLayer < Layers::NUM_LAYERS) { return mObjectToBroadPhase[inLayer]; }
        return BroadPhaseLayers::MOVING;
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char * GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override {
        switch ((JPH::BroadPhaseLayer::Type) inLayer) {
        case (JPH::BroadPhaseLayer::Type) BroadPhaseLayers::NON_MOVING:
            return "NON_MOVING";
        case (JPH::BroadPhaseLayer::Type) BroadPhaseLayers::MOVING:
            return "MOVING";
        case (JPH::BroadPhaseLayer::Type) BroadPhaseLayers::TRIGGER:
            return "TRIGGER";
        case (JPH::BroadPhaseLayer::Type) BroadPhaseLayers::DEBRIS:
            return "DEBRIS";
        default:
            return "UNKNOWN";
        }
    }
#endif

private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override {
        switch (inLayer1) {
        case Layers::NON_MOVING:
            return inLayer2 == BroadPhaseLayers::MOVING || inLayer2 == BroadPhaseLayers::DEBRIS;
        case Layers::MOVING:
            return true;
        case Layers::TRIGGER:
            return inLayer2 == BroadPhaseLayers::MOVING;
        case Layers::DEBRIS:
            return inLayer2 == BroadPhaseLayers::NON_MOVING || inLayer2 == BroadPhaseLayers::MOVING;
        default:
            return false;
        }
    }
};

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override {
        switch (inObject1) {
        case Layers::NON_MOVING:
            return inObject2 == Layers::MOVING || inObject2 == Layers::DEBRIS;
        case Layers::MOVING:
            return true;
        case Layers::TRIGGER:
            return inObject2 == Layers::MOVING;
        case Layers::DEBRIS:
            return inObject2 == Layers::NON_MOVING || inObject2 == Layers::MOVING;
        default:
            return false;
        }
    }
};

// Shape converter from Hull to JPH::ShapeRefC
static JPH::ShapeRefC createJoltShape(const Hull * hull) {
    if (!hull) return nullptr;
    switch (hull->type()) {
    case HullType::BOX: {
        const auto *          b   = static_cast<const BoxHull *>(hull);
        const auto &          ext = b->halfExtents();
        JPH::BoxShapeSettings settings(JPH::Vec3(ext.x, ext.y, ext.z));
        auto                  res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::SPHERE: {
        const auto *             s = static_cast<const SphereHull *>(hull);
        JPH::SphereShapeSettings settings(s->radius());
        auto                     res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::CAPSULE: {
        const auto *              c = static_cast<const CapsuleHull *>(hull);
        JPH::CapsuleShapeSettings settings(c->halfHeight(), c->radius());
        auto                      res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::CYLINDER: {
        const auto *               c = static_cast<const CylinderHull *>(hull);
        JPH::CylinderShapeSettings settings(c->halfHeight(), c->radius());
        auto                       res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::CONVEX: {
        const auto * cv  = static_cast<const ConvexHull *>(hull);
        const auto & pts = cv->vertices();
        if (pts.empty()) return nullptr;
        std::vector<JPH::Vec3> jpts;
        jpts.reserve(pts.size());
        for (const auto & p : pts) { jpts.emplace_back(p.x, p.y, p.z); }
        JPH::ConvexHullShapeSettings settings(jpts.data(), static_cast<int>(jpts.size()));
        auto                         res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::MESH: {
        const auto * m   = static_cast<const MeshHull *>(hull);
        const auto & v   = m->vertices();
        const auto & idx = m->indices();
        if (v.empty() || idx.empty()) return nullptr;
        JPH::VertexList jv;
        jv.reserve(v.size());
        for (const auto & pt : v) { jv.emplace_back(pt.x, pt.y, pt.z); }
        JPH::IndexedTriangleList jt;
        jt.reserve(idx.size() / 3);
        for (size_t i = 0; i + 2 < idx.size(); i += 3) { jt.emplace_back(idx[i], idx[i + 1], idx[i + 2], 0); }
        JPH::MeshShapeSettings settings(std::move(jv), std::move(jt));
        auto                   res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    }
    return nullptr;
}

class SolidEngineImpl;

class SolidImpl : public Solid {
    GN_REGISTER_RUNTIME_TYPE(Solid);
    using Solid::Solid;

public:
    SolidImpl(SolidEngineImpl * engine, const SolidDesc & desc, JPH::BodyID bodyId, Scalar mass)
        : Solid(TYPE_INFO(), "Solid"), mEngine(engine), mHull(desc.hull), mTemper(desc.temper), mBodyId(bodyId), mEntityId(desc.entityId), mMass(mass) {}

    ~SolidImpl() override = default;

    Transform transform() const override;
    void      setTransform(const Transform & transform, bool activate = true) override;

    Vector3    position() const override;
    Quaternion orientation() const override;

    Vector3 linearVelocity() const override;
    void    setLinearVelocity(const Vector3 & velocity) override;

    Vector3 angularVelocity() const override;
    void    setAngularVelocity(const Vector3 & angularVelocity) override;

    void applyForce(const Vector3 & force) override;
    void applyImpulse(const Vector3 & impulse) override;
    void applyTorque(const Vector3 & torque) override;
    void applyAngularImpulse(const Vector3 & impulse) override;

    MotionType motionType() const override;
    void       setMotionType(MotionType type) override;

    const Temper & temper() const override { return mTemper; }
    void           setTemper(const Temper & temper) override;

    const Hull * hull() const override { return mHull.get(); }
    Box          aabb() const override;

    bool isActive() const override;
    void activate() override;
    void deactivate() override;

    uint64_t entityId() const override { return mEntityId; }
    Scalar   mass() const override { return mMass; }

    JPH::BodyID bodyId() const { return mBodyId; }
    void        invalidateBody() { mBodyId = JPH::BodyID(); }

private:
    SolidEngineImpl * mEngine = nullptr;
    AutoRef<Hull>     mHull;
    Temper            mTemper;
    JPH::BodyID       mBodyId;
    uint64_t          mEntityId = 0;
    Scalar            mMass     = 1.0f;
};

class SolidEngineImpl : public SolidEngine {
    GN_REGISTER_RUNTIME_TYPE(SolidEngine);
    using SolidEngine::SolidEngine;

public:
    SolidEngineImpl(const SolidEngineDesc & desc): SolidEngine(TYPE_INFO(), "SolidEngine"), mGravity(desc.gravity), mSimulationMode(desc.simulationMode) {
        ensureJoltInitialized();

        mTempAllocator = std::make_unique<JPH::TempAllocatorImplWithMallocFallback>(32 * 1024 * 1024);

        int threads = desc.numWorkerThreads == 0 ? -1 : static_cast<int>(desc.numWorkerThreads);
        mJobSystem  = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, threads);

        mPhysicsSystem = std::make_unique<JPH::PhysicsSystem>();
        mPhysicsSystem->Init(desc.maxBodies, 0, desc.maxBodyPairs, desc.maxContactConstraints, mBPLayerInterface, mObjectVsBroadPhaseFilter,
                             mObjectLayerPairFilter);

        mPhysicsSystem->SetGravity(JPH::Vec3(mGravity.x, mGravity.y, mGravity.z));
    }

    ~SolidEngineImpl() override {
        auto & bi = mPhysicsSystem->GetBodyInterface();
        for (auto & s : mSolids) {
            if (!s->bodyId().IsInvalid()) {
                bi.RemoveBody(s->bodyId());
                bi.DestroyBody(s->bodyId());
                s->invalidateBody();
            }
        }
        mSolids.clear();
        mBodyIdToSolid.clear();
    }

    AutoRef<Solid> createSolid(const SolidDesc & desc) override {
        JPH::ShapeRefC shape = createJoltShape(desc.hull.get());
        if (!shape) return {};

        JPH::EMotionType jmt = JPH::EMotionType::Dynamic;
        switch (desc.motionType) {
        case MotionType::STATIC:
            jmt = JPH::EMotionType::Static;
            break;
        case MotionType::KINEMATIC:
            jmt = JPH::EMotionType::Kinematic;
            break;
        case MotionType::DYNAMIC:
            jmt = JPH::EMotionType::Dynamic;
            break;
        }

        JPH::ObjectLayer layer = static_cast<JPH::ObjectLayer>(desc.layer);

        JPH::RVec3 pos(desc.transform.position.x, desc.transform.position.y, desc.transform.position.z);
        JPH::Quat  rot(desc.transform.orientation.v.x, desc.transform.orientation.v.y, desc.transform.orientation.v.z, desc.transform.orientation.w);

        JPH::BodyCreationSettings settings(shape, pos, rot, jmt, layer);

        Scalar mass = desc.massOverride;
        if (mass <= 0.0f && desc.hull) { mass = desc.hull->calculateVolume() * desc.temper.density; }
        if (mass <= 0.0f) mass = 1.0f;

        if (desc.motionType == MotionType::DYNAMIC) {
            settings.mOverrideMassProperties       = JPH::EOverrideMassProperties::CalculateInertia;
            settings.mMassPropertiesOverride.mMass = mass;
        }

        settings.mRestitution    = desc.temper.restitution;
        settings.mFriction       = desc.temper.friction;
        settings.mLinearDamping  = desc.temper.linearDamping;
        settings.mAngularDamping = desc.temper.angularDamping;
        settings.mAllowSleeping  = desc.allowSleep;
        settings.mMotionQuality  = desc.ccd ? JPH::EMotionQuality::LinearCast : JPH::EMotionQuality::Discrete;

        auto &      bi   = mPhysicsSystem->GetBodyInterface();
        JPH::Body * body = bi.CreateBody(settings);
        if (!body) return {};

        auto solid = AutoRef<SolidImpl>(new SolidImpl(this, desc, body->GetID(), mass));
        body->SetUserData(reinterpret_cast<uint64_t>(solid.get()));

        bi.AddBody(body->GetID(), desc.motionType == MotionType::DYNAMIC ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);

        if (desc.motionType == MotionType::DYNAMIC) {
            bi.SetLinearVelocity(body->GetID(), JPH::Vec3(desc.linearVelocity.x, desc.linearVelocity.y, desc.linearVelocity.z));
            bi.SetAngularVelocity(body->GetID(), JPH::Vec3(desc.angularVelocity.x, desc.angularVelocity.y, desc.angularVelocity.z));
        }

        mSolids.push_back(solid);
        mBodyIdToSolid[body->GetID().GetIndexAndSequenceNumber()] = solid.get();

        return solid;
    }

    void removeSolid(Solid * solid) override {
        if (!solid) return;
        auto *      sImpl = static_cast<SolidImpl *>(solid);
        JPH::BodyID bId   = sImpl->bodyId();
        if (!bId.IsInvalid()) {
            auto & bi = mPhysicsSystem->GetBodyInterface();
            bi.RemoveBody(bId);
            bi.DestroyBody(bId);
            sImpl->invalidateBody();
            mBodyIdToSolid.erase(bId.GetIndexAndSequenceNumber());
        }
        auto it = std::find_if(mSolids.begin(), mSolids.end(), [solid](const AutoRef<SolidImpl> & s) { return s.get() == solid; });
        if (it != mSolids.end()) { mSolids.erase(it); }
    }

    void step(UnitOfTime dt) override {
        int64_t ns = dt.count();
        if (ns == 0) return;

        if (ns > 0) {
            float seconds = static_cast<float>(ns) * 1e-9f;
            int   steps   = std::max(1, static_cast<int>(std::ceil(seconds / (1.0f / 60.0f))));
            mPhysicsSystem->Update(seconds, steps, mTempAllocator.get(), mJobSystem.get());
        } else {
            // T-symmetric time reversal via Hamiltonian involution T o U_|dt| o T
            float seconds = static_cast<float>(-ns) * 1e-9f;
            int   steps   = std::max(1, static_cast<int>(std::ceil(seconds / (1.0f / 60.0f))));
            float substep = seconds / static_cast<float>(steps);

            // Invert velocities
            for (auto & s : mSolids) {
                if (s->motionType() == MotionType::DYNAMIC) {
                    Vector3 v = s->linearVelocity();
                    Vector3 w = s->angularVelocity();
                    s->setLinearVelocity(-v);
                    s->setAngularVelocity(-w);
                }
            }

            // Forward update by |seconds|
            mPhysicsSystem->Update(seconds, steps, mTempAllocator.get(), mJobSystem.get());

            // Invert velocities back and correct semi-implicit Euler step asymmetry:
            // x(t - dt) = x(t) - v*dt, compensating for the acceleration term during reversal
            Vector3 gOffset = mGravity * (substep * seconds);
            for (auto & s : mSolids) {
                if (s->motionType() == MotionType::DYNAMIC) {
                    Vector3 v = s->linearVelocity();
                    Vector3 w = s->angularVelocity();
                    s->setLinearVelocity(-v);
                    s->setAngularVelocity(-w);
                    s->setTransform(Transform(s->position() - gOffset, s->orientation()));
                }
            }
        }
    }

    void setGravity(const Vector3 & gravity) override {
        mGravity = gravity;
        mPhysicsSystem->SetGravity(JPH::Vec3(mGravity.x, mGravity.y, mGravity.z));
    }

    Vector3 gravity() const override { return mGravity; }

    bool raycast(const Vector3 & origin, const Vector3 & direction, Scalar maxDistance, RaycastHit & outHit) const override {
        outHit = RaycastHit {};
        if (maxDistance <= 0.0f) return false;

        JPH::RRayCast ray(JPH::RVec3(origin.x, origin.y, origin.z), JPH::Vec3(direction.x * maxDistance, direction.y * maxDistance, direction.z * maxDistance));

        JPH::RayCastResult hit;
        bool               hasHit = mPhysicsSystem->GetNarrowPhaseQuery().CastRay(ray, hit);
        if (hasHit) {
            outHit.hit        = true;
            outHit.distance   = hit.mFraction * maxDistance;
            JPH::RVec3 hitPos = ray.GetPointOnRay(hit.mFraction);
            outHit.position   = Vector3(static_cast<float>(hitPos.GetX()), static_cast<float>(hitPos.GetY()), static_cast<float>(hitPos.GetZ()));

            JPH::BodyLockRead lock(mPhysicsSystem->GetBodyLockInterface(), hit.mBodyID);
            if (lock.Succeeded()) {
                const JPH::Body & body   = lock.GetBody();
                JPH::Vec3         normal = body.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, hitPos);
                outHit.normal            = Vector3(normal.GetX(), normal.GetY(), normal.GetZ());
                uint64_t userData        = body.GetUserData();
                if (userData) { outHit.solid = AutoRef<Solid>(reinterpret_cast<Solid *>(userData)); }
            }
            return true;
        }
        return false;
    }

    size_t bodyCount() const override { return mSolids.size(); }

    size_t activeBodyCount() const override { return mPhysicsSystem->GetNumActiveBodies(JPH::EBodyType::RigidBody); }

    void setSimulationMode(SimulationMode mode) override { mSimulationMode = mode; }

    SimulationMode simulationMode() const override { return mSimulationMode; }

    JPH::BodyInterface & bodyInterface() { return mPhysicsSystem->GetBodyInterface(); }

private:
    Vector3                                   mGravity;
    SimulationMode                            mSimulationMode;
    BPLayerInterfaceImpl                      mBPLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl         mObjectVsBroadPhaseFilter;
    ObjectLayerPairFilterImpl                 mObjectLayerPairFilter;
    std::unique_ptr<JPH::TempAllocator>       mTempAllocator;
    std::unique_ptr<JPH::JobSystem>           mJobSystem;
    std::unique_ptr<JPH::PhysicsSystem>       mPhysicsSystem;
    std::vector<AutoRef<SolidImpl>>           mSolids;
    std::unordered_map<uint32_t, SolidImpl *> mBodyIdToSolid;
};

// SolidImpl methods implementation
Transform SolidImpl::transform() const {
    if (mBodyId.IsInvalid()) return {};
    auto &     bi = mEngine->bodyInterface();
    JPH::RVec3 p  = bi.GetPosition(mBodyId);
    JPH::Quat  q  = bi.GetRotation(mBodyId);
    return Transform(Vector3(static_cast<float>(p.GetX()), static_cast<float>(p.GetY()), static_cast<float>(p.GetZ())),
                     Quaternion(q.GetX(), q.GetY(), q.GetZ(), q.GetW()));
}

void SolidImpl::setTransform(const Transform & transform, bool activate) {
    if (mBodyId.IsInvalid()) return;
    auto & bi = mEngine->bodyInterface();
    bi.SetPositionAndRotation(mBodyId, JPH::RVec3(transform.position.x, transform.position.y, transform.position.z),
                              JPH::Quat(transform.orientation.v.x, transform.orientation.v.y, transform.orientation.v.z, transform.orientation.w),
                              activate ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);
}

Vector3 SolidImpl::position() const {
    if (mBodyId.IsInvalid()) return {};
    auto &     bi = mEngine->bodyInterface();
    JPH::RVec3 p  = bi.GetPosition(mBodyId);
    return Vector3(static_cast<float>(p.GetX()), static_cast<float>(p.GetY()), static_cast<float>(p.GetZ()));
}

Quaternion SolidImpl::orientation() const {
    if (mBodyId.IsInvalid()) return Quaternion::sIdentity();
    auto &    bi = mEngine->bodyInterface();
    JPH::Quat q  = bi.GetRotation(mBodyId);
    return Quaternion(q.GetX(), q.GetY(), q.GetZ(), q.GetW());
}

Vector3 SolidImpl::linearVelocity() const {
    if (mBodyId.IsInvalid()) return {};
    auto &    bi = mEngine->bodyInterface();
    JPH::Vec3 v  = bi.GetLinearVelocity(mBodyId);
    return Vector3(v.GetX(), v.GetY(), v.GetZ());
}

void SolidImpl::setLinearVelocity(const Vector3 & velocity) {
    if (mBodyId.IsInvalid()) return;
    auto & bi = mEngine->bodyInterface();
    bi.SetLinearVelocity(mBodyId, JPH::Vec3(velocity.x, velocity.y, velocity.z));
}

Vector3 SolidImpl::angularVelocity() const {
    if (mBodyId.IsInvalid()) return {};
    auto &    bi = mEngine->bodyInterface();
    JPH::Vec3 w  = bi.GetAngularVelocity(mBodyId);
    return Vector3(w.GetX(), w.GetY(), w.GetZ());
}

void SolidImpl::setAngularVelocity(const Vector3 & angularVelocity) {
    if (mBodyId.IsInvalid()) return;
    auto & bi = mEngine->bodyInterface();
    bi.SetAngularVelocity(mBodyId, JPH::Vec3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
}

void SolidImpl::applyForce(const Vector3 & force) {
    if (mBodyId.IsInvalid()) return;
    auto & bi = mEngine->bodyInterface();
    bi.AddForce(mBodyId, JPH::Vec3(force.x, force.y, force.z));
}

void SolidImpl::applyImpulse(const Vector3 & impulse) {
    if (mBodyId.IsInvalid()) return;
    auto & bi = mEngine->bodyInterface();
    bi.AddImpulse(mBodyId, JPH::Vec3(impulse.x, impulse.y, impulse.z));
}

void SolidImpl::applyTorque(const Vector3 & torque) {
    if (mBodyId.IsInvalid()) return;
    auto & bi = mEngine->bodyInterface();
    bi.AddTorque(mBodyId, JPH::Vec3(torque.x, torque.y, torque.z));
}

void SolidImpl::applyAngularImpulse(const Vector3 & impulse) {
    if (mBodyId.IsInvalid()) return;
    auto & bi = mEngine->bodyInterface();
    bi.AddAngularImpulse(mBodyId, JPH::Vec3(impulse.x, impulse.y, impulse.z));
}

MotionType SolidImpl::motionType() const {
    if (mBodyId.IsInvalid()) return MotionType::STATIC;
    auto & bi = mEngine->bodyInterface();
    switch (bi.GetMotionType(mBodyId)) {
    case JPH::EMotionType::Static:
        return MotionType::STATIC;
    case JPH::EMotionType::Kinematic:
        return MotionType::KINEMATIC;
    case JPH::EMotionType::Dynamic:
        return MotionType::DYNAMIC;
    }
    return MotionType::STATIC;
}

void SolidImpl::setMotionType(MotionType type) {
    if (mBodyId.IsInvalid()) return;
    auto &           bi = mEngine->bodyInterface();
    JPH::EMotionType mt = JPH::EMotionType::Dynamic;
    switch (type) {
    case MotionType::STATIC:
        mt = JPH::EMotionType::Static;
        break;
    case MotionType::KINEMATIC:
        mt = JPH::EMotionType::Kinematic;
        break;
    case MotionType::DYNAMIC:
        mt = JPH::EMotionType::Dynamic;
        break;
    }
    bi.SetMotionType(mBodyId, mt, JPH::EActivation::Activate);
}

void SolidImpl::setTemper(const Temper & temper) {
    mTemper = temper;
    if (mBodyId.IsInvalid()) return;
    auto & bi = mEngine->bodyInterface();
    bi.SetFriction(mBodyId, temper.friction);
    bi.SetRestitution(mBodyId, temper.restitution);
}

Box SolidImpl::aabb() const {
    if (mBodyId.IsInvalid()) return {};
    auto &     bi   = mEngine->bodyInterface();
    JPH::AABox jBox = bi.GetTransformedShape(mBodyId).GetWorldSpaceBounds();
    Vector3    minPt(jBox.mMin.GetX(), jBox.mMin.GetY(), jBox.mMin.GetZ());
    Vector3    maxPt(jBox.mMax.GetX(), jBox.mMax.GetY(), jBox.mMax.GetZ());
    return Box(minPt, maxPt);
}

bool SolidImpl::isActive() const {
    if (mBodyId.IsInvalid()) return false;
    return mEngine->bodyInterface().IsActive(mBodyId);
}

void SolidImpl::activate() {
    if (mBodyId.IsInvalid()) return;
    mEngine->bodyInterface().ActivateBody(mBodyId);
}

void SolidImpl::deactivate() {
    if (mBodyId.IsInvalid()) return;
    mEngine->bodyInterface().DeactivateBody(mBodyId);
}

AutoRef<SolidEngine> SolidEngine::create(const SolidEngineDesc & desc) { return AutoRef<SolidEngine>(new SolidEngineImpl(desc)); }

} // namespace GN::fiz

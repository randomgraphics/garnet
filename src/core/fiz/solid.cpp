#include "fiz-jolt-common.h"

#include <cmath>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "solid-impl.h"

namespace GN::fiz {

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

        auto solid = AutoRef<SolidImpl>(new SolidImpl(&bi, desc, body->GetID(), mass));
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

AutoRef<SolidEngine> SolidEngine::create(const SolidEngineDesc & desc) { return AutoRef<SolidEngine>(new SolidEngineImpl(desc)); }

} // namespace GN::fiz

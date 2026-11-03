#pragma once

#include "fiz-jolt-common.h"

namespace GN::fiz {

class SolidImpl : public Solid {
    GN_REGISTER_RUNTIME_TYPE(Solid);
    using Solid::Solid;

public:
    SolidImpl(JPH::BodyInterface * bi, const JPH::BodyLockInterface * bodyLocks, std::mutex * mutationMutex, const SolidDesc & desc, JPH::BodyID bodyId,
              Scalar mass)
        : Solid(TYPE_INFO(), "Solid"), mBodyInterface(bi), mBodyLocks(bodyLocks), mMutationMutex(mutationMutex), mHull(desc.hull), mTemper(desc.temper),
          mBodyId(bodyId), mEntityId(desc.entityId), mMassOverride(desc.massOverride), mMass(mass) {}

    ~SolidImpl() override = default;

    Transform transform() const override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return {};
        JPH::RVec3 p = mBodyInterface->GetPosition(mBodyId);
        JPH::Quat  q = mBodyInterface->GetRotation(mBodyId);
        return Transform(Vector3(static_cast<float>(p.GetX()), static_cast<float>(p.GetY()), static_cast<float>(p.GetZ())),
                         Quaternion(q.GetX(), q.GetY(), q.GetZ(), q.GetW()));
    }

    void setTransform(const Transform & transform, bool activate = true) override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return;
        mBodyInterface->SetPositionAndRotation(
            mBodyId, JPH::RVec3(transform.position.x, transform.position.y, transform.position.z),
            JPH::Quat(transform.orientation.v.x, transform.orientation.v.y, transform.orientation.v.z, transform.orientation.w),
            activate ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);
    }

    Vector3 position() const override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return {};
        JPH::RVec3 p = mBodyInterface->GetPosition(mBodyId);
        return Vector3(static_cast<float>(p.GetX()), static_cast<float>(p.GetY()), static_cast<float>(p.GetZ()));
    }

    Quaternion orientation() const override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return Quaternion::sIdentity();
        JPH::Quat q = mBodyInterface->GetRotation(mBodyId);
        return Quaternion(q.GetX(), q.GetY(), q.GetZ(), q.GetW());
    }

    Vector3 linearVelocity() const override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return {};
        JPH::Vec3 v = mBodyInterface->GetLinearVelocity(mBodyId);
        return Vector3(v.GetX(), v.GetY(), v.GetZ());
    }

    void setLinearVelocity(const Vector3 & velocity) override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return;
        mBodyInterface->SetLinearVelocity(mBodyId, JPH::Vec3(velocity.x, velocity.y, velocity.z));
    }

    Vector3 angularVelocity() const override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return {};
        JPH::Vec3 w = mBodyInterface->GetAngularVelocity(mBodyId);
        return Vector3(w.GetX(), w.GetY(), w.GetZ());
    }

    void setAngularVelocity(const Vector3 & angularVelocity) override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return;
        mBodyInterface->SetAngularVelocity(mBodyId, JPH::Vec3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
    }

    void applyForce(const Vector3 & force) override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return;
        mBodyInterface->AddForce(mBodyId, JPH::Vec3(force.x, force.y, force.z));
    }

    void applyImpulse(const Vector3 & impulse) override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return;
        mBodyInterface->AddImpulse(mBodyId, JPH::Vec3(impulse.x, impulse.y, impulse.z));
    }

    void applyTorque(const Vector3 & torque) override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return;
        mBodyInterface->AddTorque(mBodyId, JPH::Vec3(torque.x, torque.y, torque.z));
    }

    void applyAngularImpulse(const Vector3 & impulse) override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return;
        mBodyInterface->AddAngularImpulse(mBodyId, JPH::Vec3(impulse.x, impulse.y, impulse.z));
    }

    MotionType motionType() const override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return MotionType::STATIC;
        switch (mBodyInterface->GetMotionType(mBodyId)) {
        case JPH::EMotionType::Static:
            return MotionType::STATIC;
        case JPH::EMotionType::Kinematic:
            return MotionType::KINEMATIC;
        case JPH::EMotionType::Dynamic:
            return MotionType::DYNAMIC;
        }
        return MotionType::STATIC;
    }

    void setMotionType(MotionType type) override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return;
        JPH::EMotionType jtype = JPH::EMotionType::Dynamic;
        switch (type) {
        case MotionType::STATIC:
            jtype = JPH::EMotionType::Static;
            break;
        case MotionType::KINEMATIC:
            jtype = JPH::EMotionType::Kinematic;
            break;
        case MotionType::DYNAMIC:
            jtype = JPH::EMotionType::Dynamic;
            break;
        }
        mBodyInterface->SetMotionType(mBodyId, jtype, JPH::EActivation::Activate);
    }

    const Temper & temper() const override { return mTemper; }

    void setTemper(const Temper & temper) override {
        mTemper = temper;
        if (!mBodyId.IsInvalid() && mBodyInterface) {
            mBodyInterface->SetFriction(mBodyId, temper.friction);
            mBodyInterface->SetRestitution(mBodyId, temper.restitution);
        }
    }

    const Hull * hull() const override { return mHull.get(); }

    bool setHull(const AutoRef<Hull> & hull, bool updateMassProperties = true) override {
        if (!hull) {
            GN_ERROR(getLogger("GN.fiz.Solid"), "Solid::setHull: null Hull supplied; current collision shape is unchanged.");
            return false;
        }
        if (!mBodyInterface || !mBodyLocks || !mMutationMutex || mBodyId.IsInvalid()) {
            GN_ERROR(getLogger("GN.fiz.Solid"), "Solid::setHull: physics owner is detached or body {} is invalid; current collision shape is unchanged.",
                     mBodyId.GetIndexAndSequenceNumber());
            return false;
        }

        JPH::ShapeRefC shape = createJoltShape(hull.get());
        if (!shape) return false;

        // Serialize structural shape changes against simulation and other create/remove operations.
        std::lock_guard<std::mutex> engineLock(*mMutationMutex);
        if (mBodyId.IsInvalid()) return false;

        const bool          isDynamic = motionType() == MotionType::DYNAMIC;
        Scalar              nextMass  = mMass;
        JPH::MassProperties massProperties;
        if (isDynamic && updateMassProperties) {
            const Scalar volume = hull->calculateVolume();
            nextMass            = mMassOverride > 0 ? mMassOverride : volume * mTemper.density;
            if (!(nextMass > 0) || !std::isfinite(nextMass)) {
                GN_ERROR(getLogger("GN.fiz.Solid"),
                         "Solid::setHull: cannot derive positive mass for dynamic hull type {} (volume={}, density={}, massOverride={}).",
                         static_cast<uint32_t>(hull->type()), volume, mTemper.density, mMassOverride);
                return false;
            }
            massProperties = shape->GetMassProperties();
            if (!(massProperties.mMass > 0) || !std::isfinite(massProperties.mMass)) {
                GN_ERROR(getLogger("GN.fiz.Solid"), "Solid::setHull: Jolt hull type {} does not provide usable mass properties.",
                         static_cast<uint32_t>(hull->type()));
                return false;
            }
            massProperties.ScaleToMass(nextMass);
        }

        mBodyInterface->SetShape(mBodyId, shape.GetPtr(), false, JPH::EActivation::Activate);

        if (isDynamic && updateMassProperties) {
            JPH::BodyLockWrite bodyLock(*mBodyLocks, mBodyId);
            if (!bodyLock.Succeeded()) {
                GN_ERROR(getLogger("GN.fiz.Solid"), "Solid::setHull: Jolt body lock failed after shape replacement (body id {}).",
                         mBodyId.GetIndexAndSequenceNumber());
                return false;
            }
            auto * motion = bodyLock.GetBody().GetMotionProperties();
            if (!motion) {
                GN_ERROR(getLogger("GN.fiz.Solid"), "Solid::setHull: dynamic Jolt body has no MotionProperties after shape replacement (body id {}).",
                         mBodyId.GetIndexAndSequenceNumber());
                return false;
            }
            motion->SetMassProperties(motion->GetAllowedDOFs(), massProperties);
            mMass = nextMass;
        }

        mHull = hull;
        return true;
    }

    Box aabb() const override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return {};
        JPH::AABox b = mBodyInterface->GetTransformedShape(mBodyId).GetWorldSpaceBounds();
        return Box(b.mMin.GetX(), b.mMin.GetY(), b.mMin.GetZ(), b.mMax.GetX() - b.mMin.GetX(), b.mMax.GetY() - b.mMin.GetY(), b.mMax.GetZ() - b.mMin.GetZ());
    }

    bool isActive() const override {
        if (mBodyId.IsInvalid() || !mBodyInterface) return false;
        return mBodyInterface->IsActive(mBodyId);
    }

    void activate() override {
        if (!mBodyId.IsInvalid() && mBodyInterface) mBodyInterface->ActivateBody(mBodyId);
    }

    void deactivate() override {
        if (!mBodyId.IsInvalid() && mBodyInterface) mBodyInterface->DeactivateBody(mBodyId);
    }

    uint64_t entityId() const override { return mEntityId; }
    Scalar   mass() const override { return mMass; }

    JPH::BodyID bodyId() const { return mBodyId; }
    void        invalidateBody() { mBodyId = JPH::BodyID(); }

private:
    JPH::BodyInterface *           mBodyInterface = nullptr;
    const JPH::BodyLockInterface * mBodyLocks     = nullptr;
    std::mutex *                   mMutationMutex = nullptr;
    AutoRef<Hull>                  mHull;
    Temper                         mTemper;
    JPH::BodyID                    mBodyId;
    uint64_t                       mEntityId     = 0;
    Scalar                         mMassOverride = 0;
    Scalar                         mMass         = 1.0f;
};

} // namespace GN::fiz

#pragma once

#include "fiz-jolt-common.h"

namespace GN::fiz {

class SolidImpl : public Solid {
    GN_REGISTER_RUNTIME_TYPE(Solid);
    using Solid::Solid;

public:
    SolidImpl(JPH::BodyInterface * bi, const SolidDesc & desc, JPH::BodyID bodyId, Scalar mass)
        : Solid(TYPE_INFO(), "Solid"), mBodyInterface(bi), mHull(desc.hull), mTemper(desc.temper), mBodyId(bodyId), mEntityId(desc.entityId), mMass(mass) {}

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
    JPH::BodyInterface * mBodyInterface = nullptr;
    AutoRef<Hull>        mHull;
    Temper               mTemper;
    JPH::BodyID          mBodyId;
    uint64_t             mEntityId = 0;
    Scalar               mMass     = 1.0f;
};

} // namespace GN::fiz

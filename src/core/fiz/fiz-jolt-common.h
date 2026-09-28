#pragma once

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
#include <vector>

namespace GN::fiz {

void ensureJoltInitialized();

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

JPH::ShapeRefC createJoltShape(const Hull * hull);

} // namespace GN::fiz

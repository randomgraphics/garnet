#pragma once
#if !defined(__GN_INSIDE_FIZ_H__)
    #error "Do not include <garnet/fiz/common.h> directly. Include <garnet/GNfiz.h> instead."
#endif

#include <chrono>
#include <cstdint>

namespace GN::fiz {

/// Canonical unit of discrete integer time across GNfiz and GNengine2.
/// Stored as signed 64-bit integer nanoseconds. Stepping with negative
/// duration (-dt) allows deterministic backward evolution for time-reversible systems.
using UnitOfTime = std::chrono::nanoseconds;

/// Fundamental scalar precision in fiz. Set to 32-bit floating point.
/// Unit-agnostic: a scalar value of 1.0 can represent 1 meter (robotics/games),
/// 1 astronomical unit (celestial mechanics), or 1 nanometer (microphysics).
using Scalar = float;

/// Common math types aliased to Scalar precision.
using Vector3    = GN::Vector3<Scalar>;
using Vector4    = GN::Vector4<Scalar>;
using Quaternion = GN::Quaternion<Scalar>;
using Matrix44   = GN::Matrix44<Scalar>;
using Box        = GN::Box<Scalar>;
using Sphere     = GN::Sphere<Scalar>;

/// Transform representation in physical simulation coordinates.
struct Transform {
    Vector3    position    = {0.0f, 0.0f, 0.0f};
    Quaternion orientation = Quaternion::sIdentity();

    Transform() = default;
    Transform(const Vector3 & p, const Quaternion & q): position(p), orientation(q) {}
};

/// Discrete collision layers for broadphase filtering.
enum class CollisionLayer : uint16_t {
    NON_MOVING = 0, ///< Static scene geometry (terrain, buildings, immovable obstacles).
    MOVING     = 1, ///< Dynamic simulated entities (rigid bodies, vehicles, soft bodies).
    TRIGGER    = 2, ///< Non-physical query volumes / sensors.
    DEBRIS     = 3, ///< Small debris that does not collide with each other.
    COUNT      = 4
};

/// Motion dynamics type for simulated physical bodies.
enum class MotionType : uint8_t {
    STATIC    = 0, ///< Zero mass, immovable, unaffected by forces/collisions.
    KINEMATIC = 1, ///< Infinite mass, driven by programmatic velocity/transform targets.
    DYNAMIC   = 2  ///< Finite mass, simulated fully by equations of motion and constraints.
};

/// Simulation solver mode controlling whether dissipative effects are applied.
enum class SimulationMode : uint8_t {
    DISSIPATIVE  = 0, ///< Normal physics: friction, inelastic restitution, and damping active.
    CONSERVATIVE = 1  ///< Strictly conservative: zero dissipation, perfectly elastic, symplectic time-reversible.
};

} // namespace GN::fiz

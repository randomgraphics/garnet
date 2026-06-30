
namespace GN::e2 {

/// Strongly-typed unit of length used in this world. Defined here for future
/// use; current aliases (Location, Distance) use plain float meters for simplicity.
struct UnitOfLength {
    int64_t _value = 0;

    constexpr UnitOfLength() = default;

    explicit constexpr UnitOfLength(int64_t value): _value(value) {}

    constexpr UnitOfLength operator+(UnitOfLength o) const { return UnitOfLength(_value + o._value); }
    constexpr UnitOfLength operator-(UnitOfLength o) const { return UnitOfLength(_value - o._value); }
    constexpr UnitOfLength operator*(UnitOfLength o) const { return UnitOfLength(_value * o._value); }
    constexpr UnitOfLength operator/(UnitOfLength o) const { return UnitOfLength(_value / o._value); }
};

/// Spacial location defined by 3D coordinate within the world.
using WorldPosition = Vector3<UnitOfLength>;

// Spacial orientation defined by a rotation primitive.
using Rotation = glm::quat;

// struct Transformation {
//     glm::vec3 translation;
//     glm::quat rotation;
//     glm::vec3 scaling;
// };

// struct RigidBodyOperator : {

//     const WorldPosition & position() const { return mPosition; }

//     const Rotation & orientation() const { return mOrientation; }

//     void moveTo(const WorldPosition & pos) { mPosition = pos; }

//     void rotateTo(const Rotation & ori) { mOrientation = ori; }
//     void teleportTo(const WorldPosition & pos, const Rotation & ori) {
//         mPosition    = pos;
//         mOrientation = ori;
//     }

// private:
//     WorldPosition mPosition;
//     Rotation      mOrientation;
// };

} // namespace GN::e2
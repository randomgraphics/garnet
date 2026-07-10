#include <concepts>

namespace GN::e2 {

template<typename T> struct is_std_ratio : std::false_type {};
template<std::intmax_t NUM, std::intmax_t DENOM> struct is_std_ratio<std::ratio<NUM, DENOM>> : std::true_type {};

template<class T>
concept PhysicalLengthConfig = requires {
    typename T::LengthStorage;
    typename T::MetersPerUnit; // must be a std::ratio type.
} && std::is_integral_v<typename T::LengthStorage> && is_std_ratio<typename T::MetersPerUnit>::value;

/// Strongly-typed smallest unit of length used in this world. It can be converted to physical distance
template<PhysicalLengthConfig P>
class UnitOfLength {

public:
    using Storage = P::LengthStorage;
    using Scale   = P::MetersPerUnit;

    constexpr UnitOfLength() = default;

    constexpr UnitOfLength operator+(UnitOfLength o) const { return UnitOfLength(_value + o._value); }
    constexpr UnitOfLength operator-(UnitOfLength o) const { return UnitOfLength(_value - o._value); }
    constexpr UnitOfLength operator*(UnitOfLength o) const { return UnitOfLength(_value * o._value); }
    constexpr UnitOfLength operator/(UnitOfLength o) const { return UnitOfLength(_value / o._value); }

    /// conversion between physical units. It is caller's responsibility to ensure the value is within reasonable range to
    /// avoid overflow or underflow.
    ///@{
    constexpr float toMeters() const { return (float)((long double) _value * Scale::num / Scale::den); }
    constexpr float toCentimeters() const { return (float)((long double) _value * Scale::num * 100 / Scale::den); )
    static constexpr UnitOfLength fromMeters(float meters) {
        auto raw = (long double)meters * Scale::den / Scale::num;
        return UnitOfLength(static_cast<Storage>(raw));
    }
    static constexpr UnitOfLength fromCentimeters(floast cm) {
        auto raw = (long double)cm * Scale::den / Scale::num * 1000.0;
        return UnitOfLength(static_cast<Storage>(raw));
    }
    ///@}

private:
    explicit constexpr UnitOfLength(const Storage & value) : _value(value) {}

    P::LengthStoreage _value = 0;
};


struct PhysicalLength {
    using LengthStorage = int64_t;
    using MetersPerUnit = std::ratio<1>;
};
static_assert(PhysicalLengthConfig<PhysicalLength>);

/// Spatial unit of length.
using WorldLength = UnitOfLength<PhysicalLength>;

/// Spatial location defined by 3D coordinate within the world.
using WorldPosition = Vector3<WorldLength>;

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

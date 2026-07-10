#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/spatial.h> directly. Include <garnet/GNengine2.h> instead."
#endif

#include <concepts>
#include <cstdint>

namespace GN::e2 {

/// Strongly-typed smallest unit of length used in a world. The physical size of one unit
/// (meters-per-unit) is a per-world runtime constant (see World::metersPerUnit), so all
/// conversions to/from physical units take the scale as an argument.
template<std::integral S>
class UnitOfLength {

    // Round to nearest instead of truncating: scale divisions like 2.0 / 0.001 yield values a hair
    // below the exact result (1999.999...), which truncation would turn into an off-by-one unit.
    // (std::llround is not constexpr in C++20.)
    static constexpr S roundToStorage(double v) { return static_cast<S>(v < 0.0 ? v - 0.5 : v + 0.5); }

public:
    using Storage = S;

    constexpr UnitOfLength() = default;

    explicit constexpr UnitOfLength(Storage value): _value(value) {}

    /// Raw value in world units.
    constexpr Storage raw() const { return _value; }

    constexpr UnitOfLength operator+(UnitOfLength o) const { return UnitOfLength(_value + o._value); }
    constexpr UnitOfLength operator-(UnitOfLength o) const { return UnitOfLength(_value - o._value); }
    constexpr UnitOfLength operator*(UnitOfLength o) const { return UnitOfLength(_value * o._value); }
    constexpr UnitOfLength operator/(UnitOfLength o) const { return UnitOfLength(_value / o._value); }

    /// Conversion between world units and physical units. \p metersPerUnit is the physical size of
    /// one unit in meters. It is caller's responsibility to ensure the value is within reasonable
    /// range to avoid overflow or underflow.
    ///@{
    constexpr float toMeters(double metersPerUnit) const { return (float) ((double) _value * metersPerUnit); }
    constexpr float toCentimeters(double metersPerUnit) const { return (float) ((double) _value * metersPerUnit * 100.0); }
    static constexpr UnitOfLength fromMeters(float meters, double metersPerUnit) { return UnitOfLength(roundToStorage((double) meters / metersPerUnit)); }
    static constexpr UnitOfLength fromCentimeters(float cm, double metersPerUnit) { return UnitOfLength(roundToStorage((double) cm / 100.0 / metersPerUnit)); }
    ///@}

private:
    Storage _value = 0;
};

/// Spatial unit of length.
using WorldLength = UnitOfLength<int64_t>;

/// Spatial location defined by 3D coordinate within the world.
using WorldPosition = Vector3<WorldLength>;

// Spatial orientation defined by a rotation primitive.
using Rotation = glm::quat;

} // namespace GN::e2

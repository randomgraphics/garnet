#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>        // glm::quat, glm::mat4_cast, glm::angleAxis
#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::scale, glm::normalize
#include <glm/ext/matrix_clip_space.hpp> // glm::perspectiveRH_ZO
#include <glm/ext/matrix_transform.hpp>  // glm::lookAtRH

namespace GN::rdg {

/// Strongly-typed distance in micrometers (integer).  Defined here for future
/// use; current aliases (Location, Distance) use plain float meters for simplicity.
struct WorldUnit {
    int64_t _value = 0; ///< micrometers

    constexpr WorldUnit() = default;
    explicit constexpr WorldUnit(int64_t value): _value(value) {}

    constexpr WorldUnit operator+(WorldUnit o) const { return WorldUnit(_value + o._value); }
    constexpr WorldUnit operator-(WorldUnit o) const { return WorldUnit(_value - o._value); }
    constexpr WorldUnit operator*(WorldUnit o) const { return WorldUnit(_value * o._value); }
    constexpr WorldUnit operator/(WorldUnit o) const { return WorldUnit(_value / o._value); }
};

inline static constexpr WorldUnit operator""_um(unsigned long long v) { return WorldUnit(v); }
inline static constexpr WorldUnit operator""_mm(unsigned long long v) { return WorldUnit(v * 1'000); }
inline static constexpr WorldUnit operator""_cm(unsigned long long v) { return WorldUnit(v * 10'000); }
inline static constexpr WorldUnit operator""_meter(unsigned long long v) { return WorldUnit(v * 1'000'000); }
inline static constexpr WorldUnit operator""_km(unsigned long long v) { return WorldUnit(v * 1'000'000'000); }
inline static constexpr WorldUnit operator""_inch(unsigned long long v) { return WorldUnit(v * 25'400); }
inline static constexpr WorldUnit operator""_foot(unsigned long long v) { return WorldUnit(v * 304'800); }
inline static constexpr WorldUnit operator""_yard(unsigned long long v) { return WorldUnit(v * 914'400); }
inline static constexpr WorldUnit operator""_mile(unsigned long long v) { return WorldUnit(v * 1'609'344'000); }

using Location    = glm::vec3; ///< x, y, z in meters (float)
using Orientation = glm::quat;
using Distance    = float; ///< distance in meters (float)

inline static constexpr Orientation ZERO_ROTATION = Orientation(1.f, 0.f, 0.f, 0.f); // identity quaternion (w, x, y, z)

struct Degree;

struct Radian {
    float value;

    explicit constexpr Radian(float value_in_radians): value(value_in_radians) {}

    constexpr Radian(Degree degree);

    // Explicit copy constructor required to suppress -Wdeprecated-copy-with-user-provided-copy
    // (the user-provided copy assignment operator below would otherwise suppress the implicit copy constructor).
    constexpr Radian(const Radian &) = default;

    constexpr Radian & operator=(const Radian & other) {
        value = other.value;
        return *this;
    }

    // conversion operators
    constexpr operator float() const { return value; }

    // relationship operators
    constexpr bool operator==(const Radian & other) const { return value == other.value; }
    constexpr bool operator!=(const Radian & other) const { return value != other.value; }
    constexpr bool operator<(const Radian & other) const { return value < other.value; }
    constexpr bool operator>(const Radian & other) const { return value > other.value; }
    constexpr bool operator<=(const Radian & other) const { return value <= other.value; }
    constexpr bool operator>=(const Radian & other) const { return value >= other.value; }

    // math method operators
    constexpr Radian & operator+=(const Radian & other) {
        value += other.value;
        return *this;
    }
    constexpr Radian & operator-=(const Radian & other) {
        value -= other.value;
        return *this;
    }
    constexpr Radian & operator*=(const Radian & other) {
        value *= other.value;
        return *this;
    }
    constexpr Radian & operator/=(const Radian & other) {
        value /= other.value;
        return *this;
    }
    constexpr Radian operator+(const Radian & other) const { return Radian(value + other.value); }
    constexpr Radian operator-(const Radian & other) const { return Radian(value - other.value); }
    constexpr Radian operator*(const Radian & other) const { return Radian(value * other.value); }
    constexpr Radian operator/(const Radian & other) const { return Radian(value / other.value); }
};

struct Degree {
    float value;

    explicit constexpr Degree(float value_in_degrees): value(value_in_degrees) {}

    constexpr Degree(Radian radian): value(radian.value * 180.0f / GN_PI) {}
};

inline constexpr Radian::Radian(Degree degree): value(degree.value * GN_PI / 180.0f) {}

} // namespace GN::rdg

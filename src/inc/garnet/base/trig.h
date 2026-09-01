#ifndef __GN_BASE_TRIG_H__
#define __GN_BASE_TRIG_H__
// *****************************************************************************
/// \file
/// \brief   trigonometry vocabulary: pi constants and strongly-typed angle units
///          (Radian/ArcDegree) that convert between each other implicitly.
// *****************************************************************************

///
/// Pi
///
#define GN_PI 3.1415926535897932385f

///
/// Pi/2
///
#define GN_HALF_PI (GN_PI / 2.0f)

///
/// Pi*2
///
#define GN_TWO_PI (GN_PI * 2.0f)

namespace GN {

struct ArcDegree;

/// Strongly-typed angle in radians. Implicitly converts from ArcDegree and to raw float, so it
/// can be handed directly to trig functions while keeping the unit explicit at API boundaries.
struct Radian {
    float value;

    explicit constexpr Radian(float value_in_radians): value(value_in_radians) {}

    constexpr Radian(ArcDegree degree);

    // Explicit copy constructor required to suppress -Wdeprecated-copy-with-user-provided-copy
    // (the user-provided copy assignment operator below would otherwise suppress the implicit copy constructor).
    constexpr Radian(const Radian &) = default;

    constexpr Radian & operator=(const Radian & other) {
        value = other.value;
        return *this;
    }

    constexpr operator float() const { return value; }

    constexpr bool operator==(const Radian & other) const { return value == other.value; }
    constexpr bool operator!=(const Radian & other) const { return value != other.value; }
    constexpr bool operator<(const Radian & other) const { return value < other.value; }
    constexpr bool operator>(const Radian & other) const { return value > other.value; }
    constexpr bool operator<=(const Radian & other) const { return value <= other.value; }
    constexpr bool operator>=(const Radian & other) const { return value >= other.value; }

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

/// Strongly-typed angle in arc degrees (1/360 of a full turn). Converts implicitly to/from
/// Radian. Named ArcDegree (degree of arc, kin to arc minutes and arc seconds) because
/// "degree" alone is ambiguous — this type is the angle unit, never temperature.
struct ArcDegree {
    float value;

    explicit constexpr ArcDegree(float value_in_degrees): value(value_in_degrees) {}

    constexpr ArcDegree(Radian radian): value(radian.value * 180.0f / GN_PI) {}
};

inline constexpr Radian::Radian(ArcDegree degree): value(degree.value * GN_PI / 180.0f) {}

} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_TRIG_H__

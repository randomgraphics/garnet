#pragma once

#include <chrono>

namespace GN::rdg {

/// Time units
using Nanoseconds  = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;

/// Spatial units
struct WorldUnit {
    int64_t _value = 0; ///< Distance in micrometers.

public:
    constexpr WorldUnit(): _value(0) {}
    constexpr WorldUnit(int64_t value): _value(value) {}

    constexpr WorldUnit operator+(const WorldUnit & other) const { return WorldUnit(_value + other._value); }
    constexpr WorldUnit operator-(const WorldUnit & other) const { return WorldUnit(_value - other._value); }
    constexpr WorldUnit operator*(const WorldUnit & other) const { return WorldUnit(_value * other._value); }
    constexpr WorldUnit operator/(const WorldUnit & other) const { return WorldUnit(_value / other._value); }
}; ///< Distance in world units.

inline static constexpr WorldUnit operator""_um(unsigned long long value) { return WorldUnit(value); }
inline static constexpr WorldUnit operator""_mm(unsigned long long value) { return WorldUnit(value * 1000); }
inline static constexpr WorldUnit operator""_cm(unsigned long long value) { return WorldUnit(value * 10000); }
inline static constexpr WorldUnit operator""_meter(unsigned long long value) { return WorldUnit(value * 1000000); }
inline static constexpr WorldUnit operator""_km(unsigned long long value) { return WorldUnit(value * 1000000000); }

inline static constexpr WorldUnit operator""_inch(unsigned long long value) { return WorldUnit(value * 25400); }
inline static constexpr WorldUnit operator""_foot(unsigned long long value) { return WorldUnit(value * 304800); }
inline static constexpr WorldUnit operator""_mile(unsigned long long value) { return WorldUnit(value * 1609344000); }
inline static constexpr WorldUnit operator""_yard(unsigned long long value) { return WorldUnit(value * 914400); }

struct Candela {
    float value;
}; ///< Intensity unit [cd].
struct Lux {
    float value;
}; ///< Unit of illuminance [lx] = 1 lumen / m^2.
struct CandelaPerM2 {
    float value;
}; ///< Radiance (Luminance) in unit of [cd/m^2].

/// Photometric Intensity (I). Usually used to represent point and spot light color.
/// - Radiometric I = [W/sr]
/// - Photometric I = [cd]
struct IntensityRGB {
    float   r, g, b;   // RGB ratio normalized to [0, 1].
    Candela intensity; // luminous intensity in candela.
};

/// Photometric Irradiance (E). Total light power arriving at a surface per unit area. Can be used to represent directional light color.
/// - Radiometric E = [W/m^2]
/// - Photometric E = [lx] = [lm/m^2]
/// - For uniform hemisphere emission, E = L * π.
/// - For surece lit by a point light, E = I / r^2 * dot(n, l).
struct IrradianceRGB {
    float r, g, b;    // RGB ratio normalized to [0, 1].
    Lux   irradiance; // irradiance in lux.
};

/// Photometric Radiance (L). Can be used to represent area light color, like sky cubemap.
/// - Radiometric L = [W/sr/m^2]
/// - Photometric L = [cd/m^2]
struct RadianceRGB {
    float        r, g, b;   // RGB ratio normalized to [0, 1].
    CandelaPerM2 luminance; // luminance in candela per square meter.
};

}; // namespace GN::rdg
#pragma once

#include <chrono>

namespace GN::rdg {

/// Time units
using Nanoseconds  = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;

///< Intensity unit [cd].
struct Candela {
    float value;
};

/// Unit of illuminance [lx] = 1 lumen / m^2.
/// Describes how much light is falling on a surface.
struct Lux {
    float value;
};

/// Radiance (Luminance) in unit of [cd/m^2].
/// Describe brightness of area light source.
struct Nit {
    float value;
};

/// Photometric Intensity (I).
/// Can be used to represent intensity and color of light a surface receives from
/// point and spot light sources.
/// - Radiometric I = [W/sr]
/// - Photometric I = [cd]
struct IntensityRGB {
    float   r, g, b;   // RGB ratio normalized to [0, 1].
    Candela intensity; // luminous intensity in candela.
};

/// Photometric Irradiance (E). Total light power arriving at a surface per unit area.
/// Can be used to represent intensity and color a surface receives from a directional light.
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
    float r, g, b;   // RGB ratio normalized to [0, 1].
    Nit   luminance; // luminance in cd/m^2.
};

}; // namespace GN::rdg
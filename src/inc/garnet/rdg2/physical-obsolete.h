#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/physical.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

#include <chrono>

namespace GN::rdg2 {

// importing into rdg2 namespace for code compatibility

using Nanoseconds  = GN::gpu2::Nanoseconds;
using Microseconds = GN::gpu2::Microseconds;
using Candela = GN::gpu2::Candela;
using Lux = GN::gpu2::Lux;
using Nit = GN::gpu2::Nit;
using IntensityRGB = GN::gpu2::IntensityRGB;
using IrradianceRGB = GN::gpu2::IrradianceRGB;
using RadianceRGB = GN::gpu2::RadianceRGB;

} // namespace GN::rdg2

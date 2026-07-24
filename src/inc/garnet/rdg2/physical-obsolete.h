#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/physical.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

#include <chrono>

namespace GN::rdg2 {

// importing into rdg2 namespace for code compatibility

using Nanoseconds   = std::chrono::nanoseconds;
using Microseconds  = std::chrono::microseconds;
using Candela       = GN::e2::Candela;
using Lux           = GN::e2::Lux;
using Nit           = GN::e2::Nit;
using IntensityRGB  = GN::e2::IntensityRGB;
using IrradianceRGB = GN::e2::IrradianceRGB;
using RadianceRGB   = GN::e2::RadianceRGB;

} // namespace GN::rdg2

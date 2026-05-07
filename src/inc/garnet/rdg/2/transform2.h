#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg/2/transform2.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

// Type definitions are imported from v1 (GN::rdg). Once v1 is retired, the actual definitions will be moved here.
namespace GN::rdg2 {

using WorldUnit   = GN::rdg::WorldUnit;
using Location    = GN::rdg::Location;
using Orientation = GN::rdg::Orientation;
using Distance    = GN::rdg::Distance;
using Radian      = GN::rdg::Radian;
using Degree      = GN::rdg::Degree;

inline static constexpr Orientation ZERO_ROTATION = GN::rdg::ZERO_ROTATION;

using GN::rdg::operator""_um;
using GN::rdg::operator""_mm;
using GN::rdg::operator""_cm;
using GN::rdg::operator""_meter;
using GN::rdg::operator""_km;
using GN::rdg::operator""_inch;
using GN::rdg::operator""_foot;
using GN::rdg::operator""_yard;
using GN::rdg::operator""_mile;

} // namespace GN::rdg2

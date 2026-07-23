#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/spatial.h> directly. Include <garnet/GNengine2.h> instead."
#endif

#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>

#include <cmath>
#include <cstdint>
#include <limits>

namespace GN::e2 {

/// Strongly-typed absolute coordinate measured in the smallest unit of a world, stored as a
/// 128-bit signed integer composed of two 64-bit halves. Conversion to physical units lives on
/// PhysicalScale; converting an absolute coordinate is approximate (it generally exceeds a
/// double's mantissa) — rebase against a nearby coordinate with spatial::toLocal() and convert
/// the local result when precision matters.
class WorldCoordinate {
public:
    constexpr WorldCoordinate() = default;

    /// Construct directly from the two's complement halves; \p hi carries the sign. For a
    /// coordinate expressed as a unit count, widen a LocalCoordinate with spatial::toWorld().
    constexpr WorldCoordinate(int64_t hi, uint64_t lo): _hi(hi), _lo(lo) {}

    GN_DEFAULT_COPY(WorldCoordinate);

    constexpr WorldCoordinate operator-() const {
        uint64_t lo = ~_lo + 1;
        return WorldCoordinate((int64_t) (~(uint64_t) _hi + (lo == 0 ? 1 : 0)), lo);
    }

    constexpr WorldCoordinate operator+(WorldCoordinate o) const {
        uint64_t lo = _lo + o._lo;
        return WorldCoordinate((int64_t) ((uint64_t) _hi + (uint64_t) o._hi + (lo < _lo ? 1 : 0)), lo);
    }

    constexpr WorldCoordinate operator-(WorldCoordinate o) const {
        uint64_t lo = _lo - o._lo;
        return WorldCoordinate((int64_t) ((uint64_t) _hi - (uint64_t) o._hi - (_lo < o._lo ? 1 : 0)), lo);
    }

    constexpr WorldCoordinate operator*(WorldCoordinate o) const {
        // Low 128 bits of the product; two's complement makes the same bits correct for signed values.
        uint64_t hi = mulHi(_lo, o._lo) + (uint64_t) _hi * o._lo + _lo * (uint64_t) o._hi;
        return WorldCoordinate((int64_t) hi, _lo * o._lo);
    }

    /// Truncating division (rounds toward zero), matching built-in integer semantics.
    constexpr WorldCoordinate operator/(WorldCoordinate o) const {
        WorldCoordinate rem;
        return divRem(o, rem);
    }

    constexpr WorldCoordinate operator%(WorldCoordinate o) const {
        WorldCoordinate rem;
        divRem(o, rem);
        return rem;
    }

    constexpr WorldCoordinate & operator+=(WorldCoordinate o) { return *this = *this + o; }
    constexpr WorldCoordinate & operator-=(WorldCoordinate o) { return *this = *this - o; }
    constexpr WorldCoordinate & operator*=(WorldCoordinate o) { return *this = *this * o; }
    constexpr WorldCoordinate & operator/=(WorldCoordinate o) { return *this = *this / o; }
    constexpr WorldCoordinate & operator%=(WorldCoordinate o) { return *this = *this % o; }

    // Two's-complement ordering: the signed high half decides, then the unsigned low half.
    constexpr bool operator==(WorldCoordinate o) const { return _hi == o._hi && _lo == o._lo; }
    constexpr bool operator!=(WorldCoordinate o) const { return !(*this == o); }
    constexpr bool operator<(WorldCoordinate o) const { return _hi < o._hi || (_hi == o._hi && _lo < o._lo); }
    constexpr bool operator>(WorldCoordinate o) const { return o < *this; }
    constexpr bool operator<=(WorldCoordinate o) const { return !(o < *this); }
    constexpr bool operator>=(WorldCoordinate o) const { return !(*this < o); }

    /// Raw halves of the 128-bit two's complement value; hi() carries the sign.
    ///@{
    constexpr int64_t  hi() const { return _hi; }
    constexpr uint64_t lo() const { return _lo; }
    ///@}

    static constexpr WorldCoordinate ZERO() { return WorldCoordinate(); }

private:
    int64_t  _hi = 0; ///< high half; holds the sign
    uint64_t _lo = 0; ///< low half

    // High 64 bits of the full 128-bit product of two unsigned 64-bit values, via 32-bit halves.
    static constexpr uint64_t mulHi(uint64_t a, uint64_t b) {
        uint64_t aL = a & 0xFFFFFFFFu, aH = a >> 32;
        uint64_t bL = b & 0xFFFFFFFFu, bH = b >> 32;
        uint64_t mid = ((aL * bL) >> 32) + ((aL * bH) & 0xFFFFFFFFu) + ((aH * bL) & 0xFFFFFFFFu);
        return aH * bH + ((aL * bH) >> 32) + ((aH * bL) >> 32) + (mid >> 32);
    }

    // Truncating division (rounds toward zero) that also produces the remainder, which takes the
    // dividend's sign — the same contract as built-in integers, so (a.divRem(b, r)) * b + r == a.
    // Division by zero is undefined, as with built-in integers.
    constexpr WorldCoordinate divRem(WorldCoordinate o, WorldCoordinate & rem) const {
        // Shift-subtract long division on the magnitudes; negating INT128_MIN yields its unsigned
        // magnitude bit pattern, so even that extreme divides correctly.
        WorldCoordinate n   = _hi < 0 ? -*this : *this;
        WorldCoordinate d   = o._hi < 0 ? -o : o;
        uint64_t        qHi = 0, qLo = 0, rHi = 0, rLo = 0;
        for (int i = 127; i >= 0; --i) {
            rHi = (rHi << 1) | (rLo >> 63);
            rLo = (rLo << 1) | ((i >= 64 ? ((uint64_t) n._hi >> (i - 64)) : (n._lo >> i)) & 1);
            if (rHi > (uint64_t) d._hi || (rHi == (uint64_t) d._hi && rLo >= d._lo)) {
                rHi -= (uint64_t) d._hi + (rLo < d._lo ? 1 : 0);
                rLo -= d._lo;
                if (i >= 64)
                    qHi |= uint64_t(1) << (i - 64);
                else
                    qLo |= uint64_t(1) << i;
            }
        }
        WorldCoordinate q((int64_t) qHi, qLo);
        WorldCoordinate r((int64_t) rHi, rLo);
        rem = _hi < 0 ? -r : r;
        return (_hi < 0) != (o._hi < 0) ? -q : q;
    }
};

/// Strongly-typed coordinate in a local frame, stored as a signed 64-bit integer of world units.
/// It represents local quantities — extents, offsets, camera-relative distances — typically
/// obtained by subtracting two absolute coordinates (see spatial::toLocal()). Conversion
/// to/from physical units lives on PhysicalScale; rebasing into a local frame first is what
/// keeps large-world coordinate precision out of floating point.
class LocalCoordinate {
public:
    constexpr LocalCoordinate() = default;

    /// Local coordinate of \p value world units.
    explicit constexpr LocalCoordinate(int64_t value): _value(value) {}

    GN_DEFAULT_COPY(LocalCoordinate);

    constexpr LocalCoordinate operator-() const { return LocalCoordinate(-_value); }

    constexpr LocalCoordinate operator+(LocalCoordinate o) const { return LocalCoordinate(_value + o._value); }
    constexpr LocalCoordinate operator-(LocalCoordinate o) const { return LocalCoordinate(_value - o._value); }
    constexpr LocalCoordinate operator*(LocalCoordinate o) const { return LocalCoordinate(_value * o._value); }
    constexpr LocalCoordinate operator/(LocalCoordinate o) const { return LocalCoordinate(_value / o._value); }
    constexpr LocalCoordinate operator%(LocalCoordinate o) const { return LocalCoordinate(_value % o._value); }

    constexpr LocalCoordinate & operator+=(LocalCoordinate o) { return *this = *this + o; }
    constexpr LocalCoordinate & operator-=(LocalCoordinate o) { return *this = *this - o; }
    constexpr LocalCoordinate & operator*=(LocalCoordinate o) { return *this = *this * o; }
    constexpr LocalCoordinate & operator/=(LocalCoordinate o) { return *this = *this / o; }
    constexpr LocalCoordinate & operator%=(LocalCoordinate o) { return *this = *this % o; }

    constexpr bool operator==(LocalCoordinate o) const { return _value == o._value; }
    constexpr bool operator!=(LocalCoordinate o) const { return _value != o._value; }
    constexpr bool operator<(LocalCoordinate o) const { return _value < o._value; }
    constexpr bool operator>(LocalCoordinate o) const { return _value > o._value; }
    constexpr bool operator<=(LocalCoordinate o) const { return _value <= o._value; }
    constexpr bool operator>=(LocalCoordinate o) const { return _value >= o._value; }

    /// Raw value in world units.
    constexpr int64_t value() const { return _value; }

    static constexpr LocalCoordinate ZERO() { return LocalCoordinate(0); }

private:
    int64_t _value = 0;
};

/// 2D spatial coordinate within the world.
using WorldVector2 = Vector2<WorldCoordinate>;

/// 3D spatial coordinate within the world.
using WorldVector3 = Vector3<WorldCoordinate>;

/// 4D spatial coordinate within the world.
using WorldVector4 = Vector4<WorldCoordinate>;

/// 2D local offset/extent, in world units.
using LocalVector2 = Vector2<LocalCoordinate>;

/// 3D local offset/extent, in world units.
using LocalVector3 = Vector3<LocalCoordinate>;

/// 4D local offset/extent, in world units.
using LocalVector4 = Vector4<LocalCoordinate>;

/// Physical scale of a world: one world unit equals 10^exponent meters. Restricting scales to
/// powers of ten keeps the ratio a single small integer and every conversion a pure multiply or
/// divide by an exactly-representable constant — e.g. millimeter scale is exponent -3.
///
/// All conversion between world units and physical units lives here, bound to the scale's
/// exponent. The to-physical direction runs in exact integer math — quotient and sub-unit
/// remainder against 10^|exponent| — touching floating point only at the last step; a result
/// beyond the target type's finite range saturates to +/-infinity. The from-physical direction
/// is inherently floating-point on the input side; it rounds to the nearest whole unit, halves
/// away from zero, and saturates to the int64 unit range on overflow (NaN maps to zero).
struct PhysicalScale {
    /// One world unit = 10^exponent meters. Must stay within ±19 so every conversion factor,
    /// including the two extra decimal digits of the centimeter variants, stays exact.
    int32_t exponent = 0;

    constexpr bool operator==(const PhysicalScale & o) const { return exponent == o.exponent; }
    constexpr bool operator!=(const PhysicalScale & o) const { return exponent != o.exponent; }

    /// Common scales: the physical size of one world unit.
    ///@{
    static constexpr PhysicalScale PICOMETER() { return {-12}; }
    static constexpr PhysicalScale NANOMETER() { return {-9}; }
    static constexpr PhysicalScale MICROMETER() { return {-6}; }
    static constexpr PhysicalScale MILLIMETER() { return {-3}; }
    static constexpr PhysicalScale CENTIMETER() { return {-2}; }
    static constexpr PhysicalScale DECIMETER() { return {-1}; }
    static constexpr PhysicalScale METER() { return {0}; }
    ///@}

    /// Local quantity to physical units: exact integer math up to the final floating-point
    /// rounding; saturates to +/-infinity when the result exceeds T's finite range.
    ///@{
    template<typename T = float>
    constexpr T toMeters(LocalCoordinate v) const {
        return physicalCast<T>(unitsToPhysical(v.value(), exponent));
    }
    template<typename T = float>
    constexpr T toCentimeters(LocalCoordinate v) const {
        return physicalCast<T>(unitsToPhysical(v.value(), exponent + 2));
    }
    glm::vec3 toMeters(const LocalVector3 & v) const { return {toMeters(v.x), toMeters(v.y), toMeters(v.z)}; }
    ///@}

    /// Absolute coordinate to physical units. Unlike the local variants this is approximate: a
    /// 128-bit coordinate generally exceeds a double's 53-bit mantissa, so the result is only
    /// good to double precision — prefer rebasing with spatial::toLocal() and converting the
    /// local offset when precision matters. Saturates to +/-infinity like the local path.
    ///@{
    template<typename T = float>
    constexpr T toMeters(WorldCoordinate v) const {
        return physicalCast<T>(wideUnitsToPhysical(v, exponent));
    }
    template<typename T = float>
    constexpr T toCentimeters(WorldCoordinate v) const {
        return physicalCast<T>(wideUnitsToPhysical(v, exponent + 2));
    }
    ///@}

    /// Physical units to a local quantity, rounded to the nearest whole unit (halves away from
    /// zero) at the very last step. Out-of-range inputs saturate to the int64 unit range and
    /// NaN maps to zero. Widen the result with spatial::toWorld() to anchor it on an absolute
    /// coordinate.
    ///@{
    template<typename T = float>
    constexpr LocalCoordinate fromMeters(T meters) const {
        return LocalCoordinate(physicalToUnits((double) meters, exponent));
    }
    template<typename T = float>
    constexpr LocalCoordinate fromCentimeters(T cm) const {
        return LocalCoordinate(physicalToUnits((double) cm, exponent + 2));
    }
    ///@}

private:
    static constexpr double TWO64 = 18446744073709551616.0; // 2^64

    // 10^p for p in [0, 19] — every power of ten that fits an unsigned 64-bit integer.
    static constexpr uint64_t pow10u(int p) {
        uint64_t r = 1;
        for (int i = 0; i < p; ++i) r *= 10;
        return r;
    }

    // 10^p in floating point. Exact through 10^22: every power of ten up to there is exactly
    // representable as a double, so each step of the loop rounds exactly. Larger powers pick up
    // ordinary rounding and eventually overflow to infinity, which the callers' saturation
    // handles — the loop stops there so an absurd exponent cannot spin it billions of times.
    static constexpr double pow10d(int p) {
        double r = 1.0;
        for (int i = 0; i < p && r < std::numeric_limits<double>::infinity(); ++i) r *= 10.0;
        return r;
    }

    // High 64 bits of the full 128-bit product of two unsigned 64-bit values, via 32-bit halves.
    // Mirrors WorldCoordinate's helper, kept local so scalar conversion stays self-contained.
    static constexpr uint64_t mulHi(uint64_t a, uint64_t b) {
        uint64_t aL = a & 0xFFFFFFFFu, aH = a >> 32;
        uint64_t bL = b & 0xFFFFFFFFu, bH = b >> 32;
        uint64_t mid = ((aL * bL) >> 32) + ((aL * bH) & 0xFFFFFFFFu) + ((aH * bL) & 0xFFFFFFFFu);
        return aH * bH + ((aL * bH) >> 32) + ((aH * bL) >> 32) + (mid >> 32);
    }

    // Round to nearest, halves away from zero, saturating to the int64 range: float/double
    // inputs carry representation error (0.3f is not exactly 0.3); truncation would turn a
    // value a hair below the exact result into an off-by-one unit. The range checks replace the
    // undefined out-of-range float-to-integer cast; the +/-0.5 bias cannot push an in-range
    // value out, because doubles this close to 2^63 are spaced 1024 apart. (std::llround is not
    // constexpr in C++20.)
    static constexpr int64_t roundToUnits(double v) {
        if (v != v) return 0;                                                       // NaN
        if (v >= 9223372036854775808.0) return std::numeric_limits<int64_t>::max(); // 2^63
        if (v < -9223372036854775808.0) return std::numeric_limits<int64_t>::min();
        return (int64_t) (v < 0.0 ? v - 0.5 : v + 0.5);
    }

    // Saturating cast from the double conversion result to the caller's floating type: an
    // out-of-range double-to-float conversion is undefined behavior, so map such results to
    // +/-infinity explicitly. NaN passes through.
    template<typename T>
    static constexpr T physicalCast(double d) {
        if (d > (double) std::numeric_limits<T>::max()) return std::numeric_limits<T>::infinity();
        if (d < -(double) std::numeric_limits<T>::max()) return -std::numeric_limits<T>::infinity();
        return (T) d;
    }

    // value * 10^exp evaluated exactly in integer space; floating point enters only at the very
    // last step. A non-negative exponent scales up through a 64x64 -> 128-bit product; a negative
    // one divides with plain 64-bit quotient and sub-unit remainder (the divisor is a power of
    // ten). Quotient and remainder sit at different scales, so they combine in floating point —
    // after the exact split, the quotient stays precise whenever it fits a double's 53-bit
    // mantissa, with only the sub-unit fraction rounded. The centimeter variants can push |exp|
    // to 21, past the largest power of ten a uint64 holds (10^19); the excess peels off into an
    // exactly-representable floating-point factor instead of overflowing the integer one.
    static constexpr double unitsToPhysical(int64_t value, int exp) {
        const bool     neg = value < 0;
        const uint64_t m   = neg ? 0 - (uint64_t) value : (uint64_t) value;
        double         d   = 0.0;
        if (exp >= 0) {
            const int      e = exp < 19 ? exp : 19;
            const uint64_t f = pow10u(e);
            d                = ((double) mulHi(m, f) * TWO64 + (double) (m * f)) * pow10d(exp - e);
        } else {
            const int      e = -exp < 19 ? -exp : 19;
            const uint64_t f = pow10u(e);
            d                = ((double) (m / f) + (double) (m % f) / (double) f) / pow10d(-exp - e);
        }
        return neg ? -d : d;
    }

    // 128-bit units to physical, going through the magnitude's two 64-bit halves — summing the
    // signed halves directly would collapse small negative values (the rounded low half cancels
    // the high half exactly). Approximate by nature: the magnitude generally exceeds a double's
    // mantissa.
    static constexpr double wideUnitsToPhysical(WorldCoordinate v, int exp) {
        const bool            neg = v.hi() < 0;
        const WorldCoordinate m   = neg ? -v : v; // INT128_MIN negates to itself; its bit pattern reads correctly as an unsigned magnitude
        const double          d   = (double) (uint64_t) m.hi() * TWO64 + (double) m.lo();
        const double          r   = exp >= 0 ? d * pow10d(exp) : d / pow10d(-exp);
        return neg ? -r : r;
    }

    // Physical value to whole world units: v / 10^exp, scaled by an exactly-representable power
    // of ten so the scaling rounds only once, then rounded to units at the very last step.
    static constexpr int64_t physicalToUnits(double v, int exp) { return roundToUnits(exp >= 0 ? v / pow10d(exp) : v * pow10d(-exp)); }
};

// Spatial orientation defined by a rotation primitive.
using Rotation = glm::quat;

/// Stateless spatial utilities: World <-> Local rebasing and integer-space rotation, for scalars
/// and vectors. Grouped as static methods so the rebasing and rotation rules live in one place.
struct spatial {
    /// Offset from coordinate \p from to coordinate \p to (i.e. to - from), narrowed to the local
    /// 64-bit range. The two coordinates must be within a signed 64-bit integer of each other
    /// (asserted) — rebasing into such a local frame is what keeps physical-unit conversion
    /// (see PhysicalScale) precise.
    ///@{
    static LocalCoordinate toLocal(WorldCoordinate from, WorldCoordinate to) {
        WorldCoordinate d = to - from;
        // Locality contract: the delta fits a signed 64-bit integer iff the high half is pure
        // sign extension of the low half.
        GN_ASSERT(d.hi() == ((int64_t) d.lo() < 0 ? -1 : 0));
        return LocalCoordinate((int64_t) d.lo());
    }
    static LocalVector2 toLocal(const WorldVector2 & from, const WorldVector2 & to) { return {toLocal(from.x, to.x), toLocal(from.y, to.y)}; }
    static LocalVector3 toLocal(const WorldVector3 & from, const WorldVector3 & to) {
        return {toLocal(from.x, to.x), toLocal(from.y, to.y), toLocal(from.z, to.z)};
    }
    ///@}

    /// Widen a local offset to an absolute coordinate anchored at the origin. Lossless.
    ///@{
    static WorldCoordinate toWorld(LocalCoordinate offset) { return WorldCoordinate(offset.value() < 0 ? -1 : 0, (uint64_t) offset.value()); }
    static WorldVector2    toWorld(const LocalVector2 & offset) { return {toWorld(offset.x), toWorld(offset.y)}; }
    static WorldVector3    toWorld(const LocalVector3 & offset) { return {toWorld(offset.x), toWorld(offset.y), toWorld(offset.z)}; }
    ///@}

    /// Anchor a local offset back onto an absolute coordinate: origin + offset. Lossless.
    ///@{
    static WorldCoordinate toWorld(WorldCoordinate origin, LocalCoordinate offset) { return origin + toWorld(offset); }
    static WorldVector2 toWorld(const WorldVector2 & origin, const LocalVector2 & offset) { return {toWorld(origin.x, offset.x), toWorld(origin.y, offset.y)}; }
    static WorldVector3 toWorld(const WorldVector3 & origin, const LocalVector3 & offset) {
        return {toWorld(origin.x, offset.x), toWorld(origin.y, offset.y), toWorld(origin.z, offset.z)};
    }
    ///@}

    /// Rotate a world-coordinate vector without leaving integer space: coordinates never pass
    /// through a double, whose 53-bit mantissa would corrupt large positions. The rotation
    /// coefficients (derived from the float quaternion) are quantized to 32.32 fixed point — well
    /// beyond the quaternion's own 24-bit float precision — and each result component is
    /// accumulated with WorldCoordinate's 128-bit arithmetic. The only losses are that coefficient
    /// quantization and the final round to whole units.
    static WorldVector3 rotatedBy(const Rotation & q, const WorldVector3 & v) {
        const glm::dmat3 m = glm::mat3_cast(glm::dquat(q)); // column-major: m[column][row]
        auto row = [&](double c0, double c1, double c2) { return fxToUnits(fxCoefficient(c0) * v.x + fxCoefficient(c1) * v.y + fxCoefficient(c2) * v.z); };
        return {row(m[0][0], m[1][0], m[2][0]), row(m[0][1], m[1][1], m[2][1]), row(m[0][2], m[1][2], m[2][2])};
    }

    /// 2D counterpart: rotate \p v counterclockwise by \p radians, in integer space.
    static WorldVector2 rotatedBy(double radians, const WorldVector2 & v) {
        const double c = std::cos(radians), s = std::sin(radians);
        auto         row = [&](double c0, double c1) { return fxToUnits(fxCoefficient(c0) * v.x + fxCoefficient(c1) * v.y); };
        return {row(c, -s), row(s, c)};
    }

    /// Local-vector counterparts: the same integer-space rotation, widened through absolute
    /// coordinates. The rotated components must still fit the local 64-bit range (asserted by
    /// the narrowing rebase).
    ///@{
    static LocalVector3 rotatedBy(const Rotation & q, const LocalVector3 & v) {
        return toLocal(WorldVector3(WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()), rotatedBy(q, toWorld(v)));
    }
    static LocalVector2 rotatedBy(double radians, const LocalVector2 & v) {
        return toLocal(WorldVector2(WorldCoordinate::ZERO(), WorldCoordinate::ZERO()), rotatedBy(radians, toWorld(v)));
    }
    ///@}

private:
    // 32.32 fixed-point scale used to carry rotation coefficients into integer space.
    static constexpr int64_t kFxOne = int64_t(1) << 32;

    // Quantize a rotation coefficient (always within [-1, 1], so the scaled value fits an int64)
    // to 32.32 fixed point.
    static WorldCoordinate fxCoefficient(double c) { return toWorld(LocalCoordinate((int64_t) std::llround(c * (double) kFxOne))); }

    // Round a fixed-point sum back to whole units, halves away from zero (sign-directed half bias
    // before the truncating division).
    static WorldCoordinate fxToUnits(WorldCoordinate sum) {
        const WorldCoordinate one  = toWorld(LocalCoordinate(kFxOne));
        const WorldCoordinate half = toWorld(LocalCoordinate(kFxOne / 2));
        return (sum + (sum < WorldCoordinate::ZERO() ? -half : half)) / one;
    }
};

} // namespace GN::e2

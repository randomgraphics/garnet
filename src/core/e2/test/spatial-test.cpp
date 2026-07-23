// CPU-only unit tests for the e2 coordinate system: 128-bit absolute-coordinate arithmetic
// (WorldCoordinate), 64-bit local frames (LocalCoordinate), rebasing and integer-space rotation
// (spatial), and conversions to/from physical units with a per-world power-of-ten scale.

#include <catch2/catch_test_macros.hpp>

#include <garnet/GNengine2.h>

#include <glm/gtc/constants.hpp>

#include <cmath>
#include <cstdint>
#include <limits>

using namespace GN;
using namespace GN::e2;

namespace {
// Test shorthand: the absolute coordinate \p v units from the origin.
WorldCoordinate W(int64_t v) { return spatial::toWorld(LocalCoordinate(v)); }
} // namespace

TEST_CASE("E2 spatial: meter conversions round-trip at meter scale") {
    constexpr PhysicalScale scale = PhysicalScale::METER();
    CHECK(scale.toMeters(LocalCoordinate(5)) == 5.0f);
    CHECK(scale.fromMeters(5.0f) == LocalCoordinate(5));
    CHECK(scale.fromMeters(scale.toMeters(LocalCoordinate(7))) == LocalCoordinate(7));
    CHECK(scale.toMeters(LocalCoordinate(-3)) == -3.0f);
    CHECK(scale.fromMeters(-3.0f) == LocalCoordinate(-3));

    // rounding is to nearest, halves away from zero
    CHECK(scale.fromMeters(2.5) == LocalCoordinate(3));
    CHECK(scale.fromMeters(-2.5) == LocalCoordinate(-3));
    CHECK(scale.fromMeters(0.4) == LocalCoordinate(0));
    CHECK(scale.fromMeters(-0.4) == LocalCoordinate(0));
}

TEST_CASE("E2 spatial: conversions honor a non-trivial scale exactly") {
    constexpr PhysicalScale mm = PhysicalScale::MILLIMETER();
    CHECK(mm.toMeters(LocalCoordinate(1500)) == 1.5f);
    // The power-of-ten scale keeps this exact: 2.0 * 10^3 is 2000 with no drift, unlike the old
    // double meters-per-unit where 2.0 / 0.001 landed a hair below 2000.
    CHECK(mm.fromMeters(2.0f) == LocalCoordinate(2000));
    CHECK(mm.toCentimeters(LocalCoordinate(250)) == 25.0f);
    CHECK(mm.fromCentimeters(30.0f) == LocalCoordinate(300));

    // The sub-unit remainder keeps its sign through the last floating-point step.
    constexpr PhysicalScale dm = PhysicalScale::DECIMETER();
    CHECK(dm.toMeters(LocalCoordinate(15)) == 1.5f);
    CHECK(dm.toMeters(LocalCoordinate(-15)) == -1.5f);

    // A positive exponent scales up through the exact 128-bit product path.
    constexpr PhysicalScale km {3}; // one world unit = 1 kilometer
    CHECK(km.toMeters(LocalCoordinate(2)) == 2000.0f);
    CHECK(km.fromMeters(5000.0f) == LocalCoordinate(5));

    // The quotient stays exact in integer space even at the edge of the int64 range: the
    // nearest double to INT64_MAX is 2^63.
    CHECK(PhysicalScale::METER().toMeters<double>(LocalCoordinate(INT64_MAX)) == 9223372036854775808.0);
}

TEST_CASE("E2 spatial: centimeter factor is 100") {
    constexpr PhysicalScale scale = PhysicalScale::METER();
    CHECK(scale.toCentimeters(LocalCoordinate(2)) == 200.0f);
    CHECK(scale.fromCentimeters(500.0f) == LocalCoordinate(5));

    // The centimeter shift can push the effective exponent to 21, past the largest power of ten
    // a uint64 holds; the excess factor peels into exact floating point instead of overflowing.
    constexpr PhysicalScale top {19};
    CHECK(top.toCentimeters<double>(LocalCoordinate(3)) == 3e21);
    CHECK(top.fromCentimeters(3e21) == LocalCoordinate(3));
}

TEST_CASE("E2 spatial: absolute coordinates convert approximately") {
    constexpr PhysicalScale scale = PhysicalScale::METER();
    // Small magnitudes stay exact; the negative path must not lose the value to cancellation
    // between the two 64-bit halves.
    CHECK(scale.toMeters(spatial::toWorld(LocalCoordinate(5))) == 5.0f);
    CHECK(scale.toMeters(spatial::toWorld(LocalCoordinate(-5))) == -5.0f);
    CHECK(scale.toCentimeters(spatial::toWorld(LocalCoordinate(2))) == 200.0f);
    // Beyond the local 64-bit range: 2^64 units.
    CHECK(scale.toMeters<double>(WorldCoordinate(1, 0)) == 18446744073709551616.0);
    CHECK(PhysicalScale::MILLIMETER().toMeters<double>(WorldCoordinate(1, 0)) == 18446744073709551.616);
}

TEST_CASE("E2 spatial: conversions saturate instead of overflowing") {
    // From-physical results beyond the int64 unit range clamp to its extremes; NaN maps to zero.
    constexpr PhysicalScale nm = PhysicalScale::NANOMETER();
    CHECK(nm.fromMeters(1e30) == LocalCoordinate(INT64_MAX));
    CHECK(nm.fromMeters(-1e30) == LocalCoordinate(INT64_MIN));
    CHECK(nm.fromMeters(std::numeric_limits<float>::infinity()) == LocalCoordinate(INT64_MAX));
    CHECK(nm.fromMeters(std::numeric_limits<double>::quiet_NaN()) == LocalCoordinate(0));

    // To-physical results beyond the target float type saturate to +/-infinity rather than
    // hitting the undefined out-of-range double-to-float conversion.
    constexpr PhysicalScale top {19};
    CHECK(std::isinf(top.toCentimeters(LocalCoordinate(INT64_MAX))));
    CHECK(top.toCentimeters(LocalCoordinate(INT64_MAX)) > 0.f);
    CHECK(top.toCentimeters(LocalCoordinate(INT64_MIN)) < 0.f);
    // The same value fits a double just fine.
    CHECK(!std::isinf(top.toCentimeters<double>(LocalCoordinate(INT64_MAX))));
    // A full-range absolute coordinate saturates a float too.
    CHECK(std::isinf(top.toMeters(WorldCoordinate(INT64_MIN, 0))));
    CHECK(top.toMeters(WorldCoordinate(INT64_MIN, 0)) < 0.f);
}

TEST_CASE("E2 spatial: LocalCoordinate arithmetic matches built-in 64-bit integers") {
    CHECK(LocalCoordinate(3) + LocalCoordinate(4) == LocalCoordinate(7));
    CHECK(LocalCoordinate(3) - LocalCoordinate(4) == LocalCoordinate(-1));
    CHECK(LocalCoordinate(3) * LocalCoordinate(4) == LocalCoordinate(12));
    CHECK(LocalCoordinate(-7) / LocalCoordinate(2) == LocalCoordinate(-3));
    CHECK(LocalCoordinate(-7) % LocalCoordinate(2) == LocalCoordinate(-1));
    CHECK(-LocalCoordinate(5) == LocalCoordinate(-5));
    CHECK(LocalCoordinate(-1) < LocalCoordinate::ZERO());

    LocalCoordinate acc(10);
    acc += LocalCoordinate(5);
    acc *= LocalCoordinate(2);
    acc -= LocalCoordinate(6);
    acc /= LocalCoordinate(4);
    CHECK(acc == LocalCoordinate(6));
}

TEST_CASE("E2 spatial: toLocal/toWorld rebase between absolute coordinates and local frames") {
    CHECK(spatial::toLocal(W(3), W(10)) == LocalCoordinate(7));
    CHECK(spatial::toLocal(W(10), W(3)) == LocalCoordinate(-7));

    // The 128-bit magnitude cancels in the subtraction; only the local delta remains.
    const WorldCoordinate far  = W(INT64_MAX) + W(INT64_MAX);
    const WorldCoordinate near = far - W(42);
    CHECK(spatial::toLocal(near, far) == LocalCoordinate(42));
    CHECK(spatial::toLocal(far, near) == LocalCoordinate(-42));

    // toWorld anchors the offset back losslessly: a perfect round trip.
    CHECK(spatial::toWorld(LocalCoordinate(-42)) == W(-42));
    CHECK(spatial::toWorld(near, spatial::toLocal(near, far)) == far);

    // Component-wise rebase of coordinate vectors, both directions.
    const WorldVector3 a(far, W(5), W(-5));
    const WorldVector3 b(near, W(7), W(-5));
    const LocalVector3 d = spatial::toLocal(a, b);
    CHECK(d.x == LocalCoordinate(-42));
    CHECK(d.y == LocalCoordinate(2));
    CHECK(d.z == LocalCoordinate(0));
    const WorldVector3 r = spatial::toWorld(a, d);
    CHECK(r.x == b.x);
    CHECK(r.y == b.y);
    CHECK(r.z == b.z);
}

TEST_CASE("E2 spatial: arithmetic carries across the 64-bit boundary") {
    const WorldCoordinate max64 = W(INT64_MAX);
    const WorldCoordinate two   = W(2);

    // 2 * INT64_MAX overflows int64 storage; the 128-bit representation must hold it exactly.
    const WorldCoordinate big = max64 + max64;
    CHECK(big - max64 == max64);
    CHECK(big / two == max64);
    CHECK(max64 * two == big);
    CHECK(big + two - two == big);

    // negation and zero
    CHECK(-big + big == WorldCoordinate::ZERO());
    CHECK(-(-big) == big);

    // ordering across the boundary and around zero
    CHECK(max64 < big);
    CHECK(-big < -max64);
    CHECK(W(-1) < W(0));
    CHECK(big > W(0));

    // compound assignment goes through the same 128-bit paths
    WorldCoordinate acc = max64;
    acc += max64;
    CHECK(acc == big);
    acc /= two;
    CHECK(acc == max64);
}

TEST_CASE("E2 spatial: division and remainder match built-in integer semantics") {
    CHECK(W(7) / W(2) == W(3));
    CHECK(W(-7) / W(2) == W(-3));
    CHECK(W(7) / W(-2) == W(-3));
    CHECK(W(-7) / W(-2) == W(3));

    // the remainder takes the dividend's sign
    CHECK(W(7) % W(3) == W(1));
    CHECK(W(-7) % W(3) == W(-1));
    CHECK(W(7) % W(-3) == W(1));
    CHECK(W(-7) % W(-3) == W(-1));

    // (a / b) * b + a % b == a, including beyond the 64-bit boundary
    const WorldCoordinate a = W(INT64_MAX) + W(12345);
    const WorldCoordinate b = W(1000);
    CHECK((a / b) * b + a % b == a);
}

TEST_CASE("E2 spatial: 3D rotation stays in integer space") {
    const WorldCoordinate big = W(INT64_MAX) + W(1);

    // A half turn about Z has exact float quaternion components (w=0, z=1), so even coordinates
    // beyond double's 53-bit mantissa must come back exact.
    const auto h = spatial::rotatedBy(Rotation(0.f, 0.f, 0.f, 1.f), WorldVector3(big, W(3), W(-7)));
    CHECK(h.x == -big);
    CHECK(h.y == W(-3));
    CHECK(h.z == W(-7));

    // The identity rotation is exact everywhere.
    const auto i = spatial::rotatedBy(Rotation(1.f, 0.f, 0.f, 0.f), WorldVector3(big, W(0), -big));
    CHECK(i.x == big);
    CHECK(i.y == W(0));
    CHECK(i.z == -big);

    // A quarter turn about Z from an inexact float quaternion still lands on the lattice point.
    const auto q = spatial::rotatedBy(glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f)), WorldVector3(W(100), W(0), W(5)));
    CHECK(q.x == W(0));
    CHECK(q.y == W(100));
    CHECK(q.z == W(5));
}

TEST_CASE("E2 spatial: 2D rotation stays in integer space") {
    // Quarter turn: cos quantizes to exactly 0 and sin to exactly 1, so huge coordinates survive.
    const WorldCoordinate big = W(int64_t(1) << 60);
    const auto            q   = spatial::rotatedBy(glm::half_pi<double>(), WorldVector2(big, W(0)));
    CHECK(q.x == W(0));
    CHECK(q.y == big);

    // Eighth turn rounds each component to the nearest whole unit (100 / sqrt(2) = 70.7...).
    const auto e = spatial::rotatedBy(glm::quarter_pi<double>(), WorldVector2(W(100), W(0)));
    CHECK(e.x == W(71));
    CHECK(e.y == W(71));

    // Clockwise (negative) angle mirrors the Y component.
    const auto c = spatial::rotatedBy(-glm::quarter_pi<double>(), WorldVector2(W(100), W(0)));
    CHECK(c.x == W(71));
    CHECK(c.y == W(-71));
}

TEST_CASE("E2 spatial: local vectors rotate with the same integer-space rules") {
    // Half turn about Z with exact float components: local extents flip sign exactly.
    const auto h = spatial::rotatedBy(Rotation(0.f, 0.f, 0.f, 1.f), LocalVector3(LocalCoordinate(INT64_MAX), LocalCoordinate(3), LocalCoordinate(-7)));
    CHECK(h.x == LocalCoordinate(-INT64_MAX));
    CHECK(h.y == LocalCoordinate(-3));
    CHECK(h.z == LocalCoordinate(-7));

    // Quarter turn about Z lands the local +X offset on +Y.
    const auto q = spatial::rotatedBy(glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f)),
                                      LocalVector3(LocalCoordinate(100), LocalCoordinate(0), LocalCoordinate(5)));
    CHECK(q.x == LocalCoordinate(0));
    CHECK(q.y == LocalCoordinate(100));
    CHECK(q.z == LocalCoordinate(5));

    // 2D: eighth turn rounds to the nearest whole unit.
    const auto e = spatial::rotatedBy(glm::quarter_pi<double>(), LocalVector2(LocalCoordinate(100), LocalCoordinate(0)));
    CHECK(e.x == LocalCoordinate(71));
    CHECK(e.y == LocalCoordinate(71));
}

// CPU-only unit tests for the e2 spatial length type: conversions between world units and
// physical units with a per-world runtime scale.

#include <catch2/catch_test_macros.hpp>

#include <garnet/GNengine2.h>

using namespace GN;
using namespace GN::e2;

TEST_CASE("E2 spatial: meter conversions round-trip at scale 1.0") {
    constexpr double scale = 1.0;
    CHECK(WorldLength(5).toMeters(scale) == 5.0f);
    CHECK(WorldLength::fromMeters(5.0f, scale).raw() == 5);
    CHECK(WorldLength::fromMeters(WorldLength(7).toMeters(scale), scale).raw() == 7);
    CHECK(WorldLength(-3).toMeters(scale) == -3.0f);
    CHECK(WorldLength::fromMeters(-3.0f, scale).raw() == -3);
}

TEST_CASE("E2 spatial: conversions honor a non-trivial scale") {
    constexpr double mm = 0.001; // one world unit = 1 millimeter
    CHECK(WorldLength(1500).toMeters(mm) == 1.5f);
    // 2.0 / 0.001 lands a hair below 2000 in double math; rounding must absorb it.
    CHECK(WorldLength::fromMeters(2.0f, mm).raw() == 2000);
    CHECK(WorldLength(250).toCentimeters(mm) == 25.0f);
    CHECK(WorldLength::fromCentimeters(30.0f, mm).raw() == 300);
}

TEST_CASE("E2 spatial: centimeter factor is 100") {
    constexpr double scale = 1.0;
    CHECK(WorldLength(2).toCentimeters(scale) == 200.0f);
    CHECK(WorldLength::fromCentimeters(500.0f, scale).raw() == 5);
}

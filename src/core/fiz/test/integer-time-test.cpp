#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <garnet/GNfiz.h>

#include <chrono>

using namespace GN::fiz;

TEST_CASE("fiz UnitOfTime preserves exact integer nanosecond timeline without drift", "[fiz][time]") {
    SECTION("Zero drift integer accumulation vs float accumulation") {
        // In standard 32-bit float accumulators, dt = 1/60 accumulates significant rounding error
        float       floatAccumulator = 0.0f;
        const float floatDt          = 1.0f / 60.0f;

        UnitOfTime       intAccumulator(0);
        const UnitOfTime intDt(16'666'667); // 16.666667 ms

        // Accumulate 3600 frames (1 minute of 60 Hz simulation)
        for (int i = 0; i < 3600; ++i) {
            floatAccumulator += floatDt;
            intAccumulator += intDt;
        }

        // Exact expected integer nanoseconds: 3600 * 16666667 = 60000001200 ns
        int64_t expectedNs = 3600LL * 16'666'667LL;
        CHECK(intAccumulator.count() == expectedNs);

        // Exact cancellation with negative time steps
        for (int i = 0; i < 3600; ++i) { intAccumulator -= intDt; }
        CHECK(intAccumulator.count() == 0);
    }

    SECTION("Conversion precision and helper chronos") {
        UnitOfTime oneSecond = std::chrono::seconds(1);
        CHECK(oneSecond.count() == 1'000'000'000LL);

        UnitOfTime oneMilli = std::chrono::milliseconds(1);
        CHECK(oneMilli.count() == 1'000'000LL);

        UnitOfTime oneMicro = std::chrono::microseconds(1);
        CHECK(oneMicro.count() == 1'000LL);

        UnitOfTime sum = oneSecond + oneMilli + oneMicro;
        CHECK(sum.count() == 1'001'001'000LL);
    }
}

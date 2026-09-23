#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <garnet/GNfiz.h>

using namespace GN::fiz;

TEST_CASE("fiz operates unit-agnostic physics across macro, human, and micro scales", "[fiz][scale]") {
    SECTION("Planetary / kilometer macro scale (1 unit = 1000 meters)") {
        // At this scale, g = 0.00981 km/s^2
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, -0.00981f, 0.0f};
        engineDesc.numWorkerThreads = 1;
        auto engine                 = SolidEngine::create(engineDesc);
        REQUIRE(engine);

        // Body with radius 10 km dropped from altitude 1000 km
        SolidDesc planetoid;
        planetoid.hull                  = Hull::createSphere(10.0f);
        planetoid.motionType            = MotionType::DYNAMIC;
        planetoid.layer                 = CollisionLayer::MOVING;
        planetoid.transform.position    = {0.0f, 1000.0f, 0.0f};
        planetoid.temper.linearDamping  = 0.0f;
        planetoid.temper.angularDamping = 0.0f;
        auto body                       = engine->createSolid(planetoid);
        REQUIRE(body);

        // Step 10 seconds (10 steps of 1 second)
        UnitOfTime oneSec(1'000'000'000);
        for (int i = 0; i < 10; ++i) { engine->step(oneSec); }

        // Analytical drop: y = y0 - 0.5 * g * t^2 = 1000 - 0.5 * 0.00981 * 100 = 1000 - 0.4905 = 999.5095
        Vector3 finalPos = body->position();
        CHECK(finalPos.y == Catch::Approx(999.5095f).margin(0.05f));
    }

    SECTION("Centimeter scale (1 unit = 1 centimeter)") {
        // At this scale, g = 981 cm/s^2
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, -981.0f, 0.0f};
        engineDesc.numWorkerThreads = 1;
        auto engine                 = SolidEngine::create(engineDesc);
        REQUIRE(engine);

        // Small pebble dropped from 100 cm
        SolidDesc pebble;
        pebble.hull                  = Hull::createSphere(1.0f);
        pebble.motionType            = MotionType::DYNAMIC;
        pebble.layer                 = CollisionLayer::MOVING;
        pebble.transform.position    = {0.0f, 100.0f, 0.0f};
        pebble.temper.linearDamping  = 0.0f;
        pebble.temper.angularDamping = 0.0f;
        auto body                    = engine->createSolid(pebble);
        REQUIRE(body);

        // Step 0.2 seconds (12 steps of 16.666 ms = ~0.2 s)
        UnitOfTime dt(16'666'667);
        for (int i = 0; i < 12; ++i) { engine->step(dt); }

        // Dropped distance: ~ 0.5 * 981 * (0.2)^2 = 19.62 cm -> y ~ 80.38 cm
        Vector3 finalPos = body->position();
        CHECK(finalPos.y < 100.0f);
        CHECK(finalPos.y == Catch::Approx(80.38f).margin(2.0f));
    }
}

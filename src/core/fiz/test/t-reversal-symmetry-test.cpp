#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <garnet/GNfiz.h>

using namespace GN::fiz;

TEST_CASE("fiz conservative motion exhibits time-reversal symmetry", "[fiz][symmetry][reversal]") {
    SECTION("Ballistic free flight in zero gravity") {
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, 0.0f, 0.0f};
        engineDesc.numWorkerThreads = 1;
        auto engine                 = SolidEngine::create(engineDesc);
        REQUIRE(engine);

        SolidDesc desc;
        desc.hull                  = Hull::createSphere(0.5f);
        desc.motionType            = MotionType::DYNAMIC;
        desc.layer                 = CollisionLayer::MOVING;
        desc.transform.position    = {2.0f, 3.0f, 4.0f};
        desc.temper.linearDamping  = 0.0f;
        desc.temper.angularDamping = 0.0f;
        auto body                  = engine->createSolid(desc);
        REQUIRE(body);

        body->setLinearVelocity({10.0f, -5.0f, 2.5f});
        const Vector3 initialPos = body->position();
        const Vector3 initialVel = body->linearVelocity();

        // Advance forward by 2 seconds (120 steps of 16.666 ms)
        UnitOfTime dt(16'666'667);
        for (int i = 0; i < 120; ++i) { engine->step(dt); }

        Vector3 forwardPos = body->position();
        CHECK(forwardPos.x > initialPos.x);

        // Step backward in time by the exact same negative durations (-120 steps)
        UnitOfTime negDt(-16'666'667);
        for (int i = 0; i < 120; ++i) { engine->step(negDt); }

        Vector3 finalPos = body->position();
        Vector3 finalVel = body->linearVelocity();

        // Body must return to initial state
        CHECK(finalPos.x == Catch::Approx(initialPos.x).margin(0.05f));
        CHECK(finalPos.y == Catch::Approx(initialPos.y).margin(0.05f));
        CHECK(finalPos.z == Catch::Approx(initialPos.z).margin(0.05f));
        CHECK(finalVel.x == Catch::Approx(initialVel.x).margin(0.05f));
        CHECK(finalVel.y == Catch::Approx(initialVel.y).margin(0.05f));
        CHECK(finalVel.z == Catch::Approx(initialVel.z).margin(0.05f));
    }

    SECTION("Gravitational freefall and reversal") {
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, -9.81f, 0.0f};
        engineDesc.numWorkerThreads = 1;
        auto engine                 = SolidEngine::create(engineDesc);
        REQUIRE(engine);

        SolidDesc desc;
        desc.hull                  = Hull::createSphere(0.5f);
        desc.motionType            = MotionType::DYNAMIC;
        desc.layer                 = CollisionLayer::MOVING;
        desc.transform.position    = {0.0f, 50.0f, 0.0f};
        desc.temper.linearDamping  = 0.0f;
        desc.temper.angularDamping = 0.0f;
        auto body                  = engine->createSolid(desc);
        REQUIRE(body);

        const Vector3 initialPos = body->position();

        // Drop for 1 second (60 steps)
        UnitOfTime dt(16'666'667);
        for (int i = 0; i < 60; ++i) { engine->step(dt); }

        Vector3 fallenPos = body->position();
        CHECK(fallenPos.y < initialPos.y);

        // Step backward for 1 second (-60 steps)
        UnitOfTime negDt(-16'666'667);
        for (int i = 0; i < 60; ++i) { engine->step(negDt); }

        Vector3 reversedPos = body->position();
        Vector3 reversedVel = body->linearVelocity();

        CHECK(reversedPos.y == Catch::Approx(initialPos.y).margin(0.1f));
        CHECK(reversedVel.y == Catch::Approx(0.0f).margin(0.1f));
    }
}

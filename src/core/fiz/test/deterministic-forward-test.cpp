#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <garnet/GNfiz.h>

#include <cstring>
#include <vector>

using GN::AutoRef;
using namespace GN::fiz;

TEST_CASE("fiz forward simulation is 100 percent bit-identical across repeated runs", "[fiz][deterministic]") {
    struct BodyState {
        Vector3    pos;
        Quaternion rot;
        Vector3    linVel;
        Vector3    angVel;
    };

    auto runSimulation = [](int workerThreads) -> std::vector<BodyState> {
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, -9.81f, 0.0f};
        engineDesc.numWorkerThreads = workerThreads;
        auto engine                 = SolidEngine::create(engineDesc);

        // Static floor
        SolidDesc floorDesc;
        floorDesc.hull               = Hull::createBox({20.0f, 1.0f, 20.0f});
        floorDesc.motionType         = MotionType::STATIC;
        floorDesc.layer              = CollisionLayer::NON_MOVING;
        floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
        engine->createSolid(floorDesc);

        // Stack 16 dynamic bodies with varied shapes and slight offsets to generate chaotic collisions
        std::vector<AutoRef<Solid>> solids;
        auto                        boxHull    = Hull::createBox({0.5f, 0.5f, 0.5f});
        auto                        sphereHull = Hull::createSphere(0.5f);

        for (int i = 0; i < 16; ++i) {
            SolidDesc desc;
            desc.hull               = (i % 2 == 0) ? boxHull : sphereHull;
            desc.motionType         = MotionType::DYNAMIC;
            desc.layer              = CollisionLayer::MOVING;
            desc.transform.position = {
                ((i % 4) - 1.5f) * 1.1f + 0.05f * static_cast<float>(i),
                2.0f + static_cast<float>(i / 4) * 1.5f,
                (((i / 2) % 4) - 1.5f) * 1.1f,
            };
            desc.temper.restitution = 0.3f;
            desc.temper.friction    = 0.5f;
            desc.entityId           = i + 1;
            solids.push_back(engine->createSolid(desc));
        }

        // Simulate 60 steps of 16.666 ms
        UnitOfTime dt(16'666'667);
        for (int s = 0; s < 60; ++s) { engine->step(dt); }

        std::vector<BodyState> states;
        states.reserve(solids.size());
        for (const auto & solid : solids) { states.push_back({solid->position(), solid->orientation(), solid->linearVelocity(), solid->angularVelocity()}); }
        return states;
    };

    // 1. Run baseline simulation
    const auto baseline = runSimulation(1);
    REQUIRE(baseline.size() == 16);

    // 2. Run 10 repeated simulations under identical conditions and assert bit-identical equality
    for (int run = 0; run < 10; ++run) {
        const auto current = runSimulation(1);
        REQUIRE(current.size() == baseline.size());

        for (size_t i = 0; i < baseline.size(); ++i) {
            // Check bit-exact identity for positions
            CHECK(std::memcmp(&baseline[i].pos, &current[i].pos, sizeof(Vector3)) == 0);
            // Check bit-exact identity for orientations
            CHECK(std::memcmp(&baseline[i].rot, &current[i].rot, sizeof(Quaternion)) == 0);
            // Check bit-exact identity for velocities
            CHECK(std::memcmp(&baseline[i].linVel, &current[i].linVel, sizeof(Vector3)) == 0);
            CHECK(std::memcmp(&baseline[i].angVel, &current[i].angVel, sizeof(Vector3)) == 0);
        }
    }
}

TEST_CASE("fiz multi-threaded simulation produces consistent macroscopic trajectories", "[fiz][deterministic][threads]") {
    auto runSimWithThreads = [](int threads) -> Vector3 {
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, -9.81f, 0.0f};
        engineDesc.numWorkerThreads = threads;
        auto engine                 = SolidEngine::create(engineDesc);

        SolidDesc floorDesc;
        floorDesc.hull               = Hull::createBox({20.0f, 1.0f, 20.0f});
        floorDesc.motionType         = MotionType::STATIC;
        floorDesc.layer              = CollisionLayer::NON_MOVING;
        floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
        engine->createSolid(floorDesc);

        SolidDesc sphereDesc;
        sphereDesc.hull               = Hull::createSphere(1.0f);
        sphereDesc.motionType         = MotionType::DYNAMIC;
        sphereDesc.layer              = CollisionLayer::MOVING;
        sphereDesc.transform.position = {0.0f, 10.0f, 0.0f};
        sphereDesc.temper.restitution = 0.5f;
        auto sphere                   = engine->createSolid(sphereDesc);

        UnitOfTime dt(16'666'667);
        for (int i = 0; i < 60; ++i) { engine->step(dt); }
        return sphere->position();
    };

    auto pos1 = runSimWithThreads(1);
    auto pos2 = runSimWithThreads(2);
    auto pos4 = runSimWithThreads(4);

    CHECK(pos2.y == Catch::Approx(pos1.y).margin(0.01f));
    CHECK(pos4.y == Catch::Approx(pos1.y).margin(0.01f));
}

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <garnet/GNfiz.h>

#include <vector>

using GN::AutoRef;
using namespace GN::fiz;

TEST_CASE("fiz solid collision mechanics: stacking, restitution, and CCD", "[fiz][collision]") {
    SECTION("Vertical box stack stability") {
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, -9.81f, 0.0f};
        engineDesc.numWorkerThreads = 1;
        auto engine                 = SolidEngine::create(engineDesc);
        REQUIRE(engine);

        // Floor at y = -1 (top at y = 0)
        SolidDesc floorDesc;
        floorDesc.hull               = Hull::createBox({20.0f, 1.0f, 20.0f});
        floorDesc.motionType         = MotionType::STATIC;
        floorDesc.layer              = CollisionLayer::NON_MOVING;
        floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
        floorDesc.temper.friction    = 0.8f;
        engine->createSolid(floorDesc);

        // Stack 5 boxes vertically (each 1x1x1, half-extents 0.5)
        auto                        boxHull = Hull::createBox({0.5f, 0.5f, 0.5f});
        std::vector<AutoRef<Solid>> stack;

        for (int i = 0; i < 5; ++i) {
            SolidDesc desc;
            desc.hull                  = boxHull;
            desc.motionType            = MotionType::DYNAMIC;
            desc.layer                 = CollisionLayer::MOVING;
            desc.transform.position    = {0.0f, 0.5f + static_cast<float>(i) * 1.02f, 0.0f};
            desc.temper.restitution    = 0.0f;
            desc.temper.friction       = 0.8f;
            desc.temper.linearDamping  = 0.05f;
            desc.temper.angularDamping = 0.05f;
            stack.push_back(engine->createSolid(desc));
        }

        // Simulate 120 steps (~2 seconds) to let stack settle
        UnitOfTime dt(16'666'667);
        for (int i = 0; i < 120; ++i) { engine->step(dt); }

        // Verify stack remains vertically aligned and monotonically ordered in Y
        float prevY = 0.0f;
        for (int i = 0; i < 5; ++i) {
            Vector3 pos = stack[i]->position();
            CHECK(pos.y > prevY);
            CHECK(std::abs(pos.x) < 0.2f);
            CHECK(std::abs(pos.z) < 0.2f);
            prevY = pos.y;
        }
    }

    SECTION("Bouncing restitution comparison") {
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, -9.81f, 0.0f};
        engineDesc.numWorkerThreads = 1;
        auto engine                 = SolidEngine::create(engineDesc);
        REQUIRE(engine);

        // Floor
        SolidDesc floorDesc;
        floorDesc.hull               = Hull::createBox({50.0f, 1.0f, 50.0f});
        floorDesc.motionType         = MotionType::STATIC;
        floorDesc.layer              = CollisionLayer::NON_MOVING;
        floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
        floorDesc.temper.restitution = 0.0f;
        engine->createSolid(floorDesc);

        auto sphereHull = Hull::createSphere(0.5f);

        // Inelastic sphere (restitution 0.0)
        SolidDesc inelasticDesc;
        inelasticDesc.hull               = sphereHull;
        inelasticDesc.motionType         = MotionType::DYNAMIC;
        inelasticDesc.layer              = CollisionLayer::MOVING;
        inelasticDesc.transform.position = {-5.0f, 10.0f, 0.0f};
        inelasticDesc.temper.restitution = 0.0f;
        auto inelasticBall               = engine->createSolid(inelasticDesc);

        // Elastic sphere (restitution 0.85)
        SolidDesc elasticDesc;
        elasticDesc.hull               = sphereHull;
        elasticDesc.motionType         = MotionType::DYNAMIC;
        elasticDesc.layer              = CollisionLayer::MOVING;
        elasticDesc.transform.position = {5.0f, 10.0f, 0.0f};
        elasticDesc.temper.restitution = 0.85f;
        auto elasticBall               = engine->createSolid(elasticDesc);

        // Simulate drop and first rebound (approx 1.8 seconds = 110 steps)
        UnitOfTime dt(16'666'667);
        float      maxElasticReboundY = 0.0f;
        for (int i = 0; i < 110; ++i) {
            engine->step(dt);
            if (i > 85) { // After bounce time (~1.4s)
                maxElasticReboundY = std::max(maxElasticReboundY, elasticBall->position().y);
            }
        }

        // Inelastic ball rests near floor (radius 0.5)
        CHECK(inelasticBall->position().y < 0.6f);

        // Elastic ball should have rebounded significantly higher
        CHECK(maxElasticReboundY > 2.0f);
    }

    SECTION("CCD prevents high-velocity projectile tunneling") {
        SolidEngineDesc engineDesc;
        engineDesc.gravity          = {0.0f, 0.0f, 0.0f}; // Zero-g to isolate horizontal velocity
        engineDesc.numWorkerThreads = 1;
        auto engine                 = SolidEngine::create(engineDesc);
        REQUIRE(engine);

        // Thin wall at x = 10 (thickness 0.2, from x = 9.9 to 10.1)
        SolidDesc wallDesc;
        wallDesc.hull               = Hull::createBox({0.1f, 10.0f, 10.0f});
        wallDesc.motionType         = MotionType::STATIC;
        wallDesc.layer              = CollisionLayer::NON_MOVING;
        wallDesc.transform.position = {10.0f, 0.0f, 0.0f};
        engine->createSolid(wallDesc);

        // Fast projectile with CCD enabled: speed = 300 m/s pointing towards wall
        // In one 16.666 ms step, displacement is 5 meters, 25x larger than wall thickness
        SolidDesc bulletDesc;
        bulletDesc.hull               = Hull::createSphere(0.2f);
        bulletDesc.motionType         = MotionType::DYNAMIC;
        bulletDesc.layer              = CollisionLayer::MOVING;
        bulletDesc.transform.position = {0.0f, 0.0f, 0.0f};
        bulletDesc.linearVelocity     = {300.0f, 0.0f, 0.0f};
        bulletDesc.ccd                = true; // Enable Continuous Collision Detection
        bulletDesc.temper.restitution = 0.5f;
        auto bullet                   = engine->createSolid(bulletDesc);
        REQUIRE(bullet);

        // Step physics forward 4 steps (~66 ms, total displacement would be 20 m without wall)
        UnitOfTime dt(16'666'667);
        for (int i = 0; i < 4; ++i) { engine->step(dt); }

        // CCD ensures the bullet does not pass through the wall: position.x must be <= 10.1
        Vector3 bulletPos = bullet->position();
        CHECK(bulletPos.x <= 10.1f);
    }
}

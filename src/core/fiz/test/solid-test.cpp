#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <garnet/GNfiz.h>

using namespace GN::fiz;

TEST_CASE("fiz hull geometries compute correct volumes and bounds", "[fiz][hull]") {
    SECTION("BoxHull") {
        auto box = Hull::createBox({1.0f, 2.0f, 3.0f});
        REQUIRE(box);
        CHECK(box->type() == HullType::BOX);
        CHECK(box->calculateVolume() == Catch::Approx(8.0f * 1.0f * 2.0f * 3.0f));

        Box aabb = box->localAabb();
        CHECK(aabb.x == Catch::Approx(-1.0f));
        CHECK(aabb.y == Catch::Approx(-2.0f));
        CHECK(aabb.z == Catch::Approx(-3.0f));
        CHECK(aabb.w == Catch::Approx(2.0f));
        CHECK(aabb.h == Catch::Approx(4.0f));
        CHECK(aabb.d == Catch::Approx(6.0f));
    }

    SECTION("SphereHull") {
        auto sphere = Hull::createSphere(2.0f);
        REQUIRE(sphere);
        CHECK(sphere->type() == HullType::SPHERE);
        float expectedVolume = (4.0f / 3.0f) * 3.14159265f * 8.0f;
        CHECK(sphere->calculateVolume() == Catch::Approx(expectedVolume).epsilon(0.01f));

        Box aabb = sphere->localAabb();
        CHECK(aabb.x == Catch::Approx(-2.0f));
        CHECK(aabb.w == Catch::Approx(4.0f));
    }

    SECTION("CapsuleHull") {
        auto capsule = Hull::createCapsule(1.0f, 2.0f);
        REQUIRE(capsule);
        CHECK(capsule->type() == HullType::CAPSULE);
        CHECK(capsule->calculateVolume() > 0.0f);
    }

    SECTION("ConvexHull") {
        Vector3 points[] = {{-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f},
                            {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f},  {1.0f, 1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f}};
        auto    convex   = Hull::createConvex(points, 8);
        REQUIRE(convex);
        CHECK(convex->type() == HullType::CONVEX);
        CHECK(convex->calculateVolume() > 0.0f);
    }
}

TEST_CASE("fiz solid engine creates, advances, and contacts rigid bodies", "[fiz][solid]") {
    SolidEngineDesc engineDesc;
    engineDesc.gravity          = {0.0f, -9.81f, 0.0f};
    engineDesc.numWorkerThreads = 1;
    auto engine                 = SolidEngine::create(engineDesc);
    REQUIRE(engine);
    CHECK(engine->bodyCount() == 0);

    // 1. Create static floor at y = -1 (thickness 2, top surface at y = 0)
    SolidDesc floorDesc;
    floorDesc.hull               = Hull::createBox({50.0f, 1.0f, 50.0f});
    floorDesc.motionType         = MotionType::STATIC;
    floorDesc.layer              = CollisionLayer::NON_MOVING;
    floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
    auto floor                   = engine->createSolid(floorDesc);
    REQUIRE(floor);
    CHECK(floor->motionType() == MotionType::STATIC);
    CHECK(engine->bodyCount() == 1);

    // 2. Create dynamic sphere at y = 5.0 (radius 0.5)
    SolidDesc sphereDesc;
    sphereDesc.hull               = Hull::createSphere(0.5f);
    sphereDesc.motionType         = MotionType::DYNAMIC;
    sphereDesc.layer              = CollisionLayer::MOVING;
    sphereDesc.transform.position = {0.0f, 5.0f, 0.0f};
    sphereDesc.temper.restitution = 0.0f; // completely inelastic so it rests on the floor
    auto sphere                   = engine->createSolid(sphereDesc);
    REQUIRE(sphere);
    CHECK(sphere->motionType() == MotionType::DYNAMIC);
    CHECK(engine->bodyCount() == 2);
    CHECK(sphere->position().y == Catch::Approx(5.0f));

    // 3. Step forward by 1 second (60 steps of 16.666 ms)
    UnitOfTime stepDuration(16'666'667); // ~1/60 s
    for (int i = 0; i < 60; ++i) { engine->step(stepDuration); }

    // 4. Sphere should have fallen and come to rest on the floor top surface (y = 0 + radius 0.5 = 0.5)
    Vector3 finalPos = sphere->position();
    CHECK(finalPos.y < 5.0f);
    CHECK(finalPos.y >= Catch::Approx(0.49f).margin(0.05f));
    CHECK(finalPos.x == Catch::Approx(0.0f).margin(0.05f));
    CHECK(finalPos.z == Catch::Approx(0.0f).margin(0.05f));
}

TEST_CASE("fiz solid raycast detects intersections and returns hit details", "[fiz][raycast]") {
    SolidEngineDesc engineDesc;
    engineDesc.gravity = {0.0f, 0.0f, 0.0f};
    auto engine        = SolidEngine::create(engineDesc);
    REQUIRE(engine);

    // Place a sphere at (0, 10, 0) with radius 1.0
    SolidDesc sphereDesc;
    sphereDesc.hull               = Hull::createSphere(1.0f);
    sphereDesc.motionType         = MotionType::STATIC;
    sphereDesc.layer              = CollisionLayer::NON_MOVING;
    sphereDesc.transform.position = {0.0f, 10.0f, 0.0f};
    auto sphere                   = engine->createSolid(sphereDesc);
    REQUIRE(sphere);

    // Cast downward ray from (0, 20, 0) pointing towards (0, -1, 0)
    RaycastHit hit;
    bool       hasHit = engine->raycast({0.0f, 20.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, 50.0f, hit);
    CHECK(hasHit);
    CHECK(hit.hit);
    // Should hit top of sphere at (0, 11, 0), distance = 20 - 11 = 9
    CHECK(hit.distance == Catch::Approx(9.0f).margin(0.05f));
    CHECK(hit.position.x == Catch::Approx(0.0f).margin(0.01f));
    CHECK(hit.position.y == Catch::Approx(11.0f).margin(0.05f));
    CHECK(hit.position.z == Catch::Approx(0.0f).margin(0.01f));
    CHECK(hit.normal.y == Catch::Approx(1.0f).margin(0.05f));
    CHECK(hit.solid.get() == sphere.get());

    // Miss ray pointing in opposite direction
    RaycastHit missHit;
    bool       miss = engine->raycast({0.0f, 20.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 50.0f, missHit);
    CHECK_FALSE(miss);
    CHECK_FALSE(missHit.hit);
}

TEST_CASE("fiz solid velocity and impulse application", "[fiz][solid]") {
    SolidEngineDesc engineDesc;
    engineDesc.gravity          = {0.0f, 0.0f, 0.0f}; // zero-g environment
    engineDesc.numWorkerThreads = 1;
    auto engine                 = SolidEngine::create(engineDesc);
    REQUIRE(engine);

    SolidDesc boxDesc;
    boxDesc.hull                  = Hull::createBox({0.5f, 0.5f, 0.5f});
    boxDesc.motionType            = MotionType::DYNAMIC;
    boxDesc.layer                 = CollisionLayer::MOVING;
    boxDesc.massOverride          = 2.0f;
    boxDesc.temper.linearDamping  = 0.0f; // no drag
    boxDesc.temper.angularDamping = 0.0f;
    auto box                      = engine->createSolid(boxDesc);
    REQUIRE(box);

    CHECK(box->linearVelocity().x == Catch::Approx(0.0f));

    // Set linear velocity
    box->setLinearVelocity({5.0f, 0.0f, 0.0f});
    CHECK(box->linearVelocity().x == Catch::Approx(5.0f));

    // Advance 1 second
    engine->step(UnitOfTime(1'000'000'000));
    CHECK(box->position().x == Catch::Approx(5.0f).margin(0.1f));

    // Apply linear impulse opposite to motion: impulse = -10 (mass = 2, so delta_v = -5)
    box->applyImpulse({-10.0f, 0.0f, 0.0f});
    CHECK(box->linearVelocity().x == Catch::Approx(0.0f).margin(0.05f));
}

TEST_CASE("fiz solid removal and cleanup", "[fiz][solid]") {
    SolidEngineDesc engineDesc;
    auto            engine = SolidEngine::create(engineDesc);
    REQUIRE(engine);

    SolidDesc desc;
    desc.hull       = Hull::createSphere(1.0f);
    desc.motionType = MotionType::DYNAMIC;
    auto s1         = engine->createSolid(desc);
    auto s2         = engine->createSolid(desc);
    CHECK(engine->bodyCount() == 2);

    engine->removeSolid(s1.get());
    CHECK(engine->bodyCount() == 1);

    engine->removeSolid(s2.get());
    CHECK(engine->bodyCount() == 0);
}

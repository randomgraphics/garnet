// CPU-only verification of the engine2 world / visual-moment workflow using the Simple
// world. No GPU or window is involved: this exercises form population, the background
// simulation cadence, and self-contained snapshot capture.

#include <catch2/catch_test_macros.hpp>

#include "e2/e2-internal.h" // VisualMomentImpl (internal), reachable via the src/core include path.

#include <chrono>
#include <thread>

using namespace GN;
using namespace GN::e2;

namespace {
struct TestUniverse : Universe {};
} // namespace

TEST_CASE("E2 Simple: capture produces a self-contained, populated visual moment") {
    TestUniverse u;

    auto world = Simple::createWorld(u);
    REQUIRE(world);

    auto box   = Simple::createBox(u, WorldPosition(WorldLength(0), WorldLength(0), WorldLength(0)),
                                   Vector3<WorldLength>(WorldLength(1), WorldLength(1), WorldLength(1)));
    auto light = Simple::createPointLight(u, WorldPosition(WorldLength(3), WorldLength(3), WorldLength(3)), IntensityRGB {1.f, 1.f, 1.f, Candela {50.f}});
    REQUIRE(box);
    REQUIRE(light);

    // Unique ids within the universe.
    REQUIRE(box->id != light->id);
    REQUIRE(box->id != world->id);

    Ref<Form> forms[] = {box, light};
    world->populate({forms, 2});

    // Capture with no cameras: a Camera now sources its universe from a (GPU-backed)
    // visual domain, so this CPU-only test exercises the form/snapshot path directly.
    // Camera snapshotting is covered end-to-end by the sample.
    VisualMoment::CaptureParameters cp;

    auto moment = world->captureVisualMoment(cp);
    REQUIRE(moment);

    auto * impl = RuntimeType::cast<VisualMomentImpl>(moment.get());
    REQUIRE(impl != nullptr);
    CHECK(impl->cameras.size() == 0);
    CHECK(impl->renderables.size() == 1);
    CHECK(impl->lights.size() == 1);
    REQUIRE(impl->renderables[0].mesh);
    CHECK(impl->renderables[0].mesh->vertices.size() == 24);
    CHECK(impl->renderables[0].mesh->indices.size() == 36);
}

TEST_CASE("E2 Simple: the world evolves on its own cadence, independent of capture") {
    TestUniverse u;

    auto      world   = Simple::createWorld(u);
    auto      box     = Simple::createBox(u, WorldPosition(WorldLength(0), WorldLength(0), WorldLength(0)),
                                          Vector3<WorldLength>(WorldLength(1), WorldLength(1), WorldLength(1)));
    Ref<Form> forms[] = {box};
    world->populate({forms, 1});

    VisualMoment::CaptureParameters cp; // no cameras needed for this check

    // run() blocks, so the test provides the thread — mirroring how a real client drives a world.
    std::thread simThread([&world] { world->run(); });

    auto firstMoment = world->captureVisualMoment(cp);
    std::this_thread::sleep_for(std::chrono::milliseconds(120)); // several simulation ticks
    auto secondMoment = world->captureVisualMoment(cp);

    // Join before any assertion: a failing REQUIRE throws, and an unjoined thread terminates.
    world->stop();
    simThread.join();

    auto * first  = RuntimeType::cast<VisualMomentImpl>(firstMoment.get());
    auto * second = RuntimeType::cast<VisualMomentImpl>(secondMoment.get());
    REQUIRE(first);
    REQUIRE(second);
    REQUIRE(first->renderables.size() == 1);
    REQUIRE(second->renderables.size() == 1);

    // The box spins while the world runs, so its orientation must have changed between
    // two captures taken at different times, without the test driving update() itself.
    CHECK(first->renderables[0].rotation != second->renderables[0].rotation);
}

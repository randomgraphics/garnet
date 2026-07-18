// CPU-only verification of the engine2 world / visual-moment workflow using the Simple
// world. No GPU or window is involved: this exercises form population, the background
// simulation cadence, and self-contained snapshot capture.

#include <catch2/catch_test_macros.hpp>

#include "e2/e2-internal.h" // VisualMomentImpl (internal), reachable via the src/core include path.

#include <glm/geometric.hpp>

#include <chrono>
#include <thread>

using namespace GN;
using namespace GN::e2;

namespace {

struct TestGroupForm : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);

    TestGroupForm(Universe & u, const StrA & name = "test-group"): Form(TYPE_INFO(), u.generateUniqueIdentifier(), name) {}
};

} // namespace

TEST_CASE("E2 Simple: capture produces a self-contained, populated visual moment") {
    Universe u;

    auto world = Simple::createWorld(u);
    REQUIRE(world);

    auto box = Simple::createBox(u, WorldVector3(WorldLength(0), WorldLength(0), WorldLength(0)), WorldVector3(WorldLength(1), WorldLength(1), WorldLength(1)));
    auto light = Simple::createPointLight(u, WorldVector3(WorldLength(3), WorldLength(3), WorldLength(3)), IntensityRGB {1.f, 1.f, 1.f, Candela {50.f}});
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

TEST_CASE("E2 Simple: visual capture walks child forms") {
    Universe u;

    auto world = Simple::createWorld(u);
    auto group = referenceTo(new TestGroupForm(u));
    auto box = Simple::createBox(u, WorldVector3(WorldLength(0), WorldLength(0), WorldLength(0)), WorldVector3(WorldLength(1), WorldLength(1), WorldLength(1)));
    auto light = Simple::createPointLight(u, WorldVector3(WorldLength(3), WorldLength(3), WorldLength(3)), IntensityRGB {1.f, 1.f, 1.f, Candela {50.f}});
    REQUIRE(group);
    REQUIRE(box);
    REQUIRE(light);
    REQUIRE(group->attach(box));
    REQUIRE(group->attach(light));

    Ref<Form> forms[] = {group};
    world->populate({forms, 1});

    auto   moment = world->captureVisualMoment(VisualMoment::CaptureParameters {});
    auto * impl   = RuntimeType::cast<VisualMomentImpl>(moment.get());
    REQUIRE(impl != nullptr);
    CHECK(impl->renderables.size() == 1);
    CHECK(impl->lights.size() == 1);
    CHECK(group->world() == world.get());
    CHECK(box->world() == world.get());
    CHECK(light->world() == world.get());
}

TEST_CASE("E2 Mold: casting creates a fresh form tree") {
    Universe u;

    auto groupMold =
        Mold::create(u, "group-mold", [](const Mold::CreateParameters & cp) -> Ref<Form> { return referenceTo(new TestGroupForm(cp.universe, cp.name)); });
    auto boxMold = Mold::create(u, "box-mold", [](const Mold::CreateParameters & cp) -> Ref<Form> {
        return Simple::createBox(cp.universe, WorldVector3(WorldLength(0), WorldLength(0), WorldLength(0)),
                                 WorldVector3(WorldLength(1), WorldLength(1), WorldLength(1)));
    });
    REQUIRE(groupMold);
    REQUIRE(boxMold);
    REQUIRE(groupMold->add(boxMold, "box-child"));

    auto a = groupMold->cast(u, "group-a");
    auto b = groupMold->cast(u, "group-b");
    REQUIRE(a);
    REQUIRE(b);
    CHECK(a->id != b->id);
    REQUIRE(a->children().size() == 1);
    REQUIRE(b->children().size() == 1);
    CHECK(a->children()[0]->id != b->children()[0]->id);
    CHECK(a->children()[0]->parent() == a.get());
}

TEST_CASE("E2 Simple: unit box faces are wound CCW when viewed from outside") {
    Universe u;

    auto world = Simple::createWorld(u);
    auto box = Simple::createBox(u, WorldVector3(WorldLength(0), WorldLength(0), WorldLength(0)), WorldVector3(WorldLength(1), WorldLength(1), WorldLength(1)));
    REQUIRE(world);
    REQUIRE(box);

    Ref<Form> forms[] = {box};
    world->populate({forms, 1});

    auto   moment = world->captureVisualMoment(VisualMoment::CaptureParameters {});
    auto * impl   = RuntimeType::cast<VisualMomentImpl>(moment.get());
    REQUIRE(impl);
    REQUIRE(impl->renderables.size() == 1);
    REQUIRE(impl->renderables[0].mesh);

    const MeshData & mesh = *impl->renderables[0].mesh;
    REQUIRE(mesh.indices.size() % 3 == 0);

    for (size_t t = 0; t < mesh.indices.size(); t += 3) {
        auto & v0 = mesh.vertices[mesh.indices[t + 0]];
        auto & v1 = mesh.vertices[mesh.indices[t + 1]];
        auto & v2 = mesh.vertices[mesh.indices[t + 2]];

        // All 3 vertices of a box triangle share the face normal.
        REQUIRE(v0.normal == v1.normal);
        REQUIRE(v0.normal == v2.normal);

        // CCW from outside <=> the geometric normal of the triangle points the same way
        // as the stored outward face normal.
        glm::vec3 geometric = glm::cross(v1.position - v0.position, v2.position - v0.position);
        INFO("triangle " << (t / 3) << " normal (" << v0.normal.x << ", " << v0.normal.y << ", " << v0.normal.z << ")");
        CHECK(glm::dot(geometric, v0.normal) > 0.f);
    }
}

TEST_CASE("E2 Simple: the world evolves on its own cadence, independent of capture") {
    Universe u;

    auto world = Simple::createWorld(u);
    auto box = Simple::createBox(u, WorldVector3(WorldLength(0), WorldLength(0), WorldLength(0)), WorldVector3(WorldLength(1), WorldLength(1), WorldLength(1)));
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

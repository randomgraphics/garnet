// CPU-only verification of the engine2 world / visual-moment workflow using the Simple
// world. No GPU or window is involved: this exercises form population, the background
// simulation cadence, and self-contained snapshot capture.

#include <catch2/catch_test_macros.hpp>

#include "e2/e2-internal.h" // VisualMomentImpl (internal), reachable via the src/core include path.

#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>
#include <thread>

using namespace GN;
using namespace GN::e2;

namespace {

// Test shorthand: the absolute coordinate \p v units from the origin.
WorldCoordinate W(int64_t v) { return spatial::toWorld(LocalCoordinate(v)); }

struct CountingFacet : Facet {
    GN_REGISTER_RUNTIME_TYPE(Facet);

    int lives  = 0; // live() call count, not remaining game lives
    int enters = 0;
    int leaves = 0;

    // form()->world() as observed from inside the callbacks, to verify the documented
    // ordering: already set when enterWorld() arrives, still set during leaveWorld().
    World * worldSeenOnEnter = nullptr;
    World * worldSeenOnLeave = nullptr;

    CountingFacet(Universe & u): Facet(TYPE_INFO(), u.generateUniqueIdentifier(), "counting-facet") {}

    void live() override { ++lives; }

    void enterWorld(World &) override {
        ++enters;
        worldSeenOnEnter = form() ? form()->world() : nullptr;
    }

    void leaveWorld(World &) override {
        ++leaves;
        worldSeenOnLeave = form() ? form()->world() : nullptr;
    }
};

} // namespace

TEST_CASE("E2 Simple: capture produces a self-contained, populated visual moment") {
    Universe u;

    auto world = Simple::createWorld(u);
    REQUIRE(world);

    auto box   = Simple::createBox(u, WorldVector3(W(0), W(0), W(0)), LocalVector3(LocalCoordinate(1), LocalCoordinate(1), LocalCoordinate(1)));
    auto light = Simple::createPointLight(u, WorldVector3(W(3), W(3), W(3)), IntensityRGB {1.f, 1.f, 1.f, Candela {50.f}});
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

TEST_CASE("E2 Simple: visual capture walks child forms and composes their transforms") {
    Universe u;

    auto world = Simple::createWorld(u);
    auto group = Form::create(u, "test-group");
    auto box   = Simple::createBox(u, WorldVector3(W(0), W(0), W(0)), LocalVector3(LocalCoordinate(1), LocalCoordinate(1), LocalCoordinate(1)));
    auto light = Simple::createPointLight(u, WorldVector3(W(3), W(3), W(3)), IntensityRGB {1.f, 1.f, 1.f, Candela {50.f}});
    REQUIRE(group);
    REQUIRE(box);
    REQUIRE(light);
    REQUIRE(group->attach(box));
    REQUIRE(group->attach(light));
    group->setPosition(WorldVector3(W(5), W(6), W(7)));

    Ref<Form> forms[] = {group};
    world->populate({forms, 1});

    auto   moment = world->captureVisualMoment(VisualMoment::CaptureParameters {});
    auto * impl   = RuntimeType::cast<VisualMomentImpl>(moment.get());
    REQUIRE(impl != nullptr);
    REQUIRE(impl->renderables.size() == 1);
    REQUIRE(impl->lights.size() == 1);
    CHECK(group->world() == world.get());
    CHECK(box->world() == world.get());
    CHECK(light->world() == world.get());

    // Captured positions must be in world space: the group's offset shifts both children.
    CHECK(impl->renderables[0].translation.x == W(5));
    CHECK(impl->renderables[0].translation.y == W(6));
    CHECK(impl->renderables[0].translation.z == W(7));
    CHECK(impl->lights[0].position.x == W(8));
    CHECK(impl->lights[0].position.y == W(9));
    CHECK(impl->lights[0].position.z == W(10));
}

TEST_CASE("E2 Form: facets attach, live, and detach with their form") {
    Universe u;

    auto facet = referenceTo(new CountingFacet(u));
    {
        auto form = Form::create(u, "test-group");
        REQUIRE(form);
        CHECK(!form->addFacet({})); // null facet is rejected
        REQUIRE(form->addFacet(facet));
        CHECK(facet->form() == form.get());
        REQUIRE(form->facets().size() == 1);
        CHECK(form->facets()[0].get() == facet.get());

        auto other = Form::create(u, "other-group");
        CHECK(!other->addFacet(facet)); // a facet belongs to at most one form

        CHECK(facet->lives == 0);
        form->live();
        CHECK(facet->lives == 1);
    }
    // The facet outlived its form; its back-pointer must not dangle.
    CHECK(facet->form() == nullptr);
}

TEST_CASE("E2 Facet: world entry and exit reach facets on every path") {
    Universe u;

    auto early = referenceTo(new CountingFacet(u)); // attached before the form enters a world
    auto late  = referenceTo(new CountingFacet(u)); // attached while the form lives in a world
    auto below = referenceTo(new CountingFacet(u)); // on a child attached under a populated form

    auto form = Form::create(u, "test-group");
    REQUIRE(form);
    REQUIRE(form->addFacet(early));
    CHECK(early->enters == 0); // no world yet

    {
        auto world = Simple::createWorld(u);

        Ref<Form> forms[] = {form};
        world->populate({forms, 1});
        CHECK(early->enters == 1); // the owning form entered a world

        REQUIRE(form->addFacet(late));
        CHECK(late->enters == 1); // added to a form already living in a world

        auto child = Form::create(u, "child");
        REQUIRE(child->addFacet(below));
        CHECK(below->enters == 0);
        REQUIRE(form->attach(child));
        CHECK(below->enters == 1); // joined through the parent's world membership

        CHECK(early->leaves == 0);
    }

    // The world is gone; its destructor detaches root forms, which reaches every facet.
    CHECK(form->world() == nullptr);
    CHECK(early->leaves == 1);
    CHECK(late->leaves == 1);
    CHECK(below->leaves == 1);
    CHECK(early->enters == 1); // no spurious re-entry
}

TEST_CASE("E2 Facet: enter/leave pair up regardless of when the facet joins the form") {
    Universe u;

    auto world = Simple::createWorld(u);
    REQUIRE(world);

    auto before = referenceTo(new CountingFacet(u)); // joins the form outside any world
    auto after  = referenceTo(new CountingFacet(u)); // joins the form while it lives in the world

    auto form = Form::create(u, "test-form");
    REQUIRE(form);

    // Attached outside a world: no notification until the form enters one.
    REQUIRE(form->addFacet(before));
    CHECK(before->enters == 0);
    CHECK(before->leaves == 0);

    // The test drives enterWorld()/leaveWorld() directly, standing in for a world
    // implementation, so the enter/leave pairing is observable without world-side
    // population bookkeeping.
    REQUIRE(form->enterWorld(*world));
    CHECK(before->enters == 1);
    CHECK(before->worldSeenOnEnter == world.get()); // form()->world() set before the callback

    REQUIRE(form->addFacet(after));
    CHECK(after->enters == 1); // added to a form already in a world: notified immediately
    CHECK(after->worldSeenOnEnter == world.get());

    // Leaving reaches both facets exactly once, regardless of when they joined.
    form->leaveWorld(*world);
    CHECK(form->world() == nullptr);
    CHECK(before->leaves == 1);
    CHECK(after->leaves == 1);
    CHECK(before->worldSeenOnLeave == world.get()); // form()->world() still set during the callback
    CHECK(after->worldSeenOnLeave == world.get());
    CHECK(before->enters == 1); // leaving must not re-enter
    CHECK(after->enters == 1);

    // The pairing survives re-entry into a world.
    REQUIRE(form->enterWorld(*world));
    CHECK(before->enters == 2);
    CHECK(after->enters == 2);
    form->leaveWorld(*world);
    CHECK(before->leaves == 2);
    CHECK(after->leaves == 2);

    // A form destroyed while still inside a world tells its facets to leave on the way out.
    auto orphan = referenceTo(new CountingFacet(u));
    {
        auto doomed = Form::create(u, "doomed-form");
        REQUIRE(doomed);
        REQUIRE(doomed->addFacet(orphan));
        REQUIRE(doomed->enterWorld(*world));
        CHECK(orphan->enters == 1);
    }
    CHECK(orphan->leaves == 1);
    CHECK(orphan->worldSeenOnLeave == world.get());
    CHECK(orphan->form() == nullptr);
}

TEST_CASE("E2 Form: transform composes through the parent chain") {
    Universe u;

    auto parent = Form::create(u, "parent");
    auto child  = Form::create(u, "child");
    REQUIRE(parent);
    REQUIRE(child);
    REQUIRE(parent->attach(child));

    parent->setPosition(WorldVector3(W(10), W(20), W(30)));
    parent->setRotation(glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f)));
    child->setPosition(WorldVector3(W(100), W(0), W(0)));

    // A root form's local transform is already world space.
    CHECK(parent->worldPosition().x == W(10));
    CHECK(parent->worldPosition().y == W(20));
    CHECK(parent->worldPosition().z == W(30));

    // The child's local +X offset lands on +Y after the parent's 90-degree Z rotation.
    auto wp = child->worldPosition();
    CHECK(wp.x == W(10));
    CHECK(wp.y == W(120));
    CHECK(wp.z == W(30));

    // The composed rotation carries the parent's rotation (identity child rotation).
    auto wr = child->worldRotation();
    CHECK(glm::dot(wr, parent->rotation()) > 0.9999f);
}

TEST_CASE("E2 Mold: casting creates a fresh form tree") {
    Universe u;

    auto groupMold = Mold::create(u, "group-mold", [](const Mold::CreateParameters & cp) -> Ref<Form> { return Form::create(cp.universe, cp.name); });
    auto boxMold   = Mold::create(u, "box-mold", [](const Mold::CreateParameters & cp) -> Ref<Form> {
        return Simple::createBox(cp.universe, WorldVector3(W(0), W(0), W(0)), LocalVector3(LocalCoordinate(1), LocalCoordinate(1), LocalCoordinate(1)));
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
    auto box   = Simple::createBox(u, WorldVector3(W(0), W(0), W(0)), LocalVector3(LocalCoordinate(1), LocalCoordinate(1), LocalCoordinate(1)));
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

    auto      world   = Simple::createWorld(u);
    auto      box     = Simple::createBox(u, WorldVector3(W(0), W(0), W(0)), LocalVector3(LocalCoordinate(1), LocalCoordinate(1), LocalCoordinate(1)));
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
    // two captures taken at different times, without the test driving live() itself.
    CHECK(first->renderables[0].rotation != second->renderables[0].rotation);
}

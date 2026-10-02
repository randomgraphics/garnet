#include <garnet/GNengine2.h>
#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <cmath>

using namespace GN;
using namespace GN::e2;

TEST_CASE("E2 dynamics: local batch solves multiple boxes and persists symmetric contacts", "[e2][world-dynamics]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world);
    REQUIRE(registerDynamicsFacets(*world));
    auto ground = world->createForm(*createGroundMold(universe));
    REQUIRE(ground != 0);
    TransformFacet::Value lower, upper;
    lower.position = positionFromMeters({0, 0.3, 0}, PhysicalScale::NANOMETER());
    upper.position = positionFromMeters({0, 1, 0}, PhysicalScale::NANOMETER());
    auto a         = world->createForm(*createBoxMold(universe, lower, {}));
    REQUIRE(a != 0);
    auto b = world->createForm(*createBoxMold(universe, upper, {}));
    REQUIRE(b != 0);
    REQUIRE(world->addLaw(createDynamicsLaw()));
    LifetimeOptions lifetime;
    lifetime.maximumPopulation = 0;
    REQUIRE(world->initializeState(LifetimeState {}));
    REQUIRE(world->addLaw(createLifetimeLaw(universe, lifetime)));
    auto old = world->primeSnapshot();
    for (int i = 0; i < 240; ++i) REQUIRE(world->tick(UnitOfTime {8'333'333}));
    auto prime = world->primeSnapshot();
    REQUIRE(prime->get<TransformFacet>(a));
    REQUIRE(prime->get<TransformFacet>(b));
    auto p = positionToMeters(prime->get<TransformFacet>(a)->position, PhysicalScale::NANOMETER());
    auto q = positionToMeters(prime->get<TransformFacet>(b)->position, PhysicalScale::NANOMETER());
    CHECK(std::abs(p.y - 0.25) < 0.01);
    CHECK(q.y - p.y > 0.49);
    auto contacts = prime->get<ContactFacet>(a)->touching;
    CHECK(std::find(contacts.begin(), contacts.end(), ground) != contacts.end());
    CHECK(std::find(contacts.begin(), contacts.end(), b) != contacts.end());
    auto groundContacts = prime->get<ContactFacet>(ground)->touching;
    CHECK(std::find(groundContacts.begin(), groundContacts.end(), a) != groundContacts.end());
    auto count = prime->get<CollisionStatsFacet>(a)->count;
    for (int i = 0; i < 60; ++i) REQUIRE(world->tick(UnitOfTime {8'333'333}));
    CHECK(world->primeSnapshot()->get<CollisionStatsFacet>(a)->count == count);
    CHECK(old->get<ContactFacet>(a)->touching.empty());
    auto form = prime->form(b);
    REQUIRE(form);
    auto lifetimeFacet = form->getFacet<LifetimeFacet>(*prime);
    REQUIRE(lifetimeFacet);
    REQUIRE(lifetimeFacet->requestDestroy(*world, *form));
    REQUIRE(world->tick(UnitOfTime {8'333'333}));
    prime = world->primeSnapshot();
    CHECK_FALSE(prime->form(b));
    contacts = prime->get<ContactFacet>(a)->touching;
    CHECK(std::find(contacts.begin(), contacts.end(), b) == contacts.end());
}

TEST_CASE("E2 lifetime: capped generation has bounded horizontal speed and discards full-capacity credit", "[e2][world-dynamics]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world);
    REQUIRE(registerDynamicsFacets(*world));
    REQUIRE(world->initializeState(LifetimeState {}));
    REQUIRE(world->addLaw(createDynamicsLaw()));
    LifetimeOptions options;
    options.maximumPopulation = 3;
    REQUIRE(world->addLaw(createLifetimeLaw(universe, options)));
    REQUIRE(world->tick(UnitOfTime {10'000'000}));
    auto prime = world->primeSnapshot();
    REQUIRE(prime->query<LifetimeFacet>().size() == 1);
    auto first    = prime->query<LifetimeFacet>()[0];
    auto velocity = prime->get<MotionFacet>(first)->linearVelocity;
    CHECK(velocity.y == 0);
    CHECK(glm::length(velocity) <= 0.5);
    REQUIRE(world->tick(UnitOfTime {10'000'000}));
    REQUIRE(world->tick(UnitOfTime {10'000'000}));
    REQUIRE(world->tick(UnitOfTime {100'000'000}));
    CHECK(world->primeSnapshot()->query<LifetimeFacet>().size() == 3);
    REQUIRE(world->submit(DestroyFormIntent {first}));
    REQUIRE(world->tick(UnitOfTime {1'000'000}));
    CHECK(world->primeSnapshot()->query<LifetimeFacet>().size() == 2);
    REQUIRE(world->tick(UnitOfTime {9'000'000}));
    CHECK(world->primeSnapshot()->query<LifetimeFacet>().size() == 3);
}

TEST_CASE("E2 dynamics: solved out-of-bounds position routes deletion to later LifetimeLaw", "[e2][world-dynamics]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world);
    REQUIRE(registerDynamicsFacets(*world));
    REQUIRE(world->initializeState(LifetimeState {}));
    TransformFacet::Value transform;
    transform.position = positionFromMeters({5, -14.99, 0}, PhysicalScale::NANOMETER());
    MotionFacet::Value motion;
    motion.linearVelocity.y = -2;
    auto id                 = world->createForm(*createBoxMold(universe, transform, motion));
    REQUIRE(id != 0);
    REQUIRE(world->addLaw(createDynamicsLaw()));
    LifetimeOptions options;
    options.maximumPopulation = 0;
    REQUIRE(world->addLaw(createLifetimeLaw(universe, options)));
    auto before = world->primeSnapshot();
    REQUIRE(world->tick(UnitOfTime {10'000'000}));
    CHECK_FALSE(world->primeSnapshot()->form(id));
    CHECK(before->form(id));
}

namespace {
struct TaggedMotionValue : MotionFacet::Value {
    GN_REGISTER_RUNTIME_TYPE(MotionFacet::Value);
    TaggedMotionValue(): MotionFacet::Value(TYPE_INFO()) {}
    Ref<FacetValue> clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<FacetValue>(new TaggedMotionValue(*this));
    }
    int tag = 37;
    int applicationTag() const { return tag; }
};

} // namespace

TEST_CASE("E2 dynamics: extended payload on an exact MotionFacet survives the domain solver", "[e2][world-dynamics]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world);
    REQUIRE(registerDynamicsFacets(*world));
    TransformFacet::Value transform;
    transform.position = positionFromMeters({0, 3, 0}, PhysicalScale::NANOMETER());
    TaggedMotionValue motion;
    motion.tag = 91;
    auto mold  = Mold::create(universe, "tagged-box",
                              {
                                  {Ref<Facet>(new TransformFacet), transform.clone()},
                                  {Ref<Facet>(new MotionFacet), motion.clone()},
                                  {Ref<Facet>(new BodyFacet), Ref<FacetValue>(new BodyFacet::Value)},
                                  {Ref<Facet>(new ContactFacet), Ref<FacetValue>(new ContactFacet::Value)},
                              });
    REQUIRE(mold);
    auto id = world->createForm(*mold);
    REQUIRE(id != 0);
    REQUIRE(world->initializeState(LifetimeState {}));
    REQUIRE(world->addLaw(createDynamicsLaw()));
    LifetimeOptions options;
    options.maximumPopulation = 0;
    REQUIRE(world->addLaw(createLifetimeLaw(universe, options)));
    auto before = world->primeSnapshot();
    REQUIRE(world->tick(UnitOfTime {10'000'000}));
    auto after = world->primeSnapshot();
    REQUIRE(after->getFacet<MotionFacet>(id));
    auto value = RuntimeType::cast<const TaggedMotionValue>(after->get<MotionFacet>(id));
    REQUIRE(value);
    CHECK(value->applicationTag() == 91);
    CHECK(value->linearVelocity.y < 0);
    CHECK(before->get<MotionFacet>(id)->linearVelocity.y == 0);
    CHECK(after->get<MotionFacet>(id) == static_cast<const MotionFacet::Value *>(value));
}

namespace {
class SpatialEditLaw : public Law {
    std::function<void(LawContext &)> edit;

public:
    GN_REGISTER_RUNTIME_TYPE(Law);
    explicit SpatialEditLaw(std::function<void(LawContext &)> action): Law(TYPE_INFO(), "spatial-edit"), edit(std::move(action)) {}
    LawContract contract() const override { return {"spatial-edit", {&TransformFacet::TYPE_INFO()}, {}, true, {}}; }
    void        tick(LawContext & c) override { edit(c); }
};

FormId addSpatialForm(World & world, Universe & universe, const TransformFacet::Value & value) {
    auto mold = Mold::create(universe, "spatial", {{Ref<Facet>(new TransformFacet), value.clone()}});
    return mold ? world.createForm(*mold) : 0;
}
} // namespace

TEST_CASE("E2 spatial hierarchy: local rotation composes without losing a far absolute origin", "[e2][world-dynamics]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world->registerFacet(TransformFacet()));
    TransformFacet::Value root;
    root.position    = {WorldCoordinate(int64_t(1) << 40, 123), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()};
    root.orientation = glm::angleAxis(1.5707963267948966f, glm::vec3(0, 0, 1));
    auto parent      = addSpatialForm(*world, universe, root);
    REQUIRE(parent != 0);
    TransformFacet::Value child;
    child.parent   = parent;
    child.position = positionFromMeters({2, 0, 0}, PhysicalScale::NANOMETER());
    auto id        = addSpatialForm(*world, universe, child);
    REQUIRE(id != 0);
    REQUIRE(world->addLaw(Ref<Law>(new SpatialEditLaw([id](LawContext & c) {
        auto value        = c.prime.get<TransformFacet>(id)->clone();
        auto transform    = RuntimeType::cast<TransformFacet::Value>(value.get());
        transform->parent = 0;
        c.slate.set<TransformFacet>(id, *transform);
    }))));
    auto           before = world->primeSnapshot();
    WorldTransform pose;
    REQUIRE(resolveWorldTransform(*before, parent, pose));
    CHECK(pose.position == root.position);
    REQUIRE(resolveWorldTransform(*before, id, pose));
    auto relative = positionToMeters(pose.position, PhysicalScale::NANOMETER(), root.position);
    CHECK(std::abs(relative.x) < 1e-6);
    CHECK(std::abs(relative.y - 2) < 1e-6);
    CHECK(transformChildren(*before, parent) == DynaArray<FormId> {id});
    REQUIRE(world->tick(UnitOfTime {10'000'000}));
    auto after = world->primeSnapshot();
    REQUIRE(resolveWorldTransform(*after, id, pose));
    CHECK(pose.position == child.position);
    CHECK(transformChildren(*after, parent).empty());
    CHECK(before->get<TransformFacet>(id)->parent == parent);
}

TEST_CASE("E2 spatial hierarchy: invalid final chains reject while joint detach and deletion succeeds", "[e2][world-dynamics]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world->registerFacet(TransformFacet()));
    TransformFacet::Value root;
    auto                  parent = addSpatialForm(*world, universe, root);
    REQUIRE(parent != 0);
    TransformFacet::Value child;
    child.parent = parent;
    auto id      = addSpatialForm(*world, universe, child);
    REQUIRE(id != 0);
    int  operation = 0;
    bool succeeds  = false;
    SECTION("cycle") { operation = 0; }
    SECTION("parent deletion leaves dangling child") { operation = 1; }
    SECTION("parent facet removal leaves dangling child") { operation = 2; }
    SECTION("joint detach and parent deletion") {
        operation = 3;
        succeeds  = true;
    }
    REQUIRE(world->addLaw(Ref<Law>(new SpatialEditLaw([=](LawContext & c) {
        if (operation == 0) {
            auto value        = c.prime.get<TransformFacet>(parent)->clone();
            auto transform    = RuntimeType::cast<TransformFacet::Value>(value.get());
            transform->parent = id;
            c.slate.set<TransformFacet>(parent, *transform);
        } else if (operation == 2) {
            c.slate.remove<TransformFacet>(parent);
        } else {
            c.slate.destroy(parent);
            if (operation == 3) {
                auto value        = c.prime.get<TransformFacet>(id)->clone();
                auto transform    = RuntimeType::cast<TransformFacet::Value>(value.get());
                transform->parent = 0;
                c.slate.set<TransformFacet>(id, *transform);
            }
        }
    }))));
    CHECK(world->tick(UnitOfTime {10'000'000}) == succeeds);
    auto prime = world->primeSnapshot();
    REQUIRE(prime->form(id));
    CHECK(prime->tick() == (succeeds ? 1 : 0));
    CHECK(prime->get<TransformFacet>(id)->parent == (succeeds ? 0 : parent));
    CHECK(bool(prime->form(parent)) == !succeeds);
}

TEST_CASE("E2 spatial hierarchy: invalid initial parents and rotations are rejected", "[e2][world-dynamics]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world->registerFacet(TransformFacet()));
    TransformFacet::Value value;
    value.parent = 999999;
    CHECK(addSpatialForm(*world, universe, value) == 0);
    value.parent      = 0;
    value.orientation = Rotation(0, 0, 0, 0);
    CHECK(addSpatialForm(*world, universe, value) == 0);
}

TEST_CASE("E2 dynamics: moving parent does not apply its motion twice to a dynamic child", "[e2][world-dynamics]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(registerDynamicsFacets(*world));
    TransformFacet::Value root;
    root.position = positionFromMeters({0, 5, 0}, PhysicalScale::NANOMETER());
    MotionFacet::Value moving;
    moving.linearVelocity = {1, 0, 0};
    auto parent           = world->createForm(*createBoxMold(universe, root, moving));
    REQUIRE(parent != 0);
    TransformFacet::Value child;
    child.parent   = parent;
    child.position = positionFromMeters({2, 0, 0}, PhysicalScale::NANOMETER());
    auto id        = world->createForm(*createBoxMold(universe, child, {}));
    REQUIRE(id != 0);
    DynamicsOptions dynamics;
    dynamics.gravity = {0, 0, 0};
    REQUIRE(world->addLaw(createDynamicsLaw(dynamics)));
    LifetimeOptions lifetime;
    lifetime.maximumPopulation = 0;
    REQUIRE(world->initializeState(LifetimeState {}));
    REQUIRE(world->addLaw(createLifetimeLaw(universe, lifetime)));
    REQUIRE(world->tick(UnitOfTime {100'000'000}));
    auto           prime = world->primeSnapshot();
    WorldTransform pose;
    REQUIRE(resolveWorldTransform(*prime, parent, pose));
    CHECK(std::abs(positionToMeters(pose.position, dynamics.scale).x - 0.1) < 1e-6);
    REQUIRE(resolveWorldTransform(*prime, id, pose));
    CHECK(std::abs(positionToMeters(pose.position, dynamics.scale).x - 2) < 1e-6);
    CHECK(std::abs(positionToMeters(prime->get<TransformFacet>(id)->position, dynamics.scale).x - 1.9) < 1e-6);
}

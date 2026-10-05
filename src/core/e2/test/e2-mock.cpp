#include <catch2/catch_test_macros.hpp>
#include <garnet/GNengine2.h>
#include <stdexcept>
using namespace GN;
using namespace GN::e2;
namespace {
struct Counter : Facet {
    GN_REGISTER_RUNTIME_TYPE(Facet);
    struct Value : FacetValue {
        GN_REGISTER_RUNTIME_TYPE(FacetValue);
        int count = 0;
        Value(): FacetValue(TYPE_INFO()) {}
        Ref<FacetValue> clone() const override { return referenceTo(new Value(*this)); }
    };
    Counter(): Facet(TYPE_INFO()) {}
    explicit Counter(const RuntimeType::TypeInfo & type): Facet(type) {}
    const RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                    clone() const override { return referenceTo(new Counter(*this)); }
};

struct NeedsCounter : Facet {
    GN_REGISTER_RUNTIME_TYPE(Facet);
    struct Value : FacetValue {
        GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(): FacetValue(TYPE_INFO()) {}
        Ref<FacetValue> clone() const override { return referenceTo(new Value(*this)); }
    };
    NeedsCounter(): Facet(TYPE_INFO()) {}
    const RuntimeType::TypeInfo &            valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                               clone() const override { return referenceTo(new NeedsCounter(*this)); }
    DynaArray<const RuntimeType::TypeInfo *> requirements() const override { return {&Counter::TYPE_INFO()}; }
};

struct Add : Intent {
    GN_REGISTER_RUNTIME_TYPE(Intent);
    int amount;
    explicit Add(int v = 1): Intent(TYPE_INFO()), amount(v) {}
    Ref<Intent> clone() const override { return referenceTo(new Add(*this)); }
};

struct Changed : Event {
    GN_REGISTER_RUNTIME_TYPE(Event);
    int value;
    explicit Changed(int v): Event(TYPE_INFO()), value(v) {}
    Ref<Event> clone() const override { return referenceTo(new Changed(*this)); }
};

struct CounterLaw : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    FormId form;
    bool   fail, declareEvent;
    explicit CounterLaw(FormId f, bool failure = false, bool declared = true): Law(TYPE_INFO()), form(f), fail(failure), declareEvent(declared) {}
    LawContract contract() const override {
        return {"counter",
                {&Counter::TYPE_INFO()},
                {&Add::TYPE_INFO()},
                false,
                declareEvent ? DynaArray<const RuntimeType::TypeInfo *> {&Changed::TYPE_INFO()} : DynaArray<const RuntimeType::TypeInfo *> {}};
    }
    void tick(LawContext & c) override {
        Counter::Value v(*c.prime.get<Counter>(form));
        for (const auto & i : c.intents) v.count += RuntimeType::cast<const Add>(i)->amount;
        c.slate.set<Counter>(form, v);
        c.emit(Changed(v.count));
        if (fail) throw 1;
    }
};
Ref<Mold> counterMold(Universe & u) { return Mold::create(u, "counter", {{referenceTo(new Counter), referenceTo(new Counter::Value)}}); }
struct RemoveCounter : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    FormId id;
    explicit RemoveCounter(FormId f): Law(TYPE_INFO()), id(f) {}
    LawContract contract() const override { return {"remove", {&Counter::TYPE_INFO()}, {}, true}; }
    void        tick(LawContext & c) override { c.slate.remove<Counter>(id); }
};
} // namespace
TEST_CASE("E2 runtime: OO values preserve old versions and input copies", "[e2][world-runtime]") {
    Universe u;
    auto     w = World::create(u, "events", 2);
    REQUIRE(w->registerFacet(Counter {}));
    auto m  = counterMold(u);
    auto id = w->createForm(*m);
    REQUIRE(id != 0);
    REQUIRE(w->addLaw(referenceTo(new CounterLaw(id))));
    REQUIRE_FALSE(w->addLaw(referenceTo(new CounterLaw(id))));

    auto old = w->primeSnapshot();
    Add  request(3);
    REQUIRE(w->submit(request));
    request.amount = 100;
    REQUIRE(w->tick(UnitOfTime(10'000'000)));
    REQUIRE(w->tick(UnitOfTime(10'000'000)));
    REQUIRE(w->tick(UnitOfTime(10'000'000)));
    CHECK(old->get<Counter>(id)->count == 0);
    CHECK(w->primeSnapshot()->get<Counter>(id)->count == 3);
    CHECK(w->primeSnapshot()->form(id)->getFacet<Counter>(*old));
    uint64_t cursor = 0;
    auto     events = w->events(cursor);
    REQUIRE(events.size() == 2);
    CHECK(events[0].tick == 2);
    CHECK(events[1].tick == 3);
    CHECK(RuntimeType::cast<const Changed>(events[1].payload.get())->value == 3);
    CHECK(w->events(cursor).empty());
}
TEST_CASE("E2 runtime: failed or undeclared producer never publishes", "[e2][world-runtime]") {
    for (bool declared : {false, true}) {
        Universe u;
        auto     w = World::create(u);
        REQUIRE(w->registerFacet(Counter {}));
        auto m  = counterMold(u);
        auto id = w->createForm(*m);
        REQUIRE(w->addLaw(referenceTo(new CounterLaw(id, true, declared))));

        REQUIRE(w->submit(Add(4)));
        CHECK_FALSE(w->tick(UnitOfTime(10'000'000)));
        CHECK(w->halted());
        CHECK_FALSE(w->tick(UnitOfTime(10'000'000)));
        CHECK(w->primeSnapshot()->tick() == 0);
        CHECK(w->primeSnapshot()->get<Counter>(id)->count == 0);
        uint64_t cursor = 0;
        CHECK(w->events(cursor).empty());
    }
}
TEST_CASE("E2 runtime: requirements validate initialization and final composition", "[e2][world-runtime]") {
    Universe u;
    auto     w = World::create(u);
    REQUIRE(w->registerFacet(Counter {}));
    REQUIRE(w->registerFacet(NeedsCounter {}));
    auto bad = Mold::create(u, "missing", {{referenceTo(new NeedsCounter), referenceTo(new NeedsCounter::Value)}});
    CHECK_FALSE(bad);
    CHECK(w->primeSnapshot()->query<>().empty());
    auto good = Mold::create(
        u, "complete", {{referenceTo(new NeedsCounter), referenceTo(new NeedsCounter::Value)}, {referenceTo(new Counter), referenceTo(new Counter::Value)}});
    auto id = w->createForm(*good);
    REQUIRE(id != 0);
    REQUIRE(w->addLaw(referenceTo(new RemoveCounter(id))));

    CHECK_FALSE(w->tick(UnitOfTime(10'000'000)));
    CHECK(w->primeSnapshot()->get<Counter>(id));
    CHECK(w->primeSnapshot()->get<NeedsCounter>(id));
}

namespace {
struct RoutingState : FacetValue {
    GN_REGISTER_RUNTIME_TYPE(FacetValue);
    int total = 0;
    RoutingState(): FacetValue(TYPE_INFO()) {}
    Ref<FacetValue> clone() const override { return referenceTo(new RoutingState(*this)); }
};

struct Back : Intent {
    GN_REGISTER_RUNTIME_TYPE(Intent);
    int amount = 2;
    Back(): Intent(TYPE_INFO()) {}
    Ref<Intent> clone() const override { return referenceTo(new Back(*this)); }
};

struct Sender : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    Sender(): Law(TYPE_INFO()) {}
    LawContract contract() const override { return {"sender", {}, {&Back::TYPE_INFO()}, false}; }
    void        tick(LawContext & c) override {
        int amount = 1;
        for (const auto & intent : c.intents) amount += RuntimeType::cast<const Back>(intent)->amount;
        c.send(Add(amount));
    }
};

struct Receiver : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    Receiver(): Law(TYPE_INFO()) {}
    LawContract contract() const override { return {"receiver", {&RoutingState::TYPE_INFO()}, {&Add::TYPE_INFO()}, false}; }
    void        tick(LawContext & c) override {
        auto value = c.prime.state<RoutingState>()->clone();
        auto state = RuntimeType::cast<RoutingState>(value.get());
        for (const auto & intent : c.intents) state->total += RuntimeType::cast<const Add>(intent)->amount;
        c.slate.setState(*state);
        c.send(Back {});
        c.send(Add(3));
    }
};
} // namespace

TEST_CASE("E2 runtime: forward requests arrive now and backward/self requests next tick", "[e2][world-runtime]") {
    Universe u;
    auto     world = World::create(u);
    REQUIRE(world->registerState<RoutingState>());
    REQUIRE(world->initializeState(RoutingState {}));
    REQUIRE(world->addLaw(referenceTo(new Sender)));
    REQUIRE(world->addLaw(referenceTo(new Receiver)));

    auto initial = world->primeSnapshot();
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    CHECK(world->primeSnapshot()->state<RoutingState>()->total == 1);
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    CHECK(world->primeSnapshot()->state<RoutingState>()->total == 7);
    CHECK(initial->state<RoutingState>()->total == 0);
}

namespace {
struct BatchCompositionLaw : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    FormId id;
    explicit BatchCompositionLaw(FormId form): Law(TYPE_INFO()), id(form) {}
    LawContract contract() const override { return {"batch-composition", {&Counter::TYPE_INFO(), &NeedsCounter::TYPE_INFO()}, {}, true}; }
    void        tick(LawContext & c) override {
        if (c.prime.tick() == 0) {
            c.slate.add(id, NeedsCounter {}, NeedsCounter::Value {});
            c.slate.add(id, Counter {}, Counter::Value {});
        } else {
            c.slate.remove<Counter>(id);
            c.slate.remove<NeedsCounter>(id);
        }
    }
};

struct TaggedCounter : Counter {
    GN_REGISTER_RUNTIME_TYPE(Counter);
    TaggedCounter(): Counter(TYPE_INFO()) {}
    Ref<Facet> clone() const override { return referenceTo(new TaggedCounter(*this)); }
};

struct ExactOwnerLaw : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    const RuntimeType::TypeInfo & owned;
    FormId                        id;
    ExactOwnerLaw(const RuntimeType::TypeInfo & type, FormId form): Law(TYPE_INFO()), owned(type), id(form) {}
    LawContract contract() const override { return {"exact-owner", {&owned}, {}, false}; }
    void        tick(LawContext & c) override {
        Counter::Value value;
        value.count = 7;
        c.slate.set(id, owned, value);
    }
};
} // namespace

TEST_CASE("E2 runtime: dependency additions and removals validate the complete batch", "[e2][world-runtime]") {
    Universe u;
    auto     world = World::create(u);
    REQUIRE(world->registerFacet(Counter {}));
    REQUIRE(world->registerFacet(NeedsCounter {}));
    auto mold = Mold::create(u, "empty", {});
    auto id   = world->createForm(*mold);
    REQUIRE(world->addLaw(referenceTo(new BatchCompositionLaw(id))));

    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    auto populated = world->primeSnapshot();
    CHECK(populated->get<Counter>(id));
    CHECK(populated->get<NeedsCounter>(id));
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    CHECK_FALSE(world->primeSnapshot()->get<Counter>(id));
    CHECK_FALSE(world->primeSnapshot()->get<NeedsCounter>(id));
    CHECK(populated->get<Counter>(id));
}

TEST_CASE("E2 runtime: derived capabilities have independent exact simulation contracts", "[e2][world-runtime]") {
    Universe u;
    auto     world = World::create(u);
    REQUIRE(world->registerFacet(Counter {}));
    REQUIRE(world->registerFacet(TaggedCounter {}));
    auto base      = counterMold(u);
    auto derived   = Mold::create(u, "derived", {{referenceTo(new TaggedCounter), referenceTo(new Counter::Value)}});
    auto baseId    = world->createForm(*base);
    auto derivedId = world->createForm(*derived);
    REQUIRE(baseId != 0);
    REQUIRE(derivedId != 0);
    REQUIRE(world->addLaw(referenceTo(new ExactOwnerLaw(Counter::TYPE_INFO(), baseId))));
    REQUIRE(world->addLaw(referenceTo(new ExactOwnerLaw(TaggedCounter::TYPE_INFO(), derivedId))));

    auto initial      = world->primeSnapshot();
    auto baseQuery    = initial->query<Counter>();
    auto derivedQuery = initial->query<TaggedCounter>();
    REQUIRE(baseQuery.size() == 1);
    CHECK(baseQuery.front() == baseId);
    REQUIRE(derivedQuery.size() == 1);
    CHECK(derivedQuery.front() == derivedId);
    CHECK(initial->getFacet<Counter>(derivedId));
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    CHECK(world->primeSnapshot()->get<Counter>(baseId)->count == 7);
    CHECK(world->primeSnapshot()->get<TaggedCounter>(derivedId)->count == 7);
}

namespace {
struct ValidatedCounter : Counter {
    GN_REGISTER_RUNTIME_TYPE(Counter);
    ValidatedCounter(): Counter(TYPE_INFO()) {}
    Ref<Facet> clone() const override { return referenceTo(new ValidatedCounter(*this)); }
    bool       validate(const PrimeView & candidate, FormId owner, const FacetValue & value) const override {
        auto typed = RuntimeType::cast<const Counter::Value>(&value);
        return candidate.form(owner) && typed && typed->count >= 0;
    }
};

struct InvalidCandidateLaw : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    FormId id;
    explicit InvalidCandidateLaw(FormId target): Law(TYPE_INFO()), id(target) {}
    LawContract contract() const override { return {"invalid-candidate", {&ValidatedCounter::TYPE_INFO()}, {}, false}; }
    void        tick(LawContext & context) override {
        Counter::Value value;
        value.count = -1;
        context.slate.set<ValidatedCounter>(id, value);
    }
};
} // namespace

TEST_CASE("E2 runtime: Facet validates candidate through base protocol before publication", "[e2][world-runtime]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world->registerFacet(ValidatedCounter {}));
    auto negative   = referenceTo(new Counter::Value);
    negative->count = -1;
    auto bad        = Mold::create(universe, "invalid-domain-state", {{referenceTo(new ValidatedCounter), negative}});
    REQUIRE(bad);
    CHECK(world->createForm(*bad) == 0);
    CHECK(world->primeSnapshot()->query<>().empty());
    auto good = Mold::create(universe, "valid-domain-state", {{referenceTo(new ValidatedCounter), referenceTo(new Counter::Value)}});
    auto id   = world->createForm(*good);
    REQUIRE(id != 0);
    REQUIRE(world->addLaw(referenceTo(new InvalidCandidateLaw(id))));

    auto previous = world->primeSnapshot();
    CHECK_FALSE(world->tick(UnitOfTime(10'000'000)));
    CHECK(world->halted());
    CHECK(world->primeSnapshot()->tick() == 0);
    CHECK(world->primeSnapshot()->get<ValidatedCounter>(id)->count == 0);
    CHECK(previous->get<ValidatedCounter>(id)->count == 0);
}

namespace {
struct LateDomain : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    LateDomain(): Law(TYPE_INFO()) {}
    LawContract contract() const override { return {"late-domain", {&Counter::TYPE_INFO(), &RoutingState::TYPE_INFO()}, {&Add::TYPE_INFO()}, false}; }
    void        tick(LawContext & context) override {
        auto original = context.prime.state<RoutingState>();
        if (!original) throw std::runtime_error("late domain ran before its initial state was published");
        auto value = original->clone();
        auto state = RuntimeType::cast<RoutingState>(value.get());
        for (const auto & request : context.intents) state->total += RuntimeType::cast<const Add>(request)->amount;
        context.slate.setState(*state);
        for (auto formId : context.prime.query<Counter>()) {
            auto copy    = context.prime.get<Counter>(formId)->clone();
            auto counter = RuntimeType::cast<Counter::Value>(copy.get());
            ++counter->count;
            context.slate.set<Counter>(formId, *counter);
        }
    }
};

struct LiveInstaller : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    World &    world;
    Universe & universe;
    FormId     created = 0;
    LiveInstaller(World & w, Universe & u): Law(TYPE_INFO()), world(w), universe(u) {}
    LawContract contract() const override { return {"installer", {}, {}, false}; }
    void        tick(LawContext & context) override {
        if (context.prime.tick() != 0) return;
        if (!world.registerFacet(Counter {}) || !world.registerState<RoutingState>() || !world.initializeState(RoutingState {})) throw 1;
        auto mold = Mold::create(universe, "live-empty", {});
        created   = world.createForm(*mold);
        if (!created || !world.addFacet(created, Counter {}, Counter::Value {}) || !world.addLaw(referenceTo(new LateDomain)) || !world.submit(Add(9))) throw 1;
    }
};
} // namespace

TEST_CASE("E2 runtime: live additions publish before the new domain first executes", "[e2][world-runtime]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    auto previous = world->primeSnapshot();
    REQUIRE(world->registerFacet(Counter {}));
    REQUIRE(world->registerState<RoutingState>());
    REQUIRE(world->initializeState(RoutingState {}));
    CHECK_FALSE(world->initializeState(RoutingState {}));
    auto mold = Mold::create(universe, "late-empty", {});
    auto id   = world->createForm(*mold);
    REQUIRE(id != 0);
    REQUIRE(world->addFacet(id, Counter {}, Counter::Value {}));
    CHECK_FALSE(world->addFacet(id, Counter {}, Counter::Value {}));
    REQUIRE(world->addLaw(referenceTo(new LateDomain)));
    REQUIRE(world->submit(Add(5)));
    CHECK_FALSE(previous->form(id));
    CHECK_FALSE(world->primeSnapshot()->form(id));
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    CHECK(world->primeSnapshot()->get<Counter>(id)->count == 0);
    CHECK(world->primeSnapshot()->state<RoutingState>()->total == 0);
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    CHECK(world->primeSnapshot()->get<Counter>(id)->count == 1);
    CHECK(world->primeSnapshot()->state<RoutingState>()->total == 5);
    CHECK_FALSE(previous->form(id));
}

TEST_CASE("E2 runtime: registration inside a running Law respects the captured tick boundary", "[e2][world-runtime]") {
    Universe universe;
    auto     world     = World::create(universe);
    auto     installer = referenceTo(new LiveInstaller(*world, universe));
    REQUIRE(world->addLaw(installer));
    auto original = world->primeSnapshot();
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    REQUIRE(installer->created != 0);
    CHECK_FALSE(world->primeSnapshot()->form(installer->created));
    CHECK_FALSE(world->primeSnapshot()->state<RoutingState>());
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    CHECK(world->primeSnapshot()->get<Counter>(installer->created)->count == 0);
    CHECK(world->primeSnapshot()->state<RoutingState>()->total == 0);
    REQUIRE(world->tick(UnitOfTime(10'000'000)));
    CHECK(world->primeSnapshot()->get<Counter>(installer->created)->count == 1);
    CHECK(world->primeSnapshot()->state<RoutingState>()->total == 9);
    CHECK_FALSE(original->form(installer->created));
}

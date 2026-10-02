#include <catch2/catch_test_macros.hpp>
#include <garnet/GNengine2.h>

// This extension deliberately uses only the monolithic public API, as an app would.
namespace {
using namespace GN::e2;
struct Charge : Facet {
    GN_REGISTER_RUNTIME_TYPE(Facet);
    struct Value : FacetValue {
        GN_REGISTER_RUNTIME_TYPE(FacetValue);
        int amount = 0;
        explicit Value(int n = 0): FacetValue(TYPE_INFO()), amount(n) {}
        Ref<FacetValue> clone() const override { return Ref<FacetValue>(new Value(*this)); }

    protected:
        Value(const GN::RuntimeType::TypeInfo & type, int n): FacetValue(type), amount(n) {}
    };

    Charge(): Facet(TYPE_INFO()) {}
    const GN::RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                        clone() const override { return Ref<Facet>(new Charge(*this)); }
    int                               doubled(const Value & v) const { return v.amount * 2; }
};

struct BonusValue : Charge::Value {
    GN_REGISTER_RUNTIME_TYPE(Charge::Value);
    int bonus = 7;
    explicit BonusValue(int n): Value(TYPE_INFO(), n) {}
    Ref<FacetValue> clone() const override { return Ref<FacetValue>(new BonusValue(*this)); }
    int             total() const { return amount + bonus; }
};

struct Added : Facet {
    GN_REGISTER_RUNTIME_TYPE(Facet);
    struct Value : FacetValue {
        GN_REGISTER_RUNTIME_TYPE(FacetValue);
        int amount = 3;
        Value(): FacetValue(TYPE_INFO()) {}
        Ref<FacetValue> clone() const override { return Ref<FacetValue>(new Value(*this)); }
    };

    Added(): Facet(TYPE_INFO()) {}
    const GN::RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                        clone() const override { return Ref<Facet>(new Added(*this)); }
};

struct Audit : FacetValue {
    GN_REGISTER_RUNTIME_TYPE(FacetValue);
    int observed = 0;
    Audit(): FacetValue(TYPE_INFO()) {}
    Ref<FacetValue> clone() const override { return Ref<FacetValue>(new Audit(*this)); }
};

struct AppForm : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);
    explicit AppForm(FormId id): Form(TYPE_INFO(), id, "app-form") {}
    FormId formId() const override { return id; }
    int    customMethod() const { return 42; }
};

struct ChargeLaw final : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    ChargeLaw(): Law(TYPE_INFO()) {}
    LawContract contract() const override { return {"charge", {&Charge::TYPE_INFO()}, {}, false}; }
    void        tick(LawContext & c) override {
        for (auto id : c.prime.query<Charge>()) {
            auto next  = c.prime.get<Charge>(id)->clone();
            auto value = GN::RuntimeType::cast<Charge::Value>(next.get());
            ++value->amount;
            c.slate.set<Charge>(id, *value);
        }
    }
};

struct AuditLaw final : Law {
    GN_REGISTER_RUNTIME_TYPE(Law);
    AuditLaw(): Law(TYPE_INFO()) {}
    LawContract contract() const override { return {"audit", {&Audit::TYPE_INFO(), &Added::TYPE_INFO()}, {}, true}; }
    void        tick(LawContext & c) override {
        Audit audit;
        for (auto id : c.prime.query<Charge>()) {
            audit.observed += c.prime.get<Charge>(id)->amount;
            if (c.prime.get<Added>(id))
                c.slate.remove<Added>(id);
            else
                c.slate.add(id, Added {}, Added::Value {});
        }
        c.slate.setState(audit);
    }
};

} // namespace
TEST_CASE("E2 app extension: OO subtypes survive publication and laws share tick-start Prime", "[e2][world-runtime]") {
    Universe universe;
    auto     world = World::create(universe);
    REQUIRE(world->registerFacet(Charge {}));
    REQUIRE(world->registerFacet(Added {}));
    REQUIRE(world->registerState<Audit>());
    REQUIRE(world->initializeState(Audit {}));
    auto input = Ref<BonusValue>(new BonusValue(9));
    auto mold  = Mold::create(universe, "app-charge", {{Ref<Facet>(new Charge), input}}, [](FormId id) { return Ref<Form>(new AppForm(id)); });
    REQUIRE(mold);
    auto id = world->createForm(*mold);
    REQUIRE(id);
    input->amount = 100; // Retained caller-owned data must not mutate committed values.
    REQUIRE(world->addLaw(Ref<Law>(new ChargeLaw)));
    REQUIRE(world->addLaw(Ref<Law>(new AuditLaw)));
    CHECK_FALSE(world->registerFacet(Added {}));
    auto initial = world->primeSnapshot();
    auto appForm = GN::RuntimeType::cast<const AppForm>(initial->form(id));
    REQUIRE(appForm);
    CHECK(appForm->customMethod() == 42);
    CHECK(appForm->getFacet<Charge>(*initial)->doubled(*initial->get<Charge>(id)) == 18);
    auto oldBonus = GN::RuntimeType::cast<const BonusValue>(initial->get<Charge>(id));
    REQUIRE(oldBonus);
    CHECK(oldBonus->total() == 16);
    REQUIRE(world->tick(UnitOfTime {10'000'000}));
    auto first = world->primeSnapshot();
    CHECK(first->get<Charge>(id)->amount == 10);
    auto newBonus = GN::RuntimeType::cast<const BonusValue>(first->get<Charge>(id));
    REQUIRE(newBonus);
    CHECK(newBonus->total() == 17);
    CHECK(first->state<Audit>()->observed == 9);
    CHECK(first->getFacet<Added>(id));
    CHECK_FALSE(initial->getFacet<Added>(id));
    REQUIRE(world->tick(UnitOfTime {10'000'000}));
    auto second = world->primeSnapshot();
    CHECK(second->state<Audit>()->observed == 10);
    CHECK_FALSE(second->getFacet<Added>(id));
    CHECK(first->get<Added>(id)->amount == 3);
    CHECK(initial->get<Charge>(id)->amount == 9);
}

#include <garnet/GNengine2.h>
#include <algorithm>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>

using namespace GN;
using namespace GN::e2;
namespace {
using TypeId = uint64_t;

// DynaArray reports allocation failure instead of throwing; preserve tick abort semantics.
template<typename T, typename V>
void appendOrThrow(DynaArray<T> & array, V && value) {
    if (!array.append(std::forward<V>(value))) throw std::bad_alloc();
}

struct Entry {
    Ref<const Facet>      facet;
    Ref<const FacetValue> value;
};

struct Data {
    uint64_t                                  tick = 0;
    std::map<FormId, Ref<const Form>>         forms;
    std::map<TypeId, std::map<FormId, Entry>> columns;
    std::map<TypeId, Ref<const FacetValue>>   state;
};

template<typename T>
Ref<const T> snapshot(const T & source) {
    auto copy = source.clone();
    if (!copy || copy.get() == &source || copy->typeInfo().id != source.typeInfo().id)
        throw std::runtime_error("clone must preserve complete dynamic type in a new object");
    return copy;
}
struct FormImpl : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);
    FormId key;
    FormImpl(FormId id, const StrA & name): Form(TYPE_INFO(), id, name), key(id) {}
    FormId formId() const override { return key; }
};

struct PrimeImpl : PrimeView {
    GN_REGISTER_RUNTIME_TYPE(PrimeView);
    std::shared_ptr<const Data> data;
    explicit PrimeImpl(std::shared_ptr<const Data> d): PrimeView(TYPE_INFO(), 0, "prime"), data(std::move(d)) {}
    uint64_t        tick() const override { return data->tick; }
    Ref<const Form> form(FormId formId) const override {
        auto i = data->forms.find(formId);
        return i == data->forms.end() ? Ref<const Form> {} : i->second;
    }
    const Entry * entry(FormId formId, const RuntimeType::TypeInfo & type) const {
        auto exact = data->columns.find(type.id);
        if (exact != data->columns.end()) {
            auto item = exact->second.find(formId);
            if (item != exact->second.end()) return &item->second;
        }
        const Entry * result = nullptr;
        for (const auto & column : data->columns) {
            auto i = column.second.find(formId);
            if (i == column.second.end() || !i->second.facet->typeInfo().isDerivedFrom(type)) continue;
            if (result) return nullptr; // A base lookup is ambiguous when multiple sibling capabilities match.
            result = &i->second;
        }
        return result;
    }
    Ref<const Facet> facet(FormId formId, const RuntimeType::TypeInfo & type) const override {
        auto e = entry(formId, type);
        return e ? e->facet : Ref<const Facet> {};
    }
    Ref<const FacetValue> value(FormId formId, const RuntimeType::TypeInfo & type) const override {
        auto e = entry(formId, type);
        return e ? e->value : Ref<const FacetValue> {};
    }
    Ref<const FacetValue> state(const RuntimeType::TypeInfo & type) const override {
        auto i = data->state.find(type.id);
        return i == data->state.end() ? Ref<const FacetValue> {} : i->second;
    }
    DynaArray<FormId> query(const DynaArray<const RuntimeType::TypeInfo *> & types) const override {
        DynaArray<FormId> result;
        for (const auto & f : data->forms) {
            bool match = true;
            for (auto t : types)
                if (!t || !data->columns.count(t->id) || !data->columns.at(t->id).count(f.first)) {
                    match = false;
                    break;
                }
            if (match) appendOrThrow(result, f.first);
        }
        return result;
    }
};

struct MoldImpl : Mold {
    GN_REGISTER_RUNTIME_TYPE(Mold);
    DynaArray<FacetBinding> bindings;
    FormFactory             factory;
    MoldImpl(Universe & u, const StrA & name, DynaArray<FacetBinding> b, FormFactory f)
        : Mold(TYPE_INFO(), u.generateUniqueIdentifier(), name), bindings(std::move(b)), factory(std::move(f)) {}
    const DynaArray<FacetBinding> & facets() const override { return bindings; }
    Ref<Form>                       createForm(FormId formId) const override { return factory ? factory(formId) : Form::create(formId); }
};

struct Registration {
    const RuntimeType::TypeInfo * type;
    const RuntimeType::TypeInfo * value;
    DeletionCleanup               cleanup;
};
void validateComposition(const Data & data) {
    for (const auto & form : data.forms) {
        for (const auto & column : data.columns) {
            auto item = column.second.find(form.first);
            if (item == column.second.end()) continue;
            for (auto required : item->second.facet->requirements()) {
                bool found = false;
                if (required)
                    for (const auto & provider : data.columns) {
                        auto value = provider.second.find(form.first);
                        if (value != provider.second.end() && value->second.facet->typeInfo().isDerivedFrom(*required)) {
                            found = true;
                            break;
                        }
                    }
                if (!found) throw std::runtime_error("Facet requirement missing from final Form composition");
            }
        }
    }
    auto view = referenceTo(new PrimeImpl(std::make_shared<Data>(data)));
    for (const auto & column : data.columns)
        for (const auto & item : column.second)
            if (!item.second.facet->validate(*view, item.first, *item.second.value)) throw std::runtime_error("Facet rejected final candidate state");
}
struct Registry {
    std::map<TypeId, Registration>                  facets;
    std::map<TypeId, const RuntimeType::TypeInfo *> states;
    std::map<TypeId, size_t>                        owners, receivers;
    DynaArray<Ref<Law>>                             laws;
    DynaArray<LawContract>                          contracts;
    DynaArray<uint64_t>                             readyAfterTick;
};

struct Runtime : World, Registry {
    GN_REGISTER_RUNTIME_TYPE(World);
    Universe &                              universe;
    mutable std::mutex                      access;
    std::mutex                              advance;
    std::shared_ptr<const Data>             published = std::make_shared<Data>();
    DynaArray<DynaArray<Ref<const Intent>>> pending;
    DynaArray<Ref<const Intent>>            inbox;
    std::deque<PublishedEvent>              history;
    size_t                                  capacity;
    uint64_t                                eventSequence = 0;
    bool                                    started = false, stopped = false, advancing = false;
    StrA                                    failure;
    Data                                    queued;
    std::set<FormId>                        reservedForms;
    std::set<std::pair<FormId, TypeId>>     reservedAttachments;
    std::set<TypeId>                        reservedStates;
    Runtime(Universe & u, const StrA & name, size_t c): World(TYPE_INFO(), u.generateUniqueIdentifier(), name), universe(u), capacity(c) {}
    bool registerFacet(const Facet & f, DeletionCleanup cleanup) override {
        std::lock_guard<std::mutex> g(access);
        auto                        typeId = f.typeInfo().id;
        if (stopped || facets.count(typeId) || states.count(typeId) || !f.valueType().isDerivedFrom(FacetValue::TYPE_INFO())) return false;
        facets.emplace(typeId, Registration {&f.typeInfo(), &f.valueType(), std::move(cleanup)});
        return true;
    }
    bool registerState(const RuntimeType::TypeInfo & type) override {
        std::lock_guard<std::mutex> g(access);
        if (stopped || facets.count(type.id) || states.count(type.id) || !type.isDerivedFrom(FacetValue::TYPE_INFO())) return false;
        states[type.id] = &type;
        return true;
    }
    bool addLaw(Ref<Law> law) override {
        std::lock_guard<std::mutex> g(access);
        if (stopped || !law) return false;
        auto             c = law->contract();
        std::set<TypeId> writes, intents;
        for (auto t : c.writes)
            if (!t || (!facets.count(t->id) && !states.count(t->id)) || owners.count(t->id) || !writes.insert(t->id).second) return false;
        for (auto t : c.intents)
            if (!t || !t->isDerivedFrom(Intent::TYPE_INFO()) || receivers.count(t->id) || !intents.insert(t->id).second) return false;
        for (auto t : c.events)
            if (!t || !t->isDerivedFrom(Event::TYPE_INFO())) return false;
        auto index = laws.size();
        for (auto t : c.writes) owners[t->id] = index;
        for (auto t : c.intents) receivers[t->id] = index;
        appendOrThrow(contracts, std::move(c));
        appendOrThrow(laws, std::move(law));
        appendOrThrow(readyAfterTick, started ? published->tick + (advancing ? 2 : 1) : uint64_t(0));
        if (!pending.emplace()) throw std::bad_alloc();
        return true;
    }
    Entry binding(const Facet & f, const FacetValue & v, const Registry & registry) const {
        auto i = registry.facets.find(f.typeInfo().id);
        if (i == registry.facets.end() || !v.typeInfo().isDerivedFrom(*i->second.value)) throw std::runtime_error("invalid capability/value binding");
        return {snapshot(f), snapshot(v)};
    }
    FormId createIn(Data & data, const Mold & mold, const Registry & registry) {
        auto formId = universe.generateUniqueIdentifier();
        auto form   = mold.createForm(formId);
        if (!form || form->formId() != formId) throw std::runtime_error("Mold factory must preserve reserved Form identity");
        std::map<TypeId, Entry> values;
        for (const auto & b : mold.facets()) {
            if (!b.facet || !b.value || values.count(b.facet->typeInfo().id)) throw std::runtime_error("invalid Mold binding");
            values.emplace(b.facet->typeInfo().id, binding(*b.facet, *b.value, registry));
        }
        data.forms[formId] = form;
        for (auto & v : values) data.columns[v.first][formId] = std::move(v.second);
        return formId;
    }
    FormId createForm(const Mold & mold) override {
        std::lock_guard<std::mutex> guard(access);
        if (stopped) return 0;
        try {
            if (!started) {
                auto next   = std::make_shared<Data>(*published);
                auto formId = createIn(*next, mold, *this);
                validateComposition(*next);
                published = next;
                return formId;
            }
            Data addition;
            auto formId = createIn(addition, mold, *this);
            queued.forms.insert(addition.forms.begin(), addition.forms.end());
            for (auto & column : addition.columns) {
                queued.columns[column.first].insert(column.second.begin(), column.second.end());
                reservedAttachments.emplace(formId, column.first);
            }
            reservedForms.insert(formId);
            return formId;
        } catch (...) { return 0; }
    }
    bool addFacet(FormId formId, const Facet & facet, const FacetValue & value) override {
        std::lock_guard<std::mutex> guard(access);
        auto                        type = facet.typeInfo().id;
        if (stopped || (!published->forms.count(formId) && !reservedForms.count(formId)) || reservedAttachments.count({formId, type})) return false;
        auto column = published->columns.find(type);
        if (column != published->columns.end() && column->second.count(formId)) return false;
        try {
            auto entry = binding(facet, value, *this);
            if (!started) {
                auto next                   = std::make_shared<Data>(*published);
                next->columns[type][formId] = std::move(entry);
                validateComposition(*next);
                published = next;
            } else {
                queued.columns[type][formId] = std::move(entry);
                reservedAttachments.emplace(formId, type);
            }
            return true;
        } catch (...) { return false; }
    }
    bool initializeState(const FacetValue & value) override {
        std::lock_guard<std::mutex> guard(access);
        auto                        type = value.typeInfo().id;
        if (stopped || !states.count(type) || published->state.count(type) || reservedStates.count(type)) return false;
        try {
            auto copy = snapshot(value);
            if (!started) {
                auto next         = std::make_shared<Data>(*published);
                next->state[type] = std::move(copy);
                published         = next;
            } else {
                queued.state[type] = std::move(copy);
                reservedStates.insert(type);
            }
            return true;
        } catch (...) { return false; }
    }
    bool halted() const override {
        std::lock_guard<std::mutex> g(access);
        return stopped;
    }
    StrA error() const override {
        std::lock_guard<std::mutex> g(access);
        return failure;
    }
    Ref<PrimeView> primeSnapshot() const override {
        std::lock_guard<std::mutex> g(access);
        return referenceTo(new PrimeImpl(published));
    }
    bool submit(const Intent & intent) override {
        std::lock_guard<std::mutex> g(access);
        if (stopped || !receivers.count(intent.typeInfo().id)) return false;
        try {
            appendOrThrow(inbox, snapshot(intent));
            return true;
        } catch (...) { return false; }
    }
    DynaArray<PublishedEvent> events(uint64_t & cursor) const override {
        std::lock_guard<std::mutex> g(access);
        DynaArray<PublishedEvent>   result;
        for (const auto & e : history)
            if (e.sequence > cursor) appendOrThrow(result, e);
        if (!result.empty()) cursor = result.back().sequence;
        return result;
    }
    bool tick(UnitOfTime) override;
};

struct TickSlate : Slate {
    Runtime &         runtime;
    const Registry &  registry;
    const Data &      base;
    Data &            candidate;
    size_t            law = 0;
    std::set<FormId>  created;
    DynaArray<FormId> deleted;
    TickSlate(Runtime & r, const Registry & reg, const Data & b, Data & c): runtime(r), registry(reg), base(b), candidate(c) {}
    void structural() const {
        if (!registry.contracts[law].structural) throw std::runtime_error("Law lacks structural authority");
    }
    void owns(TypeId type) const {
        auto owner = registry.owners.find(type);
        if (owner == registry.owners.end() || owner->second != law) throw std::runtime_error("Law lacks exact concrete state ownership");
    }
    void target(FormId id) const {
        if (!base.forms.count(id) && !created.count(id)) throw std::runtime_error("invalid Form target");
    }
    FormId create(const Mold & m) override {
        structural();
        auto id = runtime.createIn(candidate, m, registry);
        created.insert(id);
        return id;
    }
    void destroy(FormId id) override {
        structural();
        if (candidate.forms.count(id) && std::find(deleted.begin(), deleted.end(), id) == deleted.end()) appendOrThrow(deleted, id);
    }
    void set(FormId id, const RuntimeType::TypeInfo & type, const FacetValue & value) override {
        owns(type.id);
        target(id);
        auto column = candidate.columns.find(type.id);
        if (column == candidate.columns.end() || !column->second.count(id)) throw std::runtime_error("set requires exact attached Facet");
        auto & entry = column->second.at(id);
        if (!value.typeInfo().isDerivedFrom(entry.facet->valueType())) throw std::runtime_error("wrong Facet value type");
        entry.value = snapshot(value);
    }
    void add(FormId id, const Facet & f, const FacetValue & v) override {
        structural();
        owns(f.typeInfo().id);
        target(id);
        if (candidate.columns[f.typeInfo().id].count(id)) throw std::runtime_error("Facet already attached");
        candidate.columns[f.typeInfo().id][id] = runtime.binding(f, v, registry);
    }
    void remove(FormId id, const RuntimeType::TypeInfo & type) override {
        structural();
        owns(type.id);
        target(id);
        if (!registry.facets.count(type.id)) throw std::runtime_error("not a registered Facet type");
        auto column = candidate.columns.find(type.id);
        if (column != candidate.columns.end()) column->second.erase(id);
    }
    void setState(const FacetValue & v) override {
        auto id = v.typeInfo().id;
        owns(id);
        if (!registry.states.count(id)) throw std::runtime_error("not world state");
        candidate.state[id] = snapshot(v);
    }
};

struct Context : LawContext {
    const Registry &                          registry;
    size_t                                    index;
    DynaArray<DynaArray<Ref<const Intent>>> & current;
    DynaArray<DynaArray<Ref<const Intent>>> & next;
    DynaArray<Ref<const Event>> &             output;
    Context(const PrimeView & p, Slate & s, UnitOfTime dt, const DynaArray<Ref<const Intent>> & i, const Registry & r, size_t n,
            DynaArray<DynaArray<Ref<const Intent>>> & c, DynaArray<DynaArray<Ref<const Intent>>> & future, DynaArray<Ref<const Event>> & o)
        : LawContext(p, s, dt, i), registry(r), index(n), current(c), next(future), output(o) {}
    void send(const Intent & intent) override {
        auto receiver = registry.receivers.find(intent.typeInfo().id);
        if (receiver == registry.receivers.end()) throw std::runtime_error("Intent receiver absent from tick registry");
        auto target = receiver->second;
        appendOrThrow(target > index ? current[target] : next[target], snapshot(intent));
    }
    void emit(const Event & event) override {
        bool allowed = false;
        for (auto type : registry.contracts[index].events)
            if (type->id == event.typeInfo().id) {
                allowed = true;
                break;
            }
        if (!allowed) throw std::runtime_error("undeclared Event producer");
        appendOrThrow(output, snapshot(event));
    }
};
bool Runtime::tick(UnitOfTime dt) {
    std::lock_guard<std::mutex> simulation(advance);
    try {
        std::shared_ptr<const Data>             base;
        DynaArray<DynaArray<Ref<const Intent>>> current, future;
        Registry                                registry;
        Data                                    additions;
        {
            std::lock_guard<std::mutex> g(access);
            if (stopped) return false;
            if (dt.count() <= 0) throw std::runtime_error("invalid timestep");
            started   = true;
            advancing = true;
            registry  = static_cast<const Registry &>(*this);
            additions = std::move(queued);
            queued    = Data {};
            base      = published;
            current   = std::move(pending);
            if (!pending.resize(laws.size())) throw std::bad_alloc();
            for (auto & i : inbox) appendOrThrow(current[receivers.at(i->typeInfo().id)], std::move(i));
            if (!inbox.empty()) inbox.clear();
        }
        auto candidate = std::make_shared<Data>(*base);
        for (const auto & form : additions.forms) candidate->forms.emplace(form.first, form.second);
        for (const auto & column : additions.columns)
            for (const auto & item : column.second) {
                if (!candidate->forms.count(item.first) || candidate->columns[column.first].count(item.first))
                    throw std::runtime_error("queued attachment target is absent or already attached");
                candidate->columns[column.first][item.first] = item.second;
            }
        for (const auto & state : additions.state) {
            if (candidate->state.count(state.first)) throw std::runtime_error("queued state is already initialized");
            candidate->state[state.first] = state.second;
        }
        if (!future.resize(registry.laws.size())) throw std::bad_alloc();
        auto                        view = referenceTo(new PrimeImpl(base));
        TickSlate                   slate(*this, registry, *base, *candidate);
        DynaArray<Ref<const Event>> output;
        for (size_t n = 0; n < registry.laws.size(); ++n) {
            if (base->tick < registry.readyAfterTick[n]) {
                for (const auto & request : current[n]) appendOrThrow(future[n], request);
                continue;
            }
            slate.law = n;
            Context c(*view, slate, dt, current[n], registry, n, current, future, output);
            registry.laws[n]->tick(c);
        }
        for (auto formId : slate.deleted) {
            candidate->forms.erase(formId);
            for (auto & c : candidate->columns) c.second.erase(formId);
        }
        if (!slate.deleted.empty())
            for (auto & c : candidate->columns)
                for (auto & e : c.second) {
                    auto & cleanup = registry.facets.at(c.first).cleanup;
                    if (!cleanup) continue;
                    auto result = cleanup(e.first, *e.second.value, slate.deleted);
                    if (!result || !result->typeInfo().isDerivedFrom(e.second.facet->valueType())) throw std::runtime_error("invalid cleanup value");
                    e.second.value = snapshot(*result);
                }
        candidate->tick = base->tick + 1;
        validateComposition(*candidate);
        std::lock_guard<std::mutex> g(access);
        auto                        nextHistory = history;
        auto                        sequence    = eventSequence;
        for (auto & e : output) {
            nextHistory.push_back({candidate->tick, ++sequence, std::move(e)});
            while (nextHistory.size() > capacity) nextHistory.pop_front();
        }
        for (size_t n = 0; n < future.size(); ++n)
            for (const auto & request : future[n]) appendOrThrow(pending[n], request);
        for (const auto & form : additions.forms) reservedForms.erase(form.first);
        for (const auto & column : additions.columns)
            for (const auto & item : column.second) reservedAttachments.erase({item.first, column.first});
        for (const auto & state : additions.state) reservedStates.erase(state.first);
        history.swap(nextHistory);
        eventSequence = sequence;
        published     = candidate;
        advancing     = false;
        return true;
    } catch (const std::exception & e) {
        std::lock_guard<std::mutex> g(access);
        stopped   = true;
        advancing = false;
        queued    = Data {};
        reservedForms.clear();
        reservedAttachments.clear();
        reservedStates.clear();
        failure = e.what();
        if (!inbox.empty()) inbox.clear();
        if (!pending.empty()) pending.clear();
        return false;
    } catch (...) {
        std::lock_guard<std::mutex> g(access);
        stopped   = true;
        advancing = false;
        queued    = Data {};
        reservedForms.clear();
        reservedAttachments.clear();
        reservedStates.clear();
        failure = "Law failed";
        if (!inbox.empty()) inbox.clear();
        if (!pending.empty()) pending.clear();
        return false;
    }
}
} // namespace
namespace GN::e2 {
Ref<Form>  Form::create(FormId id, const StrA & name) { return referenceTo(new FormImpl(id, name)); }
Ref<World> World::create(Universe & u, const StrA & name, size_t capacity) { return referenceTo(new Runtime(u, name, capacity)); }
Ref<Mold>  Mold::create(Universe & u, const StrA & name, DynaArray<FacetBinding> bindings, FormFactory factory) {
    try {
        // Each recipe owns copies, so later edits to application builders cannot alter future spawns.
        for (auto & b : bindings) {
            if (!b.facet || !b.value || !b.value->typeInfo().isDerivedFrom(b.facet->valueType())) return {};
            b.facet = snapshot(*b.facet);
            b.value = snapshot(*b.value);
        }
        for (const auto & b : bindings)
            for (auto requirement : b.facet->requirements()) {
                bool found = false;
                if (requirement)
                    for (const auto & provider : bindings)
                        if (provider.facet->typeInfo().isDerivedFrom(*requirement)) {
                            found = true;
                            break;
                        }
                if (!found) return {};
            }
        return referenceTo(new MoldImpl(u, name, std::move(bindings), std::move(factory)));
    } catch (...) { return {}; }
}
} // namespace GN::e2

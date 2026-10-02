#include <garnet/GNengine2.h>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <stdexcept>

namespace GN::e2 {
WorldVector3 positionFromMeters(const glm::dvec3 & p, PhysicalScale scale, const WorldVector3 & origin) {
    return spatial::toWorld(origin, LocalVector3(scale.fromMeters(p.x), scale.fromMeters(p.y), scale.fromMeters(p.z)));
}
glm::dvec3 positionToMeters(const WorldVector3 & p, PhysicalScale scale, const WorldVector3 & origin) {
    auto local = spatial::toLocal(origin, p);
    return {scale.toMeters<double>(local.x), scale.toMeters<double>(local.y), scale.toMeters<double>(local.z)};
}

namespace {
bool validRotation(const Rotation & q) {
    return std::isfinite(q.w) && std::isfinite(q.x) && std::isfinite(q.y) && std::isfinite(q.z) && std::abs(glm::dot(q, q) - 1.0f) <= 0.0001f;
}
bool resolvePose(const PrimeView & prime, FormId id, WorldTransform & out, const std::map<FormId, WorldTransform> * solved = nullptr) {
    std::set<FormId>                         visited;
    DynaArray<const TransformFacet::Value *> chain;
    out = WorldTransform {};
    while (id != 0) {
        if (!visited.insert(id).second) return false;
        if (solved) {
            auto it = solved->find(id);
            if (it != solved->end()) {
                out = it->second;
                break;
            }
        }
        auto transform = prime.get<TransformFacet>(id);
        if (!transform || !validRotation(transform->orientation)) return false;
        chain.append(transform);
        id = transform->parent;
    }
    for (size_t i = chain.size(); i > 0; --i) {
        const auto * transform = chain[i - 1];
        // Root translation can span all 128 bits; fixed-point rotation is only for local offsets.
        if (transform->parent == 0) {
            out.position    = transform->position;
            out.orientation = glm::normalize(transform->orientation);
        } else {
            out.position    = out.position + spatial::rotatedBy(out.orientation, transform->position);
            out.orientation = glm::normalize(out.orientation * transform->orientation);
        }
    }
    return true;
}
} // namespace
bool              resolveWorldTransform(const PrimeView & prime, FormId id, WorldTransform & out) { return id != 0 && resolvePose(prime, id, out); }
DynaArray<FormId> transformChildren(const PrimeView & prime, FormId parent) {
    DynaArray<FormId> children;
    for (auto id : prime.query<>()) {
        auto transform = prime.get<TransformFacet>(id);
        if (transform && transform->parent == parent) children.append(id);
    }
    return children;
}
bool TransformFacet::validate(const PrimeView & prime, FormId id, const FacetValue & payload) const {
    auto value = RuntimeType::cast<const Value>(&payload);
    if (!value || !validRotation(value->orientation)) return false;
    std::set<FormId> visited {id};
    for (auto parent = value->parent; parent != 0;) {
        if (!visited.insert(parent).second) return false;
        auto ancestor = prime.get<TransformFacet>(parent);
        if (!ancestor || !validRotation(ancestor->orientation)) return false;
        parent = ancestor->parent;
    }
    return true;
}
namespace {
template<typename V>
Ref<V> cloneValue(const V & value) {
    auto copy = value.clone();
    if (!copy || copy->typeInfo().id != value.typeInfo().id) throw std::runtime_error("Facet clone must preserve its dynamic type");
    auto typed = RuntimeType::cast<V>(copy.get());
    if (!typed) throw std::runtime_error("Facet clone changed its value type");
    return Ref<V>(typed);
}
struct Participant {
    FormId                     id;
    Ref<TransformFacet::Value> transform;
    Ref<MotionFacet::Value>    motion;
    const BodyFacet::Value *   body;
    glm::dvec3                 position;
    double                     inverseMass;
    Ref<ContactFacet::Value>   contact;
};

bool finite(const glm::dvec3 & v) { return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z); }
// Rebuilt every substep: no broadphase cache becomes a hidden source of causal state.
DynaArray<std::pair<size_t, size_t>> pairs(const DynaArray<Participant> & bodies, double tolerance) {
    DynaArray<size_t> order;
    for (size_t i = 0; i < bodies.size(); ++i) order.append(i);
    std::sort(order.begin(), order.end(), [&](size_t a, size_t b) {
        double x = bodies[a].position.x - bodies[a].body->halfExtent.x;
        double y = bodies[b].position.x - bodies[b].body->halfExtent.x;
        return x == y ? bodies[a].id < bodies[b].id : x < y;
    });
    DynaArray<std::pair<size_t, size_t>> result;
    for (size_t i = 0; i < order.size(); ++i) {
        auto a = order[i];
        for (size_t j = i + 1; j < order.size(); ++j) {
            auto b = order[j];
            if (bodies[b].position.x - bodies[b].body->halfExtent.x > bodies[a].position.x + bodies[a].body->halfExtent.x + tolerance) break;
            if (bodies[a].inverseMass + bodies[b].inverseMass == 0) continue;
            auto delta  = glm::abs(bodies[b].position - bodies[a].position);
            auto extent = bodies[a].body->halfExtent + bodies[b].body->halfExtent + glm::dvec3(tolerance);
            if (delta.y <= extent.y && delta.z <= extent.z) result.append({std::min(a, b), std::max(a, b)});
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}
class DynamicsLawImpl final : public Law {
    DynamicsOptions options;

public:
    GN_REGISTER_RUNTIME_TYPE(Law);
    explicit DynamicsLawImpl(const DynamicsOptions & o): Law(TYPE_INFO(), "dynamics"), options(o) {}
    LawContract contract() const override {
        return {"dynamics",
                {&TransformFacet::TYPE_INFO(), &MotionFacet::TYPE_INFO(), &ContactFacet::TYPE_INFO(), &CollisionStatsFacet::TYPE_INFO()},
                {},
                false,
                {&CollisionEvent::TYPE_INFO()}};
    }
    void tick(LawContext & c) override {
        const double           dt = std::chrono::duration<double>(c.dt).count();
        DynaArray<Participant> bodies;
        for (auto id : c.prime.query<TransformFacet, BodyFacet>()) {
            auto transform = cloneValue(*c.prime.get<TransformFacet>(id));
            auto body      = c.prime.get<BodyFacet>(id);
            auto motion    = c.prime.get<MotionFacet>(id);
            if (!c.prime.get<ContactFacet>(id)) throw std::runtime_error("Every demo body requires ContactFacet");
            if (!finite(body->halfExtent) || glm::any(glm::lessThanEqual(body->halfExtent, glm::dvec3(0))) || !std::isfinite(body->mass) || body->mass <= 0 ||
                !std::isfinite(body->friction) || body->friction < 0 || !std::isfinite(body->restitution) || body->restitution < 0 || body->restitution > 1)
                throw std::runtime_error("Invalid box body");
            WorldTransform worldTransform;
            if (!resolveWorldTransform(c.prime, id, worldTransform)) throw std::runtime_error("Invalid spatial hierarchy");
            // Attached static colliders require compound/kinematic integration, outside this demo solver.
            if (!motion)
                for (auto parent = transform->parent; parent != 0;) {
                    if (c.prime.get<MotionFacet>(parent)) throw std::runtime_error("Static collider beneath moving parent is unsupported by MVP");
                    parent = c.prime.get<TransformFacet>(parent)->parent;
                }
            if (glm::length(glm::vec3(worldTransform.orientation.x, worldTransform.orientation.y, worldTransform.orientation.z)) > 0.00001f ||
                (motion && (!finite(motion->linearVelocity) || motion->angularVelocity != glm::dvec3(0))))
                throw std::runtime_error("MVP solver supports axis-aligned boxes with zero spin only");
            bodies.append({id, transform, motion ? cloneValue(*motion) : Ref<MotionFacet::Value>(new MotionFacet::Value), body,
                           positionToMeters(worldTransform.position, options.scale, options.origin), motion ? 1 / body->mass : 0,
                           cloneValue(*c.prime.get<ContactFacet>(id))});
            if (!bodies.back().contact->touching.empty()) bodies.back().contact->touching.clear();
        }
        // Small bounded steps reduce tunnelling in the slow falling-box demo; this is not CCD.
        if (!std::isfinite(dt) || dt <= 0 || dt > 1) throw std::runtime_error("Dynamics tick must be in (0, 1] seconds");
        auto steps = static_cast<unsigned>(std::ceil(dt / (1.0 / 240.0)));
        if (steps == 0 || steps > 240) throw std::runtime_error("Dynamics tick must be in (0, 1] seconds");
        double h = dt / steps;
        for (unsigned step = 0; step < steps; ++step) {
            for (auto & b : bodies)
                if (b.inverseMass > 0) {
                    b.motion->linearVelocity += options.gravity * h;
                    b.position += b.motion->linearVelocity * h;
                }
            for (unsigned iteration = 0; iteration < 8; ++iteration) {
                for (auto indices : pairs(bodies, 0)) {
                    auto & a       = bodies[indices.first];
                    auto & b       = bodies[indices.second];
                    auto   delta   = b.position - a.position;
                    auto   overlap = a.body->halfExtent + b.body->halfExtent - glm::abs(delta);
                    if (glm::any(glm::lessThan(overlap, glm::dvec3(0)))) continue;
                    int axis = overlap.x < overlap.y ? 0 : 1;
                    if (overlap.z < overlap[axis]) axis = 2;
                    glm::dvec3 normal(0);
                    normal[axis]      = delta[axis] >= 0 ? 1 : -1;
                    double inverse    = a.inverseMass + b.inverseMass;
                    auto   correction = normal * (overlap[axis] / inverse);
                    a.position -= correction * a.inverseMass;
                    b.position += correction * b.inverseMass;
                    auto   relative = b.motion->linearVelocity - a.motion->linearVelocity;
                    double closing  = glm::dot(relative, normal);
                    if (closing >= 0) continue;
                    // Suppress tiny gravity-driven rebounds so resting contact remains stable.
                    double restitution   = closing < -0.5 ? std::min(a.body->restitution, b.body->restitution) : 0;
                    double impulse       = -(1 + restitution) * closing / inverse;
                    auto   impulseVector = normal * impulse;
                    auto   tangent       = relative - normal * closing;
                    double speed         = glm::length(tangent);
                    if (speed > 1e-12) impulseVector -= tangent / speed * std::min(speed / inverse, impulse * std::sqrt(a.body->friction * b.body->friction));
                    a.motion->linearVelocity -= impulseVector * a.inverseMass;
                    b.motion->linearVelocity += impulseVector * b.inverseMass;
                }
            }
        }
        for (auto indices : pairs(bodies, options.contactTolerance)) {
            auto & a = bodies[indices.first];
            auto & b = bodies[indices.second];
            a.contact->touching.append(b.id);
            b.contact->touching.append(a.id);
            auto old = c.prime.get<ContactFacet>(a.id);
            if (std::find(old->touching.begin(), old->touching.end(), b.id) == old->touching.end())
                c.emit(CollisionEvent {a.id, b.id, positionFromMeters((a.position + b.position) * 0.5, options.scale, options.origin)});
        }
        // Resolve locals against final parent world poses, not last tick's parents; otherwise parent motion applies twice.
        std::map<FormId, WorldTransform> solved;
        for (const auto & b : bodies)
            if (b.inverseMass > 0) {
                WorldTransform pose;
                if (!resolveWorldTransform(c.prime, b.id, pose)) throw std::runtime_error("Invalid spatial hierarchy");
                pose.position = positionFromMeters(b.position, options.scale, options.origin);
                solved.emplace(b.id, pose);
            }
        for (auto & b : bodies) {
            std::sort(b.contact->touching.begin(), b.contact->touching.end());
            if (auto stats = c.prime.get<CollisionStatsFacet>(b.id)) {
                auto next = cloneValue(*stats);
                auto old  = c.prime.get<ContactFacet>(b.id);
                for (auto other : b.contact->touching)
                    if (std::find(old->touching.begin(), old->touching.end(), other) == old->touching.end()) ++next->count;
                c.slate.set<CollisionStatsFacet>(b.id, *next);
            }
            c.slate.set<ContactFacet>(b.id, *b.contact);
            if (b.inverseMass > 0) {
                auto pose = solved.at(b.id);
                if (b.transform->parent == 0) {
                    b.transform->position    = pose.position;
                    b.transform->orientation = pose.orientation;
                } else {
                    WorldTransform parent;
                    if (!resolvePose(c.prime, b.transform->parent, parent, &solved)) throw std::runtime_error("Invalid final parent pose");
                    auto inverse             = glm::conjugate(parent.orientation);
                    b.transform->position    = spatial::rotatedBy(inverse, pose.position - parent.position);
                    b.transform->orientation = glm::normalize(inverse * pose.orientation);
                }
                c.slate.set<TransformFacet>(b.id, *b.transform);
                c.slate.set<MotionFacet>(b.id, *b.motion);
                if (b.position.y < options.destroyBelowY) c.send(DestroyFormIntent {b.id});
            }
        }
    }
};

// Xorshift state is explicitly committed with generation progress, never held in the Law.
double randomUnit(uint64_t & state) {
    state ^= state >> 12;
    state ^= state << 25;
    state ^= state >> 27;
    return static_cast<double>((state * 2685821657736338717ULL) >> 11) * (1.0 / 9007199254740992.0);
}
class LifetimeLawImpl final : public Law {
    Universe &      universe;
    LifetimeOptions options;

public:
    GN_REGISTER_RUNTIME_TYPE(Law);
    LifetimeLawImpl(Universe & u, const LifetimeOptions & o): Law(TYPE_INFO(), "lifetime"), universe(u), options(o) {}
    LawContract contract() const override {
        return {"lifetime", {&LifetimeState::TYPE_INFO(), &LifetimeFacet::TYPE_INFO()}, {&DestroyFormIntent::TYPE_INFO()}, true, {}};
    }
    void tick(LawContext & c) override {
        const double dt      = std::chrono::duration<double>(c.dt).count();
        auto         initial = c.prime.state<LifetimeState>();
        if (!initial || initial->random == 0) throw std::runtime_error("LifetimeState must have nonzero initialized RNG state");
        auto             state      = cloneValue(*initial);
        auto             live       = c.prime.query<LifetimeFacet>();
        size_t           population = live.size();
        std::set<FormId> deleted;
        for (const auto & payload : c.intents)
            if (auto intent = RuntimeType::cast<const DestroyFormIntent>(payload.get())) {
                if (c.prime.form(intent->form) && deleted.insert(intent->form).second) {
                    c.slate.destroy(intent->form);
                    if (c.prime.get<LifetimeFacet>(intent->form)) --population;
                }
            }
        double interval = 1 / options.spawnPerSecond;
        state->elapsed += dt;
        while (state->elapsed + 1e-12 >= interval && population < options.maximumPopulation) {
            state->elapsed = std::max(0.0, state->elapsed - interval);
            glm::dvec3 p;
            for (int axis = 0; axis < 3; ++axis)
                p[axis] = options.spawnMinimum[axis] + randomUnit(state->random) * (options.spawnMaximum[axis] - options.spawnMinimum[axis]);
            double                speed = randomUnit(state->random) * options.maximumHorizontalSpeed;
            double                angle = randomUnit(state->random) * 6.2831853071795864769;
            TransformFacet::Value transform;
            transform.position = positionFromMeters(p, options.scale, options.origin);
            MotionFacet::Value motion;
            motion.linearVelocity = {std::cos(angle) * speed, 0, std::sin(angle) * speed};
            VisualFacet::Value visual;
            visual.color = {static_cast<float>(0.3 + 0.6 * randomUnit(state->random)), static_cast<float>(0.3 + 0.6 * randomUnit(state->random)),
                            static_cast<float>(0.3 + 0.6 * randomUnit(state->random)), 1};
            auto mold    = createBoxMold(universe, transform, motion, {}, visual);
            if (!mold) throw std::runtime_error("Failed to construct spawn mold");
            c.slate.create(*mold);
            ++population;
        }
        if (population >= options.maximumPopulation) state->elapsed = 0;
        c.slate.setState(*state);
    }
};

} // namespace

bool LifetimeFacet::requestDestroy(World & world, const Form & form) const { return world.submit(DestroyFormIntent(form.formId())); }

bool registerDynamicsFacets(World & world) {
    return world.registerFacet(TransformFacet()) && world.registerFacet(MotionFacet()) && world.registerFacet(BodyFacet()) &&
           world.registerFacet(ContactFacet(),
                               [](FormId, const FacetValue & value, const DynaArray<FormId> & deleted) -> Ref<FacetValue> {
                                   auto old = RuntimeType::cast<const ContactFacet::Value>(&value);
                                   if (!old) throw std::runtime_error("Contact cleanup requires ContactFacet::Value");
                                   auto   next      = old->clone();
                                   auto   contacts  = RuntimeType::cast<ContactFacet::Value>(next.get());
                                   size_t remaining = 0;
                                   for (auto id : contacts->touching) {
                                       if (std::find(deleted.begin(), deleted.end(), id) == deleted.end()) contacts->touching[remaining++] = id;
                                   }
                                   contacts->touching.resize(remaining);
                                   return next;
                               }) &&
           world.registerFacet(CollisionStatsFacet()) && world.registerFacet(VisualFacet()) && world.registerFacet(LifetimeFacet()) &&
           world.registerState<LifetimeState>();
}
namespace {
template<typename F>
FacetBinding binding(const typename F::Value & value = typename F::Value()) {
    return {Ref<Facet>(new F), value.clone()};
}
} // namespace
Ref<Law> createDynamicsLaw(const DynamicsOptions & options) {
    if (!finite(options.gravity) || !std::isfinite(options.destroyBelowY) || !std::isfinite(options.contactTolerance) || options.contactTolerance < 0)
        return {};
    return Ref<Law>(new DynamicsLawImpl(options));
}
Ref<Law> createLifetimeLaw(Universe & universe, const LifetimeOptions & options) {
    if (!std::isfinite(options.spawnPerSecond) || options.spawnPerSecond <= 0 || options.spawnPerSecond > 100 || options.maximumPopulation > 1000 ||
        !std::isfinite(options.maximumHorizontalSpeed) || options.maximumHorizontalSpeed < 0 || !finite(options.spawnMinimum) ||
        !finite(options.spawnMaximum) || glm::any(glm::greaterThan(options.spawnMinimum, options.spawnMaximum)))
        return {};
    return Ref<Law>(new LifetimeLawImpl(universe, options));
}
Ref<Mold> createGroundMold(Universe & universe, PhysicalScale scale, const WorldVector3 & origin) {
    TransformFacet::Value transform;
    transform.position = positionFromMeters({0, -0.5, 0}, scale, origin);
    BodyFacet::Value body;
    body.halfExtent = {4, 0.5, 4};
    VisualFacet::Value visual;
    visual.halfExtent = {4, 0.5f, 4};
    visual.color      = {0.35f, 0.4f, 0.35f, 1};
    return Mold::create(universe, "ground",
                        {binding<TransformFacet>(transform), binding<BodyFacet>(body), binding<ContactFacet>(), binding<VisualFacet>(visual)});
}
Ref<Mold> createBoxMold(Universe & universe, const TransformFacet::Value & transform, const MotionFacet::Value & motion, const BodyFacet::Value & body,
                        const VisualFacet::Value & visual) {
    return Mold::create(universe, "box",
                        {binding<TransformFacet>(transform), binding<MotionFacet>(motion), binding<BodyFacet>(body), binding<ContactFacet>(),
                         binding<CollisionStatsFacet>(), binding<VisualFacet>(visual), binding<LifetimeFacet>()});
}
} // namespace GN::e2

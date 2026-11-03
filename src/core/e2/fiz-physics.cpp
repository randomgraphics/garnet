#include <garnet/GNengine2.h>
#include "spatial-private.h"

#include <algorithm>
#include <cmath>
#include <cfloat>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace GN::e2 {
namespace {

fiz::Vector3 toFizVector(const glm::dvec3 & v, const char * description) {
    if (!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(v.z) || std::abs(v.x) > FLT_MAX || std::abs(v.y) > FLT_MAX || std::abs(v.z) > FLT_MAX)
        throw std::runtime_error(std::string("GNfizLaw: ") + description + " exceeds GNfiz float range");
    return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)};
}

AutoRef<fiz::Hull> uniformScaledHull(const fiz::Hull & hull, float scale) {
    const float k = std::abs(scale);
    switch (hull.type()) {
    case fiz::HullType::BOX: {
        const auto & ext = static_cast<const fiz::BoxHull &>(hull).halfExtents();
        return fiz::Hull::createBox(ext * k);
    }
    case fiz::HullType::SPHERE:
        return fiz::Hull::createSphere(static_cast<const fiz::SphereHull &>(hull).radius() * k);
    case fiz::HullType::CAPSULE: {
        const auto & c = static_cast<const fiz::CapsuleHull &>(hull);
        return fiz::Hull::createCapsule(c.radius() * k, c.halfHeight() * k);
    }
    case fiz::HullType::CYLINDER: {
        const auto & c = static_cast<const fiz::CylinderHull &>(hull);
        return fiz::Hull::createCylinder(c.radius() * k, c.halfHeight() * k);
    }
    case fiz::HullType::CONVEX: {
        const auto &              vertices = static_cast<const fiz::ConvexHull &>(hull).vertices();
        std::vector<fiz::Vector3> scaled;
        scaled.reserve(vertices.size());
        for (auto p : vertices) scaled.push_back(p * scale);
        return fiz::Hull::createConvex(scaled.data(), scaled.size());
    }
    case fiz::HullType::MESH: {
        const auto &              mesh = static_cast<const fiz::MeshHull &>(hull);
        std::vector<fiz::Vector3> vertices;
        vertices.reserve(mesh.vertices().size());
        for (auto p : mesh.vertices()) vertices.push_back(p * scale);
        auto indices = mesh.indices();
        if (scale < 0)
            for (size_t i = 0; i + 2 < indices.size(); i += 3) std::swap(indices[i + 1], indices[i + 2]);
        return fiz::Hull::createMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
    }
    }
    return {};
}

AutoRef<fiz::Hull> affineBakedHull(const fiz::Hull & hull, const glm::dmat3 & linear, bool staticBody) {
    std::vector<fiz::Vector3> vertices;
    std::vector<uint32_t>     indices;
    bool                      meshGeometry = false;
    switch (hull.type()) {
    case fiz::HullType::BOX: {
        const auto & ext = static_cast<const fiz::BoxHull &>(hull).halfExtents();
        vertices.reserve(8);
        for (int x : {-1, 1})
            for (int y : {-1, 1})
                for (int z : {-1, 1}) vertices.emplace_back(ext.x * x, ext.y * y, ext.z * z);
        break;
    }
    case fiz::HullType::CONVEX: {
        const auto & source = static_cast<const fiz::ConvexHull &>(hull).vertices();
        vertices            = source;
        break;
    }
    case fiz::HullType::MESH: {
        if (!staticBody) return {};
        const auto & source = static_cast<const fiz::MeshHull &>(hull);
        vertices            = source.vertices();
        indices             = source.indices();
        meshGeometry        = true;
        break;
    }
    default:
        return {};
    }
    for (auto & p : vertices) {
        glm::dvec3 q = linear * glm::dvec3(p.x, p.y, p.z);
        if (!std::isfinite(q.x) || !std::isfinite(q.y) || !std::isfinite(q.z) || std::abs(q.x) > FLT_MAX || std::abs(q.y) > FLT_MAX || std::abs(q.z) > FLT_MAX)
            return {};
        p = fiz::Vector3(static_cast<float>(q.x), static_cast<float>(q.y), static_cast<float>(q.z));
    }
    if (meshGeometry) {
        if (glm::determinant(linear) < 0)
            for (size_t i = 0; i + 2 < indices.size(); i += 3) std::swap(indices[i + 1], indices[i + 2]);
        return fiz::Hull::createMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
    }
    return fiz::Hull::createConvex(vertices.data(), vertices.size());
}

bool sameMatrix(const glm::dmat3 & a, const glm::dmat3 & b) {
    for (int c = 0; c < 3; ++c)
        for (int r = 0; r < 3; ++r)
            if (a[c][r] != b[c][r]) return false;
    return true;
}

struct BodyRecord {
    AutoRef<fiz::Solid> solid;
    AutoRef<fiz::Hull>  sourceHull;
    AutoRef<fiz::Hull>  scaledHull;
    float               scale = 1;
    glm::dmat3          affineLinear {1.0};
    bool                affine = false;
};

class GNfizLawImpl final : public Law {
    GNfizLawOptions              options;
    AutoRef<fiz::SolidEngine>    engine;
    std::map<FormId, BodyRecord> solids;

public:
    GN_REGISTER_RUNTIME_TYPE(Law);
    explicit GNfizLawImpl(const GNfizLawOptions & o): Law(TYPE_INFO(), "gnfiz-physics"), options(o) {
        fiz::SolidEngineDesc desc;
        desc.gravity          = {static_cast<float>(options.gravity.x), static_cast<float>(options.gravity.y), static_cast<float>(options.gravity.z)};
        desc.numWorkerThreads = options.workerThreads;
        engine                = fiz::SolidEngine::create(desc);
        if (!engine) throw std::runtime_error("GNfizLaw: failed to create SolidEngine");
    }

    LawContract contract() const override {
        return {"gnfiz-physics",
                {&TransformFacet::TYPE_INFO(), &MotionFacet::TYPE_INFO(), &PhysicsFacet::TYPE_INFO()},
                {},
                false,
                {&CollisionEvent::TYPE_INFO()}};
    }

    void tick(LawContext & c) override {
        std::map<FormId, bool> present;
        for (FormId formId : c.prime.query<PhysicsFacet, TransformFacet>()) {
            const auto * physics = c.prime.get<PhysicsFacet>(formId);
            const auto * local   = c.prime.get<TransformFacet>(formId);
            if (!physics || !physics->collider) throw std::runtime_error("GNfizLaw: PhysicsFacet requires a non-null collider Hull");
            WorldTransform world;
            if (!resolveWorldTransform(c.prime, formId, world)) throw std::runtime_error("GNfizLaw: invalid Spatial transform");
            if (!std::isfinite(local->scale.x) || !std::isfinite(local->scale.y) || !std::isfinite(local->scale.z) || local->scale.x == 0 ||
                local->scale.y == 0 || local->scale.z == 0)
                throw std::runtime_error("GNfizLaw: collider scale must be nonzero and finite");
            present.emplace(formId, true);
            LocalVector3 localPosition;
            if (!spatial::tryToLocal(options.origin, world.position, localPosition)) {
                const auto & p = local->position;
                throw std::runtime_error("GNfizLaw: Form " + std::to_string(formId) + " at tick " + std::to_string(c.prime.tick()) +
                                         " has a world position outside the local physics frame (parent=" + std::to_string(local->parent) +
                                         ", local x=" + std::to_string(p.x.hi()) + ":" + std::to_string(p.x.lo()) + ", y=" + std::to_string(p.y.hi()) + ":" +
                                         std::to_string(p.y.lo()) + ", z=" + std::to_string(p.z.hi()) + ":" + std::to_string(p.z.lo()) + ")");
            }
            glm::dvec3         pose {options.scale.toMeters<double>(localPosition.x), options.scale.toMeters<double>(localPosition.y),
                                     options.scale.toMeters<double>(localPosition.z)};
            const auto         linear         = detail::affineLinear(world);
            const auto         colliderLinear = world.affineFallback ? glm::transpose(glm::mat3_cast(glm::dquat(world.orientation))) * linear : linear;
            fiz::SolidDesc     desc;
            auto               it = solids.find(formId);
            AutoRef<fiz::Hull> scaledHull;
            const bool         shapeChanged = it == solids.end() || it->second.sourceHull.get() != physics->collider.get() ||
                                              (world.affineFallback ? !it->second.affine || !sameMatrix(it->second.affineLinear, colliderLinear)
                                                                    : it->second.affine || it->second.scale != world.uniformScale);
            if (!shapeChanged) {
                scaledHull = it->second.scaledHull;
            } else if (world.uniformScale == 1.f && !world.affineFallback) {
                scaledHull = physics->collider;
            } else if (!world.affineFallback) {
                scaledHull = uniformScaledHull(*physics->collider, world.uniformScale);
            } else {
                scaledHull = affineBakedHull(*physics->collider, colliderLinear, physics->motionType == fiz::MotionType::STATIC);
            }
            if (!scaledHull) {
                GN_ERROR(
                    getLogger("GN.e2.GNfizLaw"),
                    "Form {} uses unsupported collider affine scale ({}, {}, {}) for HullType {} (dynamic concave meshes and non-uniform sphere/capsule/cylinder need an explicit proxy).",
                    formId, local->scale.x, local->scale.y, local->scale.z, static_cast<uint32_t>(physics->collider->type()));
                throw std::runtime_error("GNfizLaw: cannot bake this collider's non-uniform affine scale");
            }
            desc.hull                  = scaledHull;
            desc.motionType            = physics->motionType;
            desc.layer                 = physics->layer;
            desc.temper                = physics->temper;
            desc.massOverride          = physics->massOverride;
            desc.allowSleep            = physics->allowSleep;
            desc.ccd                   = physics->ccd;
            desc.entityId              = static_cast<uint64_t>(formId);
            desc.transform.position    = toFizVector(pose, "Form position");
            desc.transform.orientation = fiz::Quaternion(world.orientation.x, world.orientation.y, world.orientation.z, world.orientation.w);
            if (it == solids.end()) {
                auto solid = engine->createSolid(desc);
                if (!solid) throw std::runtime_error("GNfizLaw: GNfiz rejected collider for Form " + std::to_string(formId));
                it = solids
                         .emplace(formId, BodyRecord {std::move(solid), physics->collider, std::move(scaledHull), world.uniformScale, colliderLinear,
                                                      world.affineFallback})
                         .first;
            } else {
                if (shapeChanged) {
                    if (!it->second.solid->setHull(scaledHull))
                        throw std::runtime_error("GNfizLaw: failed to update scaled collider for Form " + std::to_string(formId));
                    it->second.sourceHull   = physics->collider;
                    it->second.scaledHull   = scaledHull;
                    it->second.scale        = world.uniformScale;
                    it->second.affineLinear = colliderLinear;
                    it->second.affine       = world.affineFallback;
                }
                it->second.solid->setTransform(desc.transform, true);
            }
            if (physics->motionType == fiz::MotionType::DYNAMIC) {
                const auto * motion = c.prime.get<MotionFacet>(formId);
                if (!motion) throw std::runtime_error("GNfizLaw: dynamic PhysicsFacet requires MotionFacet");
                it->second.solid->setLinearVelocity(toFizVector(motion->linearVelocity, "linear velocity"));
                it->second.solid->setAngularVelocity(toFizVector(motion->angularVelocity, "angular velocity"));
            }
        }
        for (auto it = solids.begin(); it != solids.end();) {
            if (!present.count(it->first)) {
                engine->removeSolid(it->second.solid.get());
                it = solids.erase(it);
            } else {
                ++it;
            }
        }
        engine->step(c.dt);
        std::map<FormId, DynaArray<FormId>> touching;
        for (const auto & contact : engine->contactPairs()) {
            const auto first  = static_cast<FormId>(contact.entityA);
            const auto second = static_cast<FormId>(contact.entityB);
            if (!c.prime.get<PhysicsFacet>(first) || !c.prime.get<PhysicsFacet>(second)) continue;
            touching[first].append(second);
            touching[second].append(first);
            const auto * oldFirst = c.prime.get<PhysicsFacet>(first);
            if (std::find(oldFirst->touching.begin(), oldFirst->touching.end(), second) == oldFirst->touching.end()) {
                auto point = positionFromMeters({contact.position.x, contact.position.y, contact.position.z}, options.scale, options.origin);
                c.emit(CollisionEvent {first, second, point});
            }
        }
        for (const auto & entry : solids) {
            const FormId formId   = entry.first;
            auto         partners = touching[formId];
            std::sort(partners.begin(), partners.end());
            DynaArray<FormId> uniquePartners;
            for (FormId partner : partners)
                if (uniquePartners.empty() || uniquePartners.back() != partner) uniquePartners.append(partner);
            auto next  = c.prime.get<PhysicsFacet>(formId)->clone();
            auto value = RuntimeType::cast<PhysicsFacet::Value>(next.get());
            if (!value) throw std::runtime_error("GNfizLaw: PhysicsFacet clone changed dynamic type");
            value->touching = std::move(uniquePartners);
            c.slate.set<PhysicsFacet>(formId, *value);
        }
        std::map<FormId, WorldTransform> solved;
        for (const auto & [formId, record] : solids) {
            const auto * physics = c.prime.get<PhysicsFacet>(formId);
            if (physics->motionType != fiz::MotionType::DYNAMIC) continue;
            auto           p = record.solid->position();
            auto           q = record.solid->orientation();
            WorldTransform pose;
            pose.position       = positionFromMeters({p.x, p.y, p.z}, options.scale, options.origin);
            pose.orientation    = Rotation(q.w, q.v.x, q.v.y, q.v.z);
            pose.uniformScale   = record.affine ? 1.f : record.scale;
            pose.affineFallback = record.affine;
            if (record.affine) pose.affineLinear = glm::mat3_cast(glm::dquat(pose.orientation)) * record.affineLinear;
            solved.emplace(formId, pose);
        }
        for (const auto & [formId, record] : solids) {
            const auto * physics = c.prime.get<PhysicsFacet>(formId);
            if (physics->motionType != fiz::MotionType::DYNAMIC) continue;
            const auto *          old = c.prime.get<TransformFacet>(formId);
            TransformFacet::Value transform(*old);
            const auto &          worldPose = solved.at(formId);
            if (record.affine || old->parent != 0) {
                WorldTransform parent;
                glm::dmat3     parentLinear(1.0);
                WorldVector3   parentPosition;
                if (old->parent != 0) {
                    if (!detail::resolveWorldTransform(c.prime, old->parent, parent, &solved))
                        throw std::runtime_error("GNfizLaw: invalid parent transform after physics step");
                    parentLinear   = detail::affineLinear(parent);
                    parentPosition = parent.position;
                }
                auto inverseParent = glm::inverse(parentLinear);
                if (!detail::supportedAffineCoefficients(inverseParent)) throw std::runtime_error("GNfizLaw: singular or out-of-range parent affine transform");
                WorldVector3 localOffset;
                if (!spatial::tryTransformBy(inverseParent, worldPose.position - parentPosition, localOffset))
                    throw std::runtime_error("GNfizLaw: simulated child position exceeds fixed-point affine range");
                transform.position       = localOffset;
                transform.orientation    = glm::normalize(glm::inverse(parent.orientation) * worldPose.orientation);
                transform.affineLinear   = inverseParent * detail::affineLinear(worldPose);
                transform.affineOverride = true;
                if (!detail::supportedAffineCoefficients(transform.affineLinear))
                    throw std::runtime_error("GNfizLaw: resulting local affine transform is out of range");
            } else {
                transform.position    = worldPose.position;
                transform.orientation = worldPose.orientation;
            }
            c.slate.set<TransformFacet>(formId, transform);
            MotionFacet::Value motion;
            auto               v   = record.solid->linearVelocity();
            auto               w   = record.solid->angularVelocity();
            motion.linearVelocity  = {v.x, v.y, v.z};
            motion.angularVelocity = {w.x, w.y, w.z};
            c.slate.set<MotionFacet>(formId, motion);
        }
    }
};

} // namespace

bool registerPhysicsFacet(World & world) { return world.registerFacet(PhysicsFacet()); }

bool PhysicsFacet::validate(const PrimeView & prime, FormId formId, const FacetValue & payload) const {
    const auto * value = RuntimeType::cast<const Value>(&payload);
    if (!value || !value->collider) return false;
    switch (value->motionType) {
    case fiz::MotionType::STATIC:
    case fiz::MotionType::KINEMATIC:
        break;
    case fiz::MotionType::DYNAMIC:
        if (!prime.get<MotionFacet>(formId)) return false;
        break;
    default:
        return false;
    }
    if (static_cast<uint16_t>(value->layer) >= static_cast<uint16_t>(fiz::CollisionLayer::COUNT)) return false;
    const auto & t = value->temper;
    return std::isfinite(value->massOverride) && value->massOverride >= 0 && std::isfinite(t.density) && t.density > 0 && std::isfinite(t.restitution) &&
           t.restitution >= 0 && t.restitution <= 1 && std::isfinite(t.friction) && t.friction >= 0 && std::isfinite(t.linearDamping) && t.linearDamping >= 0 &&
           std::isfinite(t.angularDamping) && t.angularDamping >= 0 && std::isfinite(t.compliance) && t.compliance >= 0;
}

Ref<Law> createGNfizLaw(const GNfizLawOptions & options) {
    if (!std::isfinite(options.gravity.x) || !std::isfinite(options.gravity.y) || !std::isfinite(options.gravity.z) || std::abs(options.gravity.x) > FLT_MAX ||
        std::abs(options.gravity.y) > FLT_MAX || std::abs(options.gravity.z) > FLT_MAX || options.scale.exponent < -19 || options.scale.exponent > 19 ||
        options.workerThreads == 0)
        return {};
    return Ref<Law>(new GNfizLawImpl(options));
}

} // namespace GN::e2

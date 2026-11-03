#include <garnet/GNengine2.h>
#include "spatial-private.h"
#include <cmath>
#include <map>
#include <set>

namespace GN::e2 {

namespace {
bool validRotation(const Rotation & q) {
    return std::isfinite(q.w) && std::isfinite(q.x) && std::isfinite(q.y) && std::isfinite(q.z) && std::abs(glm::dot(q, q) - 1.0f) <= 0.0001f;
}

bool validScale(const glm::vec3 & s) {
    return std::isfinite(s.x) && std::isfinite(s.y) && std::isfinite(s.z) && s.x != 0 && s.y != 0 && s.z != 0;
}

bool uniformScale(const glm::vec3 & s) { return s.x == s.y && s.y == s.z; }

glm::dmat3 localLinear(const TransformFacet::Value & value) {
    return value.affineOverride ? value.affineLinear
                                : glm::mat3_cast(glm::dquat(glm::normalize(value.orientation))) *
                                      glm::dmat3(value.scale.x, 0, 0, 0, value.scale.y, 0, 0, 0, value.scale.z);
}
} // namespace

namespace detail {
glm::dmat3 affineLinear(const WorldTransform & pose) {
    return pose.affineFallback ? glm::dmat3(pose.affineLinear) : glm::mat3_cast(glm::dquat(pose.orientation)) * (double) pose.uniformScale;
}

bool supportedAffineCoefficients(const glm::dmat3 & m) {
    constexpr double limit = 2147483647.0;
    for (int c = 0; c < 3; ++c)
        for (int r = 0; r < 3; ++r)
            if (!std::isfinite(m[c][r]) || std::abs(m[c][r]) >= limit) return false;
    return true;
}

bool resolveWorldTransform(const PrimeView & prime, FormId id, WorldTransform & out, const std::map<FormId, WorldTransform> * solved) {
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
        if (!transform || !validRotation(transform->orientation) || !validScale(transform->scale)) return false;
        chain.append(transform);
        id = transform->parent;
    }
    for (size_t i = chain.size(); i > 0; --i) {
        const auto * transform = chain[i - 1];
        if (transform->parent == 0) {
            out.position = transform->position;
            out.orientation = glm::normalize(transform->orientation);
            if (transform->affineOverride) {
                out.affineLinear = transform->affineLinear;
                out.affineFallback = true;
                if (!supportedAffineCoefficients(out.affineLinear) || std::abs(glm::determinant(out.affineLinear)) < 1e-20) return false;
            } else if (uniformScale(transform->scale)) {
                out.uniformScale = transform->scale.x;
                if (!std::isfinite(out.uniformScale) || std::abs(out.uniformScale) >= 2147483647.0f) return false;
            } else {
                out.affineLinear = glm::mat3_cast(glm::dquat(out.orientation)) * glm::dmat3(transform->scale.x, 0, 0, 0, transform->scale.y, 0, 0, 0, transform->scale.z);
                out.affineFallback = true;
                if (!supportedAffineCoefficients(out.affineLinear)) return false;
            }
        } else {
            const auto localRotation = glm::normalize(transform->orientation);
            if (!out.affineFallback && !transform->affineOverride && uniformScale(transform->scale)) {
                if (out.uniformScale == 1.f) {
                    out.position = out.position + spatial::rotatedBy(out.orientation, transform->position);
                } else {
                    auto parentLinear = glm::mat3_cast(glm::dquat(out.orientation)) * (double) out.uniformScale;
                    WorldVector3 offset;
                    if (!spatial::tryTransformBy(parentLinear, transform->position, offset)) return false;
                    out.position = out.position + offset;
                }
                out.orientation = glm::normalize(out.orientation * localRotation);
                out.uniformScale *= transform->scale.x;
                if (!std::isfinite(out.uniformScale) || std::abs(out.uniformScale) >= 2147483647.0f) return false;
            } else {
                const glm::dmat3 localMatrix = localLinear(*transform);
                const glm::dmat3 composed = affineLinear(out) * localMatrix;
                if (!supportedAffineCoefficients(composed)) return false;
                WorldVector3 offset;
                if (!spatial::tryTransformBy(affineLinear(out), transform->position, offset)) return false;
                out.position = out.position + offset;
                out.affineLinear = composed;
                out.affineFallback = true;
                out.orientation = glm::normalize(out.orientation * localRotation);
            }
        }
    }
    return true;
}
} // namespace detail

bool resolveWorldTransform(const PrimeView & prime, FormId id, WorldTransform & out) {
    return id != 0 && detail::resolveWorldTransform(prime, id, out, nullptr);
}

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
    if (!value || !validRotation(value->orientation) || !validScale(value->scale)) return false;
    if (value->affineOverride && (!detail::supportedAffineCoefficients(value->affineLinear) || std::abs(glm::determinant(value->affineLinear)) < 1e-20))
        return false;
    std::set<FormId> visited {id};
    for (auto parent = value->parent; parent != 0;) {
        if (!visited.insert(parent).second) return false;
        auto ancestor = prime.get<TransformFacet>(parent);
        if (!ancestor || !validRotation(ancestor->orientation) || !validScale(ancestor->scale) ||
            (ancestor->affineOverride && (!detail::supportedAffineCoefficients(ancestor->affineLinear) || std::abs(glm::determinant(ancestor->affineLinear)) < 1e-20)))
            return false;
        parent = ancestor->parent;
    }
    return true;
}

WorldVector3 positionFromMeters(const glm::dvec3 & p, PhysicalScale scale, const WorldVector3 & origin) {
    return spatial::toWorld(origin, LocalVector3(scale.fromMeters(p.x), scale.fromMeters(p.y), scale.fromMeters(p.z)));
}

glm::dvec3 positionToMeters(const WorldVector3 & p, PhysicalScale scale, const WorldVector3 & origin) {
    auto local = spatial::toLocal(origin, p);
    return {scale.toMeters<double>(local.x), scale.toMeters<double>(local.y), scale.toMeters<double>(local.z)};
}

} // namespace GN::e2

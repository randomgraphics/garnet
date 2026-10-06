#include "world-model.h"

#include <cmath>
#include <cstdio>

namespace GN::taixu::prototype {

namespace {

struct BoxDefinition {
    const char * name;
    glm::dvec3   position;
    glm::vec3    size;
    glm::vec4    color;
    float        rotationZ = 0;
};

e2::FacetBinding makeBinding(const e2::TransformFacet::Value & value) { return {e2::Ref<const e2::Facet>(new e2::TransformFacet), value.clone()}; }

e2::FacetBinding makeBinding(const e2::VisualFacet::Value & value) { return {e2::Ref<const e2::Facet>(new e2::VisualFacet), value.clone()}; }

bool valid(const BoxDefinition & box) {
    return std::isfinite(box.position.x) && std::isfinite(box.position.y) && std::isfinite(box.position.z) &&
           glm::all(glm::greaterThan(box.size, glm::vec3(0))) && std::isfinite(box.rotationZ);
}

e2::Ref<e2::Mold> makeBoxMold(e2::Universe & universe, e2::PhysicalScale scale, const BoxDefinition & box) {
    if (!valid(box)) return {};

    e2::TransformFacet::Value transform;
    transform.position    = e2::positionFromMeters(box.position, scale);
    transform.orientation = glm::angleAxis(box.rotationZ, glm::vec3(0, 0, 1));
    e2::VisualFacet::Value visual;
    visual.halfExtent = box.size * 0.5f;
    visual.color      = box.color;
    return e2::Mold::create(universe, box.name, {makeBinding(transform), makeBinding(visual)},
                            [name = StrA(box.name)](e2::FormId id) { return e2::Form::create(id, name); });
}

bool addBoxes(e2::Universe & universe, e2::World & world, e2::PhysicalScale scale, const BoxDefinition * boxes, size_t count,
              DynaArray<e2::FormId> * createdIds = nullptr) {
    DynaArray<e2::Ref<e2::Mold>> molds;
    for (size_t i = 0; i < count; ++i) {
        auto mold = makeBoxMold(universe, scale, boxes[i]);
        if (!mold) return false;
        molds.append(std::move(mold));
    }

    for (const auto & mold : molds) {
        const auto id = world.createForm(*mold);
        if (!id) return false;
        if (createdIds) createdIds->append(id);
    }
    return true;
}

bool verifyBoxes(const e2::PrimeView & prime, e2::PhysicalScale scale, const BoxDefinition * boxes, size_t count, const DynaArray<e2::FormId> & ids) {
    if (ids.size() != count) return false;
    for (size_t i = 0; i < ids.size(); ++i) {
        const auto   form      = prime.form(ids[i]);
        const auto * transform = prime.get<e2::TransformFacet>(ids[i]);
        const auto * visual    = prime.get<e2::VisualFacet>(ids[i]);
        if (!form || form->name != StrA(boxes[i].name) || !transform || !visual) {
            std::fprintf(stderr, "House Form check failed for expected=%s actual=%s found=%d transform=%d visual=%d\n", boxes[i].name,
                         form ? form->name.c_str() : "<missing>", !!form, !!transform, !!visual);
            return false;
        }
        if (glm::length(e2::positionToMeters(transform->position, scale) - boxes[i].position) > 2e-6) {
            const auto position = e2::positionToMeters(transform->position, scale);
            std::fprintf(stderr, "House Transform check failed for %s: got=(%.8f,%.8f,%.8f) expected=(%.8f,%.8f,%.8f)\n", boxes[i].name, position.x, position.y,
                         position.z, boxes[i].position.x, boxes[i].position.y, boxes[i].position.z);
            return false;
        }
        if (glm::length(visual->halfExtent - boxes[i].size * 0.5f) > 1e-5f || glm::length(visual->color - boxes[i].color) > 1e-5f) {
            std::fprintf(stderr, "House Visual check failed for %s\n", boxes[i].name);
            return false;
        }
    }
    return true;
}

} // namespace

bool createWorld(e2::Universe & universe, e2::World & world, e2::PhysicalScale scale) {
    if (!world.registerFacet(e2::TransformFacet {}) || !world.registerFacet(e2::VisualFacet {})) return false;

    constexpr BoxDefinition landmarks[] = {
        {"ground", {0.0, -0.15, 0.0}, {24.0f, 0.3f, 24.0f}, {0.25f, 0.42f, 0.28f, 1.0f}},
        {"blue-pillar", {3.5, 1.1, -1.0}, {1.2f, 2.2f, 1.2f}, {0.30f, 0.54f, 0.78f, 1.0f}},
        {"red-marker-base", {6.0, 0.6, -4.0}, {1.0f, 1.2f, 1.0f}, {0.78f, 0.32f, 0.25f, 1.0f}},
        {"red-marker-cap", {6.0, 1.6, -4.0}, {1.8f, 0.35f, 1.8f}, {0.92f, 0.72f, 0.35f, 1.0f}},
        {"tree-trunk", {-7.0, 0.7, -5.0}, {0.55f, 1.4f, 0.55f}, {0.38f, 0.28f, 0.18f, 1.0f}},
        {"tree-crown", {-7.0, 1.8, -5.0}, {2.2f, 1.2f, 2.2f}, {0.20f, 0.52f, 0.30f, 1.0f}},
    };
    return addBoxes(universe, world, scale, landmarks, std::size(landmarks));
}

bool addWoodHouse(e2::Universe & universe, e2::World & world, e2::PhysicalScale scale, const glm::dvec3 & origin) {
    if (!std::isfinite(origin.x) || !std::isfinite(origin.y) || !std::isfinite(origin.z)) return false;

    const BoxDefinition house[] = {
        {"wood-house-walls", origin + glm::dvec3(0.0, 0.85, 0.0), {3.2f, 1.7f, 2.8f}, {0.48f, 0.23f, 0.12f, 1}},
        {"wood-house-roof-left", origin + glm::dvec3(-0.75, 2.0, 0.0), {1.75f, 0.24f, 3.2f}, {0.30f, 0.13f, 0.07f, 1}, 0.48f},
        {"wood-house-roof-right", origin + glm::dvec3(0.75, 2.0, 0.0), {1.75f, 0.24f, 3.2f}, {0.36f, 0.16f, 0.08f, 1}, -0.48f},
        {"wood-house-door", origin + glm::dvec3(0.0, 0.65, 1.54), {0.72f, 1.3f, 0.16f}, {0.22f, 0.10f, 0.045f, 1}},
        {"wood-house-window-left", origin + glm::dvec3(-0.95, 1.12, 1.54), {0.55f, 0.48f, 0.16f}, {0.22f, 0.48f, 0.65f, 1}},
        {"wood-house-window-right", origin + glm::dvec3(0.95, 1.12, 1.54), {0.55f, 0.48f, 0.16f}, {0.22f, 0.48f, 0.65f, 1}},
    };
    DynaArray<e2::FormId> createdIds;
    if (!addBoxes(universe, world, scale, house, std::size(house), &createdIds)) {
        std::fprintf(stderr, "Failed to create house Forms\n");
        return false;
    }
    auto prime = world.primeSnapshot();
    if (!prime || !verifyBoxes(*prime, scale, house, std::size(house), createdIds)) {
        std::fprintf(stderr, "House edit verification failed\n");
        return false;
    }
    return true;
}

} // namespace GN::taixu::prototype

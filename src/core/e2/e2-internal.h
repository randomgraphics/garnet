#pragma once
// Internal, non-exported types shared between the engine2 (e2) implementation files.
// This header is private to src/core/e2 and is NOT part of the public e2 interface.

#include <garnet/GNengine2.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>

namespace GN::e2 {

// Query a form tree for facets that match, or derive from, the requested runtime type.
inline void queryFacetsByType(Form & root, const RuntimeType::TypeInfo & type, DynaArray<Ref<Facet>> & result) {
    for (auto & facet : root.facets()) {
        if (facet->typeInfo().isDerivedFrom(type)) result.append(facet);
    }
    for (auto & child : root.children()) queryFacetsByType(*child, type, result);
}

// ---------------------------------------------------------------------------
// MeshData — CPU-side geometry shared across frames
// ---------------------------------------------------------------------------
// Forms hand the same MeshData instance to every visual moment they contribute to. The
// visual domain caches GPU buffers keyed by MeshData::id, so geometry is uploaded once.

struct MeshData {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    int64_t             id = 0; ///< stable identity used as the visual domain's GPU-cache key
    DynaArray<Vertex>   vertices;
    DynaArray<uint16_t> indices;
};

// ---------------------------------------------------------------------------
// VisualMomentImpl — the official, self-contained visual snapshot
// ---------------------------------------------------------------------------
// This is the concrete VisualMoment produced by worlds and consumed by the official
// VisualDomain. It is a generic scene description (cameras + renderables + lights), not
// anything specific to the Simple world. Forms contribute single-entry moments which the
// world merges into one aggregate snapshot.

struct VisualMomentImpl : VisualMoment {
    GN_REGISTER_RUNTIME_TYPE(VisualMoment);

    struct Renderable {
        std::shared_ptr<const MeshData> mesh;

        // Transform kept in world units (not a baked float matrix) so the visual domain can do
        // the camera-relative rebasing in exact integer space before converting to physical
        // floats. The translation is an absolute coordinate; the scaling is an extent, so it is
        // local and converts directly.
        WorldVector3 translation;
        glm::quat    rotation = {1.f, 0.f, 0.f, 0.f};
        LocalVector3 scaling  = {LocalCoordinate(1), LocalCoordinate(1), LocalCoordinate(1)};

        glm::vec3 baseColor = glm::vec3(0.8f);
    };

    struct Light {
        WorldVector3 position;          ///< position in world units
        glm::vec3    color = {1, 1, 1}; ///< RGB already pre-scaled by luminous intensity
    };

    /// Physical size of one world unit for all lengths carried by this moment. Lengths stay in
    /// integer world units until the visual domain converts them, so the absolute-to-camera-
    /// relative rebasing happens in exact integer space before scaling.
    const PhysicalScale scale;

    DynaArray<Camera::Desc> cameras;
    DynaArray<Renderable>   renderables;
    DynaArray<Light>        lights;

    VisualMomentImpl(Universe & u, PhysicalScale scale_): VisualMoment(TYPE_INFO(), u.generateUniqueIdentifier(), "visual-moment"), scale(scale_) {}

    /// Append another moment's renderables and lights into this one.
    void merge(const VisualMomentImpl & other) {
        for (auto & r : other.renderables) renderables.append(r);
        for (auto & l : other.lights) lights.append(l);
    }
};

// ---------------------------------------------------------------------------
// Shader-facing constant layouts (must match src/core/e2/vk-shaders/box.*)
// ---------------------------------------------------------------------------

constexpr uint32_t kMaxLights = 4;

/// Per-frame uniform block, set = 0, binding = 0. std140 layout.
struct FrameConstants {
    glm::mat4 viewProj;                  ///< proj * view, Vulkan clip space (Y already flipped); the view carries no translation
    glm::vec4 ambient;                   ///< rgb ambient term, a unused
    glm::vec4 lightPosition[kMaxLights]; ///< xyz camera-relative position in meters (rebased against the primary camera), w unused
    glm::vec4 lightColor[kMaxLights];    ///< rgb intensity-scaled color, w unused
    int32_t   lightCount = 0;
    int32_t   _pad[3]    = {0, 0, 0};
};

/// Per-draw push constants used by box.vert.
struct DrawConstants {
    glm::mat4 model;
    glm::vec4 baseColor; ///< rgb base color, a unused
};

} // namespace GN::e2

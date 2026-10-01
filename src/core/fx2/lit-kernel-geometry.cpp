#include "pch.h"
#include <cmath>
#include <glm/gtc/packing.hpp>
#include <unordered_map>
namespace GN::fx2 {
namespace {
using namespace gpu2;
auto * logger = getLogger("GN.fx2.lit-geometry");

// Vertex with all 32-bit float attributes, matching lit kernel layout:
// location 0: position (vec3), location 1: normal (vec3), location 2: uv (vec2),
// location 3: tangent (vec4), location 4: color (vec4).
struct FullVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec4 tangent;
    glm::vec4 color;
};

// Build a RasterGeometry from CPU vertex/index arrays, selecting full or half precision
// and populating only the attribute slots requested by the options.
RasterGeometry buildGeometry(AutoRef<GpuContext> gpu, GpuCnC & uploads, const DynaArray<FullVertex> & cpuVerts, const DynaArray<uint32_t> & cpuIndices,
                             const LitKernelInputs::GeometryCreateOptions & opts) {
    using AF = RasterGeometry::AttributeFormat;
    RasterGeometry geometry;

    const bool     enabled[]    = {true, opts.normal, opts.uv, opts.tangent && opts.uv && opts.normal, opts.colored};
    const uint32_t components[] = {3, 3, 2, 4, 4};
    const uint32_t scalarSize   = opts.half ? 2 : 4;
    uint32_t       stride       = 0;
    for (uint32_t location = 0; location < 5; ++location) {
        if (!enabled[location]) continue;
        auto format = static_cast<AF>(static_cast<uint32_t>(opts.half ? AF::F16_1 : AF::F32_1) + components[location] - 1);
        geometry.format.attributes.append({.location = location, .binding = 0, .offset = stride, .format = format});
        stride += components[location] * scalarSize;
    }
    DynaArray<uint8_t> packed;
    if (!packed.resize(cpuVerts.size() * stride)) return {};
    for (size_t i = 0; i < cpuVerts.size(); ++i) {
        const auto &  v        = cpuVerts[i];
        const float * values[] = {&v.position.x, &v.normal.x, &v.uv.x, &v.tangent.x, &v.color.x};
        for (const auto & attribute : geometry.format.attributes) {
            auto * dst = packed.data() + i * stride + attribute.offset;
            for (uint32_t c = 0; c < components[attribute.location]; ++c) {
                const float value = values[attribute.location][c];
                if (opts.half) {
                    const uint16_t half = glm::packHalf1x16(value);
                    memcpy(dst + c * scalarSize, &half, scalarSize);
                } else {
                    memcpy(dst + c * scalarSize, &value, scalarSize);
                }
            }
        }
    }
    const bool          use16       = cpuVerts.size() <= 65536;
    const uint32_t      indexStride = use16 ? 2 : 4;
    DynaArray<uint16_t> indices16;
    if (use16) {
        if (!indices16.resize(cpuIndices.size())) return {};
        for (size_t i = 0; i < cpuIndices.size(); ++i) indices16[i] = static_cast<uint16_t>(cpuIndices[i]);
    }
    auto vb = Buffer::create("lit-geom.vertices", {.context = gpu, .size = packed.size()});
    auto ib = Buffer::create("lit-geom.indices", {.context = gpu, .size = cpuIndices.size() * indexStride});
    if (!vb || !ib) return {};
    // Allocate both resources before recording so allocation failure leaves uploads unchanged.
    uploads.uploadBuffer(vb, 0, {packed.data(), packed.size()});
    const auto * indexData = use16 ? reinterpret_cast<const uint8_t *>(indices16.data()) : reinterpret_cast<const uint8_t *>(cpuIndices.data());
    uploads.uploadBuffer(ib, 0, {indexData, cpuIndices.size() * indexStride});
    geometry.vertices.append({.buffer = vb, .offset = 0, .stride = stride});
    geometry.indices     = {.buffer = ib, .offset = 0, .stride = indexStride};
    geometry.vertexCount = static_cast<uint32_t>(cpuVerts.size());
    geometry.indexCount  = static_cast<uint32_t>(cpuIndices.size());
    return geometry;
}

// Table orientation matches the legacy gfx::createBox and the solids sample.
// clang-format off
static const glm::vec3 kFaceNormals[6] = {
    { 0,  0, -1}, // -Z  (front)
    { 0, -1,  0}, // -Y  (bottom)
    { 1,  0,  0}, // +X  (right)
    { 0,  1,  0}, // +Y  (top)
    {-1,  0,  0}, // -X  (left)
    { 0,  0,  1}, // +Z  (back)
};
// Corners run clockwise from outside; reverse triangle indices for CCW output.
// Indexed into the 8 corner positions of a unit cube [-0.5 .. +0.5].
// corners: 0(-,-,-) 1(+,-,-) 2(+,+,-) 3(-,+,-) 4(-,-,+) 5(+,-,+) 6(+,+,+) 7(-,+,+)
static const int kFaceCorners[6][4] = {
    {0, 1, 2, 3}, // -Z
    {0, 4, 5, 1}, // -Y
    {1, 5, 6, 2}, // +X
    {2, 6, 7, 3}, // +Y
    {3, 7, 4, 0}, // -X
    {7, 6, 5, 4}, // +Z
};
// clang-format on

void generateBox(DynaArray<FullVertex> & verts, DynaArray<uint32_t> & indices, float w, float h, float d, bool ccw, const glm::vec4 & vertexColor) {
    const float     hx = w * 0.5f, hy = h * 0.5f, hz = d * 0.5f;
    const glm::vec3 corners[8] = {
        {-hx, -hy, -hz}, {hx, -hy, -hz}, {hx, hy, -hz}, {-hx, hy, -hz}, {-hx, -hy, hz}, {hx, -hy, hz}, {hx, hy, hz}, {-hx, hy, hz},
    };
    static const glm::vec2 uvs[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    for (int f = 0; f < 6; ++f) {
        uint32_t base = static_cast<uint32_t>(verts.size());
        for (int v = 0; v < 4; ++v) {
            FullVertex fv;
            fv.position               = corners[kFaceCorners[f][v]];
            fv.normal                 = kFaceNormals[f];
            fv.uv                     = uvs[v];
            const glm::vec3 tangent   = glm::normalize(corners[kFaceCorners[f][1]] - corners[kFaceCorners[f][0]]);
            const glm::vec3 bitangent = corners[kFaceCorners[f][3]] - corners[kFaceCorners[f][0]];
            fv.tangent                = glm::vec4(tangent, glm::dot(glm::cross(fv.normal, tangent), bitangent) < 0 ? -1.0f : 1.0f);
            fv.color                  = vertexColor;
            verts.append(fv);
        }
        if (ccw) {
            indices.append(base + 2);
            indices.append(base + 1);
            indices.append(base + 0);
            indices.append(base + 3);
            indices.append(base + 2);
            indices.append(base + 0);
        } else {
            indices.append(base + 0);
            indices.append(base + 1);
            indices.append(base + 2);
            indices.append(base + 0);
            indices.append(base + 2);
            indices.append(base + 3);
        }
    }
}

void generateUVSphere(DynaArray<FullVertex> & verts, DynaArray<uint32_t> & indices, float radius, uint32_t slices, uint32_t stacks, bool ccw,
                      const glm::vec4 & vertexColor) {
    for (uint32_t i = 0; i <= stacks; ++i) {
        const float phi    = GN_PI * static_cast<float>(i) / static_cast<float>(stacks);
        const float sinPhi = i == 0 || i == stacks ? 0.0f : std::sin(phi);
        const float cosPhi = std::cos(phi);
        for (uint32_t j = 0; j <= slices; ++j) {
            const float theta    = j == slices ? 0.0f : GN_TWO_PI * static_cast<float>(j) / static_cast<float>(slices);
            const float sinTheta = std::sin(theta);
            const float cosTheta = std::cos(theta);
            glm::vec3   n(sinPhi * cosTheta, cosPhi, sinPhi * sinTheta);
            FullVertex  fv;
            fv.position = n * radius;
            fv.normal   = n;
            fv.uv       = {static_cast<float>(j) / slices, static_cast<float>(i) / stacks};
            fv.tangent  = glm::vec4(-sinTheta, 0.0f, cosTheta, 1.0f);
            fv.color    = vertexColor;
            verts.append(fv);
        }
    }
    for (uint32_t i = 0; i < stacks; ++i) {
        for (uint32_t j = 0; j < slices; ++j) {
            uint32_t first    = i * (slices + 1) + j;
            uint32_t second   = first + slices + 1;
            auto     triangle = [&](uint32_t a, uint32_t b, uint32_t c) {
                indices.append(a);
                indices.append(ccw ? b : c);
                indices.append(ccw ? c : b);
            };
            if (i != 0) triangle(first, first + 1, second);
            if (i + 1 != stacks) triangle(second, first + 1, second + 1);
        }
    }
}

// Edge key for midpoint caching during subdivision.
struct EdgeKey {
    uint32_t a, b;
    bool     operator==(const EdgeKey & o) const { return a == o.a && b == o.b; }
};
struct EdgeHash {
    size_t operator()(const EdgeKey & e) const {
        // Order-independent hash so (a,b) and (b,a) map to the same bucket.
        auto lo = std::min(e.a, e.b), hi = std::max(e.a, e.b);
        return std::hash<uint64_t>()(static_cast<uint64_t>(lo) << 32 | hi);
    }
};

void generateICOSphere(DynaArray<FullVertex> & verts, DynaArray<uint32_t> & indices, float radius, uint32_t subDivides, bool ccw,
                       const glm::vec4 & vertexColor) {
    // Start from a regular icosahedron with 12 vertices and 20 faces.
    const float t = (1.0f + std::sqrt(5.0f)) * 0.5f; // golden ratio
    // Normalize so the base icosahedron lies on a unit sphere.
    const float invLen = 1.0f / std::sqrt(1.0f + t * t);
    const float a = invLen, b = t * invLen;
    // clang-format off
    const glm::vec3 basePositions[12] = {
        {-a,  b,  0}, { a,  b,  0}, {-a, -b,  0}, { a, -b,  0},
        { 0, -a,  b}, { 0,  a,  b}, { 0, -a, -b}, { 0,  a, -b},
        { b,  0, -a}, { b,  0,  a}, {-b,  0, -a}, {-b,  0,  a},
    };
    // 20 triangles with CCW winding (viewed from outside).
    uint32_t baseFaces[20][3] = {
        { 0, 11,  5}, { 0,  5,  1}, { 0,  1,  7}, { 0,  7, 10}, { 0, 10, 11},
        { 1,  5,  9}, { 5, 11,  4}, {11, 10,  2}, {10,  7,  6}, { 7,  1,  8},
        { 3,  9,  4}, { 3,  4,  2}, { 3,  2,  6}, { 3,  6,  8}, { 3,  8,  9},
        { 4,  9,  5}, { 2,  4, 11}, { 6,  2, 10}, { 8,  6,  7}, { 9,  8,  1},
    };
    // clang-format on

    // Collect positions for subdivision. Normals/UVs are computed after subdivision.
    DynaArray<glm::vec3> positions;
    for (const auto & p : basePositions) positions.append(p);
    DynaArray<uint32_t> triIndices;
    for (const auto & f : baseFaces) {
        triIndices.append(f[0]);
        triIndices.append(f[1]);
        triIndices.append(f[2]);
    }

    // Subdivide: split each triangle into 4 by inserting midpoints on edges.
    for (uint32_t s = 0; s < subDivides; ++s) {
        std::unordered_map<EdgeKey, uint32_t, EdgeHash> midpoints;
        DynaArray<uint32_t>                             newIndices;
        auto                                            getMidpoint = [&](uint32_t i0, uint32_t i1) -> uint32_t {
            EdgeKey key {std::min(i0, i1), std::max(i0, i1)};
            auto    it = midpoints.find(key);
            if (it != midpoints.end()) return it->second;
            glm::vec3 mid = glm::normalize(positions[i0] + positions[i1]);
            uint32_t  idx = static_cast<uint32_t>(positions.size());
            positions.append(mid);
            midpoints[key] = idx;
            return idx;
        };
        for (size_t i = 0; i < triIndices.size(); i += 3) {
            uint32_t v0 = triIndices[i], v1 = triIndices[i + 1], v2 = triIndices[i + 2];
            uint32_t m01 = getMidpoint(v0, v1), m12 = getMidpoint(v1, v2), m20 = getMidpoint(v2, v0);
            newIndices.append(v0);
            newIndices.append(m01);
            newIndices.append(m20);
            newIndices.append(v1);
            newIndices.append(m12);
            newIndices.append(m01);
            newIndices.append(v2);
            newIndices.append(m20);
            newIndices.append(m12);
            newIndices.append(m01);
            newIndices.append(m12);
            newIndices.append(m20);
        }
        triIndices = std::move(newIndices);
    }

    // Build output vertices with normals, UVs, and tangents.
    for (const auto & p : positions) {
        glm::vec3  n = p; // already unit-length from normalization
        FullVertex fv;
        fv.position = n * radius;
        fv.normal   = n;
        // Spherical UV mapping: derive longitude/latitude from the normal.
        float u = 0.5f + std::atan2(n.z, n.x) / GN_TWO_PI;
        float v = 0.5f - std::asin(std::clamp(n.y, -1.0f, 1.0f)) / GN_PI;
        fv.uv   = {u, v};
        // Tangent aligned with longitude direction.
        float sinTheta = std::sin(std::atan2(n.z, n.x));
        float cosTheta = std::cos(std::atan2(n.z, n.x));
        fv.tangent     = glm::vec4(-sinTheta, 0.0f, cosTheta, 1.0f);
        fv.color       = vertexColor;
        verts.append(fv);
    }

    // Shared longitude-seam and pole vertices need distinct UVs and tangent frames per face.
    for (size_t i = 0; i < triIndices.size(); i += 3) {
        uint32_t   triangle[3] = {triIndices[i], triIndices[i + 1], triIndices[i + 2]};
        FullVertex face[3]     = {verts[triangle[0]], verts[triangle[1]], verts[triangle[2]]};
        bool       pole[3];
        float      minU = 1, maxU = 0;
        for (int j = 0; j < 3; ++j) {
            pole[j] = std::abs(face[j].normal.y) > 0.999999f;
            if (!pole[j]) {
                minU = std::min(minU, face[j].uv.x);
                maxU = std::max(maxU, face[j].uv.x);
            }
        }
        if (maxU - minU > 0.5f) {
            for (int j = 0; j < 3; ++j)
                if (!pole[j] && face[j].uv.x < 0.5f) face[j].uv.x += 1;
        }
        for (int j = 0; j < 3; ++j) {
            if (pole[j]) {
                face[j].uv.x      = (face[(j + 1) % 3].uv.x + face[(j + 2) % 3].uv.x) * 0.5f;
                const float theta = (face[j].uv.x - 0.5f) * GN_TWO_PI;
                face[j].tangent   = {-std::sin(theta), 0, std::cos(theta), 1};
            }
            if (pole[j] || face[j].uv.x != verts[triangle[j]].uv.x) {
                triangle[j] = static_cast<uint32_t>(verts.size());
                verts.append(face[j]);
            }
        }
        indices.append(triangle[0]);
        indices.append(triangle[ccw ? 1 : 2]);
        indices.append(triangle[ccw ? 2 : 1]);
    }
}
bool validOptions(const AutoRef<GpuContext> & gpu, const LitKernelInputs::GeometryCreateOptions & options) {
    if (!gpu) {
        GN_ERROR(logger, "Lit geometry requires a GPU context");
        return false;
    }
    if (!options.colored) return true;
    for (int i = 0; i < 3; ++i) {
        if (!std::isfinite(options.color[i])) {
            GN_ERROR(logger, "Lit geometry color[{}] must be finite, got {}", i, options.color[i]);
            return false;
        }
        if (options.half && std::abs(options.color[i]) > 65504.0f) {
            GN_ERROR(logger, "Lit geometry color[{}] exceeds the half-precision range [-65504, 65504], got {}", i, options.color[i]);
            return false;
        }
    }
    return true;
}

bool validSize(const char * name, float value, bool half) {
    if (!std::isfinite(value) || value <= 0) {
        GN_ERROR(logger, "Lit geometry {} must be finite and positive, got {}", name, value);
        return false;
    }
    if (half && value > 65504.0f) {
        GN_ERROR(logger, "Lit geometry {} exceeds the half-precision limit 65504, got {}", name, value);
        return false;
    }
    return true;
}
} // namespace

gpu2::RasterGeometry LitKernelInputs::createBox(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & uploads, const CubeCreateOptions & options) {
    if (!validOptions(gpu, options) || !validSize("width", options.width, options.half) || !validSize("height", options.height, options.half) ||
        !validSize("depth", options.depth, options.half))
        return {};
    DynaArray<FullVertex> verts;
    DynaArray<uint32_t>   idx;
    glm::vec4             vertColor(options.color, 1.0f);
    generateBox(verts, idx, options.width, options.height, options.depth, options.ccw, vertColor);
    return buildGeometry(gpu, uploads, verts, idx, options);
}

gpu2::RasterGeometry LitKernelInputs::createSphere(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & uploads, const SphereCreateOptions & options) {
    if (!validOptions(gpu, options) || !validSize("radius", options.radius, options.half)) return {};
    if (options.type == SphereCreateOptions::UV) {
        if (options.slices < 3 || options.stacks < 2) {
            GN_ERROR(logger, "Lit UV sphere requires at least 3 slices and 2 stacks, got {} slices and {} stacks", options.slices, options.stacks);
            return {};
        }
        // Bound work before allocating; also prevents index/count multiplication overflow.
        if ((uint64_t(options.slices) + 1) > (4 * 1024 * 1024) / (uint64_t(options.stacks) + 1)) {
            GN_ERROR(logger, "Lit UV sphere exceeds the 4194304-vertex limit: {} slices and {} stacks", options.slices, options.stacks);
            return {};
        }
    } else if (options.type == SphereCreateOptions::ICO) {
        if (options.subDivides > 8) {
            GN_ERROR(logger, "Lit ICO sphere subdivision level must be in [0, 8], got {}", options.subDivides);
            return {};
        }
    } else {
        GN_ERROR(logger, "Unknown lit sphere type {}", static_cast<uint32_t>(options.type));
        return {};
    }
    DynaArray<FullVertex> verts;
    DynaArray<uint32_t>   idx;
    glm::vec4             vertColor(options.color, 1.0f);
    if (options.type == SphereCreateOptions::ICO) {
        generateICOSphere(verts, idx, options.radius, options.subDivides, options.ccw, vertColor);
    } else {
        generateUVSphere(verts, idx, options.radius, options.slices, options.stacks, options.ccw, vertColor);
    }
    return buildGeometry(gpu, uploads, verts, idx, options);
}

} // namespace GN::fx2

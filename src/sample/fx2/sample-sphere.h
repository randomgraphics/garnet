#pragma once
#include <garnet/GNgpu2.h>
#include <glm/vec3.hpp>
#include <cmath>

// Sample-owned CPU generation and GPU upload; FX2 only sees ordinary buffer bindings.
inline GN::gpu2::RasterGeometry createSampleSphere(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::GpuCnC & uploads) {
    using namespace GN;
    using namespace GN::gpu2;
    struct Vertex {
        glm::vec3 position, normal;
    };
    DynaArray<Vertex>   vertices;
    DynaArray<uint32_t> indices;
    constexpr uint32_t  columns = 64, rows = 32;
    constexpr float     pi = 3.14159265358979323846f;
    for (uint32_t y = 0; y <= rows; ++y) {
        const float latitude = pi * static_cast<float>(y) / rows;
        for (uint32_t x = 0; x <= columns; ++x) {
            const float longitude = 2 * pi * static_cast<float>(x) / columns;
            glm::vec3   p(std::sin(latitude) * std::cos(longitude), std::cos(latitude), std::sin(latitude) * std::sin(longitude));
            vertices.append({p, p});
        }
    }
    for (uint32_t y = 0; y < rows; ++y)
        for (uint32_t x = 0; x < columns; ++x) {
            const uint32_t a = y * (columns + 1) + x, b = a + columns + 1;
            for (auto i : {a, a + 1, b, a + 1, b + 1, b}) indices.append(i);
        }
    auto vb = Buffer::create("sample.vertices", {.context = gpu, .size = vertices.size() * sizeof(Vertex)});
    auto ib = Buffer::create("sample.indices", {.context = gpu, .size = indices.size() * sizeof(uint32_t)});
    if (!vb || !ib) return {};
    uploads.uploadBuffer(vb, 0, {reinterpret_cast<const uint8_t *>(vertices.data()), vertices.size() * sizeof(Vertex)});
    uploads.uploadBuffer(ib, 0, {reinterpret_cast<const uint8_t *>(indices.data()), indices.size() * sizeof(uint32_t)});
    RasterGeometry geometry;
    geometry.vertices.append({.buffer = vb, .offset = 0, .stride = sizeof(Vertex)});
    geometry.indices     = {.buffer = ib, .offset = 0, .stride = sizeof(uint32_t)};
    geometry.vertexCount = static_cast<uint32_t>(vertices.size());
    geometry.indexCount  = static_cast<uint32_t>(indices.size());
    geometry.format.attributes.append({.location = 0, .binding = 0, .offset = offsetof(Vertex, position), .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.append({.location = 1, .binding = 0, .offset = offsetof(Vertex, normal), .format = RasterGeometry::AttributeFormat::F32_3});
    return geometry;
}

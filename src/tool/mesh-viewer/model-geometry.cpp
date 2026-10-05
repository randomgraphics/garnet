#include "model-scene.h"
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace GN::viewer {

bool collectModelInstances(const ModelScene & scene, DynaArray<ModelInstance> & result) {
    result.clear();
    DynaArray<glm::mat4>     transforms;
    DynaArray<ModelInstance> instances;
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        const auto & node = scene.nodes[i];
        if (node.parent < -1 || (node.parent >= 0 && static_cast<size_t>(node.parent) >= i)) return false;
        const auto transform = (node.parent < 0 ? glm::mat4(1) : transforms[node.parent]) * node.transform;
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                if (!std::isfinite(transform[c][r])) return false;
        if (transform[0][3] != 0 || transform[1][3] != 0 || transform[2][3] != 0 || transform[3][3] != 1) return false;
        const float determinant = glm::determinant(glm::mat3(transform));
        if (!std::isfinite(determinant) || determinant == 0) return false;
        transforms.append(transform);
        for (auto primitive : node.primitives) {
            if (primitive >= scene.primitives.size() || scene.primitives[primitive].material >= scene.materials.size()) return false;
            instances.append({primitive, transform});
        }
    }
    result = std::move(instances);
    return true;
}

AutoRef<ModelScene> ModelScene::createProcedural(bool sphere) {
    AutoRef<ModelScene> scene(new ModelScene(TYPE_INFO(), "procedural-model"));
    scene->sourcePath = sphere ? "generated://sphere" : "generated://box";
    scene->materials.append(Material {});
    Primitive p;
    p.name      = scene->sourcePath;
    p.texcoords = true;
    if (sphere) {
        constexpr uint32_t longitude = 32, latitude = 16;
        for (uint32_t y = 0; y <= latitude; ++y) {
            const float v     = static_cast<float>(y) / latitude;
            const float theta = v * glm::pi<float>();
            for (uint32_t x = 0; x <= longitude; ++x) {
                const float u   = static_cast<float>(x) / longitude;
                const float phi = u * glm::two_pi<float>();
                Vertex      vertex;
                vertex.normal   = {std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi)};
                vertex.position = vertex.normal;
                vertex.tangent  = {-std::sin(phi), 0, std::cos(phi), 1};
                vertex.texcoord = {u, v};
                p.vertices.append(vertex);
            }
        }
        for (uint32_t y = 0; y < latitude; ++y)
            for (uint32_t x = 0; x < longitude; ++x) {
                const uint32_t a = y * (longitude + 1) + x, b = a + longitude + 1;
                if (y != 0) {
                    p.indices.append(a);
                    p.indices.append(a + 1);
                    p.indices.append(b);
                }
                if (y + 1 != latitude) {
                    p.indices.append(a + 1);
                    p.indices.append(b + 1);
                    p.indices.append(b);
                }
            }
    } else {
        // Separate face vertices preserve hard normals and complete UV/tangent frames.
        for (int axis = 0; axis < 3; ++axis)
            for (float sign : {-1.f, 1.f}) {
                glm::vec3 n(0), u(0), v(0);
                n[axis]              = sign;
                u[(axis + 1) % 3]    = 1;
                v                    = glm::cross(n, u);
                const uint32_t first = static_cast<uint32_t>(p.vertices.size());
                for (auto uv : {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)}) {
                    Vertex vertex;
                    vertex.position = n + u * (2 * uv.x - 1) + v * (2 * uv.y - 1);
                    vertex.normal   = n;
                    vertex.tangent  = glm::vec4(u, 1);
                    vertex.texcoord = uv;
                    p.vertices.append(vertex);
                }
                for (uint32_t index : {0u, 1u, 2u, 0u, 2u, 3u}) p.indices.append(first + index);
            }
    }
    p.bounds      = {glm::vec3(-1), glm::vec3(1), true};
    scene->bounds = p.bounds;
    scene->primitives.append(std::move(p));
    Node node;
    node.name = scene->sourcePath;
    node.primitives.append(0);
    node.bounds = scene->bounds;
    scene->nodes.append(std::move(node));
    return scene;
}
} // namespace GN::viewer

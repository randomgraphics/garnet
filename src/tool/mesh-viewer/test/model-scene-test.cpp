#include <catch2/catch_test_macros.hpp>
#include "../model-scene.h"
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace GN::viewer;

TEST_CASE("viewer preserves nested affine instances without baking shared geometry", "[viewer][instances]") {
    auto scene = ModelScene::createProcedural(false);
    REQUIRE(scene);
    scene->nodes[0].transform[0][0] = -2.f;
    scene->nodes[0].transform[1][0] = 0.4f;
    scene->nodes[0].transform[1][1] = 3.f;
    scene->nodes[0].transform[3]    = glm::vec4(2, 3, 4, 1);
    ModelScene::Node child;
    child.parent       = 0;
    child.transform[3] = glm::vec4(1, 2, 3, 1);
    child.primitives.append(0);
    scene->nodes.append(child);
    GN::DynaArray<ModelInstance> instances;
    REQUIRE(collectModelInstances(*scene, instances));
    REQUIRE(instances.size() == 2);
    CHECK(instances[0].primitive == instances[1].primitive);
    const auto expected = scene->nodes[0].transform * child.transform;
    CHECK(instances[1].transform == expected);
    CHECK(glm::determinant(glm::mat3(instances[1].transform)) < 0);
    const auto & vertex = scene->primitives[0].vertices[0];
    CHECK(glm::length(glm::vec3(instances[1].transform * glm::vec4(vertex.position, 1)) - glm::vec3(expected * glm::vec4(vertex.position, 1))) < 0.00001f);
}

TEST_CASE("viewer rejects invalid instance hierarchy and singular transforms", "[viewer][instances]") {
    auto scene = ModelScene::createProcedural(true);
    REQUIRE(scene);
    GN::DynaArray<ModelInstance> instances;
    scene->nodes[0].parent = 0;
    CHECK_FALSE(collectModelInstances(*scene, instances));
    CHECK(instances.empty());
    scene->nodes[0].parent       = -1;
    scene->nodes[0].transform[0] = glm::vec4(0);
    CHECK_FALSE(collectModelInstances(*scene, instances));
}

TEST_CASE("viewer procedural meshes have outward triangles and valid tangent frames", "[viewer][model]") {
    for (bool sphere : {false, true}) {
        auto scene = ModelScene::createProcedural(sphere);
        REQUIRE(scene);
        const auto & p = scene->primitives[0];
        REQUIRE(p.indices.size() % 3 == 0);
        for (size_t i = 0; i < p.indices.size(); i += 3) {
            const auto a = p.vertices[p.indices[i]].position;
            const auto b = p.vertices[p.indices[i + 1]].position;
            const auto c = p.vertices[p.indices[i + 2]].position;
            CHECK(glm::dot(glm::cross(b - a, c - a), a + b + c) > 0);
        }
        for (const auto & vertex : p.vertices) {
            CHECK(std::abs(glm::length(vertex.normal) - 1.f) < 0.00001f);
            CHECK(std::abs(glm::dot(vertex.normal, glm::vec3(vertex.tangent))) < 0.00001f);
        }
    }
}

TEST_CASE("viewer::ModelScene classifies supported source formats", "[viewer][model]") {
    CHECK(classifyModelSourcePath("scene.fbx") == ModelSourceFormat::FBX);
    CHECK(classifyModelSourcePath("scene.gltf") == ModelSourceFormat::GLTF);
    CHECK(classifyModelSourcePath("scene.glb") == ModelSourceFormat::GLB);
    CHECK(classifyModelSourcePath("scene.stl") == ModelSourceFormat::STL);
}

TEST_CASE("viewer::ModelScene source classification is case insensitive", "[viewer][model]") {
    CHECK(classifyModelSourcePath("scene.FBX") == ModelSourceFormat::FBX);
    CHECK(classifyModelSourcePath("scene.GlTf") == ModelSourceFormat::GLTF);
    CHECK(classifyModelSourcePath("scene.GLB") == ModelSourceFormat::GLB);
    CHECK(classifyModelSourcePath("scene.StL") == ModelSourceFormat::STL);
}

TEST_CASE("viewer::ModelScene rejects unsupported source extensions", "[viewer][model]") {
    CHECK(classifyModelSourcePath("") == ModelSourceFormat::UNKNOWN);
    CHECK(classifyModelSourcePath("scene") == ModelSourceFormat::UNKNOWN);
    CHECK(classifyModelSourcePath("scene.obj") == ModelSourceFormat::UNKNOWN);
    CHECK(classifyModelSourcePath("scene.fbx.backup") == ModelSourceFormat::UNKNOWN);
}

TEST_CASE("viewer::ModelScene creates bounded debug axes without renderer dependencies", "[viewer][model]") {
    const ModelScene::Bounds bounds {.minimum = {-2, -1, -3}, .maximum = {4, 5, 6}, .valid = true};
    const auto               scene = ModelScene::createDebugVisualization(bounds, 0.01f);
    REQUIRE(scene);
    // One material/primitive per color: the cage plus three tripod axes.
    REQUIRE(scene->materials.size() == 4);
    REQUIRE(scene->primitives.size() == 4);
    for (size_t i = 0; i < scene->materials.size(); ++i) {
        CHECK(scene->materials[i].workflow == ModelScene::MaterialWorkflow::UNLIT);
        CHECK(scene->primitives[i].material == i);
        CHECK(scene->primitives[i].bounds.valid);
    }
    CHECK(scene->materials[0].baseColor == glm::vec4(1.0f, 0.8f, 0.1f, 1.0f));
    // The cage is 12 segments of 8 corners; each axis is a single segment.
    CHECK(scene->primitives[0].vertices.size() == 12 * 8);
    CHECK(scene->primitives[0].indices.size() == 12 * 36);
    CHECK(scene->primitives[1].indices.size() == 36);
    CHECK(scene->primitives[3].indices.size() == 36);
    REQUIRE(scene->nodes.size() == 1);
    CHECK(scene->nodes[0].primitives.size() == 4);
    CHECK(scene->bounds.valid);

    const auto boundsOnly = ModelScene::createDebugVisualization(bounds, 0.01f, true, false);
    const auto axesOnly   = ModelScene::createDebugVisualization(bounds, 0.01f, false, true);
    REQUIRE(boundsOnly);
    REQUIRE(axesOnly);
    REQUIRE(boundsOnly->primitives.size() == 1);
    CHECK(boundsOnly->primitives[0].indices.size() == 12 * 36);
    REQUIRE(axesOnly->primitives.size() == 3);
    for (const auto & axis : axesOnly->primitives) CHECK(axis.indices.size() == 36);
    CHECK_FALSE(ModelScene::createDebugVisualization(bounds, 0.01f, false, false));
}

static GN::StrA repositoryPath(const char * relativePath) {
    const GN::StrA root = GN::getEnv("GARNET_ROOT");
    return root.empty() ? GN::StrA(relativePath) : GN::fs::joinPath(root, relativePath);
}

static void checkImportedScene(const GN::AutoRef<ModelScene> & scene, ModelSourceFormat format) {
    REQUIRE(scene);
    CHECK(classifyModelSourcePath(scene->sourcePath) == format);
    CHECK_FALSE(scene->sourcePath.empty());
    REQUIRE_FALSE(scene->primitives.empty());
    REQUIRE_FALSE(scene->nodes.empty());
    CHECK(scene->bounds.valid);
    CHECK(scene->nodes[0].parent == -1);

    for (const auto & primitive : scene->primitives) {
        CHECK_FALSE(primitive.vertices.empty());
        CHECK_FALSE(primitive.indices.empty());
        CHECK((primitive.indices.size() % 3) == 0);
        CHECK(primitive.bounds.valid);
        for (uint32_t index : primitive.indices) CHECK(index < primitive.vertices.size());
    }
    for (size_t nodeIndex = 1; nodeIndex < scene->nodes.size(); ++nodeIndex) {
        CHECK(scene->nodes[nodeIndex].parent >= 0);
        CHECK(static_cast<size_t>(scene->nodes[nodeIndex].parent) < nodeIndex);
    }
}

TEST_CASE("viewer::ModelScene imports compact repository models", "[viewer][model]") {
    struct Fixture {
        const char *      path;
        ModelSourceFormat format;
    };
    constexpr Fixture fixtures[] = {
        {"src/3rdparty/assimp/test/models/FBX/phong_cube.fbx", ModelSourceFormat::FBX},
        {"src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF-Embedded/BoxTextured.gltf", ModelSourceFormat::GLTF},
        {"src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF-Binary/BoxTextured.glb", ModelSourceFormat::GLB},
        {"src/3rdparty/assimp/test/models/STL/triangle.stl", ModelSourceFormat::STL},
    };

    for (const Fixture & fixture : fixtures) {
        CAPTURE(fixture.path);
        const GN::StrA path = repositoryPath(fixture.path);
        if (!GN::fs::isFile(path)) SKIP("Assimp model corpus is not initialized");
        checkImportedScene(ModelScene::load({.path = path}), fixture.format);
    }
}

TEST_CASE("viewer::ModelScene imports repository ASE geometry and materials", "[viewer][model]") {
    CHECK(classifyModelSourcePath("scene.ase") == ModelSourceFormat::ASE);
    CHECK(classifyModelSourcePath("scene.AsE") == ModelSourceFormat::ASE);
    const auto scene = ModelScene::load({.path = repositoryPath("media/boxes/boxes.ase")});
    checkImportedScene(scene, ModelSourceFormat::ASE);
    REQUIRE_FALSE(scene->materials.empty());
    for (const auto & material : scene->materials) { CHECK(material.workflow == ModelScene::MaterialWorkflow::DEFAULT_LIT); }
}

TEST_CASE("viewer::ModelScene reports missing supported model", "[viewer][model]") { CHECK_FALSE(ModelScene::load({.path = "missing-model.fbx"})); }

TEST_CASE("viewer::ModelScene normalizes PBR material and embedded textures", "[viewer][model]") {
    const GN::StrA path = repositoryPath("src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF-Embedded/BoxTextured.gltf");
    if (!GN::fs::isFile(path)) SKIP("Assimp model corpus is not initialized");

    const auto scene = ModelScene::load({.path = path});
    REQUIRE(scene);
    REQUIRE_FALSE(scene->materials.empty());
    CHECK(scene->materials[0].workflow == ModelScene::MaterialWorkflow::METALLIC_ROUGHNESS);
    REQUIRE_FALSE(scene->textures.empty());
    CHECK_FALSE(scene->textures[0].embeddedData.empty());
    CHECK(scene->textures[0].path.empty());
    REQUIRE(scene->materials[0].baseColorMap >= 0);
    CHECK(scene->textures[scene->materials[0].baseColorMap].srgb);
    // The source glTF's +Z face maps bottom vertices to V=0 and top vertices
    // to V=1. Preserve that convention for the unflipped uploaded image rows.
    size_t checkedVertices = 0;
    for (const auto & primitive : scene->primitives) {
        for (const auto & vertex : primitive.vertices) {
            if (vertex.normal.z < 0.99f) continue;
            CHECK(glm::abs(vertex.texcoord.y - (vertex.position.y + 0.5f)) < 0.00001f);
            CHECK(glm::abs(vertex.texcoord.x - (5.5f - vertex.position.x)) < 0.00001f);
            CHECK(vertex.tangent.w == -1.0f);
            CHECK(glm::abs(glm::dot(vertex.normal, glm::vec3(vertex.tangent))) < 0.00001f);
            ++checkedVertices;
        }
    }
    CHECK(checkedVertices == 4);
}

TEST_CASE("viewer::ModelScene supplies unit normals when source normals are absent", "[viewer][model]") {
    const GN::StrA path = repositoryPath("src/3rdparty/assimp/test/models/glTF2/simple_skin/simple_skin.gltf");
    if (!GN::fs::isFile(path)) SKIP("Assimp model corpus is not initialized");

    const auto scene = ModelScene::load({.path = path});
    REQUIRE(scene);
    REQUIRE_FALSE(scene->primitives.empty());
    for (const auto & vertex : scene->primitives[0].vertices) CHECK(glm::abs(glm::length(vertex.normal) - 1.0f) < 0.0001f);
}

TEST_CASE("viewer::ModelScene resolves external textures and default-lit content", "[viewer][model]") {
    const GN::StrA gltfPath = repositoryPath("src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF/BoxTextured.gltf");
    const GN::StrA stlPath  = repositoryPath("src/3rdparty/assimp/test/models/STL/triangle.stl");
    if (!GN::fs::isFile(gltfPath) || !GN::fs::isFile(stlPath)) SKIP("Assimp model corpus is not initialized");

    const auto gltf = ModelScene::load({.path = gltfPath});
    REQUIRE(gltf);
    REQUIRE_FALSE(gltf->textures.empty());
    CHECK_FALSE(gltf->textures[0].path.empty());
    CHECK(gltf->textures[0].embeddedData.empty());
    CHECK(GN::fs::isFile(gltf->textures[0].path));

    const auto stl = ModelScene::load({.path = stlPath});
    REQUIRE(stl);
    REQUIRE_FALSE(stl->materials.empty());
    CHECK(stl->materials[0].workflow == ModelScene::MaterialWorkflow::DEFAULT_LIT);
    CHECK(stl->materials[0].roughness == 1.0f);
    CHECK(stl->materials[0].metallic == 0.0f);
}

TEST_CASE("viewer::ModelScene imports project media corpus", "[viewer][model][media]") {
    struct Fixture {
        const char * path;
        bool         supportedByAssimp;
    };
    constexpr Fixture fixtures[] = {
        {"media/boxes/boxes.fbx", true},
        {"media/model/R.F.R01/a01.fbx", true},
        {"media/asset-foundry/model/humanoid/humanoid.fbx", true},
        {"media/asset-foundry/model/humanoid/humanoid_ascii.fbx", true},
        {"media/asset-foundry/model/DamagedHelmet/DamagedHelmet.gltf", true},
    };

    for (const Fixture & fixture : fixtures) {
        CAPTURE(fixture.path);
        const GN::StrA path = repositoryPath(fixture.path);
        if (!GN::fs::isFile(path)) SKIP("Project media or Asset Foundry submodule is not initialized");
        const auto scene = ModelScene::load({.path = path});
        if (!fixture.supportedByAssimp) {
            CHECK_FALSE(scene);
            continue;
        }
        REQUIRE(scene);
        CHECK(scene->bounds.valid);
        CHECK_FALSE(scene->primitives.empty());
    }
}

TEST_CASE("viewer::ModelScene imports Asset Foundry Digital Forge stress model", "[viewer][model][media][.stress]") {
    const GN::StrA path = repositoryPath("media/asset-foundry/model/speeder-getaway/speeder-getaway-from-meshy-ai.glb");
    if (!GN::fs::isFile(path)) SKIP("Asset Foundry submodule is not initialized");

    const auto scene = ModelScene::load({.path = path});
    REQUIRE(scene);
    CHECK(classifyModelSourcePath(scene->sourcePath) == ModelSourceFormat::GLB);
    CHECK(scene->bounds.valid);
    CHECK_FALSE(scene->primitives.empty());
    CHECK_FALSE(scene->nodes.empty());
    CHECK_FALSE(scene->materials.empty());
}

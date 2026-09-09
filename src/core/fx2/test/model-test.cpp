#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include <garnet/base/filesys.h>

using namespace GN::fx2;

TEST_CASE("fx2::ModelScene classifies supported source formats", "[fx2][model]") {
    CHECK(ModelScene::sourceFormatFromPath("scene.fbx") == ModelScene::SourceFormat::FBX);
    CHECK(ModelScene::sourceFormatFromPath("scene.gltf") == ModelScene::SourceFormat::GLTF);
    CHECK(ModelScene::sourceFormatFromPath("scene.glb") == ModelScene::SourceFormat::GLB);
    CHECK(ModelScene::sourceFormatFromPath("scene.stl") == ModelScene::SourceFormat::STL);
}

TEST_CASE("fx2::ModelScene source classification is case insensitive", "[fx2][model]") {
    CHECK(ModelScene::sourceFormatFromPath("scene.FBX") == ModelScene::SourceFormat::FBX);
    CHECK(ModelScene::sourceFormatFromPath("scene.GlTf") == ModelScene::SourceFormat::GLTF);
    CHECK(ModelScene::sourceFormatFromPath("scene.GLB") == ModelScene::SourceFormat::GLB);
    CHECK(ModelScene::sourceFormatFromPath("scene.StL") == ModelScene::SourceFormat::STL);
}

TEST_CASE("fx2::ModelScene rejects unsupported source extensions", "[fx2][model]") {
    CHECK(ModelScene::sourceFormatFromPath("") == ModelScene::SourceFormat::UNKNOWN);
    CHECK(ModelScene::sourceFormatFromPath("scene") == ModelScene::SourceFormat::UNKNOWN);
    CHECK(ModelScene::sourceFormatFromPath("scene.obj") == ModelScene::SourceFormat::UNKNOWN);
    CHECK(ModelScene::sourceFormatFromPath("scene.fbx.backup") == ModelScene::SourceFormat::UNKNOWN);
}

static GN::StrA repositoryPath(const char * relativePath) {
    const GN::StrA root = GN::getEnv("GARNET_ROOT");
    return root.empty() ? GN::StrA(relativePath) : GN::fs::joinPath(root, relativePath);
}

static void checkImportedScene(const GN::AutoRef<ModelScene> & scene, ModelScene::SourceFormat format) {
    REQUIRE(scene);
    CHECK(scene->sourceFormat == format);
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

TEST_CASE("fx2::ModelScene imports compact repository models", "[fx2][model]") {
    struct Fixture {
        const char *             path;
        ModelScene::SourceFormat format;
    };
    constexpr Fixture fixtures[] = {
        {"src/3rdparty/assimp/test/models/FBX/phong_cube.fbx", ModelScene::SourceFormat::FBX},
        {"src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF-Embedded/BoxTextured.gltf", ModelScene::SourceFormat::GLTF},
        {"src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF-Binary/BoxTextured.glb", ModelScene::SourceFormat::GLB},
        {"src/3rdparty/assimp/test/models/STL/triangle.stl", ModelScene::SourceFormat::STL},
    };

    for (const Fixture & fixture : fixtures) {
        CAPTURE(fixture.path);
        const GN::StrA path = repositoryPath(fixture.path);
        if (!GN::fs::isFile(path)) SKIP("Assimp model corpus is not initialized");
        checkImportedScene(ModelScene::load({.path = path}), fixture.format);
    }
}

TEST_CASE("fx2::ModelScene reports missing supported model", "[fx2][model]") { CHECK_FALSE(ModelScene::load({.path = "missing-model.fbx"})); }

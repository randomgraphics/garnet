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

TEST_CASE("fx2::ModelScene creates bounded debug axes without renderer dependencies", "[fx2][model]") {
    const ModelScene::Bounds bounds {.minimum = {-2, -1, -3}, .maximum = {4, 5, 6}, .valid = true};
    const auto               scene = ModelScene::createDebugVisualization(bounds, 0.01f);
    REQUIRE(scene);
    REQUIRE(scene->materials.size() == 1);
    CHECK(scene->materials[0].workflow == ModelScene::MaterialWorkflow::UNLIT);
    REQUIRE(scene->primitives.size() == 1);
    CHECK(scene->primitives[0].sourceHadColors);
    CHECK(scene->primitives[0].vertices.size() == 15 * 8);
    CHECK(scene->primitives[0].indices.size() == 15 * 36);
    CHECK(scene->primitives[0].bounds.valid);
    REQUIRE(scene->nodes.size() == 1);
    CHECK(scene->nodes[0].primitives.size() == 1);

    const auto boundsOnly = ModelScene::createDebugVisualization(bounds, 0.01f, true, false);
    const auto axesOnly   = ModelScene::createDebugVisualization(bounds, 0.01f, false, true);
    REQUIRE(boundsOnly);
    REQUIRE(axesOnly);
    CHECK(boundsOnly->primitives[0].indices.size() == 12 * 36);
    CHECK(axesOnly->primitives[0].indices.size() == 3 * 36);
    CHECK_FALSE(ModelScene::createDebugVisualization(bounds, 0.01f, false, false));
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

TEST_CASE("fx2::ModelScene imports repository ASE geometry and materials", "[fx2][model]") {
    CHECK(ModelScene::sourceFormatFromPath("scene.ase") == ModelScene::SourceFormat::ASE);
    CHECK(ModelScene::sourceFormatFromPath("scene.AsE") == ModelScene::SourceFormat::ASE);
    const auto scene = ModelScene::load({.path = repositoryPath("media/boxes/boxes.ase")});
    checkImportedScene(scene, ModelScene::SourceFormat::ASE);
    REQUIRE_FALSE(scene->materials.empty());
    for (const auto & material : scene->materials) { CHECK(material.workflow == ModelScene::MaterialWorkflow::DEFAULT_LIT); }
}

TEST_CASE("fx2::ModelScene reports missing supported model", "[fx2][model]") { CHECK_FALSE(ModelScene::load({.path = "missing-model.fbx"})); }

TEST_CASE("fx2::ModelScene normalizes PBR material and embedded textures", "[fx2][model]") {
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

TEST_CASE("fx2::ModelScene supplies unit normals when source normals are absent", "[fx2][model]") {
    const GN::StrA path = repositoryPath("src/3rdparty/assimp/test/models/glTF2/simple_skin/simple_skin.gltf");
    if (!GN::fs::isFile(path)) SKIP("Assimp model corpus is not initialized");

    const auto scene = ModelScene::load({.path = path});
    REQUIRE(scene);
    REQUIRE_FALSE(scene->primitives.empty());
    CHECK_FALSE(scene->primitives[0].sourceHadNormals);
    for (const auto & vertex : scene->primitives[0].vertices) CHECK(glm::abs(glm::length(vertex.normal) - 1.0f) < 0.0001f);
}

TEST_CASE("fx2::ModelScene resolves external textures and default-lit content", "[fx2][model]") {
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

TEST_CASE("fx2::ModelScene imports project media corpus", "[fx2][model][media]") {
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

TEST_CASE("fx2::ModelScene imports Asset Foundry Digital Forge stress model", "[fx2][model][media][.stress]") {
    const GN::StrA path = repositoryPath("media/asset-foundry/model/character/speeder-getaway-from-meshy-ai.glb");
    if (!GN::fs::isFile(path)) SKIP("Asset Foundry submodule is not initialized");

    const auto scene = ModelScene::load({.path = path});
    REQUIRE(scene);
    CHECK(scene->sourceFormat == ModelScene::SourceFormat::GLB);
    CHECK(scene->bounds.valid);
    CHECK_FALSE(scene->primitives.empty());
    CHECK_FALSE(scene->nodes.empty());
    CHECK_FALSE(scene->materials.empty());
}

TEST_CASE("fx2::ModelAsset records immutable geometry and texture uploads once", "[fx2][model][gpu]") {
    const GN::StrA path = repositoryPath("src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF-Embedded/BoxTextured.gltf");
    if (!GN::fs::isFile(path)) SKIP("Assimp model corpus is not initialized");
    const auto gpu = GN::gpu2::GpuContext::create("model-asset-test",
                                                  GN::gpu2::GpuContext::CreateParameters {.howToPrintDeviceCaps = GN::gpu2::GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 context is available");

    const auto scene = ModelScene::load({.path = path});
    REQUIRE(scene);
    const auto asset = ModelAsset::create(gpu, scene);
    REQUIRE(asset);
    REQUIRE(asset->gpuPayload);
    CHECK(asset->scene.get() == scene.get());
    CHECK(asset->primitives.size() == scene->primitives.size());
    CHECK(asset->textures.size() == scene->textures.size());
    REQUIRE_FALSE(asset->primitives.empty());
    REQUIRE_FALSE(asset->textures.empty());
    CHECK(asset->textures[0]);
    CHECK(asset->primitives[0].vertexCount == scene->primitives[0].vertices.size());
    CHECK(asset->primitives[0].indexCount == scene->primitives[0].indices.size());

    const auto shading = ModelShading::create(gpu);
    const auto ssc     = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(shading);
    REQUIRE(shading->gpuPayload);
    REQUIRE(ssc);
    const auto draw = ModelShading::getDrawParams(ssc->takeSnapshot(), shading, asset, 0, glm::mat4(1.0f));
    CHECK(draw.vs);
    CHECK(draw.ps);
    CHECK(draw.geometry.indexCount == scene->primitives[0].indices.size());
    REQUIRE(draw.resources.size() == 2);
    CHECK(draw.resources[1].size() == 6);
}

TEST_CASE("fx2::ModelShading builds draws for glTF FBX STL and ASE materials", "[fx2][model][gpu]") {
    constexpr const char * paths[] = {
        "media/boxes/boxes.ase",
        "src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF-Embedded/BoxTextured.gltf",
        "src/3rdparty/assimp/test/models/FBX/phong_cube.fbx",
        "src/3rdparty/assimp/test/models/STL/triangle.stl",
    };
    const auto gpu = GN::gpu2::GpuContext::create("model-shading-test",
                                                  GN::gpu2::GpuContext::CreateParameters {.howToPrintDeviceCaps = GN::gpu2::GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 context is available");
    const auto shading = ModelShading::create(gpu);
    const auto ssc     = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(shading);
    REQUIRE(ssc);
    const auto snapshot = ssc->takeSnapshot();

    for (const char * relativePath : paths) {
        CAPTURE(relativePath);
        const GN::StrA path = repositoryPath(relativePath);
        if (!GN::fs::isFile(path)) SKIP("Assimp model corpus is not initialized");
        const auto scene = ModelScene::load({.path = path});
        REQUIRE(scene);
        const auto model = ModelAsset::create(gpu, scene);
        REQUIRE(model);
        for (uint32_t primitiveIndex = 0; primitiveIndex < model->primitives.size(); ++primitiveIndex) {
            const auto draw = ModelShading::getDrawParams(snapshot, shading, model, primitiveIndex, glm::mat4(1.0f));
            CHECK(draw.vs);
            CHECK(draw.ps);
            CHECK(draw.geometry.indexCount > 0);
            CHECK(draw.resources.size() == 2);
            CHECK(draw.resources[1].size() == 6);
        }
    }
}

TEST_CASE("fx2::ModelShading consumes Asset Foundry environment resources", "[fx2][model][gpu][media]") {
    const GN::StrA environmentDirectory = repositoryPath("media/asset-foundry/image/envmap/bad-salzbrunn-walking-hall");
    const GN::StrA skyboxPath           = GN::fs::joinPath(environmentDirectory, "skybox-cube.dds");
    const GN::StrA irradiancePath       = GN::fs::joinPath(environmentDirectory, "irradiance.dds");
    const GN::StrA prefilteredPath      = GN::fs::joinPath(environmentDirectory, "prefiltered.dds");
    const GN::StrA brdfPath             = GN::fs::joinPath(environmentDirectory, "brdf_lut.dds");
    if (!GN::fs::isFile(skyboxPath) || !GN::fs::isFile(irradiancePath) || !GN::fs::isFile(prefilteredPath) || !GN::fs::isFile(brdfPath)) {
        SKIP("Asset Foundry environment maps are not initialized");
    }
    const auto gpu = GN::gpu2::GpuContext::create("model-environment-test",
                                                  GN::gpu2::GpuContext::CreateParameters {.howToPrintDeviceCaps = GN::gpu2::GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 context is available");
    const auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);
    ssc->set0.envLighting = {.skyboxPath = skyboxPath, .irradiancePath = irradiancePath, .prefilteredPath = prefilteredPath, .brdfLutPath = brdfPath};
    const auto snapshot   = ssc->takeSnapshot();
    REQUIRE(snapshot.set0Resources.size() == 6);
    REQUIRE(snapshot.set0Resources[2].size() == 1);
    REQUIRE(snapshot.set0Resources[3].size() == 1);
    REQUIRE(snapshot.set0Resources[4].size() == 1);
    REQUIRE(snapshot.set0Resources[5].size() == 1);
    CHECK(snapshot.set0Resources[2][0].texture()->descriptor().faces == 6);
    CHECK(snapshot.set0Resources[3][0].texture()->descriptor().faces == 6);
    CHECK(snapshot.set0Resources[4][0].texture()->descriptor().faces == 6);
    CHECK(snapshot.set0Resources[5][0].texture()->descriptor().faces == 1);
    CHECK(snapshot.set0Payloads.size() >= 2);
}

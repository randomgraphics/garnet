#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include "../bindless-sky-material-impl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using namespace GN;
using namespace GN::fx2::bindless;
using namespace GN::gpu2;
namespace gpuBindless = GN::gpu2::bindless;

TEST_CASE("FX2 bindless skybox renders fallback and custom environment maps", "[fx2][bindless][sky][gpu]") {
    auto gpu = GpuContext::create("bindless-sky-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto heap = gpuBindless::DescriptorHeap::create("bindless-sky-test.heap", {.gpu = gpu, .capacity = 16, .materialCapacity = 1024});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-sky-test.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);

    auto kernel = SkyKernel::create(*heap, *initialization);
    REQUIRE(kernel);
    const uint32_t baseHeapSize = heap->size();
    CHECK(baseHeapSize > 0);

    // 1. Default sky material using built-in fallback sky-blue cubemap
    auto defaultParams   = kernel->defaultMaterialParameters();
    auto defaultMaterial = kernel->createMaterial(*initialization, defaultParams);
    REQUIRE(defaultMaterial);
    CHECK(getSkyMaterialIndex(defaultMaterial.get()) != uint32_t(-1));

    // 2. Custom sky material with a solid-color red cubemap
    const auto cubeDesc = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(6).setLevels(1);
    auto       redCube  = Texture::create("bindless-sky-test.red-cube", {.context = gpu, .descriptor = cubeDesc});
    REQUIRE(redCube);

    const uint8_t            redPixels[6 * 4] = {255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255};
    gpuBindless::CnC::Region regions[6];
    for (uint32_t f = 0; f < 6; ++f) {
        regions[f].face        = f;
        regions[f].dataOffset  = f * 4;
        regions[f].imageExtent = {1, 1, 1};
    }
    initialization->recordUploadImage(redCube, {redPixels, sizeof(redPixels)}, {regions, 6});

    auto customParams           = defaultParams;
    customParams.skyboxMap      = GpuResourceView {redCube}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    customParams.luminanceScale = 1.0f;

    auto customMaterial = kernel->createMaterial(*initialization, customParams);
    REQUIRE(customMaterial);
    CHECK(getSkyMaterialIndex(customMaterial.get()) != uint32_t(-1));
    CHECK(getSkyMaterialIndex(customMaterial.get()) != getSkyMaterialIndex(defaultMaterial.get()));

    auto initWork = initialization->seal();
    REQUIRE(initWork);

    // 3. Shared constants and camera
    auto ssc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 4096, .streamingCapacity = 1024});
    REQUIRE(ssc);

    SharedUniforms uniforms;
    uniforms.cameraPosition   = {0, 0, 0, 1};
    uniforms.exposure         = 1.0f;
    uniforms.renderTargetSize = {16, 16};
    uniforms.viewMatrix       = glm::lookAtRH(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    uniforms.projMatrix       = glm::perspectiveRH_ZO(glm::radians(60.0f), 1.0f, uniforms.nearPlane, uniforms.farPlane);
    uniforms.projMatrix[1][1] *= -1; // Vulkan clip space
    uniforms.projViewMatrix = uniforms.projMatrix * uniforms.viewMatrix;

    auto uniformProducer = gpuBindless::CnC::create("bindless-sky-test.uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(uniformProducer);
    auto state = ssc->recordUniformUpdate(*uniformProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    REQUIRE(state);
    auto uniformWork = uniformProducer->seal();
    REQUIRE(uniformWork);

    // 4. Render custom red skybox to 16x16 offscreen target
    const auto targetDesc   = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(16, 16).setLevels(1);
    auto       renderTarget = Texture::create("bindless-sky-test.target", {.context = gpu, .descriptor = targetDesc});
    REQUIRE(renderTarget);

    RasterTarget target;
    target.setColorTarget(0, GpuResourceView {renderTarget}).setClearColor(0, 0, 0, 1);
    target.states.cullMode = RasterState::CULL_NONE;

    auto raster = gpuBindless::Raster::create(
        "bindless-sky-test.raster", {.gpu = gpu, .target = &target, .heap = heap, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
    REQUIRE(raster);

    SkyMaterial::DrawParameters drawParams {*raster, state};
    REQUIRE(customMaterial->record(drawParams));

    auto drawWork = raster->seal();
    REQUIRE(drawWork);

    gpu->submit(GpuContext::SubmitParameters("bindless-sky-test.submit").appendWork(initWork).appendWork(uniformWork).appendWork(drawWork));
    gpu->waitForIdle();

    // 5. Read back pixels and verify red environment is rendered
    auto image = renderTarget->readback();
    REQUIRE(!image.empty());
    const uint8_t * pixels = reinterpret_cast<const uint8_t *>(image.data());
    // Check center pixel (8, 8)
    const uint32_t centerPixelOffset = (8 * 16 + 8) * 4;
    const uint8_t  r                 = pixels[centerPixelOffset + 0];
    const uint8_t  g                 = pixels[centerPixelOffset + 1];
    const uint8_t  b                 = pixels[centerPixelOffset + 2];
    const uint8_t  a                 = pixels[centerPixelOffset + 3];

    // Red cubemap should produce strong red, near-zero green/blue
    CHECK(r > 100);
    CHECK(g < 10);
    CHECK(b < 10);
    CHECK(a == 255);

    // 6. Also render default skybox to verify fallback sky-blue cubemap
    auto defaultRaster =
        gpuBindless::Raster::create("bindless-sky-test.default-raster",
                                    {.gpu = gpu, .target = &target, .heap = heap, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
    REQUIRE(defaultRaster);
    REQUIRE(defaultMaterial->record({*defaultRaster, state}));
    auto defaultDrawWork = defaultRaster->seal();
    REQUIRE(defaultDrawWork);

    gpu->submit(GpuContext::SubmitParameters("bindless-sky-test.default-submit").appendWork(defaultDrawWork));
    gpu->waitForIdle();

    auto defaultImage = renderTarget->readback();
    REQUIRE(!defaultImage.empty());
    const uint8_t * defPixels = reinterpret_cast<const uint8_t *>(defaultImage.data());
    const uint8_t   defR      = defPixels[centerPixelOffset + 0];
    const uint8_t   defG      = defPixels[centerPixelOffset + 1];
    const uint8_t   defB      = defPixels[centerPixelOffset + 2];
    // Fallback is sky-blue (#87CEEB: R=135, G=206, B=235), so B > G > R
    CHECK(defB > defR);
    CHECK(defG > defR);
}

TEST_CASE("FX2 bindless PBR renders with sky material environment lighting", "[fx2][bindless][pbr][sky][gpu]") {
    auto gpu = GpuContext::create("bindless-pbr-sky-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto heap = gpuBindless::DescriptorHeap::create("bindless-pbr-sky-test.heap", {.gpu = gpu, .capacity = 32, .materialCapacity = 4096});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-pbr-sky-test.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);

    auto skyKernel = SkyKernel::create(*heap, *initialization);
    REQUIRE(skyKernel);
    auto pbrKernel = PbrKernel::create(*heap, *initialization);
    REQUIRE(pbrKernel);

    auto skyMaterial = skyKernel->createMaterial(*initialization, skyKernel->defaultMaterialParameters());
    REQUIRE(skyMaterial);

    auto pbrParams      = pbrKernel->defaultMaterialParameters();
    pbrParams.baseColor = {1.0f, 1.0f, 1.0f, 1.0f};
    pbrParams.metallic  = 0.0f;
    pbrParams.roughness = 0.5f;
    auto pbrMaterial    = pbrKernel->createMaterial(*initialization, pbrParams);
    REQUIRE(pbrMaterial);

    // Quad covering center of screen: position, normal, texcoord
    const float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    };
    auto vb = Buffer::create("pbr-sky.vb", {.context = gpu, .size = sizeof(vertices)});
    REQUIRE(vb);
    initialization->recordUploadBuffer(vb, 0, {reinterpret_cast<const uint8_t *>(vertices), sizeof(vertices)});

    const uint16_t indices[] = {0, 1, 2, 2, 1, 3};
    auto           ib        = Buffer::create("pbr-sky.ib", {.context = gpu, .size = sizeof(indices)});
    REQUIRE(ib);
    initialization->recordUploadBuffer(ib, 0, {reinterpret_cast<const uint8_t *>(indices), sizeof(indices)});

    auto initWork = initialization->seal();
    REQUIRE(initWork);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 4096, .streamingCapacity = 1024});
    REQUIRE(ssc);

    SharedUniforms uniforms;
    uniforms.cameraPosition   = {0, 0, 2, 1};
    uniforms.numLights        = 0; // ZERO direct lights; illumination comes solely from sky IBL
    uniforms.exposure         = 1.0f;
    uniforms.renderTargetSize = {16, 16};
    uniforms.viewMatrix       = glm::lookAtRH(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    uniforms.projMatrix       = glm::perspectiveRH_ZO(glm::radians(60.0f), 1.0f, uniforms.nearPlane, uniforms.farPlane);
    uniforms.projMatrix[1][1] *= -1;
    uniforms.projViewMatrix = uniforms.projMatrix * uniforms.viewMatrix;

    auto uniformProducer = gpuBindless::CnC::create("pbr-sky.uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(uniformProducer);
    auto state = ssc->recordUniformUpdate(*uniformProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    REQUIRE(state);
    auto uniformWork = uniformProducer->seal();
    REQUIRE(uniformWork);

    const auto targetDesc   = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(16, 16).setLevels(1);
    auto       renderTarget = Texture::create("pbr-sky.target", {.context = gpu, .descriptor = targetDesc});
    REQUIRE(renderTarget);

    RasterTarget target;
    target.setColorTarget(0, GpuResourceView {renderTarget}).setClearColor(0, 0, 0, 1);
    target.states.cullMode = RasterState::CULL_NONE;

    auto raster = gpuBindless::Raster::create(
        "pbr-sky.raster", {.gpu = gpu, .target = &target, .heap = heap, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
    REQUIRE(raster);

    RasterGeometry geometry;
    geometry.vertices.push_back({.buffer = vb, .offset = 0, .stride = 8 * sizeof(float)});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::NORMAL_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 6 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});
    geometry.indices    = {.buffer = ib, .offset = 0, .stride = 2};
    geometry.indexCount = 6;

    PbrMaterial::DrawParameters drawParams {{*raster, state, geometry}, glm::mat4(1), skyMaterial};
    REQUIRE(pbrMaterial->record(drawParams));

    auto drawWork = raster->seal();
    REQUIRE(drawWork);

    gpu->submit(GpuContext::SubmitParameters("pbr-sky.submit").appendWork(initWork).appendWork(uniformWork).appendWork(drawWork));
    gpu->waitForIdle();

    auto image = renderTarget->readback();
    REQUIRE(!image.empty());
    const uint8_t * pixels            = reinterpret_cast<const uint8_t *>(image.data());
    const uint32_t  centerPixelOffset = (8 * 16 + 8) * 4;
    const uint8_t   r                 = pixels[centerPixelOffset + 0];
    const uint8_t   g                 = pixels[centerPixelOffset + 1];
    const uint8_t   b                 = pixels[centerPixelOffset + 2];

    // Surface was illuminated by sky-blue fallback cubemap (#87CEEB: R=135, G=206, B=235)
    CHECK(r > 0);
    CHECK(g > r);
    CHECK(b > r);
}

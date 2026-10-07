#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include <cstring>
#include <cmath>

using namespace GN;
using namespace GN::fx2::bindless;
using namespace GN::gpu2;
namespace gpuBindless = GN::gpu2::bindless;

TEST_CASE("FX2 bindless cel kernel validates parameters and manages material storage", "[fx2][bindless][cel][gpu]") {
    auto gpu = GpuContext::create("bindless-cel-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto heap = gpuBindless::DescriptorHeap::create("bindless-cel-test.heap", {.gpu = gpu, .capacity = 32});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-cel-test.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);

    auto cel = CelKernel::create(*heap, *initialization);
    REQUIRE(cel);

    auto params = cel->defaultMaterialParameters();
    CHECK(params.shadowThreshold == 0.5f);
    CHECK(params.outlineWidth == 0.003f);

    // Reject non-finite parameters
    auto invalidParams    = params;
    invalidParams.color.x = NAN;
    CHECK_FALSE(cel->createMaterial(*initialization, invalidParams));

    invalidParams                 = params;
    invalidParams.shadowThreshold = INFINITY;
    CHECK_FALSE(cel->createMaterial(*initialization, invalidParams));

    invalidParams              = params;
    invalidParams.outlineWidth = NAN;
    CHECK_FALSE(cel->createMaterial(*initialization, invalidParams));

    // Valid materials
    auto mat1 = cel->createMaterial(*initialization, params);
    REQUIRE(mat1);

    auto customParams         = params;
    customParams.color        = {0.2f, 0.7f, 0.9f, 1.0f};
    customParams.outlineWidth = 0.01f;
    auto mat2                 = cel->createMaterial(*initialization, customParams);
    REQUIRE(mat2);
}

TEST_CASE("FX2 bindless cel material renders cartoon steps and outline", "[fx2][bindless][cel][gpu]") {
    auto gpu = GpuContext::create("bindless-cel-render-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint32_t extent = 64;
    auto               heap   = gpuBindless::DescriptorHeap::create("bindless-cel-render-test.heap", {.gpu = gpu, .capacity = 64});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-cel-render-test.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);

    auto cel = CelKernel::create(*heap, *initialization);
    REQUIRE(cel);

    auto matParams         = cel->defaultMaterialParameters();
    matParams.color        = {1.0f, 0.5f, 0.2f, 1.0f};
    matParams.shadowTint   = {0.2f, 0.1f, 0.3f};
    matParams.outlineWidth = 0.05f;
    matParams.outlineColor = {0.0f, 0.0f, 0.0f, 1.0f};
    auto material          = cel->createMaterial(*initialization, matParams);
    REQUIRE(material);

    // CCW triangle in front facing orientation
    const float vertexData[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f,
    };
    auto vb = Buffer::create("bindless-cel-render-test.vb", {.context = gpu, .size = sizeof(vertexData)});
    REQUIRE(vb);
    initialization->recordUploadBuffer(vb, 0, {reinterpret_cast<const uint8_t *>(vertexData), sizeof(vertexData)});
    auto initWork = initialization->seal();
    REQUIRE(initWork);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 4096, .streamingCapacity = 256});
    REQUIRE(ssc);

    SharedUniforms uniforms {};
    uniforms.renderTargetSize        = {float(extent), float(extent)};
    uniforms.cameraPosition          = {0, 0, 2, 1};
    uniforms.viewMatrix              = glm::mat4(1.0f);
    uniforms.projMatrix              = glm::mat4(1.0f);
    uniforms.projViewMatrix          = glm::mat4(1.0f);
    uniforms.exposure                = 1.0f;
    uniforms.numLights               = 1;
    uniforms.lights[0].positionOrDir = {0, 0, 1, float(DirectLightUniform::DIRECTIONAL)};
    uniforms.lights[0].colorAndRange = {2, 2, 2, 0};

    auto uniformProducer = gpuBindless::CnC::create("bindless-cel-render-test.uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(uniformProducer);
    auto state = ssc->recordUniformUpdate(*uniformProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    REQUIRE(state);
    auto uniformWork = uniformProducer->seal();
    REQUIRE(uniformWork);

    gpu->submit(GpuContext::SubmitParameters("bindless-cel-render-test.init").appendWork(initWork).appendWork(uniformWork));
    gpu->waitForIdle();

    RasterGeometry geometry;
    geometry.vertices.push_back({.buffer = vb, .offset = 0, .stride = 8 * sizeof(float)});
    geometry.vertexCount = 3;
    geometry.format.attributes.push_back(
        {.location = CelMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = CelMaterial::NORMAL_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = CelMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 6 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});

    auto output = Texture::create(
        "bindless-cel-render-test.output",
        {.context    = gpu,
         .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(extent, extent).setLevels(1)});
    REQUIRE(output);

    RasterTarget target;
    target.setColorTarget(0, GpuResourceView {output}).setClearColor(0, 0, 0, 1);
    target.states.cullMode = RasterState::CULL_NONE;
    auto raster            = gpuBindless::Raster::create(
        "bindless-cel-render-test.raster",
        {.gpu = gpu, .target = &target, .heap = heap, .heapSetIndex = 0, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 2});
    REQUIRE(raster);

    CelMaterial::DrawParameters draw {{*raster, state, geometry}};
    draw.renderOutline = true;
    REQUIRE(material->record(draw));

    auto draws = raster->seal();
    REQUIRE(draws);
    gpu->submit(GpuContext::SubmitParameters("bindless-cel-render-test.draw").appendWork(draws));
    gpu->waitForIdle();

    auto image = output->readback();
    REQUIRE_FALSE(image.empty());

    const auto pixel = static_cast<const uint8_t *>(image.data()) + (extent / 2 * extent + extent / 2) * 4;
    CHECK(pixel[0] > 0);
    CHECK(pixel[3] == 255);
}

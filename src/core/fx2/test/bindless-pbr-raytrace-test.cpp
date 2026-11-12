#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <garnet/GNfx2.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <cmath>

using namespace GN;
using namespace GN::fx2;
using namespace GN::gpu2;

namespace gpuBindless = GN::gpu2;

namespace {

constexpr float PI = 3.14159265359f;

struct DirectBrdf {
    glm::vec3 diffuse;
    glm::vec3 specular;
};

DirectBrdf evaluateDirectBrdf(glm::vec3 albedo, float metallic, float roughness, float nDotV, float nDotL, float nDotH, float vDotH) {
    const glm::vec3 f0      = glm::mix(glm::vec3(0.04f), albedo, metallic);
    const glm::vec3 fresnel = f0 + (glm::vec3(1.0f) - f0) * std::pow(1.0f - vDotH, 5.0f);

    const float alpha        = roughness * roughness;
    const float alpha2       = alpha * alpha;
    const float denominator  = nDotH * nDotH * (alpha2 - 1.0f) + 1.0f;
    const float distribution = alpha2 / (PI * denominator * denominator);
    const float k            = (roughness + 1.0f) * (roughness + 1.0f) / 8.0f;
    const float geometry     = (nDotV / (nDotV * (1.0f - k) + k)) * (nDotL / (nDotL * (1.0f - k) + k));

    DirectBrdf result;
    result.diffuse  = (glm::vec3(1.0f) - fresnel) * (1.0f - metallic) * albedo / PI;
    result.specular = distribution * geometry * fresnel / (4.0f * nDotV * nDotL);
    return result;
}

glm::vec3 evaluateSplitSumSpecular(glm::vec3 prefilteredRadiance, glm::vec3 f0, glm::vec2 brdfScaleBias) {
    return prefilteredRadiance * (f0 * brdfScaleBias.x + glm::vec3(brdfScaleBias.y));
}

} // namespace

TEST_CASE("FX2 ray tracing direct PBR matches Cook-Torrance reference values", "[fx2][bindless][pbr][raytrace][math]") {
    SECTION("normal-incidence dielectric retains diffuse and dielectric specular") {
        const DirectBrdf brdf = evaluateDirectBrdf(glm::vec3(1.0f), 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);

        CHECK(brdf.diffuse.r == Catch::Approx(0.305577f).epsilon(1e-4));
        CHECK(brdf.specular.r == Catch::Approx(0.050930f).epsilon(1e-4));
        CHECK(brdf.diffuse.r + brdf.specular.r < 1.0f);
    }

    SECTION("metal has no diffuse lobe and preserves colored F0") {
        const DirectBrdf brdf = evaluateDirectBrdf(glm::vec3(1.0f, 0.25f, 0.1f), 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);

        CHECK(glm::length(brdf.diffuse) == Catch::Approx(0.0f));
        CHECK(brdf.specular.r == Catch::Approx(1.273240f).epsilon(1e-4));
        CHECK(brdf.specular.g == Catch::Approx(0.318310f).epsilon(1e-4));
        CHECK(brdf.specular.b == Catch::Approx(0.127324f).epsilon(1e-4));
    }

    SECTION("roughness lowers the aligned GGX highlight") {
        const DirectBrdf smooth = evaluateDirectBrdf(glm::vec3(1.0f), 0.0f, 0.25f, 1.0f, 1.0f, 1.0f, 1.0f);
        const DirectBrdf rough  = evaluateDirectBrdf(glm::vec3(1.0f), 0.0f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f);

        CHECK(smooth.specular.r > rough.specular.r * 10.0f);
    }
}

TEST_CASE("FX2 ray tracing split-sum IBL applies the integrated BRDF scale and bias", "[fx2][bindless][pbr][raytrace][math]") {
    const glm::vec3 environment(2.0f, 1.0f, 0.5f);

    SECTION("dielectric uses F0 times scale plus bias") {
        const glm::vec3 result = evaluateSplitSumSpecular(environment, glm::vec3(0.04f), glm::vec2(0.8f, 0.1f));
        CHECK(result[0] == Catch::Approx(0.264f));
        CHECK(result[1] == Catch::Approx(0.132f));
        CHECK(result[2] == Catch::Approx(0.066f));
    }

    SECTION("metallic F0 follows base color") {
        const glm::vec3 result = evaluateSplitSumSpecular(environment, glm::vec3(0.9f, 0.2f, 0.05f), glm::vec2(1.0f, 0.0f));
        CHECK(result[0] == Catch::Approx(1.8f));
        CHECK(result[1] == Catch::Approx(0.2f));
        CHECK(result[2] == Catch::Approx(0.025f));
    }
}

TEST_CASE("FX2 ray tracing applies the IBL specular response once", "[fx2][bindless][pbr][raytrace][gpu]") {
    auto gpu = GpuContext::create("bindless-pbr-raytrace-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu || !gpu->caps().rayQueries) SKIP("Ray-query capable Vulkan GPU is unavailable");

    auto heap = gpuBindless::DescriptorHeap::create("bindless-pbr-raytrace-test.heap", {.gpu = gpu, .capacity = 32, .materialCapacity = 1024});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-pbr-raytrace-test.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);
    auto skyKernel      = SkyKernel::create(*heap, *initialization);
    auto rayTraceKernel = RayTraceKernel::create(*heap, *initialization);
    REQUIRE(skyKernel);
    REQUIRE(rayTraceKernel);

    auto materialParameters      = PbrMaterial::Parameters {};
    materialParameters.baseColor = {1.0f, 1.0f, 1.0f, 1.0f};
    materialParameters.metallic  = 1.0f;
    materialParameters.roughness = 0.5f;
    auto material                = PbrMaterial::create(*heap, *initialization, materialParameters);
    REQUIRE(material);
    auto skyMaterial = SkyMaterial::create(*heap, *initialization, SkyMaterial::Parameters {});
    REQUIRE(skyMaterial);

    const float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
    };
    auto vertexBuffer =
        Buffer::create("bindless-pbr-raytrace-test.vertices", {.context = gpu, .size = sizeof(vertices), .accelerationStructureBuildInput = true});
    REQUIRE(vertexBuffer);
    initialization->recordUploadBuffer(vertexBuffer, 0, {reinterpret_cast<const uint8_t *>(vertices), sizeof(vertices)});

    RasterGeometry geometry;
    geometry.vertices.push_back({.buffer = vertexBuffer, .offset = 0, .stride = 8 * sizeof(float)});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::NORMAL_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 6 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});
    geometry.vertexCount = 3;

    const std::array<PbrRayTracingScene::Mesh, 1> meshes {{{&geometry, material.get(), glm::mat4(1.0f)}}};
    auto                                          scene = PbrRayTracingScene::create(*heap, *initialization, {meshes.data(), meshes.size()});
    REQUIRE(scene);

    auto constants = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 4096, .streamingCapacity = 256});
    REQUIRE(constants);
    SharedUniforms uniforms {};
    uniforms.cameraPosition   = {0.0f, 0.0f, 2.0f, 1.0f};
    uniforms.exposure         = 1.0f;
    uniforms.renderTargetSize = {16.0f, 16.0f};
    uniforms.numLights        = 0;
    uniforms.viewMatrix       = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    uniforms.projMatrix       = glm::perspectiveRH_ZO(glm::radians(60.0f), 1.0f, uniforms.nearPlane, uniforms.farPlane);
    uniforms.projMatrix[1][1] *= -1.0f;
    uniforms.projViewMatrix = uniforms.projMatrix * uniforms.viewMatrix;
    auto uniformProducer    = gpuBindless::CnC::create("bindless-pbr-raytrace-test.uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(uniformProducer);
    auto state = constants->recordUniformUpdate(*uniformProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    REQUIRE(state);

    const auto targetDescriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(16, 16).setLevels(1);
    auto       targetTexture    = Texture::create("bindless-pbr-raytrace-test.target", {.context = gpu, .descriptor = targetDescriptor});
    REQUIRE(targetTexture);
    RasterTarget target;
    target.setColorTarget(0, GpuResourceView {targetTexture}).setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    target.states.cullMode = RasterState::CULL_NONE;

    auto initializationWork = initialization->seal();
    auto uniformWork        = uniformProducer->seal();
    REQUIRE(initializationWork);
    REQUIRE(uniformWork);
    GpuResourceTable resources = sharedUniformResources(state);
    resources.resize(3);
    resources[2].resize(2);
    GpuResourceView accelerationStructure;
    accelerationStructure.setAccelerationStructure(scene->accelerationStructure());
    resources[2][0].append(std::move(accelerationStructure));
    resources[2][1].append(scene->geometryTable());

    auto raster =
        gpuBindless::Raster::create("bindless-pbr-raytrace-test.raster",
                                    {.gpu = gpu, .target = &target, .heap = heap, .heapSetIndex = 0, .passResources = resources, .numberOfDrawsHint = 1});
    REQUIRE(raster);
    CommonDrawParameters           commonDraw {*raster, state, geometry};
    RayTraceKernel::DrawParameters draw {std::move(commonDraw), scene->accelerationStructure(), glm::mat4(1.0f), skyMaterial, material.get()};
    REQUIRE(rayTraceKernel->record(draw));
    auto drawWork = raster->seal();
    REQUIRE(drawWork);
    gpu->submit(GpuContext::SubmitParameters("bindless-pbr-raytrace-test.submit").appendWork(initializationWork).appendWork(uniformWork).appendWork(drawWork));
    gpu->waitForIdle();

    auto image = targetTexture->readback();
    REQUIRE_FALSE(image.empty());
    const auto * pixel = reinterpret_cast<const uint8_t *>(image.data()) + (8 * 16 + 8) * 4;
    // The fallback sky is (135,206,235)/255 and the fallback BRDF LUT has
    // scale=bias=1. With metallic white F0, split-sum radiance is twice the
    // environment. The output tone mapper should therefore yield R ~= 131;
    // a second raw sky contribution would push it above 150.
    CHECK(pixel[0] > 120);
    CHECK(pixel[0] < 145);
    CHECK(pixel[1] > 145);
    CHECK(pixel[2] > pixel[1]);

    SharedUniforms directUniforms          = uniforms;
    directUniforms.numLights               = 1;
    directUniforms.lights[0].positionOrDir = {0.0f, 0.0f, -1.0f, float(DirectLightUniform::DIRECTIONAL)};
    directUniforms.lights[0].colorAndRange = {1.0f, 1.0f, 1.0f, 0.0f};
    auto directUniformProducer             = gpuBindless::CnC::create("bindless-pbr-raytrace-test.direct-uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(directUniformProducer);
    auto directState = constants->recordUniformUpdate(*directUniformProducer, {reinterpret_cast<const uint8_t *>(&directUniforms), sizeof(directUniforms)});
    REQUIRE(directState);
    auto directUniformWork = directUniformProducer->seal();
    REQUIRE(directUniformWork);

    const auto directTargetDescriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(16, 16).setLevels(1);
    auto       directTargetTexture    = Texture::create("bindless-pbr-raytrace-test.direct-target", {.context = gpu, .descriptor = directTargetDescriptor});
    REQUIRE(directTargetTexture);
    RasterTarget directTarget;
    directTarget.setColorTarget(0, GpuResourceView {directTargetTexture}).setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    directTarget.states.cullMode = RasterState::CULL_NONE;
    auto directResources         = sharedUniformResources(directState);
    directResources.resize(3);
    directResources[2].resize(2);
    GpuResourceView directAccelerationStructure;
    directAccelerationStructure.setAccelerationStructure(scene->accelerationStructure());
    directResources[2][0].append(std::move(directAccelerationStructure));
    directResources[2][1].append(scene->geometryTable());
    auto directRaster = gpuBindless::Raster::create(
        "bindless-pbr-raytrace-test.direct-raster",
        {.gpu = gpu, .target = &directTarget, .heap = heap, .heapSetIndex = 0, .passResources = directResources, .numberOfDrawsHint = 1});
    REQUIRE(directRaster);
    RasterState directDrawState;
    directDrawState.cullMode  = RasterState::CULL_NONE;
    directDrawState.frontFace = RasterState::FRONT_CCW;
    RayTraceKernel::DrawParameters directDraw {{*directRaster, directState, geometry, &directDrawState},
                                               scene->accelerationStructure(),
                                               glm::mat4(1.0f),
                                               {},
                                               material.get()};
    REQUIRE(rayTraceKernel->record(directDraw));
    auto directDrawWork = directRaster->seal();
    REQUIRE(directDrawWork);
    gpu->submit(GpuContext::SubmitParameters("bindless-pbr-raytrace-test.direct-submit").appendWork(directUniformWork).appendWork(directDrawWork));
    gpu->waitForIdle();
    auto directImage = directTargetTexture->readback();
    REQUIRE_FALSE(directImage.empty());
    const auto * directPixel = reinterpret_cast<const uint8_t *>(directImage.data()) + (8 * 16 + 8) * 4;
    // At normal incidence, roughness 0.5 and F0=1 give D/4 = 1.27324.
    // With unit directional irradiance the shader maps this to about 143/255.
    CHECK(directPixel[0] > 138);
    CHECK(directPixel[0] < 149);
    CHECK(directPixel[1] == directPixel[0]);
    CHECK(directPixel[2] == directPixel[0]);
}

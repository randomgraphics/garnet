#include <catch2/catch_test_macros.hpp>

#include <garnet/GNfx2.h>

#include <cstring>

using namespace GN;
using namespace GN::fx2::bindless;
using namespace GN::gpu2;
namespace gpuBindless = GN::gpu2::bindless;

TEST_CASE("FX2 bindless SSC stores independent uniform versions and reuses streaming ranges", "[fx2][bindless][ssc][gpu]") {
    auto gpu = GpuContext::create("bindless-ssc-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 4096, .streamingCapacity = 256});
    REQUIRE(ssc);

    auto heap = gpuBindless::DescriptorHeap::create("bindless-ssc-test.heap", {.gpu = gpu, .capacity = 1});
    REQUIRE(heap);
    auto producer = gpuBindless::CnC::create("bindless-ssc-test.upload", {.gpu = gpu, .heap = heap});
    REQUIRE(producer);

    const uint32_t a      = 0x12345678;
    const uint32_t b      = 0x87654321;
    auto           stateA = ssc->recordUniformUpdate(*producer, {reinterpret_cast<const uint8_t *>(&a), sizeof(a)});
    auto           stateB = ssc->recordUniformUpdate(*producer, {reinterpret_cast<const uint8_t *>(&b), sizeof(b)});
    REQUIRE(stateA);
    REQUIRE(stateB);
    CHECK(stateA->view().bufferView.offset != stateB->view().bufferView.offset);
    auto upload = producer->seal();
    REQUIRE(upload);
    gpu->submit(GpuContext::SubmitParameters("bindless-ssc-test.upload").appendWork(upload));
    gpu->waitForIdle();

    const auto readValue = [](const AutoRef<SharedShaderConstants::UniformState> & state) {
        const auto view  = state->view();
        auto       data  = view.buffer()->readContent(static_cast<size_t>(view.bufferView.offset), sizeof(uint32_t));
        uint32_t   value = 0;
        if (data.size() == sizeof(value)) std::memcpy(&value, data.data(), sizeof(value));
        return value;
    };
    CHECK(readValue(stateA) == a);
    CHECK(readValue(stateB) == b);

    auto first = ssc->allocateStreaming(200);
    REQUIRE(first.id != SharedShaderConstants::INVALID_STREAMING_ID);
    REQUIRE(first.bytes.size() == 200);
    std::memset(first.bytes.data(), 0x5a, first.bytes.size());
    CHECK(ssc->allocateStreaming(64).id == SharedShaderConstants::INVALID_STREAMING_ID);
    ssc->releaseStreaming(first.id);
    auto reused = ssc->allocateStreaming(200);
    REQUIRE(reused.id != SharedShaderConstants::INVALID_STREAMING_ID);
    CHECK(reused.view.bufferView.offset == first.view.bufferView.offset);
    CHECK(reused.bytes[0] == 0x5a);
    auto otherSsc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 1024, .streamingCapacity = 256});
    REQUIRE(otherSsc);
    auto foreign = otherSsc->allocateStreaming(200);
    REQUIRE(foreign.id != SharedShaderConstants::INVALID_STREAMING_ID);
    otherSsc->releaseStreaming(reused.id);
    CHECK(otherSsc->allocateStreaming(64).id == SharedShaderConstants::INVALID_STREAMING_ID);
    otherSsc->releaseStreaming(foreign.id);
    ssc->releaseStreaming(reused.id);
    ssc->releaseStreaming(reused.id); // Stale releases are harmless.
}

TEST_CASE("FX2 bindless unlit and Lambertian materials render expected pixels", "[fx2][bindless][material][gpu]") {
    auto gpu = GpuContext::create("bindless-material-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint32_t extent = 64;
    auto               heap   = gpuBindless::DescriptorHeap::create("bindless-material-test.heap", {.gpu = gpu, .capacity = 64});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-material-test.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);
    auto unlit   = UnlitKernel::create(*heap, *initialization);
    auto lambert = LambertianKernel::create(*heap, *initialization);
    REQUIRE(unlit);
    REQUIRE(lambert);

    auto unlitParameters    = unlit->defaultMaterialParameters();
    unlitParameters.color   = {0.8f, 0.2f, 0.1f, 1};
    auto unlitMaterial      = unlit->createMaterial(*initialization, unlitParameters);
    auto lambertParameters  = lambert->defaultMaterialParameters();
    lambertParameters.color = {0.8f, 0.2f, 0.1f, 1};
    auto lambertMaterial    = lambert->createMaterial(*initialization, lambertParameters);
    REQUIRE(unlitMaterial);
    REQUIRE(lambertMaterial);

    // A full-screen triangle with a +Z normal isolates the lighting and material response.
    const float vertexData[] = {-1, -1, 0, 0, 0, 1, 3, -1, 0, 0, 0, 1, -1, 3, 0, 0, 0, 1};
    auto        vertices     = Buffer::create("bindless-material-test.vertices", {.context = gpu, .size = sizeof(vertexData)});
    REQUIRE(vertices);
    initialization->recordUploadBuffer(vertices, 0, {reinterpret_cast<const uint8_t *>(vertexData), sizeof(vertexData)});
    auto initWork = initialization->seal();
    REQUIRE(initWork);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 4096, .streamingCapacity = 256});
    REQUIRE(ssc);
    SharedUniforms uniforms;
    uniforms.renderTargetSize = {float(extent), float(extent)};
    uniforms.cameraPosition   = {0, 0, 2, 1};
    uniforms.exposure         = 1;
    uniforms.numLights        = 1;
    // The test triangle uses Vulkan's default positive-height viewport winding, so its
    // fragment normal is flipped by the shader's back-face handling.
    uniforms.lights[0].positionOrDir = {0, 0, 1, float(DirectLightUniform::DIRECTIONAL)};
    uniforms.lights[0].colorAndRange = {4, 4, 4, 0};
    auto uniformProducer             = gpuBindless::CnC::create("bindless-material-test.uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(uniformProducer);
    auto state = ssc->recordUniformUpdate(*uniformProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    REQUIRE(state);
    auto uniformWork = uniformProducer->seal();
    REQUIRE(uniformWork);
    gpu->submit(GpuContext::SubmitParameters("bindless-material-test.uploads").appendWork(initWork).appendWork(uniformWork));
    gpu->waitForIdle();

    RasterGeometry geometry;
    geometry.vertices.push_back({.buffer = vertices, .offset = 0, .stride = 6 * sizeof(float)});
    geometry.vertexCount = 3;
    geometry.format.attributes.push_back(
        {.location = LambertianMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = LambertianMaterial::NORMAL_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_3});

    const auto renderAndRead = [&](bool useLambertian) {
        auto output = Texture::create(
            "bindless-material-test.output",
            {.context    = gpu,
             .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(extent, extent).setLevels(1)});
        REQUIRE(output);
        RasterTarget target;
        target.setColorTarget(0, GpuResourceView(output)).setClearColor(0, 0, 0, 1);
        target.states.cullMode = RasterState::CULL_NONE;
        auto raster            = gpuBindless::Raster::create(
            "bindless-material-test.raster",
            {.gpu = gpu, .target = &target, .heap = heap, .heapSetIndex = 0, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
        REQUIRE(raster);
        if (useLambertian) {
            LambertianMaterial::DrawParameters draw {{*raster, state, geometry}};
            REQUIRE(lambertMaterial->record(draw));
        } else {
            UnlitMaterial::DrawParameters draw {{*raster, state, geometry}};
            REQUIRE(unlitMaterial->record(draw));
        }
        auto draws = raster->seal();
        REQUIRE(draws);
        gpu->submit(GpuContext::SubmitParameters("bindless-material-test.draw").appendWork(draws));
        gpu->waitForIdle();
        return output->readback();
    };

    auto unlitImage = renderAndRead(false);
    auto litImage   = renderAndRead(true);
    REQUIRE_FALSE(unlitImage.empty());
    REQUIRE_FALSE(litImage.empty());
    const auto unlitPixel = static_cast<const uint8_t *>(unlitImage.data()) + (extent / 2 * extent + extent / 2) * 4;
    const auto litPixel   = static_cast<const uint8_t *>(litImage.data()) + (extent / 2 * extent + extent / 2) * 4;
    CHECK(unlitPixel[0] > 180);
    CHECK(unlitPixel[1] > 35);
    CHECK(unlitPixel[1] < 70);
    CHECK(litPixel[0] > 40);
    CHECK(litPixel[1] > 8);
    CHECK(litPixel[0] < unlitPixel[0]);
}

TEST_CASE("FX2 bindless PBR materials render expected pixels", "[fx2][bindless][material][pbr][gpu]") {
    auto gpu = GpuContext::create("bindless-pbr-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint32_t extent = 64;
    auto               heap   = gpuBindless::DescriptorHeap::create("bindless-pbr-test.heap", {.gpu = gpu, .capacity = 64});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-pbr-test.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);
    auto pbr = PbrKernel::create(*heap, *initialization);
    REQUIRE(pbr);

    auto dielectricParams      = pbr->defaultMaterialParameters();
    dielectricParams.baseColor = {0.8f, 0.2f, 0.1f, 1};
    dielectricParams.metallic  = 0.0f;
    dielectricParams.roughness = 0.8f;
    auto dielectricMaterial    = pbr->createMaterial(*initialization, dielectricParams);
    REQUIRE(dielectricMaterial);

    auto metallicParams      = pbr->defaultMaterialParameters();
    metallicParams.baseColor = {0.8f, 0.2f, 0.1f, 1};
    metallicParams.metallic  = 1.0f;
    metallicParams.roughness = 0.2f;
    auto metallicMaterial    = pbr->createMaterial(*initialization, metallicParams);
    REQUIRE(metallicMaterial);

    // CCW triangle: (-1, -1), (-1, 3), (3, -1) so gl_FrontFacing is true in Vulkan clip space.
    const float vertexData[] = {
        -1, -1, 0, 0, 0, 1, 0, 0, -1, 3, 0, 0, 0, 1, 0, 2, 3, -1, 0, 0, 0, 1, 2, 0,
    };
    auto vertices = Buffer::create("bindless-pbr-test.vertices", {.context = gpu, .size = sizeof(vertexData)});
    REQUIRE(vertices);
    initialization->recordUploadBuffer(vertices, 0, {reinterpret_cast<const uint8_t *>(vertexData), sizeof(vertexData)});
    auto initWork = initialization->seal();
    REQUIRE(initWork);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 4096, .streamingCapacity = 256});
    REQUIRE(ssc);
    SharedUniforms uniforms;
    uniforms.renderTargetSize        = {float(extent), float(extent)};
    uniforms.cameraPosition          = {0, 0, 2, 1};
    uniforms.exposure                = 1;
    uniforms.numLights               = 1;
    uniforms.lights[0].positionOrDir = {0, 0, -1, float(DirectLightUniform::DIRECTIONAL)};
    uniforms.lights[0].colorAndRange = {4, 4, 4, 0};
    auto uniformProducer             = gpuBindless::CnC::create("bindless-pbr-test.uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(uniformProducer);
    auto state = ssc->recordUniformUpdate(*uniformProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    REQUIRE(state);
    auto uniformWork = uniformProducer->seal();
    REQUIRE(uniformWork);
    gpu->submit(GpuContext::SubmitParameters("bindless-pbr-test.uploads").appendWork(initWork).appendWork(uniformWork));
    gpu->waitForIdle();

    RasterGeometry geometry;
    geometry.vertices.push_back({.buffer = vertices, .offset = 0, .stride = 8 * sizeof(float)});
    geometry.vertexCount = 3;
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::NORMAL_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 6 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});

    const auto renderAndRead = [&](const AutoRef<PbrMaterial> & material) {
        auto output = Texture::create(
            "bindless-pbr-test.output",
            {.context    = gpu,
             .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(extent, extent).setLevels(1)});
        REQUIRE(output);
        RasterTarget target;
        target.setColorTarget(0, GpuResourceView(output)).setClearColor(0, 0, 0, 1);
        target.states.cullMode = RasterState::CULL_NONE;
        auto raster            = gpuBindless::Raster::create(
            "bindless-pbr-test.raster",
            {.gpu = gpu, .target = &target, .heap = heap, .heapSetIndex = 0, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
        REQUIRE(raster);
        PbrMaterial::DrawParameters draw {{*raster, state, geometry}};
        REQUIRE(material->record(draw));
        auto draws = raster->seal();
        REQUIRE(draws);
        gpu->submit(GpuContext::SubmitParameters("bindless-pbr-test.draw").appendWork(draws));
        gpu->waitForIdle();
        return output->readback();
    };

    auto dielectricImage = renderAndRead(dielectricMaterial);
    auto metallicImage   = renderAndRead(metallicMaterial);
    REQUIRE_FALSE(dielectricImage.empty());
    REQUIRE_FALSE(metallicImage.empty());
    const auto dielectricPixel = static_cast<const uint8_t *>(dielectricImage.data()) + (extent / 2 * extent + extent / 2) * 4;
    const auto metallicPixel   = static_cast<const uint8_t *>(metallicImage.data()) + (extent / 2 * extent + extent / 2) * 4;
    CHECK(dielectricPixel[0] > 0);
    CHECK(metallicPixel[0] > 0);
    CHECK(dielectricPixel[3] == 255);
    CHECK(metallicPixel[3] == 255);
}

TEST_CASE("FX2 bindless material keeps heap allocations until completion or discard", "[fx2][bindless][material][lifetime][gpu]") {
    auto gpu = GpuContext::create("bindless-material-lifetime", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto heap = gpuBindless::DescriptorHeap::create("bindless-material-lifetime.heap", {.gpu = gpu, .capacity = 8, .materialCapacity = 64});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-material-lifetime.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);
    auto kernel = UnlitKernel::create(*heap, *initialization);
    REQUIRE(kernel);
    const uint32_t kernelDescriptorCount = heap->size();

    auto sampled = Texture::create(
        "bindless-material-lifetime.sampled",
        {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1)});
    auto sampler = Sampler::create("bindless-material-lifetime.sampler", {.context = gpu});
    REQUIRE(sampled);
    REQUIRE(sampler);
    const uint8_t            texel[] = {255, 255, 255, 255};
    gpuBindless::CnC::Region region;
    region.imageExtent = {1, 1, 1};
    initialization->recordUploadImage(sampled, {texel, sizeof(texel)}, {&region, 1});

    auto parameters     = kernel->defaultMaterialParameters();
    parameters.colorMap = GpuResourceView(sampled).setImageViewType(GpuResourceView::ImageView::SAMPLED);
    parameters.sampler  = sampler;
    auto material       = kernel->createMaterial(*initialization, parameters);
    REQUIRE(material);
    CHECK(heap->size() == kernelDescriptorCount + 2);
    auto initWork = initialization->seal();
    REQUIRE(initWork);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 1024, .streamingCapacity = 256});
    REQUIRE(ssc);
    SharedUniforms uniforms;
    auto           uniformProducer = gpuBindless::CnC::create("bindless-material-lifetime.uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(uniformProducer);
    auto state = ssc->recordUniformUpdate(*uniformProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    REQUIRE(state);
    auto uniformWork = uniformProducer->seal();
    REQUIRE(uniformWork);

    const float vertexData[] = {-1, -1, 0, 0, 0, 3, -1, 0, 1, 0, -1, 3, 0, 0, 1};
    auto        vertices     = Buffer::create("bindless-material-lifetime.vertices", {.context = gpu, .size = sizeof(vertexData)});
    REQUIRE(vertices);
    // The init payload is already sealed; upload geometry through a separate producer.
    auto geometryProducer = gpuBindless::CnC::create("bindless-material-lifetime.geometry", {.gpu = gpu, .heap = heap});
    REQUIRE(geometryProducer);
    geometryProducer->recordUploadBuffer(vertices, 0, {reinterpret_cast<const uint8_t *>(vertexData), sizeof(vertexData)});
    auto geometryWork = geometryProducer->seal();
    REQUIRE(geometryWork);

    auto output = Texture::create(
        "bindless-material-lifetime.output",
        {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(16, 16).setLevels(1)});
    REQUIRE(output);
    RasterTarget target;
    target.setColorTarget(0, GpuResourceView(output)).setClearColor(0, 0, 0, 1);
    target.states.cullMode = RasterState::CULL_NONE;
    auto raster =
        gpuBindless::Raster::create("bindless-material-lifetime.raster",
                                    {.gpu = gpu, .target = &target, .heap = heap, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
    REQUIRE(raster);
    RasterGeometry geometry;
    geometry.vertices.push_back({.buffer = vertices, .offset = 0, .stride = 5 * sizeof(float)});
    geometry.format.attributes.push_back(
        {.location = UnlitMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = UnlitMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});
    geometry.vertexCount = 3;
    UnlitMaterial::DrawParameters draw {{*raster, state, geometry}};
    REQUIRE(material->record(draw));
    auto drawWork = raster->seal();
    REQUIRE(drawWork);

    // An unsubmitted contender cannot reuse the sole material chunk while payloads retain the material.
    auto contenderProducer = gpuBindless::CnC::create("bindless-material-lifetime.contender", {.gpu = gpu, .heap = heap});
    REQUIRE(contenderProducer);
    CHECK_FALSE(kernel->createMaterial(*contenderProducer, parameters));
    CHECK(heap->size() == kernelDescriptorCount + 2);
    material.clear();

    GpuContext::SubmitParameters submit("bindless-material-lifetime.submit");
    submit.appendWork(initWork).appendWork(uniformWork).appendWork(geometryWork).appendWork(drawWork);
    gpu->submit(submit);
    initWork.clear();
    uniformWork.clear();
    geometryWork.clear();
    drawWork.clear();
    gpu->waitForIdle();
    CHECK(heap->size() == kernelDescriptorCount);

    // Completion released the chunk and custom descriptor slots, so a replacement can allocate them.
    auto replacementProducer = gpuBindless::CnC::create("bindless-material-lifetime.replacement", {.gpu = gpu, .heap = heap});
    REQUIRE(replacementProducer);
    auto replacement = kernel->createMaterial(*replacementProducer, parameters);
    REQUIRE(replacement);
    CHECK(heap->size() == kernelDescriptorCount + 2);
    auto replacementRaster =
        gpuBindless::Raster::create("bindless-material-lifetime.discard-raster",
                                    {.gpu = gpu, .target = &target, .heap = heap, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
    REQUIRE(replacementRaster);
    UnlitMaterial::DrawParameters replacementDraw {{*replacementRaster, state, geometry}};
    REQUIRE(replacement->record(replacementDraw));
    auto discardDrawWork = replacementRaster->seal();
    REQUIRE(discardDrawWork);
    replacement.clear();
    auto replacementWork = replacementProducer->seal();
    REQUIRE(replacementWork);
    replacementWork.clear();
    CHECK(heap->size() == kernelDescriptorCount + 2);
    discardDrawWork.clear();
    CHECK(heap->size() == kernelDescriptorCount);
}

TEST_CASE("FX2 bindless PBR material keeps heap allocations until completion or discard", "[fx2][bindless][material][pbr][lifetime][gpu]") {
    auto gpu = GpuContext::create("bindless-pbr-lifetime", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto heap = gpuBindless::DescriptorHeap::create("bindless-pbr-lifetime.heap", {.gpu = gpu, .capacity = 8, .materialCapacity = 80});
    REQUIRE(heap);
    auto initialization = gpuBindless::CnC::create("bindless-pbr-lifetime.init", {.gpu = gpu, .heap = heap});
    REQUIRE(initialization);
    auto kernel = PbrKernel::create(*heap, *initialization);
    REQUIRE(kernel);
    const uint32_t kernelDescriptorCount = heap->size();

    auto sampled = Texture::create(
        "bindless-pbr-lifetime.sampled",
        {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1)});
    auto sampler = Sampler::create("bindless-pbr-lifetime.sampler", {.context = gpu});
    REQUIRE(sampled);
    REQUIRE(sampler);
    const uint8_t            texel[] = {255, 255, 255, 255};
    gpuBindless::CnC::Region region;
    region.imageExtent = {1, 1, 1};
    initialization->recordUploadImage(sampled, {texel, sizeof(texel)}, {&region, 1});

    auto parameters         = kernel->defaultMaterialParameters();
    parameters.baseColorMap = GpuResourceView(sampled).setImageViewType(GpuResourceView::ImageView::SAMPLED);
    parameters.sampler      = sampler;
    auto material           = kernel->createMaterial(*initialization, parameters);
    REQUIRE(material);
    CHECK(heap->size() == kernelDescriptorCount + 2);
    auto initWork = initialization->seal();
    REQUIRE(initWork);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 1024, .streamingCapacity = 256});
    REQUIRE(ssc);
    SharedUniforms uniforms;
    auto           uniformProducer = gpuBindless::CnC::create("bindless-pbr-lifetime.uniform", {.gpu = gpu, .heap = heap});
    REQUIRE(uniformProducer);
    auto state = ssc->recordUniformUpdate(*uniformProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    REQUIRE(state);
    auto uniformWork = uniformProducer->seal();
    REQUIRE(uniformWork);

    const float vertexData[] = {
        -1, -1, 0, 0, 0, 1, 0, 0, -1, 3, 0, 0, 0, 1, 0, 2, 3, -1, 0, 0, 0, 1, 2, 0,
    };
    auto vertices = Buffer::create("bindless-pbr-lifetime.vertices", {.context = gpu, .size = sizeof(vertexData)});
    REQUIRE(vertices);
    auto geometryProducer = gpuBindless::CnC::create("bindless-pbr-lifetime.geometry", {.gpu = gpu, .heap = heap});
    REQUIRE(geometryProducer);
    geometryProducer->recordUploadBuffer(vertices, 0, {reinterpret_cast<const uint8_t *>(vertexData), sizeof(vertexData)});
    auto geometryWork = geometryProducer->seal();
    REQUIRE(geometryWork);

    auto output = Texture::create(
        "bindless-pbr-lifetime.output",
        {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(16, 16).setLevels(1)});
    REQUIRE(output);
    RasterTarget target;
    target.setColorTarget(0, GpuResourceView(output)).setClearColor(0, 0, 0, 1);
    target.states.cullMode = RasterState::CULL_NONE;
    auto raster            = gpuBindless::Raster::create(
        "bindless-pbr-lifetime.raster", {.gpu = gpu, .target = &target, .heap = heap, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
    REQUIRE(raster);
    RasterGeometry geometry;
    geometry.vertices.push_back({.buffer = vertices, .offset = 0, .stride = 8 * sizeof(float)});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::NORMAL_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = PbrMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 6 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});
    geometry.vertexCount = 3;
    PbrMaterial::DrawParameters draw {{*raster, state, geometry}};
    REQUIRE(material->record(draw));
    auto drawWork = raster->seal();
    REQUIRE(drawWork);

    // An unsubmitted contender cannot reuse the sole material chunk while payloads retain the material.
    auto contenderProducer = gpuBindless::CnC::create("bindless-pbr-lifetime.contender", {.gpu = gpu, .heap = heap});
    REQUIRE(contenderProducer);
    CHECK_FALSE(kernel->createMaterial(*contenderProducer, parameters));
    CHECK(heap->size() == kernelDescriptorCount + 2);
    material.clear();

    GpuContext::SubmitParameters submit("bindless-pbr-lifetime.submit");
    submit.appendWork(initWork).appendWork(uniformWork).appendWork(geometryWork).appendWork(drawWork);
    gpu->submit(submit);
    initWork.clear();
    uniformWork.clear();
    geometryWork.clear();
    drawWork.clear();
    gpu->waitForIdle();
    CHECK(heap->size() == kernelDescriptorCount);

    // Completion released the chunk and custom descriptor slots, so a replacement can allocate them.
    auto replacementProducer = gpuBindless::CnC::create("bindless-pbr-lifetime.replacement", {.gpu = gpu, .heap = heap});
    REQUIRE(replacementProducer);
    auto replacement = kernel->createMaterial(*replacementProducer, parameters);
    REQUIRE(replacement);
    CHECK(heap->size() == kernelDescriptorCount + 2);
    auto replacementRaster =
        gpuBindless::Raster::create("bindless-pbr-lifetime.discard-raster",
                                    {.gpu = gpu, .target = &target, .heap = heap, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 1});
    REQUIRE(replacementRaster);
    PbrMaterial::DrawParameters replacementDraw {{*replacementRaster, state, geometry}};
    REQUIRE(replacement->record(replacementDraw));
    auto discardDrawWork = replacementRaster->seal();
    REQUIRE(discardDrawWork);
    replacement.clear();
    auto replacementWork = replacementProducer->seal();
    REQUIRE(replacementWork);
    replacementWork.clear();
    CHECK(heap->size() == kernelDescriptorCount + 2);
    discardDrawWork.clear();
    CHECK(heap->size() == kernelDescriptorCount);
}

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace GN;
using namespace GN::fx2;
using namespace GN::gpu2;

namespace {
// An external effect only needs public Kernel and gpu2 contracts; no central registry.
struct CustomKernel final : Kernel {
    GN_REGISTER_RUNTIME_TYPE(Kernel);
    CustomKernel(): Kernel(TYPE_INFO(), "custom-kernel") {}
    Execution execution() const override { return Execution::COMPUTE; }
};

struct RecordingRaster final : GpuRaster {
    GN_REGISTER_RUNTIME_TYPE(GpuRaster);
    RasterTarget              destination;
    DynaArray<DrawParameters> draws;
    RecordingRaster(): GpuRaster(TYPE_INFO(), "recording-raster") {}
    void                 draw(const DrawParameters & draw) override { draws.append(draw); }
    const RasterTarget & target() const override { return destination; }
    AutoRef<GpuPayload>  seal() override { return {}; }
};

struct Fixture {
    AutoRef<GpuContext>            gpu = GpuContext::create("kernel-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    AutoRef<SharedShaderConstants> ssc;
    AutoRef<UnlitKernel>           unlit;
    AutoRef<Buffer>                vertices;
    UnlitKernel::Inputs            inputs;
    bool                           initialize() {
        ssc   = SharedShaderConstants::create({.gpu = gpu});
        unlit = UnlitKernel::create(gpu);
        if (!ssc || !unlit) return false;
        ssc->set0.camera.cameraPosition   = {0, 0, 1};
        ssc->set0.camera.cameraFov        = ArcDegree(90);
        ssc->set0.camera.aspectRatio      = 1;
        ssc->set0.camera.viewWidthInPixel = ssc->set0.camera.viewHeightInPixel = 64;
        const float positions[]                                                = {-1, -1, 0, 0, -1, 0, -0.5f, 1, 0};
        vertices = Buffer::create("kernel.positions", {.context = gpu, .size = sizeof(positions)});
        if (!vertices || !vertices->setContent({reinterpret_cast<const uint8_t *>(positions), sizeof(positions)})) return false;
        inputs.geometry.vertices.append({.buffer = vertices, .offset = 0, .stride = 12});
        inputs.geometry.vertexCount = 3;
        inputs.geometry.format.attributes.append({.location = 0, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
        return true;
    }
    ~Fixture() {
        if (gpu) gpu->waitForIdle();
    }
};
} // namespace

TEST_CASE("fx2 kernels support external typed effects without E2", "[fx2][kernel]") {
    AutoRef<Kernel> kernel(new CustomKernel);
    CHECK(kernel->execution() == Kernel::Execution::COMPUTE);
    CHECK(RuntimeType::cast<CustomKernel>(kernel.get()));
    CHECK_FALSE(UnlitKernel::create({}));
    CHECK_FALSE(SkyboxKernel::create({}));
}

TEST_CASE("fx2 unlit records independent values and leaves caller states intact", "[fx2][kernel][gpu]") {
    Fixture f;
    if (!f.gpu) SKIP("No gpu2 device available");
    REQUIRE(f.initialize());
    const auto      shared = f.ssc->takeSnapshot();
    RecordingRaster raster;
    REQUIRE(f.unlit->record(raster, shared.set0Resources, f.inputs));
    REQUIRE(raster.draws.size() == 1);
    CHECK(raster.draws[0].states == RasterState {});
    auto original  = raster.draws[0].immediates;
    f.inputs.color = {0, 1, 0, 1};
    REQUIRE(f.unlit->record(raster, shared.set0Resources, f.inputs));
    CHECK(raster.draws[1].immediates.get() != original.get());
    f.inputs.useVertexColor = true;
    CHECK_FALSE(f.unlit->record(raster, shared.set0Resources, f.inputs));
    CHECK(raster.draws.size() == 2);
    auto skybox = SkyboxKernel::create(f.gpu);
    REQUIRE(skybox);
    REQUIRE(skybox->record(raster, shared.set0Resources));
    const auto & sky = raster.draws.back();
    RasterState  expected;
    expected.cullMode   = RasterState::CULL_NONE;
    expected.depthState = RasterState::DepthState {RasterState::Compare::LESS_EQUAL, false};
    CHECK(sky.states == expected);
    CHECK_FALSE(skybox->record(raster, {}));
}

TEST_CASE("fx2 unlit reuses one raster and inherits alpha blending", "[fx2][kernel][gpu]") {
    Fixture f;
    if (!f.gpu) SKIP("No gpu2 device available");
    REQUIRE(f.initialize());
    auto texture = Texture::create(
        "kernel.target",
        {.context = f.gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(64, 64).setLevels(1)});
    REQUIRE(texture);
    GpuResourceView view;
    view.resource = texture;
    RasterTarget target;
    target.setColorTarget(0, view).setClearColor(0, 0, 0, 1);
    target.states.cullMode = RasterState::CULL_NONE;
    auto & blend           = target.colorTargets[0].blendState;
    blend.colorSrc         = RasterTarget::BlendState::SRC_ALPHA;
    blend.colorDst         = RasterTarget::BlendState::INV_SRC_ALPHA;
    auto raster            = GpuRaster::create("kernel-two-draws", {.gpu = f.gpu, .target = &target});
    REQUIRE(raster);
    const auto shared = f.ssc->takeSnapshot();
    f.inputs.color    = {1, 0, 0, 0.5f};
    REQUIRE(f.unlit->record(*raster, shared.set0Resources, f.inputs));
    f.inputs.color           = {0, 1, 0, 1};
    f.inputs.worldFromObject = glm::translate(glm::mat4(1), glm::vec3(1, 0, 0));
    REQUIRE(f.unlit->record(*raster, shared.set0Resources, f.inputs));
    f.inputs.color = {0, 0, 1, 1}; // Changing caller data cannot affect already recorded draws.
    auto payload   = raster->seal();
    REQUIRE(payload);
    GpuContext::SubmitParameters submit("kernel-two-draws");
    for (const auto & upload : shared.set0Payloads) submit.appendWork(upload);
    submit.appendWork(payload);
    f.gpu->submit(submit);
    f.gpu->waitForIdle();
    const auto image = texture->readback();
    REQUIRE_FALSE(image.empty());
    const auto * pixels = static_cast<const uint8_t *>(image.data());
    const auto * left   = pixels + (32 * 64 + 16) * 4;
    const auto * right  = pixels + (32 * 64 + 48) * 4;
    CHECK(left[0] >= 126);
    CHECK(left[0] <= 129);
    CHECK(left[1] == 0);
    CHECK(left[2] == 0);
    CHECK(right[0] == 0);
    CHECK(right[1] == 255);
    CHECK(right[2] == 0);
}

TEST_CASE("fx2 unlit handles optional texture and vertex color without dummy attributes", "[fx2][kernel][gpu]") {
    Fixture f;
    if (!f.gpu) SKIP("No gpu2 device available");
    REQUIRE(f.initialize());
    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec4 color;
    };
    const Vertex data[]   = {{{-1, -1, 0}, {0, 0}, {1, 0.5f, 0.25f, 1}},
                             {{0, -1, 0}, {1, 0}, {1, 0.5f, 0.25f, 1}},
                             {{-0.5f, 1, 0}, {0.5f, 1}, {1, 0.5f, 0.25f, 1}}};
    auto         vertices = Buffer::create("kernel.colored", {.context = f.gpu, .size = sizeof(data)});
    REQUIRE(vertices);
    REQUIRE(vertices->setContent({reinterpret_cast<const uint8_t *>(data), sizeof(data)}));
    auto tex = Texture::create(
        "kernel.color-map",
        {.context = f.gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1)});
    auto staging = Buffer::create("kernel.texture-upload", {.context = f.gpu, .size = 4, .mappable = true});
    REQUIRE(tex);
    REQUIRE(staging);
    {
        auto mapped = staging->map();
        REQUIRE(mapped.data());
        const uint8_t pixel[] = {128, 255, 255, 255};
        memcpy(mapped.data(), pixel, 4);
    }
    auto cnc = GpuCnC::create({.gpu = f.gpu});
    REQUIRE(cnc);
    GpuCnC::Region region;
    region.imageExtent = {1, 1, 1};
    cnc->copyBufferToImage({.src = staging, .dst = tex, .regions = {&region, 1}});
    f.gpu->submit(GpuContext::SubmitParameters("kernel.texture-upload").appendWork(cnc->seal()));
    for (bool textured : {false, true})
        for (bool colored : {false, true}) {
            CAPTURE(textured, colored);
            auto output = Texture::create(
                "kernel.optional-target",
                {.context    = f.gpu,
                 .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(64, 64).setLevels(1)});
            REQUIRE(output);
            GpuResourceView view;
            view.resource = output;
            RasterTarget target;
            target.setColorTarget(0, view).setClearColor(0, 0, 0, 1);
            target.states.cullMode = RasterState::CULL_NONE;
            auto raster            = GpuRaster::create("kernel.optional", {.gpu = f.gpu, .target = &target});
            REQUIRE(raster);
            f.inputs.geometry.vertices[0] = {.buffer = vertices, .offset = 0, .stride = sizeof(Vertex)};
            f.inputs.geometry.format.attributes.resize(1);
            if (textured)
                f.inputs.geometry.format.attributes.append(
                    {.location = 2, .binding = 0, .offset = offsetof(Vertex, uv), .format = RasterGeometry::AttributeFormat::F32_2});
            if (colored)
                f.inputs.geometry.format.attributes.append(
                    {.location = 4, .binding = 0, .offset = offsetof(Vertex, color), .format = RasterGeometry::AttributeFormat::F32_4});
            f.inputs.colorMap.resource = textured ? tex : AutoRef<Texture> {};
            f.inputs.useVertexColor    = colored;
            auto shared                = f.ssc->takeSnapshot();
            REQUIRE(f.unlit->record(*raster, shared.set0Resources, f.inputs));
            GpuContext::SubmitParameters submit("kernel.optional");
            for (auto & upload : shared.set0Payloads) submit.appendWork(upload);
            submit.appendWork(raster->seal());
            f.gpu->submit(submit);
            f.gpu->waitForIdle();
            auto image = output->readback();
            REQUIRE_FALSE(image.empty());
            auto pixel = static_cast<const uint8_t *>(image.data()) + (32 * 64 + 16) * 4;
            CHECK(std::abs(int(pixel[0]) - (textured ? 128 : 255)) <= 1);
            CHECK(std::abs(int(pixel[1]) - (colored ? 128 : 255)) <= 1);
            CHECK(std::abs(int(pixel[2]) - (colored ? 64 : 255)) <= 1);
        }
}

TEST_CASE("fx2 single-version SSC orders uploads between camera consumers", "[fx2][kernel][ssc][gpu]") {
    Fixture f;
    if (!f.gpu) SKIP("No gpu2 device available");
    REQUIRE(f.initialize());
    auto a                              = f.ssc->takeSnapshot();
    f.ssc->set0.camera.cameraPosition.x = 1;
    auto b                              = f.ssc->takeSnapshot();
    REQUIRE(a.set0Resources[1][0].resource == b.set0Resources[1][0].resource);
    AutoRef<Texture>    images[2];
    AutoRef<GpuPayload> draws[2];
    for (int i = 0; i < 2; ++i) {
        images[i] = Texture::create(
            "kernel.camera-target",
            {.context = f.gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(64, 64).setLevels(1)});
        REQUIRE(images[i]);
        GpuResourceView view;
        view.resource = images[i];
        RasterTarget target;
        target.setColorTarget(0, view).setClearColor(0, 0, 0, 1);
        target.states.cullMode = RasterState::CULL_NONE;
        auto raster            = GpuRaster::create("kernel.camera", {.gpu = f.gpu, .target = &target});
        REQUIRE(raster);
        REQUIRE(f.unlit->record(*raster, (i == 0 ? a : b).set0Resources, f.inputs));
        draws[i] = raster->seal();
        REQUIRE(draws[i]);
    }
    GpuContext::SubmitParameters submit("kernel.camera-order");
    for (auto & upload : a.set0Payloads) submit.appendWork(upload);
    submit.appendWork(draws[0]);
    for (auto & upload : b.set0Payloads) submit.appendWork(upload);
    submit.appendWork(draws[1]);
    f.gpu->submit(submit);
    f.gpu->waitForIdle();
    for (int i = 0; i < 2; ++i) {
        auto image = images[i]->readback();
        REQUIRE_FALSE(image.empty());
        auto pixel = static_cast<const uint8_t *>(image.data()) + (32 * 64 + 16) * 4;
        CHECK(pixel[0] == (i == 0 ? 255 : 0));
    }
}

TEST_CASE("fx2 callers preserve SSC initialization when abandoning the first snapshot", "[fx2][kernel][ssc][gpu]") {
    Fixture f;
    if (!f.gpu) SKIP("No gpu2 device available");
    REQUIRE(f.initialize());
    f.ssc->set0.camera.exposure = 1;
    auto abandoned              = f.ssc->takeSnapshot();
    // SSC hands initialization out once. Preserve its payloads even though these
    // snapshot bindings and their intended draws are discarded.
    auto initialization = std::move(abandoned.set0Payloads);
    abandoned           = {};
    auto shared         = f.ssc->takeSnapshot();
    auto skybox         = SkyboxKernel::create(f.gpu);
    REQUIRE(skybox);
    auto texture = Texture::create(
        "kernel.initialization-target",
        {.context = f.gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(64, 64).setLevels(1)});
    REQUIRE(texture);
    GpuResourceView view;
    view.resource = texture;
    RasterTarget target;
    target.setColorTarget(0, view).setClearColor(0, 0, 0, 1);
    auto raster = GpuRaster::create("kernel.initialization", {.gpu = f.gpu, .target = &target});
    REQUIRE(raster);
    REQUIRE(skybox->record(*raster, shared.set0Resources));
    GpuContext::SubmitParameters submit("kernel.initialization");
    for (const auto & work : initialization) submit.appendWork(work);
    for (const auto & work : shared.set0Payloads) submit.appendWork(work);
    submit.appendWork(raster->seal());
    f.gpu->submit(submit);
    f.gpu->waitForIdle();
    auto image = texture->readback();
    REQUIRE_FALSE(image.empty());
    const auto * center = static_cast<const uint8_t *>(image.data()) + (32 * 64 + 32) * 4;
    CHECK(center[0] > 0);
    CHECK(center[1] > center[0]);
    CHECK(center[2] > center[1]);
}

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include <limits>
using namespace GN;
using namespace GN::fx2;
using namespace GN::gpu2;
namespace {
struct LitRecordingRaster final : GpuRaster {
    GN_REGISTER_RUNTIME_TYPE(GpuRaster);
    RasterTarget              destination;
    DynaArray<DrawParameters> draws;
    LitRecordingRaster(): GpuRaster(TYPE_INFO(), "lit-recording") {}
    void                 draw(const DrawParameters & d) override { draws.append(d); }
    const RasterTarget & target() const override { return destination; }
    AutoRef<GpuPayload>  seal() override { return {}; }
};
} // namespace
TEST_CASE("fx2 typed lit kernels preserve state and isolate invocation uniforms", "[fx2][kernel][gpu]") {
    auto gpu = GpuContext::create("lit-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 device available");
    auto init = GpuCnC::create({.gpu = gpu});
    REQUIRE(init);
    auto pbr     = PbrKernel::create(gpu, *init);
    auto cel     = CelKernel::create(gpu, *init);
    auto lambert = LambertianKernel::create(gpu, *init);
    REQUIRE(pbr);
    REQUIRE(cel);
    REQUIRE(lambert);
    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);
    auto shared   = ssc->takeSnapshot();
    auto vertices = Buffer::create("lit.vertices", {.context = gpu, .size = 72});
    REQUIRE(vertices);
    PbrKernel::Inputs input;
    input.geometry.vertices.append({.buffer = vertices, .offset = 0, .stride = 24});
    input.geometry.vertexCount = 3;
    input.geometry.format.attributes.append({.location = 0, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    input.geometry.format.attributes.append({.location = 1, .binding = 0, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_3});
    input.states.cullMode = RasterState::CULL_NONE;
    auto uploads          = GpuCnC::create({.gpu = gpu});
    REQUIRE(uploads);
    LitRecordingRaster raster;
    REQUIRE(pbr->record(raster, *uploads, shared.set0Resources, input));
    REQUIRE(raster.draws.size() == 1);
    CHECK(raster.draws[0].states == input.states);
    input.color = {0, 1, 0, 1};
    REQUIRE(pbr->record(raster, *uploads, shared.set0Resources, input));
    CHECK(raster.draws[0].resources[1][5][0].resource != raster.draws[1].resources[1][5][0].resource);
    input.metallic = std::numeric_limits<float>::quiet_NaN();
    CHECK_FALSE(pbr->record(raster, *uploads, shared.set0Resources, input));
    CHECK(raster.draws.size() == 2);
    input.metallic       = 0;
    input.useVertexColor = true;
    CHECK_FALSE(pbr->record(raster, *uploads, shared.set0Resources, input));
    input.useVertexColor = false;
    LambertianKernel::Inputs li;
    static_cast<LitKernelInputs &>(li) = input;
    REQUIRE(lambert->record(raster, *uploads, shared.set0Resources, li));
    CelKernel::Inputs ci;
    static_cast<LitKernelInputs &>(ci) = input;
    REQUIRE(cel->record(raster, *uploads, shared.set0Resources, ci));
    REQUIRE(raster.draws.size() == 5);
    CHECK(raster.draws[3].states == input.states);
    RasterState outline = input.states;
    outline.cullMode    = RasterState::CULL_FRONT;
    outline.depthState  = RasterState::DepthState {RasterState::Compare::LESS_EQUAL, true};
    CHECK(raster.draws[4].states == outline);
    gpu->waitForIdle();
}
TEST_CASE("fx2 lit kernels render position-normal buffers with independent private values", "[fx2][kernel][gpu]") {
    auto gpu = GpuContext::create("lit-render-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 device available");
    auto init = GpuCnC::create({.gpu = gpu});
    REQUIRE(init);
    auto pbr     = PbrKernel::create(gpu, *init);
    auto cel     = CelKernel::create(gpu, *init);
    auto lambert = LambertianKernel::create(gpu, *init);
    REQUIRE(pbr);
    REQUIRE(cel);
    REQUIRE(lambert);
    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);
    ssc->set0.camera.cameraPosition   = {0, 0, 1};
    ssc->set0.camera.cameraFov        = ArcDegree(90);
    ssc->set0.camera.aspectRatio      = 1;
    ssc->set0.camera.viewWidthInPixel = ssc->set0.camera.viewHeightInPixel = 64;
    ssc->set0.camera.exposure                                              = 1;
    ssc->set0.envLighting.environmentLuminanceScale                        = 0;
    const float data[]                                                     = {-1, -1, 0, 0, 0, 1, 0, -1, 0, 0, 0, 1, -.5f, 1, 0, 0, 0, 1};
    auto        vertices                                                   = Buffer::create("lit.vertices", {.context = gpu, .size = sizeof(data)});
    REQUIRE(vertices);
    init->uploadBuffer(vertices, 0, {reinterpret_cast<const uint8_t *>(data), sizeof(data)});
    gpu->submit(GpuContext::SubmitParameters("lit.init").appendWork(init->seal()));
    for (int kind = 0; kind < 3; ++kind) {
        CAPTURE(kind);
        auto output = Texture::create(
            "lit.target",
            {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(64, 64).setLevels(1)});
        REQUIRE(output);
        GpuResourceView view;
        view.resource = output;
        RasterTarget target;
        target.setColorTarget(0, view).setClearColor(0, 0, 0, 1);
        target.states.cullMode = RasterState::CULL_NONE;
        auto raster            = GpuRaster::create("lit.render", {.gpu = gpu, .target = &target});
        auto uploads           = GpuCnC::create({.gpu = gpu});
        REQUIRE(raster);
        REQUIRE(uploads);
        auto shared = ssc->takeSnapshot();
        for (int side = 0; side < 2; ++side) {
            LitKernelInputs common;
            common.geometry.vertices.append({.buffer = vertices, .offset = 0, .stride = 24});
            common.geometry.vertexCount = 3;
            common.geometry.format.attributes.append({.location = 0, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
            common.geometry.format.attributes.append({.location = 1, .binding = 0, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_3});
            common.color                 = {0, 0, 0, 1};
            common.emissive              = side ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
            common.worldFromObject[3][0] = float(side);
            if (kind == 0) {
                PbrKernel::Inputs i;
                static_cast<LitKernelInputs &>(i) = common;
                REQUIRE(pbr->record(*raster, *uploads, shared.set0Resources, i));
            }
            if (kind == 1) {
                LambertianKernel::Inputs i;
                static_cast<LitKernelInputs &>(i) = common;
                REQUIRE(lambert->record(*raster, *uploads, shared.set0Resources, i));
            }
            if (kind == 2) {
                CelKernel::Inputs i;
                static_cast<LitKernelInputs &>(i) = common;
                i.outlineWidth                    = 0;
                i.specularIntensity               = 0;
                i.rimIntensity                    = 0;
                REQUIRE(cel->record(*raster, *uploads, shared.set0Resources, i));
            }
        }
        GpuContext::SubmitParameters submit("lit.render");
        for (auto & p : shared.set0Payloads) submit.appendWork(p);
        submit.appendWork(uploads->seal()).appendWork(raster->seal());
        gpu->submit(submit);
        gpu->waitForIdle();
        auto image = output->readback();
        REQUIRE_FALSE(image.empty());
        auto pixels = static_cast<const uint8_t *>(image.data());
        auto left   = pixels + (32 * 64 + 16) * 4;
        auto right  = pixels + (32 * 64 + 48) * 4;
        CHECK(left[0] > 100);
        CHECK(left[1] < 5);
        CHECK(right[0] < 5);
        CHECK(right[1] > 100);
    }
}

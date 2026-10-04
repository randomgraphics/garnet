#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include <limits>
#include <thread>
#include <glm/gtc/packing.hpp>
using namespace GN;
using namespace GN::fx2;
using namespace GN::gpu2;
namespace {
struct LitRecordingRaster final : GpuRaster {
    GN_REGISTER_RUNTIME_TYPE(GpuRaster);
    RasterTarget destination;
    struct RecordedDraw {
        AutoRef<GpuShader>  vs, hs, ds, gs, ps;
        RasterState         states;
        RasterGeometry      geometry;
        GpuResourceTable    resources;
        AutoRef<const Blob> immediates;
        RecordedDraw(const DrawParameters & p)
            : vs(p.vs), hs(p.hs), ds(p.ds), gs(p.gs), ps(p.ps), states(p.states), geometry(p.geometry), resources(p.resources), immediates(p.immediates) {}
    };
    DynaArray<RecordedDraw> draws;
    LitRecordingRaster(): GpuRaster(TYPE_INFO(), "lit-recording") {}
    void                 recordDraw(const DrawParameters & d) override { draws.append(d); }
    const RasterTarget & target() const override { return destination; }
    AutoRef<GpuPayload>  seal() override { return {}; }
};
} // namespace
TEST_CASE("fx2 typed lit kernels preserve state and isolate invocation uniforms", "[fx2][lit][kernel][gpu]") {
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
    input.geometry.vertices.push_back({.buffer = vertices, .offset = 0, .stride = 24});
    input.geometry.vertexCount = 3;
    input.geometry.format.attributes.push_back({.location = 0, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    input.geometry.format.attributes.push_back({.location = 1, .binding = 0, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_3});
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
TEST_CASE("fx2 lit kernels render position-normal buffers with independent private values", "[fx2][lit][kernel][gpu]") {
    bool generated = false, half = false;
    SECTION("position-normal input") {}
    SECTION("generated float geometry") { generated = true; }
    SECTION("generated half geometry") {
        generated = true;
        half      = true;
    }
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
    init->recordUploadBuffer(vertices, 0, {reinterpret_cast<const uint8_t *>(data), sizeof(data)});
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
            common.geometry.vertices.push_back({.buffer = vertices, .offset = 0, .stride = 24});
            common.geometry.vertexCount = 3;
            common.geometry.format.attributes.push_back({.location = 0, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
            common.geometry.format.attributes.push_back({.location = 1, .binding = 0, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_3});
            common.color                 = {0, 0, 0, 1};
            common.emissive              = side ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
            common.worldFromObject[3][0] = float(side);
            if (generated) {
                if (side == 0) {
                    LitKernelInputs::CubeCreateOptions options;
                    options.width   = 0.8f;
                    options.height  = 1;
                    options.depth   = 0.2f;
                    options.half    = half;
                    common.geometry = LitKernelInputs::createBox(gpu, *uploads, options);
                } else {
                    LitKernelInputs::SphereCreateOptions options;
                    options.radius  = 0.4f;
                    options.half    = half;
                    common.geometry = LitKernelInputs::createSphere(gpu, *uploads, options);
                }
                REQUIRE(common.geometry.indexCount > 0);
                common.worldFromObject[3][0] -= 0.5f;
            }
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

TEST_CASE("fx2 renders 1024 independent materials in one raster", "[fx2][lit][kernel][gpu]") {
    auto gpu = GpuContext::create("many-materials", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 device available");
    auto initialization = GpuCnC::create({.gpu = gpu});
    REQUIRE(initialization);
    auto pbr = PbrKernel::create(gpu, *initialization);
    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(pbr);
    REQUIRE(ssc);
    constexpr uint32_t grid = 32, tile = 4, extent = grid * tile;
    ssc->set0.camera.cameraPosition   = {0, 0, 1};
    ssc->set0.camera.cameraFov        = ArcDegree(90);
    ssc->set0.camera.aspectRatio      = 1;
    ssc->set0.camera.viewWidthInPixel = ssc->set0.camera.viewHeightInPixel = extent;
    ssc->set0.camera.exposure                                              = 1;
    ssc->set0.envLighting.environmentLuminanceScale                        = 0;
    const float vertices[]                                                 = {-1, -1, 0, 0, 0, 1, 3, -1, 0, 0, 0, 1, -1, 3, 0, 0, 0, 1};
    auto        buffer = Buffer::create("many-materials.vertices", {.context = gpu, .size = sizeof(vertices)});
    REQUIRE(buffer);
    initialization->recordUploadBuffer(buffer, 0, {reinterpret_cast<const uint8_t *>(vertices), sizeof(vertices)});
    gpu->submit(GpuContext::SubmitParameters("many-materials.init").appendWork(initialization->seal()));
    auto output = Texture::create(
        "many-materials.output",
        {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(extent, extent).setLevels(1)});
    REQUIRE(output);
    GpuResourceView view;
    view.resource = output;
    RasterTarget target;
    target.setColorTarget(0, view).setClearColor(0, 0, 0, 1);
    target.states.cullMode     = RasterState::CULL_NONE;
    constexpr size_t   workers = 4;
    AutoRef<GpuRaster> rasters[workers];
    AutoRef<GpuCnC>    uploads[workers];
    bool               recorded[workers] {};
    for (size_t i = 0; i < workers; ++i) {
        rasters[i] = GpuRaster::create("many-materials.render", {.gpu = gpu, .target = &target, .numberOfDrawsHint = grid * grid});
        uploads[i] = GpuCnC::create({.gpu = gpu});
        REQUIRE(rasters[i]);
        REQUIRE(uploads[i]);
    }
    const auto        shared = ssc->takeSnapshot();
    PbrKernel::Inputs input;
    input.geometry.vertices.push_back({.buffer = buffer, .offset = 0, .stride = 24});
    input.geometry.vertexCount = 3;
    input.geometry.format.attributes.push_back({.location = 0, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    input.geometry.format.attributes.push_back({.location = 1, .binding = 0, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_3});
    input.color = {0, 0, 0, 1};
    // Each worker owns its recorder; the kernel and immutable GPU inputs are shared.
    // Catch assertions stay on the joining thread because worker assertions are unsupported.
    std::thread threads[workers];
    for (size_t i = 0; i < workers; ++i) {
        threads[i] = std::thread([&, i, local = input]() mutable {
            bool ok = true;
            for (uint32_t y = 0; y < grid; ++y) {
                for (uint32_t x = 0; x < grid; ++x) {
                    local.emissive           = {0.01f * (x + 1), 0.01f * (y + 1), 0};
                    local.states.viewport    = RasterState::Viewport {float(x * tile), float(y * tile), float(tile), float(tile)};
                    local.states.scissorRect = RasterState::ScissorRect {int32_t(x * tile), int32_t(y * tile), tile, tile};
                    ok                       = pbr->record(*rasters[i], *uploads[i], shared.set0Resources, local) && ok;
                }
            }
            recorded[i] = ok;
        });
    }
    for (auto & thread : threads) thread.join();
    for (size_t worker = 0; worker < workers; ++worker) {
        CAPTURE(worker);
        REQUIRE(recorded[worker]);
        GpuContext::SubmitParameters submit("many-materials.render");
        for (const auto & payload : shared.set0Payloads) submit.appendWork(payload);
        submit.appendWork(uploads[worker]->seal()).appendWork(rasters[worker]->seal());
        gpu->submit(submit);
        gpu->waitForIdle();
        const auto image = output->readback();
        REQUIRE_FALSE(image.empty());
        const auto pixels = image.plane().toRGBA8(image.data());
        const auto pixel  = [&](uint32_t x, uint32_t y) { return pixels[(y * tile + tile / 2) * extent + x * tile + tile / 2]; };
        for (uint32_t y = 0; y < grid; ++y) {
            for (uint32_t x = 0; x < grid; ++x) {
                CAPTURE(x, y);
                const auto p = pixel(x, y);
                CHECK(p.r > 0);
                CHECK(p.g > 0);
                CHECK(p.b == 0);
                if (x) CHECK(p.r > pixel(x - 1, y).r);
                if (y) CHECK(p.g > pixel(x, y - 1).g);
            }
        }
    }
}

TEST_CASE("fx2 generated lit geometry preserves packed attributes and surface frames", "[fx2][lit][geometry][gpu]") {
    auto gpu = GpuContext::create("lit-geometry-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 device available");
    for (int shape = 0; shape < 3; ++shape)
        for (bool half : {false, true})
            for (bool ccw : {false, true}) {
                CAPTURE(shape, half, ccw);
                auto uploads = GpuCnC::create({.gpu = gpu});
                REQUIRE(uploads);
                LitKernelInputs::CubeCreateOptions box;
                box.width   = 2;
                box.height  = 4;
                box.depth   = 6;
                box.half    = half;
                box.ccw     = ccw;
                box.colored = true;
                box.color   = {0.25f, 0.5f, 1};
                LitKernelInputs::SphereCreateOptions sphere;
                static_cast<LitKernelInputs::GeometryCreateOptions &>(sphere) = box;
                sphere.radius                                                 = 2;
                sphere.slices                                                 = 12;
                sphere.stacks                                                 = 6;
                sphere.subDivides                                             = 1;
                sphere.type   = shape == 2 ? LitKernelInputs::SphereCreateOptions::ICO : LitKernelInputs::SphereCreateOptions::UV;
                auto geometry = shape == 0 ? LitKernelInputs::createBox(gpu, *uploads, box) : LitKernelInputs::createSphere(gpu, *uploads, sphere);
                REQUIRE(geometry.vertices.size() == 1);
                REQUIRE(geometry.format.attributes.size() == 5);
                CHECK(geometry.indexCount == (shape == 0 ? 36u : shape == 1 ? 360u : 240u));
                CHECK(geometry.indices.stride == 2);
                gpu->submit(GpuContext::SubmitParameters("lit.geometry").appendWork(uploads->seal()));
                const auto vertices = geometry.vertices[0].buffer->readContent();
                const auto indices  = geometry.indices.buffer->readContent();
                REQUIRE(vertices.size() == size_t(geometry.vertexCount) * geometry.vertices[0].stride);
                REQUIRE(indices.size() == size_t(geometry.indexCount) * geometry.indices.stride);
                auto attribute = [&](uint32_t vertex, uint32_t location) {
                    const auto &   a     = geometry.format.attributes[location];
                    const uint32_t count = location == 2 ? 2 : location < 2 ? 3 : 4;
                    glm::vec4      value(0);
                    for (uint32_t c = 0; c < count; ++c) {
                        const auto * src = vertices.data() + vertex * geometry.vertices[0].stride + a.offset + c * (half ? 2 : 4);
                        if (half) {
                            uint16_t v;
                            memcpy(&v, src, 2);
                            value[c] = glm::unpackHalf1x16(v);
                        } else {
                            memcpy(&value[c], src, 4);
                        }
                    }
                    return value;
                };
                for (uint32_t v = 0; v < geometry.vertexCount; ++v) {
                    const auto p = glm::vec3(attribute(v, 0)), n = glm::vec3(attribute(v, 1));
                    const auto t = attribute(v, 3);
                    CHECK(std::abs(glm::length(n) - 1) < 0.002f);
                    CHECK(std::abs(glm::length(glm::vec3(t)) - 1) < 0.002f);
                    CHECK(std::abs(glm::dot(n, glm::vec3(t))) < 0.002f);
                    CHECK(attribute(v, 4) == glm::vec4(box.color, 1));
                    if (shape == 0) {
                        CHECK(std::abs(p.x) == 1);
                        CHECK(std::abs(p.y) == 2);
                        CHECK(std::abs(p.z) == 3);
                    } else
                        CHECK(std::abs(glm::length(p) - 2) < 0.002f);
                }
                for (uint32_t i = 0; i < geometry.indexCount; i += 3) {
                    uint16_t ids[3];
                    memcpy(ids, indices.data() + i * 2, sizeof(ids));
                    glm::vec3 p[3], n[3];
                    glm::vec2 uv[3];
                    for (int j = 0; j < 3; ++j) {
                        REQUIRE(ids[j] < geometry.vertexCount);
                        p[j]  = attribute(ids[j], 0);
                        n[j]  = attribute(ids[j], 1);
                        uv[j] = attribute(ids[j], 2);
                    }
                    const float facing = glm::dot(glm::cross(p[1] - p[0], p[2] - p[0]), n[0] + n[1] + n[2]);
                    CHECK((ccw ? facing : -facing) > 0);
                    CHECK(std::max({uv[0].x, uv[1].x, uv[2].x}) - std::min({uv[0].x, uv[1].x, uv[2].x}) <= (shape ? 0.501f : 1.001f));
                    const auto  duv1 = uv[1] - uv[0], duv2 = uv[2] - uv[0];
                    const float det = duv1.x * duv2.y - duv1.y * duv2.x;
                    REQUIRE(std::abs(det) > 0.00001f);
                    const auto bitangent = ((p[2] - p[0]) * duv1.x - (p[1] - p[0]) * duv2.x) / det;
                    const auto tangent   = attribute(ids[0], 3);
                    CHECK(glm::dot(glm::cross(n[0], glm::vec3(tangent)) * tangent.w, bitangent) > 0);
                }
            }
}

TEST_CASE("fx2 lit geometry handles optional attributes and rejects invalid options", "[fx2][lit][geometry][gpu]") {
    auto gpu = GpuContext::create("lit-options-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 device available");
    auto uploads = GpuCnC::create({.gpu = gpu});
    REQUIRE(uploads);
    for (uint32_t mask = 0; mask < 32; ++mask) {
        CAPTURE(mask);
        LitKernelInputs::CubeCreateOptions options;
        options.uv          = (mask & 1) != 0;
        options.normal      = (mask & 2) != 0;
        options.tangent     = (mask & 4) != 0;
        options.colored     = (mask & 8) != 0;
        options.half        = (mask & 16) != 0;
        const auto geometry = LitKernelInputs::createBox(gpu, *uploads, options);
        REQUIRE(geometry.vertexCount == 24);
        const bool enabled[] = {true, options.normal, options.uv, options.tangent && options.normal && options.uv, options.colored};
        uint32_t   offset = 0, index = 0;
        for (uint32_t location = 0; location < 5; ++location) {
            if (!enabled[location]) continue;
            REQUIRE(index < geometry.format.attributes.size());
            const auto & attribute = geometry.format.attributes[index++];
            CHECK(attribute.location == location);
            CHECK(attribute.offset == offset);
            const uint32_t count = location == 2 ? 2 : location < 2 ? 3 : 4;
            CHECK(uint32_t(attribute.format) ==
                  uint32_t(options.half ? RasterGeometry::AttributeFormat::F16_1 : RasterGeometry::AttributeFormat::F32_1) + count - 1);
            offset += count * (options.half ? 2 : 4);
        }
        CHECK(index == geometry.format.attributes.size());
        CHECK(offset == geometry.vertices[0].stride);
    }
    LitKernelInputs::CubeCreateOptions box;
    box.width = 0;
    CHECK(LitKernelInputs::createBox(gpu, *uploads, box).vertices.empty());
    box.width = std::numeric_limits<float>::infinity();
    CHECK(LitKernelInputs::createBox(gpu, *uploads, box).vertices.empty());
    box.width   = 1;
    box.half    = true;
    box.colored = true;
    box.color.x = 70000;
    CHECK(LitKernelInputs::createBox(gpu, *uploads, box).vertices.empty());
    CHECK(LitKernelInputs::createBox({}, *uploads).vertices.empty());
    LitKernelInputs::SphereCreateOptions sphere;
    sphere.slices = 2;
    CHECK(LitKernelInputs::createSphere(gpu, *uploads, sphere).vertices.empty());
    sphere.slices = std::numeric_limits<uint32_t>::max();
    CHECK(LitKernelInputs::createSphere(gpu, *uploads, sphere).vertices.empty());
    sphere.stacks = std::numeric_limits<uint32_t>::max();
    CHECK(LitKernelInputs::createSphere(gpu, *uploads, sphere).vertices.empty());
    sphere.type       = LitKernelInputs::SphereCreateOptions::ICO;
    sphere.subDivides = 9;
    CHECK(LitKernelInputs::createSphere(gpu, *uploads, sphere).vertices.empty());
    sphere.type = static_cast<LitKernelInputs::SphereCreateOptions::Type>(10);
    CHECK(LitKernelInputs::createSphere(gpu, *uploads, sphere).vertices.empty());
    sphere           = {};
    sphere.slices    = 256;
    sphere.stacks    = 256;
    const auto large = LitKernelInputs::createSphere(gpu, *uploads, sphere);
    CHECK(large.vertexCount == 257 * 257);
    CHECK(large.indices.stride == 4);
    gpu->submit(GpuContext::SubmitParameters("lit.options").appendWork(uploads->seal()));
    gpu->waitForIdle();
}

TEST_CASE("fx2 lit surfaces and cel bands survive a later skybox", "[fx2][lit][gpu]") {
    auto gpu = GpuContext::create("lit-depth-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 device available");
    auto initialization = GpuCnC::create({.gpu = gpu});
    REQUIRE(initialization);
    auto pbr    = PbrKernel::create(gpu, *initialization);
    auto cel    = CelKernel::create(gpu, *initialization);
    auto skybox = SkyboxKernel::create(gpu);
    auto ssc    = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(pbr);
    REQUIRE(cel);
    REQUIRE(skybox);
    REQUIRE(ssc);
    ssc->set0.camera.cameraPosition   = {0, 0, 3};
    ssc->set0.camera.viewWidthInPixel = ssc->set0.camera.viewHeightInPixel = 128;
    ssc->set0.camera.aspectRatio                                           = 1;
    ssc->set0.envLighting.environmentLuminanceScale                        = 0;
    // PBR uses image-based lighting; supply a calibrated floor without external texture assets.
    ssc->set0.envLighting.environmentAmbientFloor = 500;
    SharedShaderConstants::DirectLight sun;
    sun.type                    = SharedShaderConstants::DirectLight::DIRECTIONAL;
    sun.directional.orientation = glm::quat(glm::vec3(0.6f, 0.8f, 0));
    sun.directional.irradiance  = {1, 0.98f, 0.95f, {500}};
    ssc->set0.directLighting.append(sun);
    auto geometry = LitKernelInputs::createSphere(gpu, *initialization);
    REQUIRE(geometry.indexCount > 0);
    gpu->submit(GpuContext::SubmitParameters("lit-depth.init").appendWork(initialization->seal()));
    for (int kind = 0; kind < 2; ++kind) {
        CAPTURE(kind);
        auto output = Texture::create(
            "lit-depth.color",
            {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(128, 128).setLevels(1)});
        auto depth = Texture::create(
            "lit-depth.depth",
            {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(128, 128).setLevels(1)});
        REQUIRE(output);
        REQUIRE(depth);
        GpuResourceView colorView, depthView;
        colorView.resource = output;
        depthView.resource = depth;
        RasterTarget target;
        target.setColorTarget(0, colorView).setDepthStencilTarget(depthView).setClearDepth(1).setClearColor(0, 0, 0, 1);
        target.states.depthState = RasterState::DepthState {RasterState::Compare::LESS, true};
        auto raster              = GpuRaster::create("lit-depth.raster", {.gpu = gpu, .target = &target});
        auto uploads             = GpuCnC::create({.gpu = gpu});
        REQUIRE(raster);
        REQUIRE(uploads);
        const auto shared = ssc->takeSnapshot();
        if (kind == 0) {
            PbrKernel::Inputs input;
            input.geometry = geometry;
            input.color    = {0.8f, 0.3f, 0.08f, 1};
            REQUIRE(pbr->record(*raster, *uploads, shared.set0Resources, input));
        } else {
            CelKernel::Inputs input;
            input.geometry     = geometry;
            input.color        = {0.8f, 0.3f, 0.08f, 1};
            input.outlineWidth = 0.01f;
            input.rimIntensity = 150;
            REQUIRE(cel->record(*raster, *uploads, shared.set0Resources, input));
        }
        REQUIRE(skybox->record(*raster, shared.set0Resources));
        GpuContext::SubmitParameters submit("lit-depth.render");
        for (const auto & payload : shared.set0Payloads) submit.appendWork(payload);
        submit.appendWork(uploads->seal()).appendWork(raster->seal());
        gpu->submit(submit);
        const auto image = output->readback();
        REQUIRE_FALSE(image.empty());
        if (const auto path = getEnv("GN_FX2_TEST_SNAPSHOT"); !path.empty()) image.save(StrA::format("{}-{}.png", path, kind).data());
        const auto * pixels  = static_cast<const uint8_t *>(image.data());
        uint32_t     visible = 0, litBand = 0, shadowBand = 0;
        for (uint32_t y = 20; y < 108; ++y)
            for (uint32_t x = 20; x < 108; ++x) {
                const auto * pixel = pixels + (y * 128 + x) * 4;
                if (pixel[0] > 30) {
                    ++visible;
                    if (pixel[0] > 108) ++litBand;
                    if (pixel[0] > 70 && pixel[0] < 90) ++shadowBand;
                }
            }
        CHECK(visible > 1000);
        CHECK(pixels[0] == 0);
        CHECK(pixels[(64 * 128 + 64) * 4] > 30);
        if (kind == 1) {
            CHECK(litBand > 100);
            CHECK(shadowBand > 100);
        }
    }
}

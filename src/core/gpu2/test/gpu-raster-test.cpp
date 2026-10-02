#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>

#include <atomic>

using namespace GN;
using namespace GN::gpu2;

TEST_CASE("GPU2: geometry and resource tables transfer storage on move", "[gpu2][raster]") {
    RasterGeometry geometry;
    geometry.format.attributes.append(RasterGeometry::VertexAttribute {.location = 3});
    geometry.vertices.append(RasterGeometry::GeometryBuffer {.offset = 16, .stride = 24});
    geometry.instances.append(RasterGeometry::GeometryBuffer {.offset = 32, .stride = 48});
    geometry.indices.offset   = 64;
    geometry.vertexCount      = 7;
    geometry.instanceCount    = 2;
    geometry.indexCount       = 9;
    const auto     attributes = geometry.format.attributes.data();
    const auto     vertices   = geometry.vertices.data();
    const auto     instances  = geometry.instances.data();
    RasterGeometry moved(std::move(geometry));
    CHECK(moved.format.attributes.data() == attributes);
    CHECK(moved.vertices.data() == vertices);
    CHECK(moved.instances.data() == instances);
    CHECK(geometry.format.attributes.empty());
    CHECK(geometry.vertices.empty());
    CHECK(geometry.instances.empty());
    geometry.vertices.append(RasterGeometry::GeometryBuffer {});
    geometry = std::move(moved);
    CHECK(geometry.format.attributes.data() == attributes);
    CHECK(geometry.vertices.data() == vertices);
    CHECK(geometry.instances.data() == instances);
    CHECK(geometry.indices.offset == 64);
    CHECK(geometry.vertexCount == 7);
    CHECK(geometry.instanceCount == 2);
    CHECK(geometry.indexCount == 9);

    GpuResourceTable table;
    table.resize(1);
    table[0].resize(1);
    table[0][0].resize(1);
    const auto       sets     = table.data();
    const auto       bindings = table[0].data();
    const auto       views    = table[0][0].data();
    GpuResourceTable movedTable(std::move(table));
    CHECK(table.empty());
    CHECK(movedTable.data() == sets);
    CHECK(movedTable[0].data() == bindings);
    CHECK(movedTable[0][0].data() == views);
    table.resize(2);
    table = std::move(movedTable);
    CHECK(movedTable.empty());
    CHECK(table.data() == sets);
    CHECK(table[0].data() == bindings);
    CHECK(table[0][0].data() == views);
}

TEST_CASE("GPU2: GpuRaster empty raster clears render target to blue", "[gpu2][raster][gpu]") {
    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 16, H = 16;

    // Offscreen RGBA8 texture used as the render target.
    auto texture = Texture::create(
        "rt", Texture::CreateParameters {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, H)});
    if (!texture) SKIP("RGBA8 color-attachment texture unavailable");

    // Clear-only raster pass: no draw calls, pure blue clear color.
    GpuResourceView view;
    view.resource = texture; // AutoRef<Texture> implicitly widens to AutoRef<Entity>
    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget(view));
    rt.setClearColor(0.0f, 0.0f, 1.0f);
    auto raster = GpuRaster::create("clear-blue", {.gpu = gpu, .target = &rt});
    REQUIRE(raster);
    REQUIRE(raster->target().clearColor.f4[3] == 1.0f);
    AutoRef<GpuPayload> rasterPayload = raster->seal();

    // Submit and block on CPU until the GPU fence fires.
    std::atomic<bool> done = false;
    gpu->submit(GpuContext::SubmitParameters("clear-blue").appendWork(rasterPayload).setOnComplete([&done] { done = true; }));
    while (!done) gpu->pump();

    // Readback and verify every pixel is (R=0, G=0, B=255, A=255).
    gfx::img::Image image = texture->readback();
    REQUIRE_FALSE(image.empty());
    REQUIRE(image.width() == W);
    REQUIRE(image.height() == H);

    // toRGBA8 converts from the native pixel format (RGBA8 UNORM) — identity here.
    std::vector<gfx::img::RGBA8> pixels = image.plane().toRGBA8(image.data());
    REQUIRE(pixels.size() == (size_t) (W * H));
    for (const auto & px : pixels) {
        CHECK(px.r == 0u);
        CHECK(px.g == 0u);
        CHECK(px.b == 255u);
        CHECK(px.a == 255u);
    }
}

TEST_CASE("GPU2: raster validates attachment subresources and clips to depth mip extent", "[gpu2][raster][gpu]") {
    auto gpu = GpuContext::create("mip-target-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");
    auto color = Texture::create(
        "mip-color", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(8, 8).setLevels(4)});
    REQUIRE(color);
    GpuResourceView colorView;
    colorView.resource = color;
    RasterTarget target;
    target.setColorTarget(0, colorView);
    target.colorTargets[0].target.mip = 4;
    CHECK_FALSE(GpuRaster::create("invalid-color-mip", {.gpu = gpu, .target = &target}));
    target.colorTargets[0].target.mip  = 0;
    target.colorTargets[0].target.face = 1;
    CHECK_FALSE(GpuRaster::create("invalid-color-face", {.gpu = gpu, .target = &target}));
    target.colorTargets[0].target.face = 0;
    const auto depthFormat             = gpu->caps().defaultDepthFormat;
    if (depthFormat == gfx::img::PixelFormat::UNKNOWN()) SKIP("No depth attachment format");
    auto depth = Texture::create("mip-depth", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(depthFormat).setDimensions(8, 8).setLevels(4)});
    REQUIRE(depth);
    GpuResourceView depthView;
    depthView.resource = depth;
    target.setDepthStencilTarget(depthView);
    target.depthStencilTarget.mip = 4;
    CHECK_FALSE(GpuRaster::create("invalid-depth-mip", {.gpu = gpu, .target = &target}));
    target.depthStencilTarget.mip  = 0;
    target.depthStencilTarget.face = 1;
    CHECK_FALSE(GpuRaster::create("invalid-depth-face", {.gpu = gpu, .target = &target}));
    target.depthStencilTarget.face = 0;
    target.setClearColor(1, 0, 0, 1);
    auto initial = GpuRaster::create("clear-base-red", {.gpu = gpu, .target = &target});
    REQUIRE(initial);
    // A smaller depth mip limits rendering even when the color attachment remains mip zero.
    target.depthStencilTarget.mip = 1;
    target.setClearColor(0, 0, 1, 1);
    auto smaller = GpuRaster::create("clear-depth-mip-blue", {.gpu = gpu, .target = &target});
    REQUIRE(smaller);
    gpu->submit(GpuContext::SubmitParameters("depth-mip-extent").appendWork(initial->seal()).appendWork(smaller->seal()));
    gpu->waitForIdle();
    auto image = color->readback();
    REQUIRE_FALSE(image.empty());
    auto pixels = image.plane().toRGBA8(image.data());
    REQUIRE(pixels.size() == 64);
    for (uint32_t y = 0; y < 8; ++y)
        for (uint32_t x = 0; x < 8; ++x) {
            const bool covered = x < 4 && y < 4;
            CHECK(pixels[y * 8 + x].r == (covered ? 0 : 255));
            CHECK(pixels[y * 8 + x].b == (covered ? 255 : 0));
        }
}

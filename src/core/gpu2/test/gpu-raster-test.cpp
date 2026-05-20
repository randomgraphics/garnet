#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>

#include <atomic>

using namespace GN;
using namespace GN::gpu2;

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
    auto raster = GpuRaster::create({.gpu = gpu, .target = rt});
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

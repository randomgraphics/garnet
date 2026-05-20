#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>

#include <atomic>
#include <vector>

using namespace GN;
using namespace GN::gpu2;

namespace {

// Submit a payload and spin-pump until the GPU signals completion.
void submitAndWait(const AutoRef<GpuContext> & gpu, const AutoRef<GpuPayload> & payload) {
    std::atomic<bool> done = false;
    gpu->submit(GpuContext::SubmitParameters("hazard-test").appendWork(payload).setOnComplete([&done] { done = true; }));
    while (!done) gpu->pump();
}

// Clear a single-layer texture to the given color via a valid (non-hazardous) render pass.
// Returns false if the pass could not be constructed.
bool clearToColor(const AutoRef<GpuContext> & gpu, const AutoRef<Texture> & tex, float r, float g, float b) {
    GpuResourceView view;
    view.resource = tex;
    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = view});
    rt.setClearColor(r, g, b);
    auto raster = GpuRaster::create({.gpu = gpu, .target = rt});
    if (!raster) return false;
    auto payload = raster->seal();
    if (!payload) return false;
    submitAndWait(gpu, payload);
    return true;
}

// Returns the pixels of a texture as RGBA8; empty on failure.
std::vector<gfx::img::RGBA8> readbackRGBA8(const AutoRef<Texture> & tex) {
    gfx::img::Image image = tex->readback();
    if (image.empty()) return {};
    return image.plane().toRGBA8(image.data());
}

} // namespace

// ---------------------------------------------------------------------------
// Hazard: same texture bound to two color target slots
// ---------------------------------------------------------------------------

TEST_CASE("GPU2: render target hazard — same texture on two color slots aborts render pass", "[gpu2][raster][hazard][gpu]") {
    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;
    auto tex = Texture::create("rt", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, H)});
    if (!tex) SKIP("RGBA8 texture unavailable");

    // Establish a known color: pure red.
    REQUIRE(clearToColor(gpu, tex, 1.0f, 0.0f, 0.0f));
    {
        auto pixels = readbackRGBA8(tex);
        REQUIRE(pixels.size() == W * H);
        for (const auto & px : pixels) REQUIRE(px.r == 255u);
    }

    // Hazardous render: same view on slot 0 AND slot 1 (clear to blue — must be skipped).
    GpuResourceView view;
    view.resource = tex;
    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = view});
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = view});
    rt.setClearColor(0.0f, 0.0f, 1.0f);
    auto raster = GpuRaster::create({.gpu = gpu, .target = rt});
    REQUIRE(raster);
    submitAndWait(gpu, raster->seal());

    // Pixels must still be red: the hazardous render was detected and skipped.
    auto pixels = readbackRGBA8(tex);
    REQUIRE(pixels.size() == W * H);
    for (const auto & px : pixels) {
        CHECK(px.r == 255u);
        CHECK(px.g == 0u);
        CHECK(px.b == 0u);
    }
}

// ---------------------------------------------------------------------------
// No hazard: distinct cubemap faces on separate color slots
// ---------------------------------------------------------------------------

TEST_CASE("GPU2: render target no hazard — distinct cubemap faces on separate color slots", "[gpu2][raster][hazard][gpu]") {
    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;
    // Square 6-face cubemap; each face is a distinct array layer.
    auto cubemap = Texture::create("cube", {
                                               .context    = gpu,
                                               .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, H).setFaces(6),
                                           });
    if (!cubemap) SKIP("Cubemap RGBA8 texture unavailable");

    // Build a view for a single face (one mip, one layer) at the given face index.
    auto faceView = [&](uint32_t face) {
        GpuResourceView v;
        v.resource = cubemap;
        v.setSubresourceIndex(GpuResourceView::SubresourceIndex {.face = face});
        v.setSubresourceExtent(GpuResourceView::SubresourceExtent {.numMipLevels = 1, .numArrayLayers = 1});
        return v;
    };

    // Face 0 → slot 0, face 1 → slot 1. Same texture object, non-overlapping subresource
    // ranges ([face=0,layers=1) vs [face=1,layers=1)) — this is not a hazard.
    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = faceView(0)});
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = faceView(1)});
    rt.setClearColor(0.0f, 0.0f, 1.0f);

    auto raster = GpuRaster::create({.gpu = gpu, .target = rt});
    REQUIRE(raster);

    // The render pass should execute (no hazard). Submission must complete without error.
    submitAndWait(gpu, raster->seal());
    SUCCEED("Cubemap render with distinct faces on separate color slots completed successfully");
}

// ---------------------------------------------------------------------------
// Hazard: same depth-stencil texture also used as color target
// ---------------------------------------------------------------------------

TEST_CASE("GPU2: render target hazard — same texture as both color and depth-stencil aborts render pass", "[gpu2][raster][hazard][gpu]") {
    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");

    auto caps = gpu->caps();
    if (caps.defaultDepthFormat == gfx::img::PixelFormat::UNKNOWN()) SKIP("No depth format available");

    constexpr uint32_t W = 8, H = 8;
    // A color target for the actual render (so the pass has something to draw to).
    auto colorTex =
        Texture::create("color", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, H)});
    if (!colorTex) SKIP("RGBA8 texture unavailable");

    // A depth texture whose format is used as BOTH color and depth target — clear abuse.
    auto depthTex = Texture::create("depth", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(caps.defaultDepthFormat).setDimensions(W, H)});
    if (!depthTex) SKIP("Depth texture unavailable");

    REQUIRE(clearToColor(gpu, colorTex, 1.0f, 0.0f, 0.0f)); // prime color to red

    GpuResourceView colorView, depthView;
    colorView.resource = colorTex;
    depthView.resource = depthTex;

    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = colorView});
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = depthView}); // slot 1: depth tex as color
    rt.setDepthStencilTarget(depthView);                                     // also as depth: hazard
    rt.setClearColor(0.0f, 0.0f, 1.0f);

    auto raster = GpuRaster::create({.gpu = gpu, .target = rt});
    REQUIRE(raster);
    submitAndWait(gpu, raster->seal());

    // The hazard (depth tex on two slots) should abort the pass; colorTex is still red.
    auto pixels = readbackRGBA8(colorTex);
    REQUIRE(pixels.size() == W * H);
    for (const auto & px : pixels) {
        CHECK(px.r == 255u);
        CHECK(px.g == 0u);
        CHECK(px.b == 0u);
    }
}

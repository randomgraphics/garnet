// Per-aspect render-pass state tracking on a depth-stencil texture.
// Verifies that depth-only and stencil-only views of the same DS image can coexist in
// one render pass without being flagged as a hazard, and that conflicts on the *same*
// aspect are still detected.

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>

#include "../vk-raster-state-tracker.h"
#include "../vk-texture.h"

using namespace GN;
using namespace GN::gpu2;

namespace {

TextureGpuImageState::ImageState sampledShaderRead() {
    TextureGpuImageState::ImageState s;
    s.layout = vk::ImageLayout::eShaderReadOnlyOptimal;
    s.access = vk::AccessFlagBits::eShaderRead;
    s.stages = vk::PipelineStageFlagBits::eFragmentShader;
    s.usage  = "sampled (test)";
    return s;
}

TextureGpuImageState::ImageState storageWrite() {
    TextureGpuImageState::ImageState s;
    s.layout = vk::ImageLayout::eGeneral;
    s.access = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite;
    s.stages = vk::PipelineStageFlagBits::eFragmentShader;
    s.usage  = "storage (test)";
    return s;
}

/// Build an ImageView covering subresource (mip 0, face 0) with the given format.
GpuResourceView::ImageView wholeView(gfx::img::PixelFormat fmt) {
    GpuResourceView::ImageView v;
    v.range  = {{0, 0}, {1, 1}};
    v.format = fmt;
    return v;
}

/// Given a combined depth-stencil texture format, return the rapid-image PixelFormat that selects
/// only the depth aspect of that layout. \c aspectFromView in the tracker recognises these PFs and
/// derives \c vk::ImageAspectFlagBits::eDepth.
///
/// Naming convention: R = depth, G = stencil, X = padding/zero. \c RXX_32_8_24_FLOAT keeps the
/// 32-bit float depth in red and zeros out the stencil and the 24-bit pad — i.e. depth-only of D32S8.
gfx::img::PixelFormat depthOnlyFormat(gfx::img::PixelFormat combined) {
    using PF = gfx::img::PixelFormat;
    if (combined == PF::RG_24_UNORM_8_UINT()) return PF::RX_24_8_UNORM();  // D24S8 → depth-only
    if (combined == PF::RGX_32_FLOAT_8_UINT_24()) return PF::R_32_FLOAT(); // D32S8 → depth-only
    return PF::UNKNOWN();                                                  // unknown layout
}

/// Stencil-only counterpart.
gfx::img::PixelFormat stencilOnlyFormat(gfx::img::PixelFormat) {
    using PF = gfx::img::PixelFormat;
    return PF::R_8_UINT(); // stencil only format is always R8_UINT, with the stencil byte swizzled to red. aspectFromView detects this and treats it as
                           // stencil-only.
}

} // namespace

TEST_CASE("GPU2: per-aspect tracking — depth-stencil texture with depth-only and stencil-only views in same pass", "[gpu2][raster][hazard][gpu]") {
    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");

    // Whichever combined depth-stencil format the device prefers — desktop GPUs usually pick D24S8,
    // MoltenVK on Apple silicon picks D32S8.
    const auto dsFormat = gpu->caps().defaultDepthFormat;
    if (dsFormat == gfx::img::PixelFormat::UNKNOWN()) SKIP("No depth-stencil format available on this device");

    constexpr uint32_t W = 8, H = 8;
    auto               tex = Texture::create("ds", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(dsFormat).setDimensions(W, H)});
    if (!tex) SKIP("Depth-stencil texture creation failed");

    auto * vkTex = RuntimeType::cast<TextureVulkanBase>(tex.get());
    REQUIRE(vkTex);

    const auto depthView   = wholeView(depthOnlyFormat(dsFormat));
    const auto stencilView = wholeView(stencilOnlyFormat(dsFormat));

    SECTION("depth-only sampled + stencil-only sampled => no hazard") {
        // The user's particular case: same DS texture, two views, different aspects, same pass.
        // Pre-refactor the tracker had a single layout per (mip, face) and couldn't represent
        // a depth view in one layout while stencil sat in another. Post-refactor, depth and
        // stencil are independent planes in the per-subresource map and don't collide.
        RasterStateTrackerVulkan tracker;
        REQUIRE(tracker.addAttachment(vkTex, depthView, sampledShaderRead()));
        REQUIRE(tracker.addAttachment(vkTex, stencilView, sampledShaderRead()));
    }

    SECTION("depth-write + stencil-write same subresource => no hazard (independent aspects)") {
        // Writing both aspects at the same time is legal under separate depth/stencil layouts —
        // the depth and stencil planes are tracked independently and neither write conflicts with
        // the other.
        RasterStateTrackerVulkan tracker;
        REQUIRE(tracker.addAttachment(vkTex, depthView, storageWrite()));
        REQUIRE(tracker.addAttachment(vkTex, stencilView, storageWrite()));
    }

    SECTION("two writes on the same aspect => hazard") {
        // Sanity check that we still flag a real conflict: two write bindings via the same depth
        // view must produce a write/write hazard on the depth plane.
        RasterStateTrackerVulkan tracker;
        REQUIRE(tracker.addAttachment(vkTex, depthView, storageWrite()));
        REQUIRE_FALSE(tracker.addAttachment(vkTex, depthView, storageWrite()));
    }

    SECTION("depth-write + stencil-read => no hazard; depth-write + depth-read => hazard") {
        // Cross-aspect read/write is fine; same-aspect read/write is the classic hazard.
        RasterStateTrackerVulkan tracker;
        REQUIRE(tracker.addAttachment(vkTex, depthView, storageWrite()));
        REQUIRE(tracker.addAttachment(vkTex, stencilView, sampledShaderRead()));
        REQUIRE_FALSE(tracker.addAttachment(vkTex, depthView, sampledShaderRead()));
    }
}

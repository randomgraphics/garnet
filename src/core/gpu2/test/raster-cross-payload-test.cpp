#if GN_BUILD_HAS_VULKAN

// Include vk-gpu-payload.h first — it pulls in rapid-vulkan before any other TU can do so.
#include "../vk-gpu-payload.h"
#include <catch2/catch_test_macros.hpp>
#include <atomic>

using namespace GN;
using namespace GN::gpu2;

// ---------------------------------------------------------------------------
// Test infrastructure
// ---------------------------------------------------------------------------

// A minimal payload that registers one texture with the batch tracker and emits barriers,
// with no actual rendering. Used to exercise cross-payload state-tracking logic in isolation.
struct TrackerPayload : GpuPayloadVulkan {
    GN_REGISTER_RUNTIME_TYPE(GpuPayloadVulkan);

    enum class Usage { COLOR_TARGET, SAMPLED_TEXTURE };

    AutoRef<Texture> texture;
    Usage            usage;

    TrackerPayload(const StrA & name, AutoRef<Texture> t, Usage u)
        : GpuPayloadVulkan(name), texture(std::move(t)), usage(u) {}

    void recordForVulkanSubmit(const RecordContext & ctx) override {
        if (!ctx.batchTracker || !texture) return;
        auto * vkTex = RuntimeType::cast<TextureVulkanBase>(texture.get());
        if (!vkTex) return;

        auto & tracker = *ctx.batchTracker;
        GpuResourceView view; // default range = full mip/face extent

        if (usage == Usage::COLOR_TARGET)
            tracker.addColorTarget(vkTex, view);
        else
            tracker.addSampledTexture(vkTex, view);

        tracker.emitPrePassBarriers(ctx.cmd.handle());
    }
};

static AutoRef<GpuContext> makeGpu() {
    // Enable Vulkan validation in debug builds; a wrong oldLayout triggers a break there.
    return GpuContext::create("gpu", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE, .debug = GpuContext::DebugMode::ENABLED});
}

static AutoRef<Texture> makeColorTex(const AutoRef<GpuContext> & gpu, const char * name = "tex") {
    return Texture::create(name, {
        .context    = gpu,
        .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(8, 8),
    });
}

// Submit a batch of payloads and block until the GPU fence fires.
template<typename... Payloads>
static void submitAndWait(const AutoRef<GpuContext> & gpu, Payloads &&... payloads) {
    std::atomic<bool> done = false;
    auto              sp   = GpuContext::SubmitParameters("cross-payload-test");
    (sp.appendWork(payloads), ...);
    sp.setOnComplete([&done] { done = true; });
    gpu->submit(sp);
    while (!done) gpu->pump();
}

// Return the final layout of the color plane at (mip=0, face=0) after flushToResources().
static vk::ImageLayout finalColorLayout(const AutoRef<Texture> & tex) {
    const auto * vkTex = RuntimeType::cast<TextureVulkanBase>(tex.get());
    if (!vkTex) return vk::ImageLayout::eUndefined;
    const auto * plane = vkTex->getState().get(0, 0, vk::ImageAspectFlagBits::eColor);
    return plane ? plane->layout : vk::ImageLayout::eUndefined;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

// Payload 1: texture used as color attachment → eColorAttachmentOptimal.
// Payload 2: same texture used as sampled      → eShaderReadOnlyOptimal.
// Both recorded into the same VkCommandBuffer via one submit() call.
// The tracker must emit a barrier for P2 using eColorAttachmentOptimal as oldLayout
// (not eUndefined), which is exactly what cross-payload state propagation provides.
// After flushToResources(), the texture's stored layout must be eShaderReadOnlyOptimal.
TEST_CASE("RasterStateTracker: cross-payload color-attachment to sampled transition", "[gpu2][tracker][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto tex = makeColorTex(gpu);
    if (!tex) SKIP("RGBA8 texture unavailable");

    auto p1 = AutoRef<TrackerPayload>::make("p1-color",   tex, TrackerPayload::Usage::COLOR_TARGET);
    auto p2 = AutoRef<TrackerPayload>::make("p2-sampled", tex, TrackerPayload::Usage::SAMPLED_TEXTURE);
    submitAndWait(gpu, p1, p2);

    CHECK(finalColorLayout(tex) == vk::ImageLayout::eShaderReadOnlyOptimal);
}

// Both payloads use the same layout; the tracker must not emit a barrier for P2
// (incoming is already eColorAttachmentOptimal after P1). Final layout: eColorAttachmentOptimal.
TEST_CASE("RasterStateTracker: cross-payload same layout requires no inter-payload barrier", "[gpu2][tracker][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto tex = makeColorTex(gpu);
    if (!tex) SKIP("RGBA8 texture unavailable");

    auto p1 = AutoRef<TrackerPayload>::make("p1", tex, TrackerPayload::Usage::COLOR_TARGET);
    auto p2 = AutoRef<TrackerPayload>::make("p2", tex, TrackerPayload::Usage::COLOR_TARGET);
    submitAndWait(gpu, p1, p2);

    CHECK(finalColorLayout(tex) == vk::ImageLayout::eColorAttachmentOptimal);
}

// Three-payload chain: color → sampled → color.
// Requires two layout transitions (P1→P2, P2→P3), both using the correct "from" layout.
TEST_CASE("RasterStateTracker: three-payload layout chain propagates correctly", "[gpu2][tracker][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto tex = makeColorTex(gpu);
    if (!tex) SKIP("RGBA8 texture unavailable");

    auto p1 = AutoRef<TrackerPayload>::make("p1", tex, TrackerPayload::Usage::COLOR_TARGET);
    auto p2 = AutoRef<TrackerPayload>::make("p2", tex, TrackerPayload::Usage::SAMPLED_TEXTURE);
    auto p3 = AutoRef<TrackerPayload>::make("p3", tex, TrackerPayload::Usage::COLOR_TARGET);
    submitAndWait(gpu, p1, p2, p3);

    // P3 left the texture in eColorAttachmentOptimal; the P2→P3 barrier must have used
    // eShaderReadOnlyOptimal as oldLayout (not eUndefined) — validated by debug layers.
    CHECK(finalColorLayout(tex) == vk::ImageLayout::eColorAttachmentOptimal);
}

// Two independent textures, each touched by a subset of payloads.
// Verifies that the tracker handles multiple textures per batch without cross-contamination.
TEST_CASE("RasterStateTracker: two textures, interleaved payloads, independent state", "[gpu2][tracker][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto texA = makeColorTex(gpu, "A");
    auto texB = makeColorTex(gpu, "B");
    if (!texA || !texB) SKIP("RGBA8 textures unavailable");

    // A: color → sampled (two transitions)
    // B: color only (one transition, no inter-payload change)
    auto pA1 = AutoRef<TrackerPayload>::make("A1", texA, TrackerPayload::Usage::COLOR_TARGET);
    auto pB1 = AutoRef<TrackerPayload>::make("B1", texB, TrackerPayload::Usage::COLOR_TARGET);
    auto pA2 = AutoRef<TrackerPayload>::make("A2", texA, TrackerPayload::Usage::SAMPLED_TEXTURE);
    submitAndWait(gpu, pA1, pB1, pA2);

    CHECK(finalColorLayout(texA) == vk::ImageLayout::eShaderReadOnlyOptimal);
    CHECK(finalColorLayout(texB) == vk::ImageLayout::eColorAttachmentOptimal);
}

#endif // GN_BUILD_HAS_VULKAN

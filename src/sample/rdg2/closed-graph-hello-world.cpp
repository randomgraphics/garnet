// Closed render graph hello world: render an empty frame cleared to a solid
// color, entirely through the closed-graph public API. Each frame is three
// quests — frame-begin (swapchain acquire), clear (raster pass with no draws),
// frame-end (present request) — compiled into a plan and executed with one
// gathered GPU submission.
//
// Run with "t" as the first argument for headless test mode: renders 10 frames
// without a window and verifies the final backbuffer contents via readback.

#include <garnet/GNrdg2.h>
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>

using namespace GN;
using namespace GN::rdg2;
using namespace GN::gpu2;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.rdg");

// The solid color of this hello world. Component values are byte-exact so the
// test-mode readback can compare without rounding tolerance.
static constexpr float kClearR = 0.0f, kClearG = 0.0f, kClearB = 1.0f, kClearA = 1.0f;

/// Read back the freshly rendered backbuffer and verify corner and center
/// pixels match the clear color. Must run before present() invalidates the
/// frame view, which is why test mode skips the frame-end quest on its final
/// frame.
static bool verifySolidColor(const GpuResourceView & view, uint32_t width, uint32_t height) {
    auto tex = view.texture();
    if (!tex) {
        GN_ERROR(sLogger, "verifySolidColor: could not get backbuffer texture");
        return false;
    }
    gfx::img::Image image = tex->readback();
    if (image.empty()) {
        GN_ERROR(sLogger, "verifySolidColor: readback returned empty image");
        return false;
    }
    auto         pixels   = image.plane().toRGBA8(image.data());
    const size_t expected = (size_t) width * (size_t) height;
    if (pixels.size() != expected) {
        GN_ERROR(sLogger, "verifySolidColor: unexpected pixel count {} (expected {})", pixels.size(), expected);
        return false;
    }
    const auto check = [](const char * where, const auto & p) {
        const bool ok = p.r == (uint8_t) (kClearR * 255.f) && p.g == (uint8_t) (kClearG * 255.f) && p.b == (uint8_t) (kClearB * 255.f);
        if (!ok) { GN_ERROR(sLogger, "verifySolidColor: {} pixel mismatch — got ({},{},{},{})", where, p.r, p.g, p.b, p.a); }
        return ok;
    };
    const bool cornerOk = check("corner", pixels[0]);
    const bool centerOk = check("center", pixels[(size_t) (height / 2) * width + width / 2]);
    if (!cornerOk || !centerOk) return false;
    GN_INFO(sLogger, "verifySolidColor: PASSED");
    return true;
}

/// A clear-only "renderer": reads the acquired frame relic, records a raster
/// pass with no draws (the render pass clear does the work), republishes the
/// backbuffer, and emits the sealed payload.
static QuestRef makeClearQuest(ArtifactRef backbuffer) {
    Quest::CreateParameters q;
    q.name = "clear";
    // The read side binds to frame-begin's in-plan publication — an in-plan
    // producer overrides the declared relic selector.
    q.artifactUses.append(ArtifactUse {.name = "backbuffer", .artifact = backbuffer, .access = ArtifactAccess::READ_WRITE, .root = true});
    q.execute = [backbuffer](QuestContext & ctx) -> QuestResult {
        auto acquired = ctx.read<SwapchainFrameEntity>(backbuffer);
        if (!acquired) return QuestResult::failed("no acquired frame to clear");

        RasterTarget target;
        target.colorTargets.append(RasterTarget::ColorTarget {});
        target.setClearColor(kClearR, kClearG, kClearB, kClearA);
        target.setColorTarget(0, acquired->frame.view);

        GpuRaster::CreateParameters rcp;
        rcp.gpu     = ctx.gpu();
        rcp.target  = &target;
        auto raster = GpuRaster::create("clear", rcp);
        if (!raster) return QuestResult::failed("raster creation failed");
        ctx.emit(raster->seal());

        ctx.publish(backbuffer, AutoRef<Entity>(new SwapchainFrameEntity(acquired->frame)));
        return QuestResult::succeeded();
    };
    return Quest::create(q);
}

int main(int argc, const char ** argv) {
    bool testMode = (argc > 1) && (argv[1][0] == 't');
    if (testMode) { GN_INFO(sLogger, "Running in test mode"); }

    enableCRTMemoryCheck();

    auto gpu = GpuContext::create("gpu_context", GpuContext::CreateParameters {});
    if (!gpu) return -1;

    // Create a main window and surface; test mode runs headless.
    uint32_t                     windowWidth  = 1280;
    uint32_t                     windowHeight = 720;
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        window = std::unique_ptr<win::Window>(win::createWindow(
            win::WindowCreateParameters {.caption = "Garnet 3D - Closed Graph Hello World", .clientWidth = windowWidth, .clientHeight = windowHeight}));
        if (!window) return -1;
        window->show();

        // The sample owns the surface; it is destroyed after the swapchain, before the GPU context.
        surface = window->createVulkanSurfaceHandle(gpu->getVulkanInstanceHandle());
        if (!surface) return -1;
    }

    Swapchain::CreateDesc scDesc {.gpu = gpu, .width = windowWidth, .height = windowHeight};
    if (surface) scDesc.setSurface(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) return -1;

    // The frame's only artifact: frame-begin publishes each acquired frame into it.
    auto backbuffer = Artifact::create("backbuffer");

    int totalFrames  = testMode ? 10 : 0;
    int frameCounter = 1;
    while (totalFrames == 0 || frameCounter <= totalFrames) {
        if (window && !window->runUntilNoNewEvents()) break;

        // Test mode verifies the final frame via readback, which must happen
        // before present — so that frame omits the frame-end quest.
        const bool presentThisFrame = !(testMode && frameCounter == totalFrames);

        DynaArray<QuestRef> quests;
        quests.append(createFrameBeginQuest({.swapchain = swapchain, .backbuffer = backbuffer}));
        quests.append(makeClearQuest(backbuffer));
        if (presentThisFrame) quests.append(createFrameEndQuest({.swapchain = swapchain, .backbuffer = backbuffer}));

        auto plan = Plan::compile({.quests = quests});
        if (!plan) return -1;

        auto execution = Execution::run({.plan = plan, .gpu = gpu, .name = StrA::format("frame {}", frameCounter)});
        if (!execution || execution->status() != Execution::Status::SUCCEEDED) return -1;

        if (!presentThisFrame) {
            auto relic = backbuffer->content<SwapchainFrameEntity>();
            if (!relic || !verifySolidColor(relic->frame.view, windowWidth, windowHeight)) return -1;
        }

        ++frameCounter;
    }

    // Destroy the surface after the swapchain and before the GPU context (the Vulkan instance).
    gpu->waitForIdle();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);
    return 0;
}

#include <garnet/GNgpu2.h>
#include <garnet/GNwin.h>

#include <cstdio>

using namespace GN;
using namespace GN::gpu2;
using namespace GN::win;

int main(int argc, const char ** argv) {
    // Pass 't' as the first argument to render a fixed number of headless frames.
    const bool testMode = argc > 1 && argv[1][0] == 't';

    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {});
    if (!gpu) {
        std::fprintf(stderr, "Failed to create GPU context\n");
        return -1;
    }

    uint32_t                width = 1280, height = 720;
    std::unique_ptr<Window> window;
    intptr_t                surface = 0;
    if (!testMode) {
        window.reset(createWindow(WindowCreateParameters {.caption = "GPU2 Simple Text", .clientWidth = width, .clientHeight = height}));
        if (!window) {
            std::fprintf(stderr, "Failed to create window\n");
            return -1;
        }
        window->show();
        surface = window->createVulkanSurfaceHandle(gpu->getVulkanInstanceHandle());
        if (!surface) {
            std::fprintf(stderr, "Failed to get Vulkan surface\n");
            return -1;
        }
        const auto clientSize = window->getClientSize();
        width                 = clientSize.x;
        height                = clientSize.y;
    }

    Swapchain::CreateDesc swapchainDesc;
    swapchainDesc.setGpu(gpu).setName("simple-text-swapchain").setDimensions(width, height);
    if (surface) swapchainDesc.setSurface(surface);
    auto swapchain = Swapchain::create(swapchainDesc);
    if (!swapchain) {
        std::fprintf(stderr, "Failed to create swapchain\n");
        return -1;
    }

    // Bindless Raster requires a heap, even when a pass only clears or records built-in text.
    auto heap = DescriptorHeap::create("simple-text-heap", {.gpu = gpu, .capacity = 1});
    if (!heap) {
        std::fprintf(stderr, "Failed to create descriptor heap\n");
        return -1;
    }

    RasterTarget target;
    target.colorTargets.append(RasterTarget::ColorTarget {});
    target.setClearColor(0.08f, 0.14f, 0.22f);

    const int totalFrames = testMode ? 3 : 0;
    int       frameCount  = 0;
    while (totalFrames == 0 || frameCount < totalFrames) {
        if (window && !window->runUntilNoNewEvents()) break;

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) {
            std::fprintf(stderr, "prepare() returned empty frame\n");
            return -1;
        }

        target.setColorTarget(0, frame.view);
        auto raster = Raster::create("simple-text-raster", {.gpu = gpu, .target = &target, .heap = heap});
        if (!raster) {
            std::fprintf(stderr, "Failed to create raster\n");
            return -1;
        }

        raster->recordText({.text = "GPU2 BUILT-IN ASCII TEXT", .origin = {72.0f, 72.0f}, .glyphSize = {16.0f, 24.0f}, .color = {0.92f, 0.96f, 1.0f, 1.0f}});
        raster->recordText(
            {.text = "Embedded 5x7 glyphs - no font files", .origin = {72.0f, 128.0f}, .glyphSize = {8.0f, 12.0f}, .color = {0.45f, 0.78f, 0.98f, 1.0f}});
        raster->recordText({.text = "Small", .origin = {72.0f, 190.0f}, .glyphSize = {8.0f, 12.0f}, .color = {0.55f, 0.9f, 0.68f, 1.0f}});
        raster->recordText({.text = "Medium", .origin = {72.0f, 220.0f}, .glyphSize = {16.0f, 24.0f}, .color = {0.98f, 0.78f, 0.42f, 1.0f}});
        raster->recordText({.text = "Large", .origin = {72.0f, 270.0f}, .glyphSize = {28.0f, 42.0f}, .color = {1.0f, 0.55f, 0.48f, 1.0f}});

        AutoRef<GpuPayload> payload = raster->seal();
        gpu->submit(GpuContext::SubmitParameters("simple-text-frame").appendWork(payload).waitFor(frame.ready));
        swapchain->present(*payload);
        ++frameCount;
    }

    gpu->waitForIdle();
    target.setColorTarget(0, {});
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);
    return 0;
}

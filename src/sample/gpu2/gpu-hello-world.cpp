#include <garnet/GNgpu2.h>
#include <garnet/GNwin.h>

#include <cstdio>

using namespace GN;
using namespace GN::gpu2;
using namespace GN::win;

int main(int argc, const char ** argv) {
    // Pass 't' as first arg to run headlessly for a fixed frame count (CI / smoke test).
    bool testMode = argc > 1 && argv[1][0] == 't';

    // GpuContext owns the Vulkan instance + physical-device selection. Must outlive all GPU objects.
    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {});
    if (!gpu) {
        std::fprintf(stderr, "Failed to create GPU context\n");
        return -1;
    }

    // Default resolution; may be overridden below if the OS adjusts the window for DPI.
    uint32_t W = 1280, H = 720;

    std::unique_ptr<Window> window;
    intptr_t                surface = 0; // VkSurfaceKHR (opaque handle); 0 means headless.
    if (!testMode) {
        window.reset(createWindow(WindowCreateParameters {.caption = "GPU Hello World", .clientWidth = W, .clientHeight = H}));
        if (!window) {
            std::fprintf(stderr, "Failed to create window\n");
            return -1;
        }
        window->show();
        // Vulkan surfaces are created from the native window handle, not by the swapchain itself.
        surface = window->getVulkanSurfaceHandle(gpu->getVulkanInstanceHandle());
        if (!surface) {
            std::fprintf(stderr, "Failed to get Vulkan surface\n");
            return -1;
        }

        // The OS/compositor may silently clamp or adjust the requested client area.
        auto acquiredSize = window->getClientSize();
        if (acquiredSize.x != W || acquiredSize.y != H) {
            W = acquiredSize.x;
            H = acquiredSize.y;
            std::fprintf(stdout, "Use adjusted frame buffer size: %ux%u\n", W, H);
        }
    }

    // Swapchain manages the image ring-buffer that drives present. In headless mode it renders
    // offscreen (no VkSurfaceKHR), which is enough for correctness tests.
    Swapchain::CreateDesc scDesc;
    scDesc.setGpu(gpu).setName("swapchain").setDimensions(W, H);
    if (surface) scDesc.setSurface(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) {
        std::fprintf(stderr, "Failed to create swapchain\n");
        return -1;
    }

    // totalFrames == 0 means "run until the window is closed".
    int totalFrames  = testMode ? 10 : 0;
    int frameCounter = 0;
    while (totalFrames == 0 || frameCounter < totalFrames) {
        // Drain the OS message queue; returns false when the window receives WM_CLOSE.
        if (window && !window->runUntilNoNewEvents()) break;

        // prepare() acquires the next swapchain image and returns a Token that fires when the
        // image is ready to write. An empty view means the swapchain is lost or resized.
        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) {
            std::fprintf(stderr, "prepare() returned empty frame\n");
            return -1;
        }

        // Empty raster pass: no draw calls, just clears the backbuffer to pure blue.
        // seal() is called immediately so it acts as a clear-only pass.
        auto rt = AutoRef<RasterTarget>::make("hello-world-target");
        rt->colorTargets.append(RasterTarget::ColorTarget(frame.view));
        rt->setClearColor(0.0f, 0.0f, 1.0f);
        auto                raster        = GpuRaster::create({.gpu = gpu, .target = rt});
        AutoRef<GpuPayload> rasterPayload = raster->seal();

        // Submit the clear pass, gated on frame.ready so we don't write before the image is acquired.
        gpu->submit(GpuContext::SubmitParameters("frame").appendWork(rasterPayload).waitFor(frame.ready));

        // present() schedules the image for display, gated on rasterPayload so the clear finishes
        // before the compositor takes ownership.
        swapchain->present(*rasterPayload);
        ++frameCounter;
    }

    return 0;
}

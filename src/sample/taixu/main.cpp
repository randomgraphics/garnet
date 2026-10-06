// Minimal standalone host for the Taixu world prototype.

#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

using namespace GN;
using namespace GN::gpu2;

namespace {

struct Host {
    AutoRef<GpuContext> gpu;
    std::unique_ptr<win::Window> window;
    intptr_t surface = 0;
    AutoRef<Swapchain> swapchain;

    ~Host() {
        if (gpu) gpu->waitForIdle();
        swapchain.clear();
        if (surface) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);
    }
};

bool renderHeadless(const AutoRef<GpuContext> & gpu, const char * outputPath, uint32_t width, uint32_t height) {
    auto color = Texture::create("taixu.headless-color",
                                 {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8())
                                                                       .setDimensions(width, height)
                                                                       .setLevels(1)});
    if (!color) return false;

    GpuResourceView colorView;
    colorView.resource = color;
    RasterTarget target;
    target.setColorTarget(0, colorView).setClearColor(0.35f, 0.58f, 0.78f, 1.0f);

    auto raster = GpuRaster::create("taixu.headless-frame", {.gpu = gpu, .target = &target});
    if (!raster) return false;
    auto frame = raster->seal();
    if (!frame) return false;

    GpuContext::SubmitParameters submission("taixu.headless");
    submission.appendWork(frame);
    gpu->submit(submission);

    // Texture readback waits for GPU completion; this path is intentionally for verification.
    const auto image = color->readback();
    if (image.empty()) return false;

    image.save(std::string(outputPath));
    return true;
}

} // namespace

int main(int argc, const char ** argv) {
    constexpr uint32_t width = 1280;
    constexpr uint32_t height = 720;

    bool headless = false;
    const char * outputPath = "taixu-headless.png";
    if (argc > 1 && std::string(argv[1]) == "--headless") {
        headless = true;
        if (argc > 2) outputPath = argv[2];
    } else if (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
        std::puts("Usage: GNsample-taixu [--headless [output.png]]");
        return 0;
    }

    if (headless) {
#if GN_BUILD_HAS_MSW
        _putenv_s("DISPLAY", "");
#else
        // Vulkan platform probing may try to connect to a stale X11 DISPLAY even for offscreen work.
        unsetenv("DISPLAY");
#endif
    }

    Host host;
    host.gpu = GpuContext::create("taixu-prototype", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!host.gpu) return 1;

    if (headless) return renderHeadless(host.gpu, outputPath, width, height) ? 0 : 1;

    host.window.reset(win::createWindow({.caption = "Taixu", .clientWidth = width, .clientHeight = height}));
    if (!host.window) return 1;
    host.window->show();

    host.surface = host.window->createVulkanSurfaceHandle(host.gpu->getVulkanInstanceHandle());
    if (!host.surface) return 1;

    Swapchain::CreateDesc swapchainDesc {.gpu = host.gpu, .width = width, .height = height};
    swapchainDesc.setSurface(host.surface);
    host.swapchain = Swapchain::create(swapchainDesc);
    if (!host.swapchain) return 1;

    while (host.window->runUntilNoNewEvents()) {
        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;

        RasterTarget target;
        target.setColorTarget(0, acquired.view).setClearColor(0.35f, 0.58f, 0.78f, 1.0f);
        auto raster = GpuRaster::create("taixu.clear-frame", {.gpu = host.gpu, .target = &target});
        if (!raster) return 1;
        auto frame = raster->seal();
        if (!frame) return 1;

        GpuContext::SubmitParameters submission("taixu.frame");
        submission.appendWork(frame).waitFor(acquired.ready);
        host.gpu->submit(submission);
        host.swapchain->present(*frame);
    }

    return 0;
}

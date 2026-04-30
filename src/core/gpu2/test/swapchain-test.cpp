#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>
#include <garnet/GNwin.h>

#include <memory>

using namespace GN;
using namespace GN::gpu2;
using namespace GN::win;

TEST_CASE("GPU2: Vulkan swapchain prepare and present", "[gpu2][swapchain][gpu][vulkan]") {
    auto gpuContext = GpuContext::create("gpu_context", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpuContext) SKIP("No GPU context available");

    constexpr uint32_t windowWidth  = 640;
    constexpr uint32_t windowHeight = 480;

    WindowCreateParameters wcp;
    wcp.caption      = "GNtest-gpu2-swapchain";
    wcp.clientWidth  = windowWidth;
    wcp.clientHeight = windowHeight;
    std::unique_ptr<Window> window(createWindow(wcp));
    if (!window) SKIP("Window creation failed (no display?)");

    window->show();

    intptr_t surface = window->getVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
    if (!surface) SKIP("No Vulkan surface for window");

    Swapchain::CreateDesc scDesc;
    scDesc.setGpu(gpuContext).setName("test_swapchain").setWindow(surface).setDimensions(windowWidth, windowHeight);
    AutoRef<Swapchain> swapchain = Swapchain::create(scDesc);
    if (!swapchain) SKIP("Swapchain creation failed (Vulkan surface or device?)");

    constexpr int kFrames = 4;
    for (int i = 0; i < kFrames; ++i) {
        (void) window->runUntilNoNewEvents();

        Swapchain::Frame frame = swapchain->prepare();
        REQUIRE_FALSE(frame.view.empty());

        // Empty frame: pass prepare()'s ready signal directly to present.
        swapchain->present(*frame.ready);
    }
}

TEST_CASE("GPU2: headless Vulkan swapchain prepare and present", "[gpu2][swapchain][gpu][vulkan][headless]") {
    auto gpuContext = GpuContext::create("gpu_context_headless", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpuContext) SKIP("No GPU context available");

    constexpr uint32_t w = 64, h = 64;

    Swapchain::CreateDesc scDesc;
    scDesc.setGpu(gpuContext).setName("test_swapchain_headless").setDimensions(w, h);
    // \c window stays zero: rapid-vulkan builds a headless swapchain (no VkSurfaceKHR).
    AutoRef<Swapchain> swapchain = Swapchain::create(scDesc);
    if (!swapchain) SKIP("Headless swapchain creation failed");

    constexpr int kFrames = 4;
    for (int i = 0; i < kFrames; ++i) {
        Swapchain::Frame frame = swapchain->prepare();
        REQUIRE_FALSE(frame.view.empty());

        // Empty frame: pass prepare()'s ready signal directly to present.
        swapchain->present(*frame.ready);
    }
}

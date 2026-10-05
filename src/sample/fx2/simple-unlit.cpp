#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

using namespace GN;
using namespace GN::gpu2;

namespace {
// Drain before destroying the native surface, including early failure returns.
struct Host {
    AutoRef<GpuContext>          gpu;
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    AutoRef<Swapchain>           swapchain;
    ~Host() {
        if (gpu) gpu->waitForIdle();
        swapchain.clear();
        if (surface) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);
    }
};
} // namespace

int main(int argc, const char * argv[]) {
    const bool headless = argc > 1 && argv[1][0] == 't';
    Host       host;
    host.gpu = GpuContext::create("unlit-kernel-sample", {});
    if (!host.gpu) return 1;
    constexpr uint32_t width = 640, height = 480;
    if (!headless) {
        host.window.reset(win::createWindow({.caption = "FX2 typed unlit kernel", .clientWidth = width, .clientHeight = height}));
        if (!host.window) return 1;
        host.window->show();
        host.surface = host.window->createVulkanSurfaceHandle(host.gpu->getVulkanInstanceHandle());
        if (!host.surface) return 1;
    }
    Swapchain::CreateDesc swapchain {.gpu = host.gpu, .width = width, .height = height};
    swapchain.setSurface(host.surface);
    host.swapchain = Swapchain::create(swapchain);
    auto unlit     = fx2::UnlitKernel::create(host.gpu);
    auto constants = fx2::SharedShaderConstants::create({.gpu = host.gpu});
    if (!host.swapchain || !unlit || !constants) return 1;
    constants->set0.camera.cameraPosition    = {0, 0, 3};
    constants->set0.camera.viewWidthInPixel  = width;
    constants->set0.camera.viewHeightInPixel = height;
    constants->set0.camera.aspectRatio       = float(width) / height;

    // Plain GPU data: the effect needs only positions, not a mesh/asset object.
    const float positions[] = {-0.6f, -0.7f, 0, 0.6f, -0.7f, 0, 0, 0.7f, 0};
    auto        vertices    = Buffer::create("unlit.positions", {.context = host.gpu, .size = sizeof(positions)});
    auto        upload      = GpuCnC::create({.gpu = host.gpu});
    if (!vertices || !upload) return 1;
    upload->recordUploadBuffer(vertices, 0, {reinterpret_cast<const uint8_t *>(positions), sizeof(positions)});
    auto initialization = upload->seal();
    if (!initialization) return 1;
    fx2::UnlitKernel::Inputs input;
    input.geometry.vertices.append({.buffer = vertices, .offset = 0, .stride = 3 * sizeof(float)});
    input.geometry.format.attributes.append({.location = 0, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    input.geometry.vertexCount = 3;

    for (int frame = 0; !headless || frame < 3; ++frame) {
        if (host.window && !host.window->runUntilNoNewEvents()) break;
        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;
        RasterTarget target;
        target.setColorTarget(0, acquired.view).setClearColor(0.02f, 0.03f, 0.05f, 1);
        target.states.cullMode = RasterState::CULL_NONE;
        auto raster            = GpuRaster::create("unlit.two-invocations", {.gpu = host.gpu, .target = &target});
        if (!raster) return 1;
        constants->set0.frameConstants.frameCounter = frame;
        auto shared                                 = constants->takeSnapshot();
        input.color                                 = {1, 0.2f, 0.05f, 1};
        input.worldFromObject                       = glm::translate(glm::mat4(1), glm::vec3(-0.7f, 0, 0));
        if (!unlit->record(*raster, shared.set0Resources, input)) return 1;
        input.color           = {0.1f, 0.7f, 1, 1};
        input.worldFromObject = glm::translate(glm::mat4(1), glm::vec3(0.7f, 0, 0));
        if (!unlit->record(*raster, shared.set0Resources, input)) return 1;
        auto draws = raster->seal();
        if (!draws) return 1;
        GpuContext::SubmitParameters submission("unlit.frame");
        if (initialization) submission.appendWork(initialization);
        for (const auto & work : shared.set0Payloads) submission.appendWork(work);
        submission.appendWork(draws).waitFor(acquired.ready);
        host.gpu->submit(submission);
        initialization.clear();
        host.swapchain->present(*draws);
    }
    return 0;
}

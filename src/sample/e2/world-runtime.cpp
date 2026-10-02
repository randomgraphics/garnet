#include <garnet/GNengine2.h>
#include <garnet/GNwin.h>
#include <glm/gtc/matrix_transform.hpp>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace GN;
using namespace GN::e2;
using namespace GN::gpu2;

namespace {
// Drain before releasing the presentation surface, including early failure exits.
struct Presentation {
    AutoRef<GpuContext>          gpu;
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    AutoRef<Swapchain>           swapchain;
    ~Presentation() {
        if (gpu) gpu->waitForIdle();
        swapchain.clear();
        if (surface) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);
    }
};

} // namespace

int main(int argc, const char * argv[]) {
    const bool headless = argc > 1 && argv[1][0] == 't';
    Universe   universe;
    auto       world = World::create(universe, "falling-bodies");
    const auto scale = PhysicalScale::MICROMETER();
    if (!world || !registerDynamicsFacets(*world)) {
        std::cerr << "Failed to register world capabilities\n";
        return 1;
    }
    if (!world->addLaw(createDynamicsLaw({.scale = scale})) || !world->addLaw(createLifetimeLaw(universe, {.scale = scale}))) {
        std::cerr << "Failed to register laws\n";
        return 1;
    }
    auto ground = createGroundMold(universe, scale);
    if (!ground || !world->createForm(*ground) || !world->initializeState(LifetimeState {})) {
        std::cerr << "Failed to create initial world state\n";
        return 1;
    }
    auto initial = world->primeSnapshot();

    Presentation host;
    host.gpu = GpuContext::create("world-runtime", {});
    if (!host.gpu) return 1;
    constexpr uint32_t width = 960, height = 640;
    if (!headless) {
        host.window.reset(win::createWindow({.caption = "E2 Prime / Slate / Law", .clientWidth = width, .clientHeight = height}));
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
    auto depth     = Texture::create(
        "world.depth", {.context = host.gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(width, height)});
    if (!host.swapchain || !unlit || !constants || !depth) return 1;
    GpuResourceView depthView;
    depthView.resource = depth;
    const glm::vec3 eye {12, 11, 16};
    constants->set0.camera.cameraPosition    = eye;
    constants->set0.camera.cameraOrientation = glm::quat_cast(glm::mat3(glm::inverse(glm::lookAtRH(eye, glm::vec3(0, 2, 0), glm::vec3(0, 1, 0)))));
    constants->set0.camera.viewWidthInPixel  = width;
    constants->set0.camera.viewHeightInPixel = height;
    constants->set0.camera.aspectRatio       = float(width) / height;

    // Unit cube triangles; simulation shapes and this presentation geometry are independent.
    const glm::vec3 corners[] = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};
    const unsigned  indices[] = {0, 2, 1, 0, 3, 2, 4, 5, 6, 4, 6, 7, 0, 1, 5, 0, 5, 4, 3, 7, 6, 3, 6, 2, 0, 4, 7, 0, 7, 3, 1, 2, 6, 1, 6, 5};
    glm::vec3       vertices[36];
    for (size_t i = 0; i < 36; ++i) vertices[i] = corners[indices[i]];
    auto buffer = Buffer::create("world.box", {.context = host.gpu, .size = sizeof(vertices)});
    auto upload = GpuCnC::create({.gpu = host.gpu});
    if (!buffer || !upload) return 1;
    upload->recordUploadBuffer(buffer, 0, {reinterpret_cast<const uint8_t *>(vertices), sizeof(vertices)});
    auto initialization = upload->seal();
    if (!initialization) return 1;
    fx2::UnlitKernel::Inputs draw;
    draw.geometry.vertices.append({.buffer = buffer, .offset = 0, .stride = sizeof(glm::vec3)});
    draw.geometry.format.attributes.append({.location = 0, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    draw.geometry.vertexCount = 36;
    draw.states.cullMode      = RasterState::CULL_NONE;
    draw.states.depthState    = RasterState::DepthState {RasterState::Compare::LESS, true};

    std::atomic<bool> done             = false;
    std::atomic<bool> simulationFailed = false;
    // jthread joins before World/Universe destruction, even if rendering exits early.
    std::jthread simulation([&](std::stop_token stop) {
        auto deadline = std::chrono::steady_clock::now();
        for (uint64_t tick = 0; !stop.stop_requested() && (!headless || tick < 600); ++tick) {
            if (!world->tick(UnitOfTime {10'000'000})) {
                simulationFailed = true;
                break;
            }
            if (!headless) {
                deadline += std::chrono::milliseconds(10);
                std::this_thread::sleep_until(deadline);
            }
        }
        done = true;
    });
    uint64_t     cursor = 0, eventCount = 0, lastTick = 0;
    int          frames = 0;
    for (;;) {
        if (host.window && !host.window->runUntilNoNewEvents()) break;
        auto prime = world->primeSnapshot();
        if (prime->tick() < lastTick || initial->tick() != 0 || initial->query<>().size() != 1) return 1;
        lastTick = prime->tick();
        if (prime->query<LifetimeFacet>().size() > 1000) return 1;
        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;
        RasterTarget target;
        target.setColorTarget(0, acquired.view).setDepthStencilTarget(depthView).setClearColor(0.025f, 0.035f, 0.055f, 1).setClearDepth(1);
        auto raster = GpuRaster::create("world.presentation", {.gpu = host.gpu, .target = &target});
        if (!raster) return 1;
        constants->set0.frameConstants.frameCounter = frames;
        auto shared                                 = constants->takeSnapshot();
        for (auto id : prime->query<TransformFacet, VisualFacet>()) {
            WorldTransform transform;
            if (!resolveWorldTransform(*prime, id, transform)) return 1;
            const auto & visual  = *prime->get<VisualFacet>(id);
            draw.worldFromObject = glm::translate(glm::mat4(1), glm::vec3(positionToMeters(transform.position, scale))) *
                                   glm::mat4_cast(transform.orientation) * glm::scale(glm::mat4(1), visual.halfExtent);
            draw.color           = visual.color;
            if (!unlit->record(*raster, shared.set0Resources, draw)) return 1;
        }
        auto draws = raster->seal();
        if (!draws) return 1;
        GpuContext::SubmitParameters submission("world.presentation");
        if (initialization) submission.appendWork(initialization);
        for (const auto & payload : shared.set0Payloads) submission.appendWork(payload);
        submission.appendWork(draws).waitFor(acquired.ready);
        host.gpu->submit(submission);
        initialization.clear();
        host.swapchain->present(*draws);
        eventCount += world->events(cursor).size();
        ++frames;
        if (headless && done && lastTick == world->primeSnapshot()->tick()) break;
        // Bound CPU/GPU recording lead while simulation runs independently.
        host.gpu->waitForIdle();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    simulation.request_stop();
    simulation.join();
    if (simulationFailed) {
        std::cerr << world->error().data() << '\n';
        return 1;
    }
    std::cout << "world-runtime: ticks=" << lastTick << " frames=" << frames << " observed-events=" << eventCount << '\n';
    return headless && (lastTick != 600 || !eventCount) ? 1 : 0;
}

#include <garnet/GNengine2.h>
#include <glm/gtc/matrix_transform.hpp>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string_view>
#include <thread>

using namespace GN;
using namespace GN::e2;
using namespace GN::e2::basis;

int main(int argc, const char * argv[]) {
    bool         headless = false, testMode = false;
    const char * snapshotPath = nullptr;
    for (int i = 1; i < argc; ++i) {
        const std::string_view argument(argv[i]);
        if (argument == "t" || argument == "--headless") {
            headless = testMode = true;
        } else if (argument == "--window-test") {
            testMode = true;
        } else if (argument == "--snapshot" && i + 1 < argc) {
            snapshotPath = argv[++i];
        } else {
            std::cerr << "Usage: GNsample-e2-world-runtime [t|--headless|--window-test] [--snapshot output.png]\n";
            return 1;
        }
    }
    if (snapshotPath && !headless) {
        std::cerr << "--snapshot requires --headless\n";
        return 1;
    }
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

    constexpr uint32_t width = 960, height = 640;
    Ref<Platform>      os;
    if (!headless) {
        os = Platform::create({.universe = universe, .caption = "E2 Prime / Slate / Law", .width = width, .height = height});
        if (!os) return 1;
    }

    auto visual = Visual::create({.universe = universe, .platform = os});
    if (!visual) return 1;
    auto box = visual->assets().findMesh(Assets::MESH_BOX);
    if (!box) {
        std::cerr << "The built-in white-model box is unavailable\n";
        return 1;
    }

    const glm::vec3 eye {12, 11, 16};
    Visual::Camera  camera;
    camera.position     = positionFromMeters(eye, scale);
    camera.orientation  = glm::quat_cast(glm::mat3(glm::inverse(glm::lookAtRH(eye, glm::vec3(0, 2, 0), glm::vec3(0, 1, 0)))));
    camera.nearPlane    = positionFromMeters(glm::dvec3(0.1), scale).x;
    camera.farPlane     = positionFromMeters(glm::dvec3(1000), scale).x;
    camera.fovYInDegree = 60.0f;
    camera.exposure     = 0.002f;

    std::atomic<bool> done             = false;
    std::atomic<bool> simulationFailed = false;

    // Declared after World and Visual so every early return joins before their destruction.
    std::jthread simulation([&](std::stop_token stop) {
        auto deadline = std::chrono::steady_clock::now();
        for (uint64_t tick = 0; !stop.stop_requested() && (!testMode || tick < 600); ++tick) {
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

    uint64_t cursor = 0, eventCount = 0, lastTick = 0;
    int      frames = 0;
    for (;;) {
        if (os && !os->processEvents()) break;
        auto prime = world->primeSnapshot();
        if (prime->tick() < lastTick || initial->tick() != 0 || initial->query<>().size() != 1) return 1;
        lastTick = prime->tick();
        if (prime->query<LifetimeFacet>().size() > 1000) return 1;

        auto tableau       = extractTableau(*prime, camera, scale);
        tableau.clearColor = {{0.025f, 0.035f, 0.055f, 1.0f}};
        tableau.clearDepth = 1.0f;
        for (auto & object : tableau.objects) {
            if (!object.meshId) object.meshId = box->id;
        }
        visual->renderFrame(tableau);

        eventCount += world->events(cursor).size();
        ++frames;
        if (testMode && done && lastTick == world->primeSnapshot()->tick()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    simulation.request_stop();
    simulation.join();
    if (simulationFailed) {
        std::cerr << world->error().data() << '\n';
        return 1;
    }
    if (headless) {
        auto image = visual->readbackFrame();
        if (image.empty()) {
            std::cerr << "Visual produced no headless frame\n";
            return 1;
        }
        // Inspect display-encoded bytes; rapid-image does not convert SRGB planes.
        auto plane             = image.plane();
        plane.format           = gfx::img::PixelFormat::RGBA8();
        const auto pixels      = plane.toRGBA8(image.data());
        size_t     whitePixels = 0;
        for (const auto & pixel : pixels) {
            if (pixel.r > 30 && pixel.r == pixel.g && pixel.g == pixel.b) ++whitePixels;
        }
        if (whitePixels < 1000) {
            std::cerr << "Final frame does not contain visible white-model geometry\n";
            return 1;
        }
        if (snapshotPath) {
            gfx::img::Image output(gfx::img::ImageDesc {}.set2D(gfx::img::PixelFormat::RGBA8(), image.width(), image.height()), image.data(), image.size());
            output.save(snapshotPath);
        }
        std::cout << "headless: white-model-pixels=" << whitePixels << '\n';
    }
    std::cout << "world-runtime: ticks=" << lastTick << " frames=" << frames << " observed-events=" << eventCount << '\n';
    return testMode && (lastTick != 600 || !eventCount) ? 1 : 0;
}

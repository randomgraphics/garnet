// simple-world.cpp — engine2 sample demonstrating the world / visual-moment workflow.
//
// It builds a Simple world containing one spinning box and one point light, starts the
// world running on its own background simulation thread, then — on the main thread — repeatedly
// captures a self-contained visual moment and hands it to the official VisualDomain to render.
// Simulation cadence and rendering cadence are fully independent; the only thing shared
// between them is the snapshot.
//
// Usage: simple-world [t]
//   t — headless test mode: render a fixed number of frames with no window, then exit.

#include <garnet/GNengine2.h>

#include <glm/gtc/quaternion.hpp>

#include <chrono>
#include <thread>

using namespace GN;
using namespace GN::e2;

namespace {
// A concrete universe; the base universe has a protected constructor.
struct SampleUniverse : Universe {};

UnitOfLength m(int64_t meters) { return UnitOfLength(meters); }
} // namespace

int main(int argc, const char ** argv) {
    bool testMode = argc > 1 && argv[1][0] == 't';

    SampleUniverse universe;

    // Operating-system domain (window + event pump). Skipped in headless test mode.
    Ref<OperatingDomain> os;
    if (!testMode) {
        os = OperatingDomain::create({.universe = universe, .caption = "Garnet engine2 - Simple World", .width = 1280, .height = 720});
        if (!os) return -1;
    }

    // Official visual domain (GPU rendering). Renders headlessly when there is no OS domain.
    auto visual = VisualDomain::create({.universe = universe, .os = os});
    if (!visual) return -1;

    // Build the simple world: a 1m box at the origin and a point light off to one side.
    auto world = Simple::createWorld(universe);
    auto box   = Simple::createBox(universe, WorldPosition(m(0), m(0), m(0)), Vector3<UnitOfLength>(m(1), m(1), m(1)));
    auto light = Simple::createPointLight(universe, WorldPosition(m(3), m(4), m(3)), IntensityRGB {1.f, 0.95f, 0.85f, Candela {80.f}});

    Ref<Form> forms[] = {box, light};
    world->populate({forms, 2});

    // A camera looking at the origin from +Z. The camera takes its universe from the
    // domain; its observer description is set directly on the mutable `desc` member.
    auto camera = Camera::create({.domain = visual});
    if (!camera) return -1;
    camera->desc.position     = WorldPosition(m(0), m(0), m(4));
    camera->desc.orientation  = glm::quat(1.f, 0.f, 0.f, 0.f); // identity -> looking down -Z
    camera->desc.nearPlane    = m(1);
    camera->desc.farPlane     = m(100);
    camera->desc.fovYInDegree = 60.f;
    Ref<Camera> cameras[] = {camera};

    // Start the world evolving on its own thread.
    world->run();

    const int totalFrames = testMode ? 60 : 0; // 0 = run until the window closes
    for (int frame = 0; totalFrames == 0 || frame < totalFrames; ++frame) {
        if (os && !os->processEvents()) break;

        VisualMoment::CaptureParameters cp;
        cp.domain  = visual;
        cp.cameras = {cameras, 1};

        auto moment = world->captureVisualMoment(cp);
        visual->render(moment);

        if (testMode) std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    return 0;
}

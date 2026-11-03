#include <garnet/GNengine2.h>
#include <glm/gtc/matrix_transform.hpp>
#include <atomic>
#include <bit>
#include <charconv>
#include <chrono>
#include <iostream>
#include <string_view>
#include <thread>

using namespace GN;
using namespace GN::e2;
using namespace GN::e2::basis;

namespace {
void hashWord(uint64_t & hash, uint64_t value) {
    for (unsigned int i = 0; i < 8; ++i) {
        hash ^= (value >> (i * 8)) & 0xff;
        hash *= 1099511628211ULL;
    }
}

uint64_t worldStateFingerprint(const PrimeView & prime) {
    uint64_t hash = 14695981039346656037ULL;
    hashWord(hash, prime.tick());
    for (auto id : prime.query<TransformFacet>()) {
        const auto * value = prime.get<TransformFacet>(id);
        hashWord(hash, id);
        hashWord(hash, value->parent);
        for (const auto & coordinate : {value->position.x, value->position.y, value->position.z}) {
            hashWord(hash, static_cast<uint64_t>(coordinate.hi()));
            hashWord(hash, coordinate.lo());
        }
        for (float component :
             {value->orientation.w, value->orientation.x, value->orientation.y, value->orientation.z, value->scale.x, value->scale.y, value->scale.z})
            hashWord(hash, std::bit_cast<uint32_t>(component));
        hashWord(hash, value->affineOverride ? 1 : 0);
        if (value->affineOverride)
            for (int column = 0; column < 3; ++column)
                for (int row = 0; row < 3; ++row) hashWord(hash, std::bit_cast<uint64_t>(value->affineLinear[column][row]));
    }
    for (auto id : prime.query<MotionFacet>()) {
        const auto * value = prime.get<MotionFacet>(id);
        hashWord(hash, id);
        for (double component : {value->linearVelocity.x, value->linearVelocity.y, value->linearVelocity.z, value->angularVelocity.x, value->angularVelocity.y,
                                 value->angularVelocity.z})
            hashWord(hash, std::bit_cast<uint64_t>(component));
    }
    for (auto id : prime.query<PhysicsFacet>()) {
        const auto * value = prime.get<PhysicsFacet>(id);
        hashWord(hash, id);
        hashWord(hash, static_cast<uint64_t>(value->touching.size()));
        for (auto other : value->touching) hashWord(hash, other);
    }
    if (const auto * lifetime = prime.state<LifetimeState>()) {
        hashWord(hash, lifetime->random);
        hashWord(hash, std::bit_cast<uint64_t>(lifetime->elapsed));
    }
    return hash;
}
} // namespace

int main(int argc, const char * argv[]) {
    bool         headless = false, testMode = false, affineProbe = false;
    const char * snapshotPath  = nullptr;
    uint32_t     workerThreads = 1;
    for (int i = 1; i < argc; ++i) {
        const std::string_view argument(argv[i]);
        if (argument == "t" || argument == "--headless") {
            headless = testMode = true;
        } else if (argument == "--window-test") {
            testMode = true;
        } else if (argument == "--affine-probe") {
            affineProbe = true;
        } else if (argument == "--snapshot" && i + 1 < argc) {
            snapshotPath = argv[++i];
        } else if (argument == "--workers" && i + 1 < argc) {
            const std::string_view count(argv[++i]);
            const auto             parsed = std::from_chars(count.data(), count.data() + count.size(), workerThreads);
            if (parsed.ec != std::errc {} || parsed.ptr != count.data() + count.size() || workerThreads == 0 || workerThreads > 64) {
                std::cerr << "--workers must be an integer in [1, 64]\n";
                return 1;
            }
        } else {
            std::cerr << "Usage: GNsample-e2-world-runtime [t|--headless|--window-test] [--affine-probe] [--snapshot output.png] [--workers count]\n";
            return 1;
        }
    }
    if (snapshotPath && !headless) {
        std::cerr << "--snapshot requires --headless\n";
        return 1;
    }
    Universe         universe;
    auto             world     = World::create(universe, "falling-bodies");
    const auto       scale     = PhysicalScale::MICROMETER();
    constexpr size_t MAX_CUBES = 256;
    if (!world || !registerDynamicsFacets(*world) || !registerPhysicsFacet(*world)) {
        std::cerr << "Failed to register world capabilities\n";
        return 1;
    }
    if (!world->addLaw(createGNfizLaw({.scale = scale, .workerThreads = workerThreads})) ||
        !world->addLaw(createLifetimeLaw(universe, {.scale = scale, .maximumPopulation = MAX_CUBES, .includeGNfizCollider = true}))) {
        std::cerr << "Failed to register laws\n";
        return 1;
    }
    const WorldVector3 worldOrigin {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()};
    auto               ground = createGroundMold(universe, scale, worldOrigin, true);
    if (!ground || !world->createForm(*ground) || !world->initializeState(LifetimeState {})) {
        std::cerr << "Failed to create initial world state\n";
        return 1;
    }
    FormId   affineProbeId = 0;
    Rotation initialProbeOrientation {1, 0, 0, 0};
    if (affineProbe) {
        TransformFacet::Value parentTransform;
        parentTransform.position = positionFromMeters({0, 3, 0}, scale, worldOrigin);
        parentTransform.scale    = {1.4f, 0.8f, 1.1f};
        VisualFacet::Value parentVisual;
        parentVisual.scale = glm::vec3(0.65f);
        DynaArray<FacetBinding> parentFacets {
            {Ref<const Facet>(new TransformFacet), parentTransform.clone()},
            {Ref<const Facet>(new VisualFacet), parentVisual.clone()},
        };
        auto parentMold = Mold::create(universe, "affine-probe-parent", std::move(parentFacets));
        if (!parentMold) {
            std::cerr << "Failed to create affine probe parent\n";
            return 1;
        }
        const FormId          parentId = world->createForm(*parentMold);
        TransformFacet::Value childTransform;
        childTransform.parent      = parentId;
        childTransform.position    = positionFromMeters({0, 1, 0}, scale, worldOrigin);
        childTransform.orientation = glm::angleAxis(0.45f, glm::vec3(0, 0, 1));
        childTransform.scale       = glm::vec3(0.8f);
        initialProbeOrientation    = glm::normalize(parentTransform.orientation * childTransform.orientation);
        MotionFacet::Value probeMotion;
        probeMotion.angularVelocity = {0.1, 0.2, 0.05};
        VisualFacet::Value probeVisual;
        probeVisual.scale = glm::vec3(0.35f);
        PhysicsFacet::Value probePhysics;
        probePhysics.collider   = fiz::Hull::createBox({0.25f, 0.25f, 0.25f});
        probePhysics.motionType = fiz::MotionType::DYNAMIC;
        probePhysics.layer      = fiz::CollisionLayer::MOVING;
        DynaArray<FacetBinding> probeFacets {
            {Ref<const Facet>(new TransformFacet), childTransform.clone()},
            {Ref<const Facet>(new MotionFacet), probeMotion.clone()},
            {Ref<const Facet>(new VisualFacet), probeVisual.clone()},
            {Ref<const Facet>(new PhysicsFacet), probePhysics.clone()},
        };
        auto probeMold = Mold::create(universe, "affine-dynamic-probe", std::move(probeFacets));
        if (!parentId || !probeMold || !(affineProbeId = world->createForm(*probeMold))) {
            std::cerr << "Failed to create affine collider probe\n";
            return 1;
        }
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
            auto prime = world->primeSnapshot();
            auto cubes = prime->query<LifetimeFacet>();
            for (FormId id : cubes) {
                WorldTransform transform;
                if (resolveWorldTransform(*prime, id, transform) && positionToMeters(transform.position, scale, worldOrigin).y < -15.0)
                    world->submit(DestroyFormIntent {id});
            }
            if (cubes.size() >= MAX_CUBES) {
                const size_t toDelete = cubes.size() - MAX_CUBES + 1;
                for (size_t i = 0; i < toDelete && i < cubes.size(); ++i) { world->submit(DestroyFormIntent {cubes[i]}); }
            }
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

    uint64_t cursor = 0, eventCount = 0, eventFingerprint = 14695981039346656037ULL, lastTick = 0;
    int      frames = 0;
    for (;;) {
        if (os && !os->processEvents()) break;
        auto prime = world->primeSnapshot();
        if (prime->tick() < lastTick || initial->tick() != 0 || initial->query<>().size() != (affineProbe ? 3 : 1)) return 1;
        lastTick = prime->tick();
        if (prime->query<LifetimeFacet>().size() > MAX_CUBES) return 1;

        auto tableau       = extractTableau(*prime, camera, scale);
        tableau.clearColor = {{0.025f, 0.035f, 0.055f, 1.0f}};
        tableau.clearDepth = 1.0f;
        for (auto & object : tableau.objects) {
            if (!object.meshId) object.meshId = box->id;
        }
        visual->renderFrame(tableau);

        for (const auto & published : world->events(cursor)) {
            ++eventCount;
            if (auto collision = RuntimeType::cast<const CollisionEvent>(published.payload.get())) {
                hashWord(eventFingerprint, collision->a);
                hashWord(eventFingerprint, collision->b);
                for (const auto & coordinate : {collision->point.x, collision->point.y, collision->point.z}) {
                    hashWord(eventFingerprint, static_cast<uint64_t>(coordinate.hi()));
                    hashWord(eventFingerprint, coordinate.lo());
                }
            } else {
                hashWord(eventFingerprint, published.payload->typeInfo().id);
            }
        }
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
    if (affineProbe) {
        const auto     finalPrime = world->primeSnapshot();
        const auto *   value      = finalPrime->get<TransformFacet>(affineProbeId);
        const auto *   physics    = finalPrime->get<PhysicsFacet>(affineProbeId);
        WorldTransform finalPose;
        const bool     resolved       = value && resolveWorldTransform(*finalPrime, affineProbeId, finalPose);
        const float    orientationDot = resolved ? std::abs(glm::dot(initialProbeOrientation, finalPose.orientation)) : 1.f;
        if (!resolved || !value->affineOverride || !physics || physics->touching.empty() || orientationDot > 0.9999f) {
            std::cerr << "Affine collider probe failed: form-present=" << (value != nullptr) << " affine-override=" << (value ? value->affineOverride : false)
                      << " world-pose-resolved=" << resolved << " touching=" << (physics ? physics->touching.size() : 0)
                      << " initial-final-orientation-dot=" << orientationDot << '\n';
            return 1;
        }
        std::cout << "affine-probe: non-uniform parent, rotated child and dynamic angular motion passed\n";
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
    std::cout << "world-runtime: ticks=" << lastTick << " frames=" << frames << " observed-events=" << eventCount << " worker-threads=" << workerThreads
              << " prime-fingerprint=" << std::hex << worldStateFingerprint(*world->primeSnapshot()) << " event-fingerprint=" << eventFingerprint << std::dec
              << '\n';
    return testMode && (lastTick != 600 || !eventCount) ? 1 : 0;
}

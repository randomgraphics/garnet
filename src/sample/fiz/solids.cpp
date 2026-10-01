// solids.cpp — Standalone visual sample demonstrating GNfiz solid rigid body simulation
// with high-throughput multi-threaded solving across hundreds/thousands of colliding objects.
//
// Usage: GNsample-fiz-solids [options] [frames]
//   test | t           — headless test mode: runs simulation for 60 steps, asserts physics motion, and exits cleanly.
//   --vsync | -v       — enable vertical sync (default: disabled)
//   --scale | -s <val> — simulation time scale relative to real time (default: 1.0, e.g. 0.5 for half-speed slowmo)

#include <garnet/GNfiz.h>
#include "render-helpers.h"
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#if GN_BUILD_HAS_MSW
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

using namespace GN;
using namespace GN::fiz;
using namespace GN::fx2;
using namespace GN::gpu2;
using namespace GN::util;
using namespace fizsample;

static GN::Logger * sLogger = GN::getLogger("GN.sample.fiz.solids");

namespace {

static SharedShaderConstants::Snapshot updateSsc(SharedShaderConstants * ssc, const RasterTarget & target, const glm::vec3 & eye, const glm::vec3 & targetPos,
                                                 int frameIdx) {
    const glm::vec3 kUp(0.0f, 1.0f, 0.0f);
    const glm::mat4 camToWorld = glm::inverse(glm::lookAtRH(eye, targetPos, kUp));
    const auto      rasterSize = target.calcRasterSizeInPixel();

    ssc->set0.camera.cameraPosition       = eye;
    ssc->set0.camera.cameraOrientation    = glm::quat_cast(glm::mat3(camToWorld));
    ssc->set0.camera.aspectRatio          = static_cast<float>(rasterSize.x) / static_cast<float>(rasterSize.y);
    ssc->set0.camera.viewWidthInPixel     = rasterSize.x;
    ssc->set0.camera.viewHeightInPixel    = rasterSize.y;
    ssc->set0.frameConstants.frameCounter = frameIdx;

    return ssc->takeSnapshot();
}

enum ModelKind : uint8_t { MODEL_FLOOR = 0, MODEL_BOX = 1, MODEL_SPHERE = 2, MODEL_PROJECTILE = 3, MODEL_WALL = 4, MODEL_COUNT = 5 };

struct SimulatedEntity {
    AutoRef<Solid> solid;
    ModelKind      kind;
};

// ─── Simulation Environment ──────────────────────────────────────────────────

class SimulationArena {
public:
    AutoRef<SolidEngine>         engine;
    std::vector<SimulatedEntity> entities;
    uint32_t                     configuredThreads = 0;

    void reset(uint32_t workerThreads, size_t initialBodyCount = 1000) {
        configuredThreads = workerThreads;
        entities.clear();

        SolidEngineDesc desc;
        desc.gravity               = {0.0f, -9.81f, 0.0f};
        desc.numWorkerThreads      = workerThreads;
        desc.maxBodies             = 32768;
        desc.maxBodyPairs          = 32768;
        desc.maxContactConstraints = 32768;
        engine                     = SolidEngine::create(desc);

        // 1. Static ground floor (size: 80 x 2 x 80)
        SolidDesc floorDesc;
        floorDesc.hull               = Hull::createBox({40.0f, 1.0f, 40.0f});
        floorDesc.motionType         = MotionType::STATIC;
        floorDesc.layer              = CollisionLayer::NON_MOVING;
        floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
        floorDesc.temper.restitution = 0.2f;
        floorDesc.temper.friction    = 0.6f;
        auto floor                   = engine->createSolid(floorDesc);
        entities.push_back({floor, MODEL_FLOOR});

        // 2. Static boundary walls to contain the colliding avalanche
        createWall(fiz::Vector3(40.0f, 10.0f, 0.0f), fiz::Vector3(1.0f, 10.0f, 40.0f));  // X+
        createWall(fiz::Vector3(-40.0f, 10.0f, 0.0f), fiz::Vector3(1.0f, 10.0f, 40.0f)); // X-
        createWall(fiz::Vector3(0.0f, 10.0f, 40.0f), fiz::Vector3(40.0f, 10.0f, 1.0f));  // Z+
        createWall(fiz::Vector3(0.0f, 10.0f, -40.0f), fiz::Vector3(40.0f, 10.0f, 1.0f)); // Z-

        // 3. Populate dynamic rigid bodies in a structured 3D grid
        spawnGrid(initialBodyCount);
    }

    void spawnGrid(size_t targetCount) {
        int dimXZ = static_cast<int>(std::ceil(std::sqrt(static_cast<float>(targetCount) / 10.0f)));
        dimXZ     = std::max(dimXZ, 6);

        float spacing = 1.8f;
        float halfDim = static_cast<float>(dimXZ) * 0.5f * spacing;

        auto boxHull    = Hull::createBox({0.6f, 0.6f, 0.6f});
        auto sphereHull = Hull::createSphere(0.6f);

        size_t spawned = 0;
        int    layer   = 0;
        while (spawned < targetCount) {
            float y = 2.0f + static_cast<float>(layer) * 1.8f;
            for (int x = 0; x < dimXZ && spawned < targetCount; ++x) {
                for (int z = 0; z < dimXZ && spawned < targetCount; ++z) {
                    float px = -halfDim + static_cast<float>(x) * spacing + ((layer & 1) ? 0.4f : -0.4f);
                    float pz = -halfDim + static_cast<float>(z) * spacing + ((layer & 2) ? 0.4f : -0.4f);

                    SolidDesc desc;
                    bool      isSphere      = (spawned % 2 == 0);
                    desc.hull               = isSphere ? sphereHull : boxHull;
                    desc.motionType         = MotionType::DYNAMIC;
                    desc.layer              = CollisionLayer::MOVING;
                    desc.transform.position = {px, y, pz};
                    desc.temper.restitution = 0.3f;
                    desc.temper.friction    = 0.5f;
                    desc.temper.density     = 1000.0f;
                    desc.entityId           = spawned + 10;

                    auto solid = engine->createSolid(desc);
                    entities.push_back({solid, isSphere ? MODEL_SPHERE : MODEL_BOX});
                    ++spawned;
                }
            }
            ++layer;
        }
    }

    void launchProjectile(const glm::vec3 & eye, const glm::vec3 & target) {
        glm::vec3 dir = glm::normalize(target - eye);
        SolidDesc desc;
        desc.hull               = Hull::createSphere(1.8f);
        desc.motionType         = MotionType::DYNAMIC;
        desc.layer              = CollisionLayer::MOVING;
        desc.transform.position = {eye.x, eye.y, eye.z};
        desc.linearVelocity     = {dir.x * 60.0f, dir.y * 60.0f, dir.z * 60.0f};
        desc.massOverride       = 150.0f; // Heavy wrecker ball
        desc.ccd                = true;   // CCD ensures no tunneling
        desc.temper.restitution = 0.8f;
        desc.temper.friction    = 0.2f;

        auto solid = engine->createSolid(desc);
        entities.push_back({solid, MODEL_PROJECTILE});
    }

private:
    void createWall(const fiz::Vector3 & pos, const fiz::Vector3 & halfExtents) {
        SolidDesc desc;
        desc.hull               = Hull::createBox(halfExtents);
        desc.motionType         = MotionType::STATIC;
        desc.layer              = CollisionLayer::NON_MOVING;
        desc.transform.position = pos;
        auto wall               = engine->createSolid(desc);
        entities.push_back({wall, MODEL_WALL});
    }
};

} // namespace

int main(int argc, const char ** argv) {
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::setvbuf(stderr, nullptr, _IONBF, 0);

    bool  testMode  = false;
    int   framesArg = 0;
    bool  vsync     = false;
    float timeScale = 1.0f;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "test") == 0 || argv[i][0] == 't') {
            testMode = true;
        } else if (std::strcmp(argv[i], "--vsync") == 0 || std::strcmp(argv[i], "-v") == 0) {
            vsync = true;
        } else if ((std::strcmp(argv[i], "--scale") == 0 || std::strcmp(argv[i], "-s") == 0) && i + 1 < argc) {
            timeScale = static_cast<float>(std::atof(argv[++i]));
        } else if (std::strncmp(argv[i], "--scale=", 8) == 0) {
            timeScale = static_cast<float>(std::atof(argv[i] + 8));
        } else if (std::strncmp(argv[i], "-s=", 3) == 0) {
            timeScale = static_cast<float>(std::atof(argv[i] + 3));
        } else {
            framesArg = std::atoi(argv[i]);
        }
    }
    timeScale = std::max(0.0f, timeScale);

    if (testMode) {
        GN_INFO(sLogger, "Running GNsample-fiz-solids in test mode");
    } else {
        GN_INFO(sLogger,
                "Interactive visual mode (vsync {}, scale {:.2f}x): Press ESC to quit, [1,2,4,8,0] threads, [B] spawn, [Space] wrecking ball, [R] "
                "reset, [ [ / ] ] scale, [P] pause, [A/D/W/S/Q/E] camera",
                vsync ? "on" : "off", timeScale);
    }

    enableCRTMemoryCheck();

    const uint32_t W = 1280, H = 720;

    // ─── GPU & Shading Pipeline ──────────────────────────────────────────────
    auto gpuContext = GpuContext::create("gpu", GpuContext::CreateParameters {});
    if (!gpuContext) {
        GN_ERROR(sLogger, "Failed to create GPU context");
        return -1;
    }

    auto ssc = SharedShaderConstants::create({.gpu = gpuContext});
    if (!ssc) return -1;

    ssc->set0.envLighting = {
        .skyboxPath                = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds",
        .irradiancePath            = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds",
        .prefilteredPath           = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
        .brdfLutPath               = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
        .environmentLuminanceScale = 3000.f,
    };

    auto initialization = GpuCnC::create({.gpu = gpuContext});
    if (!initialization) return -1;
    auto pbr    = PbrKernel::create(gpuContext, *initialization);
    auto skybox = SkyboxKernel::create(gpuContext);
    if (!pbr || !skybox) return -1;

    // Build procedural 3D model assets: Floor, Box, Sphere, Projectile, Wall
    PbrKernel::Inputs modelAssets[MODEL_COUNT];
    modelAssets[MODEL_FLOOR]      = boxInputs(gpuContext, *initialization, {40.0f, 1.0f, 40.0f}, {0.25f, 0.28f, 0.32f, 1.0f}, 0.1f, 0.8f);
    modelAssets[MODEL_BOX]        = boxInputs(gpuContext, *initialization, {0.6f, 0.6f, 0.6f}, {0.92f, 0.68f, 0.20f, 1.0f}, 0.3f, 0.4f); // Golden boxes
    modelAssets[MODEL_SPHERE]     = sphereInputs(gpuContext, *initialization, 0.6f, 16, 12, {0.20f, 0.75f, 0.85f, 1.0f}, 0.5f, 0.2f); // Cyan metallic spheres
    modelAssets[MODEL_PROJECTILE] = sphereInputs(gpuContext, *initialization, 1.8f, 24, 18, {0.95f, 0.25f, 0.20f, 1.0f}, 0.8f, 0.2f); // Heavy red wrecker ball
    modelAssets[MODEL_WALL]       = boxInputs(gpuContext, *initialization, {1.0f, 1.0f, 1.0f}, {0.18f, 0.20f, 0.22f, 0.8f}, 0.0f, 0.9f);

    for (int k = 0; k < MODEL_COUNT; ++k) {
        if (!modelAssets[k].geometry.indexCount) {
            GN_ERROR(sLogger, "Failed to create model asset {}", k);
            return -1;
        }
    }

    // ─── Window & Swapchain ──────────────────────────────────────────────────
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        window.reset(win::createWindow(
            win::WindowCreateParameters {.caption = "Garnet Fiz Solids [Multi-Threaded Physics Benchmark]", .clientWidth = W, .clientHeight = H}));
        if (!window) return -1;
        window->show();
        surface = window->createVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
        if (!surface) return -1;
    }

    Swapchain::CreateDesc scDesc {.gpu = gpuContext, .width = W, .height = H, .vsync = vsync};
    if (surface) scDesc.setSurface(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) return -1;

    auto depthTex = Texture::create(
        "depth", {.context = gpuContext, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(W, H)});
    if (!depthTex) return -1;
    GpuResourceView depthView;
    depthView.resource = depthTex;

    RasterTarget rasterTarget;
    rasterTarget.colorTargets.append(RasterTarget::ColorTarget {});
    rasterTarget.setDepthStencilTarget(depthView).setClearColor(0.08f, 0.09f, 0.12f, 1.f).setClearDepth(1.f);
    // Lit kernels inherit depth policy; depth writes keep the later skybox behind the solids.
    rasterTarget.states.depthState = RasterState::DepthState {RasterState::Compare::LESS, true};

    // Initial asset uploads
    bool initialUploadsSubmitted = false;

    // ─── Physics Simulation Setup ────────────────────────────────────────────
    SimulationArena arena;
    const uint32_t  hwThreads = std::max(1u, std::thread::hardware_concurrency());
    arena.reset(0, 1024); // 0 = auto-detect hardware concurrency, 1024 initial dynamic bodies

    GN_INFO(sLogger, "Physics simulation initialized with {} bodies across {} hardware threads", arena.entities.size() - 5, hwThreads);

    // Baseline 1-thread physics step duration for speedup calculation
    float baseline1ThreadMs = 0.0f;
    float currentStepMs     = 0.0f;
    float avgStepMs         = 0.0f;
    float avgDrawMs         = 0.0f;
    float avgFps            = 0.0f;

    // Camera control
    float     orbitAngle   = 0.4f;
    float     cameraDist   = 32.0f;
    float     cameraHeight = 14.0f;
    glm::vec3 cameraCenter(0.0f, 6.0f, 0.0f);

    // ─── Main Render & Simulation Loop ───────────────────────────────────────
    auto      lastTime        = std::chrono::high_resolution_clock::now();
    double    timeAccumulator = 0.0;
    int       frameIdx        = 0;
    const int totalFrames     = testMode ? 60 : framesArg;

    while (totalFrames == 0 || frameIdx < totalFrames) {
        ++frameIdx;
        if (window && !window->runUntilNoNewEvents()) break;

        // Input Handling
        if (window) {
            if (window->getKeyStatus(win::KeyCode::ESCAPE).down) break;

            // [1], [2], [4], [8], [0] = Thread count switches
            if (window->getKeyStatus(win::KeyCode::_1).down && arena.configuredThreads != 1) {
                arena.reset(1, arena.entities.size() > 5 ? arena.entities.size() - 5 : 1000);
                baseline1ThreadMs = 0.0f;
                GN_INFO(sLogger, "Switched physics solver to 1 worker thread (single-threaded baseline)");
            } else if (window->getKeyStatus(win::KeyCode::_2).down && arena.configuredThreads != 2) {
                arena.reset(2, arena.entities.size() > 5 ? arena.entities.size() - 5 : 1000);
                GN_INFO(sLogger, "Switched physics solver to 2 worker threads");
            } else if (window->getKeyStatus(win::KeyCode::_4).down && arena.configuredThreads != 4) {
                arena.reset(4, arena.entities.size() > 5 ? arena.entities.size() - 5 : 1000);
                GN_INFO(sLogger, "Switched physics solver to 4 worker threads");
            } else if (window->getKeyStatus(win::KeyCode::_8).down && arena.configuredThreads != 8) {
                arena.reset(8, arena.entities.size() > 5 ? arena.entities.size() - 5 : 1000);
                GN_INFO(sLogger, "Switched physics solver to 8 worker threads");
            } else if (window->getKeyStatus(win::KeyCode::_0).down && arena.configuredThreads != 0) {
                arena.reset(0, arena.entities.size() > 5 ? arena.entities.size() - 5 : 1000);
                GN_INFO(sLogger, "Switched physics solver to auto-detected threads ({} cores)", hwThreads);
            }

            // [B] = Spawn +250 dynamic bodies
            static bool bWasDown = false;
            bool        bDown    = window->getKeyStatus(win::KeyCode::B).down;
            if (bDown && !bWasDown) {
                arena.spawnGrid(250);
                GN_INFO(sLogger, "Spawned +250 dynamic bodies (Total: {})", arena.entities.size() - 5);
            }
            bWasDown = bDown;

            // [Space] = Launch heavy projectile
            static bool spaceWasDown = false;
            bool        spaceDown    = window->getKeyStatus(win::KeyCode::SPACEBAR).down;
            if (spaceDown && !spaceWasDown) {
                glm::vec3 eye(cameraDist * std::sin(orbitAngle), cameraHeight, cameraDist * std::cos(orbitAngle));
                arena.launchProjectile(eye, cameraCenter);
                GN_INFO(sLogger, "Launched heavy wrecking projectile into stack!");
            }
            spaceWasDown = spaceDown;

            // [R] = Reset stack
            static bool rWasDown = false;
            bool        rDown    = window->getKeyStatus(win::KeyCode::R).down;
            if (rDown && !rWasDown) {
                arena.reset(arena.configuredThreads, 1024);
                GN_INFO(sLogger, "Reset physics arena to 1024 bodies");
            }
            rWasDown = rDown;

            // Camera orbit and zoom keys
            if (window->getKeyStatus(win::KeyCode::LEFT).down || window->getKeyStatus(win::KeyCode::A).down) orbitAngle -= 0.02f;
            if (window->getKeyStatus(win::KeyCode::RIGHT).down || window->getKeyStatus(win::KeyCode::D).down) orbitAngle += 0.02f;
            if (window->getKeyStatus(win::KeyCode::UP).down || window->getKeyStatus(win::KeyCode::W).down) cameraHeight = std::min(45.0f, cameraHeight + 0.3f);
            if (window->getKeyStatus(win::KeyCode::DOWN).down || window->getKeyStatus(win::KeyCode::S).down) cameraHeight = std::max(2.0f, cameraHeight - 0.3f);
            if (window->getKeyStatus(win::KeyCode::Q).down) cameraDist = std::min(70.0f, cameraDist + 0.4f);
            if (window->getKeyStatus(win::KeyCode::E).down) cameraDist = std::max(6.0f, cameraDist - 0.4f);

            // [ / ] = Adjust time scale, [P] = Pause / Resume
            static bool lbracketWasDown = false;
            bool        lbracketDown    = window->getKeyStatus(win::KeyCode::LBRACKET).down;
            if (lbracketDown && !lbracketWasDown) {
                timeScale = std::max(0.0f, timeScale - 0.25f);
                GN_INFO(sLogger, "Simulation time scale: {:.2f}x", timeScale);
            }
            lbracketWasDown = lbracketDown;

            static bool rbracketWasDown = false;
            bool        rbracketDown    = window->getKeyStatus(win::KeyCode::RBRACKET).down;
            if (rbracketDown && !rbracketWasDown) {
                timeScale += 0.25f;
                GN_INFO(sLogger, "Simulation time scale: {:.2f}x", timeScale);
            }
            rbracketWasDown = rbracketDown;

            static bool  pWasDown    = false;
            static float pausedScale = 1.0f;
            bool         pDown       = window->getKeyStatus(win::KeyCode::P).down;
            if (pDown && !pWasDown) {
                if (timeScale > 0.0f) {
                    pausedScale = timeScale;
                    timeScale   = 0.0f;
                    GN_INFO(sLogger, "Simulation paused");
                } else {
                    timeScale = (pausedScale > 0.0f) ? pausedScale : 1.0f;
                    GN_INFO(sLogger, "Simulation resumed: {:.2f}x", timeScale);
                }
            }
            pWasDown = pDown;
        } else {
            // Auto orbit in headless / test mode
            orbitAngle += 0.01f;
        }

        // Compute frame rate
        auto  now        = std::chrono::high_resolution_clock::now();
        float frameDt    = std::chrono::duration<float>(now - lastTime).count();
        lastTime         = now;
        float instantFps = frameDt > 0.0f ? 1.0f / frameDt : 60.0f;
        avgFps           = (avgFps == 0.0f) ? instantFps : (avgFps * 0.95f + instantFps * 0.05f);

        // ─── Step Physics with Fixed Timestep Accumulator ────────────────────
        constexpr UnitOfTime kPhysicsStep(16'666'667); // 60 Hz fixed tick
        constexpr double     kFixedStepSec = 1.0 / 60.0;

        if (testMode) {
            // Headless test mode maintains deterministic single-step progression
            const auto stepStart = std::chrono::high_resolution_clock::now();
            arena.engine->step(kPhysicsStep);
            const auto stepEnd = std::chrono::high_resolution_clock::now();

            currentStepMs = std::chrono::duration<float, std::milli>(stepEnd - stepStart).count();
            avgStepMs     = (avgStepMs == 0.0f) ? currentStepMs : (avgStepMs * 0.9f + currentStepMs * 0.1f);
        } else {
            float clampedDt = std::max(0.0f, std::min(frameDt, 0.1f));
            timeAccumulator += static_cast<double>(clampedDt) * static_cast<double>(timeScale);

            constexpr int kMaxSubsteps = 5;
            int           substeps     = 0;
            float         totalStepMs  = 0.0f;

            while (timeAccumulator >= kFixedStepSec && substeps < kMaxSubsteps) {
                const auto stepStart = std::chrono::high_resolution_clock::now();
                arena.engine->step(kPhysicsStep);
                const auto stepEnd = std::chrono::high_resolution_clock::now();

                totalStepMs += std::chrono::duration<float, std::milli>(stepEnd - stepStart).count();
                timeAccumulator -= kFixedStepSec;
                ++substeps;
            }

            if (substeps >= kMaxSubsteps) { timeAccumulator = std::fmod(timeAccumulator, kFixedStepSec); }

            if (substeps > 0) {
                currentStepMs = totalStepMs / static_cast<float>(substeps);
                avgStepMs     = (avgStepMs == 0.0f) ? currentStepMs : (avgStepMs * 0.9f + currentStepMs * 0.1f);
            }
        }

        if (arena.configuredThreads == 1 && (baseline1ThreadMs == 0.0f || frameIdx % 60 == 0)) { baseline1ThreadMs = avgStepMs; }

        // Update window title telemetry
        if (window && frameIdx % 10 == 0) {
            uint32_t activeThreads = arena.configuredThreads == 0 ? hwThreads : arena.configuredThreads;
            size_t   bodyCount     = arena.entities.size() > 5 ? arena.entities.size() - 5 : 0;
            size_t   activeCount   = arena.engine->activeBodyCount();

            std::string title =
                StrA::format("Garnet Fiz | Bodies: {} ({} active) | Physics: {:.2f} ms [{} threads] | Draw: {:.2f} ms | FPS: {:.0f} | Sim: {:.2f}x", bodyCount,
                             activeCount, avgStepMs, activeThreads, avgDrawMs, avgFps, timeScale)
                    .data();

            if (baseline1ThreadMs > 0.0f && activeThreads > 1) {
                float speedup = baseline1ThreadMs / std::max(0.001f, avgStepMs);
                title += StrA::format(" | Speedup: {:.1f}x", speedup).data();
            }

#if GN_BUILD_HAS_MSW
            ::SetWindowTextA((HWND) window->getWindowHandle(), title.c_str());
#endif

            if (frameIdx % 60 == 0) { GN_INFO(sLogger, "{}", title); }
        }

        // ─── Render Frame ────────────────────────────────────────────────────
        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) return -1;

        rasterTarget.setColorTarget(0, frame.view);

        glm::vec3                       eye(cameraDist * std::sin(orbitAngle), cameraHeight, cameraDist * std::cos(orbitAngle));
        SharedShaderConstants::Snapshot sscSnapshot = updateSsc(ssc.get(), rasterTarget, eye, cameraCenter, frameIdx);

        DynaArray<AutoRef<GpuPayload>> renderWorks;
        renderWorks.append(sscSnapshot.set0Payloads);

        auto                        tRaster0 = std::chrono::high_resolution_clock::now();
        GpuRaster::CreateParameters rcp;
        rcp.gpu            = gpuContext;
        rcp.target         = &rasterTarget;
        auto kernelUploads = GpuCnC::create({.gpu = gpuContext});
        auto r             = GpuRaster::create("fiz-solids-raster", rcp);
        if (r) {
            // Draw all physical solids (walls are kept as invisible collision barriers)
            for (const auto & e : arena.entities) {
                if (!e.solid) continue;
                if (e.kind == MODEL_WALL) {
                    // Containment boundary walls are invisible physics barriers so they don't occlude the scene or skybox.
                    continue;
                }
                auto draw = modelAssets[e.kind];
                if (!draw.geometry.indexCount) continue;

                Transform t = e.solid->transform();

                glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), glm::vec3(t.position.x, t.position.y, t.position.z)) *
                                           glm::mat4_cast(glm::quat(t.orientation.w, t.orientation.v.x, t.orientation.v.y, t.orientation.v.z));
                draw.worldFromObject     = worldTransform;
                if (!pbr->record(*r, *kernelUploads, sscSnapshot.set0Resources, draw)) GN_ERROR(sLogger, "Failed to record PBR draw");
            }

            // Skybox
            if (!skybox->record(*r, sscSnapshot.set0Resources)) GN_ERROR(sLogger, "Failed to record skybox");
            renderWorks.append(kernelUploads->seal());
            renderWorks.append(r->seal());
        }
        auto  tRaster1 = std::chrono::high_resolution_clock::now();
        float drawMs   = std::chrono::duration<float, std::milli>(tRaster1 - tRaster0).count();
        avgDrawMs      = (avgDrawMs == 0.0f) ? drawMs : (avgDrawMs * 0.95f + drawMs * 0.05f);

        // Submit GPU work
        GpuContext::SubmitParameters submit(StrA::format("frame {}", frameIdx));
        if (!initialUploadsSubmitted) {
            submit.appendWork(initialization->seal());
            initialUploadsSubmitted = true;
        }

        for (size_t i = 0; i < renderWorks.size(); ++i) {
            if (!renderWorks[i]) continue;
            if (i + 1 == renderWorks.size())
                submit.appendWork(renderWorks[i]).waitFor(frame.ready);
            else
                submit.appendWork(renderWorks[i]);
        }
        gpuContext->submit(submit);
        if (!renderWorks.empty() && renderWorks.back()) swapchain->present(*renderWorks.back());
    }

    if (testMode) {
        GN_INFO(sLogger, "Test mode completed successfully: {} steps simulated, avg physics time: {:.2f} ms, avg draw time: {:.2f} ms", frameIdx, avgStepMs,
                avgDrawMs);
    }

    gpuContext->waitForIdle();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);

    return 0;
}

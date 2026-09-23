// solids.cpp — Standalone visual sample demonstrating GNfiz solid rigid body simulation
// with high-throughput multi-threaded solving across hundreds/thousands of colliding objects.
//
// Usage: GNsample-fiz-solids [t]
//   t — headless test mode: runs simulation for 60 steps, asserts physics motion, and exits cleanly.

#include <garnet/GNfiz.h>
#include <garnet/GNfx2.h>
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

static GN::Logger * sLogger = GN::getLogger("GN.sample.fiz.solids");

namespace {

static constexpr float PI = 3.14159265358979323846f;

// ─── Procedural ModelScene Builders ──────────────────────────────────────────

struct ProceduralScene : ModelScene {
    explicit ProceduralScene(const StrA & name): ModelScene(ModelScene::TYPE_INFO(), name) {}
};

static AutoRef<ModelScene> createBoxScene(const glm::vec3 & halfExtents, const glm::vec4 & color, float metallic = 0.2f, float roughness = 0.5f) {
    AutoRef<ModelScene> result(new ProceduralScene("box-scene"));
    result->sourcePath = "generated://box";

    ModelScene::Material material;
    material.name      = "box-mat";
    material.workflow  = ModelScene::MaterialWorkflow::DEFAULT_LIT;
    material.baseColor = color;
    material.metallic  = metallic;
    material.roughness = roughness;
    result->materials.append(material);

    ModelScene::Primitive primitive;
    primitive.name     = "box-prim";
    primitive.material = 0;
    primitive.bounds   = {-halfExtents, halfExtents, true};

    // 6 faces * 4 vertices = 24 vertices
    static const glm::vec3 faceNormals[6] = {
        {0.0f, 0.0f, 1.0f},  // Z+
        {0.0f, 0.0f, -1.0f}, // Z-
        {1.0f, 0.0f, 0.0f},  // X+
        {-1.0f, 0.0f, 0.0f}, // X-
        {0.0f, 1.0f, 0.0f},  // Y+
        {0.0f, -1.0f, 0.0f}  // Y-
    };

    static const glm::vec4 faceTangents[6] = {
        {1.0f, 0.0f, 0.0f, 1.0f},  // Z+
        {-1.0f, 0.0f, 0.0f, 1.0f}, // Z-
        {0.0f, 0.0f, -1.0f, 1.0f}, // X+
        {0.0f, 0.0f, 1.0f, 1.0f},  // X-
        {1.0f, 0.0f, 0.0f, 1.0f},  // Y+
        {1.0f, 0.0f, 0.0f, 1.0f}   // Y-
    };

    static const glm::vec3 faceVertices[6][4] = {// Z+
                                                 {{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}},
                                                 // Z-
                                                 {{1, -1, -1}, {-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}},
                                                 // X+
                                                 {{1, -1, 1}, {1, -1, -1}, {1, 1, -1}, {1, 1, 1}},
                                                 // X-
                                                 {{-1, -1, -1}, {-1, -1, 1}, {-1, 1, 1}, {-1, 1, -1}},
                                                 // Y+
                                                 {{-1, 1, 1}, {1, 1, 1}, {1, 1, -1}, {-1, 1, -1}},
                                                 // Y-
                                                 {{-1, -1, -1}, {1, -1, -1}, {1, -1, 1}, {-1, -1, 1}}};

    for (int f = 0; f < 6; ++f) {
        uint32_t baseIdx = static_cast<uint32_t>(primitive.vertices.size());
        for (int v = 0; v < 4; ++v) {
            ModelScene::Vertex vert;
            vert.position = faceVertices[f][v] * halfExtents;
            vert.normal   = faceNormals[f];
            vert.tangent  = faceTangents[f];
            vert.texcoord = glm::vec2((v == 1 || v == 2) ? 1.0f : 0.0f, (v >= 2) ? 1.0f : 0.0f);
            vert.color    = color;
            primitive.vertices.append(vert);
        }
        primitive.indices.append(baseIdx + 0);
        primitive.indices.append(baseIdx + 1);
        primitive.indices.append(baseIdx + 2);
        primitive.indices.append(baseIdx + 0);
        primitive.indices.append(baseIdx + 2);
        primitive.indices.append(baseIdx + 3);
    }

    result->primitives.append(std::move(primitive));

    ModelScene::Node node;
    node.name = "root";
    node.primitives.append(0);
    node.bounds = result->primitives[0].bounds;
    result->nodes.append(std::move(node));

    return result;
}

static AutoRef<ModelScene> createSphereScene(float radius, int slices, int stacks, const glm::vec4 & color, float metallic = 0.3f, float roughness = 0.2f) {
    AutoRef<ModelScene> result(new ProceduralScene("sphere-scene"));
    result->sourcePath = "generated://sphere";

    ModelScene::Material material;
    material.name      = "sphere-mat";
    material.workflow  = ModelScene::MaterialWorkflow::DEFAULT_LIT;
    material.baseColor = color;
    material.metallic  = metallic;
    material.roughness = roughness;
    result->materials.append(material);

    ModelScene::Primitive primitive;
    primitive.name     = "sphere-prim";
    primitive.material = 0;
    primitive.bounds   = {glm::vec3(-radius), glm::vec3(radius), true};

    for (int i = 0; i <= stacks; ++i) {
        float phi    = PI * static_cast<float>(i) / static_cast<float>(stacks);
        float sinPhi = std::sin(phi);
        float cosPhi = std::cos(phi);

        for (int j = 0; j <= slices; ++j) {
            float theta    = 2.0f * PI * static_cast<float>(j) / static_cast<float>(slices);
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            glm::vec3          n(sinPhi * cosTheta, cosPhi, sinPhi * sinTheta);
            ModelScene::Vertex vert;
            vert.position = n * radius;
            vert.normal   = n;
            vert.tangent  = glm::vec4(-sinTheta, 0.0f, cosTheta, 1.0f);
            vert.texcoord = glm::vec2(static_cast<float>(j) / slices, static_cast<float>(i) / stacks);
            vert.color    = color;
            primitive.vertices.append(vert);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            uint32_t first  = static_cast<uint32_t>((i * (slices + 1)) + j);
            uint32_t second = first + slices + 1;

            primitive.indices.append(first);
            primitive.indices.append(first + 1);
            primitive.indices.append(second);

            primitive.indices.append(second);
            primitive.indices.append(first + 1);
            primitive.indices.append(second + 1);
        }
    }

    result->primitives.append(std::move(primitive));

    ModelScene::Node node;
    node.name = "root";
    node.primitives.append(0);
    node.bounds = result->primitives[0].bounds;
    result->nodes.append(std::move(node));

    return result;
}

// ─── Shared Shader Constants Helper ──────────────────────────────────────────

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

    bool testMode  = false;
    int  framesArg = 0;
    if (argc > 1) {
        if (argv[1][0] == 't') {
            testMode = true;
        } else {
            framesArg = std::atoi(argv[1]);
        }
    }

    if (testMode) {
        GN_INFO(sLogger, "Running GNsample-fiz-solids in test mode");
    } else {
        GN_INFO(sLogger, "Interactive visual mode: Press ESC to quit, [1,2,4,8,0] for threads, [B] spawn, [Space] wrecking ball, [R] "
                         "reset, [A/D/W/S/Q/E] camera");
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

    auto modelShading = ModelShading::create(gpuContext);
    if (!modelShading) return -1;

    // Build procedural 3D model assets: Floor, Box, Sphere, Projectile, Wall
    AutoRef<ModelAsset> modelAssets[MODEL_COUNT];
    modelAssets[MODEL_FLOOR] = ModelAsset::create(gpuContext, createBoxScene({40.0f, 1.0f, 40.0f}, {0.25f, 0.28f, 0.32f, 1.0f}, 0.1f, 0.8f));
    modelAssets[MODEL_BOX]   = ModelAsset::create(gpuContext, createBoxScene({0.6f, 0.6f, 0.6f}, {0.92f, 0.68f, 0.20f, 1.0f}, 0.3f, 0.4f)); // Golden boxes
    modelAssets[MODEL_SPHERE] =
        ModelAsset::create(gpuContext, createSphereScene(0.6f, 16, 12, {0.20f, 0.75f, 0.85f, 1.0f}, 0.5f, 0.2f)); // Cyan metallic spheres
    modelAssets[MODEL_PROJECTILE] =
        ModelAsset::create(gpuContext, createSphereScene(1.8f, 24, 18, {0.95f, 0.25f, 0.20f, 1.0f}, 0.8f, 0.2f)); // Heavy red wrecker ball
    modelAssets[MODEL_WALL] = ModelAsset::create(gpuContext, createBoxScene({1.0f, 1.0f, 1.0f}, {0.18f, 0.20f, 0.22f, 0.8f}, 0.0f, 0.9f));

    for (int k = 0; k < MODEL_COUNT; ++k) {
        if (!modelAssets[k]) {
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

    Swapchain::CreateDesc scDesc {.gpu = gpuContext, .width = W, .height = H};
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
    float avgFps            = 0.0f;

    // Camera control
    float     orbitAngle   = 0.4f;
    float     cameraDist   = 32.0f;
    float     cameraHeight = 14.0f;
    glm::vec3 cameraCenter(0.0f, 6.0f, 0.0f);

    // Fixed-size blob pool for push constants to eliminate per-object allocation overhead
    struct PushConstants {
        glm::mat4 world;
        glm::mat4 normal;
    };
    FixedBlob<uint8_t> pushConstantPool(sizeof(PushConstants));

    // ─── Main Render & Simulation Loop ───────────────────────────────────────
    auto      lastTime    = std::chrono::high_resolution_clock::now();
    int       frameIdx    = 0;
    const int totalFrames = testMode ? 60 : framesArg;

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
        } else {
            // Auto orbit in headless / test mode
            orbitAngle += 0.01f;
        }

        // ─── Step Physics with High-Precision Timing ─────────────────────────
        const auto stepStart = std::chrono::high_resolution_clock::now();
        arena.engine->step(UnitOfTime(16'666'667)); // 60 Hz integer step
        const auto stepEnd = std::chrono::high_resolution_clock::now();

        currentStepMs = std::chrono::duration<float, std::milli>(stepEnd - stepStart).count();
        avgStepMs     = (avgStepMs == 0.0f) ? currentStepMs : (avgStepMs * 0.9f + currentStepMs * 0.1f);

        if (arena.configuredThreads == 1 && (baseline1ThreadMs == 0.0f || frameIdx % 60 == 0)) { baseline1ThreadMs = avgStepMs; }

        // Compute frame rate
        auto  now        = std::chrono::high_resolution_clock::now();
        float frameDt    = std::chrono::duration<float>(now - lastTime).count();
        lastTime         = now;
        float instantFps = frameDt > 0.0f ? 1.0f / frameDt : 60.0f;
        avgFps           = (avgFps == 0.0f) ? instantFps : (avgFps * 0.95f + instantFps * 0.05f);

        // Update window title telemetry
        if (window && frameIdx % 10 == 0) {
            uint32_t activeThreads = arena.configuredThreads == 0 ? hwThreads : arena.configuredThreads;
            size_t   bodyCount     = arena.entities.size() > 5 ? arena.entities.size() - 5 : 0;
            size_t   activeCount   = arena.engine->activeBodyCount();

            std::string title = StrA::format("Garnet Fiz | Bodies: {} ({} active) | Physics: {:.2f} ms [{} threads] | FPS: {:.0f}", bodyCount, activeCount,
                                             avgStepMs, activeThreads, avgFps)
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

        // Pre-create base draw parameters for each model kind once per frame
        GpuRaster::DrawParameters baseDraw[MODEL_COUNT];
        for (int k = 0; k < MODEL_COUNT; ++k) {
            if (modelAssets[k]) { baseDraw[k] = ModelShading::getDrawParams(sscSnapshot, modelShading, modelAssets[k], 0, glm::mat4(1.0f)); }
        }

        GpuRaster::CreateParameters rcp;
        rcp.gpu    = gpuContext;
        rcp.target = &rasterTarget;
        auto r     = GpuRaster::create("fiz-solids-raster", rcp);
        if (r) {
            // Draw all physical solids (walls are kept as invisible collision barriers)
            for (const auto & e : arena.entities) {
                if (!e.solid) continue;
                if (e.kind == MODEL_WALL) {
                    // Containment boundary walls are invisible physics barriers so they don't occlude the scene or skybox.
                    continue;
                }
                const auto & base = baseDraw[e.kind];
                if (!base.vs || !base.ps) continue;

                Transform t = e.solid->transform();

                glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), glm::vec3(t.position.x, t.position.y, t.position.z)) *
                                           glm::mat4_cast(glm::quat(t.orientation.w, t.orientation.v.x, t.orientation.v.y, t.orientation.v.z));

                const glm::mat4     normalTransform = glm::transpose(glm::inverse(worldTransform));
                const PushConstants constants {worldTransform, normalTransform};

                auto draw       = base;
                draw.immediates = pushConstantPool.allocate(reinterpret_cast<const uint8_t *>(&constants));
                r->draw(draw);
            }

            // Skybox
            r->draw(ssc->getSkyboxDrawParams(sscSnapshot.set0Resources));
            renderWorks.append(r->seal());
        }

        // Submit GPU work
        GpuContext::SubmitParameters submit(StrA::format("frame {}", frameIdx));
        if (!initialUploadsSubmitted) {
            if (const auto p = modelShading->uploadPayload()) submit.appendWork(p);
            for (int k = 0; k < MODEL_COUNT; ++k) {
                if (const auto p = modelAssets[k]->uploadPayload()) submit.appendWork(p);
            }
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

    if (testMode) { GN_INFO(sLogger, "Test mode completed successfully: {} steps simulated, avg physics time: {:.2f} ms", frameIdx, avgStepMs); }

    gpuContext->waitForIdle();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);

    return 0;
}

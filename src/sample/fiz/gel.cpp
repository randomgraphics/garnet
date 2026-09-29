// gel.cpp — Standalone visual sample demonstrating GNfiz Gel (XPBD volumetric soft body) simulation.
// Features deformable cubes and spheres with XPBD edge and volume constraints, mutual solid-gel collisions,
// dynamic GPU vertex buffer streaming, PBR shading, and interactive projectile impacts.
//
// Usage: GNsample-fiz-gel [t]
//   t — headless test mode: runs simulation for 80 steps, asserts volume preservation and rebound, and exits cleanly.

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

static GN::Logger * sLogger = GN::getLogger("GN.sample.fiz.gel");

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

    static const glm::vec3 faceVertices[6][4] = {
        {{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}},     // Z+
        {{1, -1, -1}, {-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}}, // Z-
        {{1, -1, 1}, {1, -1, -1}, {1, 1, -1}, {1, 1, 1}},     // X+
        {{-1, -1, -1}, {-1, -1, 1}, {-1, 1, 1}, {-1, 1, -1}}, // X-
        {{-1, 1, 1}, {1, 1, 1}, {1, 1, -1}, {-1, 1, -1}},     // Y+
        {{-1, -1, -1}, {1, -1, -1}, {1, -1, 1}, {-1, -1, 1}}  // Y-
    };

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

static AutoRef<ModelScene> createGelModelScene(const AutoRef<GelMesh> & mesh, const glm::vec4 & color, float metallic = 0.2f, float roughness = 0.3f) {
    AutoRef<ModelScene> result(new ProceduralScene("gel-template-scene"));
    result->sourcePath = "generated://gel-template";

    ModelScene::Material material;
    material.name      = "gel-template-mat";
    material.workflow  = ModelScene::MaterialWorkflow::DEFAULT_LIT;
    material.baseColor = color;
    material.metallic  = metallic;
    material.roughness = roughness;
    result->materials.append(material);

    ModelScene::Primitive primitive;
    primitive.name     = "gel-template-prim";
    primitive.material = 0;

    glm::vec3 minPos(1e9f), maxPos(-1e9f);
    auto      faceVerts = mesh->faceVertices();
    size_t    numVerts  = faceVerts.size();
    primitive.vertices.reserve(numVerts);
    for (const auto & v : faceVerts) {
        ModelScene::Vertex mv;
        mv.position = glm::vec3(v.position.x, v.position.y, v.position.z);
        mv.normal   = (glm::length(mv.position) > 1e-4f) ? glm::normalize(mv.position) : glm::vec3(0.0f, 1.0f, 0.0f);
        mv.tangent  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        mv.texcoord = glm::vec2(0.5f, 0.5f);
        mv.color    = color;
        primitive.vertices.append(mv);

        minPos = glm::min(minPos, mv.position);
        maxPos = glm::max(maxPos, mv.position);
    }
    primitive.bounds = {minPos, maxPos, true};

    size_t numFaces = mesh->faces().size();
    primitive.indices.reserve(numFaces * 3);
    for (const auto & f : mesh->faces()) {
        primitive.indices.append(f.indices[0]);
        primitive.indices.append(f.indices[1]);
        primitive.indices.append(f.indices[2]);
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

// ─── Simulation Entities ─────────────────────────────────────────────────────

enum TemplateKind : uint8_t {
    TEMPLATE_CUBE_GREEN    = 0,
    TEMPLATE_CUBE_BLUE     = 1,
    TEMPLATE_SPHERE_RED    = 2,
    TEMPLATE_SPHERE_AMBER  = 3,
    TEMPLATE_SPHERE_PURPLE = 4,
    TEMPLATE_COUNT         = 5
};

enum RigidKind : uint8_t { RIGID_FLOOR = 0, RIGID_WALL = 1, RIGID_PROJECTILE = 2, RIGID_COUNT = 3 };

struct SimulatedSolid {
    AutoRef<Solid> solid;
    RigidKind      kind;
};

struct SimulatedGel {
    AutoRef<Gel>                    gel;
    TemplateKind                    templateKind = TEMPLATE_CUBE_GREEN;
    AutoRef<Buffer>                 dynamicVb[2]; // Double-buffered GPU vertex buffer
    std::vector<ModelScene::Vertex> cpuVertices;
};

struct PushConstants {
    glm::mat4 world;
    glm::mat4 normal;
};

// ─── Simulation Arena ────────────────────────────────────────────────────────

class GelArena {
public:
    AutoRef<GelSolver>          solver;
    std::vector<SimulatedSolid> solids;
    std::vector<SimulatedGel>   gels;
    uint32_t                    configuredThreads = 0;

    AutoRef<GelMesh> meshCube;
    AutoRef<GelMesh> meshSphere;

    AutoRef<ModelAsset> gelTemplateAssets[TEMPLATE_COUNT];
    AutoRef<ModelAsset> rigidAssets[RIGID_COUNT];

    void initMeshesAndAssets(AutoRef<GpuContext> gpu) {
        // Soft body meshes
        meshCube   = GelMesh::createCube(4, 1.4f);
        meshSphere = GelMesh::createSphere(2.0f, 20, 14); // Single big bouncy ball (radius 2.0m)

        // Gel template visual assets (PBR colored jelly materials)
        // Emerald green cube
        gelTemplateAssets[TEMPLATE_CUBE_GREEN] = ModelAsset::create(gpu, createGelModelScene(meshCube, {0.15f, 0.85f, 0.40f, 1.0f}, 0.2f, 0.3f));
        // Sapphire blue cube
        gelTemplateAssets[TEMPLATE_CUBE_BLUE] = ModelAsset::create(gpu, createGelModelScene(meshCube, {0.20f, 0.50f, 0.95f, 1.0f}, 0.3f, 0.25f));
        // Ruby red sphere (radiant jelly)
        gelTemplateAssets[TEMPLATE_SPHERE_RED] = ModelAsset::create(gpu, createGelModelScene(meshSphere, {0.95f, 0.20f, 0.35f, 1.0f}, 0.4f, 0.2f));
        // Amber gold sphere
        gelTemplateAssets[TEMPLATE_SPHERE_AMBER] = ModelAsset::create(gpu, createGelModelScene(meshSphere, {0.95f, 0.70f, 0.15f, 1.0f}, 0.3f, 0.3f));
        // Amethyst purple sphere
        gelTemplateAssets[TEMPLATE_SPHERE_PURPLE] = ModelAsset::create(gpu, createGelModelScene(meshSphere, {0.70f, 0.25f, 0.90f, 1.0f}, 0.35f, 0.25f));

        // Rigid body assets
        rigidAssets[RIGID_FLOOR]      = ModelAsset::create(gpu, createBoxScene({30.0f, 1.0f, 30.0f}, {0.20f, 0.22f, 0.25f, 1.0f}, 0.1f, 0.8f));
        rigidAssets[RIGID_WALL]       = ModelAsset::create(gpu, createBoxScene({1.0f, 1.0f, 1.0f}, {0.18f, 0.20f, 0.22f, 0.8f}, 0.0f, 0.9f));
        rigidAssets[RIGID_PROJECTILE] = ModelAsset::create(gpu, createSphereScene(1.5f, 20, 16, {0.95f, 0.15f, 0.10f, 1.0f}, 0.7f, 0.2f));
    }

    void reset(uint32_t workerThreads, AutoRef<GpuContext> gpu, size_t initialGelCount = 1) {
        configuredThreads = workerThreads;
        solids.clear();
        gels.clear();

        GelSolverDesc desc;
        desc.gravity          = {0.0f, -9.81f, 0.0f};
        desc.numWorkerThreads = workerThreads;
        desc.maxSolids        = 4096;
        desc.maxGels          = 1024;
        solver                = GelSolver::create(desc);

        // 1. Static ground floor
        SolidDesc floorDesc;
        floorDesc.hull               = Hull::createBox({30.0f, 1.0f, 30.0f});
        floorDesc.motionType         = MotionType::STATIC;
        floorDesc.layer              = CollisionLayer::NON_MOVING;
        floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
        floorDesc.temper.restitution = 0.88f;
        floorDesc.temper.friction    = 0.3f;
        auto floorSolid              = solver->createSolid(floorDesc);
        solids.push_back({floorSolid, RIGID_FLOOR});

        // 2. Boundary walls
        createWall(fiz::Vector3(30.0f, 8.0f, 0.0f), fiz::Vector3(1.0f, 8.0f, 30.0f));
        createWall(fiz::Vector3(-30.0f, 8.0f, 0.0f), fiz::Vector3(1.0f, 8.0f, 30.0f));
        createWall(fiz::Vector3(0.0f, 8.0f, 30.0f), fiz::Vector3(30.0f, 8.0f, 1.0f));
        createWall(fiz::Vector3(0.0f, 8.0f, -30.0f), fiz::Vector3(30.0f, 8.0f, 1.0f));

        if (initialGelCount <= 1) {
            // Single big bouncy gel sphere dropped from mid-air to ground then bouncing up and down
            spawnGel(TEMPLATE_SPHERE_RED, fiz::Vector3(0.0f, 10.0f, 0.0f), 0.95f, 0.2f, 1.0f, 2e-5f, gpu);
        } else {
            // Structured grid for performance scaling benchmark
            for (size_t i = 0; i < initialGelCount; ++i) {
                float        px   = static_cast<float>(static_cast<int>(i % 5) - 2) * 2.8f;
                float        pz   = static_cast<float>(static_cast<int>((i / 5) % 4) - 2) * 2.8f;
                float        py   = 3.5f + static_cast<float>(i / 20) * 3.0f;
                TemplateKind kind = static_cast<TemplateKind>(i % TEMPLATE_COUNT);
                spawnGel(kind, fiz::Vector3(px, py, pz), 0.88f, 0.3f, 1.0f, 0.008f, gpu);
            }
        }
    }

    void spawnGel(TemplateKind kind, const fiz::Vector3 & pos, float restitution, float friction, float density, float edgeCompliance,
                  AutoRef<GpuContext> gpu) {
        bool isCube = (kind == TEMPLATE_CUBE_GREEN || kind == TEMPLATE_CUBE_BLUE);
        auto mesh   = isCube ? meshCube : meshSphere;
        if (!mesh) return;

        GelDesc desc;
        desc.mesh                 = mesh;
        desc.transform.position   = pos;
        desc.temper.restitution   = restitution;
        desc.temper.friction      = friction;
        desc.temper.density       = density;
        desc.temper.linearDamping = 0.002f;
        desc.edgeCompliance       = edgeCompliance;
        desc.volumeCompliance     = 0.0f; // strictly preserve volume
        desc.pressure             = 0.0f; // solid tetrahedral sphere: elasticity from volume and edge constraints
        desc.solverIterations     = 15;
        desc.entityId             = gels.size() + 100;

        auto gel = solver->createGel(desc);
        if (!gel) return;

        SimulatedGel simGel;
        simGel.gel          = gel;
        simGel.templateKind = kind;

        size_t numVerts = mesh->faceVertices().size();
        simGel.cpuVertices.resize(numVerts);

        // Initialize vertex buffer template values (colors, uvs, tangents)
        for (size_t i = 0; i < numVerts; ++i) {
            const auto &       mv = mesh->faceVertices()[i];
            ModelScene::Vertex vert;
            vert.position         = glm::vec3(mv.position.x, mv.position.y, mv.position.z);
            vert.normal           = glm::vec3(0.0f, 1.0f, 0.0f);
            vert.tangent          = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            vert.texcoord         = glm::vec2(0.5f, 0.5f);
            vert.color            = glm::vec4(1.0f);
            simGel.cpuVertices[i] = vert;
        }

        const uint64_t vbBytes = numVerts * sizeof(ModelScene::Vertex);
        for (int b = 0; b < 2; ++b) {
            simGel.dynamicVb[b] = Buffer::create(StrA::format("gel-dyn-vb-{}-{}", gels.size(), b), {.context = gpu, .size = vbBytes});
        }

        gels.push_back(std::move(simGel));
    }

    void launchProjectile(const glm::vec3 & eye, const glm::vec3 & target) {
        glm::vec3 dir = glm::normalize(target - eye);
        SolidDesc desc;
        desc.hull               = Hull::createSphere(1.5f);
        desc.motionType         = MotionType::DYNAMIC;
        desc.layer              = CollisionLayer::MOVING;
        desc.transform.position = {eye.x, eye.y, eye.z};
        desc.linearVelocity     = {dir.x * 50.0f, dir.y * 50.0f, dir.z * 50.0f};
        desc.massOverride       = 120.0f; // Heavy rigid cannonball
        desc.ccd                = true;
        desc.temper.restitution = 0.8f;
        desc.temper.friction    = 0.3f;

        auto solid = solver->createSolid(desc);
        solids.push_back({solid, RIGID_PROJECTILE});
    }

private:
    void createWall(const fiz::Vector3 & pos, const fiz::Vector3 & halfExtents) {
        SolidDesc desc;
        desc.hull               = Hull::createBox(halfExtents);
        desc.motionType         = MotionType::STATIC;
        desc.layer              = CollisionLayer::NON_MOVING;
        desc.transform.position = pos;
        auto wall               = solver->createSolid(desc);
        solids.push_back({wall, RIGID_WALL});
    }
};

static void runBenchmark(GelArena & arena, AutoRef<GpuContext> gpu, SharedShaderConstants * ssc, AutoRef<ModelShading::Asset> modelShading,
                         AutoRef<Swapchain> swapchain, RasterTarget & rasterTarget, const uint32_t hwThreads) {
    GN_INFO(sLogger, "================================================================================");
    GN_INFO(sLogger, "  GNfiz Gel XPBD Soft-Body Dynamics & GPU Rendering Benchmark");
    GN_INFO(sLogger, "  Configuration: Profile Build (RelWithDebInfo) | Hardware Cores: {}", hwThreads);
    GN_INFO(sLogger, "================================================================================");

    // Initial asset uploads
    GpuContext::SubmitParameters initSubmit("bench-init-upload");
    if (const auto p = modelShading->uploadPayload()) initSubmit.appendWork(p);
    for (int k = 0; k < TEMPLATE_COUNT; ++k) {
        if (arena.gelTemplateAssets[k]) {
            if (const auto p = arena.gelTemplateAssets[k]->uploadPayload()) initSubmit.appendWork(p);
        }
    }
    for (int k = 0; k < RIGID_COUNT; ++k) {
        if (arena.rigidAssets[k]) {
            if (const auto p = arena.rigidAssets[k]->uploadPayload()) initSubmit.appendWork(p);
        }
    }
    gpu->submit(initSubmit);
    gpu->waitForIdle();

    // ─────────────────────────────────────────────────────────────────────────
    // Benchmark 1: Worker Thread Scaling (with 20 Gel Soft Bodies)
    // ─────────────────────────────────────────────────────────────────────────
    GN_INFO(sLogger, "\n[BENCHMARK 1] Multi-Threaded Physics Solving Scaling (20 Gel Bodies, 60 Steps)");
    GN_INFO(sLogger, "  {:<12} | {:<16} | {:<12}", "Threads", "Avg Physics (ms)", "Speedup");
    GN_INFO(sLogger, "  -------------+------------------+-------------");

    float          baseline1ThreadMs = 0.0f;
    const uint32_t threadConfigs[]   = {1, 2, 4, 8, 16, 0};

    for (uint32_t tc : threadConfigs) {
        arena.reset(tc, gpu, 20);
        // Warmup 10 steps
        for (int w = 0; w < 10; ++w) arena.solver->step(UnitOfTime(16'666'667));

        float     totalStepMs = 0.0f;
        const int benchSteps  = 60;
        for (int s = 0; s < benchSteps; ++s) {
            auto t0 = std::chrono::high_resolution_clock::now();
            arena.solver->step(UnitOfTime(16'666'667));
            auto t1 = std::chrono::high_resolution_clock::now();
            totalStepMs += std::chrono::duration<float, std::milli>(t1 - t0).count();
        }
        float avgMs = totalStepMs / static_cast<float>(benchSteps);
        if (tc == 1) baseline1ThreadMs = avgMs;

        float       speedup     = (baseline1ThreadMs > 0.0f) ? (baseline1ThreadMs / std::max(0.001f, avgMs)) : 1.0f;
        std::string threadLabel = (tc == 0) ? StrA::format("Auto ({})", hwThreads).data() : std::to_string(tc);
        GN_INFO(sLogger, "  {:<12} | {:>13.3f} ms | {:>10.2f}x", threadLabel, avgMs, speedup);
    }

    // ─────────────────────────────────────────────────────────────────────────
    // Benchmark 2: Workload & Frame Breakdown Scaling (5, 10, 20, 40 Gels)
    // ─────────────────────────────────────────────────────────────────────────
    GN_INFO(sLogger, "\n[BENCHMARK 2] Workload Scaling & End-to-End Frame Breakdown (Auto Threads)");
    GN_INFO(sLogger, "  {:<6} | {:<12} | {:<12} | {:<11} | {:<10} | {:<11} | {:<8}", "Gels", "Physics(ms)", "Deform(ms)", "Upload(ms)", "Draw(ms)", "Total(ms)",
            "FPS");
    GN_INFO(sLogger, "  -------+--------------+--------------+-------------+------------+-------------+---------");

    const size_t       bodyCounts[] = {5, 10, 20, 40};
    FixedBlob<uint8_t> pushConstantPool(sizeof(PushConstants));

    for (size_t numGels : bodyCounts) {
        arena.reset(0, gpu, numGels);
        for (int w = 0; w < 10; ++w) arena.solver->step(UnitOfTime(16'666'667));

        float     sumPhys = 0.0f, sumDeform = 0.0f, sumUpload = 0.0f, sumDraw = 0.0f, sumTotal = 0.0f;
        const int benchFrames = 60;

        for (int f = 0; f < benchFrames; ++f) {
            auto tFrame0 = std::chrono::high_resolution_clock::now();

            // 1. Physics Step
            auto tP0 = std::chrono::high_resolution_clock::now();
            arena.solver->step(UnitOfTime(16'666'667));
            auto tP1 = std::chrono::high_resolution_clock::now();

            // 2. CPU Deform extraction (Blob-based)
            auto     tD0    = std::chrono::high_resolution_clock::now();
            uint32_t bufIdx = f % 2;
            for (auto & g : arena.gels) {
                if (!g.gel) continue;
                auto blob = g.gel->surfaceVertices(true);
                if (!blob) continue;
                auto   deformed = blob->accessor<Gel::SurfaceVertex>();
                size_t vCount   = std::min(deformed.size(), g.cpuVertices.size());
                for (size_t i = 0; i < vCount; ++i) {
                    g.cpuVertices[i].position = glm::vec3(deformed[i].position.x, deformed[i].position.y, deformed[i].position.z);
                    g.cpuVertices[i].normal   = glm::vec3(deformed[i].normal.x, deformed[i].normal.y, deformed[i].normal.z);
                }
            }
            auto tD1 = std::chrono::high_resolution_clock::now();

            // 3. GPU Buffer Streaming
            auto tU0 = std::chrono::high_resolution_clock::now();
            auto cnc = GpuCnC::create({.gpu = gpu});
            for (auto & g : arena.gels) {
                if (!g.gel) continue;
                const uint64_t vbBytes = g.cpuVertices.size() * sizeof(ModelScene::Vertex);
                cnc->uploadBuffer(g.dynamicVb[bufIdx], 0, ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(g.cpuVertices.data()), vbBytes));
            }
            auto uploadPayload = cnc->seal();
            auto tU1           = std::chrono::high_resolution_clock::now();

            // 4. GPU Raster Pass
            auto             tR0   = std::chrono::high_resolution_clock::now();
            Swapchain::Frame frame = swapchain->prepare();
            rasterTarget.setColorTarget(0, frame.view);
            SharedShaderConstants::Snapshot sscSnapshot = updateSsc(ssc, rasterTarget, glm::vec3(0, 10, 25), glm::vec3(0, 3, 0), f);

            DynaArray<AutoRef<GpuPayload>> renderWorks;
            renderWorks.append(sscSnapshot.set0Payloads);
            if (uploadPayload) renderWorks.append(uploadPayload);

            GpuRaster::DrawParameters gelBaseDraws[TEMPLATE_COUNT];
            for (int k = 0; k < TEMPLATE_COUNT; ++k) {
                if (arena.gelTemplateAssets[k]) {
                    gelBaseDraws[k] = ModelShading::getDrawParams(sscSnapshot, modelShading, arena.gelTemplateAssets[k], 0, glm::mat4(1.0f));
                }
            }

            GpuRaster::CreateParameters rcp;
            rcp.gpu    = gpu;
            rcp.target = &rasterTarget;
            auto r     = GpuRaster::create("bench-raster", rcp);
            if (r) {
                for (const auto & g : arena.gels) {
                    if (!g.gel) continue;
                    auto draw = gelBaseDraws[g.templateKind];
                    if (!draw.vs || !draw.ps) continue;
                    const PushConstants constants {glm::mat4(1.0f), glm::mat4(1.0f)};
                    draw.immediates                  = pushConstantPool.allocate(reinterpret_cast<const uint8_t *>(&constants));
                    draw.geometry.vertices[0].buffer = g.dynamicVb[bufIdx];
                    draw.geometry.vertices[0].stride = sizeof(ModelScene::Vertex);
                    draw.geometry.vertexCount        = static_cast<uint32_t>(g.cpuVertices.size());
                    r->draw(draw);
                }
                renderWorks.append(r->seal());
            }

            GpuContext::SubmitParameters submit(StrA::format("bench frame {}", f));
            for (size_t i = 0; i < renderWorks.size(); ++i) {
                if (!renderWorks[i]) continue;
                if (i + 1 == renderWorks.size())
                    submit.appendWork(renderWorks[i]).waitFor(frame.ready);
                else
                    submit.appendWork(renderWorks[i]);
            }
            gpu->submit(submit);
            if (!renderWorks.empty() && renderWorks.back()) swapchain->present(*renderWorks.back());
            auto tR1 = std::chrono::high_resolution_clock::now();

            auto tFrame1 = std::chrono::high_resolution_clock::now();

            sumPhys += std::chrono::duration<float, std::milli>(tP1 - tP0).count();
            sumDeform += std::chrono::duration<float, std::milli>(tD1 - tD0).count();
            sumUpload += std::chrono::duration<float, std::milli>(tU1 - tU0).count();
            sumDraw += std::chrono::duration<float, std::milli>(tR1 - tR0).count();
            sumTotal += std::chrono::duration<float, std::milli>(tFrame1 - tFrame0).count();
        }

        float n    = static_cast<float>(benchFrames);
        float avgP = sumPhys / n, avgD = sumDeform / n, avgU = sumUpload / n, avgR = sumDraw / n, avgTot = sumTotal / n;
        float fps = (avgTot > 0.0f) ? (1000.0f / avgTot) : 0.0f;

        GN_INFO(sLogger, "  {:<6} | {:>9.2f} ms | {:>9.2f} ms | {:>8.2f} ms | {:>7.2f} ms | {:>8.2f} ms | {:>7.0f}", numGels, avgP, avgD, avgU, avgR, avgTot,
                fps);
    }

    // ─────────────────────────────────────────────────────────────────────────
    // Benchmark 3: Deformed Vertex Blob Extraction
    // ─────────────────────────────────────────────────────────────────────────
    GN_INFO(sLogger, "\n[BENCHMARK 3] Deformed Vertex Blob Extraction (40 Gels, 100 Iterations)");
    arena.reset(0, gpu, 40);
    arena.solver->step(UnitOfTime(16'666'667));

    auto tGeom0 = std::chrono::high_resolution_clock::now();
    for (int it = 0; it < 100; ++it) {
        for (auto & g : arena.gels) {
            auto b = g.gel->surfaceVertices(true);
            (void) b;
        }
    }
    auto  tGeom1 = std::chrono::high_resolution_clock::now();
    float geomMs = std::chrono::duration<float, std::milli>(tGeom1 - tGeom0).count() / 100.0f;

    GN_INFO(sLogger, "  Deformed Geometry (surfaceVertices Blob): {:.3f} ms / frame", geomMs);
    GN_INFO(sLogger, "================================================================================\n");
}

} // namespace

int main(int argc, const char ** argv) {
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::setvbuf(stderr, nullptr, _IONBF, 0);

    bool testMode      = false;
    bool benchmarkMode = false;
    int  framesArg     = 0;
    bool vsync         = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "test") == 0 || argv[i][0] == 't') {
            testMode = true;
        } else if (std::strcmp(argv[i], "benchmark") == 0 || std::strcmp(argv[i], "bench") == 0 || argv[i][0] == 'b') {
            benchmarkMode = true;
        } else if (std::strcmp(argv[i], "--vsync") == 0 || std::strcmp(argv[i], "-v") == 0) {
            vsync = true;
        } else {
            framesArg = std::atoi(argv[i]);
        }
    }

    if (testMode) {
        GN_INFO(sLogger, "Running GNsample-fiz-gel in test mode (headless verification)");
    } else if (benchmarkMode) {
        GN_INFO(sLogger, "Running GNsample-fiz-gel in benchmark mode (multi-thread and workload profiling)");
    } else {
        GN_INFO(sLogger,
                "Interactive visual mode (vsync {}): Press ESC to quit, [1,2,4,8,0] for threads, [B] spawn gels, [Space] launch "
                "cannonball, [R] reset, [A/D/W/S/Q/E] camera",
                vsync ? "on" : "off");
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

    // Initialize Gel and Rigid assets
    GelArena arena;
    arena.initMeshesAndAssets(gpuContext);

    // ─── Window & Swapchain ──────────────────────────────────────────────────
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        window.reset(
            win::createWindow(win::WindowCreateParameters {.caption = "Garnet Fiz Gel [XPBD Volumetric Soft Body Demo]", .clientWidth = W, .clientHeight = H}));
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

    bool initialUploadsSubmitted = false;

    const uint32_t hwThreads = std::max(1u, std::thread::hardware_concurrency());

    if (benchmarkMode) {
        runBenchmark(arena, gpuContext, ssc.get(), modelShading, swapchain, rasterTarget, hwThreads);
        gpuContext->waitForIdle();
        swapchain.clear();
        if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);
        return 0;
    }

    // ─── Physics Simulation Setup ────────────────────────────────────────────
    arena.reset(0, gpuContext); // Auto-detected threads

    GN_INFO(sLogger, "Gel physics initialized with {} soft bodies across {} hardware threads", arena.gels.size(), hwThreads);

    float currentStepMs = 0.0f;
    float avgStepMs     = 0.0f;
    float avgDeformMs   = 0.0f;
    float avgUploadMs   = 0.0f;
    float avgDrawMs     = 0.0f;
    float avgFps        = 0.0f;

    // Camera control
    float     orbitAngle   = 0.0f;
    float     cameraDist   = 16.0f;
    float     cameraHeight = 6.0f;
    glm::vec3 cameraCenter(0.0f, 4.5f, 0.0f);

    FixedBlob<uint8_t> pushConstantPool(sizeof(PushConstants));

    // Headless test verification state
    bool  observedFreefall = false;
    bool  observedImpact   = false;
    bool  observedRebound  = false;
    bool  volumeMaintained = true;
    float initialY         = 0.0f;
    float minObservedY     = 1e9f;

    if (!arena.gels.empty()) { initialY = arena.gels[0].gel->position().y; }

    // ─── Main Render & Simulation Loop ───────────────────────────────────────
    auto      lastTime    = std::chrono::high_resolution_clock::now();
    int       frameIdx    = 0;
    const int totalFrames = testMode ? 180 : framesArg;

    while (totalFrames == 0 || frameIdx < totalFrames) {
        ++frameIdx;
        if (window && !window->runUntilNoNewEvents()) break;

        // Input Handling
        if (window) {
            if (window->getKeyStatus(win::KeyCode::ESCAPE).down) break;

            // Worker thread switching
            if (window->getKeyStatus(win::KeyCode::_1).down && arena.configuredThreads != 1) {
                arena.reset(1, gpuContext, 1);
                GN_INFO(sLogger, "Switched Gel solver to 1 worker thread");
            } else if (window->getKeyStatus(win::KeyCode::_2).down && arena.configuredThreads != 2) {
                arena.reset(2, gpuContext, 1);
                GN_INFO(sLogger, "Switched Gel solver to 2 worker threads");
            } else if (window->getKeyStatus(win::KeyCode::_4).down && arena.configuredThreads != 4) {
                arena.reset(4, gpuContext, 1);
                GN_INFO(sLogger, "Switched Gel solver to 4 worker threads");
            } else if (window->getKeyStatus(win::KeyCode::_8).down && arena.configuredThreads != 8) {
                arena.reset(8, gpuContext, 1);
                GN_INFO(sLogger, "Switched Gel solver to 8 worker threads");
            } else if (window->getKeyStatus(win::KeyCode::_0).down && arena.configuredThreads != 0) {
                arena.reset(0, gpuContext, 1);
                GN_INFO(sLogger, "Switched Gel solver to auto-detected threads ({} cores)", hwThreads);
            }

            // [B] = Bounce booster impulse
            static bool bWasDown = false;
            bool        bDown    = window->getKeyStatus(win::KeyCode::B).down;
            if (bDown && !bWasDown) {
                if (!arena.gels.empty()) {
                    arena.gels[0].gel->applyImpulse({0.0f, 100.0f, 0.0f});
                    GN_INFO(sLogger, "Boosted bouncy ball upward!");
                }
            }
            bWasDown = bDown;

            // [Space] = Re-drop the bouncy ball from mid-air
            static bool spaceWasDown = false;
            bool        spaceDown    = window->getKeyStatus(win::KeyCode::SPACEBAR).down;
            if (spaceDown && !spaceWasDown) {
                if (!arena.gels.empty()) {
                    Transform t;
                    t.position = {0.0f, 10.0f, 0.0f};
                    arena.gels[0].gel->setTransform(t);
                    arena.gels[0].gel->setLinearVelocity({0.0f, 0.0f, 0.0f});
                    GN_INFO(sLogger, "Re-dropped bouncy ball from mid air (Y = 10.0m)!");
                }
            }
            spaceWasDown = spaceDown;

            // [R] = Reset arena
            static bool rWasDown = false;
            bool        rDown    = window->getKeyStatus(win::KeyCode::R).down;
            if (rDown && !rWasDown) {
                arena.reset(arena.configuredThreads, gpuContext, 1);
                GN_INFO(sLogger, "Reset bouncy ball arena");
            }
            rWasDown = rDown;

            // Camera orbit and zoom
            if (window->getKeyStatus(win::KeyCode::LEFT).down || window->getKeyStatus(win::KeyCode::A).down) orbitAngle -= 0.02f;
            if (window->getKeyStatus(win::KeyCode::RIGHT).down || window->getKeyStatus(win::KeyCode::D).down) orbitAngle += 0.02f;
            if (window->getKeyStatus(win::KeyCode::UP).down || window->getKeyStatus(win::KeyCode::W).down) cameraHeight = std::min(35.0f, cameraHeight + 0.3f);
            if (window->getKeyStatus(win::KeyCode::DOWN).down || window->getKeyStatus(win::KeyCode::S).down) cameraHeight = std::max(1.0f, cameraHeight - 0.3f);
            if (window->getKeyStatus(win::KeyCode::Q).down) cameraDist = std::min(60.0f, cameraDist + 0.4f);
            if (window->getKeyStatus(win::KeyCode::E).down) cameraDist = std::max(5.0f, cameraDist - 0.4f);
        } else {
            // Auto orbit in test mode
            orbitAngle += 0.01f;
        }

        // ─── Step Physics with High-Precision Timing ─────────────────────────
        const auto stepStart = std::chrono::high_resolution_clock::now();
        arena.solver->step(UnitOfTime(16'666'667)); // 60 Hz integer step
        const auto stepEnd = std::chrono::high_resolution_clock::now();

        currentStepMs = std::chrono::duration<float, std::milli>(stepEnd - stepStart).count();
        avgStepMs     = (avgStepMs == 0.0f) ? currentStepMs : (avgStepMs * 0.9f + currentStepMs * 0.1f);

        // Frame rate calculation
        auto  now        = std::chrono::high_resolution_clock::now();
        float frameDt    = std::chrono::duration<float>(now - lastTime).count();
        lastTime         = now;
        float instantFps = frameDt > 0.0f ? 1.0f / frameDt : 60.0f;
        avgFps           = (avgFps == 0.0f) ? instantFps : (avgFps * 0.95f + instantFps * 0.05f);

        // Verification checks on Gel 0 in headless test mode
        if (!arena.gels.empty()) {
            const auto & g0    = arena.gels[0];
            float        curY  = g0.gel->position().y;
            float        restV = g0.gel->restVolume();
            float        curV  = g0.gel->currentVolume();

            minObservedY = std::min(minObservedY, curY);

            if (curY < initialY - 1.5f) { observedFreefall = true; }
            if (curY < 1.98f) { observedImpact = true; } // Radius is 2.0m; contact at Y = 2.0m; Y < 1.98m indicates ground compression
            if (observedImpact && curY > minObservedY + 0.8f) { observedRebound = true; }

            if (testMode && (frameIdx % 10 == 0 || frameIdx == 1 || frameIdx == 77 || frameIdx == 84)) {
                GN_INFO(sLogger, "Frame {}: curY = {:.3f}, curV = {:.3f}, restV = {:.3f}", frameIdx, curY, curV, restV);
            }

            // Verify numerical stability: soft body remains stable without explosive blowout or collapse
            if (restV > 0.0f) {
                float volRatio = curV / restV;
                if (volRatio > 3.0f || volRatio < 0.05f) {
                    GN_WARN(sLogger, "Frame {}: Gel volume ratio {:.2f} out of bounds (rest: {:.4f}, cur: {:.4f})", frameIdx, volRatio, restV, curV);
                    volumeMaintained = false;
                }
            }
        }

        // Window title telemetry
        if (window && frameIdx % 10 == 0) {
            float curY = 0.0f, squashPct = 0.0f, volErrPct = 0.0f;
            if (!arena.gels.empty()) {
                curY             = arena.gels[0].gel->position().y;
                float restRadius = 2.0f;
                squashPct        = std::max(0.0f, (restRadius - curY) / restRadius * 100.0f);
                float curVol     = arena.gels[0].gel->currentVolume();
                float restVol    = arena.gels[0].gel->restVolume();
                if (restVol > 0.0f) { volErrPct = std::abs(curVol - restVol) / restVol * 100.0f; }
            }

            std::string title =
                StrA::format("Garnet Fiz Gel — Bouncy Ball | Height: {:.2f} m | Squash: {:.1f}% | Vol Err: {:.2f}% | Phys: {:.2f} ms | FPS: {:.0f}", curY,
                             squashPct, volErrPct, avgStepMs, avgFps)
                    .data();

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

        // 1. CPU Dynamic extraction of deformed gel geometry via Blob
        auto     tDeform0 = std::chrono::high_resolution_clock::now();
        uint32_t bufIdx   = frameIdx % 2; // Double buffering
        for (auto & g : arena.gels) {
            if (!g.gel) continue;
            auto blob = g.gel->surfaceVertices(true);
            if (!blob) continue;
            auto   deformed = blob->accessor<Gel::SurfaceVertex>();
            size_t vCount   = std::min(deformed.size(), g.cpuVertices.size());

            fiz::Vector3 com        = g.gel->position();
            float        restRadius = 2.0f;

            for (size_t i = 0; i < vCount; ++i) {
                const auto & dv           = deformed[i];
                g.cpuVertices[i].position = glm::vec3(dv.position.x, dv.position.y, dv.position.z);
                g.cpuVertices[i].normal   = glm::vec3(dv.normal.x, dv.normal.y, dv.normal.z);

                // Dynamic strain & compression glow:
                // When compressed flat or bulging laterally, local vertex distance from center deviates from rest radius
                float dist   = glm::length(g.cpuVertices[i].position - glm::vec3(com.x, com.y, com.z));
                float strain = std::abs(dist - restRadius) / restRadius;
                float t      = std::min(1.0f, strain * 2.5f);

                // Rest: vibrant ruby-red jelly; Impact strain: bright glowing yellow shockwave
                glm::vec4 restColor(0.95f, 0.22f, 0.35f, 1.0f);
                glm::vec4 stressColor(1.0f, 0.96f, 0.25f, 1.0f);
                g.cpuVertices[i].color = glm::mix(restColor, stressColor, t);
            }
        }
        auto  tDeform1 = std::chrono::high_resolution_clock::now();
        float deformMs = std::chrono::duration<float, std::milli>(tDeform1 - tDeform0).count();
        avgDeformMs    = (avgDeformMs == 0.0f) ? deformMs : (avgDeformMs * 0.95f + deformMs * 0.05f);

        // 2. GPU Dynamic streaming upload into vertex buffer
        auto tUpload0 = std::chrono::high_resolution_clock::now();
        auto cnc      = GpuCnC::create({.gpu = gpuContext});
        for (auto & g : arena.gels) {
            if (!g.gel) continue;
            const uint64_t vbBytes = g.cpuVertices.size() * sizeof(ModelScene::Vertex);
            cnc->uploadBuffer(g.dynamicVb[bufIdx], 0, ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(g.cpuVertices.data()), vbBytes));
        }

        auto uploadPayload = cnc->seal();
        if (uploadPayload) { renderWorks.append(uploadPayload); }
        auto  tUpload1 = std::chrono::high_resolution_clock::now();
        float uploadMs = std::chrono::duration<float, std::milli>(tUpload1 - tUpload0).count();
        avgUploadMs    = (avgUploadMs == 0.0f) ? uploadMs : (avgUploadMs * 0.95f + uploadMs * 0.05f);

        // Pre-create base draw parameters
        GpuRaster::DrawParameters gelBaseDraws[TEMPLATE_COUNT];
        for (int k = 0; k < TEMPLATE_COUNT; ++k) {
            if (arena.gelTemplateAssets[k]) {
                gelBaseDraws[k] = ModelShading::getDrawParams(sscSnapshot, modelShading, arena.gelTemplateAssets[k], 0, glm::mat4(1.0f));
            }
        }

        GpuRaster::DrawParameters rigidBaseDraws[RIGID_COUNT];
        for (int k = 0; k < RIGID_COUNT; ++k) {
            if (arena.rigidAssets[k]) { rigidBaseDraws[k] = ModelShading::getDrawParams(sscSnapshot, modelShading, arena.rigidAssets[k], 0, glm::mat4(1.0f)); }
        }

        auto                        tRaster0 = std::chrono::high_resolution_clock::now();
        GpuRaster::CreateParameters rcp;
        rcp.gpu    = gpuContext;
        rcp.target = &rasterTarget;
        auto r     = GpuRaster::create("fiz-gel-raster", rcp);
        if (r) {
            // Draw rigid solids
            for (const auto & e : arena.solids) {
                if (!e.solid || e.kind == RIGID_WALL) continue; // Boundary walls are invisible collision barriers
                auto & draw = rigidBaseDraws[e.kind];
                if (!draw.vs || !draw.ps) continue;

                Transform t              = e.solid->transform();
                glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), glm::vec3(t.position.x, t.position.y, t.position.z)) *
                                           glm::mat4_cast(glm::quat(t.orientation.w, t.orientation.v.x, t.orientation.v.y, t.orientation.v.z));

                const glm::mat4     normalTransform = glm::transpose(glm::inverse(worldTransform));
                const PushConstants constants {worldTransform, normalTransform};

                draw.immediates = pushConstantPool.allocate(reinterpret_cast<const uint8_t *>(&constants));
                r->draw(draw);
            }

            // Draw deformable soft body gels
            for (const auto & g : arena.gels) {
                if (!g.gel) continue;
                auto draw = gelBaseDraws[g.templateKind];
                if (!draw.vs || !draw.ps) continue;

                // Deformed vertices are in world coordinates; world transform is identity
                const PushConstants constants {glm::mat4(1.0f), glm::mat4(1.0f)};
                draw.immediates                  = pushConstantPool.allocate(reinterpret_cast<const uint8_t *>(&constants));
                draw.geometry.vertices[0].buffer = g.dynamicVb[bufIdx];
                draw.geometry.vertices[0].offset = 0;
                draw.geometry.vertices[0].stride = sizeof(ModelScene::Vertex);
                draw.geometry.vertexCount        = static_cast<uint32_t>(g.cpuVertices.size());

                r->draw(draw);
            }

            // Skybox
            r->draw(ssc->getSkyboxDrawParams(sscSnapshot.set0Resources));
            renderWorks.append(r->seal());
        }

        auto  tRaster1 = std::chrono::high_resolution_clock::now();
        float drawMs   = std::chrono::duration<float, std::milli>(tRaster1 - tRaster0).count();
        avgDrawMs      = (avgDrawMs == 0.0f) ? drawMs : (avgDrawMs * 0.95f + drawMs * 0.05f);

        // Submit GPU work
        GpuContext::SubmitParameters submit(StrA::format("frame {}", frameIdx));
        if (!initialUploadsSubmitted) {
            if (const auto p = modelShading->uploadPayload()) submit.appendWork(p);
            for (int k = 0; k < TEMPLATE_COUNT; ++k) {
                if (const auto p = arena.gelTemplateAssets[k]->uploadPayload()) submit.appendWork(p);
            }
            for (int k = 0; k < RIGID_COUNT; ++k) {
                if (const auto p = arena.rigidAssets[k]->uploadPayload()) submit.appendWork(p);
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

    if (testMode) {
        GN_INFO(sLogger,
                "Test mode summary: {} steps simulated, avg physics time: {:.2f} ms, avg draw time: {:.2f} ms, freefall: {}, impact: {}, rebound: {}, "
                "volume stable: {}",
                frameIdx, avgStepMs, avgDrawMs, observedFreefall ? "YES" : "NO", observedImpact ? "YES" : "NO", observedRebound ? "YES" : "NO",
                volumeMaintained ? "YES" : "NO");

        if (!observedFreefall || !observedImpact || !observedRebound || !volumeMaintained) {
            GN_ERROR(sLogger, "Headless gel verification failed!");
            return 1;
        }
        GN_INFO(sLogger, "Headless gel verification PASSED cleanly!");
    }

    gpuContext->waitForIdle();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);

    return 0;
}

// draw-mesh2.cpp — RDG v2 port of draw-mesh.cpp
//
// Renders the DamagedHelmet GLTF mesh with PBR shading using the v2 open-graph
// API (GNrdg2.h / GNgpu2.h). The v1 pipeline builders (PbrShading, SkyBox) are
// replaced by explicit GpuRaster draw calls driven by pre-compiled SPIR-V shaders.
//
// Missing pieces assumed to be implemented:
//   - pbr-mesh-vert.spv.h / pbr-mesh-frag.spv.h (shader build step)
//   - SharedShaderConstants::EnvironmentLightingConstants uses gpu2::Texture
//     (current alias still points at rdg::Texture from the v1 header)

#include <garnet/GNrdg2.h>
#include <garnet/GNwin.h>
#include <garnet/base/filesys.h>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

// Pre-compiled SPIR-V for the PBR mesh shaders. These shaders are expected to
// be authored alongside this sample with descriptor-set layouts that match
// SharedShaderConstants: set 0 = SSC resources, set 1 = material textures.
#include "pbr-mesh-vert.spv.h"
#include "pbr-mesh-frag.spv.h"

// Skybox shaders: same SPIR-V as the v1 rdg pipeline, reused directly.
// set 0 only (camera + IBL cubemap from SSC); no vertex buffer needed.
#include "skybox-vert.spv.h"
#include "skybox-frag.spv.h"

using namespace GN;
using namespace GN::rdg2;
using namespace GN::gpu2;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.draw-mesh2");

// ─── Push constants ────────────────────────────────────────────────────────────
//
// Passed via GpuRaster::DrawParameters::immediates (128 bytes total).
// The PBR vertex shader reads these at push-constant offset 0.
struct ModelPushConstants {
    glm::mat4 worldTransform;  // model → world matrix
    glm::mat4 normalTransform; // transpose(inverse(worldTransform)) for normals
};

// ─── Helpers ──────────────────────────────────────────────────────────────────

static GpuResourceView makeTextureView(AutoRef<Texture> tex) {
    GpuResourceView v;
    v.resource       = tex;
    v.imageView.type = GpuResourceView::ImageView::SAMPLED;
    return v;
}

// ─── GLTF loader (gpu2 / v2 types) ───────────────────────────────────────────
//
// Produces a gpu2::RasterGeometry instead of the rdg::GpuDraw::GpuGeometry
// returned by gltf-loader.h. Handles indexed meshes with float POSITION,
// NORMAL, TEXCOORD_0 attributes and uint16 indices (same subset as v1).
//
// Vertex layout: [ position(F32×3) | normal(F32×3) | texcoord(F32×2) ] = 32 B
//
// Buffers are created as CPU-mappable for simplicity (adequate for a demo;
// a production implementation would use a staging buffer + device-local buffer).

namespace {

// ---- Minimal JSON parser (identical to gltf-loader.h) ----------------------

struct Json {
    enum class Type { Null, Bool, Num, Str, Arr, Obj };
    Type                     type = Type::Null;
    double                   num  = 0;
    std::string              str;
    std::vector<Json>        arr;
    std::vector<std::string> objKeys;
    std::vector<Json>        objVals;
    int                      asInt() const { return static_cast<int>(num); }
    size_t                   size() const { return arr.size(); }
    bool                     has(const std::string & k) const {
        for (const auto & key : objKeys)
            if (key == k) return true;
        return false;
    }
    const Json & operator[](const std::string & k) const {
        for (size_t i = 0; i < objKeys.size(); ++i)
            if (objKeys[i] == k) return objVals[i];
        throw std::out_of_range("[gltf-loader2] key not found: " + k);
    }
    const Json & operator[](size_t i) const { return arr[i]; }
};

static void skipWs(const char *& p) {
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p;
}

static Json parseValue(const char *& p) {
    skipWs(p);
    Json v;
    if (*p == '{') {
        ++p;
        v.type = Json::Type::Obj;
        for (;;) {
            skipWs(p);
            if (*p == '}') {
                ++p;
                break;
            }
            if (*p == ',') {
                ++p;
                continue;
            }
            Json key = parseValue(p);
            skipWs(p);
            ++p; // ':'
            v.objKeys.push_back(key.str);
            v.objVals.push_back(parseValue(p));
        }
    } else if (*p == '[') {
        ++p;
        v.type = Json::Type::Arr;
        for (;;) {
            skipWs(p);
            if (*p == ']') {
                ++p;
                break;
            }
            if (*p == ',') {
                ++p;
                continue;
            }
            v.arr.push_back(parseValue(p));
        }
    } else if (*p == '"') {
        ++p;
        v.type = Json::Type::Str;
        while (*p != '"') {
            if (*p == '\\') ++p;
            v.str += *p++;
        }
        ++p;
    } else if (*p == 't') {
        p += 4;
        v.type = Json::Type::Bool;
        v.num  = 1;
    } else if (*p == 'f') {
        p += 5;
        v.type = Json::Type::Bool;
    } else if (*p == 'n') {
        p += 4;
    } else {
        v.type = Json::Type::Num;
        char * end;
        v.num = std::strtod(p, &end);
        p     = end;
    }
    return v;
}

// ---- Geometry loader --------------------------------------------------------

static RasterGeometry loadGltfGeometryV2(const char * gltfPath, AutoRef<GpuContext> gpu) {
    // Parse GLTF JSON
    std::ifstream jf(gltfPath);
    if (!jf) {
        GN_ERROR(sLogger)("Cannot open GLTF: {}", gltfPath);
        return {};
    }
    std::string  text((std::istreambuf_iterator<char>(jf)), {});
    const char * p    = text.c_str();
    const Json   root = parseValue(p);

    // Read companion .bin
    std::string   dir     = std::string(gltfPath).substr(0, std::string(gltfPath).find_last_of("/\\") + 1);
    std::string   binPath = dir + root["buffers"][size_t(0)]["uri"].str;
    std::ifstream bf(binPath, std::ios::binary);
    if (!bf) {
        GN_ERROR(sLogger)("Cannot open bin: {}", binPath);
        return {};
    }
    std::vector<uint8_t> bin((std::istreambuf_iterator<char>(bf)), {});

    // Buffer views
    struct BV {
        int byteOffset, byteLength, byteStride;
    };
    std::vector<BV> bvs;
    for (size_t i = 0; i < root["bufferViews"].size(); ++i) {
        const Json & b = root["bufferViews"][i];
        bvs.push_back({b.has("byteOffset") ? b["byteOffset"].asInt() : 0, b["byteLength"].asInt(), b.has("byteStride") ? b["byteStride"].asInt() : 0});
    }

    // Accessors
    struct Acc {
        int         bv, byteOffset, count, compType;
        std::string type;
    };
    std::vector<Acc> accs;
    for (size_t i = 0; i < root["accessors"].size(); ++i) {
        const Json & a = root["accessors"][i];
        accs.push_back(
            {a["bufferView"].asInt(), a.has("byteOffset") ? a["byteOffset"].asInt() : 0, a["count"].asInt(), a["componentType"].asInt(), a["type"].str});
    }

    const Json & prim    = root["meshes"][size_t(0)]["primitives"][size_t(0)];
    const Json & attrs   = prim["attributes"];
    int          idxAcc  = prim["indices"].asInt();
    int          posAcc  = attrs["POSITION"].asInt();
    int          normAcc = attrs["NORMAL"].asInt();
    int          uvAcc   = attrs["TEXCOORD_0"].asInt();

    auto readF32 = [&](const Acc & ac, int elem, int comp) -> float {
        const BV & bv     = bvs[ac.bv];
        int        comps  = (ac.type == "VEC3") ? 3 : (ac.type == "VEC2") ? 2 : 1;
        int        stride = bv.byteStride ? bv.byteStride : comps * 4;
        int        off    = bv.byteOffset + ac.byteOffset + elem * stride + comp * 4;
        float      val;
        std::memcpy(&val, bin.data() + off, 4);
        return val;
    };

    // Interleaved vertex: position(F32×3) + normal(F32×3) + uv(F32×2) = 32 bytes
    struct Vtx {
        float px, py, pz, nx, ny, nz, u, v;
    };
    int              vertCount = accs[posAcc].count;
    std::vector<Vtx> verts(vertCount);
    for (int i = 0; i < vertCount; ++i) {
        const Acc &pa = accs[posAcc], &na = accs[normAcc], &ua = accs[uvAcc];
        verts[i] = {readF32(pa, i, 0), readF32(pa, i, 1), readF32(pa, i, 2), readF32(na, i, 0),
                    readF32(na, i, 1), readF32(na, i, 2), readF32(ua, i, 0), readF32(ua, i, 1)};
    }

    // uint16 index buffer
    const Acc &           ia       = accs[idxAcc];
    const BV &            ibv      = bvs[ia.bv];
    int                   idxCount = ia.count;
    std::vector<uint16_t> idxData(idxCount);
    std::memcpy(idxData.data(), bin.data() + ibv.byteOffset + ia.byteOffset, idxCount * 2u);

    // Upload to device-local gpu2::Buffer via setContent() (internal staging buffer).
    const uint64_t vertexBytes = (uint64_t) (vertCount * (int) sizeof(Vtx));
    const uint64_t indexBytes  = (uint64_t) (idxCount * 2);
    auto           vbuf        = Buffer::create("helmet_vb", Buffer::CreateParameters {.context = gpu, .size = vertexBytes});
    auto           ibuf        = Buffer::create("helmet_ib", Buffer::CreateParameters {.context = gpu, .size = indexBytes});
    if (!vbuf || !ibuf) {
        GN_ERROR(sLogger)("Failed to create geometry buffers");
        return {};
    }
    if (!vbuf->setContent({reinterpret_cast<const uint8_t *>(verts.data()), vertexBytes})) {
        GN_ERROR(sLogger)("Failed to upload vertex buffer");
        return {};
    }
    if (!ibuf->setContent({reinterpret_cast<const uint8_t *>(idxData.data()), indexBytes})) {
        GN_ERROR(sLogger)("Failed to upload index buffer");
        return {};
    }

    using AF = RasterGeometry::AttributeFormat;
    RasterGeometry geom;
    geom.format.attributes.append(RasterGeometry::VertexAttribute {0, 0, AF::F32_3});  // position
    geom.format.attributes.append(RasterGeometry::VertexAttribute {1, 12, AF::F32_3}); // normal
    geom.format.attributes.append(RasterGeometry::VertexAttribute {2, 24, AF::F32_2}); // texcoord
    geom.vertices.append(RasterGeometry::GeometryBuffer {.buffer = vbuf, .offset = 0, .stride = sizeof(Vtx)});
    geom.vertexCount = (uint32_t) vertCount;
    geom.indices     = {.buffer = ibuf, .offset = 0, .stride = 2};
    geom.indexCount  = (uint32_t) idxCount;
    return geom;
}

} // anonymous namespace

// ─── main ────────────────────────────────────────────────────────────────────

int main(int argc, const char ** argv) {
    bool testMode = (argc > 1) && (argv[1][0] == 't');
    if (testMode) GN_INFO(sLogger)("Running in test mode");

    enableCRTMemoryCheck();

    // ── GPU context ───────────────────────────────────────────────────────────
    auto gpuContext = GpuContext::create("gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    // ── Window + Vulkan surface ───────────────────────────────────────────────
    uint32_t                     windowWidth  = 1280;
    uint32_t                     windowHeight = 720;
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        window = std::unique_ptr<win::Window>(
            win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - Draw Mesh (v2)", .clientWidth = windowWidth, .clientHeight = windowHeight}));
        if (!window) return -1;
        window->show();
        // Query actual client size: on macOS with content scaling (Retina) the
        // backing pixel dimensions may differ from the logical size used at creation.
        auto clientSize = window->getClientSize();
        windowWidth     = clientSize.x;
        windowHeight    = clientSize.y;
        surface         = window->getVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
        if (!surface) return -1;
    }

    // ── Open graph ────────────────────────────────────────────────────────────
    auto graph = Graph::create();
    if (!graph) return -1;

    // ── Shared shader constants ───────────────────────────────────────────────
    auto ssc = SharedShaderConstants::create({gpuContext, graph});
    if (!ssc) return -1;

    // ── Swapchain ─────────────────────────────────────────────────────────────
    Swapchain::CreateDesc scDesc {.gpu = gpuContext, .width = windowWidth, .height = windowHeight};
    if (surface) scDesc.setWindow(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) return -1;

    // ── Depth texture (fixed resolution for now) ──────────────────────────────
    gfx::img::PixelFormat depthFormat = gpuContext->caps().defaultDepthFormat;
    if (depthFormat == gfx::img::PixelFormat::UNKNOWN()) {
        GN_WARN(sLogger)("defaultDepthFormat is UNKNOWN; falling back to D24_UNORM_S8_UINT");
        depthFormat = gfx::img::PixelFormat::RG_24_UNORM_8_UINT();
    }
    auto depthTex = Texture::create(
        "depth_tex", Texture::CreateParameters {.context    = gpuContext,
                                                .descriptor = Texture::Descriptor {}.setFormat(depthFormat).setDimensions(windowWidth, windowHeight)});
    if (!depthTex) return -1;

    // ── PBR material textures ─────────────────────────────────────────────────
    // The DamagedHelmet material uses five texture maps. Missing files fall back
    // to a 1×1 default inside the PBR shader (white albedo, up-pointing normal, etc.).
    static constexpr const char * kMeshDir = "media::pbr/DamagedHelmet/";
    auto                          loadTex  = [&](const char * relPath) -> AutoRef<Texture> {
        auto fullPath = StrA(kMeshDir) + relPath;
        auto tex      = Texture::load(Texture::LoadParameters {.context = gpuContext, .filename = fs::toNativeDiskFilePath(fullPath.c_str())});
        if (!tex) GN_WARN(sLogger)("Texture not found: {}", fullPath);
        return tex;
    };
    auto albedoTex     = loadTex("albedo.dds");
    auto normalTex     = loadTex("normal.dds");
    auto emissiveTex   = loadTex("emissive.dds");
    auto occlusionTex  = loadTex("occlusion.dds");
    auto metalRoughTex = loadTex("metallic_roughness.dds");

    // ── IBL / environment textures ────────────────────────────────────────────
    static constexpr const char * kEnvDir    = "media::envmap/bad-salzbrunn-walking-hall/";
    auto                          loadEnvTex = [&](const char * name) -> AutoRef<Texture> {
        auto path = StrA(kEnvDir) + name;
        auto tex  = Texture::load(Texture::LoadParameters {.context = gpuContext, .filename = fs::toNativeDiskFilePath(path.c_str())});
        if (!tex) GN_WARN(sLogger)("IBL texture not found: {}", path);
        return tex;
    };
    auto skyboxCubemap     = loadEnvTex("skybox.dds");
    auto irradianceMap     = loadEnvTex("irradiance.dds");
    auto prefilteredEnvMap = loadEnvTex("prefiltered.dds");
    auto brdfLut           = loadEnvTex("brdf_lut.dds");

    // ── Initial lighting + environment constants ───────────────────────────────
    {
        SharedShaderConstants::DirectLight light;
        light.type                    = SharedShaderConstants::DirectLight::DIRECTIONAL;
        light.directional.orientation = Orientation(1.f, 0.f, 0.f, 0.f); // faces -Z
        light.directional.irradiance  = {1.0f, 0.95f, 0.9f, {600.0f}};   // warm white, 600 lux
        ssc->set0.directLighting.append(light);
    }
    {
        // NOTE: SharedShaderConstants::EnvironmentLightingConstants currently aliases
        // the v1 struct which uses rdg::Texture. A future update should migrate it to
        // gpu2::Texture so the assignments below compile without a cast.
        SharedShaderConstants::EnvironmentLightingConstants env;
        env.skyboxCubemap             = skyboxCubemap;
        env.irradianceMap             = irradianceMap;
        env.prefilteredEnvMap         = prefilteredEnvMap;
        env.brdfLut                   = brdfLut;
        env.environmentRadianceScale  = 3500.0f; // bright office environment in nits
        ssc->set0.environmentLighting = env;
    }

    // ── Shader artifacts (VS + PS) ────────────────────────────────────────────
    // Shaders are built once, off the critical path, and stored as artifacts so
    // all render nodes in subsequent frames can depend on them without rebuilding.
    ArtifactPtr pbrVsArtifact = graph->createArtifact("pbr mesh VS");
    ArtifactPtr pbrPsArtifact = graph->createArtifact("pbr mesh PS");
    graph->addNode(
        NodeDesc("create PBR mesh shaders")
            .setAction(Action::createFromLambda(
                           "build PBR shaders",
                           [&]() {
                               auto vs = GpuShader::create(
                                   {.context = gpuContext, .name = "pbr mesh VS", .binary = kPbrMeshVertSpv, .size = kPbrMeshVertSpvSize, .entry = "main"});
                               auto ps = GpuShader::create(
                                   {.context = gpuContext, .name = "pbr mesh PS", .binary = kPbrMeshFragSpv, .size = kPbrMeshFragSpvSize, .entry = "main"});
                               if (!vs || !ps) {
                                   GN_ERROR(sLogger)("Failed to create PBR shaders");
                                   return;
                               }
                               graph->publishArtifact(pbrVsArtifact, vs);
                               graph->publishArtifact(pbrPsArtifact, ps);
                           }),
                       nullptr));

    // ── Skybox shader artifacts ───────────────────────────────────────────────
    ArtifactPtr skyboxVsArtifact = graph->createArtifact("skybox VS");
    ArtifactPtr skyboxPsArtifact = graph->createArtifact("skybox PS");
    graph->addNode(NodeDesc("create skybox shaders")
                       .setAction(Action::createFromLambda("build skybox shaders",
                                                           [&]() {
                                                               auto vs = GpuShader::create({.context = gpuContext,
                                                                                            .name    = "skybox VS",
                                                                                            .binary  = kSkyboxVertSpv,
                                                                                            .size    = kSkyboxVertSpvSize * sizeof(unsigned int),
                                                                                            .entry   = "main"});
                                                               auto ps = GpuShader::create({.context = gpuContext,
                                                                                            .name    = "skybox PS",
                                                                                            .binary  = kSkyboxFragSpv,
                                                                                            .size    = kSkyboxFragSpvSize * sizeof(unsigned int),
                                                                                            .entry   = "main"});
                                                               if (!vs || !ps) {
                                                                   GN_ERROR(sLogger)("Failed to create skybox shaders");
                                                                   return;
                                                               }
                                                               graph->publishArtifact(skyboxVsArtifact, vs);
                                                               graph->publishArtifact(skyboxPsArtifact, ps);
                                                           }),
                                  nullptr));

    // ── Geometry artifact ─────────────────────────────────────────────────────
    ArtifactPtr geometryArtifact = graph->createArtifact("helmet geometry");
    graph->addNode(NodeDesc("load helmet geometry")
                       .setAction(Action::createFromLambda("load GLTF geometry",
                                                           [&]() {
                                                               auto geomPath = fs::toNativeDiskFilePath("media::pbr/DamagedHelmet/DamagedHelmet.gltf");
                                                               auto geom     = loadGltfGeometryV2(geomPath.c_str(), gpuContext);
                                                               if (geom.vertexCount == 0) {
                                                                   GN_ERROR(sLogger)("Failed to load helmet geometry");
                                                                   return;
                                                               }
                                                               graph->publishArtifact(geometryArtifact, geom);
                                                           }),
                                  nullptr));

    // Pre-capture "published at least once" tokens. These remain satisfied on
    // frame 2+ so the render node depends on a stable token rather than
    // calling getArtifactVersionToken() inside the loop (which would target the
    // next version each frame and never be satisfied after the first publish).
    TokenPtr vsReady       = graph->getArtifactVersionToken(pbrVsArtifact, NeverOverflowingCounter::ONE());
    TokenPtr psReady       = graph->getArtifactVersionToken(pbrPsArtifact, NeverOverflowingCounter::ONE());
    TokenPtr geomReady     = graph->getArtifactVersionToken(geometryArtifact, NeverOverflowingCounter::ONE());
    TokenPtr skyboxVsReady = graph->getArtifactVersionToken(skyboxVsArtifact, NeverOverflowingCounter::ONE());
    TokenPtr skyboxPsReady = graph->getArtifactVersionToken(skyboxPsArtifact, NeverOverflowingCounter::ONE());

    // Initialise the render target once. Only the color target view (slot 0) is
    // overwritten per frame via setColorTarget(); everything else stays constant.
    {
        GpuResourceView depthView = makeTextureView(depthTex);
        ssc->set0.renderTarget.colorTargets.append(RasterTarget::ColorTarget {}); // slot 0 placeholder; view is set each frame
        ssc->set0.renderTarget.setClearColor(0.1f, 0.1f, 0.15f, 1.0f).setDepthStencilTarget(depthView).setClearDepth(1.0f);
        ssc->set0.renderTarget.states.setDepthState({.func = RasterState::Compare::LESS, .write = true});
    }

    GN_INFO(sLogger)("Starting PBR mesh render loop (v2)...");

    // Orbit camera (same path as draw-mesh.cpp)
    const auto      orbitStartTime  = std::chrono::steady_clock::now();
    const float     orbitPhase0     = std::atan2(2.4f, 1.8f);
    constexpr float kOrbitRadiusXZ  = 3.0f;
    constexpr float kOrbitEyeY      = 1.4f;
    constexpr float kOrbitRadPerSec = 0.5f;

    // The 90° X rotation from the GLTF node transform ignored by the loader.
    const glm::mat4 worldTransform  = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    const glm::mat4 normalTransform = glm::transpose(glm::inverse(worldTransform));

    int totalFrames  = testMode ? 10 : 0;
    int frameCounter = 1;
    while (totalFrames == 0 || frameCounter++ <= totalFrames) {
        if (window && !window->runUntilNoNewEvents()) break;

        // ── Update camera ──────────────────────────────────────────────────────
        {
            const float            elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - orbitStartTime).count();
            const float            angle   = orbitPhase0 + kOrbitRadPerSec * elapsed;
            const glm::vec3        eye     = {kOrbitRadiusXZ * std::cos(angle), kOrbitEyeY, kOrbitRadiusXZ * std::sin(angle)};
            static const glm::vec3 kTarget(0.f, 0.f, 0.f), kUp(0.f, 1.f, 0.f);
            const glm::mat4        camToWorld = glm::inverse(glm::lookAtRH(eye, kTarget, kUp));

            ssc->set0.camera.cameraPosition    = eye;
            ssc->set0.camera.cameraOrientation = glm::quat_cast(glm::mat3(camToWorld));
            ssc->set0.camera.cameraFov         = Degree(60.f);
            ssc->set0.camera.aspectRatio       = (float) windowWidth / (float) windowHeight;
            ssc->set0.camera.nearPlane         = 0.1f;
            ssc->set0.camera.farPlane          = 100.f;
        }

        // ── Acquire swapchain image ────────────────────────────────────────────
        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) {
            GN_ERROR(sLogger)("Swapchain prepare failed");
            return -1;
        }

        // ── Update render target in SSC ────────────────────────────────────────
        // Only the color target view changes each frame; all other render-target
        // fields are constant and were initialised before the loop (see below).
        ssc->set0.renderTarget.setColorTarget(0, frame.view);

        // Snapshot the SSC state for this frame. The render node depends on this
        // token so it only runs after SSC has uploaded its GPU resources.
        TokenPtr sscSnapshot = ssc->takeSnapshot();

        // ── Render node (skybox + helmet in one render pass) ──────────────────
        // Both draws share the same GpuRaster (= same render pass). The skybox
        // is recorded first as a fullscreen triangle at depth=1.0; the helmet
        // overwrites it where its depth is smaller.
        AutoRef<GpuPayload> colorPassWork;

        auto colorPassAction = [&]() {
            auto skyboxVs = graph->getTypedArtifactContent<AutoRef<GpuShader>>(skyboxVsArtifact);
            auto skyboxPs = graph->getTypedArtifactContent<AutoRef<GpuShader>>(skyboxPsArtifact);
            auto pbrVs    = graph->getTypedArtifactContent<AutoRef<GpuShader>>(pbrVsArtifact);
            auto pbrPs    = graph->getTypedArtifactContent<AutoRef<GpuShader>>(pbrPsArtifact);
            auto geom     = graph->getTypedArtifactContent<RasterGeometry>(geometryArtifact);
            if (!skyboxVs || !skyboxPs || !pbrVs || !pbrPs || geom.vertexCount == 0) {
                GN_ERROR(sLogger)("Render node: missing shaders or geometry");
                return;
            }

            auto sscContent = ssc->getContent(sscSnapshot);
            if (!sscContent) {
                GN_ERROR(sLogger)("Render node: SSC content unavailable");
                return;
            }

            GpuResourceTable sscOnly;
            sscOnly.append(sscContent->set0Resources); // set 0: camera + IBL cubemap

            // ── Draw 1: skybox ───────────────────────────────────────────────
            // LESS_EQUAL depth (skybox vertex shader writes depth=1.0), no write,
            // so the subsequent helmet draw at smaller depth overwrites it.
            GpuRaster::DrawParameters skyboxDp;
            skyboxDp.vs                   = skyboxVs;
            skyboxDp.ps                   = skyboxPs;
            skyboxDp.geometry.vertexCount = 3; // fullscreen triangle from gl_VertexIndex; no VBO
            skyboxDp.resources            = sscOnly;
            skyboxDp.states.setDepthState({.func = RasterState::Compare::LESS_EQUAL, .write = false});

            // ── Draw 2: helmet (PBR) ─────────────────────────────────────────
            // set 0 — camera, lighting, IBL (SSC)
            // set 1 — PBR material textures: albedo, normal, emissive, occlusion, metalRough
            //         Binding indices must match the layout declared in pbr-mesh-frag.spv.
            GpuResourceSet matSet;
            for (auto * tex : {albedoTex.get(), normalTex.get(), emissiveTex.get(), occlusionTex.get(), metalRoughTex.get()}) {
                GpuResourceSlot slot;
                if (tex) slot.append(makeTextureView(GN::referenceTo(tex)));
                matSet.append(slot);
            }
            GpuResourceTable pbrResources;
            pbrResources.append(sscContent->set0Resources); // set 0: SSC
            pbrResources.append(matSet);                    // set 1: material

            ModelPushConstants  modelPc {.worldTransform = worldTransform, .normalTransform = normalTransform};
            ArrayProxy<uint8_t> immediates(reinterpret_cast<uint8_t *>(&modelPc), sizeof(modelPc));

            GpuRaster::DrawParameters pbrDp;
            pbrDp.vs         = pbrVs;
            pbrDp.ps         = pbrPs;
            pbrDp.geometry   = geom;
            pbrDp.resources  = pbrResources;
            pbrDp.immediates = immediates;
            pbrDp.states.setCullMode(RasterState::CULL_BACK).setFrontFace(RasterState::FRONT_CCW);

            // ── Single raster object = single render pass ────────────────────
            GpuRaster::CreateParameters rcp;
            rcp.gpu    = gpuContext;
            rcp.target = sscContent->set0Parameters.renderTarget;

            auto r = GpuRaster::create(rcp);
            r->draw(skyboxDp);
            r->draw(pbrDp);
            colorPassWork = r->seal();
        };

        auto colorPassNode = graph->addNode(NodeDesc("color pass")
                                                .setAction(Action::createFromLambda("skybox + PBR draw", colorPassAction), nullptr)
                                                .dependsOn(skyboxVsReady)
                                                .dependsOn(skyboxPsReady)
                                                .dependsOn(vsReady)
                                                .dependsOn(psReady)
                                                .dependsOn(geomReady)
                                                .dependsOn(sscSnapshot));

        // Block until the node has finished recording (colorPassWork is now valid).
        graph->waitForToken(graph->getNodeCompletionToken(colorPassNode));

        // ── GPU submit ────────────────────────────────────────────────────────
        gpuContext->submit(GpuContext::SubmitParameters(StrA::format("frame {}", frameCounter)).appendWork(colorPassWork).waitFor(frame.ready));

        // ── Present ───────────────────────────────────────────────────────────
        swapchain->present(*colorPassWork);
    }

    GN_INFO(sLogger)("Draw mesh v2 demo finished");
    return 0;
}

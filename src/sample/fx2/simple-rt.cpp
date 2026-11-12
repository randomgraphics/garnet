#include "gltf-loader.h"
#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <array>
#include <cstddef>
#include <cmath>
#include <cstdio>
#include <memory>

using namespace GN;
using namespace GN::gpu2;
namespace fx  = GN::fx2;
namespace gpu = GN::gpu2;

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

struct WaitForGpuIdle {
    AutoRef<GpuContext> gpu;

    ~WaitForGpuIdle() {
        if (gpu) gpu->waitForIdle();
    }
};
} // namespace

int main(int argc, const char * argv[]) {
    const bool headless = argc > 1 && argv[1][0] == 't';
    Host       host;
    host.gpu = GpuContext::create("pbr-rt-kernel-sample", {});
    if (!host.gpu) return 1;
    constexpr uint32_t width = 1280, height = 720;
    if (!headless) {
        host.window.reset(win::createWindow({.caption = "FX2 PBR / ray tracing (T); orbit (Space)", .clientWidth = width, .clientHeight = height}));
        if (!host.window) return 1;
        host.window->show();
        host.surface = host.window->createVulkanSurfaceHandle(host.gpu->getVulkanInstanceHandle());
        if (!host.surface) return 1;
    }
    Swapchain::CreateDesc swapchain {.gpu = host.gpu, .width = width, .height = height};
    swapchain.setSurface(host.surface);
    host.swapchain = Swapchain::create(swapchain);

    auto heap = gpu::DescriptorHeap::create("pbr.heap", {.gpu = host.gpu, .capacity = 128});
    if (!heap) return 1;
    auto upload = gpu::CnC::create("pbr.initialization", {.gpu = host.gpu, .heap = heap});
    if (!upload) return 1;

    auto pbr       = fx::PbrKernel::create(*heap, *upload);
    auto sky       = fx::SkyKernel::create(*heap, *upload);
    auto rayTrace  = fx::RayTraceKernel::create(*heap, *upload);
    auto unlit     = fx::UnlitKernel::create(*heap, *upload);
    auto constants = fx::SharedShaderConstants::create({.gpu = host.gpu, .uniformCapacity = 64 * 1024, .streamingCapacity = 64 * 1024});
    if (!host.swapchain || !heap || !pbr || !sky || !rayTrace || !unlit || !constants) return 1;

    // ─── Depth buffer ──────────────────────────────────────────────────────────
    const auto depthDesc   = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(width, height).setLevels(1);
    auto       depthBuffer = Texture::create("pbr.depth", {.context = host.gpu, .descriptor = depthDesc});
    if (!depthBuffer) return 1;

    // ─── Environment / Sky material ───────────────────────────────────────────
    auto skyboxTex      = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/qwantani_sunset_puresky/skybox.dds"});
    auto irradianceTex  = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/qwantani_sunset_puresky/irradiance.dds"});
    auto prefilteredTex = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/qwantani_sunset_puresky/prefiltered.dds"});
    auto brdfLutTex     = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/qwantani_sunset_puresky/brdf_lut.dds"});

    auto skyParams = fx::SkyMaterial::Parameters {};
    if (skyboxTex) skyParams.skyboxMap = GpuResourceView {skyboxTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (irradianceTex) skyParams.irradianceMap = GpuResourceView {irradianceTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (prefilteredTex) skyParams.prefilteredMap = GpuResourceView {prefilteredTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (brdfLutTex) skyParams.brdfLut = GpuResourceView {brdfLutTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    skyParams.luminanceScale = 1.0f;
    auto skyMaterial         = fx::SkyMaterial::create(*heap, *upload, skyParams);
    if (!skyMaterial) return 1;

    // ─── Female head model and PBR material ─────────────────────────────────
    StrA           gltfNativePath = fs::toNativeDiskFilePath("media::asset-foundry/model/misc/female_head_1.gltf");
    RasterGeometry helmetGeometry = gltf::loadGltfGeometry(gltfNativePath.c_str(), host.gpu, *upload, true);

    // Fallback triangle if gltf cannot be loaded
    AutoRef<Buffer> fallbackVb;
    if (helmetGeometry.vertexCount == 0) {
        const float fallbackPositions[] = {-0.6f, -0.7f, 0, 0, 0, 1, 0, 1, 0.6f, -0.7f, 0, 0, 0, 1, 1, 1, 0, 0.7f, 0, 0, 0, 1, 0.5f, 0};
        fallbackVb = Buffer::create("pbr.positions", {.context = host.gpu, .size = sizeof(fallbackPositions), .accelerationStructureBuildInput = true});
        if (!fallbackVb) return 1;
        upload->recordUploadBuffer(fallbackVb, 0, {reinterpret_cast<const uint8_t *>(fallbackPositions), sizeof(fallbackPositions)});
        helmetGeometry.vertices.push_back({.buffer = fallbackVb, .offset = 0, .stride = 8 * sizeof(float)});
        helmetGeometry.format.attributes.push_back(
            {.location = fx::PbrMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
        helmetGeometry.format.attributes.push_back(
            {.location = fx::PbrMaterial::NORMAL_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_3});
        helmetGeometry.format.attributes.push_back(
            {.location = fx::PbrMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 6 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});
        helmetGeometry.vertexCount = 3;
    }

    auto baseColorTex  = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/model/misc/female_head_1_img0.jpg"});
    auto normalTex     = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/model/misc/female_head_1_img2.jpg"});
    auto metalRoughTex = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/model/misc/female_head_1_img1.jpg"});

    auto helmetParams = fx::PbrMaterial::Parameters {};
    if (baseColorTex) helmetParams.baseColorMap = GpuResourceView {baseColorTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (normalTex) helmetParams.normalMap = GpuResourceView {normalTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (metalRoughTex) helmetParams.metalRoughMap = GpuResourceView {metalRoughTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    helmetParams.metallic  = 1.0f;
    helmetParams.roughness = 1.0f;
    auto helmetMaterial    = fx::PbrMaterial::create(*heap, *upload, helmetParams);
    if (!helmetMaterial) return 1;
    const glm::mat4                                   helmetRotation(1);
    const std::array<fx::PbrRayTracingScene::Mesh, 1> sceneMeshes {{{&helmetGeometry, helmetMaterial.get(), helmetRotation}}};
    auto                                              scene = fx::PbrRayTracingScene::create(*heap, *upload, {sceneMeshes.data(), sceneMeshes.size()});
    if (!scene) return 1;
    auto asc = scene->accelerationStructure();
    if (!asc) return 1;

    auto initialization = upload->seal();
    if (!initialization) return 1;

    RasterState skyState;
    skyState.cullMode   = RasterState::CULL_NONE;
    skyState.depthState = RasterState::DepthState {.func = RasterState::Compare::ALWAYS, .write = false};

    RasterState helmetState;
    helmetState.cullMode   = RasterState::CULL_BACK;
    helmetState.frontFace  = RasterState::FRONT_CCW;
    helmetState.depthState = RasterState::DepthState {.func = RasterState::Compare::LESS_EQUAL, .write = true};

    bool  rayTracingEnabled = true;
    bool  tWasDown          = false;
    bool  rotationPaused    = false;
    bool  spaceWasDown      = false;
    float camAngle          = 0;
    // Wait before scene and material locals release buffers still referenced by submitted draws.
    WaitForGpuIdle waitForGpuIdle {host.gpu};
    for (int frame = 0; !headless || frame < 3; ++frame) {
        if (host.window && !host.window->runUntilNoNewEvents()) break;
        if (host.window) {
            const bool tDown = host.window->getKeyStatus(win::KeyCode::T).down;
            if (tDown && !tWasDown) rayTracingEnabled = !rayTracingEnabled;
            tWasDown             = tDown;
            const bool spaceDown = host.window->getKeyStatus(win::KeyCode::SPACEBAR).down;
            if (spaceDown && !spaceWasDown) rotationPaused = !rotationPaused;
            spaceWasDown = spaceDown;
        }
        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;

        RasterTarget target;
        target.setColorTarget(0, acquired.view).setClearColor(0.02f, 0.03f, 0.05f, 1);
        target.setDepthStencilTarget(GpuResourceView {depthBuffer}).setClearDepth(1.0f);

        auto uniformUploads = gpu::CnC::create("pbr.uniform-upload", {.gpu = host.gpu, .heap = heap});
        if (!uniformUploads) return 1;

        fx::SharedUniforms uniforms;
        if (!rotationPaused) camAngle += 0.01f;
        constexpr float camDist          = 1.3f;
        uniforms.cameraPosition          = {camDist * std::sin(camAngle), 0.5f, camDist * std::cos(camAngle), 1};
        uniforms.numLights               = 0; // turn off direct lighting for now; we'll just use the environment map for lighting
        uniforms.lights[0].positionOrDir = {0.5f, 1.0f, 0.5f, float(fx::DirectLightUniform::DIRECTIONAL)};
        uniforms.lights[0].colorAndRange = {2.5f, 2.5f, 2.5f, 0.0f};
        uniforms.exposure                = 1.0f;
        uniforms.renderTargetSize        = {width, height};
        uniforms.viewMatrix              = glm::lookAtRH(glm::vec3(uniforms.cameraPosition), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        uniforms.projMatrix              = glm::perspectiveRH_ZO(glm::radians(60.f), float(width) / height, uniforms.nearPlane, uniforms.farPlane);
        uniforms.projMatrix[1][1] *= -1; // Vulkan clip space
        uniforms.projViewMatrix = uniforms.projMatrix * uniforms.viewMatrix;
        uniforms.frameCounter   = static_cast<uint32_t>(frame);

        auto state = constants->recordUniformUpdate(*uniformUploads, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
        if (!state) return 1;
        auto uniformWork = uniformUploads->seal();
        if (!uniformWork) return 1;

        auto passResources = fx::sharedUniformResources(state);
        if (rayTracingEnabled) {
            passResources.resize(3);
            passResources[2].resize(2);
            GpuResourceView accelerationStructure;
            accelerationStructure.setAccelerationStructure(asc);
            passResources[2][0].append(std::move(accelerationStructure));
            passResources[2][1].append(scene->geometryTable());
        }
        auto raster = gpu::Raster::create(
            "pbr.render", {.gpu = host.gpu, .target = &target, .heap = heap, .heapSetIndex = 0, .passResources = passResources, .numberOfDrawsHint = 2});
        if (!raster) return 1;

        // 1. Draw skybox
        fx::SkyMaterial::DrawParameters skyDraw {*raster, state, &skyState};
        if (!sky->record(*skyMaterial, skyDraw)) return 1;

        // 2. Draw helmet oriented upright
        if (rayTracingEnabled) {
            fx::RayTraceKernel::DrawParameters rtDraw {{*raster, state, helmetGeometry, &helmetState}, asc, helmetRotation, skyMaterial, helmetMaterial.get()};
            if (!rayTrace->record(rtDraw)) return 1;
        } else {
            fx::PbrMaterial::DrawParameters helmetDraw {{*raster, state, helmetGeometry, &helmetState}, helmetRotation, skyMaterial};
            if (!pbr->record(*helmetMaterial, helmetDraw)) return 1;
        }

        auto draws = raster->seal();
        if (!draws) return 1;

        GpuContext::SubmitParameters submission("pbr.frame");
        if (initialization) submission.appendWork(initialization);
        submission.appendWork(uniformWork);
        submission.appendWork(draws).waitFor(acquired.ready);
        host.gpu->submit(submission);
        initialization.clear();
        host.swapchain->present(*draws);
    }
    return 0;
}

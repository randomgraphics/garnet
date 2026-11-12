#include "gltf-loader.h"
#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
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

    // Persistent HDR surface for progressive ray-traced samples; alpha tracks covered model pixels.
    auto accumulationTexture = Texture::create(
        "pbr.accumulation",
        {.context    = host.gpu,
         .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_16_16_16_16_FLOAT()).setDimensions(width, height).setLevels(1)});
    if (!accumulationTexture) return 1;
    const GpuResourceView         accumulationView = GpuResourceView {accumulationTexture}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    fx::UnlitMaterial::Parameters accumulationMaterialParameters;
    accumulationMaterialParameters.colorMap = accumulationView;
    auto accumulationMaterial               = fx::UnlitMaterial::create(*heap, *upload, accumulationMaterialParameters);
    if (!accumulationMaterial) return 1;
    const float compositeVertices[]   = {-1, -1, 0.5f, 0, 1, 1, -1, 0.5f, 1, 1, 1, 1, 0.5f, 1, 0, -1, -1, 0.5f, 0, 1, 1, 1, 0.5f, 1, 0, -1, 1, 0.5f, 0, 0};
    auto        compositeVertexBuffer = Buffer::create("pbr.composite-vertices", {.context = host.gpu, .size = sizeof(compositeVertices)});
    if (!compositeVertexBuffer) return 1;
    upload->recordUploadBuffer(compositeVertexBuffer, 0, {reinterpret_cast<const uint8_t *>(compositeVertices), sizeof(compositeVertices)});
    RasterGeometry compositeGeometry;
    compositeGeometry.vertices.push_back({.buffer = compositeVertexBuffer, .offset = 0, .stride = 5 * sizeof(float)});
    compositeGeometry.format.attributes.push_back(
        {.location = fx::UnlitMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    compositeGeometry.format.attributes.push_back(
        {.location = fx::UnlitMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});
    compositeGeometry.vertexCount = 6;
    auto initialization           = upload->seal();
    if (!initialization) return 1;

    RasterState skyState;
    skyState.cullMode   = RasterState::CULL_NONE;
    skyState.depthState = RasterState::DepthState {.func = RasterState::Compare::ALWAYS, .write = false};

    RasterState helmetState;
    helmetState.cullMode   = RasterState::CULL_BACK;
    helmetState.frontFace  = RasterState::FRONT_CCW;
    helmetState.depthState = RasterState::DepthState {.func = RasterState::Compare::LESS_EQUAL, .write = true};

    bool           rayTracingEnabled = true;
    bool           tWasDown = false, rotationPaused = false, spaceWasDown = false;
    float          camAngle                = 0;
    uint32_t       accumulationSampleCount = 0;
    bool           havePreviousCamera      = false;
    glm::vec3      previousCameraPosition(0);
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
        if (!rotationPaused) camAngle += 0.01f;
        constexpr float camDist = 1.3f;
        const glm::vec3 cameraPosition(camDist * std::sin(camAngle), 0.5f, camDist * std::cos(camAngle));
        const glm::vec3 cameraDelta = cameraPosition - previousCameraPosition;
        const bool      cameraMoved = !havePreviousCamera || glm::dot(cameraDelta, cameraDelta) > 1e-8f;
        if (cameraMoved || !rayTracingEnabled) accumulationSampleCount = 0;
        previousCameraPosition = cameraPosition;
        havePreviousCamera     = true;
        auto acquired          = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;

        auto uniformUploads = gpu::CnC::create("pbr.uniform-upload", {.gpu = host.gpu, .heap = heap});
        if (!uniformUploads) return 1;
        fx::SharedUniforms uniforms;
        uniforms.cameraPosition   = glm::vec4(cameraPosition, 1);
        uniforms.numLights        = 0;
        uniforms.exposure         = 1.0f;
        uniforms.renderTargetSize = glm::vec2(width, height);
        uniforms.viewMatrix       = glm::lookAtRH(cameraPosition, glm::vec3(0), glm::vec3(0, 1, 0));
        uniforms.projMatrix       = glm::perspectiveRH_ZO(glm::radians(60.f), float(width) / float(height), uniforms.nearPlane, uniforms.farPlane);
        uniforms.projMatrix[1][1] *= -1;
        uniforms.projViewMatrix = uniforms.projMatrix * uniforms.viewMatrix;
        uniforms.frameCounter   = accumulationSampleCount;
        auto state              = constants->recordUniformUpdate(*uniformUploads, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
        if (!state) return 1;
        auto uniformWork = uniformUploads->seal();
        if (!uniformWork) return 1;

        AutoRef<gpu::GpuPayload> accumulationDraws;
        if (rayTracingEnabled) {
            RasterTarget accumulationTarget;
            accumulationTarget.setColorTarget(0, GpuResourceView {accumulationTexture});
            accumulationTarget.loadColor = !cameraMoved && accumulationSampleCount > 0;
            accumulationTarget.setClearColor(0, 0, 0, 0);
            accumulationTarget.setDepthStencilTarget(GpuResourceView {depthBuffer}).setClearDepth(1.0f);
            auto & accumulationBlend   = accumulationTarget.colorTargets[0].blendState;
            accumulationBlend.colorSrc = RasterTarget::BlendState::SRC_ALPHA;
            accumulationBlend.colorDst = RasterTarget::BlendState::INV_SRC_ALPHA;
            accumulationBlend.alphaSrc = RasterTarget::BlendState::ONE;
            accumulationBlend.alphaDst = RasterTarget::BlendState::INV_SRC_ALPHA;
            auto rtResources           = fx::sharedUniformResources(state);
            rtResources.resize(3);
            rtResources[2].resize(2);
            GpuResourceView accelerationStructure;
            accelerationStructure.setAccelerationStructure(asc);
            rtResources[2][0].append(std::move(accelerationStructure));
            rtResources[2][1].append(scene->geometryTable());
            auto accumulationRaster = gpu::Raster::create(
                "pbr.accumulation",
                {.gpu = host.gpu, .target = &accumulationTarget, .heap = heap, .heapSetIndex = 0, .passResources = rtResources, .numberOfDrawsHint = 1});
            if (!accumulationRaster) return 1;
            fx::RayTraceKernel::DrawParameters rtDraw {{*accumulationRaster, state, helmetGeometry, &helmetState},
                                                       asc,
                                                       helmetRotation,
                                                       skyMaterial,
                                                       helmetMaterial.get()};
            if (!rayTrace->record(rtDraw)) return 1;
            accumulationDraws = accumulationRaster->seal();
            if (!accumulationDraws) return 1;
            ++accumulationSampleCount;
        }

        RasterTarget target;
        target.setColorTarget(0, acquired.view).setClearColor(0.02f, 0.03f, 0.05f, 1);
        target.setDepthStencilTarget(GpuResourceView {depthBuffer}).setClearDepth(1.0f);
        if (rayTracingEnabled) {
            auto & blend   = target.colorTargets[0].blendState;
            blend.colorSrc = RasterTarget::BlendState::SRC_ALPHA;
            blend.colorDst = RasterTarget::BlendState::INV_SRC_ALPHA;
            blend.alphaSrc = RasterTarget::BlendState::ONE;
            blend.alphaDst = RasterTarget::BlendState::INV_SRC_ALPHA;
        }
        auto raster = gpu::Raster::create(
            "pbr.composite",
            {.gpu = host.gpu, .target = &target, .heap = heap, .heapSetIndex = 0, .passResources = fx::sharedUniformResources(state), .numberOfDrawsHint = 2});
        if (!raster) return 1;
        fx::SkyMaterial::DrawParameters skyDraw {*raster, state, &skyState};
        if (!sky->record(*skyMaterial, skyDraw)) return 1;
        if (rayTracingEnabled) {
            RasterState                       compositeState = skyState;
            fx::UnlitMaterial::DrawParameters compositeDraw {{*raster, state, compositeGeometry, &compositeState}, glm::inverse(uniforms.projViewMatrix)};
            if (!unlit->record(*accumulationMaterial, compositeDraw)) return 1;
        } else {
            fx::PbrMaterial::DrawParameters helmetDraw {{*raster, state, helmetGeometry, &helmetState}, helmetRotation, skyMaterial};
            if (!pbr->record(*helmetMaterial, helmetDraw)) return 1;
        }
        auto draws = raster->seal();
        if (!draws) return 1;
        GpuContext::SubmitParameters submission("pbr.frame");
        if (initialization) submission.appendWork(initialization);
        submission.appendWork(uniformWork);
        if (accumulationDraws) submission.appendWork(accumulationDraws);
        submission.appendWork(draws).waitFor(acquired.ready);
        host.gpu->submit(submission);
        initialization.clear();
        host.swapchain->present(*draws);
    }
    return 0;
}

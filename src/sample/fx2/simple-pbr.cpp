#include "gltf-loader.h"
#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <cmath>
#include <memory>

using namespace GN;
using namespace GN::gpu2;
namespace fxBindless  = GN::fx2::bindless;
namespace gpuBindless = GN::gpu2::bindless;

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
} // namespace

int main(int argc, const char * argv[]) {
    const bool headless = argc > 1 && argv[1][0] == 't';
    Host       host;
    host.gpu = GpuContext::create("pbr-kernel-sample", {});
    if (!host.gpu) return 1;
    constexpr uint32_t width = 1280, height = 720;
    if (!headless) {
        host.window.reset(win::createWindow({.caption = "FX2 bindless PBR helmet & skybox", .clientWidth = width, .clientHeight = height}));
        if (!host.window) return 1;
        host.window->show();
        host.surface = host.window->createVulkanSurfaceHandle(host.gpu->getVulkanInstanceHandle());
        if (!host.surface) return 1;
    }
    Swapchain::CreateDesc swapchain {.gpu = host.gpu, .width = width, .height = height};
    swapchain.setSurface(host.surface);
    host.swapchain = Swapchain::create(swapchain);

    auto heap = gpuBindless::DescriptorHeap::create("pbr.heap", {.gpu = host.gpu, .capacity = 128});
    if (!heap) return 1;
    auto upload = gpuBindless::CnC::create("pbr.initialization", {.gpu = host.gpu, .heap = heap});
    if (!upload) return 1;

    auto pbr       = fxBindless::PbrKernel::create(*heap, *upload);
    auto sky       = fxBindless::SkyKernel::create(*heap, *upload);
    auto constants = fxBindless::SharedShaderConstants::create({.gpu = host.gpu, .uniformCapacity = 64 * 1024, .streamingCapacity = 64 * 1024});
    if (!host.swapchain || !heap || !pbr || !sky || !constants) return 1;

    // ─── Depth buffer ──────────────────────────────────────────────────────────
    const auto depthDesc   = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(width, height).setLevels(1);
    auto       depthBuffer = Texture::create("pbr.depth", {.context = host.gpu, .descriptor = depthDesc});
    if (!depthBuffer) return 1;

    // ─── Environment / Sky material ───────────────────────────────────────────
    auto skyboxTex      = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds"});
    auto irradianceTex  = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds"});
    auto prefilteredTex = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds"});
    auto brdfLutTex     = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds"});

    auto skyParams = sky->defaultMaterialParameters();
    if (skyboxTex) skyParams.skyboxMap = GpuResourceView {skyboxTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (irradianceTex) skyParams.irradianceMap = GpuResourceView {irradianceTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (prefilteredTex) skyParams.prefilteredMap = GpuResourceView {prefilteredTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (brdfLutTex) skyParams.brdfLut = GpuResourceView {brdfLutTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    skyParams.luminanceScale = 1.0f;
    auto skyMaterial         = sky->createMaterial(*upload, skyParams);
    if (!skyMaterial) return 1;

    // ─── DamagedHelmet model and PBR material ─────────────────────────────────
    StrA           gltfNativePath = fs::toNativeDiskFilePath("media::asset-foundry/model/DamagedHelmet/DamagedHelmet.gltf");
    RasterGeometry helmetGeometry = gltf::loadGltfGeometry(gltfNativePath.c_str(), host.gpu, *upload);

    // Fallback triangle if gltf cannot be loaded
    AutoRef<Buffer> fallbackVb;
    if (helmetGeometry.vertexCount == 0) {
        const float fallbackPositions[] = {-0.6f, -0.7f, 0, 0, 0, 1, 0, 1, 0.6f, -0.7f, 0, 0, 0, 1, 1, 1, 0, 0.7f, 0, 0, 0, 1, 0.5f, 0};
        fallbackVb                      = Buffer::create("pbr.positions", {.context = host.gpu, .size = sizeof(fallbackPositions)});
        if (!fallbackVb) return 1;
        upload->recordUploadBuffer(fallbackVb, 0, {reinterpret_cast<const uint8_t *>(fallbackPositions), sizeof(fallbackPositions)});
        helmetGeometry.vertices.push_back({.buffer = fallbackVb, .offset = 0, .stride = 8 * sizeof(float)});
        helmetGeometry.format.attributes.push_back(
            {.location = fxBindless::PbrMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
        helmetGeometry.format.attributes.push_back({.location = fxBindless::PbrMaterial::NORMAL_LOCATION,
                                                    .binding  = 0,
                                                    .offset   = 3 * sizeof(float),
                                                    .format   = RasterGeometry::AttributeFormat::F32_3});
        helmetGeometry.format.attributes.push_back({.location = fxBindless::PbrMaterial::TEXCOORD_LOCATION,
                                                    .binding  = 0,
                                                    .offset   = 6 * sizeof(float),
                                                    .format   = RasterGeometry::AttributeFormat::F32_2});
        helmetGeometry.vertexCount = 3;
    }

    auto baseColorTex  = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/model/DamagedHelmet/baseColor_1.jpg"});
    auto normalTex     = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/model/DamagedHelmet/normal_1-gl.jpg"});
    auto emissiveTex   = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/model/DamagedHelmet/emissive_1.jpg"});
    auto occlusionTex  = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/model/DamagedHelmet/occlusion_1.jpg"});
    auto metalRoughTex = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/model/DamagedHelmet/metallicRoughness_1.jpg"});

    auto helmetParams = pbr->defaultMaterialParameters();
    if (baseColorTex) helmetParams.baseColorMap = GpuResourceView {baseColorTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (normalTex) helmetParams.normalMap = GpuResourceView {normalTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (emissiveTex) helmetParams.emissiveMap = GpuResourceView {emissiveTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (occlusionTex) helmetParams.occlusionMap = GpuResourceView {occlusionTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    if (metalRoughTex) helmetParams.metalRoughMap = GpuResourceView {metalRoughTex}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    helmetParams.metallic  = 1.0f;
    helmetParams.roughness = 1.0f;
    auto helmetMaterial    = pbr->createMaterial(*upload, helmetParams);
    if (!helmetMaterial) return 1;

    auto initialization = upload->seal();
    if (!initialization) return 1;

    RasterState skyState;
    skyState.cullMode   = RasterState::CULL_NONE;
    skyState.depthState = RasterState::DepthState {.func = RasterState::Compare::ALWAYS, .write = false};

    RasterState helmetState;
    helmetState.cullMode   = RasterState::CULL_BACK;
    helmetState.frontFace  = RasterState::FRONT_CCW;
    helmetState.depthState = RasterState::DepthState {.func = RasterState::Compare::LESS_EQUAL, .write = true};

    for (int frame = 0; !headless || frame < 3; ++frame) {
        if (host.window && !host.window->runUntilNoNewEvents()) break;
        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;

        RasterTarget target;
        target.setColorTarget(0, acquired.view).setClearColor(0.02f, 0.03f, 0.05f, 1);
        target.setDepthStencilTarget(GpuResourceView {depthBuffer}).setClearDepth(1.0f);

        auto uniformUploads = gpuBindless::CnC::create("pbr.uniform-upload", {.gpu = host.gpu, .heap = heap});
        if (!uniformUploads) return 1;

        fxBindless::SharedUniforms uniforms;
        const float                camAngle = static_cast<float>(frame) * 0.01f;
        constexpr float            camDist  = 2.6f;
        uniforms.cameraPosition             = {camDist * std::sin(camAngle), 0.5f, camDist * std::cos(camAngle), 1};
        uniforms.numLights                  = 1;
        uniforms.lights[0].positionOrDir    = {0.5f, 1.0f, 0.5f, float(fxBindless::DirectLightUniform::DIRECTIONAL)};
        uniforms.lights[0].colorAndRange    = {2.5f, 2.5f, 2.5f, 0.0f};
        uniforms.exposure                   = 1.0f;
        uniforms.renderTargetSize           = {width, height};
        uniforms.viewMatrix                 = glm::lookAtRH(glm::vec3(uniforms.cameraPosition), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        uniforms.projMatrix                 = glm::perspectiveRH_ZO(glm::radians(60.f), float(width) / height, uniforms.nearPlane, uniforms.farPlane);
        uniforms.projMatrix[1][1] *= -1; // Vulkan clip space
        uniforms.projViewMatrix = uniforms.projMatrix * uniforms.viewMatrix;
        uniforms.frameCounter   = static_cast<uint32_t>(frame);

        auto state = constants->recordUniformUpdate(*uniformUploads, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
        if (!state) return 1;
        auto uniformWork = uniformUploads->seal();
        if (!uniformWork) return 1;

        auto raster = gpuBindless::Raster::create("pbr.render", {.gpu               = host.gpu,
                                                                 .target            = &target,
                                                                 .heap              = heap,
                                                                 .heapSetIndex      = 0,
                                                                 .passResources     = fxBindless::sharedUniformResources(state),
                                                                 .numberOfDrawsHint = 2});
        if (!raster) return 1;

        // 1. Draw skybox
        fxBindless::SkyMaterial::DrawParameters skyDraw {*raster, state, &skyState};
        if (!skyMaterial->record(skyDraw)) return 1;

        // 2. Draw helmet oriented upright
        const glm::mat4                         helmetRotation = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
        fxBindless::PbrMaterial::DrawParameters helmetDraw {{*raster, state, helmetGeometry, &helmetState}, helmetRotation, skyMaterial};
        if (!helmetMaterial->record(helmetDraw)) return 1;

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

#include "sample-sphere.h"
#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>

using namespace GN;
using namespace GN::fx2;
using namespace GN::gpu2;

static GN::Logger * sLogger = GN::getLogger("GN.sample.fx2.cel");

static SharedShaderConstants::Snapshot updateSsc(SharedShaderConstants * ssc, const RasterTarget & target, int frameIdx) {
    // Orbit camera around Y axis.
    const float            orbitAngle = static_cast<float>(frameIdx) * 0.002f;
    constexpr float        kRadius    = 3.0f;
    const glm::vec3        eye        = {kRadius * std::sin(orbitAngle), 1.2f, kRadius * std::cos(orbitAngle)};
    static const glm::vec3 kTarget(0.f, 0.f, 0.f), kUp(0.f, 1.f, 0.f);
    const glm::mat4        camToWorld = glm::inverse(glm::lookAtRH(eye, kTarget, kUp));

    const auto rasterSize = target.calcRasterSizeInPixel();

    ssc->set0.camera.cameraPosition       = eye;
    ssc->set0.camera.cameraOrientation    = glm::quat_cast(glm::mat3(camToWorld));
    ssc->set0.camera.aspectRatio          = static_cast<float>(rasterSize.x) / static_cast<float>(rasterSize.y);
    ssc->set0.camera.viewWidthInPixel     = rasterSize.x;
    ssc->set0.camera.viewHeightInPixel    = rasterSize.y;
    ssc->set0.frameConstants.frameCounter = frameIdx;

    // Direct directional sun light for crisp cel shadows
    ssc->set0.directLighting.clear();
    SharedShaderConstants::DirectLight sun;
    sun.type                    = SharedShaderConstants::DirectLight::DIRECTIONAL;
    sun.directional.orientation = glm::quat(glm::vec3(0.6f, 0.8f, 0.0f));
    // SSC uses photometric lighting and defaults to exposure 0.002 (a 500-nit reference).
    sun.directional.irradiance = {1.0f, 0.98f, 0.95f, {500.0f}};
    ssc->set0.directLighting.append(sun);

    ssc->set0.envLighting.environmentAmbientFloor   = 0.1f;
    ssc->set0.envLighting.environmentLuminanceScale = 1200.0f;

    return ssc->takeSnapshot();
}

int main(int argc, const char ** argv) {
    bool testMode = (argc > 1) && (argv[1][0] == 't');
    if (testMode) { GN_INFO(sLogger, "Running in test mode"); }

    enableCRTMemoryCheck();

    const uint32_t W = 1280, H = 720;

    // ─── GPU ──────────────────────────────────────────────────────────────────
    auto gpuContext = GpuContext::create("gpu", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    // ─── Shared shader constants ──────────────────────────────────────────────
    auto ssc = SharedShaderConstants::create({.gpu = gpuContext});
    if (!ssc) return -1;
    auto skybox = SkyboxKernel::create(gpuContext);
    if (!skybox) return -1;

    ssc->set0.envLighting = {
        .skyboxPath                = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds",
        .irradiancePath            = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds",
        .prefilteredPath           = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
        .brdfLutPath               = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
        .environmentLuminanceScale = 1200.f,
    };

    auto initializationRecorder = GpuCnC::create({.gpu = gpuContext});
    if (!initializationRecorder) return -1;
    auto kernel = CelKernel::create(gpuContext, *initializationRecorder);
    if (!kernel) return -1;
    CelKernel::Inputs inputs;
    inputs.geometry = createSampleSphere(gpuContext, *initializationRecorder);
    if (!inputs.geometry.indexCount) return -1;
    inputs.states.cullMode   = RasterState::CULL_BACK;
    inputs.states.frontFace  = RasterState::FRONT_CCW;
    inputs.states.depthState = RasterState::DepthState {RasterState::Compare::LESS, true};
    inputs.color             = {0.8f, 0.3f, 0.08f, 1};
    inputs.outlineWidth      = 0.01f;
    inputs.rimIntensity      = 150.0f; // Keep the stylized rim visible at the same camera exposure.
    auto initialization      = initializationRecorder->seal();
    if (!initialization) return -1;

    // ─── Window + swapchain ───────────────────────────────────────────────────
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        window.reset(win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - Cel / Anime NPR (fx2)", .clientWidth = W, .clientHeight = H}));
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
    rasterTarget.setDepthStencilTarget(depthView).setClearColor(0.1f, 0.12f, 0.16f, 1.f).setClearDepth(1.f);

    int totalFrames = testMode ? 5 : 0;
    int frameIdx    = 0;
    while (totalFrames == 0 || frameIdx < totalFrames) {
        ++frameIdx;
        if (window && !window->runUntilNoNewEvents()) break;

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) return -1;

        rasterTarget.setColorTarget(0, frame.view);
        SharedShaderConstants::Snapshot sscSnapshot = updateSsc(ssc, rasterTarget, frameIdx);
        auto                            uploads     = GpuCnC::create({.gpu = gpuContext});
        auto                            raster      = GpuRaster::create("sample.frame", {.gpu = gpuContext, .target = &rasterTarget});
        if (!uploads || !raster || !kernel->record(*raster, *uploads, sscSnapshot.set0Resources, inputs) || !skybox->record(*raster, sscSnapshot.set0Resources))
            return -1;
        auto parameters = uploads->seal();
        auto rendered   = raster->seal();
        if (!parameters || !rendered) return -1;
        GpuContext::SubmitParameters submit("sample.frame");
        if (initialization) submit.appendWork(initialization);
        for (const auto & payload : sscSnapshot.set0Payloads) {
            if (!payload) return -1;
            submit.appendWork(payload);
        }
        submit.appendWork(parameters).appendWork(rendered).waitFor(frame.ready);
        gpuContext->submit(submit);
        initialization.clear();
        swapchain->present(*rendered);
    }

    gpuContext->waitForIdle();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);
    return 0;
}

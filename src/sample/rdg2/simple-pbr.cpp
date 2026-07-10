#include <garnet/GNrdg2.h>
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>

using namespace GN;
using namespace GN::rdg2;
using namespace GN::gpu2;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.rdg2.pbr");

static SharedShaderConstants::Snapshot updateSsc(SharedShaderConstants * ssc, const RasterTarget & target, int frameIdx) {
    // Orbit camera around Y axis; helmet stays fixed at origin.
    const float            orbitAngle = static_cast<float>(frameIdx) * 0.001f;
    constexpr float        kRadius    = 3.f;
    const glm::vec3        eye        = {kRadius * std::sin(orbitAngle), 1.4f, kRadius * std::cos(orbitAngle)};
    static const glm::vec3 kTarget(0.f, 0.f, 0.f), kUp(0.f, 1.f, 0.f);
    const glm::mat4        camToWorld = glm::inverse(glm::lookAtRH(eye, kTarget, kUp));

    // Update per-frame SSC data before takeSnapshot() freezes it into the UBO.
    const auto rasterSize = target.calcRasterSizeInPixel();

    ssc->set0.camera.cameraPosition       = eye;
    ssc->set0.camera.cameraOrientation    = glm::quat_cast(glm::mat3(camToWorld));
    ssc->set0.camera.aspectRatio          = static_cast<float>(rasterSize.x) / static_cast<float>(rasterSize.y);
    ssc->set0.camera.viewWidthInPixel     = rasterSize.x;
    ssc->set0.camera.viewHeightInPixel    = rasterSize.y;
    ssc->set0.frameConstants.frameCounter = frameIdx;

    // Packs current set0 into staging buffers and returns the GPU upload payloads.
    return ssc->takeSnapshot();
}

int main(int argc, const char ** argv) {
    bool testMode = (argc > 1) && (argv[1][0] == 't');
    if (testMode) { GN_INFO(sLogger)("Running in test mode"); }

    enableCRTMemoryCheck();

    const uint32_t W = 1280, H = 720;

    // ─── GPU ──────────────────────────────────────────────────────────────────
    auto gpuContext = GpuContext::create("gpu", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    // ─── Shared shader constants ──────────────────────────────────────────────
    // SSC owns the skybox shaders and env texture loading. Set envLighting paths
    // before the first takeSnapshot(); path changes are loaded synchronously by takeSnapshot().
    auto ssc = SharedShaderConstants::create({.gpu = gpuContext});
    if (!ssc) return -1;

    ssc->set0.envLighting = {
        .skyboxPath               = "media::envmap/bad-salzbrunn-walking-hall/skybox-cube.dds",
        .irradiancePath           = "media::envmap/bad-salzbrunn-walking-hall/irradiance.dds",
        .prefilteredPath          = "media::envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
        .brdfLutPath              = "media::envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
        .environmentRadianceScale = 3500.f,
    };

    auto helmet = PbrShading::load(gpuContext, {
                                                   .gltfPath       = "media::pbr/DamagedHelmet/DamagedHelmet.gltf",
                                                   .albedoPath     = "media::pbr/DamagedHelmet/baseColor_1.jpg",
                                                   .normalPath     = "media::pbr/DamagedHelmet/normal_1-gl.jpg",
                                                   .emissivePath   = "media::pbr/DamagedHelmet/emissive_1.jpg",
                                                   .occlusionPath  = "media::pbr/DamagedHelmet/occlusion_1.jpg",
                                                   .metalRoughPath = "media::pbr/DamagedHelmet/metallicRoughness_1.jpg",
                                               });
    if (!helmet) return -1;

    // ─── Window + swapchain ───────────────────────────────────────────────────
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        window.reset(win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - PBR (rdg2)", .clientWidth = W, .clientHeight = H}));
        if (!window) return -1;
        window->show();
        surface = window->createVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
        if (!surface) return -1;
    }
    Swapchain::CreateDesc scDesc {.gpu = gpuContext, .width = W, .height = H};
    if (surface) scDesc.setSurface(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) return -1;

    // Depth buffer; shared across frames (depth contents don't need to persist).
    auto depthTex = Texture::create(
        "depth", {.context = gpuContext, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(W, H)});
    if (!depthTex) return -1;
    GpuResourceView depthView;
    depthView.resource = depthTex;

    RasterTarget rasterTarget;
    rasterTarget.colorTargets.append(RasterTarget::ColorTarget {});
    rasterTarget.setDepthStencilTarget(depthView).setClearColor(0.05f, 0.05f, 0.1f, 1.f).setClearDepth(1.f);

    bool helmetUploadSubmitted = false;

    int totalFrames = testMode ? 5 : 0;
    int frameIdx    = 0;
    while (totalFrames == 0 || frameIdx < totalFrames) {
        ++frameIdx;
        if (window && !window->runUntilNoNewEvents()) break;

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) return -1;

        rasterTarget.setColorTarget(0, frame.view);
        SharedShaderConstants::Snapshot           sscSnapshot = updateSsc(ssc, rasterTarget, frameIdx);
        gpu2::ArrayContainer<AutoRef<GpuPayload>> renderWorks;
        auto                                      drawScene = [&]() {
            renderWorks.append(sscSnapshot.set0Payloads);

            GpuRaster::CreateParameters rcp;
            rcp.gpu    = gpuContext;
            rcp.target = &rasterTarget;

            auto r = GpuRaster::create("simple-pbr", rcp);
            if (!r) return;
            glm::mat4 helmetMat = glm::mat4(1.f);
            r->draw(PbrShading::getDrawParams(sscSnapshot, helmet, helmetMat));
            r->draw(ssc->getSkyboxDrawParams(sscSnapshot.set0Resources));
            renderWorks.append(r->seal());
        };
        drawScene();

        // ─── Single frame submit ──────────────────────────────────────────────
        // Payloads ordered: PBR asset upload → set0 (env + UBO) → render work.
        GpuContext::SubmitParameters submit(StrA::format("frame {}", frameIdx));

        if (!helmetUploadSubmitted) {
            submit.appendWork(helmet->gpuPayload);
            helmetUploadSubmitted = true;
        }

        // set0 payloads (env uploads + UBO) then render work; last payload waits for frame.ready.
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

    // Drain the GPU before AutoRef destructors release Vulkan resources, then destroy the
    // sample-owned surface after the swapchain and before the GPU context (the Vulkan instance).
    gpuContext->waitForIdle();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);
    return 0;
}

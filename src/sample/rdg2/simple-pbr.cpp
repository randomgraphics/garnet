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

static VersionedArtifact updateSsc(SharedShaderConstants * ssc, const Swapchain::Frame & frame, const GpuResourceView & depthView, uint32_t width,
                                   uint32_t height, int frameIdx) {
    // Orbit camera around Y axis; helmet stays fixed at origin.
    const float            orbitAngle = static_cast<float>(frameIdx) * 0.001f;
    constexpr float        kRadius    = 3.f;
    const glm::vec3        eye        = {kRadius * std::sin(orbitAngle), 1.4f, kRadius * std::cos(orbitAngle)};
    static const glm::vec3 kTarget(0.f, 0.f, 0.f), kUp(0.f, 1.f, 0.f);
    const glm::mat4        camToWorld = glm::inverse(glm::lookAtRH(eye, kTarget, kUp));

    // Update per-frame SSC data before takeSnapshot() freezes it into the UBO.
    gpu2::RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget(frame.view));
    rt.setDepthStencilTarget(depthView).setClearColor(0.05f, 0.05f, 0.1f, 1.f).setClearDepth(1.f);

    ssc->set0.renderTarget                = rt;
    ssc->set0.camera.cameraPosition       = eye;
    ssc->set0.camera.cameraOrientation    = glm::quat_cast(glm::mat3(camToWorld));
    ssc->set0.camera.aspectRatio          = static_cast<float>(width) / static_cast<float>(height);
    ssc->set0.frameConstants.frameCounter = frameIdx;

    // Packs current set0 into a staging buffer via a CPU-only graph node.
    return ssc->takeSnapshot();
}

int main(int argc, const char ** argv) {
    bool testMode = (argc > 1) && (argv[1][0] == 't');
    if (testMode) { GN_INFO(sLogger)("Running in test mode"); }

    enableCRTMemoryCheck();

    const uint32_t W = 1280, H = 720;

    // ─── GPU + graph ──────────────────────────────────────────────────────────
    auto gpuContext = GpuContext::create("gpu", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;
    auto graph = Graph::create();
    if (!graph) return -1;

    // ─── Shared shader constants ──────────────────────────────────────────────
    // SSC owns the skybox shaders and env texture loading. Set envLighting paths
    // before the first takeSnapshot(); async IO starts automatically.
    auto ssc = SharedShaderConstants::create({.gpu = gpuContext, .graph = graph});
    if (!ssc) return -1;

    ssc->set0.envLighting = {
        .skyboxPath               = "media::envmap/bad-salzbrunn-walking-hall/skybox-cube.dds",
        .irradiancePath           = "media::envmap/bad-salzbrunn-walking-hall/irradiance.dds",
        .prefilteredPath          = "media::envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
        .brdfLutPath              = "media::envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
        .environmentRadianceScale = 3500.f,
    };

    // Kick off async PBR asset loading. v1 defaults (unit cube + 1×1 matte textures) are
    // published synchronously before this returns; helmetReady is satisfied on frame 1.
    auto helmet = PbrShading::load(gpuContext, graph,
                                   {
                                       .gltfPath       = "media::pbr/DamagedHelmet/DamagedHelmet.gltf",
                                       .albedoPath     = "media::pbr/DamagedHelmet/baseColor_1.jpg",
                                       .normalPath     = "media::pbr/DamagedHelmet/normal_1-gl.jpg",
                                       .emissivePath   = "media::pbr/DamagedHelmet/emissive_1.jpg",
                                       .occlusionPath  = "media::pbr/DamagedHelmet/occlusion_1.jpg",
                                       .metalRoughPath = "media::pbr/DamagedHelmet/metallicRoughness_1.jpg",
                                   });

    // ─── Window + swapchain ───────────────────────────────────────────────────
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        window.reset(win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - PBR (rdg2)", .clientWidth = W, .clientHeight = H}));
        if (!window) return -1;
        window->show();
        surface = window->getVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
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

    // ─── Per-artifact setup state ─────────────────────────────────────────────
    // PBR asset upload is submitted once per version change (v1 on frame 1, v2 after async IO).
    // SSC upload payloads (env textures + UBO) are managed internally and emitted via set0Payloads.
    AutoRef<GpuPayload> lastHelmetPayload;

    int totalFrames = testMode ? 5 : 0;
    int frameIdx    = 0;
    while (totalFrames == 0 || frameIdx < totalFrames) {
        ++frameIdx;
        if (window && !window->runUntilNoNewEvents()) break;

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) return -1;

        VersionedArtifact                         sscSnapshot = updateSsc(ssc, frame, depthView, W, H, frameIdx);
        gpu2::ArrayContainer<AutoRef<GpuPayload>> renderWorks;
        auto                                      drawScene = [&]() {
            auto sc = ssc->getContent(sscSnapshot.artifact);
            if (!sc) return;
            renderWorks.append(sc->set0Payloads);
            auto pbrContent = PbrShading::getContent(graph, helmet.artifact);
            if (!pbrContent) return;

            GpuRaster::CreateParameters rcp;
            rcp.gpu    = gpuContext;
            rcp.target = sc->set0Parameters.renderTarget;

            auto r = GpuRaster::create(rcp);
            if (!r) return;
            glm::mat4 helmetMat = glm::mat4(1.f);
            r->draw(PbrShading::getDrawParams(sc, pbrContent, helmetMat));
            r->draw(ssc->getSkyboxDrawParams(sc->set0Resources));
            renderWorks.append(r->seal());
        };

        // Draw node: pure CPU — records raster commands and seals GpuPayloads.
        // No gpu->submit() here; the single per-frame submit happens below.
        auto drawNode = graph->addNode(
            NodeDesc("draw").dependsOn(sscSnapshot.version).dependsOn(helmet.version).setAction(Action::createFromLambda("draw", drawScene), nullptr));
        graph->waitForToken(graph->getNodeCompletionToken(drawNode));

        // ─── Single frame submit ──────────────────────────────────────────────
        // Payloads ordered: PBR asset upload → set0 (env + UBO) → render work.
        GpuContext::SubmitParameters submit(StrA::format("frame {}", frameIdx));

        // PBR asset: submit upload once per version change.
        auto pbrContent    = PbrShading::getContent(graph, helmet.artifact);
        auto helmetPayload = pbrContent ? pbrContent->gpuPayload : AutoRef<GpuPayload>();
        if (helmetPayload.get() != lastHelmetPayload.get()) {
            if (helmetPayload) submit.appendWork(helmetPayload);
            lastHelmetPayload = helmetPayload;
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

    // Drain the GPU before AutoRef destructors release Vulkan resources.
    gpuContext->waitForIdle();
    return 0;
}

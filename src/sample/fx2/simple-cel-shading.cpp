#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>

using namespace GN;
using namespace GN::fx2;
using namespace GN::gpu2;
using namespace GN::util;

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
    sun.directional.irradiance  = {1.0f, 0.98f, 0.95f, {1.0f}};
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

    ssc->set0.envLighting = {
        .skyboxPath                = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds",
        .irradiancePath            = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds",
        .prefilteredPath           = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
        .brdfLutPath               = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
        .environmentLuminanceScale = 1200.f,
    };

    StrA modelPath = "media::asset-foundry/model/DamagedHelmet/DamagedHelmet.gltf";
    if (argc > 1 && argv[1][0] != 't') {
        modelPath = argv[1];
    } else if (argc > 2) {
        modelPath = argv[2];
    }

    auto modelScene = ModelScene::load({.path = modelPath});
    if (!modelScene) {
        GN_WARN(sLogger, "Failed to load requested model '{}', falling back to DamagedHelmet.gltf", modelPath);
        modelScene = ModelScene::load({.path = "media::asset-foundry/model/DamagedHelmet/DamagedHelmet.gltf"});
        if (!modelScene) return -1;
    }

    CelModelShading::Config celConfig;
    celConfig.shadowThreshold     = 0.5f;
    celConfig.shadowFeather       = 0.02f;
    celConfig.deepShadowThreshold = 0.25f;
    celConfig.deepShadowFeather   = 0.02f;
    celConfig.shadowTint          = glm::vec3(0.6f, 0.65f, 0.8f);
    celConfig.deepShadowTint      = glm::vec3(0.4f, 0.42f, 0.55f);
    celConfig.specularThreshold   = 0.75f;
    celConfig.specularShininess   = 40.0f;
    celConfig.specularIntensity   = 1.2f;
    celConfig.rimIntensity        = 0.6f;
    celConfig.rimThreshold        = 0.65f;
    celConfig.rimFeather          = 0.05f;
    celConfig.rimTint             = glm::vec3(1.0f, 0.98f, 1.0f);
    celConfig.outlineWidth        = 0.0035f;
    celConfig.outlineColor        = glm::vec4(0.12f, 0.1f, 0.14f, 1.0f);

    auto celShading = CelModelShading::create(gpuContext, celConfig);
    if (!celShading) return -1;
    auto modelAsset = ModelAsset::create(gpuContext, modelScene);
    if (!modelAsset) return -1;

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

    bool celShadingUploadSubmitted = false;
    bool modelUploadSubmitted      = false;

    int totalFrames = testMode ? 5 : 0;
    int frameIdx    = 0;
    while (totalFrames == 0 || frameIdx < totalFrames) {
        ++frameIdx;
        if (window && !window->runUntilNoNewEvents()) break;

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) return -1;

        rasterTarget.setColorTarget(0, frame.view);
        SharedShaderConstants::Snapshot sscSnapshot = updateSsc(ssc, rasterTarget, frameIdx);
        DynaArray<AutoRef<GpuPayload>>  renderWorks;

        auto drawScene = [&]() {
            renderWorks.append(sscSnapshot.set0Payloads);

            GpuRaster::CreateParameters rcp;
            rcp.gpu    = gpuContext;
            rcp.target = &rasterTarget;

            auto r = GpuRaster::create("simple-cel", rcp);
            if (!r) return;

            DynaArray<glm::mat4> nodeTransforms;
            nodeTransforms.resize(modelScene->nodes.size());

            auto renderPrimitive = [&](uint32_t primIndex, const glm::mat4 & transform) {
                // Pass 1: Surface cel shading (cull back)
                const auto surfaceDraw = CelModelShading::getDrawParams(sscSnapshot, celShading, modelAsset, primIndex, transform);
                if (surfaceDraw.vs && surfaceDraw.ps) r->draw(surfaceDraw);

                // Pass 2: Inverted-hull outline (cull front)
                const auto outlineDraw = CelModelShading::getOutlineDrawParams(sscSnapshot, celShading, modelAsset, primIndex, transform);
                if (outlineDraw.vs && outlineDraw.ps) r->draw(outlineDraw);
            };

            if (modelScene->nodes.empty()) {
                renderPrimitive(0, glm::mat4(1.0f));
            } else {
                for (size_t nodeIndex = 0; nodeIndex < modelScene->nodes.size(); ++nodeIndex) {
                    const auto &    node           = modelScene->nodes[nodeIndex];
                    const glm::mat4 parentToWorld  = node.parent >= 0 ? nodeTransforms[static_cast<size_t>(node.parent)] : glm::mat4(1.f);
                    const glm::mat4 worldTransform = parentToWorld * node.transform;
                    nodeTransforms[nodeIndex]      = worldTransform;
                    for (uint32_t primitiveIndex : node.primitives) { renderPrimitive(primitiveIndex, worldTransform); }
                }
            }

            r->draw(ssc->getSkyboxDrawParams(sscSnapshot.set0Resources));
            renderWorks.append(r->seal());
        };
        drawScene();

        GpuContext::SubmitParameters submit(StrA::format("frame {}", frameIdx));

        if (!celShadingUploadSubmitted) {
            if (const auto payload = celShading->uploadPayload()) submit.appendWork(payload);
            celShadingUploadSubmitted = true;
        }
        if (!modelUploadSubmitted) {
            if (const auto payload = modelAsset->uploadPayload()) submit.appendWork(payload);
            modelUploadSubmitted = true;
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

    gpuContext->waitForIdle();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);
    return 0;
}

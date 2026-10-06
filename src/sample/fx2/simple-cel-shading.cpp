#include "sample-sphere.h"
#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <memory>
#include <cmath>

using namespace GN;
using namespace GN::gpu2;
namespace fxBindless  = GN::fx2::bindless;
namespace gpuBindless = GN::gpu2::bindless;

namespace {
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
    host.gpu = GpuContext::create("cel-shading-sample", {});
    if (!host.gpu) return 1;

    uint32_t width = 1280, height = 720;
    if (!headless) {
        host.window.reset(win::createWindow({.caption = "FX2 bindless Cel / Anime NPR shading", .clientWidth = width, .clientHeight = height}));
        if (!host.window) return 1;
        host.window->show();
        host.surface = host.window->createVulkanSurfaceHandle(host.gpu->getVulkanInstanceHandle());
        if (!host.surface) return 1;
        const auto size = host.window->getClientSize();
        if (size.x != 0 && size.y != 0) {
            width  = size.x;
            height = size.y;
        }
    }

    Swapchain::CreateDesc scDesc {.gpu = host.gpu, .width = width, .height = height};
    scDesc.setSurface(host.surface);
    host.swapchain = Swapchain::create(scDesc);

    auto heap = gpuBindless::DescriptorHeap::create("cel.heap", {.gpu = host.gpu, .capacity = 32});
    if (!heap) return 1;
    auto upload = gpuBindless::CnC::create("cel.initialization", {.gpu = host.gpu, .heap = heap});
    if (!upload) return 1;

    auto cel       = fxBindless::CelKernel::create(*heap, *upload);
    auto constants = fxBindless::SharedShaderConstants::create({.gpu = host.gpu, .uniformCapacity = 64 * 1024, .streamingCapacity = 64 * 1024});
    if (!host.swapchain || !heap || !cel || !constants) return 1;

    // Depth buffer
    const auto depthDesc   = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(width, height).setLevels(1);
    auto       depthBuffer = Texture::create("cel.depth", {.context = host.gpu, .descriptor = depthDesc});
    if (!depthBuffer) return 1;
    GpuResourceView depthView(depthBuffer);

    // Create sample sphere mesh
    RasterGeometry sphereGeometry = createSampleSphere(host.gpu, *upload);
    if (!sphereGeometry.indexCount) return 1;

    // Stylized anime NPR material tuning
    auto celParams                = cel->defaultMaterialParameters();
    celParams.color               = {0.9f, 0.45f, 0.25f, 1.0f};
    celParams.shadowThreshold     = 0.5f;
    celParams.shadowFeather       = 0.02f;
    celParams.deepShadowThreshold = 0.25f;
    celParams.deepShadowFeather   = 0.02f;
    celParams.shadowTint          = {0.55f, 0.55f, 0.72f};
    celParams.deepShadowTint      = {0.35f, 0.35f, 0.50f};
    celParams.specularThreshold   = 0.75f;
    celParams.specularShininess   = 40.0f;
    celParams.specularIntensity   = 1.2f;
    celParams.rimThreshold        = 0.60f;
    celParams.rimFeather          = 0.05f;
    celParams.rimIntensity        = 0.75f;
    celParams.rimTint             = {1.0f, 0.95f, 0.9f};
    celParams.outlineWidth        = 0.008f;
    celParams.outlineColor        = {0.12f, 0.10f, 0.15f, 1.0f};

    auto material = cel->createMaterial(*upload, celParams);
    if (!material) return 1;

    auto initializationWork = upload->seal();
    if (!initializationWork) return 1;

    RasterTarget target;
    target.setDepthStencilTarget(depthView);
    target.states.cullMode   = RasterState::CULL_BACK;
    target.states.frontFace  = RasterState::FRONT_CCW;
    target.states.depthState = RasterState::DepthState {RasterState::Compare::LESS_EQUAL, true};

    int totalFrames = headless ? 5 : 0;
    int frameIdx    = 0;

    while (totalFrames == 0 || frameIdx < totalFrames) {
        ++frameIdx;
        if (host.window && !host.window->runUntilNoNewEvents()) break;

        auto frame = host.swapchain->prepare();
        if (frame.view.empty()) return 1;

        target.setColorTarget(0, frame.view).setClearColor(0.12f, 0.14f, 0.18f, 1.0f).setClearDepth(1.0f);

        // Orbit camera around Y axis
        const float     angle  = static_cast<float>(frameIdx) * 0.015f;
        constexpr float radius = 2.8f;
        const glm::vec3 eye {radius * std::sin(angle), 0.8f, radius * std::cos(angle)};

        fxBindless::SharedUniforms uniforms {};
        uniforms.frameCounter     = static_cast<uint32_t>(frameIdx);
        uniforms.renderTargetSize = {float(width), float(height)};
        uniforms.cameraPosition   = {eye.x, eye.y, eye.z, 1.0f};
        uniforms.viewMatrix       = glm::lookAtRH(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        uniforms.projMatrix       = glm::perspectiveRH_ZO(glm::radians(45.0f), float(width) / float(height), uniforms.nearPlane, uniforms.farPlane);
        uniforms.projMatrix[1][1] *= -1; // Vulkan clip space
        uniforms.projViewMatrix = uniforms.projMatrix * uniforms.viewMatrix;
        uniforms.exposure       = 1.0f;

        // Directional sun light
        uniforms.numLights               = 1;
        uniforms.lights[0].positionOrDir = {0.6f, 0.8f, 0.5f, float(fxBindless::DirectLightUniform::DIRECTIONAL)};
        uniforms.lights[0].colorAndRange = {1.8f, 1.75f, 1.6f, 0.0f};

        auto frameProducer = gpuBindless::CnC::create("cel.uniform", {.gpu = host.gpu, .heap = heap});
        if (!frameProducer) return 1;
        auto state = constants->recordUniformUpdate(*frameProducer, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
        if (!state) return 1;
        auto uniformWork = frameProducer->seal();
        if (!uniformWork) return 1;

        auto raster = gpuBindless::Raster::create(
            "cel.raster",
            {.gpu = host.gpu, .target = &target, .heap = heap, .heapSetIndex = 0, .passResources = sharedUniformResources(state), .numberOfDrawsHint = 2});
        if (!raster) return 1;

        fxBindless::CelMaterial::DrawParameters draw {{*raster, state, sphereGeometry, &target.states}};
        draw.object2WorldTransform = glm::mat4(1.0f);
        draw.renderOutline         = true;
        if (!material->record(draw)) return 1;

        auto rasterWork = raster->seal();
        if (!rasterWork) return 1;

        GpuContext::SubmitParameters submit("cel.frame");
        if (initializationWork) {
            submit.appendWork(initializationWork);
            initializationWork.clear();
        }
        submit.appendWork(uniformWork).appendWork(rasterWork).waitFor(frame.ready);
        host.gpu->submit(submit);

        host.swapchain->present(*rasterWork);
    }

    if (host.gpu) host.gpu->waitForIdle();
    return 0;
}

#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
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
    constexpr uint32_t width = 640, height = 480;
    if (!headless) {
        host.window.reset(win::createWindow({.caption = "FX2 bindless PBR kernel", .clientWidth = width, .clientHeight = height}));
        if (!host.window) return 1;
        host.window->show();
        host.surface = host.window->createVulkanSurfaceHandle(host.gpu->getVulkanInstanceHandle());
        if (!host.surface) return 1;
    }
    Swapchain::CreateDesc swapchain {.gpu = host.gpu, .width = width, .height = height};
    swapchain.setSurface(host.surface);
    host.swapchain = Swapchain::create(swapchain);
    auto heap      = gpuBindless::DescriptorHeap::create("pbr.heap", {.gpu = host.gpu, .capacity = 32});
    if (!heap) return 1;
    auto upload = gpuBindless::CnC::create("pbr.initialization", {.gpu = host.gpu, .heap = heap});
    if (!upload) return 1;
    auto pbr       = fxBindless::PbrKernel::create(*heap, *upload);
    auto constants = fxBindless::SharedShaderConstants::create({.gpu = host.gpu, .uniformCapacity = 64 * 1024, .streamingCapacity = 64 * 1024});
    if (!host.swapchain || !heap || !pbr || !constants) return 1;

    fxBindless::SharedUniforms uniforms;
    uniforms.cameraPosition          = {0, 0, 3, 1};
    uniforms.numLights               = 1;
    uniforms.lights[0].positionOrDir = {0.4f, -0.6f, -1.0f, float(fxBindless::DirectLightUniform::DIRECTIONAL)};
    uniforms.lights[0].colorAndRange = {4.0f, 3.8f, 3.4f, 0.0f};
    uniforms.renderTargetSize        = {width, height};
    uniforms.viewMatrix              = glm::lookAtRH(glm::vec3(uniforms.cameraPosition), glm::vec3(0), glm::vec3(0, 1, 0));
    uniforms.projMatrix              = glm::perspectiveRH_ZO(glm::radians(60.f), float(width) / height, uniforms.nearPlane, uniforms.farPlane);
    uniforms.projMatrix[1][1] *= -1; // Vulkan clip space inverts the projection's Y axis.
    uniforms.projViewMatrix = uniforms.projMatrix * uniforms.viewMatrix;

    // Interleaved position/UV data keeps this sample independent of mesh/asset helpers.
    const float positions[] = {-0.6f, -0.7f, 0, 0, 0, 1, 0, 1, 0.6f, -0.7f, 0, 0, 0, 1, 1, 1, 0, 0.7f, 0, 0, 0, 1, 0.5f, 0};
    auto        vertices    = Buffer::create("pbr.positions", {.context = host.gpu, .size = sizeof(positions)});
    if (!vertices || !upload) return 1;
    upload->recordUploadBuffer(vertices, 0, {reinterpret_cast<const uint8_t *>(positions), sizeof(positions)});
    uint8_t pixels[4 * 4 * 4] = {};
    for (uint32_t y = 0; y < 4; ++y) {
        for (uint32_t x = 0; x < 4; ++x) {
            const auto    offset = (y * 4 + x) * 4;
            const uint8_t value  = ((x + y) & 1) ? 255 : 0;
            pixels[offset] = pixels[offset + 1] = pixels[offset + 2] = value;
            pixels[offset + 3]                                       = 255;
        }
    }
    auto checker = Texture::create(
        "pbr.base-color-checker",
        {.context = host.gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(4, 4).setLevels(1)});
    using Filter  = Sampler::Descriptor::Filter;
    using Address = Sampler::Descriptor::Address;
    auto nearest  = Sampler::create("pbr.nearest", {.context    = host.gpu,
                                                    .descriptor = {.minFilter = Filter::NEAREST,
                                                                   .magFilter = Filter::NEAREST,
                                                                   .mipFilter = Filter::NEAREST,
                                                                   .addressU  = Address::CLAMP_TO_EDGE,
                                                                   .addressV  = Address::CLAMP_TO_EDGE,
                                                                   .addressW  = Address::CLAMP_TO_EDGE,
                                                                   .maxLod    = 0}});
    if (!checker || !nearest) return 1;
    gpuBindless::CnC::Region region;
    region.imageExtent = {4, 4, 1};
    upload->recordUploadImage(checker, {pixels, sizeof(pixels)}, {&region, 1});
    auto materialParameters         = pbr->defaultMaterialParameters();
    materialParameters.baseColorMap = GpuResourceView(checker).setImageViewType(GpuResourceView::ImageView::SAMPLED);
    materialParameters.sampler      = nearest;
    materialParameters.metallic     = 0.05f;
    materialParameters.roughness    = 0.8f;
    auto leftMaterial               = pbr->createMaterial(*upload, materialParameters);
    materialParameters.baseColor    = {0.1f, 0.7f, 1, 1};
    materialParameters.metallic     = 0.85f;
    materialParameters.roughness    = 0.22f;
    auto rightMaterial              = pbr->createMaterial(*upload, materialParameters);
    if (!leftMaterial || !rightMaterial) return 1;
    auto initialization = upload->seal();
    if (!initialization) return 1;

    RasterGeometry geometry;
    geometry.vertices.push_back({.buffer = vertices, .offset = 0, .stride = 8 * sizeof(float)});
    geometry.format.attributes.push_back(
        {.location = fxBindless::PbrMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = fxBindless::PbrMaterial::NORMAL_LOCATION, .binding = 0, .offset = 3 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_3});
    geometry.format.attributes.push_back(
        {.location = fxBindless::PbrMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = 6 * sizeof(float), .format = RasterGeometry::AttributeFormat::F32_2});
    geometry.vertexCount = 3;

    for (int frame = 0; !headless || frame < 3; ++frame) {
        if (host.window && !host.window->runUntilNoNewEvents()) break;
        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;
        RasterTarget target;
        target.setColorTarget(0, acquired.view).setClearColor(0.02f, 0.03f, 0.05f, 1);
        target.states.cullMode = RasterState::CULL_NONE;
        auto uniformUploads    = gpuBindless::CnC::create("pbr.uniform-upload", {.gpu = host.gpu, .heap = heap});
        if (!uniformUploads) return 1;
        uniforms.frameCounter = static_cast<uint32_t>(frame);
        auto state            = constants->recordUniformUpdate(*uniformUploads, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
        if (!state) return 1;
        auto uniformWork = uniformUploads->seal();
        if (!uniformWork) return 1;
        auto raster = gpuBindless::Raster::create("pbr.two-materials", {.gpu           = host.gpu,
                                                                        .target        = &target,
                                                                        .heap          = heap,
                                                                        .heapSetIndex  = 0,
                                                                        .passResources = fxBindless::sharedUniformResources(state),

                                                                        .numberOfDrawsHint = 2});
        if (!raster) return 1;
        const fxBindless::PbrMaterial::DrawParameters leftDraw {{*raster, state, geometry}, glm::translate(glm::mat4(1), glm::vec3(-0.7f, 0, 0))};
        const fxBindless::PbrMaterial::DrawParameters rightDraw {{*raster, state, geometry}, glm::translate(glm::mat4(1), glm::vec3(0.7f, 0, 0))};
        if (!leftMaterial->record(leftDraw)) return 1;
        if (!rightMaterial->record(rightDraw)) return 1;
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

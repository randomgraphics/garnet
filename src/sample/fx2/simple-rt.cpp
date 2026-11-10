#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

using namespace GN;
using namespace GN::gpu2;
namespace fx  = GN::fx2;
namespace gpu = GN::gpu2;

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

struct Vertex {
    float position[3];
    float normal[3];
    float uv[2];
};

struct MeshBuffers {
    AutoRef<Buffer> vertices;
    AutoRef<Buffer> indices;
    RasterGeometry  geometry;
};

MeshBuffers makeMesh(CnC & upload, AutoRef<GpuContext> gpu, const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices) {
    MeshBuffers result;
    result.vertices = Buffer::create("rt.mesh.vertices", {.context = gpu, .size = vertices.size() * sizeof(Vertex), .accelerationStructureBuildInput = true});
    result.indices  = Buffer::create("rt.mesh.indices", {.context = gpu, .size = indices.size() * sizeof(uint32_t), .accelerationStructureBuildInput = true});
    if (!result.vertices || !result.indices) return {};
    upload.recordUploadBuffer(result.vertices, 0, {reinterpret_cast<const uint8_t *>(vertices.data()), vertices.size() * sizeof(Vertex)});
    upload.recordUploadBuffer(result.indices, 0, {reinterpret_cast<const uint8_t *>(indices.data()), indices.size() * sizeof(uint32_t)});
    result.geometry.vertices.push_back({.buffer = result.vertices, .offset = 0, .stride = sizeof(Vertex)});
    result.geometry.format.attributes = {
        {.location = fx::PbrMaterial::POSITION_LOCATION, .binding = 0, .offset = offsetof(Vertex, position), .format = RasterGeometry::AttributeFormat::F32_3},
        {.location = fx::PbrMaterial::NORMAL_LOCATION, .binding = 0, .offset = offsetof(Vertex, normal), .format = RasterGeometry::AttributeFormat::F32_3},
        {.location = fx::PbrMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = offsetof(Vertex, uv), .format = RasterGeometry::AttributeFormat::F32_2},
    };
    result.geometry.vertexCount    = static_cast<uint32_t>(vertices.size());
    result.geometry.indices.buffer = result.indices;
    result.geometry.indices.stride = sizeof(uint32_t);
    result.geometry.indexCount     = static_cast<uint32_t>(indices.size());
    return result;
}

void makeSphere(std::vector<Vertex> & vertices, std::vector<uint32_t> & indices) {
    constexpr uint32_t rings = 20, slices = 32;
    for (uint32_t y = 0; y <= rings; ++y) {
        const float v     = float(y) / rings;
        const float theta = v * glm::pi<float>();
        for (uint32_t x = 0; x <= slices; ++x) {
            const float     u   = float(x) / slices;
            const float     phi = u * glm::two_pi<float>();
            const glm::vec3 n(std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi));
            const glm::vec3 p = glm::vec3(0, 0.85f, 0) + n * 0.85f;
            vertices.push_back({{p.x, p.y, p.z}, {n.x, n.y, n.z}, {u, v}});
        }
    }
    for (uint32_t y = 0; y < rings; ++y)
        for (uint32_t x = 0; x < slices; ++x) {
            const uint32_t a = y * (slices + 1) + x, b = a + slices + 1;
            indices.insert(indices.end(), {a, b, a + 1, a + 1, b, b + 1});
        }
}

void makeFloor(std::vector<Vertex> & vertices, std::vector<uint32_t> & indices) {
    vertices = {{{-8, 0, -8}, {0, 1, 0}, {0, 0}}, {{-8, 0, 8}, {0, 1, 0}, {0, 1}}, {{8, 0, 8}, {0, 1, 0}, {1, 1}}, {{8, 0, -8}, {0, 1, 0}, {1, 0}}};
    indices  = {0, 1, 2, 0, 2, 3};
}

} // namespace

int main(int argc, const char * argv[]) {
    bool directLight = true;
    bool rasterMode  = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--no-direct-light") == 0)
            directLight = false;
        else if (std::strcmp(argv[i], "--direct-light") == 0)
            directLight = true;
        else if (std::strcmp(argv[i], "--raster") == 0)
            rasterMode = true;
        else if (std::strcmp(argv[i], "--headless") != 0) {
            std::fprintf(stderr, "Usage: GNsample-fx2-raytracing [--raster] [--no-direct-light] [--headless]\n");
            return 1;
        }
    }
    bool headless = false;
    for (int i = 1; i < argc; ++i)
        if (std::strcmp(argv[i], "--headless") == 0) headless = true;
    constexpr uint32_t width = 1280, height = 720;
    Host               host;
    host.gpu = GpuContext::create("fx2-raytracing-sample", {});
    if (!host.gpu) return 1;
    if (!rasterMode && !host.gpu->caps().rayQueries) {
        std::fprintf(stderr, "This GPU does not support Vulkan ray queries.\n");
        return 1;
    }
    if (!headless) {
        host.window.reset(win::createWindow({.caption = "FX2 ray tracing: L toggles direct light", .clientWidth = width, .clientHeight = height}));
        if (!host.window) return 1;
        host.window->show();
        host.surface = host.window->createVulkanSurfaceHandle(host.gpu->getVulkanInstanceHandle());
        if (!host.surface) return 1;
    }
    Swapchain::CreateDesc swapchainDesc {.gpu = host.gpu, .width = width, .height = height};
    swapchainDesc.setSurface(host.surface);
    host.swapchain = Swapchain::create(swapchainDesc);
    if (!host.swapchain) return 1;

    auto heap      = DescriptorHeap::create("rt.heap", {.gpu = host.gpu, .capacity = 128});
    auto init      = CnC::create("rt.init", {.gpu = host.gpu, .heap = heap});
    auto kernel    = rasterMode ? AutoRef<fx::RayTraceKernel>() : fx::RayTraceKernel::create(*heap, *init);
    auto pbr       = fx::PbrKernel::create(*heap, *init);
    auto skyKernel = fx::SkyKernel::create(*heap, *init);
    auto unlit     = fx::UnlitKernel::create(*heap, *init);
    auto constants = fx::SharedShaderConstants::create({.gpu = host.gpu, .uniformCapacity = 64 * 1024, .streamingCapacity = 64 * 1024});
    if (!heap || !init || (!rasterMode && !kernel) || !pbr || !skyKernel || !unlit || !constants) return 1;

    std::vector<Vertex>   sphereVertices, floorVertices;
    std::vector<uint32_t> sphereIndices, floorIndices;
    makeSphere(sphereVertices, sphereIndices);
    makeFloor(floorVertices, floorIndices);
    auto sphere = makeMesh(*init, host.gpu, sphereVertices, sphereIndices);
    auto floor  = makeMesh(*init, host.gpu, floorVertices, floorIndices);
    if (!sphere.vertices || !floor.vertices) return 1;
    fx::PbrMaterial::Parameters whiteParams;
    whiteParams.baseColor = {0.82f, 0.82f, 0.82f, 1};
    whiteParams.roughness = 1.0f;
    whiteParams.metallic  = 0;
    auto white            = fx::PbrMaterial::create(*heap, *init, whiteParams);
    if (!white) return 1;
    const std::array<fx::PbrRayTracingScene::Mesh, 2> sceneMeshes {{
        {&sphere.geometry, white.get(), glm::mat4(1)},
        {&floor.geometry, white.get(), glm::mat4(1)},
    }};
    auto                                              scene = fx::PbrRayTracingScene::create(*heap, *init, {sceneMeshes.data(), sceneMeshes.size()});
    if (!scene) return 1;

    auto skybox      = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox.dds"});
    auto irradiance  = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds"});
    auto prefiltered = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds"});
    auto brdf        = Texture::load({.context = host.gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds"});
    if (!skybox || !irradiance || !prefiltered || !brdf) return 1;
    fx::SkyMaterial::Parameters skyParams;
    skyParams.skyboxMap      = GpuResourceView {skybox}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    skyParams.irradianceMap  = GpuResourceView {irradiance}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    skyParams.prefilteredMap = GpuResourceView {prefiltered}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    skyParams.brdfLut        = GpuResourceView {brdf}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    auto sky                 = fx::SkyMaterial::create(*heap, *init, skyParams);
    if (!sky) return 1;

    const auto outputDesc = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_16_16_16_16_FLOAT()).setDimensions(width, height).setLevels(1);
    auto       output     = Texture::create("rt.output", {.context = host.gpu, .descriptor = outputDesc});
    fx::UnlitMaterial::Parameters outputParams;
    outputParams.colorMap = GpuResourceView {output}.setImageViewType(GpuResourceView::ImageView::SAMPLED);
    auto outputMaterial   = fx::UnlitMaterial::create(*heap, *init, outputParams);
    if (!output || !outputMaterial) return 1;
    struct QuadVertex {
        glm::vec3 position;
        glm::vec2 uv;
    };
    constexpr std::array<QuadVertex, 6> quad {
        {{{-1, -1, 0.5f}, {0, 0}}, {{1, -1, 0.5f}, {1, 0}}, {{1, 1, 0.5f}, {1, 1}}, {{-1, -1, 0.5f}, {0, 0}}, {{1, 1, 0.5f}, {1, 1}}, {{-1, 1, 0.5f}, {0, 1}}}};
    auto quadBuffer = Buffer::create("rt.quad", {.context = host.gpu, .size = sizeof(quad)});
    if (!quadBuffer) return 1;
    init->recordUploadBuffer(quadBuffer, 0, {reinterpret_cast<const uint8_t *>(quad.data()), sizeof(quad)});
    RasterGeometry quadGeometry;
    quadGeometry.vertices.push_back({.buffer = quadBuffer, .offset = 0, .stride = sizeof(QuadVertex)});
    quadGeometry.format.attributes = {
        {.location = fx::UnlitMaterial::POSITION_LOCATION,
         .binding  = 0,
         .offset   = offsetof(QuadVertex, position),
         .format   = RasterGeometry::AttributeFormat::F32_3},
        {.location = fx::UnlitMaterial::TEXCOORD_LOCATION, .binding = 0, .offset = offsetof(QuadVertex, uv), .format = RasterGeometry::AttributeFormat::F32_2}};
    quadGeometry.vertexCount = 6;
    const auto depthDesc     = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(width, height).setLevels(1);
    auto       depth         = Texture::create("rt.depth", {.context = host.gpu, .descriptor = depthDesc});
    if (!depth) return 1;
    auto initWork = init->seal();
    if (!initWork) return 1;
    GpuContext::SubmitParameters initSubmit("rt.init-submit");
    initSubmit.appendWork(initWork);
    host.gpu->submit(initSubmit);
    host.gpu->waitForIdle();

    bool keyWasDown = false;
    for (int frame = 0; !headless || frame < 3; ++frame) {
        if (host.window) {
            if (!host.window->runUntilNoNewEvents()) break;
            const bool keyDown = host.window->getKeyStatus(win::KeyCode::L).down;
            if (keyDown && !keyWasDown) {
                directLight = !directLight;
                std::fprintf(stdout, "Direct light %s\n", directLight ? "on" : "off");
            }
            keyWasDown = keyDown;
            if (host.window->getKeyStatus(win::KeyCode::ESCAPE).down) break;
        }
        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;
        fx::SharedUniforms u;
        u.cameraPosition          = {4.2f, 3.5f, 6.0f, 1};
        u.numLights               = directLight ? 1 : 0;
        u.lights[0].positionOrDir = {0.45f, 1.0f, 0.35f, float(fx::DirectLightUniform::DIRECTIONAL)};
        u.lights[0].colorAndRange = {3.0f, 3.0f, 3.0f, 0};
        u.exposure                = 1.0f;
        u.renderTargetSize        = {float(width), float(height)};
        u.viewMatrix              = glm::lookAtRH(glm::vec3(u.cameraPosition), {0, 0.85f, 0}, {0, 1, 0});
        u.projMatrix              = glm::perspectiveRH_ZO(glm::radians(55.0f), float(width) / height, u.nearPlane, u.farPlane);
        u.projMatrix[1][1] *= -1;
        u.projViewMatrix   = u.projMatrix * u.viewMatrix;
        auto uniformUpload = CnC::create("rt.uniforms", {.gpu = host.gpu, .heap = heap});
        auto state         = constants->recordUniformUpdate(*uniformUpload, {reinterpret_cast<const uint8_t *>(&u), sizeof(u)});
        auto uniformWork   = uniformUpload->seal();
        if (rasterMode) {
            RasterTarget target;
            target.setColorTarget(0, acquired.view).setClearColor(0, 0, 0, 1);
            target.setDepthStencilTarget(GpuResourceView {depth}).setClearDepth(1);
            auto raster =
                Raster::create("rt.raster-reference",
                               {.gpu = host.gpu, .target = &target, .heap = heap, .passResources = fx::sharedUniformResources(state), .numberOfDrawsHint = 3});
            if (!raster) return 1;
            RasterState skyState;
            skyState.cullMode   = RasterState::CULL_NONE;
            skyState.depthState = RasterState::DepthState {.func = RasterState::Compare::ALWAYS, .write = false};
            RasterState objectState;
            objectState.cullMode   = RasterState::CULL_BACK;
            objectState.frontFace  = RasterState::FRONT_CCW;
            objectState.depthState = RasterState::DepthState {.func = RasterState::Compare::LESS_EQUAL, .write = true};
            if (!skyKernel->record(*sky, {*raster, state, &skyState})) return 1;
            fx::PbrMaterial::DrawParameters sphereDraw {{*raster, state, sphere.geometry, &objectState}, glm::mat4(1), sky};
            fx::PbrMaterial::DrawParameters floorDraw {{*raster, state, floor.geometry, &objectState}, glm::mat4(1), sky};
            if (!pbr->record(*white, sphereDraw) || !pbr->record(*white, floorDraw)) return 1;
            auto draws = raster->seal();
            if (!uniformWork || !draws) return 1;
            GpuContext::SubmitParameters submission("rt.raster-reference-frame");
            submission.appendWork(uniformWork).appendWork(draws).waitFor(acquired.ready);
            host.gpu->submit(submission);
            host.swapchain->present(*draws);
            continue;
        }

        auto rtResources = fx::RayTraceKernel::createPassResources(*scene, state, output);
        auto rt          = CnC::create("rt.trace", {.gpu = host.gpu, .heap = heap, .passResources = rtResources});
        if (!kernel->record({*rt, *scene, sky, state, output, width, height})) return 1;
        auto         rtWork = rt->seal();
        RasterTarget target;
        target.setColorTarget(0, acquired.view).setClearColor(0, 0, 0, 1);
        target.setDepthStencilTarget(GpuResourceView {depth}).setClearDepth(1);
        auto compositeResources = fx::sharedUniformResources(state);
        compositeResources[1].resize(3);
        compositeResources[1][2].append(GpuResourceView {output}.setImageViewType(GpuResourceView::ImageView::SAMPLED));
        auto raster =
            Raster::create("rt.composite", {.gpu = host.gpu, .target = &target, .heap = heap, .passResources = compositeResources, .numberOfDrawsHint = 1});
        RasterState rs;
        rs.cullMode = RasterState::CULL_NONE;
        fx::UnlitMaterial::DrawParameters draw {{*raster, state, quadGeometry, &rs}, glm::inverse(u.projViewMatrix)};
        if (!uniformWork || !rtWork || !raster || !unlit->record(*outputMaterial, draw)) return 1;
        auto                         draws = raster->seal();
        GpuContext::SubmitParameters submission("rt.frame");
        submission.appendWork(uniformWork).appendWork(rtWork).appendWork(draws).waitFor(acquired.ready);
        host.gpu->submit(submission);
        host.swapchain->present(*draws);
    }
    return 0;
}

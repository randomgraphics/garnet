#include "pch.h"
#include <garnet/GNbase.h>
#include <garnet/GNwin.h>
#include <garnet/base/filesys.h>

using namespace GN;
using namespace GN::rdg;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.render-graph");

// -------------------------------------------------------------------------
// Box mesh geometry
// -------------------------------------------------------------------------

struct Vertex {
    float px, py, pz; // position
    float nx, ny, nz; // normal
    float u, v;       // texcoord
};
static_assert(sizeof(Vertex) == 32);

// One quad face = 2 triangles (6 vertices), winding CCW viewed from outside.
// clang-format off
static const Vertex kBoxVertices[] = {
    // Front face (+Z, normal 0,0,1)
    {-0.5f,-0.5f, 0.5f,  0,0,1,  0,1},
    { 0.5f,-0.5f, 0.5f,  0,0,1,  1,1},
    { 0.5f, 0.5f, 0.5f,  0,0,1,  1,0},
    {-0.5f,-0.5f, 0.5f,  0,0,1,  0,1},
    { 0.5f, 0.5f, 0.5f,  0,0,1,  1,0},
    {-0.5f, 0.5f, 0.5f,  0,0,1,  0,0},
    // Back face (-Z, normal 0,0,-1)
    { 0.5f,-0.5f,-0.5f,  0,0,-1,  0,1},
    {-0.5f,-0.5f,-0.5f,  0,0,-1,  1,1},
    {-0.5f, 0.5f,-0.5f,  0,0,-1,  1,0},
    { 0.5f,-0.5f,-0.5f,  0,0,-1,  0,1},
    {-0.5f, 0.5f,-0.5f,  0,0,-1,  1,0},
    { 0.5f, 0.5f,-0.5f,  0,0,-1,  0,0},
    // Right face (+X, normal 1,0,0)
    { 0.5f,-0.5f, 0.5f,  1,0,0,  0,1},
    { 0.5f,-0.5f,-0.5f,  1,0,0,  1,1},
    { 0.5f, 0.5f,-0.5f,  1,0,0,  1,0},
    { 0.5f,-0.5f, 0.5f,  1,0,0,  0,1},
    { 0.5f, 0.5f,-0.5f,  1,0,0,  1,0},
    { 0.5f, 0.5f, 0.5f,  1,0,0,  0,0},
    // Left face (-X, normal -1,0,0)
    {-0.5f,-0.5f,-0.5f, -1,0,0,  0,1},
    {-0.5f,-0.5f, 0.5f, -1,0,0,  1,1},
    {-0.5f, 0.5f, 0.5f, -1,0,0,  1,0},
    {-0.5f,-0.5f,-0.5f, -1,0,0,  0,1},
    {-0.5f, 0.5f, 0.5f, -1,0,0,  1,0},
    {-0.5f, 0.5f,-0.5f, -1,0,0,  0,0},
    // Top face (+Y, normal 0,1,0)
    {-0.5f, 0.5f, 0.5f,  0,1,0,  0,1},
    { 0.5f, 0.5f, 0.5f,  0,1,0,  1,1},
    { 0.5f, 0.5f,-0.5f,  0,1,0,  1,0},
    {-0.5f, 0.5f, 0.5f,  0,1,0,  0,1},
    { 0.5f, 0.5f,-0.5f,  0,1,0,  1,0},
    {-0.5f, 0.5f,-0.5f,  0,1,0,  0,0},
    // Bottom face (-Y, normal 0,-1,0)
    {-0.5f,-0.5f,-0.5f,  0,-1,0,  0,1},
    { 0.5f,-0.5f,-0.5f,  0,-1,0,  1,1},
    { 0.5f,-0.5f, 0.5f,  0,-1,0,  1,0},
    {-0.5f,-0.5f,-0.5f,  0,-1,0,  0,1},
    { 0.5f,-0.5f, 0.5f,  0,-1,0,  1,0},
    {-0.5f,-0.5f, 0.5f,  0,-1,0,  0,0},
};
// clang-format on

// -------------------------------------------------------------------------
// Helpers
// -------------------------------------------------------------------------

static AutoRef<Buffer> createBoxVertexBuffer(ArtifactDatabase & db, AutoRef<GpuContext> gpu) {
    auto buf = PersistentBuffer::create(db, "box_vertex_buffer", PersistentBuffer::CreateParameters {.context = gpu, .size = sizeof(kBoxVertices)});
    if (!buf) {
        GN_ERROR(sLogger)("Failed to create box vertex buffer");
        return {};
    }
    if (!buf->setContent(kBoxVertices, sizeof(kBoxVertices))) {
        GN_ERROR(sLogger)("Failed to upload box vertex data");
        return {};
    }
    return buf;
}

static GpuDraw::GpuGeometry buildBoxGeometry(AutoRef<Buffer> vertexBuffer) {
    GpuDraw::GpuGeometry geom;
    geom.format.attributes.append(GpuDraw::GpuGeometry::VertexAttribute {0, 0, GpuDraw::GpuGeometry::AttributeFormat::F32_3});  // position
    geom.format.attributes.append(GpuDraw::GpuGeometry::VertexAttribute {1, 12, GpuDraw::GpuGeometry::AttributeFormat::F32_3}); // normal
    geom.format.attributes.append(GpuDraw::GpuGeometry::VertexAttribute {2, 24, GpuDraw::GpuGeometry::AttributeFormat::F32_2}); // texcoord
    GpuDraw::GpuGeometry::GeometryBuffer vb;
    vb.buffer = std::move(vertexBuffer);
    vb.offset = 0;
    vb.stride = sizeof(Vertex);
    geom.vertices.append(vb);
    geom.vertexCount = static_cast<uint32_t>(sizeof(kBoxVertices) / sizeof(Vertex));
    return geom;
}

static AutoRef<PbrShading::Material> loadPbrMaterial(ArtifactDatabase & db, AutoRef<GpuContext> gpu) {
    auto fp = fs::openFile("media::pbr/lined-metal-sheeting/lined-metal-sheeting.material", std::ios::in);
    if (fp && fp->readable()) {
        auto mat = PbrShading::Material::load(db, "pbr_material",
                                              PbrShading::Material::LoadParameters {
                                                  .gpu      = gpu,
                                                  .source   = fp,
                                                  .basePath = "media::pbr/lined-metal-sheeting/",
                                              });
        if (mat) return mat;
        GN_WARN(sLogger)("Failed to load PBR material from file, using empty material");
    }
    // Fallback: empty material
    static const char empty[1] = {};
    AutoRef<MemFile>  memFile  = AutoRef<MemFile>::make(const_cast<char *>(empty), 0, "pbr_material");
    return PbrShading::Material::load(db, "pbr_material", PbrShading::Material::LoadParameters {.gpu = gpu, .source = memFile});
}

static SharedShaderConstants::DirectLightingInformation buildLighting() {
    SharedShaderConstants::DirectLightingInformation lighting;
    SharedShaderConstants::DirectLight               light;
    light.type                    = SharedShaderConstants::DirectLight::DIRECTIONAL;
    light.directional.orientation = Orientation(1.f, 0.f, 0.f, 0.f); // identity → light faces -Z
    light.directional.irradiance  = {1.0f, 0.95f, 0.9f, {3000.0f}};  // warm white, 3000 lux
    lighting.lights.append(light);
    return lighting;
}

// -------------------------------------------------------------------------
// main
// -------------------------------------------------------------------------

int main(int, const char **) {
    enableCRTMemoryCheck();

    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    if (!db) return -1;

    auto renderGraph = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    if (!renderGraph) return -1;

    auto gpuContext = GpuContext::create(*db, "gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    auto pbrShading = PbrShading::create(*db, "pbr_shading", PbrShading::CreateParameters {.gpu = gpuContext});
    if (!pbrShading) return -1;

    auto material = loadPbrMaterial(*db, gpuContext);
    if (!material) return -1;

    auto vertexBuffer = createBoxVertexBuffer(*db, gpuContext);
    if (!vertexBuffer) return -1;

    auto window = std::unique_ptr<win::Window>(
        win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - PBR Box", .clientWidth = 1280, .clientHeight = 720}));
    if (!window) return -1;
    window->show();

    // Window owns the surface; do not destroy it. Destroy backbuffer before window.
    intptr_t surface = window->getVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
    if (!surface) return -1;

    auto backbuffer = Backbuffer::create(
        *db, "backbuffer",
        Backbuffer::CreateParameters {.context = gpuContext, .descriptor = Backbuffer::Descriptor {}.setWindow(surface).setDimensions(1280, 720)});
    if (!backbuffer) return -1;
    const auto & bbDesc = backbuffer->descriptor();

    auto depthTexture = Texture::create(
        *db, "depth_texture",
        Texture::CreateParameters {.context    = gpuContext,
                                   .descriptor = Texture::Descriptor {}.setDimensions(1280, 720).setFormat(gfx::img::PixelFormat::RG_24_UNORM_8_UINT())});

    auto renderTarget = RenderTarget::create(*db, "render_target", RenderTarget::CreateParameters {});
    if (!renderTarget) return -1;
    renderTarget->addColorTarget(backbuffer);
    renderTarget->setClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    renderTarget->setDepthStencilTarget(depthTexture);
    renderTarget->setDepthState(RenderTarget::DepthState {.func = RenderTarget::Compare::LESS, .write = true});

    auto prepareAction = PrepareBackbuffer::create(*db, "prepare_action", PrepareBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!prepareAction) return -1;
    // auto clearAction = ClearRenderTarget::create(*db, "clear_action", ClearRenderTarget::CreateParameters {.gpu = gpuContext});
    // if (!clearAction) return -1;
    auto presentAction = PresentBackbuffer::create(*db, "present_action", PresentBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!presentAction) return -1;

    auto boxGeometry = buildBoxGeometry(vertexBuffer);
    auto lighting    = buildLighting();

    // SharedShaderConstants: camera at (1.8, 1.4, 2.4) m looking at origin, 60° FOV.
    auto sharedConstants = SharedShaderConstants::create(*db, "shared_constants", SharedShaderConstants::CreateParameters {.gpu = gpuContext});
    if (sharedConstants) {
        // Derive camera orientation from a lookAt.
        const glm::vec3   eye(1.8f, 1.4f, 2.4f), target(0.f, 0.f, 0.f), up(0.f, 1.f, 0.f);
        const glm::mat4   camToWorld = glm::inverse(glm::lookAtRH(eye, target, up));
        const Orientation camOrient  = glm::quat_cast(glm::mat3(camToWorld));

        SharedShaderConstants::ViewInformation view;
        view.cameraPosition    = Location {1.8f, 1.4f, 2.4f};
        view.cameraOrientation = camOrient;
        view.cameraFov         = Degree(60.f);
        view.aspectRatio       = (float) bbDesc.width / (float) bbDesc.height;
        view.nearPlane         = 0.1f;
        view.farPlane          = 100.f;
        view.renderTarget      = renderTarget;
        sharedConstants->setViewInformation(view);
        sharedConstants->setDirectLightingInformation(lighting);
    }

    GN_INFO(sLogger)("Starting PBR box render loop...");

    while (window->runUntilNoNewEvents()) {
        // Build shared constants (upload UBOs); SSC build returns SubGraph with upload workflow(s).
        auto       sscSubGraph = sharedConstants->build(*renderGraph);
        Workflow * uploadWf    = sscSubGraph.workflows.empty() ? nullptr : sscSubGraph.workflows[0];

        auto renderWorkflow = renderGraph->createWorkflow("Render");
        if (!renderWorkflow) break;

        renderWorkflow->appendTask("Prepare", prepareAction, PrepareBackbuffer::A::make(backbuffer));
        // renderWorkflow->appendTask("Clear", clearAction, ClearRenderTarget::A::make(renderTarget));

        PbrShading::BuildParameters pbrParams;
        pbrParams.renderGraph             = renderGraph.get();
        pbrParams.sharedShaderConstants   = sharedConstants;
        pbrParams.material                = material;
        pbrParams.geometry                = boxGeometry;
        pbrParams.locationInWorldSpace    = {0, 0, 0}; // box at world origin
        pbrParams.orientationInWorldSpace = glm::angleAxis(glm::radians(30.f), glm::vec3(0.f, 1.f, 0.f));
        auto pbrSubGraph                  = pbrShading->build(pbrParams);
        if (pbrSubGraph.builtResult == Action::ExecutionResult::PASSED && !pbrSubGraph.workflows.empty()) {
            for (const auto & task : pbrSubGraph.workflows[0]->tasks()) renderWorkflow->appendTask(task.name, task.action, task.arguments);
        }

        renderWorkflow->appendTask("Present", presentAction, PresentBackbuffer::A::make(backbuffer));

        // Submit upload workflow first (if any), then render workflow.
        Workflow * workflows[] = {uploadWf, renderWorkflow};
        auto       submission  = renderGraph->submit(
            RenderGraph::SubmitParameters {.workflows = SafeArrayAccessor<Workflow *>(uploadWf ? workflows : &renderWorkflow, uploadWf ? 2u : 1u),
                                           .name      = "Frame"});
        if (!submission) {
            GN_ERROR(sLogger)("Failed to submit render graph");
            break;
        }
        auto result = submission->result();
        if (result.executionResult == Action::ExecutionResult::FAILED) break;
        if (result.executionResult == Action::ExecutionResult::WARNING) { GN_WARN(sLogger)("Render graph completed with warnings"); }

        // Submission owns the workflows and will delete them when it is destroyed. Clear subgraph
        // workflow lists so SubGraph destructors do not double-delete (destroy order: submission first, then subgraphs).
        sscSubGraph.workflows.clear();
    }

    GN_INFO(sLogger)("PBR box demo finished");
    return 0;
}

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
    auto buf = Buffer::create(db, "box_vertex_buffer", Buffer::CreateParameters {.context = gpu, .size = sizeof(kBoxVertices)});
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

static GpuGeometry buildBoxGeometry(AutoRef<Buffer> vertexBuffer) {
    GpuGeometry geom;
    geom.format.attributes.append(GpuGeometry::VertexAttribute {0, 0, GpuGeometry::AttributeFormat::F32_3});  // position
    geom.format.attributes.append(GpuGeometry::VertexAttribute {1, 12, GpuGeometry::AttributeFormat::F32_3}); // normal
    geom.format.attributes.append(GpuGeometry::VertexAttribute {2, 24, GpuGeometry::AttributeFormat::F32_2}); // texcoord
    GpuGeometry::GeometryBuffer vb;
    vb.buffer = std::move(vertexBuffer);
    vb.offset = 0;
    vb.size   = sizeof(kBoxVertices);
    vb.stride = sizeof(Vertex);
    geom.vertices.append(vb);
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

static WorldToClipTransformChain buildCamera(uint32_t width, uint32_t height) {
    WorldToClipTransformChain camera;
    camera.setCamera({1.8f, 1.4f, 2.4f}, // eye: slightly off-axis to show 3 faces
                     {0.0f, 0.0f, 0.0f}, // look at box center
                     {0.0f, 1.0f, 0.0f}  // world up
    );
    camera.setPerspective(glm::radians(60.f),             // 60 degree vertical FOV
                          (float) width / (float) height, // aspect ratio
                          0.1f,                           // near plane
                          100.0f                          // far plane
    );
    return camera;
}

static AffineTransform buildModelTransform() {
    AffineTransform model;
    model.reset();
    // Rotate box 30 degrees around Y so we see a corner
    model.setRotation(glm::radians(30.f), glm::vec3(0.f, 1.f, 0.f));
    return model;
}

static SharedShaderConstants::DirectLightingInformation buildLighting() {
    SharedShaderConstants::DirectLightingInformation lighting;
    SharedShaderConstants::DirectLight               light;
    light.type                    = SharedShaderConstants::DirectLight::DIRECTIONAL;
    light.directional.orientation = Quaternionf::sIdentity();
    light.directional.irradiance  = {1.0f, 0.95f, 0.9f, {3000.0f}}; // warm white, 3000 lux
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

    auto window = win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - PBR Box", .clientWidth = 1280, .clientHeight = 720});
    if (!window) return -1;
    // Window owns the surface; do not destroy it. Destroy backbuffer before window.
    intptr_t surface = window->getVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
    if (!surface) return -1;

    auto backbuffer = Backbuffer::create(
        *db, "backbuffer",
        Backbuffer::CreateParameters {.context = gpuContext, .descriptor = Backbuffer::Descriptor {}.setWindow(surface).setDimensions(1280, 720)});
    if (!backbuffer) return -1;
    const auto & bbDesc = backbuffer->descriptor();

    auto renderTarget = RenderTarget::create(*db, "render_target", RenderTarget::CreateParameters {});
    if (!renderTarget) return -1;
    renderTarget->colors.append(RenderTarget::ColorTarget {.target = GpuImageView {.image = backbuffer}}.setClearColor(0.1f, 0.1f, 0.15f, 1.0f));

    auto prepareAction = PrepareBackbuffer::create(*db, "prepare_action", PrepareBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!prepareAction) return -1;
    auto clearAction = ClearRenderTarget::create(*db, "clear_action", ClearRenderTarget::CreateParameters {.gpu = gpuContext});
    if (!clearAction) return -1;
    auto presentAction = PresentBackbuffer::create(*db, "present_action", PresentBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!presentAction) return -1;

    // Build scene transforms once (static scene, no animation)
    auto camera      = buildCamera(bbDesc.width, bbDesc.height);
    auto modelXform  = buildModelTransform();
    auto boxGeometry = buildBoxGeometry(vertexBuffer);
    auto lighting    = buildLighting();

    // SharedShaderConstants: set view and lighting
    auto sharedConstants = SharedShaderConstants::create(*db, "shared_constants", SharedShaderConstants::CreateParameters {.gpu = gpuContext});
    if (sharedConstants) {
        SharedShaderConstants::ViewInformation view;
        view.worldToClip    = camera.matrix();
        view.cameraPosition = Location {WorldUnit((int64_t) (1.8f * 1e6f)), WorldUnit((int64_t) (1.4f * 1e6f)), WorldUnit((int64_t) (2.4f * 1e6f))};
        view.renderTarget   = renderTarget;
        sharedConstants->setViewInformation(view);
        sharedConstants->setDirectLightingInformation(lighting);
    }

    GN_INFO(sLogger)("Starting PBR box render loop...");

    while (window->runUntilNoNewEvents()) {
        auto renderWorkflow = renderGraph->createWorkflow("Render");
        if (!renderWorkflow) break;

        renderWorkflow->appendTask("Prepare", prepareAction, PrepareBackbuffer::A::make(backbuffer));
        renderWorkflow->appendTask("Clear", clearAction, ClearRenderTarget::A::make(renderTarget));

        PbrShading::BuildParameters pbrParams;
        pbrParams.renderGraph           = renderGraph.get();
        pbrParams.sharedShaderConstants = sharedConstants;
        pbrParams.material              = material;
        pbrParams.geometry              = boxGeometry;
        pbrParams.modelToWorld          = modelXform;
        pbrParams.worldToClip           = camera.matrix();
        auto pbrSubGraph                = pbrShading->build(pbrParams);
        if (pbrSubGraph.builtResult == Action::ExecutionResult::PASSED && !pbrSubGraph.workflows.empty()) {
            for (auto & task : pbrSubGraph.workflows[0]->tasks) renderWorkflow->appendTask(task.name, task.action, task.arguments);
        }

        renderWorkflow->appendTask("Present", presentAction, PresentBackbuffer::A::make(backbuffer));

        auto submission = renderGraph->submit(RenderGraph::SubmitParameters {.workflows = SafeArrayAccessor<Workflow *>(&renderWorkflow, 1), .name = "Frame"});
        if (!submission) {
            GN_ERROR(sLogger)("Failed to submit render graph");
            break;
        }
        auto result = submission->result();
        if (result.executionResult == Action::ExecutionResult::FAILED) break;
        if (result.executionResult == Action::ExecutionResult::WARNING) { GN_WARN(sLogger)("Render graph completed with warnings"); }
    }

    GN_INFO(sLogger)("PBR box demo finished");
    return 0;
}

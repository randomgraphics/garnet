#include "gltf-loader.h"
#include "pch.h"
#include <garnet/GNbase.h>
#include <garnet/GNwin.h>
#include <garnet/base/filesys.h>

using namespace GN;
using namespace GN::rdg;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.render-graph");

// -------------------------------------------------------------------------
// Helpers
// -------------------------------------------------------------------------

static AutoRef<PbrShading::Material> loadPbrMaterial(AutoRef<GpuContext> gpu) {
    auto fp = fs::openFile("media::pbr/DamagedHelmet/DamagedHelmet.material", std::ios::in);
    if (fp && fp->readable()) {
        auto mat = PbrShading::Material::load("pbr_material", PbrShading::Material::LoadParameters {
                                                                  .gpu      = gpu,
                                                                  .source   = fp,
                                                                  .basePath = "media::pbr/DamagedHelmet/",
                                                              });
        if (mat) return mat;
        GN_WARN(sLogger)("Failed to load PBR material from file, using empty material");
    }
    // Fallback: empty material
    static const char empty[1] = {};
    AutoRef<MemFile>  memFile  = AutoRef<MemFile>::make(const_cast<char *>(empty), 0, "pbr_material");
    return PbrShading::Material::load("pbr_material", PbrShading::Material::LoadParameters {.gpu = gpu, .source = memFile});
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

    auto renderGraph = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    if (!renderGraph) return -1;

    auto gpuContext = GpuContext::create("gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    auto pbrShading = PbrShading::create("pbr_shading", PbrShading::CreateParameters {.gpu = gpuContext});
    if (!pbrShading) return -1;

    auto skybox = SkyBox::create("skybox", SkyBox::CreateParameters {.gpu = gpuContext});
    if (!skybox) return -1;

    // Load environment cubemap. Falls back to 1×1 black if absent.
    auto skyboxCubemap = Texture::load(Texture::LoadParameters {
        .context  = gpuContext,
        .filename = fs::toNativeDiskFilePath("media::envmap/bad-salzbrunn-walking-hall/skybox.dds"),
    });
    if (!skyboxCubemap) { GN_WARN(sLogger)("Skybox cubemap not found; using fallback"); }

    auto material = loadPbrMaterial(gpuContext);
    if (!material) return -1;

    auto helmetGeometry = gltf::loadGltfGeometry(fs::toNativeDiskFilePath("media::pbr/DamagedHelmet/DamagedHelmet.gltf").c_str(), gpuContext);
    if (helmetGeometry.vertexCount == 0) return -1;

    auto window = std::unique_ptr<win::Window>(
        win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - Damaged Helmet", .clientWidth = 1280, .clientHeight = 720}));
    if (!window) return -1;
    window->show();

    // Window owns the surface; do not destroy it. Destroy backbuffer before window.
    intptr_t surface = window->getVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
    if (!surface) return -1;

    auto backbuffer =
        Backbuffer::create("backbuffer", Backbuffer::CreateParameters {.context    = gpuContext,
                                                                       .descriptor = Backbuffer::Descriptor {}.setWindow(surface).setDimensions(1280, 720)});
    if (!backbuffer) return -1;
    const auto & bbDesc = backbuffer->descriptor();

    auto depthTexture = Texture::create("depth_texture", Texture::CreateParameters {.context    = gpuContext,
                                                                                    .descriptor = Texture::Descriptor {}.setDimensions(1280, 720).setFormat(
                                                                                        gfx::img::PixelFormat::RG_24_UNORM_8_UINT())});

    auto renderTarget = RenderTarget::create("render_target", RenderTarget::CreateParameters {});
    if (!renderTarget) return -1;
    renderTarget->addColorTarget(backbuffer);
    renderTarget->setClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    renderTarget->setDepthStencilTarget(depthTexture);
    renderTarget->setDepthState(RenderTarget::DepthState {.func = RenderTarget::Compare::LESS, .write = true});

    auto prepareAction = PrepareBackbuffer::create("prepare_action", PrepareBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!prepareAction) return -1;
    // auto clearAction = ClearRenderTarget::create("clear_action", ClearRenderTarget::CreateParameters {.gpu = gpuContext});
    // if (!clearAction) return -1;
    auto presentAction = PresentBackbuffer::create("present_action", PresentBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!presentAction) return -1;

    auto lighting = buildLighting();

    // SharedShaderConstants: camera at (1.8, 1.4, 2.4) m looking at origin, 60° FOV.
    auto sharedConstants = SharedShaderConstants::create("shared_constants", SharedShaderConstants::CreateParameters {.gpu = gpuContext});
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
        if (skyboxCubemap) {
            SharedShaderConstants::EnvironmentLightingInformation env;
            env.skyboxCubemap = skyboxCubemap;
            sharedConstants->setEnvironmentLightingInformation(env);
        }
    }

    GN_INFO(sLogger)("Starting PBR helmet render loop...");

    while (window->runUntilNoNewEvents()) {
        // Build shared constants (upload UBOs); SSC build returns SubGraph with upload workflow(s).
        auto sscSubGraph = sharedConstants->build(*renderGraph);

        auto renderWorkflow = renderGraph->createWorkflow("Render");
        if (!renderWorkflow) break;

        renderWorkflow.appendTask("Prepare", prepareAction, PrepareBackbuffer::A::make(backbuffer));
        // renderWorkflow.appendTask("Clear", clearAction, ClearRenderTarget::A::make(renderTarget));

        // Skybox renders before geometry (fullscreen quad at max depth; PBR geometry overwrites it).
        SkyBox::BuildParameters skyboxParams;
        skyboxParams.renderGraph          = renderGraph.get();
        skyboxParams.sharedShaderConstants = sharedConstants;
        auto skyboxSubGraph               = skybox->build(skyboxParams);
        if (skyboxSubGraph.builtResult == Action::ExecutionResult::PASSED && !skyboxSubGraph.workflows.empty()) {
            for (const auto & task : skyboxSubGraph.workflows[0].tasks())
                renderWorkflow.appendTask(task.name, task.action, task.arguments);
        }

        PbrShading::BuildParameters pbrParams;
        pbrParams.renderGraph           = renderGraph.get();
        pbrParams.sharedShaderConstants = sharedConstants;
        pbrParams.material              = material;
        pbrParams.geometry              = helmetGeometry;
        pbrParams.locationInWorldSpace  = {0, 0, 0};
        // Apply the 90° X rotation from the GLTF node transform (ignored by the loader).
        pbrParams.orientationInWorldSpace = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        auto pbrSubGraph                  = pbrShading->build(pbrParams);
        if (pbrSubGraph.builtResult == Action::ExecutionResult::PASSED && !pbrSubGraph.workflows.empty()) {
            for (const auto & task : pbrSubGraph.workflows[0].tasks()) renderWorkflow.appendTask(task.name, task.action, task.arguments);
        }

        renderWorkflow.appendTask("Present", presentAction, PresentBackbuffer::A::make(backbuffer));

        // Submit upload workflow first (if any), then render workflow.
        DynaArray<Workflow> toSubmit;
        if (!sscSubGraph.workflows.empty()) { toSubmit.append(std::move(sscSubGraph.workflows[0])); }
        toSubmit.append(std::move(renderWorkflow));
        auto submission =
            renderGraph->submit(RenderGraph::SubmitParameters {.workflows = SafeArrayAccessor<Workflow>(toSubmit.data(), toSubmit.size()), .name = "Frame"});
        if (!submission) {
            GN_ERROR(sLogger)("Failed to submit render graph");
            break;
        }
        auto result = submission->result();
        if (result.executionResult == Action::ExecutionResult::FAILED) break;
        if (result.executionResult == Action::ExecutionResult::WARNING) { GN_WARN(sLogger)("Render graph completed with warnings"); }
    }

    GN_INFO(sLogger)("PBR helmet demo finished");
    return 0;
}

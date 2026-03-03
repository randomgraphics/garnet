#include "pch.h"
#include <garnet/GNbase.h>

using namespace GN;
using namespace GN::rdg;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.render-graph");

int main(int, const char **) {
    enableCRTMemoryCheck();

    // Create artifact database with auto-registration of built-in artifacts
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    if (!db) {
        GN_ERROR(sLogger)("Failed to create artifact database");
        return -1;
    }

    // Create render graph
    auto renderGraph = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    if (!renderGraph) {
        GN_ERROR(sLogger)("Failed to create render graph");
        return -1;
    }

    // Create GPU context (no window/size; those live on Backbuffer)
    auto gpuContext = GpuContext::create(*db, "gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    // SharedShaderConstants for view (optional; PbrShading uses it for worldToClip when set)
    auto sharedConstants = SharedShaderConstants::create(*db, "shared_constants", SharedShaderConstants::CreateParameters {.gpu = gpuContext});
    if (sharedConstants) {
        SharedShaderConstants::ViewInformation view;
        view.worldToClip = Matrix44f::sIdentity(); // identity for minimal; replace with proper view-proj later
        sharedConstants->setViewInformation(view);
    }

    // PbrShading effect (Vulkan)
    auto pbrShading = PbrShading::create(*db, "pbr_shading", PbrShading::CreateParameters {.gpu = gpuContext});
    if (!pbrShading) return -1;

    // Optional: load material from in-memory blob (minimal; for embedding later)
    AutoRef<PbrShading::Material> material;
    {
        static const char dummy[1] = {};
        MemFile memFile(const_cast<char *>(dummy), 0, "pbr_material");
        if (memFile.readable())
            material = PbrShading::Material::load(*db, "pbr_material", PbrShading::Material::LoadParameters {.gpu = gpuContext, .source = &memFile});
    }

    // Create and load texture
    auto texture = Texture::load(*db, Texture::LoadParameters {.context = gpuContext, .filename = "media::texture/earth.jpg"});
    if (!texture) return -1;

    // Create and load mesh
    auto mesh = Mesh::load(*db, Mesh::LoadParameters {.context = gpuContext, .filename = "media::cube/cube.fbx"});
    if (!mesh) return -1;

    // Create a main window of 1280x720
    auto window = win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - Rendering Demo", .clientWidth = 1280, .clientHeight = 720});
    if (!window) return -1;

    // Create backbuffer (window and size are part of Backbuffer descriptor)
    auto backbuffer = Backbuffer::create(*db, "backbuffer", Backbuffer::CreateParameters {.context = gpuContext, .descriptor = {.win = window}});
    if (!backbuffer) return -1;
    const auto & backbufferDesc = backbuffer->descriptor();

    // Create and initialize depth texture (match backbuffer size)
    auto depthDesc    = Texture::Descriptor {};
    depthDesc.format  = gfx::img::PixelFormat::RG_24_UNORM_8_UINT();
    depthDesc.width   = backbufferDesc.width;
    depthDesc.height  = backbufferDesc.height;
    auto depthTexture = Texture::create(*db, "depth_texture", Texture::CreateParameters {.context = gpuContext, .descriptor = depthDesc});
    if (!depthTexture) return -1;

    // Create and initialize sampler
    auto samplerDesc      = Sampler::Descriptor {};
    samplerDesc.filterMin = Sampler::Filter::LINEAR;
    samplerDesc.filterMag = Sampler::Filter::LINEAR;
    samplerDesc.filterMip = Sampler::Filter::LINEAR;
    samplerDesc.addressU  = Sampler::AddressMode::REPEAT;
    samplerDesc.addressV  = Sampler::AddressMode::REPEAT;
    samplerDesc.addressW  = Sampler::AddressMode::REPEAT;
    auto sampler          = Sampler::create(*db, "sampler", Sampler::CreateParameters {.context = gpuContext, .descriptor = samplerDesc});
    if (!sampler) return -1;

    // Create and initialize actions (each creates itself and registers via admit())
    auto prepareAction = PrepareBackbuffer::create(*db, "prepare_action", PrepareBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!prepareAction) return -1;

    auto clearAction = ClearRenderTarget::create(*db, "clear_action", ClearRenderTarget::CreateParameters {.gpu = gpuContext});
    if (!clearAction) return -1;

    auto presentAction = PresentBackbuffer::create(*db, "present_action", PresentBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!presentAction) return -1;

    // Create render target artifact (backbuffer + depth; clear values on artifact)
    auto renderTarget = RenderTarget::create(*db, "render_target", RenderTarget::CreateParameters {});
    if (!renderTarget) return -1;
    renderTarget->colors.append({.target = GpuImageView {.image = backbuffer}});
    renderTarget->colors[0].clearColor.f4[0] = 0.2f;
    renderTarget->colors[0].clearColor.f4[1] = 0.3f;
    renderTarget->colors[0].clearColor.f4[2] = 0.4f;
    renderTarget->colors[0].clearColor.f4[3] = 1.0f;
    renderTarget->depthStencil.target        = depthTexture;
    renderTarget->depthStencil.clearDepth    = 1.0f;
    renderTarget->depthStencil.clearStencil  = 0;

    GN_INFO(sLogger)("Starting render loop...");

    // Render loop: prepare, clear, PBR draw, present until prepare fails
    while (true) {
        // Schedule render workflow
        auto renderWorkflow = renderGraph->createWorkflow("Render");
        if (renderWorkflow) {
            // Task: Prepare backbuffer
            auto prepareTask              = Workflow::Task("Prepare");
            prepareTask.action            = prepareAction;
            auto prepareArgs              = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A());
            prepareArgs->backbuffer.value = backbuffer;
            prepareTask.arguments         = prepareArgs;
            renderWorkflow->tasks.append(prepareTask);

            // Task: Clear render target (color + depth; clear values on RenderTarget artifact)
            auto clearTask                = Workflow::Task("Clear");
            clearTask.action              = clearAction;
            auto clearArgs                = AutoRef<ClearRenderTarget::A>(new ClearRenderTarget::A());
            clearArgs->renderTarget.value = renderTarget;
            clearTask.arguments           = clearArgs;
            renderWorkflow->tasks.append(clearTask);

            // Task: PBR draw (from PbrShading::build)
            PbrShading::BuildParameters pbrParams;
            pbrParams.renderGraph           = renderGraph.get();
            pbrParams.sharedShaderConstants = sharedConstants;
            pbrParams.material              = material;
            pbrParams.geometry              = {};  // empty -> backend draws 3 vertices (placeholder)
            pbrParams.modelToWorld.reset();
            pbrParams.worldToClip = Matrix44f::sIdentity();
            auto pbrSubGraph      = pbrShading->build(pbrParams);
            if (pbrSubGraph.builtResult == Action::ExecutionResult::PASSED && !pbrSubGraph.workflows.empty() && !pbrSubGraph.workflows[0]->tasks.empty()) {
                auto & t = pbrSubGraph.workflows[0]->tasks[0];
                renderWorkflow->appendTask(t.name, t.action, t.arguments);
            }

            // Task: Present backbuffer
            auto presentTask              = Workflow::Task("Present");
            presentTask.action            = presentAction;
            auto presentArgs              = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A());
            presentArgs->backbuffer.value = backbuffer;
            presentTask.arguments         = presentArgs;
            renderWorkflow->tasks.append(presentTask);
        }

        // Submit render graph for execution
        Workflow * w = renderWorkflow;
        auto submission = renderGraph->submit(SubmitParameters {.workflows = SafeArrayAccessor<Workflow *>(&w, 1), .name = "Frame"});
        if (!submission) {
            GN_ERROR(sLogger)("Failed to submit render graph");
            break;
        }

        // Wait for completion and get result
        auto result = submission->result();

        // If prepare failed (window closed), exit loop
        if (result.executionResult == Action::ExecutionResult::FAILED) {
            GN_INFO(sLogger)("Render graph submission failed (likely window closed), exiting");
            break;
        }

        if (result.executionResult == Action::ExecutionResult::WARNING) { GN_WARN(sLogger)("Render graph submission completed with warnings"); }
    }

    GN_INFO(sLogger)("Render graph draw mesh completed");
    return 0;
}

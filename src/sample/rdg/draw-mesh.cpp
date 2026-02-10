#include "pch.h"

using namespace GN;
using namespace GN::rg;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.render-graph");

int main(int argc, const char * argv[]) {
    enableCRTMemoryCheck();

    // Parse command line
    CommandLineArguments cmdargs(argc, argv);
    if (cmdargs.status != CommandLineArguments::CONTINUE_EXECUTION) {
        if (cmdargs.status == CommandLineArguments::SHOW_HELP) { cmdargs.showDefaultHelp(); }
        return cmdargs.status == CommandLineArguments::SHOW_HELP ? 0 : -1;
    }

    // Create artifact database with auto-registration of built-in artifacts
    AutoRef<ArtifactDatabase> db = ArtifactDatabase::create({});
    if (!db) {
        GN_ERROR(sLogger)("Failed to create artifact database");
        return -1;
    }

    // Create and initialize GPU context
    auto gpuContext = db->spawnAndReset<GpuContext>("gpu_context");
    if (!gpuContext) {
        GN_ERROR(sLogger)("Failed to create and initialize GPU context");
        return -1;
    }
    auto [displayWidth, displayHeight] = gpuContext->dimension();

    // Create and load texture
    auto texture = db->spawnAndLoad<Texture>("texture", "media::texture/earth.jpg");
    if (!texture) {
        GN_ERROR(sLogger)("Failed to create and load texture");
        return -1;
    }

    // Create and load mesh
    auto mesh = db->spawnAndLoad<Mesh>("mesh", "media::cube/cube.model.xml");
    if (!mesh) {
        GN_ERROR(sLogger)("Failed to create and load mesh");
        return -1;
    }

    // Create and initialize backbuffer
    auto backbuffer = db->spawnAndReset<Backbuffer>("backbuffer", Backbuffer::Descriptor {displayWidth, displayHeight});
    if (!backbuffer) {
        GN_ERROR(sLogger)("Failed to create and initialize backbuffer");
        return -1;
    }

    // Create and initialize depth texture
    auto depthDesc    = Texture::Descriptor {};
    depthDesc.format  = gfx::img::PixelFormat::D24S8();
    depthDesc.width   = displayWidth;
    depthDesc.height  = displayHeight;
    auto depthTexture = db->spawnAndReset<Texture>("depth", depthDesc);
    if (!depthTexture) {
        GN_ERROR(sLogger)("Failed to create and initialize depth texture");
        return -1;
    }

    // Create and initialize sampler
    auto samplerDesc      = Sampler::Descriptor {};
    samplerDesc.filterMin = Sampler::Filter::LINEAR;
    samplerDesc.filterMag = Sampler::Filter::LINEAR;
    samplerDesc.filterMip = Sampler::Filter::LINEAR;
    samplerDesc.addressU  = Sampler::AddressMode::REPEAT;
    samplerDesc.addressV  = Sampler::AddressMode::REPEAT;
    samplerDesc.addressW  = Sampler::AddressMode::REPEAT;
    auto sampler          = db->spawnAndReset<Sampler>("sampler", samplerDesc);
    if (!sampler) {
        GN_ERROR(sLogger)("Failed to create and initialize sampler");
        return -1;
    }

    // Create render graph
    AutoRef<RenderGraph> renderGraph = RenderGraph::create({});
    if (!renderGraph) {
        GN_ERROR(sLogger)("Failed to create render graph");
        return -1;
    }

    // Create and initialize actions
    auto prepareAction = db->spawnAndReset<PrepareBackbuffer>("prepare_action");
    if (!prepareAction) {
        GN_ERROR(sLogger)("Failed to create and initialize PrepareBackbuffer action");
        return -1;
    }

    auto clearAction = db->spawnAndReset<ClearRenderTarget>("clear_action");
    if (!clearAction) {
        GN_ERROR(sLogger)("Failed to create and initialize ClearRenderTarget action");
        return -1;
    }

    auto clearDepthAction = db->spawnAndReset<ClearDepthStencil>("clear_depth_action");
    if (!clearDepthAction) {
        GN_ERROR(sLogger)("Failed to create and initialize ClearDepthStencil action");
        return -1;
    }

    auto composeAction = db->spawnAndReset<Compose>("compose_action");
    if (!composeAction) {
        GN_ERROR(sLogger)("Failed to create and initialize Compose action");
        return -1;
    }

    auto presentAction = db->spawnAndReset<PresentBackbuffer>("present_action");
    if (!presentAction) {
        GN_ERROR(sLogger)("Failed to create and initialize PresentBackbuffer action");
        return -1;
    }

    GN_INFO(sLogger)("Starting render loop...");

    // Render loop: prepare, clear, compose, present until prepare fails
    while (true) {
        // Schedule render workflow
        auto renderWorkflow = renderGraph->schedule();
        if (renderWorkflow) {
            renderWorkflow->name = "Render";

            // Task: Prepare backbuffer
            auto prepareTask   = Workflow::Task {};
            prepareTask.action = prepareAction;
            auto prepareArgs = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A(Artifact::Identification(PrepareBackbuffer::A::TYPE, "prepare_args"), 0));
            prepareArgs->backbuffer.set(backbuffer);
            prepareTask.arguments = prepareArgs;
            renderWorkflow->tasks.append(prepareTask);

            // Task: Clear render target
            auto clearTask   = Workflow::Task {};
            clearTask.action = clearAction;

            auto clearArgs = AutoRef<ClearRenderTarget::A>(new ClearRenderTarget::A(Artifact::Identification(ClearRenderTarget::A::TYPE, "clear_args"), 0));

            auto clearColor = ClearRenderTarget::A::ClearColor {};
            clearColor.r    = 0.2f;
            clearColor.g    = 0.3f;
            clearColor.b    = 0.4f;
            clearColor.a    = 1.0f;
            clearArgs->color.set(clearColor);

            auto rt   = RenderTarget {};
            rt.target = backbuffer;
            rt.sub    = Texture::SubresourceIndex();
            clearArgs->renderTarget.set(rt);

            clearTask.arguments = clearArgs;
            renderWorkflow->tasks.append(clearTask);

            // Task: Clear depth
            auto clearDepthTask   = Workflow::Task {};
            clearDepthTask.action = clearDepthAction;

            auto clearDepthArgs =
                AutoRef<ClearDepthStencil::A>(new ClearDepthStencil::A(Artifact::Identification(ClearDepthStencil::A::TYPE, "clear_depth_args"), 0));

            clearDepthArgs->depth.set(1.0f);
            clearDepthArgs->stencil.set(0);

            auto depthRt   = RenderTarget {};
            depthRt.target = depthTexture;
            depthRt.sub    = Texture::SubresourceIndex();
            clearDepthArgs->depthStencil.set(depthRt);

            clearDepthTask.arguments = clearDepthArgs;
            renderWorkflow->tasks.append(clearDepthTask);

            // Task: Compose (draw mesh with texture)
            auto composeTask   = Workflow::Task {};
            composeTask.action = composeAction;

            auto composeArgs = AutoRef<Compose::A>(new Compose::A(Artifact::Identification(Compose::A::TYPE, "compose_args"), 0));

            composeArgs->mesh.set(mesh);
            composeArgs->color.set(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
            composeArgs->textures.set(0, texture);

            auto composeRt   = RenderTarget {};
            composeRt.target = backbuffer;
            composeRt.sub    = Texture::SubresourceIndex();
            composeArgs->renderTargets.set(0, composeRt);

            auto composeDepthRt   = RenderTarget {};
            composeDepthRt.target = depthTexture;
            composeDepthRt.sub    = Texture::SubresourceIndex();
            composeArgs->depthStencil.set(composeDepthRt);

            composeTask.arguments = composeArgs;
            renderWorkflow->tasks.append(composeTask);

            // Task: Present backbuffer
            auto presentTask   = Workflow::Task {};
            presentTask.action = presentAction;
            auto presentArgs = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A(Artifact::Identification(PresentBackbuffer::A::TYPE, "present_args"), 0));
            presentArgs->backbuffer.set(backbuffer);
            presentTask.arguments = presentArgs;
            renderWorkflow->tasks.append(presentTask);
        }

        // Submit render graph for execution
        auto submission = renderGraph->submit({});
        if (!submission) {
            GN_ERROR(sLogger)("Failed to submit render graph");
            break;
        }

        // Wait for completion and get result
        auto result = submission->result();

        // If prepare failed (window closed), exit loop
        if (result.result == Action::ExecutionResult::FAILED) {
            GN_INFO(sLogger)("Render graph submission failed (likely window closed), exiting");
            break;
        }

        if (result.result == Action::ExecutionResult::WARNING) { GN_WARN(sLogger)("Render graph submission completed with warnings"); }
    }

    GN_INFO(sLogger)("Render graph draw mesh completed");
    return 0;
}

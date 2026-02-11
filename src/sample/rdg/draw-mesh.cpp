#include "pch.h"

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

    // Create GPU context (artifact creates itself and registers via admit()), then reset
    auto gpuContext = GpuContext::create(*db);
    if (!gpuContext) {
        GN_ERROR(sLogger)("Failed to create GPU context");
        return -1;
    }
    if (!gpuContext->reset(GpuContext::ResetParameters {})) {
        GN_ERROR(sLogger)("Failed to initialize GPU context");
        return -1;
    }
    auto [displayWidth, displayHeight] = gpuContext->dimension();

    // Create and load texture
    auto texture = Texture::create(*db);
    if (!texture || !texture->load("media::texture/earth.jpg")) {
        GN_ERROR(sLogger)("Failed to create and load texture");
        return -1;
    }

    // Create and load mesh
    auto mesh = Mesh::create(*db);
    if (!mesh || !mesh->load("media::cube/cube.model.xml")) {
        GN_ERROR(sLogger)("Failed to create and load mesh");
        return -1;
    }

    // Create backbuffer and reset
    auto backbuffer = Backbuffer::create(*db);
    if (!backbuffer || !backbuffer->reset(Backbuffer::Descriptor {displayWidth, displayHeight})) {
        GN_ERROR(sLogger)("Failed to create and initialize backbuffer");
        return -1;
    }

    // Create and initialize depth texture
    auto depthDesc    = Texture::Descriptor {};
    depthDesc.format  = gfx::img::PixelFormat::RG_24_UNORM_8_UINT();
    depthDesc.width   = displayWidth;
    depthDesc.height  = displayHeight;
    auto depthTexture = Texture::create(*db);
    if (!depthTexture || !depthTexture->reset(depthDesc)) {
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
    auto sampler          = Sampler::create(*db);
    if (!sampler || !sampler->reset(samplerDesc)) {
        GN_ERROR(sLogger)("Failed to create and initialize sampler");
        return -1;
    }

    // Create and initialize actions (each creates itself and registers via admit())
    auto prepareAction = PrepareBackbuffer::create(*db, "prepare_action");
    if (!prepareAction || !prepareAction->reset()) {
        GN_ERROR(sLogger)("Failed to create and initialize PrepareBackbuffer action");
        return -1;
    }

    auto clearAction = ClearRenderTarget::create(*db, "clear_action");
    if (!clearAction || !clearAction->reset()) {
        GN_ERROR(sLogger)("Failed to create and initialize ClearRenderTarget action");
        return -1;
    }

    auto clearDepthAction = ClearDepthStencil::create(*db, "clear_depth_action");
    if (!clearDepthAction || !clearDepthAction->reset()) {
        GN_ERROR(sLogger)("Failed to create and initialize ClearDepthStencil action");
        return -1;
    }

    auto presentAction = PresentBackbuffer::create(*db, "present_action");
    if (!presentAction || !presentAction->reset()) {
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
            auto prepareArgs   = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A());
            prepareArgs->backbuffer.set(backbuffer);
            prepareTask.arguments = prepareArgs;
            renderWorkflow->tasks.append(prepareTask);

            // Task: Clear render target
            auto clearTask   = Workflow::Task {};
            clearTask.action = clearAction;

            auto clearArgs = AutoRef<ClearRenderTarget::A>(new ClearRenderTarget::A());

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

            auto clearDepthArgs = AutoRef<ClearDepthStencil::A>(new ClearDepthStencil::A());

            clearDepthArgs->depth.set(1.0f);
            clearDepthArgs->stencil.set(0);

            auto depthRt   = RenderTarget {};
            depthRt.target = depthTexture;
            depthRt.sub    = Texture::SubresourceIndex();
            clearDepthArgs->depthStencil.set(depthRt);

            clearDepthTask.arguments = clearDepthArgs;
            renderWorkflow->tasks.append(clearDepthTask);

            // Task: Compose (draw mesh with texture) - disabled until Compose is uncommented in actions.h
            // auto composeTask = Workflow::Task {};
            // composeTask.action = composeAction;
            // auto composeArgs = AutoRef<Compose::A>(new Compose::A());
            // ... set composeArgs and append composeTask

            // Task: Present backbuffer
            auto presentTask   = Workflow::Task {};
            presentTask.action = presentAction;
            auto presentArgs   = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A());
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

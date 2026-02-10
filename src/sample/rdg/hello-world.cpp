#include "pch.h"

using namespace GN;
using namespace GN::rdg;

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

    // Create and initialize headless GPU context
    auto gpuContext = db->spawnAndReset<GpuContext>("gpu_context", GpuContext::ResetParameters {.win    = nullptr, // null window = headless mode
                                                                                                .width  = 1280,
                                                                                                .height = 720});
    if (!gpuContext) {
        GN_ERROR(sLogger)("Failed to create and initialize GPU context");
        return -1;
    }
    auto [displayWidth, displayHeight] = gpuContext->dimension();

    // Create and initialize backbuffer
    auto backbuffer = db->spawnAndReset<Backbuffer>("backbuffer", Backbuffer::Descriptor {displayWidth, displayHeight});
    if (!backbuffer) {
        GN_ERROR(sLogger)("Failed to create and initialize backbuffer");
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

    auto presentAction = db->spawnAndReset<PresentBackbuffer>("present_action");
    if (!presentAction) {
        GN_ERROR(sLogger)("Failed to create and initialize PresentBackbuffer action");
        return -1;
    }

    // Schedule render workflow
    auto renderWorkflow = renderGraph->schedule();
    if (renderWorkflow) {
        renderWorkflow->name = "Render";

        // Task: Prepare backbuffer
        auto prepareTask   = Workflow::Task {};
        prepareTask.action = prepareAction;
        auto prepareArgs   = db->spawn<PrepareBackbuffer::A>("prepare_args");
        prepareArgs->backbuffer.set(backbuffer);
        prepareTask.arguments = prepareArgs;
        renderWorkflow->tasks.append(prepareTask);

        // Task: Clear render target
        auto clearTask   = Workflow::Task {};
        clearTask.action = clearAction;
        auto clearArgs   = db->spawn<ClearRenderTarget::A>("clear_args");

        auto color = ClearRenderTarget::A::ClearColor {};
        color.r    = 0.39f;
        color.g    = 0.58f;
        color.b    = 0.93f;
        color.a    = 1.0f;
        clearArgs->color.set(color);

        auto rt   = RenderTarget {};
        rt.target = backbuffer;
        rt.sub    = Texture::SubresourceIndex();
        clearArgs->renderTarget.set(rt);

        clearTask.arguments = clearArgs;
        renderWorkflow->tasks.append(clearTask);

        // Task: Present backbuffer
        auto presentTask   = Workflow::Task {};
        presentTask.action = presentAction;
        auto presentArgs   = db->spawn<PresentBackbuffer::A>("present_args");
        presentArgs->backbuffer.set(backbuffer);
        presentTask.arguments = presentArgs;
        renderWorkflow->tasks.append(presentTask);
    }

    // Submit render graph for execution
    auto submission = renderGraph->submit({});
    if (!submission) {
        GN_ERROR(sLogger)("Failed to submit render graph");
        return -1;
    }

    // Wait for completion and get result
    auto result = submission->result();
    if (result.result != Action::ExecutionResult::PASSED) {
        GN_WARN(sLogger)("Render graph submission: {}", result.result == Action::ExecutionResult::FAILED ? "FAILED" : "WARNING");
    }

    GN_INFO(sLogger)("Render graph hello world completed");
    return 0;
}

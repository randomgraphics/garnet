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

    // Create GPU context (artifact creates itself and registers via admit() in Artifact ctor), then reset
    auto gpuContext = GpuContext::create(*db, "gpu_context", GpuContext::CreateParameters {.win = nullptr, .width = 1280, .height = 720});
    if (!gpuContext) return -1;
    auto [displayWidth, displayHeight] = gpuContext->dimension();

    // Create backbuffer and reset
    auto backbuffer = Backbuffer::create(*db, "backbuffer", Backbuffer::Descriptor {displayWidth, displayHeight});
    if (!backbuffer) return -1;

    // Create actions (each creates itself and registers via admit()), then reset
    auto prepareAction = PrepareBackbuffer::create(*db, "prepare_action", PrepareBackbuffer::CreateParameters {.context = gpuContext});

    auto clearAction = ClearRenderTarget::create(*db, "clear_action");
    if (!clearAction || !clearAction->reset()) {
        GN_ERROR(sLogger)("Failed to create and initialize ClearRenderTarget action");
        return -1;
    }

    auto presentAction = PresentBackbuffer::create(*db, "present_action");
    if (!presentAction || !presentAction->reset()) {
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
        auto prepareArgs   = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A());
        prepareArgs->backbuffer.set(backbuffer);
        prepareTask.arguments = prepareArgs;
        renderWorkflow->tasks.append(prepareTask);

        // Task: Clear render target
        auto clearTask   = Workflow::Task {};
        clearTask.action = clearAction;
        auto clearArgs   = AutoRef<ClearRenderTarget::A>(new ClearRenderTarget::A());

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
        auto presentArgs   = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A());
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

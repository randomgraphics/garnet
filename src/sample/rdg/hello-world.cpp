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

    // Create GPU context (artifact creates itself and registers via admit() in Artifact ctor)
    auto gpuContext = GpuContext::create(*db, "gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    // Create backbuffer (window and size are part of Backbuffer descriptor)
    const uint32_t displayWidth = 1280, displayHeight = 720;
    auto           backbuffer = Backbuffer::create(
                  *db, "backbuffer",
                  Backbuffer::CreateParameters {.context = gpuContext, .descriptor = {.win = nullptr, .width = displayWidth, .height = displayHeight}});
    if (!backbuffer) return -1;

    // Create actions
    auto prepareAction = PrepareBackbuffer::create(*db, "prepare_action", PrepareBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!prepareAction) return -1;

    auto clearAction = ClearRenderTarget::create(*db, "clear_action", ClearRenderTarget::CreateParameters {.gpu = gpuContext});
    if (!clearAction) return -1;

    auto presentAction = PresentBackbuffer::create(*db, "present_action", PresentBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!presentAction) return -1;

    // Schedule render workflow
    auto renderWorkflow = renderGraph->schedule();
    if (!renderWorkflow) {
        GN_ERROR(sLogger)("Failed to schedule render workflow");
        return -1;
    }

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

    auto color            = ClearRenderTarget::A::ClearValues {};
    color.colors[0].f4[0] = 0.39f;
    color.colors[0].f4[1] = 0.58f;
    color.colors[0].f4[2] = 0.93f;
    color.colors[0].f4[3] = 1.0f;
    clearArgs->clearValues.set(color);

    auto rt             = RenderTarget {};
    rt.colors[0].target = backbuffer;
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

    // Submit render graph for execution
    auto submission = renderGraph->submit({});
    if (!submission) {
        GN_ERROR(sLogger)("Failed to submit render graph");
        return -1;
    }

    // Wait for completion and get result
    auto result = submission->result();
    if (Action::ExecutionResult::FAILED == result.executionResult) {
        GN_ERROR(sLogger)("Render graph submission failed");
        return -1;
    }
    if (Action::ExecutionResult::WARNING == result.executionResult) { GN_WARN(sLogger)("Render graph submission completed with warnings"); }

    // Read texture back into an image, then save the image to file.
    auto image = backbuffer->readback();
    if (image.empty()) {
        GN_ERROR(sLogger)("Failed to get image from texture readback action");
        return -1;
    }
    image.save("output.png");

    GN_INFO(sLogger)("Render graph hello world completed");
    return 0;
}

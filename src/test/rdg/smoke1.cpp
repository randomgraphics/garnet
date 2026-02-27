#include <garnet/GNrdg.h>

using namespace GN;
using namespace GN::rdg;

static GN::Logger * sLogger = GN::getLogger("GN.test.rdg.smoke1");

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
    auto renderWorkflow = renderGraph->createWorkflow("Render");
    if (!renderWorkflow) {
        GN_ERROR(sLogger)("Failed to create render workflow");
        return -1;
    }

    // Task: Prepare backbuffer
    auto prepareTask              = Workflow::Task("Prepare backbuffer");
    prepareTask.action            = prepareAction;
    auto prepareArgs              = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A());
    prepareArgs->backbuffer.value = backbuffer;
    prepareTask.arguments         = prepareArgs;
    renderWorkflow->tasks.append(prepareTask);

    // Task: Clear render target (solid red for easy verification of readback/save)
    auto clearTask         = Workflow::Task("Clear render target");
    clearTask.action       = clearAction;
    auto clearArgs         = AutoRef<ClearRenderTarget::A>(new ClearRenderTarget::A());
    auto color             = ClearRenderTarget::A::ClearValues {};
    color.colors[0].f4[0]  = 1.0f; // R
    color.colors[0].f4[1]  = 0.0f; // G
    color.colors[0].f4[2]  = 0.0f; // B
    color.colors[0].f4[3]  = 1.0f; // A
    clearArgs->clearValues = color;
    auto rt                = RenderTarget {};
    rt.colors.append(GpuImageView {.image = backbuffer, .subresourceIndex = {}, .subresourceRange = {}});
    clearArgs->renderTarget.value = rt;
    clearTask.arguments           = clearArgs;
    renderWorkflow->tasks.append(clearTask);

    // Task: Present backbuffer
    auto presentTask              = Workflow::Task("Present backbuffer");
    presentTask.action            = presentAction;
    auto presentArgs              = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A());
    presentArgs->backbuffer.value = backbuffer;
    presentTask.arguments         = presentArgs;
    renderWorkflow->tasks.append(presentTask);

    // Submit render graph for execution
    auto submission = renderGraph->submit({.workflows = {&renderWorkflow, 1}});
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

    // Dump submission state to stdout
    auto dumpState = submission->dumpState();
    GN_INFO(sLogger)("Submission state:\n{}", dumpState.state);

    // Read texture back into an image, then save the image to file.
    auto image = backbuffer->readback();
    if (image.empty()) {
        GN_ERROR(sLogger)("Failed to get image from texture readback action");
        return -1;
    }
    // image.save("output.png");

    // Verify content: clear color is solid red (1, 0, 0, 1). Backbuffer may be RGBA or BGRA.
    {
        size_t          cx = image.width() / 2, cy = image.height() / 2;
        const uint8_t * p    = image.at({}, (size_t) cx, (size_t) cy, 0);
        uint32_t        step = (uint32_t) image.step();
        if (step >= 3) {
            // Expect either RGBA red (255,0,0,255) or BGRA red (0,0,255,255)
            bool isRedRGBA = (p[0] == 255 && p[1] == 0 && p[2] == 0);
            bool isRedBGRA = (p[0] == 0 && p[1] == 0 && p[2] == 255);
            if (isRedRGBA || isRedBGRA) {
                GN_INFO(sLogger)("output.png verified: center pixel matches clear color (red)");
            } else {
                GN_ERROR(sLogger)("output.png center pixel ({},{},{}) - expected red (255,0,0) or (0,0,255)", (int) p[0], (int) p[1], (int) p[2]);
                return -1;
            }
        }
    }

    GN_INFO(sLogger)("RDG smoke1 completed");
    return 0;
}

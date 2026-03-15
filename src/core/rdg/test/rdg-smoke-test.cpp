#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>

using namespace GN;
using namespace GN::rdg;

TEST_CASE("RDG smoke: prepare then present immediately (no draws)", "[rdg][smoke][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);

    auto renderGraph = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(renderGraph);

    auto gpuContext = GpuContext::create(*db, "gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    const uint32_t w = 64, h = 64;
    auto backbuffer = Backbuffer::create(*db, "backbuffer",
                                         Backbuffer::CreateParameters {.context = gpuContext, .descriptor = Backbuffer::Descriptor {}.setDimensions(w, h)});
    REQUIRE(backbuffer);

    auto prepareAction = PrepareBackbuffer::create(*db, "prepare", PrepareBackbuffer::CreateParameters {.gpu = gpuContext});
    REQUIRE(prepareAction);
    auto presentAction = PresentBackbuffer::create(*db, "present", PresentBackbuffer::CreateParameters {.gpu = gpuContext});
    REQUIRE(presentAction);

    auto wf = renderGraph->createWorkflow("Render");
    REQUIRE(wf);
    auto prepArgs        = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A());
    prepArgs->backbuffer = backbuffer;
    wf->appendTask(Workflow::Task("Prepare backbuffer", prepareAction, prepArgs));
    auto presentArgs        = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A());
    presentArgs->backbuffer = backbuffer;
    wf->appendTask(Workflow::Task("Present backbuffer", presentAction, presentArgs));

    auto submission = renderGraph->submit({.workflows = {&wf, 1}});
    REQUIRE(submission);
    REQUIRE(submission->result().executionResult != Action::ExecutionResult::FAILED);
}

TEST_CASE("RDG smoke: clear-to-red headless render", "[rdg][smoke][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);

    auto renderGraph = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(renderGraph);

    auto gpuContext = GpuContext::create(*db, "gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    const uint32_t displayWidth = 1280, displayHeight = 720;
    auto           backbuffer = Backbuffer::create(
        *db, "backbuffer",
        Backbuffer::CreateParameters {.context = gpuContext, .descriptor = Backbuffer::Descriptor {}.setDimensions(displayWidth, displayHeight)});
    REQUIRE(backbuffer);

    auto renderTarget = RenderTarget::create(*db, "render_target", RenderTarget::CreateParameters {});
    REQUIRE(renderTarget);
    renderTarget->addColorTarget(backbuffer);
    renderTarget->setClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    auto prepareAction = PrepareBackbuffer::create(*db, "prepare_action", PrepareBackbuffer::CreateParameters {.gpu = gpuContext});
    REQUIRE(prepareAction);

    auto clearAction = ClearRenderTarget::create(*db, "clear_action", ClearRenderTarget::CreateParameters {.gpu = gpuContext});
    REQUIRE(clearAction);

    auto presentAction = PresentBackbuffer::create(*db, "present_action", PresentBackbuffer::CreateParameters {.gpu = gpuContext});
    REQUIRE(presentAction);

    auto renderWorkflow = renderGraph->createWorkflow("Render");
    REQUIRE(renderWorkflow);

    auto prepareTask        = Workflow::Task("Prepare backbuffer");
    prepareTask.action      = prepareAction;
    auto prepareArgs        = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A());
    prepareArgs->backbuffer = backbuffer;
    prepareTask.arguments   = prepareArgs;
    renderWorkflow->appendTask(std::move(prepareTask));

    auto clearArgs          = AutoRef<ClearRenderTarget::A>(new ClearRenderTarget::A());
    clearArgs->renderTarget = renderTarget;
    renderWorkflow->appendTask(Workflow::Task("Clear render target", clearAction, clearArgs));

    auto presentTask        = Workflow::Task("Present backbuffer");
    presentTask.action      = presentAction;
    auto presentArgs        = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A());
    presentArgs->backbuffer = backbuffer;
    presentTask.arguments   = presentArgs;
    renderWorkflow->appendTask(std::move(presentTask));

    auto submission = renderGraph->submit({.workflows = {&renderWorkflow, 1}});
    REQUIRE(submission);

    auto result = submission->result();
    REQUIRE(result.executionResult != Action::ExecutionResult::FAILED);

    auto image = backbuffer->readback();
    REQUIRE_FALSE(image.empty());

    size_t          cx = image.width() / 2, cy = image.height() / 2;
    const uint8_t * p    = image.at({}, cx, cy, 0);
    uint32_t        step = (uint32_t) image.step();
    if (step >= 3) {
        bool isRedRGBA = (p[0] == 255 && p[1] == 0 && p[2] == 0);
        bool isRedBGRA = (p[0] == 0 && p[1] == 0 && p[2] == 255);
        CHECK((isRedRGBA || isRedBGRA));
    }
}

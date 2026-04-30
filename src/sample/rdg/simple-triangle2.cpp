#include <garnet/GNrdg2.h>
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>

#include "solid-triangle-vert.spv.h"
#include "solid-triangle-frag.spv.h"

using namespace GN;
using namespace GN::rdg2;
using namespace GN::gpu2;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.gpu2");

int main(int argc, const char ** argv) {

    bool testMode = (argc > 1) && (argv[1][0] == 't');
    if (testMode) { GN_INFO(sLogger)("Running in test mode"); }

    enableCRTMemoryCheck();

    // Create GPU context
    auto gpuContext = GpuContext::create("gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    // Create the open graph
    auto graph = Graph::create();
    if (!graph) return -1;

    // Create a main window and surface of 1280x720
    uint32_t                     windowWidth  = 1280;
    uint32_t                     windowHeight = 720;
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        window = std::unique_ptr<win::Window>(
            win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - GPU Triangle", .clientWidth = windowWidth, .clientHeight = windowHeight}));
        if (!window) return -1;
        window->show();

        // Window owns the surface; no need to destroy it explicitly. Destroy swapchain before window.
        surface = window->getVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
        if (!surface) return -1;
    }

    // Artifacts for vertex and pixel shaders (SPIR-V blobs via GpuShader::create).
    ArtifactPtr solidVs = graph->createArtifact("solid triangle VS");
    ArtifactPtr solidPs = graph->createArtifact("solid triangle PS");

    if (!graph->addNode(
            NodeDesc("create solid triangle shaders")
                .setAction(
                    Action::createFromLambda(
                        "build solid triangle shaders",
                        [&]() {
                            auto vertBlob = referenceTo(new SimpleBlob<unsigned int>(kSolidTriangleVertSpvSize, kSolidTriangleVertSpv));
                            auto fragBlob = referenceTo(new SimpleBlob<unsigned int>(kSolidTriangleFragSpvSize, kSolidTriangleFragSpv));
                            auto vs       = GpuShader::create(
                                {.context = gpuContext, .name = "solid triangle VS", .binary = vertBlob->data(), .size = vertBlob->size(), .entry = "main"});
                            auto ps = GpuShader::create(
                                {.context = gpuContext, .name = "solid triangle PS", .binary = fragBlob->data(), .size = fragBlob->size(), .entry = "main"});
                            if (!vs || !ps) return;
                            graph->publishArtifact(solidVs, vs);
                            graph->publishArtifact(solidPs, ps);
                        }),
                    nullptr))) {
        return -1;
    }

    // Create swapchain for presentation.
    Swapchain::CreateDesc scDesc {.gpu = gpuContext, .width = windowWidth, .height = windowHeight};
    if (surface) scDesc.setWindow(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) return -1;

    // Capture "shaders published at least once" tokens before the loop so they remain
    // satisfied on frame 2+ (getArtifactVersionToken with OOO() means "next version after
    // current" — calling it each frame would target a version the shader node never publishes).
    TokenPtr solidVsReady = graph->getArtifactVersionToken(solidVs, NeverOverflowingCounter::ONE());
    TokenPtr solidPsReady = graph->getArtifactVersionToken(solidPs, NeverOverflowingCounter::ONE());

    int totalFrames  = testMode ? 10 : 0;
    int frameCounter = 1;
    while (totalFrames == 0 || frameCounter++ <= totalFrames) {
        // process window events
        if (window && !window->runUntilNoNewEvents()) break;

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) return -1;

        // Record the color pass inside a graph node so shader artifacts are ready.
        AutoRef<GpuPayload> colorPassWork;
        auto                colorPassAction = [&]() {
            auto vs = graph->getTypedArtifactContent<AutoRef<GpuShader>>(solidVs);
            auto ps = graph->getTypedArtifactContent<AutoRef<GpuShader>>(solidPs);
            if (!vs || !ps) return;

            GpuRaster::CreateParameters rcp;
            rcp.gpu = gpuContext;
            rcp.renderTarget.addColorTarget(frame.view);

            auto                      r = GpuRaster::create(rcp);
            GpuRaster::DrawParameters drawParams;
            drawParams.vs                   = vs;
            drawParams.ps                   = ps;
            drawParams.geometry.vertexCount = 3; ///< Full-screen triangle from gl_VertexIndex (no vertex buffer).
            r->draw(drawParams);
            colorPassWork = r->seal();
        };
        auto colorPassNode = graph->addNode(NodeDesc("main color pass")
                                                .setAction(Action::createFromLambda("main color pass", colorPassAction), nullptr)
                                                .dependsOn(solidVsReady)
                                                .dependsOn(solidPsReady));

        // Wait for the color pass node to finish recording.
        graph->waitForToken(graph->getNodeCompletionToken(colorPassNode));

        // Submit: wait for swapchain image ready on GPU, then run the color pass.
        gpuContext->submit(GpuContext::SubmitParameters(StrA::format("frame {}", frameCounter)).appendWork(colorPassWork).waitFor(frame.ready));

        // Present after the color pass payload completes on GPU.
        swapchain->present(*colorPassWork);
    }

    return 0;
}

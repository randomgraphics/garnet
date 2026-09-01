#include <garnet/GNrdg2.h>
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>

#include "solid-triangle-vert.spv.h"
#include "solid-triangle-frag.spv.h"

using namespace GN;
using namespace GN::rdg2;
using namespace GN::gpu2;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.open-graph-simple-triangle");

struct ShaderArtifactContent final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    AutoRef<GpuShader> shader;

    explicit ShaderArtifactContent(AutoRef<GpuShader> shader_): Entity(TYPE_INFO(), "ShaderArtifactContent"), shader(std::move(shader_)) {}
};

/// Read back the freshly rendered backbuffer and verify the corner pixel matches
/// the clear color (blue) and the center pixel matches the triangle color (red).
/// The triangle covers the central ~1/3 of the screen in NDC (see solid-triangle.vert),
/// so the screen center is inside it. Must be called before swapchain->present(), since
/// present releases the active backbuffer and invalidates the frame view for readback.
/// Returns true on success.
static bool verifyBackbuffer(const GpuResourceView & view, uint32_t width, uint32_t height) {
    auto tex = view.texture();
    if (!tex) {
        GN_ERROR(sLogger, "verifyBackbuffer: could not get backbuffer texture");
        return false;
    }
    gfx::img::Image image = tex->readback();
    if (image.empty()) {
        GN_ERROR(sLogger, "verifyBackbuffer: readback returned empty image");
        return false;
    }
    auto         pixels   = image.plane().toRGBA8(image.data());
    const size_t expected = (size_t) width * (size_t) height;
    if (pixels.size() != expected) {
        GN_ERROR(sLogger, "verifyBackbuffer: unexpected pixel count {} (expected {})", pixels.size(), expected);
        return false;
    }
    // Expectations: clear color (0,0,1,1) → blue background, frag shader → red triangle.
    const auto & corner   = pixels[0];
    const auto & center   = pixels[(size_t) (height / 2) * width + width / 2];
    const bool   cornerOk = corner.r == 0 && corner.g == 0 && corner.b == 255;
    const bool   centerOk = center.r == 255 && center.g == 0 && center.b == 0;
    if (!cornerOk) {
        GN_ERROR(sLogger, "verifyBackbuffer: corner pixel mismatch — got ({},{},{},{}), expected (0,0,255,*) [background blue]", corner.r, corner.g, corner.b,
                 corner.a);
    }
    if (!centerOk) {
        GN_ERROR(sLogger, "verifyBackbuffer: center pixel mismatch — got ({},{},{},{}), expected (255,0,0,*) [triangle red]", center.r, center.g, center.b,
                 center.a);
    }
    if (!cornerOk || !centerOk) return false;
    GN_INFO(sLogger, "verifyBackbuffer: PASSED — corner=({},{},{},{}) center=({},{},{},{})", corner.r, corner.g, corner.b, corner.a, center.r, center.g,
            center.b, center.a);
    return true;
}

int main(int argc, const char ** argv) {

    bool testMode = (argc > 1) && (argv[1][0] == 't');
    if (testMode) { GN_INFO(sLogger, "Running in test mode"); }

    enableCRTMemoryCheck();

    // Create GPU context
    auto gpuContext = GpuContext::create("gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

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

        // The sample owns the surface; it is destroyed after the swapchain, before the GPU context.
        surface = window->createVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
        if (!surface) return -1;
    }

    // Create the open graph
    auto graph = Graph::create();
    if (!graph) return -1;

    // Artifacts for vertex and pixel shaders (SPIR-V blobs via GpuShader::create).
    ArtifactPtr solidVs = graph->createArtifact("solid triangle VS");
    ArtifactPtr solidPs = graph->createArtifact("solid triangle PS");

    // add node to create shaders.
    if (!graph->addNode(NodeDesc("create solid triangle shaders", [&]() {
            auto vertBlob = referenceTo(new SimpleBlob<unsigned int>(kSolidTriangleVertSpvSize, kSolidTriangleVertSpv));
            auto fragBlob = referenceTo(new SimpleBlob<unsigned int>(kSolidTriangleFragSpvSize, kSolidTriangleFragSpv));
            auto vs =
                GpuShader::create({.context = gpuContext, .name = "solid triangle VS", .binary = vertBlob->data(), .size = vertBlob->size(), .entry = "main"});
            auto ps =
                GpuShader::create({.context = gpuContext, .name = "solid triangle PS", .binary = fragBlob->data(), .size = fragBlob->size(), .entry = "main"});
            if (!vs || !ps) return;
            graph->publishArtifact(solidVs, AutoRef<ShaderArtifactContent>(new ShaderArtifactContent(vs)));
            graph->publishArtifact(solidPs, AutoRef<ShaderArtifactContent>(new ShaderArtifactContent(ps)));
        }))) {
        return -1;
    }

    // Create swapchain for presentation.
    Swapchain::CreateDesc scDesc {.gpu = gpuContext, .width = windowWidth, .height = windowHeight};
    if (surface) scDesc.setSurface(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) return -1;

    RasterTarget rasterTarget;
    rasterTarget.colorTargets.append(RasterTarget::ColorTarget {});
    rasterTarget.setClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Clear to solid blue.

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
        rasterTarget.setColorTarget(0, frame.view);

        // create the main rendering node.
        AutoRef<GpuPayload> colorPassWork;
        auto                colorPassAction = [&]() {
            auto vsContent = solidVs->content<ShaderArtifactContent>();
            auto psContent = solidPs->content<ShaderArtifactContent>();
            if (!vsContent || !psContent || !vsContent->shader || !psContent->shader) return;

            GpuRaster::CreateParameters rcp;
            rcp.gpu    = gpuContext;
            rcp.target = &rasterTarget;

            GpuRaster::DrawParameters drawParams;
            drawParams.vs                   = vsContent->shader;
            drawParams.ps                   = psContent->shader;
            drawParams.geometry.vertexCount = 3; ///< Full-screen triangle from gl_VertexIndex (no vertex buffer).

            auto r = GpuRaster::create("open-graph-simple-triangle", rcp);
            r->draw(drawParams);
            colorPassWork = r->seal();
        };
        auto colorPassNode = graph->addNode(NodeDesc("main color pass", colorPassAction).dependsOn(solidVsReady).dependsOn(solidPsReady));

        // Wait for the color pass node to generate GPU payload.
        graph->waitForToken(graph->getNodeCompletionToken(colorPassNode));

        // Submit the payload to GPU, waiting on swapchain image ready.
        gpuContext->submit(GpuContext::SubmitParameters(StrA::format("frame {}", frameCounter)).appendWork(colorPassWork).waitFor(frame.ready));

        // In test mode, verify the rendered output on the last frame.
        if (testMode && frameCounter > totalFrames) {
            if (!verifyBackbuffer(frame.view, windowWidth, windowHeight)) return -1;
        }

        // Present the frame, after the color pass payload
        swapchain->present(*colorPassWork);
    }

    // Destroy the surface after the swapchain and before the GPU context (the Vulkan instance).
    gpuContext->waitForIdle();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);
    return 0;
}

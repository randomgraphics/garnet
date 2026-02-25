#include "pch.h"

#include "solid-triangle-vert.spv.h"
#include "solid-triangle-frag.spv.h"

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

    // Create and load texture (commented out until Texture::load is implemented/linked)
    // auto texture = Texture::load(*db, Texture::LoadParameters {.context = gpuContext, .filename = "media::texture/earth.jpg"});
    // if (!texture) return -1;

    // Create and load mesh (commented out until Mesh::load is implemented/linked)
    // auto mesh = Mesh::load(*db, Mesh::LoadParameters {.context = gpuContext, .filename = "media::cube/cube.fbx"});
    // if (!mesh) return -1;

    // Create a main window of 1280x720
    auto window = win::createWindow(win::WindowCreateParameters {.caption = "Garnet 3D - Rendering Demo", .clientWidth = 1280, .clientHeight = 720});
    if (!window) return -1;

    // Create backbuffer (window and size are part of Backbuffer descriptor)
    auto backbuffer = Backbuffer::create(*db, "backbuffer", Backbuffer::CreateParameters {.context = gpuContext, .descriptor = {.win = window}});
    if (!backbuffer) return -1;

    // Depth texture not used by current workflow (clear + draw triangle to backbuffer only); skip until Texture::create path is implemented.
    // auto depthTexture = Texture::create(*db, "depth_texture", ...);

    // Create and initialize sampler (commented out until Sampler::create is implemented/linked)
    // auto samplerDesc      = Sampler::Descriptor {};
    // samplerDesc.filterMin = Sampler::Filter::LINEAR;
    // ...
    // auto sampler          = Sampler::create(*db, "sampler", Sampler::CreateParameters {.context = gpuContext, .descriptor = samplerDesc});
    // if (!sampler) return -1;

    // Create and initialize actions (each creates itself and registers via admit())
    auto prepareAction = PrepareBackbuffer::create(*db, "prepare_action", PrepareBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!prepareAction) return -1;

    auto clearAction = ClearRenderTarget::create(*db, "clear_action", ClearRenderTarget::CreateParameters {.gpu = gpuContext});
    if (!clearAction) return -1;

    // ClearDepthStencil not implemented yet; re-enable in Phase 5 when aligning sample APIs.
    // auto clearDepthAction = ClearDepthStencil::create(*db, "clear_depth_action", ClearDepthStencil::CreateParameters {.context = gpuContext});
    // if (!clearDepthAction) return -1;

    auto presentAction = PresentBackbuffer::create(*db, "present_action", PresentBackbuffer::CreateParameters {.gpu = gpuContext});
    if (!presentAction) return -1;

    // GenericDraw with SPIR-V from compiled headers (Phase 4); workflow task added in Phase 5.
    auto                          vertBlob = referenceTo(new SimpleBlob<unsigned int>(kSolidTriangleVertSpvSize, kSolidTriangleVertSpv));
    auto                          fragBlob = referenceTo(new SimpleBlob<unsigned int>(kSolidTriangleFragSpvSize, kSolidTriangleFragSpv));
    GenericDraw::CreateParameters drawParams {};
    drawParams.context     = gpuContext;
    drawParams.vs          = GenericDraw::ShaderStageDesc {.shaderBinary = vertBlob, .entryPoint = "main"};
    drawParams.ps          = GenericDraw::ShaderStageDesc {.shaderBinary = fragBlob, .entryPoint = "main"};
    auto genericDrawAction = GenericDraw::create(*db, "draw_triangle", drawParams);
    if (!genericDrawAction) return -1;

    GN_INFO(sLogger)("Starting render loop...");

    // Render loop: prepare, clear, compose, present until prepare fails
    while (true) {
        // Schedule render workflow
        auto renderWorkflow = renderGraph->schedule("Render");
        if (renderWorkflow) {
            renderWorkflow->name = "Render";

            // Task: Prepare backbufferg
            auto prepareTask              = Workflow::Task("Prepare");
            prepareTask.action            = prepareAction;
            auto prepareArgs              = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A());
            prepareArgs->backbuffer.value = backbuffer;
            prepareTask.arguments         = prepareArgs;
            renderWorkflow->tasks.append(prepareTask);

            // Task: Clear render target (clearValues + renderTarget; no depth/stencil clear for solid triangle)
            auto clearTask                              = Workflow::Task("Clear");
            clearTask.action                            = clearAction;
            auto                              clearArgs = AutoRef<ClearRenderTarget::A>(new ClearRenderTarget::A());
            ClearRenderTarget::A::ClearValues clearVals {};
            clearVals.colors[0].f4[0] = 0.2f;
            clearVals.colors[0].f4[1] = 0.3f;
            clearVals.colors[0].f4[2] = 0.4f;
            clearVals.colors[0].f4[3] = 1.0f;
            clearVals.depth           = 1.0f;
            clearVals.stencil         = 0;
            clearArgs->clearValues    = clearVals;
            RenderTarget clearRt {};
            clearRt.colors.append(RenderTarget::ColorTarget {.target = backbuffer, .subresourceIndex = {}});
            clearArgs->renderTarget.value = clearRt;
            clearTask.arguments           = clearArgs;
            renderWorkflow->tasks.append(clearTask);

            // Task: Draw solid triangle (GenericDraw; pipeline created in Phase 6)
            auto drawTask                          = Workflow::Task("DrawTriangle");
            drawTask.action                        = genericDrawAction;
            auto drawArgs                          = AutoRef<GenericDraw::A>(new GenericDraw::A());
            drawArgs->mesh.value.sub.vertexCount   = 3;
            drawArgs->mesh.value.sub.instanceCount = 1;
            drawArgs->mesh.value.sub.firstVertex   = 0;
            drawArgs->mesh.value.sub.firstInstance = 0;
            RenderTarget drawRt {};
            drawRt.colors.append(RenderTarget::ColorTarget {.target = backbuffer, .subresourceIndex = {}});
            drawArgs->renderTarget.value = drawRt;
            drawTask.arguments           = drawArgs;
            renderWorkflow->tasks.append(drawTask);

            // Task: Present backbuffer
            auto presentTask              = Workflow::Task("Present");
            presentTask.action            = presentAction;
            auto presentArgs              = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A());
            presentArgs->backbuffer.value = backbuffer;
            presentTask.arguments         = presentArgs;
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
        if (result.executionResult == Action::ExecutionResult::FAILED) {
            GN_INFO(sLogger)("Render graph submission failed (likely window closed), exiting");
            break;
        }

        if (result.executionResult == Action::ExecutionResult::WARNING) { GN_WARN(sLogger)("Render graph submission completed with warnings"); }
    }

    GN_INFO(sLogger)("Render graph draw mesh completed");
    return 0;
}

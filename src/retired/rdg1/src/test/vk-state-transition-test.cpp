/**
 * Tests for texture, backbuffer, and buffer state transition tracking on the
 * command buffer (first use = resource state, subsequent = CB state; flush on submit).
 */

#include "common.h"
#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>
#include "../vk-buffer-state.h"
#include "../vk-command-buffer.h"
#include "../vk-submission-context.h"
#include "../vk-texture.h"
#include "../vk-backbuffer.h"
#include "../vk-persistent-buffer.h"
#include <memory>

using namespace GN;
using namespace GN::rdg;

namespace {

TextureVulkan *          asTextureVulkan(Texture * t) { return t ? dynamic_cast<TextureVulkan *>(t) : nullptr; }
PersistentBufferVulkan * asPersistentBufferVulkan(Buffer * b) { return b ? dynamic_cast<PersistentBufferVulkan *>(b) : nullptr; }

static const TextureState::ImageState SHADER_READ {
    vk::ImageLayout::eShaderReadOnlyOptimal,
    vk::AccessFlagBits::eShaderRead,
    vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader,
};
static const TextureState::ImageState COLOR_ATTACHMENT {
    vk::ImageLayout::eColorAttachmentOptimal,
    vk::AccessFlagBits::eColorAttachmentWrite,
    vk::PipelineStageFlagBits::eColorAttachmentOutput,
};

// Custom action: in execute(), gets the command buffer, transitions texture/buffer (and
// optionally backbuffer), asserts get*State and transition return values, then lets CB
// submit so we can verify flush on the resources.
struct StateTransitionTestAction : public Action {
    GN_REGISTER_RUNTIME_TYPE(Action);

    AutoRef<GpuContextVulkan> mGpu;

    explicit StateTransitionTestAction(const StrA & name, AutoRef<GpuContextVulkan> gpu): Action(TYPE_INFO(), name), mGpu(std::move(gpu)) {}

    struct A : public Arguments {
        GN_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}
        AutoRef<Texture>    texture;
        AutoRef<Buffer>     buffer;
        AutoRef<Backbuffer> backbuffer; // optional; only exercised if non-null and has image

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (texture) list.readList.insert(texture.get());
            if (buffer) list.readList.insert(buffer.get());
            if (backbuffer) list.writeList.insert(backbuffer.get());
        }
    };

    Action::PrepareResult prepare(TaskInfo & taskInfo, Arguments &) override {
        auto & sc = taskInfo.submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(sc.commandBufferManager.prepare(taskInfo));
        return {PASSED, 1};
    }

    ExecutionResult execute(TaskInfo & taskInfo, size_t, Arguments & arguments) override {
        auto * a = static_cast<A *>(&arguments); // type guaranteed by appendTask
        if (!a) return FAILED;

        auto & sc = taskInfo.submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo, CommandBufferManagerVulkan::GRAPHICS);
        if (!cb) {
            // Debug: action failed because execute() returned a failed CommandBuffer (e.g. task not in manager, or begin() failed).
            return FAILED;
        }

        // ---- Texture ----
        TextureVulkan * tex = asTextureVulkan(a->texture.get());
        if (tex && tex->image()) {
            GpuResourceView::SubresourceRange range {};
            range.i                = {0, 0};
            range.e.numMipLevels   = 1;
            range.e.numArrayLayers = 1;

            // First transition: resource state (e.g. UNDEFINED) -> SHADER_READ; should return true.
            bool changed = cb.transitionTexture(tex, range, SHADER_READ);
            CHECK(changed);

            const auto * st = cb.getTextureState(tex, 0, 0);
            REQUIRE(st);
            CHECK(st->curr.layout == vk::ImageLayout::eShaderReadOnlyOptimal);
            CHECK(st->curr.access == vk::AccessFlagBits::eShaderRead);
            CHECK(st->prev.layout == vk::ImageLayout::eUndefined);
            CHECK(st->prev.access == vk::AccessFlagBits::eNone);

            // Same state again: no change; transitionTexture returns false.
            bool changed2 = cb.transitionTexture(tex, range, SHADER_READ);
            CHECK(!changed2);

            // Transition to another state.
            bool changed3 = cb.transitionTexture(tex, range, COLOR_ATTACHMENT);
            CHECK(changed3);
            const auto * st2 = cb.getTextureState(tex, 0, 0);
            REQUIRE(st2);
            CHECK(st2->curr.layout == vk::ImageLayout::eColorAttachmentOptimal);
            CHECK(st2->prev.layout == vk::ImageLayout::eShaderReadOnlyOptimal);
        }

        // ---- Buffer ----
        Buffer * buf = a->buffer.get();
        if (buf) {
            const vk::AccessFlags        shaderRead = vk::AccessFlagBits::eShaderRead;
            const vk::PipelineStageFlags stages     = vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader;
            vk::DeviceSize               offset = 0, size = 256;

            bool bufChanged = cb.transitionBuffer(buf, offset, size, shaderRead, stages);
            CHECK(bufChanged);

            const auto * bst = cb.getBufferState(buf);
            REQUIRE(bst);
            CHECK(bst->curr.access == shaderRead);
            CHECK(bst->curr.stage == stages);
            CHECK(bst->prev.access == vk::AccessFlagBits::eNone);
            CHECK(bst->prev.stage == vk::PipelineStageFlagBits::eBottomOfPipe);

            // Same state again: no change.
            bool bufChanged2 = cb.transitionBuffer(buf, offset, size, shaderRead, stages);
            CHECK(!bufChanged2);

            // New state.
            const vk::AccessFlags        transferRead  = vk::AccessFlagBits::eTransferRead;
            const vk::PipelineStageFlags transferStage = vk::PipelineStageFlagBits::eTransfer;
            bool                         bufChanged3   = cb.transitionBuffer(buf, offset, size, transferRead, transferStage);
            CHECK(bufChanged3);
            const auto * bst2 = cb.getBufferState(buf);
            REQUIRE(bst2);
            CHECK(bst2->curr.access == transferRead);
            CHECK(bst2->prev.access == shaderRead);
        }

        // ---- Backbuffer (only if we have an image; e.g. after PrepareBackbuffer) ----
        BackbufferVulkan * bb = a->backbuffer ? dynamic_cast<BackbufferVulkan *>(a->backbuffer.get()) : nullptr;
        if (bb && bb->backBufferImage()) {
            bool bbChanged = cb.transitionBackbuffer(bb, COLOR_ATTACHMENT);
            CHECK(bbChanged);
            const auto * bbst = cb.getBackbufferState(bb);
            REQUIRE(bbst);
            CHECK(bbst->curr.layout == vk::ImageLayout::eColorAttachmentOptimal);
        } else {
            // API contract: null bb or no image -> transitionBackbuffer returns false, getBackbufferState returns nullptr.
            CHECK(!cb.transitionBackbuffer(nullptr, SHADER_READ));
            CHECK(cb.getBackbufferState(nullptr) == nullptr);
            if (bb) CHECK(cb.getBackbufferState(bb) == nullptr); // no active image
        }

        return PASSED;
    }
};

} // namespace

// ---------------------------------------------------------------------------
// Texture state: first use copies from resource, same state no-op, flush on submit
// ---------------------------------------------------------------------------

TEST_CASE("StateTransition: texture transition and getTextureState", "[rdg][state-transition][gpu]") {
    auto gpu = GpuContext::create("gpu_tex", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto vkGpu = gpu.staticCastTo<GpuContextVulkan>();
    if (!vkGpu) SKIP("Not Vulkan");

    auto tex = Texture::create("tex_st", {
                                             .context = gpu,
                                             .descriptor =
                                                 {
                                                     .format = gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM(),
                                                     .width  = 4,
                                                     .height = 4,
                                                     .faces  = 1,
                                                     .levels = 1,
                                                 },
                                         });
    REQUIRE(tex);
    auto * tv = asTextureVulkan(tex.get());
    REQUIRE(tv);
    REQUIRE(tv->image());

    auto buf = PersistentBuffer::create("buf_st", {
                                                      .context = gpu,
                                                      .size    = 256,
                                                      .usage   = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM)),
                                                  });
    REQUIRE(buf);

    auto action   = AutoRef<StateTransitionTestAction>(new StateTransitionTestAction("state_test", vkGpu));
    auto args     = AutoRef<StateTransitionTestAction::A>::make();
    args->texture = tex;
    args->buffer  = buf;

    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto wf = rg->createWorkflow("wf");
    wf.appendTask("task", action, args);

    auto sub = rg->submit({.workflows = ArrayView<Workflow>(&wf, 1)});
    REQUIRE(sub);
    auto result = sub->result();
    CHECK(result.executionResult == Action::PASSED);

    // After submit, texture resource state should be flushed (last transition was COLOR_ATTACHMENT).
    const auto * resSt = tv->state().get(0, 0);
    REQUIRE(resSt);
    CHECK(resSt->curr.layout == vk::ImageLayout::eColorAttachmentOptimal);
    CHECK(resSt->curr.access == vk::AccessFlagBits::eColorAttachmentWrite);
}

// ---------------------------------------------------------------------------
// Buffer state: first use copies from resource, same state no-op, flush on submit
// ---------------------------------------------------------------------------

TEST_CASE("StateTransition: buffer transition and getBufferState", "[rdg][state-transition][gpu]") {
    auto gpu = GpuContext::create("gpu_buf", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto vkGpu = gpu.staticCastTo<GpuContextVulkan>();
    if (!vkGpu) SKIP("Not Vulkan");

    auto tex = Texture::create("tex_buf", {
                                              .context = gpu,
                                              .descriptor =
                                                  {
                                                      .format = gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM(),
                                                      .width  = 2,
                                                      .height = 2,
                                                      .faces  = 1,
                                                      .levels = 1,
                                                  },
                                          });
    REQUIRE(tex);

    auto buf = PersistentBuffer::create("buf_buf", {
                                                       .context = gpu,
                                                       .size    = 512,
                                                       .usage   = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM)),
                                                   });
    REQUIRE(buf);
    auto * pv = asPersistentBufferVulkan(buf.get());
    REQUIRE(pv);

    auto action   = AutoRef<StateTransitionTestAction>(new StateTransitionTestAction("state_buf", vkGpu));
    auto args     = AutoRef<StateTransitionTestAction::A>::make();
    args->texture = tex;
    args->buffer  = buf;

    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto wf = rg->createWorkflow("wf");
    wf.appendTask("task", action, args);

    auto sub = rg->submit({.workflows = ArrayView<Workflow>(&wf, 1)});
    REQUIRE(sub);
    CHECK(sub->result().executionResult == Action::PASSED);

    // Flush: buffer resource should have last transition (transfer read).
    CHECK(pv->state().curr.access == vk::AccessFlagBits::eTransferRead);
    CHECK(pv->state().curr.stage == vk::PipelineStageFlagBits::eTransfer);
}

// ---------------------------------------------------------------------------
// Backbuffer: transitionBackbuffer/getBackbufferState with null or no image
// ---------------------------------------------------------------------------

TEST_CASE("StateTransition: backbuffer API with null or no image", "[rdg][state-transition][gpu]") {
    auto gpu = GpuContext::create("gpu_bb", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto vkGpu = gpu.staticCastTo<GpuContextVulkan>();
    if (!vkGpu) SKIP("Not Vulkan");

    auto tex = Texture::create("tex_bb", {
                                             .context = gpu,
                                             .descriptor =
                                                 {
                                                     .format = gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM(),
                                                     .width  = 2,
                                                     .height = 2,
                                                     .faces  = 1,
                                                     .levels = 1,
                                                 },
                                         });
    REQUIRE(tex);
    auto buf = PersistentBuffer::create("buf_bb", {
                                                      .context = gpu,
                                                      .size    = 64,
                                                      .usage   = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM)),
                                                  });
    REQUIRE(buf);

    auto action   = AutoRef<StateTransitionTestAction>(new StateTransitionTestAction("state_bb", vkGpu));
    auto args     = AutoRef<StateTransitionTestAction::A>::make();
    args->texture = tex;
    args->buffer  = buf;
    // args->backbuffer left null; action will call transitionBackbuffer(nullptr) and getBackbufferState(nullptr).

    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto wf = rg->createWorkflow("wf");
    wf.appendTask("task", action, args);

    auto sub = rg->submit({.workflows = ArrayView<Workflow>(&wf, 1)});
    REQUIRE(sub);
    CHECK(sub->result().executionResult == Action::PASSED);
}

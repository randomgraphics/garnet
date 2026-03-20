#pragma once
#include "submission.h"
#include "vk-command-buffer.h"
#include "vk-gpu-context.h"
#include "vk-buffer-state.h"
#include "vk-texture.h"
#include <garnet/rdg/actions.h>
#include <vector>

namespace GN::rdg {

struct Buffer;

class ResourceTrackerVulkan;

class RenderPassManagerVulkan {
public:
    inline static const uint64_t TYPE = RuntimeType::getNextUniqueTypeId();

    struct ConstructParameters {
        AutoRef<GpuContextVulkan> gpu;
    };

    struct RenderPass : public NoCopy {

        RenderPass(const TaskInfo & taskInfo, Action::ExecutionResult result = Action::ExecutionResult::FAILED, vk::CommandBuffer commandBuffer = {},
                   const RenderTarget * drawTarget = nullptr)
            : mTaskInfo(taskInfo), mResult(result), mCommandBuffer(commandBuffer), mDrawTarget(drawTarget) {}

        RenderPass(RenderPass && o): mTaskInfo(o.mTaskInfo), mResult(o.mResult), mCommandBuffer(std::move(o.mCommandBuffer)), mDrawTarget(o.mDrawTarget) {
            o.mResult     = Action::ExecutionResult::FAILED;
            o.mDrawTarget = nullptr;
            GN_ASSERT(!o.mCommandBuffer);
        }

        ~RenderPass() {
            if (mResult == Action::ExecutionResult::PASSED && mCommandBuffer) {
                static auto sLogger = getLogger("GN.rdg.vk");
                GN_VERBOSE(sLogger)("{} - ending render pass", mTaskInfo);
                mCommandBuffer.endRendering();
            }
        }

        RenderPass & operator=(RenderPass && o) {
            if (this == &o) return *this;
            mResult        = o.mResult;
            mCommandBuffer = std::move(o.mCommandBuffer);
            mDrawTarget    = o.mDrawTarget;
            o.mResult      = Action::ExecutionResult::FAILED;
            o.mDrawTarget  = nullptr;
            GN_ASSERT(!o.mCommandBuffer);
            return *this;
        }

        operator Action::ExecutionResult() const { return mResult; }

        /// Current draw target for this executed entry; non-null only for draw tasks.
        const RenderTarget * drawTarget() const { return mDrawTarget; }

    private:
        const TaskInfo &        mTaskInfo;
        Action::ExecutionResult mResult        = Action::ExecutionResult::FAILED;
        vk::CommandBuffer       mCommandBuffer = {};
        const RenderTarget *    mDrawTarget    = nullptr;
    };

    RenderPassManagerVulkan(const ConstructParameters & params): mGpu(params.gpu) {}

    ~RenderPassManagerVulkan() = default;

    /// Called by tasks in prepare pass to collect render target information.
    /// \param taskInfo The task information.
    /// \param renderTarget The render target. Or null, if the resuing current render target.
    Action::ExecutionResult prepareDraw(TaskInfo & taskInfo, AutoRef<RenderTarget> renderTarget);

    /// Called by draw tasks in prepare pass to register texture transitions for the current render pass.
    /// Resources are transitioned to shader-read layout before the render pass begins (in execute).
    /// If the same resource is already registered with a different state in this pass, returns FAILED with a detailed error.
    Action::ExecutionResult registerDrawTextureTransitions(TaskInfo & taskInfo, const GpuShaderAction::GpuResourceTable & resources);

    /// Called by draw tasks in prepare pass to register buffer transitions for the current render pass.
    /// Same conflict rule as textures.
    Action::ExecutionResult registerDrawBufferTransitions(TaskInfo & taskInfo, const GpuShaderAction::GpuResourceTable & resources,
                                                          const GpuDraw::GpuGeometry & geom);

    /// Called by present backbuffer task to end rendering to backbuffer.
    /// If current render target is not this backbufer, then do nothing.
    Action::ExecutionResult preparePresent(TaskInfo & taskInfo, AutoRef<Backbuffer> backbuffer);

    /// Called by task in execution pass. Returns RenderPass with result, optional command buffer for ending, and current draw target (for draw tasks).
    /// \param cb RDG command buffer (for texture layout tracking and recording); null for tasks that do not record (e.g. present).
    RenderPass execute(TaskInfo & taskInfo, CommandBufferManagerVulkan::CommandBuffer * cb);

private:
    struct TextureTransitionKey {
        TextureVulkan * tex  = nullptr;
        uint32_t        mip  = 0;
        uint32_t        face = 0;
        bool            operator<(const TextureTransitionKey & o) const {
            if (tex != o.tex) return tex < o.tex;
            if (mip != o.mip) return mip < o.mip;
            return face < o.face;
        }
    };

    struct BufferTransitionKey {
        Buffer *       buffer = nullptr;
        vk::DeviceSize offset = 0;
        vk::DeviceSize size   = 0;
        bool           operator<(const BufferTransitionKey & o) const {
            if (buffer != o.buffer) return buffer < o.buffer;
            if (offset != o.offset) return offset < o.offset;
            return size < o.size;
        }
    };

    struct Entry {
        uint64_t              taskIndex            = 0;
        AutoRef<Backbuffer>   present              = {};
        AutoRef<RenderTarget> draw                 = {};
        int32_t               renderPassBeginIndex = -1; // index into mEntries of first entry in this pass; -1 if not in a pass (e.g. present)
        std::map<TextureTransitionKey, TextureVulkan::ImageState> textureTransitions;
        std::map<BufferTransitionKey, BufferState>                bufferTransitions;

        bool isDraw() const { return !present && !!draw; }
        bool isPresent() const { return !!present; }
    };

    AutoRef<GpuContextVulkan> mGpu;
    std::vector<Entry>        mEntries;                 // appended in prepare order; execute walks by index.
    size_t                    mCurrentExecuteIndex = 0; // next entry index to execute.

    bool beginRenderPass(const RenderTarget & renderTarget, vk::CommandBuffer vkCommandBuffer, CommandBufferManagerVulkan::CommandBuffer * rdgCommandBuffer,
                         const Entry * drawResourceTransitions);
};

} // namespace GN::rdg

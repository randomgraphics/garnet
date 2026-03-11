#pragma once
#include "vk-gpu-context.h"
#include "submission.h"

namespace GN::rdg {

class ResourceTrackerVulkan;

class RenderPassManagerVulkan {
public:
    inline static const uint64_t TYPE = getNextUniqueTypeId();

    struct ConstructParameters {
        AutoRef<GpuContextVulkan> gpu;
    };

    struct RenderPass : public NoCopy {

        RenderPass(const TaskInfo & taskInfo, Action::ExecutionResult result = Action::ExecutionResult::FAILED, vk::CommandBuffer commandBuffer = {})
            : mTaskInfo(taskInfo), mResult(result), mCommandBuffer(commandBuffer) {}

        RenderPass(RenderPass && o): mTaskInfo(o.mTaskInfo), mResult(o.mResult), mCommandBuffer(std::move(o.mCommandBuffer)) {
            o.mResult = Action::ExecutionResult::FAILED;
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
            o.mResult      = Action::ExecutionResult::FAILED;
            GN_ASSERT(!o.mCommandBuffer);
            return *this;
        }

        operator Action::ExecutionResult() const { return mResult; }

    private:
        const TaskInfo &        mTaskInfo;
        Action::ExecutionResult mResult        = Action::ExecutionResult::FAILED;
        vk::CommandBuffer       mCommandBuffer = {};
    };

    RenderPassManagerVulkan(const ConstructParameters & params): mGpu(params.gpu) {}

    ~RenderPassManagerVulkan() = default;

    /// Called by tasks in prepare pass to collect render target information.
    /// \param taskInfo The task information.
    /// \param renderTarget The render target. Or null, if the resuing current render target.
    Action::ExecutionResult prepareDraw(TaskInfo & taskInfo, AutoRef<RenderTarget> renderTarget);

    /// Called by present backbuffer task to end rendering to backbuffer.
    /// If current render target is not this backbufer, then do nothing.
    Action::ExecutionResult preparePresent(TaskInfo & taskInfo, AutoRef<Backbuffer> backbuffer);

    /// Called by task in execution pass to begin render pass.
    RenderPass execute(TaskInfo & taskInfo, vk::CommandBuffer commandBuffer);

    /// Current render target for the given task (set by prepareDraw). Null if task has no draw target.
    const RenderTarget * getCurrentDrawTarget(uint64_t taskIndex) const;

private:
    struct Entry {
        AutoRef<Backbuffer>   present = {};
        AutoRef<RenderTarget> draw    = {};

        bool isDraw() const { return !present && !!draw; }
        bool isPresent() const { return !!present; }
    };

    AutoRef<GpuContextVulkan> mGpu;
    std::map<uint64_t, Entry> mEntries; // key is the task index. Needs sorted container. Can't use unordered_map.

    bool beginRenderPass(const RenderTarget & renderTarget, vk::CommandBuffer commandBuffer);
};

} // namespace GN::rdg

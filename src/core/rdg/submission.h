#pragma once

#include <garnet/GNrdg.h>
#include <future>
#include <mutex>

namespace GN::rdg {

struct RenderPassManager : RefCounter {
public:
    virtual ~RenderPassManager() = default;
protected:
    RenderPassManager() = default;
};

/// Implementation of Submission. Holds all intermediate data and context for a single submit.
/// Processes workflows asynchronously: validate, build dependency graph, topological sort, execute.
class SubmissionImpl : public Submission {
public:
    struct Context : RefCounter {
    public:
        virtual ~Context() = default;
    protected:
        Context() = default;
    };

    /// Construct and start the submission asynchronously. Takes ownership of \p pendingWorkflows (pointers).
    SubmissionImpl(DynaArray<Workflow *> pendingWorkflows, const Parameters & params);

    ~SubmissionImpl() override;

    bool isFinished() override;

    Result result() override;

    auto setRenderPassManager(AutoRef<RenderPassManager> renderPassManager) -> void;
    auto getRenderPassManager() const -> AutoRef<RenderPassManager> { return mRenderPassManager; }

private:
    /// Deletes all work items and clears intermediate data (workflows, dependency graph). Safe to call multiple times.
    void cleanup(bool cleanupPendingWorkflows = true) noexcept;

    std::future<Result> mFuture;
    Result              mResult;
    std::mutex          mResultMutex;

    AutoRef<RenderPassManager>   mRenderPassManager;

    // Owned workflows (taken from graph on construction)
    DynaArray<Workflow *>        mWorkflows;
    DynaArray<Workflow *>        mValidatedWorkflows;
    DynaArray<DynaArray<size_t>> mDependencyGraph;

    bool              validateTask(const Workflow::Task & task, const StrA & workflowName, size_t taskIndex);
    bool              validateAndBuildDependencyGraph();
    DynaArray<size_t> topologicalSort();
    Result            run(Parameters params);
};

} // namespace GN::rdg

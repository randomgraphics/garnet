#pragma once

#include <garnet/GNrdg.h>
#include <future>
#include <mutex>

namespace GN::rdg {

/// Implementation of Submission. Holds all intermediate data and context for a single submit.
/// Processes workflows asynchronously: validate, build dependency graph, topological sort, execute.
class SubmissionImpl : public Submission {
public:
    /// Construct and start the submission asynchronously. Takes ownership of \p pendingWorkflows (pointers).
    SubmissionImpl(DynaArray<Workflow *> pendingWorkflows, const Parameters & params);

    ~SubmissionImpl() override;

    bool isFinished() override;
    Result result() override;

private:
    /// Deletes all work items and clears intermediate data (workflows, dependency graph). Safe to call multiple times.
    void cleanup();

    std::future<void>       mFuture;
    std::mutex              mResultMutex;
    Action::ExecutionResult mResult;
    StrA                    mDebugStats;
    bool                    mFinished;

    // Owned workflows (taken from graph on construction)
    DynaArray<Workflow *> mWorkflows;
    // Validated workflows (subset / reordered for execution)
    DynaArray<Workflow *> mValidatedWorkflows;
    // For each workflow index, indices of workflows it depends on
    DynaArray<DynaArray<size_t>> mDependencyGraph;

    bool validateTask(const Workflow::Task & task, const StrA & workflowName, size_t taskIndex);
    bool validateAndBuildDependencyGraph();
    DynaArray<size_t> topologicalSort();
    Action::ExecutionResult executeTask(const Workflow::Task & task, Workflow * workflow, size_t taskIndex);
    Action::ExecutionResult executeWorkflow(size_t workflowIndex);
    void run(Parameters params);
};

} // namespace GN::rdg

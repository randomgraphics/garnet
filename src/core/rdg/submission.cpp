#include "submission.h"
#include <limits>
#include <chrono>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

SubmissionImpl::SubmissionImpl(DynaArray<Workflow *> pendingWorkflows, const Parameters & params): mResult(Action::ExecutionResult::PASSED), mFinished(false) {
    mWorkflows = std::move(pendingWorkflows);
    mFuture    = std::async(std::launch::async, [this, params]() { run(params); });
}

SubmissionImpl::~SubmissionImpl() {
    // Always wait for pending run to finish before destroying; then cleanup work items.
    if (mFuture.valid()) mFuture.get();
    cleanup();
}

void SubmissionImpl::cleanup() {
    for (Workflow * w : mWorkflows) delete w;
    mWorkflows.clear();
    mValidatedWorkflows.clear();
    mDependencyGraph.clear();
}

bool SubmissionImpl::isFinished() { return mFuture.valid() && mFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

Submission::Result SubmissionImpl::result() {
    if (mFuture.valid()) mFuture.get();
    std::lock_guard<std::mutex> lock(mResultMutex);
    Result                      r;
    r.result     = mResult;
    r.debugStats = mDebugStats;
    return r;
}

bool SubmissionImpl::validateTask(const Workflow::Task & task, const StrA & workflowName, size_t taskIndex) {
    if (!task.action) {
        GN_ERROR(sLogger)("Workflow '{}' task {}: action is null", workflowName, taskIndex);
        return false;
    }
    if (!task.arguments) {
        GN_ERROR(sLogger)("Workflow '{}' task {}: arguments is null", workflowName, taskIndex);
        return false;
    }
    return true;
}

bool SubmissionImpl::validateAndBuildDependencyGraph() {
    for (size_t workflowIdx = 0; workflowIdx < mWorkflows.size(); ++workflowIdx) {
        Workflow * workflow = mWorkflows[workflowIdx];
        GN_ASSERT(workflow);

        for (size_t taskIdx = 0; taskIdx < workflow->tasks.size(); ++taskIdx) {
            const Workflow::Task & task = workflow->tasks[taskIdx];
            if (!validateTask(task, workflow->name, taskIdx)) return false;
        }

        mValidatedWorkflows.append(workflow);
    }

    mDependencyGraph.resize(mValidatedWorkflows.size());

    for (size_t workflowIdx = 0; workflowIdx < mValidatedWorkflows.size(); ++workflowIdx) {
        Workflow * workflow = mValidatedWorkflows[workflowIdx];

        for (Workflow * depWorkflow : workflow->dependencies) {
            size_t depWorkflowIdx = std::numeric_limits<size_t>::max();
            for (size_t i = 0; i < mValidatedWorkflows.size(); ++i) {
                if (mValidatedWorkflows[i] == depWorkflow) {
                    depWorkflowIdx = i;
                    break;
                }
            }

            if (depWorkflowIdx == std::numeric_limits<size_t>::max()) {
                GN_ERROR(sLogger)("Workflow '{}' depends on workflow that is not in the validated workflow list", workflow->name);
                return false;
            }

            mDependencyGraph[workflowIdx].append(depWorkflowIdx);
        }
    }

    return true;
}

DynaArray<size_t> SubmissionImpl::topologicalSort() {
    DynaArray<size_t> result;

    DynaArray<DynaArray<size_t>> reverseGraph;
    reverseGraph.resize(mValidatedWorkflows.size());

    for (size_t i = 0; i < mDependencyGraph.size(); ++i) {
        for (size_t depIdx : mDependencyGraph[i]) { reverseGraph[depIdx].append(i); }
    }

    DynaArray<size_t> inDegree(mValidatedWorkflows.size(), 0);
    for (size_t i = 0; i < mDependencyGraph.size(); ++i) { inDegree[i] = mDependencyGraph[i].size(); }

    DynaArray<size_t> queue;
    for (size_t i = 0; i < inDegree.size(); ++i) {
        if (inDegree[i] == 0) { queue.append(i); }
    }

    while (!queue.empty()) {
        size_t current = queue.front();
        queue.eraseIdx(0);
        result.append(current);

        for (size_t dependentIdx : reverseGraph[current]) {
            GN_ASSERT(inDegree[dependentIdx] > 0);
            inDegree[dependentIdx]--;
            if (inDegree[dependentIdx] == 0) { queue.append(dependentIdx); }
        }
    }

    if (result.size() != mValidatedWorkflows.size()) {
        GN_ERROR(sLogger)("Circular dependency detected in workflow graph");
        result.clear();
        return result;
    }

    return result;
}

Action::ExecutionResult SubmissionImpl::executeTask(const Workflow::Task & task, Workflow * workflow, size_t taskIndex) {
    if (!task.action) {
        GN_ERROR(sLogger)("Workflow '{}' task {}: action is null", workflow->name, taskIndex);
        return Action::ExecutionResult::FAILED;
    }
    if (!task.arguments) {
        GN_ERROR(sLogger)("Workflow '{}' task {}: arguments is null", workflow->name, taskIndex);
        return Action::ExecutionResult::FAILED;
    }
    return task.action->execute(*task.arguments);
}

Action::ExecutionResult SubmissionImpl::executeWorkflow(size_t workflowIndex) {
    Workflow * workflow = mValidatedWorkflows[workflowIndex];
    GN_ASSERT(workflow);

    for (size_t taskIdx = 0; taskIdx < workflow->tasks.size(); ++taskIdx) {
        const Workflow::Task &  task   = workflow->tasks[taskIdx];
        Action::ExecutionResult result = executeTask(task, workflow, taskIdx);

        if (result == Action::ExecutionResult::FAILED) {
            GN_ERROR(sLogger)("Workflow '{}' task {} execution failed", workflow->name, taskIdx);
            return Action::ExecutionResult::FAILED;
        }

        if (result == Action::ExecutionResult::WARNING) {
            GN_WARN(sLogger)("Workflow '{}' task {} execution completed with warnings", workflow->name, taskIdx);
        }
    }

    return Action::ExecutionResult::PASSED;
}

void SubmissionImpl::run(Parameters params) {
    cleanup(); // clean up any residual data from previous runs.

    auto setResult = [this](Action::ExecutionResult result, StrA debugStats) {
        std::lock_guard<std::mutex> lock(mResultMutex);
        mResult     = result;
        mDebugStats = std::move(debugStats);
        mFinished   = true;
    };

    if (!validateAndBuildDependencyGraph()) {
        setResult(Action::ExecutionResult::FAILED, "Workflow graph compilation failed");
        cleanup();
        return;
    }

    DynaArray<size_t> executionOrder = topologicalSort();
    if (executionOrder.empty()) {
        setResult(Action::ExecutionResult::FAILED, "Topological sort failed - circular dependency detected");
        cleanup();
        return;
    }

    bool         hasWarning     = false;
    const size_t totalWorkflows = executionOrder.size();

    for (size_t workflowIdx : executionOrder) {
        Action::ExecutionResult result = executeWorkflow(workflowIdx);

        if (result == Action::ExecutionResult::FAILED) {
            GN_ERROR(sLogger)("Workflow '{}' execution failed, stopping execution", mValidatedWorkflows[workflowIdx]->name);
            setResult(Action::ExecutionResult::FAILED, params.debug ? StrA::format("Execution failed at workflow index {}", workflowIdx) : StrA());
            cleanup();
            return;
        }

        if (result == Action::ExecutionResult::WARNING) {
            GN_WARN(sLogger)("Workflow '{}' execution completed with warnings", mValidatedWorkflows[workflowIdx]->name);
            hasWarning = true;
        }
    }

    Action::ExecutionResult finalResult = hasWarning ? Action::ExecutionResult::WARNING : Action::ExecutionResult::PASSED;
    if (hasWarning) {
        GN_TRACE(sLogger)("Successfully executed {} workflows sequentially with warnings", totalWorkflows);
    } else {
        GN_TRACE(sLogger)("Successfully executed {} workflows sequentially", totalWorkflows);
    }

    StrA debugStats;
    if (params.debug) {
        debugStats = StrA::format("Executed {} workflows successfully", totalWorkflows);
        if (hasWarning) { debugStats += " (with warnings)"; }
    }
    setResult(finalResult, std::move(debugStats));
    cleanup();
}

} // namespace GN::rdg

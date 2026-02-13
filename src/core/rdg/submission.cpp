#include "submission.h"
#include <limits>
#include <chrono>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

SubmissionImpl::SubmissionImpl(DynaArray<Workflow *> pendingWorkflows, const Parameters & params) {
    GN_VERBOSE(sLogger)("SubmissionImpl constructor: {} workflows.", pendingWorkflows.size());
    mWorkflows = std::move(pendingWorkflows);
    mFuture    = std::async(std::launch::async, [this, params]() -> Result { return run(params); });
}

SubmissionImpl::~SubmissionImpl() {
    // Always wait for pending run to finish before destroying; then cleanup work items.
    if (mFuture.valid()) mFuture.get();
    cleanup();
}

void SubmissionImpl::cleanup(bool cleanupPendingWorkflows) noexcept {
    try {
        if (cleanupPendingWorkflows) {
            for (Workflow * w : mWorkflows) delete w;
            mWorkflows.clear();
        }
        mValidatedWorkflows.clear();
        mDependencyGraph.clear();
    } catch (...) { GN_ERROR(sLogger)("Exception occurred during cleanup."); }
}

bool SubmissionImpl::isFinished() { return mFuture.valid() && mFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

Submission::Result SubmissionImpl::result() {
    std::lock_guard<std::mutex> lock(mResultMutex);
    if (mFuture.valid()) mResult = std::move(mFuture.get());
    return mResult;
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

    GN_VERBOSE(sLogger)("Validated {} workflows.", mValidatedWorkflows.size());

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

    GN_VERBOSE(sLogger)("Dependency graph built: {} workflows.", mValidatedWorkflows.size());
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

Submission::Result SubmissionImpl::run(Parameters) {
    cleanup(false); // clean up any residual data from previous runs, but keep pending workflows.

    auto setResult = [this](Action::ExecutionResult executionResult) -> Result {
        Result r;
        r.executionResult = executionResult;
        return r;
    };

    // shortcut for empty workflows.
    if (mWorkflows.empty()) {
        GN_VERBOSE(sLogger)("No workflows to execute, returning PASSED.");
        return setResult(Action::ExecutionResult::PASSED);
    }

    try {
        // step 1: validate and build dependency graph.
        if (!validateAndBuildDependencyGraph()) {
            cleanup();
            return setResult(Action::ExecutionResult::FAILED);
        }

        // step 2: topological sort.
        DynaArray<size_t> executionOrder = topologicalSort();
        if (executionOrder.empty()) {
            GN_ERROR(sLogger)("Topological sort failed - circular dependency detected");
            cleanup();
            return setResult(Action::ExecutionResult::FAILED);
        }

        // step 3: prepare all tasks in topological order.
        bool hasWarning = false;
        for (size_t workflowIdx : executionOrder) {
            Workflow * workflow = mValidatedWorkflows[workflowIdx];
            GN_ASSERT(workflow);
            for (size_t taskIdx = 0; taskIdx < workflow->tasks.size(); ++taskIdx) {
                const Workflow::Task & task = workflow->tasks[taskIdx];
                GN_ASSERT(task.action && task.arguments); // have been validated in validateTask().
                auto result = task.action->prepare(*task.arguments);
                if (result == Action::ExecutionResult::FAILED) {
                    GN_ERROR(sLogger)("Task '{}' preparation failed", task.action->name);
                    return setResult(Action::ExecutionResult::FAILED);
                }
                if (result == Action::ExecutionResult::WARNING) {
                    GN_VERBOSE(sLogger)("Task '{}' preparation completed with warnings", task.action->name);
                    hasWarning = true;
                }
            }
        }

        // step 4: execute workflows sequentially in topological order.
        for (size_t workflowIdx : executionOrder) {
            Workflow * workflow = mValidatedWorkflows[workflowIdx];
            GN_ASSERT(workflow);
            for (size_t taskIdx = 0; taskIdx < workflow->tasks.size(); ++taskIdx) {
                const Workflow::Task & task = workflow->tasks[taskIdx];
                GN_ASSERT(task.action && task.arguments); // have been validated in validateTask().
                auto result = task.action->execute(*task.arguments);
                if (result == Action::ExecutionResult::FAILED) {
                    GN_ERROR(sLogger)("Task '{}' execution failed", task.action->name);
                    return setResult(Action::ExecutionResult::FAILED);
                } else if (result == Action::ExecutionResult::WARNING) {
                    GN_VERBOSE(sLogger)("Task '{}' execution completed with warnings", task.action->name);
                    hasWarning = true;
                }
            }
        }

        // Done
        return setResult(hasWarning ? Action::ExecutionResult::WARNING : Action::ExecutionResult::PASSED);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger)("Exception occurred during submission: {}", e.what());
        cleanup();
        return setResult(Action::ExecutionResult::FAILED);
    } catch (...) {
        GN_ERROR(sLogger)("Exception occurred during submission.");
        cleanup();
        return setResult(Action::ExecutionResult::FAILED);
    }
}

} // namespace GN::rdg

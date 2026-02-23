#include "pch.h"
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
    if (mFuture.valid()) mResult = mFuture.get();
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

    auto setResult = [](Action::ExecutionResult executionResult) -> Result {
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
        GN_ASSERT(executionOrder.size() == mValidatedWorkflows.size());

        // step 3: prepare all tasks in topological order.
        struct PendingTask {
            Workflow::Task *                          task;
            TaskInfo                                  info;
            std::unique_ptr<Action::ExecutionContext> context;
        };
        DynaArray<PendingTask> pendingTasks;
        bool                   hasWarning = false;
        for (size_t workflowIdx : executionOrder) {
            Workflow * workflow = mValidatedWorkflows[workflowIdx];
            GN_ASSERT(workflow);
            for (size_t taskIdx = 0; taskIdx < workflow->tasks.size(); ++taskIdx) {
                Workflow::Task & task = workflow->tasks[taskIdx];
                GN_ASSERT(task.action && task.arguments); // have been validated in validateTask().
                pendingTasks.append(PendingTask {.task    = &task,
                                                 .info    = TaskInfo {.submission = *this,
                                                                      .workflow   = workflow->name.empty() ? StrA("[unnamed workflow]") : workflow->name,
                                                                      .task       = task.name.empty() ? StrA("[unnamed task]") : task.name,
                                                                      .index      = (uint64_t) pendingTasks.size()},
                                                 .context = {}});
                auto & pt = pendingTasks.back();
                GN_VERBOSE(sLogger)("Preparing workflow '{}' task '{}'", pt.info.workflow, pt.info.task);
                auto [result, context] = task.action->prepare(pt.info, *task.arguments);
                pt.context = std::unique_ptr<Action::ExecutionContext>(context); // need to do this before error check to ensure it is released even on error.
                if (result == Action::ExecutionResult::FAILED) {
                    GN_ERROR(sLogger)("Workflow '{}' task '{}' preparation failed", pt.info.workflow, pt.info.task);
                    return setResult(Action::ExecutionResult::FAILED);
                }
                if (result == Action::ExecutionResult::WARNING) {
                    GN_VERBOSE(sLogger)("Workflow '{}' task '{}' preparation completed with warnings", pt.info.workflow, pt.info.task);
                    hasWarning = true;
                }
            }
        }

        // Emit prepare signal and check for errors.
        auto signalResults = allTasksPrepared.emit(*this);
        for (auto r : signalResults.results) {
            if (r == Action::ExecutionResult::FAILED) { return setResult(Action::ExecutionResult::FAILED); }
            if (r == Action::ExecutionResult::WARNING) { hasWarning = true; }
        }

        // step 4: execute workflows sequentially in topological order.
        for (size_t i = 0; i < pendingTasks.size(); ++i) {
            auto & pt = pendingTasks[i];
            GN_VERBOSE(sLogger)("Executing workflow '{}' task '{}'", pt.info.workflow, pt.info.task);
            auto result = pt.task->action->execute(pt.info, *pt.task->arguments, pt.context.get());
            if (result == Action::ExecutionResult::FAILED) {
                GN_ERROR(sLogger)("Workflow '{}' task '{}' execution failed", pt.info.workflow, pt.info.task);
                return setResult(Action::ExecutionResult::FAILED);
            }
            if (result == Action::ExecutionResult::WARNING) {
                GN_VERBOSE(sLogger)("Workflow '{}' task '{}' execution completed with warnings", pt.info.workflow, pt.info.task);
                hasWarning = true;
            }
        }

        // Emit execute signal and check for errors.
        signalResults = allTasksExecuted.emit(*this);
        for (auto r : signalResults.results) {
            if (r == Action::ExecutionResult::FAILED) { return setResult(Action::ExecutionResult::FAILED); }
            if (r == Action::ExecutionResult::WARNING) { hasWarning = true; }
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

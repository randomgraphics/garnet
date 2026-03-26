#include "pch.h"
#include "submission.h"
#include <chrono>
#include <unordered_set>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

SubmissionImpl::SubmissionImpl(DynaArray<WorkflowImplPayload *> pendingWorkflows, const RenderGraph::SubmitParameters & params): Submission(params.name) {
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
            for (WorkflowImplPayload * w : mWorkflows) delete w;
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
        GN_ERROR(sLogger)("Workflow '{}' task[{}] {}: action is null", workflowName, taskIndex, task.name);
        return false;
    }
    if (!task.arguments) {
        GN_ERROR(sLogger)("Workflow '{}' task[{}] {}: arguments is null", workflowName, taskIndex, task.name);
        return false;
    }
    return true;
}

bool SubmissionImpl::validateAndBuildDependencyGraph() {
    for (size_t workflowIdx = 0; workflowIdx < mWorkflows.size(); ++workflowIdx) {
        auto * payload = mWorkflows[workflowIdx];
        GN_ASSERT(payload);

        for (size_t taskIdx = 0; taskIdx < payload->tasks.size(); ++taskIdx) {
            const Workflow::Task & task = payload->tasks[taskIdx];
            if (!validateTask(task, payload->name, taskIdx)) return false;
        }

        mValidatedWorkflows.append(payload);
    }

    GN_VERBOSE(sLogger)("Validated {} workflows.", mValidatedWorkflows.size());

    mDependencyGraph.resize(mValidatedWorkflows.size());

    // Collect per-workflow: artifacts read and written (from task arguments via firstArtifactArgument/next).
    using ArtifactSet = std::unordered_set<const Artifact *>;
    DynaArray<ArtifactSet> workflowReads(mValidatedWorkflows.size());
    DynaArray<ArtifactSet> workflowWrites(mValidatedWorkflows.size());
    for (size_t i = 0; i < mValidatedWorkflows.size(); ++i) {
        auto * w = mValidatedWorkflows[i];
        for (const Workflow::Task & task : w->tasks) {
            Arguments * args = task.arguments.get();
            if (!args) continue;
            Arguments::ArtifactReadWriteList list {workflowReads[i], workflowWrites[i]};
            args->addToReadWriteList(list);
        }
    }

    // Build edges: A depends on B (B must run before A) per dependency-graph.h Task comments:
    // "Task A depends on task B only if A is newer than B and any of: (1) A reads or writes an artifact B writes to,
    //  (2) A writes an artifact B reads from." So for workflow i (newer) and j (older): i depends on j when
    // (i reads or writes X and j writes X) or (i writes X and j reads X).
    for (size_t i = 0; i < mValidatedWorkflows.size(); ++i) {
        for (size_t j = 0; j < mValidatedWorkflows.size(); ++j) {
            if (i == j) continue;
            if (mValidatedWorkflows[i]->sequence <= mValidatedWorkflows[j]->sequence) continue;
            bool conflict = false;
            for (const Artifact * a : workflowWrites[j]) {
                if (workflowReads[i].count(a) || workflowWrites[i].count(a)) {
                    conflict = true;
                    break;
                }
            }
            if (!conflict) {
                for (const Artifact * a : workflowReads[j]) {
                    if (workflowWrites[i].count(a)) {
                        conflict = true;
                        break;
                    }
                }
            }
            if (conflict) mDependencyGraph[i].append(j);
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

static const char * executionResultStr(Action::ExecutionResult r) {
    switch (r) {
    case Action::ExecutionResult::PASSED:
        return "PASSED";
    case Action::ExecutionResult::WARNING:
        return "WARNING";
    case Action::ExecutionResult::FAILED:
        return "FAILED";
    default:
        return "?";
    }
}

// static StrA usageFlagStr(Arguments::UsageBits u) {
//     StrA s;
//     if (u.optional) s += "Optional|";
//     if (u.reading) s += "Reading|";
//     if (u.writing) s += "Writing|";
//     if (s.empty())
//         s = "None";
//     else
//         s.popback(); // trailing |
//     return s;
// }

Submission::State SubmissionImpl::dumpState() const {
    std::lock_guard<std::mutex> lock(mStateMutex);

    Submission::State result;
    StrA &            out = result.state;
    out += "========== Submission State ==========\n";
    out += StrA::format("  Finished:        {}\n", mRunResult.has_value() ? "yes" : "no");
    if (mRunResult) {
        out += StrA::format("  Overall result:  {}\n", executionResultStr(*mRunResult));
    } else {
        out += "  Overall result:  (not yet completed or not yet retrieved)\n";
    }
    out += StrA::format("  Workflows:       {} total, {} validated\n", mWorkflows.size(), mValidatedWorkflows.size());
    out += StrA::format("  Tasks recorded:  {}\n", mTaskStates.size());
    out += "--------------------------------------\n";

    if (mExecutionOrder.empty() && mTaskStates.empty()) {
        out += "  (No task state yet - submission may not have started or validation failed.)\n";
        out += "==========================================\n";
        return result;
    }

    // Dump by execution order: for each workflow in mExecutionOrder, list its tasks from mTaskStates.
    for (size_t orderIdx = 0; orderIdx < mExecutionOrder.size(); ++orderIdx) {
        size_t wfIdx = mExecutionOrder[orderIdx];
        if (wfIdx >= mValidatedWorkflows.size()) continue;
        auto * payload = mValidatedWorkflows[wfIdx];
        if (!payload) continue;

        out += StrA::format("\n--- Workflow [{}] \"{}\" (sequence={}, order={}) ---\n", wfIdx, payload->name.empty() ? "[unnamed]" : payload->name.c_str(),
                            (long long) payload->sequence, (unsigned long) orderIdx);

        // Dependencies (workflows that must run before this one)
        if (wfIdx < mDependencyGraph.size() && !mDependencyGraph[wfIdx].empty()) {
            out += "  Depends on workflows: ";
            for (size_t k = 0; k < mDependencyGraph[wfIdx].size(); ++k) {
                if (k > 0) out += ", ";
                out += StrA::format("{}", (unsigned long) mDependencyGraph[wfIdx][k]);
            }
            out += "\n";
        }

        // Tasks in this workflow: iterate actual tasks to get arguments, match ts by name for state.
        const StrA wfName = payload->name.empty() ? StrA("[unnamed workflow]") : payload->name;
        for (size_t taskIdx = 0; taskIdx < payload->tasks.size(); ++taskIdx) {
            const Workflow::Task & task  = payload->tasks[taskIdx];
            const StrA             tName = task.name.empty() ? StrA("[unnamed task]") : task.name;

            // Find matching TaskExecutionState
            const TaskExecutionState * ts = nullptr;
            for (const auto & t : mTaskStates) {
                if (t.workflowName == wfName && t.taskName == tName) {
                    ts = &t;
                    break;
                }
            }

            out += StrA::format("  Task [{}] \"{}\":\n", ts ? (unsigned long) ts->index : (unsigned long) taskIdx, tName.empty() ? "[unnamed]" : tName.c_str());
            if (ts) {
                out += StrA::format("      validation passed:  {}\n", ts->validationPassed ? "yes" : "no");
                out += StrA::format("      prepare:             {}\n", ts->prepareDone ? executionResultStr(ts->prepareResult) : "(skipped/not run)");
                out += StrA::format("      execute:             {}\n", ts->executeDone ? executionResultStr(ts->executeResult) : "(skipped/not run)");
                out += StrA::format("      has warning:         {}\n",
                                    (ts->prepareResult == Action::ExecutionResult::WARNING || ts->executeResult == Action::ExecutionResult::WARNING) ? "yes"
                                                                                                                                                     : "no");
                out += StrA::format("      finished:             {}\n", ts->executeDone ? "yes" : "no");
            }

            // Artifact arguments: arg name, usage; per-artifact: type (id & name), artifact name
            if (task.arguments) {
                const Arguments &                    args = *task.arguments;
                std::unordered_set<const Artifact *> r, w;
                Arguments::ArtifactReadWriteList     list {r, w};
                args.addToReadWriteList(list);
                // Print all resources read by this task
                out += "      read artifacts:\n";
                for (const Artifact * a : r) {
                    auto typeName = a->typeInfo().name.empty() ? std::string_view("[unknown type]") : a->typeInfo().name;
                    out += StrA::format("        [type:{} id:{}] name:\"{}\"\n", typeName, (unsigned long long) a->typeInfo().id, a->name.c_str());
                }
                // Print all resources written by this task
                out += "      write artifacts:\n";
                for (const Artifact * a : w) {
                    auto typeName = a->typeInfo().name.empty() ? std::string_view("[unknown type]") : a->typeInfo().name;
                    out += StrA::format("        [type:{} id:{}] name:\"{}\"\n", typeName, (unsigned long long) a->typeInfo().id, a->name.c_str());
                }
            }
        }
    }

    out += "\n==========================================\n";

    for (size_t i = 0; i < mDependencyGraph.size(); ++i) result.workflowDependencies[(uint64_t) i] = mDependencyGraph[i];
    return result;
}

Submission::Result SubmissionImpl::run(const RenderGraph::SubmitParameters &) {
    cleanup(false); // clean up any residual data from previous runs, but keep pending workflows.

    auto setResult = [](Action::ExecutionResult executionResult) -> Result {
        Result r;
        r.executionResult = executionResult;
        return r;
    };

    // shortcut for empty workflows.
    if (mWorkflows.empty()) {
        GN_VERBOSE(sLogger)("No workflows to execute, returning PASSED.");
        std::lock_guard<std::mutex> lock(mStateMutex);
        mTaskStates.clear();
        mExecutionOrder.clear();
        mRunResult = Action::ExecutionResult::PASSED;
        return setResult(Action::ExecutionResult::PASSED);
    }

    try {
        auto signalTheEndOfSubmission = AutoFinalizer([this]() { endOfSubmission.emit(*this); });

        // step 1: validate and build dependency graph.
        if (!validateAndBuildDependencyGraph()) {
            cleanup();
            std::lock_guard<std::mutex> lock(mStateMutex);
            mTaskStates.clear();
            mExecutionOrder.clear();
            mRunResult = Action::ExecutionResult::FAILED;
            return setResult(Action::ExecutionResult::FAILED);
        }

        // step 2: topological sort.
        DynaArray<size_t> executionOrder = topologicalSort();
        if (executionOrder.empty()) {
            GN_ERROR(sLogger)("Topological sort failed - circular dependency detected");
            cleanup();
            std::lock_guard<std::mutex> lock(mStateMutex);
            mTaskStates.clear();
            mExecutionOrder.clear();
            mRunResult = Action::ExecutionResult::FAILED;
            return setResult(Action::ExecutionResult::FAILED);
        }
        GN_ASSERT(executionOrder.size() == mValidatedWorkflows.size());

        {
            std::lock_guard<std::mutex> lock(mStateMutex);
            mExecutionOrder = executionOrder;
        }

        // step 3: prepare all tasks in topological order.
        struct PendingTask {
            const Workflow::Task * task;
            TaskInfo               info;
            size_t                 remainingSteps;
        };
        std::list<PendingTask> pendingTasks;
        bool                   hasWarning = false;
        for (size_t executionOrderIdx = 0; executionOrderIdx < executionOrder.size(); ++executionOrderIdx) {
            size_t workflowIdx = executionOrder[executionOrderIdx];
            GN_ASSERT(workflowIdx < mValidatedWorkflows.size());
            WorkflowImplPayload * payload = mValidatedWorkflows[workflowIdx];
            GN_ASSERT(payload);
            for (size_t taskIdx = 0; taskIdx < payload->tasks.size(); ++taskIdx) {
                const Workflow::Task & task = payload->tasks[taskIdx];
                GN_ASSERT(task.action && task.arguments); // have been validated in validateTask().
                StrA     wfName = payload->name.empty() ? StrA("[unnamed workflow]") : payload->name;
                StrA     tName  = task.name.empty() ? StrA("[unnamed task]") : task.name;
                uint64_t idx    = (uint64_t) pendingTasks.size();
                pendingTasks.insert(
                    pendingTasks.end(),
                    PendingTask {.task           = &task,
                                 .info           = TaskInfo {.submission = *this, .workflow = wfName, .task = tName, .index = idx, .action = *task.action},
                                 .remainingSteps = 0});
                {
                    std::lock_guard<std::mutex> lock(mStateMutex);
                    mTaskStates.append(TaskExecutionState {.workflowName = wfName, .taskName = tName, .index = idx, .validationPassed = true});
                }
                auto & pt = pendingTasks.back();
                GN_VERBOSE(sLogger)("Preparing {}", pt.info);
                auto prepareResult = task.action->prepare(pt.info, *task.arguments);
                {
                    std::lock_guard<std::mutex> lock(mStateMutex);
                    if (pt.info.index < mTaskStates.size()) {
                        mTaskStates[pt.info.index].prepareDone   = true;
                        mTaskStates[pt.info.index].prepareResult = prepareResult.result;
                    }
                }
                if (prepareResult.result == Action::ExecutionResult::FAILED) {
                    GN_ERROR(sLogger)("{}: preparation failed", pt.info);
                    std::lock_guard<std::mutex> lock2(mStateMutex);
                    mRunResult = Action::ExecutionResult::FAILED;
                    return setResult(Action::ExecutionResult::FAILED);
                }
                if (prepareResult.result == Action::ExecutionResult::WARNING) {
                    GN_VERBOSE(sLogger)("{}: preparation completed with warnings", pt.info);
                    hasWarning = true;
                }
                pt.remainingSteps = prepareResult.remainingSteps;
                if (0 == pt.remainingSteps) {
                    // the task is done already. remove it from the pending list.
                    pendingTasks.pop_back();
                }
            }
        }

        // // Emit prepare signal and check for errors.
        // auto signalResults = allTasksPrepared.emit(*this);
        // for (auto r : signalResults.results) {
        //     if (r == Action::ExecutionResult::FAILED) {
        //         std::lock_guard<std::mutex> lock(mStateMutex);
        //         mRunResult = Action::ExecutionResult::FAILED;
        //         return setResult(Action::ExecutionResult::FAILED);
        //     }
        //     if (r == Action::ExecutionResult::WARNING) { hasWarning = true; }
        // }

        // step 4: execute workflows sequentially in topological order.
        // for (size_t i = 0; i < pendingTasks.size(); ++i) {
        size_t currentStep = 0;
        while (!pendingTasks.empty()) {
            for (auto iter = pendingTasks.begin(); iter != pendingTasks.end();) {
                auto & pt = *iter;
                GN_VERBOSE(sLogger)("Executing {}, step = {}", pt.info, currentStep);
                auto result = pt.task->action->execute(pt.info, currentStep, *pt.task->arguments);
                {
                    std::lock_guard<std::mutex> lock(mStateMutex);
                    if (pt.info.index < mTaskStates.size()) {
                        mTaskStates[pt.info.index].executeDone   = true;
                        mTaskStates[pt.info.index].executeResult = result;
                    }
                }
                if (result == Action::ExecutionResult::FAILED) {
                    GN_ERROR(sLogger)("{}: execution failed", pt.info);
                    std::lock_guard<std::mutex> lock(mStateMutex);
                    mRunResult = Action::ExecutionResult::FAILED;
                    return setResult(Action::ExecutionResult::FAILED);
                }
                if (result == Action::ExecutionResult::WARNING) {
                    GN_VERBOSE(sLogger)("{}: execution completed with warnings", pt.info);
                    hasWarning = true;
                }
                GN_ASSERT(pt.remainingSteps > 0);
                --pt.remainingSteps;
                if (0 == pt.remainingSteps) {
                    // the task is done already. remove it from the pending list.
                    iter = pendingTasks.erase(iter);
                } else {
                    ++iter;
                }
            }
            ++currentStep;
        }

        // // Emit execute signal and check for errors.
        // signalResults = allTasksExecuted.emit(*this);
        // for (auto r : signalResults.results) {
        //     if (r == Action::ExecutionResult::FAILED) {
        //         std::lock_guard<std::mutex> lock(mStateMutex);
        //         mRunResult = Action::ExecutionResult::FAILED;
        //         return setResult(Action::ExecutionResult::FAILED);
        //     }
        //     if (r == Action::ExecutionResult::WARNING) { hasWarning = true; }
        // }

        // // Signal EoS before returning the result to user thread.
        // signalTheEndOfSubmission.proceed();

        // Done
        Action::ExecutionResult finalResult = hasWarning ? Action::ExecutionResult::WARNING : Action::ExecutionResult::PASSED;
        {
            std::lock_guard<std::mutex> lock(mStateMutex);
            mRunResult = finalResult;
        }
        return setResult(finalResult);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger)("Exception occurred during submission: {}", e.what());
        cleanup();
        std::lock_guard<std::mutex> lock(mStateMutex);
        mTaskStates.clear();
        mExecutionOrder.clear();
        mRunResult = Action::ExecutionResult::FAILED;
        return setResult(Action::ExecutionResult::FAILED);
    } catch (...) {
        GN_ERROR(sLogger)("Exception occurred during submission.");
        cleanup();
        std::lock_guard<std::mutex> lock(mStateMutex);
        mTaskStates.clear();
        mExecutionOrder.clear();
        mRunResult = Action::ExecutionResult::FAILED;
        return setResult(Action::ExecutionResult::FAILED);
    }
}

} // namespace GN::rdg

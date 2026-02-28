#include "pch.h"
#include "submission.h"
#include <limits>
#include <chrono>
#include <unordered_set>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

SubmissionImpl::SubmissionImpl(DynaArray<WorkflowImpl *> pendingWorkflows, const RenderGraph::SubmitParameters & params) {
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
        auto workflow = mWorkflows[workflowIdx];
        GN_ASSERT(workflow);

        for (size_t taskIdx = 0; taskIdx < workflow->tasks.size(); ++taskIdx) {
            const Workflow::Task & task = workflow->tasks[taskIdx];
            if (!validateTask(task, workflow->name, taskIdx)) return false;
        }

        mValidatedWorkflows.append(workflow);
    }

    GN_VERBOSE(sLogger)("Validated {} workflows.", mValidatedWorkflows.size());

    mDependencyGraph.resize(mValidatedWorkflows.size());

    // Collect per-workflow: artifacts read and written (from task arguments via firstArtifactArgument/next).
    using ArtifactSet = std::unordered_set<const Artifact *>;
    DynaArray<ArtifactSet> workflowReads(mValidatedWorkflows.size());
    DynaArray<ArtifactSet> workflowWrites(mValidatedWorkflows.size());
    for (size_t i = 0; i < mValidatedWorkflows.size(); ++i) {
        auto w = mValidatedWorkflows[i];
        for (const Workflow::Task & task : w->tasks) {
            Arguments * args = task.arguments.get();
            if (!args) continue;
            for (const Arguments::ArtifactArgument * p = args->firstArtifactArgument(); p; p = p->next()) {
                const auto usage = p->usage();
                const bool read  = usage.reading;
                const bool write = usage.writing;
                const auto arts  = p->artifacts();
                for (size_t k = 0; k < arts.size(); ++k) {
                    const Artifact * a = arts[k];
                    if (!a) continue;
                    if (read) workflowReads[i].insert(a);
                    if (write) workflowWrites[i].insert(a);
                }
            }
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

static StrA usageFlagStr(Arguments::UsageBits u) {
    StrA s;
    if (u.optional) s += "Optional|";
    if (u.reading) s += "Reading|";
    if (u.writing) s += "Writing|";
    if (s.empty())
        s = "None";
    else
        s.popback(); // trailing |
    return s;
}

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
        auto w = mValidatedWorkflows[wfIdx];
        if (!w) continue;

        out += StrA::format("\n--- Workflow [{}] \"{}\" (sequence={}, order={}) ---\n", wfIdx, w->name.empty() ? "[unnamed]" : w->name.c_str(),
                            (long long) w->sequence, (unsigned long) orderIdx);

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
        const StrA wfName = w->name.empty() ? StrA("[unnamed workflow]") : w->name;
        for (size_t taskIdx = 0; taskIdx < w->tasks.size(); ++taskIdx) {
            const Workflow::Task & task  = w->tasks[taskIdx];
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

            // Artifact arguments: arg name, usage; per-artifact: type (id & name), artifact name, sequence
            if (task.arguments) {
                const Arguments * args = task.arguments.get();
                out += "      artifact arguments:\n";
                for (const Arguments::ArtifactArgument * p = args->firstArtifactArgument(); p; p = p->next()) {
                    const char * argName  = p->name() ? p->name() : "[unnamed]";
                    StrA         usageStr = usageFlagStr(p->usage());
                    const auto   arts     = p->artifacts();
                    if (arts.size() == 0) {
                        out += StrA::format("        - \"{}\"  usage={}  (no artifact bound)\n", argName, usageStr.c_str());
                    } else {
                        out += StrA::format("        - \"{}\"  usage={}\n", argName, usageStr.c_str());
                        for (size_t k = 0; k < arts.size(); ++k) {
                            const Artifact * a = arts[k];
                            if (!a) continue;
                            const char * typeName = a->typeName ? a->typeName : "(unknown)";
                            out += StrA::format("            artifact: type=0x{:x} ({}), name=\"{}\", sequence={}\n", (unsigned long) a->typeId, typeName,
                                                a->name.c_str(), (unsigned long) a->sequence);
                        }
                    }
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
                StrA     wfName = workflow->name.empty() ? StrA("[unnamed workflow]") : workflow->name;
                StrA     tName  = task.name.empty() ? StrA("[unnamed task]") : task.name;
                uint64_t idx    = (uint64_t) pendingTasks.size();
                pendingTasks.append(
                    PendingTask {.task = &task, .info = TaskInfo {.submission = *this, .workflow = wfName, .task = tName, .index = idx}, .context = {}});
                {
                    std::lock_guard<std::mutex> lock(mStateMutex);
                    mTaskStates.append(TaskExecutionState {.workflowName = wfName, .taskName = tName, .index = idx, .validationPassed = true});
                }
                auto & pt = pendingTasks.back();
                GN_VERBOSE(sLogger)("Preparing workflow '{}' task '{}'", pt.info.workflow, pt.info.task);
                auto [result, context] = task.action->prepare(pt.info, *task.arguments);
                pt.context = std::unique_ptr<Action::ExecutionContext>(context); // need to do this before error check to ensure it is released even on error.
                {
                    std::lock_guard<std::mutex> lock(mStateMutex);
                    if (pt.info.index < mTaskStates.size()) {
                        mTaskStates[pt.info.index].prepareDone   = true;
                        mTaskStates[pt.info.index].prepareResult = result;
                    }
                }
                if (result == Action::ExecutionResult::FAILED) {
                    GN_ERROR(sLogger)("Workflow '{}' task '{}' preparation failed", pt.info.workflow, pt.info.task);
                    std::lock_guard<std::mutex> lock2(mStateMutex);
                    mRunResult = Action::ExecutionResult::FAILED;
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
            if (r == Action::ExecutionResult::FAILED) {
                std::lock_guard<std::mutex> lock(mStateMutex);
                mRunResult = Action::ExecutionResult::FAILED;
                return setResult(Action::ExecutionResult::FAILED);
            }
            if (r == Action::ExecutionResult::WARNING) { hasWarning = true; }
        }

        // step 4: execute workflows sequentially in topological order.
        for (size_t i = 0; i < pendingTasks.size(); ++i) {
            auto & pt = pendingTasks[i];
            GN_VERBOSE(sLogger)("Executing workflow '{}' task '{}'", pt.info.workflow, pt.info.task);
            auto result = pt.task->action->execute(pt.info, *pt.task->arguments, pt.context.get());
            {
                std::lock_guard<std::mutex> lock(mStateMutex);
                if (pt.info.index < mTaskStates.size()) {
                    mTaskStates[pt.info.index].executeDone   = true;
                    mTaskStates[pt.info.index].executeResult = result;
                }
            }
            if (result == Action::ExecutionResult::FAILED) {
                GN_ERROR(sLogger)("Workflow '{}' task '{}' execution failed", pt.info.workflow, pt.info.task);
                std::lock_guard<std::mutex> lock(mStateMutex);
                mRunResult = Action::ExecutionResult::FAILED;
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
            if (r == Action::ExecutionResult::FAILED) {
                std::lock_guard<std::mutex> lock(mStateMutex);
                mRunResult = Action::ExecutionResult::FAILED;
                return setResult(Action::ExecutionResult::FAILED);
            }
            if (r == Action::ExecutionResult::WARNING) { hasWarning = true; }
        }

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

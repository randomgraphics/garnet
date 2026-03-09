#pragma once

#include <garnet/GNrdg.h>
#include <future>
#include <mutex>
#include <optional>
#include "runtime-type.h"

#define GN_RDG_FAIL_ON_FAIL(expr, ...)                                                                            \
    do {                                                                                                          \
        auto result___ = (expr);                                                                                  \
        if (result___ != Action::PASSED && result___ != Action::WARNING) GN_UNLIKELY {                            \
                if constexpr (GN_COUNT_ARGS(__VA_ARGS__) > 0) { GN_ERROR(GN::getLogger("GN.rdg"))(__VA_ARGS__); } \
                return result___;                                                                                 \
            }                                                                                                     \
    } while (false)

#define GN_RDG_FAIL_ON_FALSE(expr, ...)                                                                           \
    do {                                                                                                          \
        if (!(expr)) GN_UNLIKELY {                                                                                \
                if constexpr (GN_COUNT_ARGS(__VA_ARGS__) > 0) { GN_ERROR(GN::getLogger("GN.rdg"))(__VA_ARGS__); } \
                return Action::FAILED;                                                                            \
            }                                                                                                     \
    } while (false)

namespace GN::rdg {

class SubmissionImpl;

struct TaskInfo {
    // /// A action might be used in multiple tasks. So we need a context structure to store data associated to a particular task.
    // struct ExecutionContext : public RuntimeType {
    //     virtual ~ExecutionContext() = default;

    // protected:
    //     ExecutionContext(uint64_t typeId, const char * typeName): RuntimeType(typeId, typeName) {}
    // };

    SubmissionImpl & submission; ///< the submission that the task belongs to.
    const StrA       workflow;   ///< name of the workflow that the task belongs to.
    const StrA       task;       ///< name of the task.
    const uint64_t   index;  ///< index of the task within the entire submission. Can also be used as the unique identifier of the task within the submission.
    Action &         action; ///< The action processing this task.
    // AutoRef<ExecutionContext> context {}; ///< context for the task. Usually created by the action's prepare() method and referenced by the execute() method.
};

struct WorkflowImpl : public Workflow {
    mutable uint64_t sequence = 0;

    explicit WorkflowImpl(const StrA & name_) { name = name_; }

    static WorkflowImpl * promote(Workflow * workflow) { return static_cast<WorkflowImpl *>(workflow); }
};

/// Implementation of Submission. Holds all intermediate data and context for a single submit.
/// Processes workflows asynchronously: validate, build dependency graph, topological sort, execute.
class SubmissionImpl : public Submission {
public:
    struct Context : RefCounter, RuntimeType {
    public:
        virtual ~Context() = default;

    protected:
        using RuntimeType::RuntimeType;
    };

    /// Construct and start the submission asynchronously. Takes ownership of \p pendingWorkflows (pointers).
    SubmissionImpl(DynaArray<WorkflowImpl *> pendingWorkflows, const RenderGraph::SubmitParameters & params);

    ~SubmissionImpl() override;

    bool isFinished() override;

    Result result() override;

    Submission::State dumpState() const override;

    template<typename T>
    AutoRef<T> getSumissionContext() const {
        auto ctx = mExecutionContexts.find(T::TYPE_ID);
        if (ctx == mExecutionContexts.end()) { return {}; }
        GN_ASSERT(ctx->second->typeId == T::TYPE_ID);
        return AutoRef<T>(ctx->second->template castTo<T>());
    }

    void setSubmissionContext(AutoRef<Context> ctx) {
        if (!ctx) GN_UNLIKELY {
                GN_ERROR(GN::getLogger("GN.rdg"))("SubmissionImpl::setExecutionContext: context is null");
                return;
            }
        mExecutionContexts[ctx->typeId] = ctx;
    }

    template<typename T, typename... Args>
    T & ensureSubmissionContext(Args &&... args) {
        auto ctx = mExecutionContexts.find(T::TYPE_ID);
        if (ctx != mExecutionContexts.end()) { return *ctx->second->template castTo<T>(); }
        auto newCtx                    = AutoRef<T>(new T(*this, std::forward<Args>(args)...));
        mExecutionContexts[T::TYPE_ID] = newCtx;
        return *newCtx;
    }

    // Signaled after all pending tasks are successfully prepared.
    Signal<Action::ExecutionResult(SubmissionImpl &)> allTasksPrepared;

    // Signaled after all pending tasks are successfully executed.
    Signal<Action::ExecutionResult(SubmissionImpl &)> allTasksExecuted;

private:
    /// Per-task execution state for dumpState().
    struct TaskExecutionState {
        StrA                    workflowName;
        StrA                    taskName;
        uint64_t                index {};
        bool                    validationPassed {};
        bool                    prepareDone {};
        bool                    executeDone {};
        Action::ExecutionResult prepareResult {Action::ExecutionResult::PASSED};
        Action::ExecutionResult executeResult {Action::ExecutionResult::PASSED};
    };

    /// Deletes all work items and clears intermediate data (workflows, dependency graph). Safe to call multiple times.
    void cleanup(bool cleanupPendingWorkflows = true) noexcept;

    std::future<Result> mFuture;
    Result              mResult;
    mutable std::mutex  mResultMutex;

    std::unordered_map<uint64_t, AutoRef<Context>> mExecutionContexts;

    // Owned workflows (taken from graph on construction)
    DynaArray<WorkflowImpl *>    mWorkflows;
    DynaArray<WorkflowImpl *>    mValidatedWorkflows;
    DynaArray<DynaArray<size_t>> mDependencyGraph;

    // State for dumpState() (written by run(), read by dumpState())
    mutable std::mutex                     mStateMutex;
    DynaArray<TaskExecutionState>          mTaskStates;
    DynaArray<size_t>                      mExecutionOrder;
    std::optional<Action::ExecutionResult> mRunResult;

    bool              validateTask(const Workflow::Task & task, const StrA & workflowName, size_t taskIndex);
    bool              validateAndBuildDependencyGraph();
    DynaArray<size_t> topologicalSort();
    Result            run(const RenderGraph::SubmitParameters & params);
};

} // namespace GN::rdg

template<>
struct fmt::formatter<GN::rdg::TaskInfo> {
    constexpr auto parse(format_parse_context & ctx) { return ctx.begin(); }
    template<typename Context>
    constexpr auto format(GN::rdg::TaskInfo const & taskInfo, Context & ctx) const {
        return format_to(ctx.out(), "TaskInfo(submission={}, workflow={}, task[{}]={})", taskInfo.submission.name, taskInfo.workflow, taskInfo.index,
                         taskInfo.task);
    }
};

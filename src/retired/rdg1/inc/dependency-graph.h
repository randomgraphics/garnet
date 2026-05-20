#pragma once

#include <garnet/GNbase.h>

#include <concepts>
#include <unordered_map>
#include <unordered_set>

namespace GN::rdg {

// Render graph: workflows are created (possibly from multiple threads), then executed in topological order.
//
// - Action: contains the code/logic for an operation. Declares a set of predefined parameters (input and/or output).
// - Task: one Action + one arguments value (an action-defined struct, type-erased as std::any) to run that action.
// - Workflow: a sequence of tasks run in strict sequential order. A workflow can depend on completion of other workflows.
// - RenderGraph: create workflows (thread-safe); submit() submits selected list of workflows for async execution in a topological order that satisfies
// dependencies.

/// The basic building block of the render graph module. Base class of everything that
/// needs reference counting and runtime type information.
struct Artifact : public RefCounter, public RuntimeType {
    GN_API GN_REGISTER_RUNTIME_TYPE();

    const StrA name;

    virtual ~Artifact();

protected:
    /// Constructor
    Artifact(const TypeInfo & type, const StrA & name);
};

inline Artifact::Artifact(const TypeInfo & type, const StrA & name): RuntimeType(type), name(name) {}

inline Artifact::~Artifact() = default;

/// A helper class to wrap anything as an artifact.
/// \param T    Type of the value to wrap.
template<typename T>
class TypedArtifact : public Artifact {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(Artifact);

    T value;

    /// Create a new artifact with default value.
    AutoRef<TypedArtifact> create(const StrA & name) { return AutoRef<TypedArtifact>(new TypedArtifact(name)); }

    /// Create a new artifact with a specific value (copy).
    AutoRef<TypedArtifact> create(const StrA & name, const T & value) {
        auto p   = AutoRef<TypedArtifact>(new TypedArtifact(name, value));
        p->value = value;
        return p;
    }

    /// Create a new artifact with a specific value (move).
    AutoRef<TypedArtifact> create(const StrA & name, T && value) { return AutoRef<TypedArtifact>(new TypedArtifact(name, std::move(value))); }

private:
    // Constructor: only used by create() methods.
    TypedArtifact(const StrA & name): Artifact(TYPE_INFO(), name), value() {}
    TypedArtifact(const StrA & name, const T & v): Artifact(TYPE_INFO(), name), value(v) {}
    TypedArtifact(const StrA & name, T && v): Artifact(TYPE_INFO(), name), value(std::move(v)) {}
};

template<class T>
concept DerivedFromArtifact = std::derived_from<T, Artifact>;

/// Base class of arguments for an action.
struct Arguments : public Artifact {
    GN_API GN_REGISTER_RUNTIME_TYPE(Artifact);

    struct ArtifactReadWriteList {
        std::unordered_set<const Artifact *> & readList;
        std::unordered_set<const Artifact *> & writeList;
    };

    /// Called by Submission class to collect artifact usage information.
    /// \param list The list of artifact IDs that are read by the action.
    /// The subclass is supposed to add/append data to the lists. Do not clear or modify the existing content of the lists.
    virtual void addToReadWriteList(ArtifactReadWriteList & list) const = 0;

protected:
    /// For argument structs that have no logical name; subclasses use Arguments(TYPE_INFO()).
    explicit Arguments(const TypeInfo & type): Artifact(type, "") {}
};

/// A opaque struct to store information of each task in a submission. Defined in submission.h.
struct TaskInfo;

/// Base class of all actions. An action holds the logic for an operation and declares its parameters (input/output).
struct Action : public Artifact {
    GN_API GN_REGISTER_RUNTIME_TYPE(Artifact);

    enum ExecutionResult {
        PASSED,  ///< the action executed successfully.
        WARNING, ///< the action executed successfully, but with warnings.
        FAILED,  ///< the action failed; dependents may be skipped.
        DROPPED, ///< the action is dropped; dependents are skipped.
    };

    struct PrepareResult {
        ExecutionResult result         = FAILED;
        size_t          remainingSteps = 0;

        PrepareResult() = default;
        PrepareResult(ExecutionResult r, size_t steps = 0): result(r), remainingSteps(steps) {}
        operator bool() const { return result == PASSED || result == WARNING; }
    };

    /// Prepare for execution. Returns success code and number of additional steps required to complete the action.
    /// \param submission The submission that is executing the action.
    /// \param taskInfo The information of the task that is executing the action.
    /// \param arguments The arguments for the action.
    /// \return The prepare result and how many additional steps (excluding this prepare step) are required to complete the action.
    virtual PrepareResult prepare(TaskInfo & taskInfo, Arguments & arguments) = 0;

    /// Execute the action with the given arguments. The context is the same as the one returned from prepare().
    /// \param submission The submission that is executing the action.
    /// \param taskInfo Same as the one passed to prepare().
    /// \param arguments Same as the one passed to prepare().
    /// \param context The context returned from prepare().
    /// \return The execution result.
    virtual ExecutionResult execute(TaskInfo & taskInfo, size_t step, Arguments & arguments) = 0;

protected:
    /// Inherit constructor from Artifact
    using Artifact::Artifact;
};

/// A workflow is a sequence of tasks run in sequential order. It can depend on completion of other workflows.
/// The render graph runs workflows in a topological order that satisfies these dependencies.
/// Workflow is move-only (like unique_ptr); it holds a pointer to an internal payload.
struct Workflow {
    GN_NO_COPY(Workflow);

    /// Represents a single task in the workflow. This is the atomic execution unit of the render graph.
    /// \note
    /// - Task A is considered newer than task B, if any of the following is true:
    ///   - A and B belong to same workflow. A's index is greater than B's index in the tasks array.
    ///   - A and B belong to different workflows. A's workflow is newer than B's.
    /// - Task A is considered to depend on task B, only if A is newer than B and any of the following is true:
    ///   - A is reading or writing to an artifact that B is writing to.
    ///   - A is writing to an artifact that B is reading from.
    struct Task {
        StrA               name; //< name for logging and debugging (not required, but recommended. No need to be unique).
        AutoRef<Action>    action;
        AutoRef<Arguments> arguments;

        explicit Task(const StrA & name_): name(name_) {}

        Task(const StrA & name_, AutoRef<Action> action_, AutoRef<Arguments> arguments_)
            : name(name_), action(std::move(action_)), arguments(std::move(arguments_)) {}

        Task & setName(const StrA & name_) {
            this->name = name_;
            return *this;
        }

        Task & setAction(AutoRef<Action> action_) {
            this->action = std::move(action_);
            return *this;
        }

        Task & setArguments(AutoRef<Arguments> arguments_) {
            this->arguments = std::move(arguments_);
            return *this;
        }
    };

    /// Payload holds all workflow data. The workflow object holds a pointer to the payload.
    struct Payload {
        StrA            name;
        DynaArray<Task> tasks;

        virtual ~Payload() = default;

        bool validateAndAppend(Task && task) {
            const size_t taskIndex = tasks.size();
            auto *       log       = GN::getLogger("GN.rdg");
            if (!task.action) GN_UNLIKELY {
                    GN_ERROR(log)("Workflow '{}' task [{}] '{}': action is null", name, taskIndex, task.name);
                    return false;
                }
            if (!task.arguments) GN_UNLIKELY {
                    GN_ERROR(log)("Workflow '{}' task [{}] '{}': arguments is null", name, taskIndex, task.name);
                    return false;
                }
            // if (!task.action->validate(*task.arguments)) GN_UNLIKELY {
            //         GN_ERROR(log)("Workflow '{}' task [{}] '{}': argument validation failed", name, taskIndex, task.name);
            //         return false;
            //     }
            tasks.append(std::move(task));
            return true;
        }
    };

    constexpr Workflow() = default;

    Workflow(Workflow && other) noexcept: mPayload(other.mPayload) { other.mPayload = nullptr; }

    Workflow & operator=(Workflow && other) noexcept {
        if (this == &other) return *this;
        drop();
        mPayload       = other.mPayload;
        other.mPayload = nullptr;
        return *this;
    }

    ~Workflow() { drop(); }

    bool valid() const { return mPayload != nullptr; }

    explicit operator bool() const { return valid(); }

    /// Name for logging and debugging (not required, but recommended. No need to be unique).
    StrA name() const { return mPayload ? mPayload->name : StrA(); }

    /// Read-only access to the task list. Use appendTask() to add tasks (validates at append time).
    SafeArrayAccessor<const Task> tasks() const {
        if (!mPayload) return SafeArrayAccessor<const Task>();
        return SafeArrayAccessor<const Task>(mPayload->tasks);
    }

    /// Append a task. Validates action non-null, arguments non-null, and action->validate(arguments) at append time; on failure logs and does not append.
    Workflow & appendTask(Task && task) {
        if (!mPayload) GN_UNLIKELY {
                static auto * log = GN::getLogger("GN.rdg");
                GN_ERROR(log)("Task '{}': Can't apend task to workflow of null payload.", task.name);
                return *this;
            }
        mPayload->validateAndAppend(std::move(task));
        return *this;
    }

    Workflow & appendTask(const StrA & name_, AutoRef<Action> action_, AutoRef<Arguments> arguments_) {
        return appendTask(Task(name_, std::move(action_), std::move(arguments_)));
    }

    /// Drop and release a single workflow's payload. The workflow becomes empty.
    void drop() {
        safeDelete(mPayload);
        GN_ASSERT(!mPayload);
    }

private:
    Payload * mPayload = nullptr;
    friend class RenderGraphImpl; // will modify mPayload directly.
};

struct Submission : RefCounter {
    const StrA name;

    /// Result of execution
    struct Result {
        Action::ExecutionResult executionResult {};
        // for future extension, like individual task result, etc.
    };

    struct State {
        StrA                                            state;
        std::unordered_map<uint64_t, DynaArray<size_t>> workflowDependencies;
    };

    virtual ~Submission() = default;

    /// Check if submitted workflows are all finished.
    /// \todo: is this finished on CPU or finished on GPU? Maybe add a new method isProcessed() for CPU-only check.
    virtual bool isFinished() = 0;

    /// Get execution result of the submitted workflows. Will block calling thread until all workflows are finished.
    virtual Result result() = 0;

    /// Dump detailed state and status of the submission to a human-readable string.
    /// Includes per-task name, execution status, dependencies, validation, warnings, skipped, and finished state.
    virtual State dumpState() const = 0;

protected:
    Submission(const StrA & name_): name(name_) {}
};

/// Render graph: create workflows (thread-safe), then submit them for async execution.
struct RenderGraph {
    struct CreateParameters {
        // For future use
    };

    struct SubmitParameters {
        /// The order of the workflow is important. A workflow in front of the array (smaller index)
        /// is considered older than the ones in the back of the array (larger index).
        /// Also, a workflow is always newer than any previously submitted workflows.
        /// On submit, ownership of each workflow's payload is transferred to the submission.
        SafeArrayAccessor<Workflow> workflows;

        /// name of the submission. For logging and debugging.
        StrA name = "[unnamed submission]"_s;
    };

    /// Create a new render graph instance
    static GN_API RenderGraph * create(const CreateParameters & params);

    /// @brief Destroy the render graph instance. This method will try its best to cancel all pending tasks and workflows.
    /// Once the method returns, the render graph is no longer usable, all pointers returned from createWorkflow() are invalidated,
    /// all workflows are either finished or cancelled. The detailed result can be queried via the submission object returned by submit().
    virtual ~RenderGraph() = default;

    /// Create a new workflow.
    /// \param name The name of the workflow.
    /// \return a workflow by value (move-only). Empty workflow on allocation failure.
    virtual Workflow createWorkflow(StrA name) = 0;

    /// Submit workflows for <b>blocking</b> async execution in a topological order that satisfies workflow dependencies.
    virtual AutoRef<Submission> submit(const SubmitParameters & params) = 0;

protected:
    RenderGraph() = default;
};

} // namespace GN::rdg

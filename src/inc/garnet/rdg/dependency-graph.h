#pragma once

#include <garnet/GNbase.h>
#include <garnet/rdg/rtti.h>

#include <concepts>
#include <functional>
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

struct ArtifactDatabase;

/// The basic building block of the render graph module. Base class of everything that could be added to a render graph.
struct Artifact : public RefCounter, public RuntimeType {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE();

    ArtifactDatabase & database;
    const StrA         name;
    const uint64_t     sequence; ///< the unique integer identifier of the artifact in the artifact database.

    virtual ~Artifact();

protected:
    /// Constructor
    Artifact(ArtifactDatabase & db, const TypeInfo & type, const StrA & name);
};

/// Database of all artifacts. Artifact is uniquely identified by its type and name, or by its sequence number.
/// The database is only holding weak references to all artifacts, so it is not responsible for the lifetime of the artifacts.
struct ArtifactDatabase {
    struct CreateParameters {
        // TBD
    };

    /// Create a new artifact database instance
    static GN_API ArtifactDatabase * create(const CreateParameters & params);

    virtual ~ArtifactDatabase() = default;

    /// Acquire a new unique sequence number for the resource.
    /// \return the non-zero sequence number if successful, 0 if the type and name combination exists already and/or artifact pointer is null.
    /// \note this method is called by Artifact::constructor only. Artifact create()/load() must check artifact.sequence after construction;
    ///       if 0 (duplicate type+name), delete the new instance and return null.
    virtual uint64_t admit(Artifact * artifact) = 0;

    /// Erase an artifact instance by its sequence number. Usually called by destructor of the artifact.
    virtual bool erase(uint64_t sequence) = 0;

    /// Search for an artifact instance by type and name.
    virtual auto fetch(uint64_t type, const StrA & name) -> AutoRef<Artifact> = 0;

    /// Search for an artifact instance by its sequence number. Faster than search by ID.
    virtual auto fetch(uint64_t sequence) -> AutoRef<Artifact> = 0;

protected:
    ArtifactDatabase() = default;
};

inline Artifact::Artifact(ArtifactDatabase & db, const TypeInfo & type, const StrA & name)
    : RuntimeType(type), database(db), name(name), sequence(database.admit(this)) {}

inline Artifact::~Artifact() {
    if (sequence) database.erase(sequence);
}

/// A helper class to wrap anything as an artifact.
/// \param T    Type of the value to wrap.
template<typename T>
class TypedArtifact : public Artifact {
public:
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Artifact);

    T value;

    /// Create a new artifact with default value.
    AutoRef<TypedArtifact> create(ArtifactDatabase & db, const StrA & name) {
        auto p = AutoRef<TypedArtifact>(new TypedArtifact(db, name));
        if (0 == p->sequence) GN_UNLIKELY return {}; // most likely a duplicate type+name
        return p;
    }

    /// Create a new artifact with a specific value (copy).
    AutoRef<TypedArtifact> create(ArtifactDatabase & db, const StrA & name, const T & value) {
        auto p = AutoRef<TypedArtifact>(new TypedArtifact(db, name, value));
        if (0 == p->sequence) GN_UNLIKELY return {}; // most likely a duplicate type+name
        p->value = value;
        return p;
    }

    /// Create a new artifact with a specific value (move).
    AutoRef<TypedArtifact> create(ArtifactDatabase & db, const StrA & name, T && value) {
        auto p = AutoRef<TypedArtifact>(new TypedArtifact(db, name, std::move(value)));
        if (0 == p->sequence) GN_UNLIKELY return {}; // most likely a duplicate type+name
        return p;
    }

private:
    // Constructor: only used by create() methods.
    TypedArtifact(ArtifactDatabase & db, const StrA & name): Artifact(db, TYPE_INFO(), name), value() {}
    TypedArtifact(ArtifactDatabase & db, const StrA & name, const T & v): Artifact(db, TYPE_INFO(), name), value(v) {}
    TypedArtifact(ArtifactDatabase & db, const StrA & name, T && v): Artifact(db, TYPE_INFO(), name), value(std::move(v)) {}
};

template<class T>
concept DerivedFromArtifact = std::derived_from<T, Artifact>;

/// Base class of arguments for an action. This is not a subclass of Artifact, since it is means to be one time use: create, pass to action, and forget.
struct Arguments : public RefCounter, public RuntimeType {
    struct ArtifactReadWriteList {
        std::unordered_set<const Artifact *> & readList;
        std::unordered_set<const Artifact *> & writeList;
    };

    /// Called by Submission class to collect artifact usage information.
    /// \param list The list of artifact IDs that are read by the action.
    /// The subclass is supposed to add/append data to the lists. Do not clear or modify the existing content of the lists.
    virtual void addToReadWriteList(ArtifactReadWriteList & list) const = 0;

protected:
    using RuntimeType::RuntimeType;
};

/// A opaque struct to store information of each task in a submission. Defined in submission.h.
struct TaskInfo;

/// Base class of all actions. An action holds the logic for an operation and declares its parameters (input/output).
struct Action : public Artifact {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Artifact);

    enum ExecutionResult {
        PASSED,  ///< the action executed successfully.
        WARNING, ///< the action executed successfully, but with warnings.
        FAILED,  ///< the action failed; dependents may be skipped.
        DROPPED, ///< the action is dropped; dependents are skipped.
    };

    /// Prepare for execution. Returns success code and an optional execution context that will be later passed to execute().
    /// \param submission The submission that is executing the action.
    /// \param taskInfo The information of the task that is executing the action.
    /// \param arguments The arguments for the action.
    /// \return The execution result.
    virtual ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) = 0;

    /// Execute the action with the given arguments. The context is the same as the one returned from prepare().
    /// \param submission The submission that is executing the action.
    /// \param taskInfo Same as the one passed to prepare().
    /// \param arguments Same as the one passed to prepare().
    /// \param context The context returned from prepare().
    /// \return The execution result.
    virtual ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) = 0;

protected:
    /// Inherit constructor from Artifact
    using Artifact::Artifact;
};

/// A workflow is a sequence of tasks run in sequential order. It can depend on completion of other workflows.
/// The render graph runs workflows in a topological order that satisfies these dependencies.
struct Workflow {
    /// Name for logging and debugging (not required, but recommended. No need to be unique).
    StrA name;

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

    DynaArray<Task> tasks;

    Workflow & appendTask(Task && task) {
        tasks.append(std::move(task));
        return *this;
    }

    Workflow & appendTask(const StrA & name_, AutoRef<Action> action_, AutoRef<Arguments> arguments_) {
        tasks.append(Task(name_, std::move(action_), std::move(arguments_)));
        return *this;
    }

    // /// Collect usage of all artifacts
    // std::unordered_map<uint64_t, Arguments::UsageBits> collectArtifactArguments() const {
    //     std::unordered_map<uint64_t, Arguments::UsageBits> result;
    //     for (const Task & task : tasks) {
    //         if (!task.arguments) GN_LIKELY continue;
    //         for (const Arguments::ArtifactArgument * p = task.arguments->firstArtifactArgument(); p; p = p->next()) {
    //             for (const Artifact * a : p->artifacts()) {
    //                 if (a) GN_LIKELY result[a->typeId] += p->usage();
    //             }
    //         }
    //     }
    //     return result;
    // }
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

// /// A transient arena is a temporary memory pool that is used to allocate memory for the tasks that are executed.
// /// It will be automatically deleted, along with all allocated memory, after the next submission is completed or cancelled.
// /// Accessing the added arena after calling submit() is prohibited and will result in undefined behavior.
// struct TransientArena {
//     virtual ~TransientArena() = default;

//     GN_NO_COPY(TransientArena);
//     GN_NO_MOVE(TransientArena);

// protected:
//     TransientArena() = default;
// };

/// Render graph: create workflows (thread-safe), then submit them for async execution.
struct RenderGraph {
    struct CreateParameters {
        // For future use
    };

    struct SubmitParameters {
        /// The order of the workflow is important. A workflow in front of the array (smaller index)
        /// is considered older than the ones in the back of the array (larger index).
        /// Also, a workflow is always newer than any previously submitted workflows.
        SafeArrayAccessor<Workflow *> workflows;

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
    /// \return a pointer to the created workflow. The pointer is valid after passed to submit().
    ///         Modifying submitted workflow is undefined behavior.
    virtual Workflow * createWorkflow(StrA name) = 0;

    // /// Add a transient arena to the render graph. The arena is used to allocate temporary used only by the the next submission.
    // /// It will be automatically deleted, along with all allocated memory, after the next submission is completed or cancelled.
    // /// Accessing the added arena after calling submit() is prohibited and will result in undefined behavior.
    // /// \param arena The transient arena to add.
    // virtual void addTransientArena(TransientArena * arena) = 0;

    /// Submit workflows for <b>blocking</b> async execution in a topological order that satisfies workflow dependencies.
    virtual AutoRef<Submission> submit(const SubmitParameters & params) = 0;

    /// Helper function to properly drop a workflow.
    /// \note You can't drop an already submitted workflow. That'll cause undefined behavior.
    void dropWorkflow(Workflow * workflow) {
        if (!workflow) return;
        workflow->tasks.clear();
        submit({.workflows = {&workflow, 1}, .name = StrA::format("Dropped workflow {}", workflow->name)});
    }

protected:
    RenderGraph() = default;
};

} // namespace GN::rdg

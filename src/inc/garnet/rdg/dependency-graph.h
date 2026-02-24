#pragma once

#include "garnet/base/array.h"
#include <garnet/GNbase.h>

#include <concepts>
#include <functional>
#include <optional>
#include <unordered_map>

namespace GN::rdg {

// Render graph: workflows are scheduled (possibly from multiple threads), then executed in topological order.
//
// - Action: contains the code/logic for an operation. Declares a set of predefined parameters (input and/or output).
// - Task: one Action + one arguments value (an action-defined struct, type-erased as std::any) to run that action.
// - Workflow: a sequence of tasks run in strict sequential order. A workflow can depend on completion of other workflows.
// - RenderGraph: schedule workflows (thread-safe); submit() submits all scheduled workflows for async execution in a topological order that satisfies
// dependencies.
//   After submit(), the graph is reset and all workflow pointers from schedule() are invalidated.

struct ArtifactDatabase;

struct RuntimeType {
    const uint64_t type;

    template<typename T>
    T * castTo() {
        if (type == T::TYPE) GN_LIKELY return static_cast<T *>(this);
        return nullptr;
    }

    template<typename T>
    const T * castTo() const {
        if (type == T::TYPE) GN_LIKELY return static_cast<const T *>(this);
        return nullptr;
    }

protected:
    RuntimeType(uint64_t type): type(type) {}
};

/// Artifact represents an atomic resource that can be used as input or output of a task.
struct Artifact : public RefCounter, public RuntimeType {
    ArtifactDatabase & database;
    const StrA         name;
    const uint64_t     sequence; ///< the unique integer identifier of the artifact in the artifact database.

    virtual ~Artifact() {}

protected:
    /// Constructor
    Artifact(ArtifactDatabase & db, uint64_t type, const StrA & name);
};

template<class T>
concept DerivedFromArtifact = std::derived_from<T, Artifact>;

/// Database of all artifacts. Artifact is uniquely identified by its type and name, or by its sequence number.
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

    /// Erase an artifact instance by its sequence number.
    virtual bool erase(uint64_t sequence) = 0;

    /// Search for an artifact instance by type and name.
    virtual auto fetch(uint64_t type, const StrA & name) -> AutoRef<Artifact> = 0;

    /// Search for an artifact instance by its sequence number. Faster than search by ID.
    virtual auto fetch(uint64_t sequence) -> AutoRef<Artifact> = 0;

protected:
    ArtifactDatabase() = default;
};

inline Artifact::Artifact(ArtifactDatabase & db, uint64_t type, const StrA & name)
    : RuntimeType(type), database(db), name(name), sequence(database.admit(this)) {}

/// Base class of arguments for an action. This is not a subclass of Artifact, since it is means to be one time use: create, pass to action, and forget.
class Arguments : public RefCounter, public RuntimeType {
public:
    enum class UsageFlag {
        None     = 0,
        Optional = 1 << 0,
        Reading  = 1 << 1,
        Writing  = 1 << 2,
        // Aliases for convenience
        N  = None,
        O  = Optional,
        R  = Reading,
        W  = Writing,
        RW = Reading | Writing,
    };

    friend constexpr UsageFlag operator|(UsageFlag a, UsageFlag b) { return UsageFlag(uint32_t(a) | uint32_t(b)); }
    friend constexpr UsageFlag operator&(UsageFlag a, UsageFlag b) { return UsageFlag(uint32_t(a) & uint32_t(b)); }

    /// Type-erased base for artifact parameters. Enables iteration and dependency analysis without knowing concrete argument types.
    struct Argument {
        virtual ~Argument() {}

        const char * name() const { return mName; }
        UsageFlag    usage() const { return mUsage; }

        virtual SafeArrayAccessor<const Artifact *> artifacts() const = 0;

    protected:
        Argument(ReflectionRegister & rr, const char * name, UsageFlag usageFlags): mName(name), mUsage(usageFlags) { rr.enlist(this); }

    private:
        const char * mName;
        UsageFlag    mUsage;
    };

    /// Base class of all parameters that references one or more artifacts
    template<UsageFlag UFlags = UsageFlag::None>
    struct ArtifactArgument : Argument {
    protected:
        ArtifactArgument(ReflectionRegister & rr, const char * name) : Argument(rr, name, UFlags) {}
    };

    /// Represents a single artifact parameter of an action.
    /// T must be a subclass of Artifact.
    template<DerivedFromArtifact T, UsageFlag UFlags = UsageFlag::None>
    struct SingleArtifact : public ArtifactArgument<UFlags> {
        using ArtifactArgument<UFlags>::ArtifactArgument;

        SafeArrayAccessor<const Artifact *> artifacts() const override { return SafeArrayAccessor<const Artifact *>(value.addr(), 1); }

        AutoRef<T> value;
    };

    template<DerivedFromArtifact T, UsageFlag UFlags = UsageFlag::None>
    using ReadOnly = SingleArtifact<T, UFlags | UsageFlag::Reading>;

    template<DerivedFromArtifact T, UsageFlag UFlags = UsageFlag::None>
    using WriteOnly = SingleArtifact<T, UFlags | UsageFlag::Writing>;

    template<DerivedFromArtifact T, UsageFlag UFlags = UsageFlag::None>
    using ReadWrite = SingleArtifact<T, UFlags | UsageFlag::Reading | UsageFlag::Writing>;

    template<DerivedFromArtifact T, size_t Count, UsageFlag UFlags = UsageFlag::None>
    struct ArrayArtifact : public ArtifactArgument<UFlags> {
        using ArtifactArgument<UFlags>::ArtifactArgument;

        SafeArrayAccessor<const Artifact *> artifacts() const override { return SafeArrayAccessor<const Artifact *>(values, Count); }

        AutoRef<T> values[Count];
    };

    template<typename T, size_t COUNT, UsageFlag UFlags = UsageFlag::None>
    using ReadOnlyArray = ArrayArtifact<T, COUNT, UFlags | UsageFlag::Reading>;

    template<typename T, size_t COUNT, UsageFlag UFlags = UsageFlag::None>
    using WriteOnlyArray = ArrayArtifact<T, COUNT, UFlags | UsageFlag::Writing>;

    template<typename T, size_t COUNT, UsageFlag UFlags = UsageFlag::None>
    using ReadWriteArray = ArrayArtifact<T, COUNT, UFlags | UsageFlag::Reading | UsageFlag::Writing>;

    SafeArrayAccessor<const Argument *> arguments() const { return auto_reflection.enlistments; }

protected:
    struct ArgumentReflection {
        DynaArray<Argument *> enlistments;

        void enlist(Argument * arg) {
            GN_ASSERT(arg);
            GN_ASSERT(arg->name() != nullptr);
            GN_ASSERT(arg->usage() != UsageFlag::None);
            enlistments.append(arg);
        }
    };

    Arguments(uint64_t type): RuntimeType(type) {}

    /// This is the member variable that register artifact arguments defined in subclasses to an internal list
    /// that submission class can query.
    ArgumentReflection auto_reflection;

};

struct Submission;
struct TaskInfo;

/// Base class of all actions. An action holds the logic for an operation and declares its parameters (input/output).
struct Action : public Artifact {
    enum ExecutionResult {
        PASSED,  ///< the action executed successfully.
        WARNING, ///< the action executed successfully, but with warnings.
        FAILED,  ///< the action failed; dependents may be skipped.
    };

    /// A action might be used in multiple tasks. So we need a context structure to store data associated to a particular task.
    struct ExecutionContext : public RuntimeType {
        virtual ~ExecutionContext() = default;

    protected:
        ExecutionContext(uint64_t type): RuntimeType(type) {}
    };

    /// Prepare for execution. Returns success code and an optional execution context that will be later passed to execute().
    /// \param submission The submission that is executing the action.
    /// \param taskInfo The information of the task that is executing the action.
    /// \param arguments The arguments for the action.
    /// \return A pair of execution result and an optional execution context.
    virtual std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo & taskInfo, Arguments & arguments) = 0;

    /// Execute the action with the given arguments. The context is the same as the one returned from prepare().
    /// \param submission The submission that is executing the action.
    /// \param taskInfo Same as the one passed to prepare().
    /// \param arguments Same as the one passed to prepare().
    /// \param context The context returned from prepare().
    /// \return The execution result.
    virtual ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments, ExecutionContext * context) = 0;

protected:
    /// Inherit constructor from Artifact
    using Artifact::Artifact;
};

/// A workflow is a sequence of tasks run in strict sequential order. It can depend on completion of other workflows.
/// The render graph runs workflows in a topological order that satisfies these dependencies.
struct Workflow {
    /// Name for logging and debugging (not required, but recommended. No need to be unique).
    StrA name;

    /// The unique monotonically increasing sequence number of the workflow.
    /// If (A->sequence - B->sequence) > 0, then A is considered newer than, or "after", B.
    /// We use signed integer as the sequence number. So it can overflow safely.
    int64_t sequence;

    /// Represents a single task in the workflow. This is the atomic execution unit of the render graph.
    /// \note
    /// - Task A is considered newer than task B, if any of the following is true:
    ///   - A and B belong to same workflow. A's index is greater than B's index in the tasks array.
    ///   - A and B belong to different workflows. A's workflow is newer than B's.
    /// - Task A is considered to depend on task B, only if A is newer than B and any of the following is true:
    ///   - A is reading or writing to an artifact that B is writing to.
    ///   - A is writing to an artifact that B is reading from.
    struct Task {
        explicit Task(const StrA & name): name(name) {}
        StrA               name; //< name for logging and debugging (not required, but recommended. No need to be unique).
        AutoRef<Action>    action;
        AutoRef<Arguments> arguments;
    };

    DynaArray<Task> tasks;
};

struct TaskInfo {
    Submission &   submission; ///< the submission that the task belongs to.
    const StrA     workflow;   ///< name of the workflow that the task belongs to.
    const StrA     task;       ///< name of the task.
    const uint64_t index;      ///< index of the task within the entire submission. Can also be used as the unique identifier of the task within the submission.
};

struct Submission : RefCounter {
    struct Parameters {
        StrA name; ///< name of the submission.
    };

    /// Result of execution
    struct Result {
        Action::ExecutionResult executionResult {};
        // for future extension, like individual task result, etc.
    };

    virtual ~Submission() = default;

    /// Check if submitted workflows are all finished.
    /// \todo: is this finished on CPU or finished on GPU? Maybe add a new method isProcessed() for CPU-only check.
    virtual bool isFinished() = 0;

    /// Get execution result of the submitted workflows. Will block calling thread until all workflows are finished.
    virtual Result result() = 0;

protected:
    Submission() = default;
};

/// Render graph: schedule workflows (thread-safe), then submit them for async execution.
struct RenderGraph {
    struct CreateParameters {
        // For future use
    };

    /// Create a new render graph instance
    static GN_API RenderGraph * create(const CreateParameters & params);

    /// @brief Destroy the render graph instance. This method will try its best to cancel all pending tasks and workflows.
    /// Once the method returns, the render graph is no longer usable, all pointers returned from schedule() are invalidated,
    /// all workflows are either finished or cancelled. The detailed result can be queried via the submission object returned by submit().
    virtual ~RenderGraph() = default;

    /// Schedule a new workflow.
    /// \param name The name of the workflow.
    /// \return a pointer to the scheduled workflow. The pointer is valid until submit() is called.
    /// \note
    ///  - The returned workflow is free to modify until submit() is called.
    ///  - Call to submit() will invalidate all scheduled workflow pointers.
    ///  - Modifying workflow that has been submitted is undefined behavior.
    ///  - A newly scheduled workflow is always considered newer than any previously scheduled workflow.
    ///  - A task in newer workflow might depend on a task in older workflow, but never vise versa.
    virtual Workflow * schedule(StrA name) = 0;

    /// Submit all scheduled workflows for async execution in a topological order that satisfies workflow dependencies.
    /// Returns immediately; execution may not be complete when this method returns.
    /// After submission, the graph is reset to initial state and ready for new scheduling.
    virtual AutoRef<Submission> submit(const Submission::Parameters & params) = 0;

protected:
    RenderGraph() = default;
};

} // namespace GN::rdg

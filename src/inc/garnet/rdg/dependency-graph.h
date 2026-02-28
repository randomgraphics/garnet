#pragma once

#include <garnet/GNbase.h>

#include <concepts>
#include <functional>
#include <unordered_map>

namespace GN::rdg {

// Render graph: workflows are created (possibly from multiple threads), then executed in topological order.
//
// - Action: contains the code/logic for an operation. Declares a set of predefined parameters (input and/or output).
// - Task: one Action + one arguments value (an action-defined struct, type-erased as std::any) to run that action.
// - Workflow: a sequence of tasks run in strict sequential order. A workflow can depend on completion of other workflows.
// - RenderGraph: create workflows (thread-safe); submit() submits selected list of workflows for async execution in a topological order that satisfies
// dependencies.

struct ArtifactDatabase;

struct RuntimeType {
    /// The unique identifier of the type for fast lookup and comparison.
    /// This is a runtime ID generated each time application starts.
    /// It is guaranteed to be unique within the entire application, even across DLL boundaries.
    /// But DO NOT use it in external assets or files, since they are not stable and will change from session to session.
    const uint64_t typeId;

    /// The name of the type. No need to be unique, for now.
    /// \note We could enforce uniqueness of the name too, if we want a stable ID for each type in the future.
    const char * const typeName;

    template<typename T>
    T * castTo() {
        if (typeId == T::TYPE_ID) GN_LIKELY return static_cast<T *>(this);
        return nullptr;
    }

    template<typename T>
    const T * castTo() const {
        if (typeId == T::TYPE_ID) GN_LIKELY return static_cast<const T *>(this);
        return nullptr;
    }

protected:
    RuntimeType(uint64_t typeId_, const char * typeName_): typeId(typeId_), typeName(typeName_) {}

    /// Returns a 64-bit integer that uniquely map to the name.
    /// Same name always returns the same ID.
    static GN_API uint64_t getStableTypeIdFromName(const char * name);
};

/// The basic building block of the render graph module. Base class of everthing that could be added to a render graph.
struct Artifact : public RefCounter, public RuntimeType {
    ArtifactDatabase & database;
    const StrA         name;
    const uint64_t     sequence; ///< the unique integer identifier of the artifact in the artifact database.

    virtual ~Artifact() {}

protected:
    /// Constructor
    Artifact(ArtifactDatabase & db, uint64_t typeId, const char * typeName, const StrA & name);
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

inline Artifact::Artifact(ArtifactDatabase & db, uint64_t typeId, const char * typeName, const StrA & name)
    : RuntimeType(typeId, typeName), database(db), name(name), sequence(database.admit(this)) {}

/// A helper class to wrap anything as an artifact.
/// \param T    Type of the value to wrap.
/// \param NAME Name of the artifact.
template<typename T, const char * NAME>
class TypedArtifact : public Artifact {
public:
    inline static const uint64_t TYPE_ID   = getStableTypeIdFromName(StrA::format("{}_{}", NAME, Guid::createRandom().toStr()).data());
    inline static const char *   TYPE_NAME = NAME;

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
    TypedArtifact(ArtifactDatabase & db, const StrA & name): Artifact(db, TYPE_ID, TYPE_NAME, name), value() {}
    TypedArtifact(ArtifactDatabase & db, const StrA & name, const T & v): Artifact(db, TYPE_ID, TYPE_NAME, name), value(v) {}
    TypedArtifact(ArtifactDatabase & db, const StrA & name, T && v): Artifact(db, TYPE_ID, TYPE_NAME, name), value(std::move(v)) {}
};

/// Base class of arguments for an action. This is not a subclass of Artifact, since it is means to be one time use: create, pass to action, and forget.
class Arguments : public RefCounter, public RuntimeType {
public:
    struct UsageFlag {
        bool optional : 1 = false;
        bool reading  : 1 = false;
        bool writing  : 1 = false;

        constexpr bool operator==(const UsageFlag & other) const { return optional == other.optional && reading == other.reading && writing == other.writing; }
        constexpr bool operator!=(const UsageFlag & other) const { return optional != other.optional || reading != other.reading || writing != other.writing; }

        constexpr UsageFlag & operator+=(const UsageFlag & other) {
            optional &= other.optional;
            reading |= other.reading;
            writing |= other.writing;
            return *this;
        }

        friend constexpr UsageFlag operator+(UsageFlag a, const UsageFlag & b) { return a += b; }
    };

    struct Usage {
        inline static constexpr UsageFlag None           = {false, false, false};
        inline static constexpr UsageFlag Optional       = {true, false, false};
        inline static constexpr UsageFlag Reading        = {false, true, false};
        inline static constexpr UsageFlag Writing        = {false, false, true};
        inline static constexpr UsageFlag ReadingWriting = {false, true, true};
        inline static constexpr UsageFlag O              = Optional;
        inline static constexpr UsageFlag R              = Reading;
        inline static constexpr UsageFlag W              = Writing;
        inline static constexpr UsageFlag RW             = ReadingWriting;
    };

    /// Base class of all parameters that references one or more artifacts.
    /// Enlisted into a doubly linked list via DoubleLink member for zero-allocation iteration; no vector.
    struct ArtifactArgument {
        virtual ~ArtifactArgument() {}

        auto name() const -> const char * { return mName; }
        auto usage() const -> UsageFlag { return mUsage; }

        /// Returns list of artifacts referenced by this argument.
        virtual auto artifacts() const -> SafeArrayAccessor<const Artifact * const> = 0;

        /// Linked-list iteration (no allocation). \c nullptr when no next/prev.
        const ArtifactArgument * next() const { return mLink.next ? static_cast<const ArtifactArgument *>(mLink.next->context) : nullptr; }
        const ArtifactArgument * prev() const { return mLink.prev ? static_cast<const ArtifactArgument *>(mLink.prev->context) : nullptr; }

    protected:
        ArtifactArgument(Arguments * owner, const char * name, UsageFlag usage): mName(name), mUsage(usage) {
            mLink.context = this;
            owner->enlist(this);
        }

    private:
        const char * mName;
        UsageFlag    mUsage;
        DoubleLink   mLink;
        friend class Arguments;
    };

    /// Represents a single artifact parameter of an action.
    /// T must be a subclass of Artifact.
    template<DerivedFromArtifact T, UsageFlag UFlags = Usage::None>
    struct SingleArtifact : public ArtifactArgument {
        SingleArtifact(Arguments * owner, const char * name): ArtifactArgument(owner, name, UFlags) {}

        SafeArrayAccessor<const Artifact * const> artifacts() const override { return {(const Artifact * const *) value.addr(), 1}; }

        AutoRef<T> value;

    private:
        mutable DynaArray<const Artifact *> mArtifacts;
    };

    template<DerivedFromArtifact T, UsageFlag UFlags = Usage::None>
    using ReadOnlyArtifact = SingleArtifact<T, UFlags + Usage::Reading>;

    template<DerivedFromArtifact T, UsageFlag UFlags = Usage::None>
    using WriteOnlyArtifact = SingleArtifact<T, UFlags + Usage::Writing>;

    template<DerivedFromArtifact T, UsageFlag UFlags = Usage::None>
    using ReadWriteArtifact = SingleArtifact<T, UFlags + Usage::Reading + Usage::Writing>;

    template<DerivedFromArtifact T, size_t Count, UsageFlag UFlags = Usage::None>
    struct ArtifactArray : public ArtifactArgument {
        ArtifactArray(Arguments * owner, const char * name): ArtifactArgument(owner, name, UFlags) {}

        SafeArrayAccessor<const Artifact * const> artifacts() const override { return {(const Artifact * const *) values[0].addr(), Count}; }

        AutoRef<T> values[Count];
    };

    template<typename T, size_t COUNT, UsageFlag UFlags = Usage::None>
    using ReadOnlyArray = ArtifactArray<T, COUNT, UFlags + Usage::Reading>;

    template<typename T, size_t COUNT, UsageFlag UFlags = Usage::None>
    using WriteOnlyArray = ArtifactArray<T, COUNT, UFlags + Usage::Writing>;

    template<typename T, size_t COUNT, UsageFlag UFlags = Usage::None>
    using ReadWriteArray = ArtifactArray<T, COUNT, UFlags + Usage::Reading + Usage::Writing>;

    template<typename T, UsageFlag UFlags = Usage::None>
    struct ArtifactVector : public ArtifactArgument {
        ArtifactVector(Arguments * owner, const char * name): ArtifactArgument(owner, name, UFlags) {}

        SafeArrayAccessor<const Artifact * const> artifacts() const override { return {(const Artifact * const *) values[0].addr(), values.size()}; }

        DynaArray<AutoRef<T>> values;
    };

    template<typename T, UsageFlag UFlags = Usage::None>
    using ReadOnlyVector = ArtifactVector<T, UFlags + Usage::Reading>;

    template<typename T, UsageFlag UFlags = Usage::None>
    using WriteOnlyVector = ArtifactVector<T, UFlags + Usage::Writing>;

    template<typename T, UsageFlag UFlags = Usage::None>
    using ReadWriteVector = ArtifactVector<T, UFlags + Usage::Reading + Usage::Writing>;

    /// Returns the first artifact argument in the enlistment list. Iterate with \c p->next() until \c nullptr. No allocation.
    const ArtifactArgument * firstArtifactArgument() const { return mHead ? static_cast<const ArtifactArgument *>(mHead->context) : nullptr; }

protected:
    using RuntimeType::RuntimeType;

private:
    DoubleLink * mHead = nullptr;

    /// Only called by ArtifactArgument constructor to enlist the argument into the list.
    void enlist(ArtifactArgument * arg) {
        GN_ASSERT(arg);
        GN_ASSERT(arg->name() != nullptr);
        GN_ASSERT(arg->usage() != Usage::None);
        DoubleLink * link = const_cast<DoubleLink *>(&arg->mLink);
        GN_ASSERT(link->prev == nullptr && link->next == nullptr);

        if (!mHead)
            mHead = link;
        else {
            GN_ASSERT(mHead->prev == nullptr);
            link->linkBefore(mHead);
            mHead = link;
        }
    }

    friend struct ArtifactArgument;
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
        ExecutionContext(uint64_t typeId, const char * typeName): RuntimeType(typeId, typeName) {}
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
        explicit Task(const StrA & name): name(name) {}
        StrA               name; //< name for logging and debugging (not required, but recommended. No need to be unique).
        AutoRef<Action>    action;
        AutoRef<Arguments> arguments;
    };

    DynaArray<Task> tasks;

    /// Collect usage of all artifacts
    std::unordered_map<uint64_t, Arguments::UsageFlag> collectArtifactArguments() const {
        std::unordered_map<uint64_t, Arguments::UsageFlag> result;
        for (const Task & task : tasks) {
            if (!task.arguments) GN_LIKELY continue;
            for (const Arguments::ArtifactArgument * p = task.arguments->firstArtifactArgument(); p; p = p->next()) {
                for (const Artifact * a : p->artifacts()) {
                    if (a) GN_LIKELY result[a->typeId] += p->usage();
                }
            }
        }
        return result;
    }
};

struct TaskInfo {
    Submission &   submission; ///< the submission that the task belongs to.
    const StrA     workflow;   ///< name of the workflow that the task belongs to.
    const StrA     task;       ///< name of the task.
    const uint64_t index;      ///< index of the task within the entire submission. Can also be used as the unique identifier of the task within the submission.
};

struct Submission : RefCounter {
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
    Submission() = default;
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

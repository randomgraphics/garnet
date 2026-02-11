#pragma once

#include <garnet/GNbase.h>

#include <functional>
#include <unordered_map>
#include <optional>

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

/// Artifact represents an atomic resource that can be used as input or output of a task.
struct Artifact : public RefCounter {
    struct Identification {
        const Guid & type;
        StrA         name;

        Identification(const Guid & type, const StrA & name): type(type), name(name) {}

        Identification(const Identification & other): type(other.type), name(other.name) {}

        Identification(Identification && other): type(other.type), name(std::move(other.name)) {}
    };

    ArtifactDatabase &   database;
    const Identification id;
    const uint64_t       sequence; ///< unique number of the artifact in the artifact database.

    virtual ~Artifact() {};

    template<typename T>
    T * castTo() {
        if (id.type == T::TYPE) GN_LIKELY return static_cast<T *>(this);
        return nullptr;
    }

    template<typename T>
    const T * castTo() const {
        if (id.type == T::TYPE) GN_LIKELY return static_cast<const T *>(this);
        return nullptr;
    }

protected:
    /// Constructor
    Artifact(ArtifactDatabase & db, const Identification & id_, uint64_t seq): database(db), id(id_), sequence(seq) {}

private:
};

struct ArtifactDatabase {
    /// Parameters for creating an artifact database
    struct CreateParameters {
        // If true, then automatically register all built-in artifacts defined in this header. Or else, create an completely
        // empty database w/o any artifact type registered.
        bool autoAdmitAllBuiltInArtifacts = true;
    };

    /// Create a new artifact database instance
    static GN_API ArtifactDatabase * create(const CreateParameters & params);

    virtual ~ArtifactDatabase() = default;

    /// Admit a new artifact type to the database.
    virtual bool admit(const Guid & type, std::function<AutoRef<Artifact>(const StrA & name, uint64_t sequence)> && creator) = 0;

    /// Spawn a new artifact instance of the given type. Fail if the ID exists already.
    virtual auto spawn(const Artifact::Identification &) -> AutoRef<Artifact> = 0;

    /// Search for an artifact instance by its ID.
    virtual auto fetch(const Artifact::Identification &) -> AutoRef<Artifact> = 0;

    /// Search for an artifact instance by its sequence number. Faster than search by ID.
    virtual auto fetch(uint64_t) -> AutoRef<Artifact> = 0;

    /// Erase an artifact instance by its ID.
    virtual bool erase(const Artifact::Identification &) = 0;

    /// Erase an artifact instance by its sequence number. Faster than erase by ID.
    virtual bool erase(uint64_t sequence) = 0;

    template<typename T>
    AutoRef<T> spawn(const StrA & name) {
        auto a = spawn({T::TYPE, name});
        if (!a) return {};
        auto t = a->template castTo<T>();
        if (!t) return {};
        return AutoRef<T>(t);
    }

    // A helper template to create and reset certain type of artifact in one function call.
    template<typename T, typename... CREATE_ARGS>
    AutoRef<T> spawnAndReset(const StrA & name, CREATE_ARGS &&... args) {
        auto a = spawn({T::TYPE, name});
        if (!a) return {};
        auto t = a->template castTo<T>();
        if (!t) return {};
        if (!t->reset(std::forward<CREATE_ARGS>(args)...)) return {};
        return AutoRef<T>(t);
    }

    // A helper template to create and load certain type of artifacts in one function call
    template<typename T, typename... LOAD_ARGS>
    AutoRef<T> spawnAndLoad(const StrA & name, LOAD_ARGS &&... args) {
        auto a = spawn({T::TYPE, name});
        if (!a) return {};
        auto t = a->template castTo<T>();
        if (!t) return {};
        if (!t->load(std::forward<LOAD_ARGS>(args)...)) return {};
        return AutoRef<T>(t);
    }
};

/// Base class of arguments for an action.
struct Arguments : Artifact {
    enum class UsageFlag {
        None = 0,
        Optional = 1 << 0,
        Reading  = 1 << 1,
        Writing  = 1 << 2,
        // Aliases for convenience
        N = None,
        O = Optional,
        R = Reading,
        W = Writing,
    };

    friend constexpr UsageFlag operator|(UsageFlag a, UsageFlag b) { return UsageFlag(uint32_t(a) | uint32_t(b)); }
    friend constexpr UsageFlag operator&(UsageFlag a, UsageFlag b) { return UsageFlag(uint32_t(a) & uint32_t(b)); }

    /// Base class of all typed parameters (for use by concrete action implementations).
    template<UsageFlag UFlags = UsageFlag::None>
    struct Parameter {
        static constexpr bool IS_OPTIONAL = (UFlags & UsageFlag::Optional) != UsageFlag::None;
        static constexpr bool IS_REQUIRED = (UFlags & UsageFlag::Optional) == UsageFlag::None;
        static constexpr bool IS_Reading  = (UFlags & UsageFlag::Reading) != UsageFlag::None;
        static constexpr bool IS_Writing  = (UFlags & UsageFlag::Writing) != UsageFlag::None;
    };

    /// Represents a single parameter of an action.
    template<typename T, UsageFlag UFlags = UsageFlag::None>
    struct SingleParameter : public Parameter<UFlags> {
        void set(const T & value) { mValue = value; }
        void reset() { mValue.reset(); }
        auto get() const -> const T * { return mValue.has_value() ? &mValue.value() : nullptr; }
        auto get() -> T * { return mValue.has_value() ? &mValue.value() : nullptr; }

    private:
        std::optional<T> mValue;
    };

    template<typename T, UsageFlag UFlags = UsageFlag::None>
    using ReadOnly = SingleParameter<T, UFlags | UsageFlag::Reading>;

    template<typename T, UsageFlag UFlags = UsageFlag::None>
    using WriteOnly = SingleParameter<T, UFlags | UsageFlag::Writing>;

    template<typename T, UsageFlag UFlags = UsageFlag::None>
    using ReadWrite = SingleParameter<T, UFlags | UsageFlag::Reading | UsageFlag::Writing>;

    template<typename T, size_t Count, UsageFlag UFlags = UsageFlag::None>
    struct ArrayParameter : public Parameter<UFlags> {
        void set(size_t index, const T & value) {
            if (index >= Count) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.rg"))("ArrayParameter: index out of range");
                    return;
                }
            mStorage[index] = value;
            mValues[index]  = &mStorage[index].value();
        }

        void reset(size_t index) {
            if (index >= Count) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.rg"))("ArrayParameter: index out of range");
                    return;
                }
            mValues[index] = nullptr;
            mStorage[index].reset();
        }

        void reset() {
            for (size_t i = 0; i < Count; ++i) {
                mValues[i] = nullptr;
                mStorage[i].reset();
            }
        }

        const T * get(size_t index) const {
            if (index >= Count) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.rg"))("ArrayParameter: index out of range");
                    return nullptr;
                }
            return mValues[index];
        }

        const T * const * get() const { return mValues; }

    private:
        T *              mValues[Count]  = {};
        std::optional<T> mStorage[Count] = {};
    };

    template<typename Key, typename Value, UsageFlag UFlags = UsageFlag::None>
    struct MapParameter : public Parameter<UFlags> {
        void set(const Key & key, const Value & value) { mValue[key] = value; }
        void reset() { mValue.reset(); }
        void reset(const Key & key) { mValue.erase(key); }
        auto get(const Key & key) const -> const Value * { return mValue.find(key) != mValue.end() ? &mValue.find(key)->second : nullptr; }
        auto get(const Key & key) -> Value * { return mValue.find(key) != mValue.end() ? &mValue.find(key)->second : nullptr; }

    private:
        std::unordered_map<Key, Value> mValue;
    };

    template<typename T, size_t COUNT, UsageFlag UFlags = UsageFlag::None>
    using ReadOnlyArray = ArrayParameter<T, COUNT, UFlags | UsageFlag::Reading>;

    template<typename T, size_t COUNT, UsageFlag UFlags = UsageFlag::None>
    using WriteOnlyArray = ArrayParameter<T, COUNT, UFlags | UsageFlag::Writing>;

    template<typename T, size_t COUNT, UsageFlag UFlags = UsageFlag::None>
    using ReadWriteArray = ArrayParameter<T, COUNT, UFlags | UsageFlag::Reading | UsageFlag::Writing>;

    template<typename Key, typename Value, UsageFlag UFlags = UsageFlag::None>
    using ReadOnlyMap = MapParameter<Key, Value, UFlags | UsageFlag::Reading>;

    template<typename Key, typename Value, UsageFlag UFlags = UsageFlag::None>
    using WriteOnlyMap = MapParameter<Key, Value, UFlags | UsageFlag::Writing>;

    template<typename Key, typename Value, UsageFlag UFlags = UsageFlag::None>
    using ReadWriteMap = MapParameter<Key, Value, UFlags | UsageFlag::Reading | UsageFlag::Writing>;

protected:
    using Artifact::Artifact;
};

/// Base class of all actions. An action holds the logic for an operation and declares its parameters (input/output).
struct Action : public Artifact {
    enum ExecutionResult {
        PASSED,  ///< the action executed successfully.
        FAILED,  ///< the action failed; dependents may be skipped.
        WARNING, ///< the action executed with warnings.
    };

    /// Execute the action with the given arguments.
    virtual ExecutionResult execute(Arguments & arguments) = 0;

protected:
    /// Inherit constructor from Artifact
    using Artifact::Artifact;
};

/// A workflow is a sequence of tasks run in strict sequential order. It can depend on completion of other workflows.
/// The render graph runs workflows in a topological order that satisfies these dependencies.
struct Workflow {
    /// Name for logging and debugging (need not be unique).
    StrA name;

    /// Represents a single task in the workflow. This is the atomic execution unit of the render graph.
    struct Task {
        AutoRef<Action>    action;
        AutoRef<Arguments> arguments;
    };

    /// Tasks in this workflow; executed in order.
    DynaArray<Task> tasks;

    /// Other workflows that must complete before this workflow runs.
    DynaArray<Workflow *> dependencies;
};

struct Submission : RefCounter {
    /// Parameters for submission.
    struct Parameters {
        bool debug = false; ///< when true, generate debug statistics
    };

    /// Result of execution
    struct Result {
        Action::ExecutionResult result;
        StrA                    debugStats; ///< per-workflow/task stats when debug is true
    };

    virtual ~Submission() = default;

    //// Check if submitted workflows are all finished.
    virtual bool isFinished() = 0;

    /// Get execution result of the submitted workflows. Will block calling thread until all workflows are finished.
    virtual Result result() = 0;

protected:
    Submission() = default;
};

/// Render graph: schedule workflows (thread-safe), then submit them for async execution.
struct RenderGraph {
    struct CreateParameters {
        // TBD
    };

    /// Create a new render graph instance
    static GN_API RenderGraph * create(const CreateParameters & params);

    /// @brief Destroy the render graph instance. This method will try its best to cancel all pending tasks and workflows.
    /// Once the method returns, the render graph is no longer usable, all pointers returned from schedule() are invalidated,
    /// all workflows are either finished or cancelled. The detailed result can be queried via the submission object returned by submit().
    virtual ~RenderGraph() = default;

    /// Schedule a new workflow. Thread-safe; multiple threads may call schedule() in parallel.
    /// Returns a pointer to the scheduled workflow. The pointer is valid until submit() is called.
    /// After submit(), all pointers returned from schedule() are invalidated and the graph is ready to schedule new workflows.
    virtual Workflow * schedule() = 0;

    /// Submit all scheduled workflows for async execution in a topological order that satisfies workflow dependencies.
    /// Returns immediately; execution may not be complete when this method returns.
    /// After submission, the graph is reset to initial state and ready for new scheduling.
    virtual AutoRef<Submission> submit(const Submission::Parameters & params) = 0;

protected:
    RenderGraph() = default;
};

} // namespace GN::rdg

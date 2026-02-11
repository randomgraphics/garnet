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
    ArtifactDatabase & database;
    const Guid &       type;
    const StrA         name;
    const uint64_t     sequence; ///< the unique integer identifier of the artifact in the artifact database.

    virtual ~Artifact() {}

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
    /// Constructor
    Artifact(ArtifactDatabase & db, const Guid & type, const StrA & name);
};

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
    virtual auto fetch(const Guid & type, const StrA & name) -> AutoRef<Artifact> = 0;

    /// Search for an artifact instance by its sequence number. Faster than search by ID.
    virtual auto fetch(uint64_t sequence) -> AutoRef<Artifact> = 0;

protected:
    ArtifactDatabase() = default;
};

inline Artifact::Artifact(ArtifactDatabase & db, const Guid & type, const StrA & name)
: database(db), type(type), name(name), sequence(database.admit(this)) {}

/// Base class of arguments for an action.
struct Arguments : Artifact {
    enum Usage : uint32_t {
        OPTIONAL = 1 << 0,
        READING  = 1 << 1,
        WRITING  = 1 << 2,
    };

    /// Base class of all typed parameters (for use by concrete action implementations).
    template<uint32_t Usage = 0>
    struct Parameter {
        static constexpr bool OPTIONAL = (Usage & Arguments::OPTIONAL) != 0;
        static constexpr bool REQUIRED = !(Usage & Arguments::OPTIONAL);
        static constexpr bool READING  = (Usage & Arguments::READING) != 0;
        static constexpr bool WRITING  = (Usage & Arguments::WRITING) != 0;
    };

    /// Represents a single parameter of an action.
    template<typename T, uint32_t Usage>
    struct SingleParameter : public Parameter<Usage> {
        void set(const T & value) { mValue = value; }
        void reset() { mValue.reset(); }
        auto get() const -> const T * { return mValue.has_value() ? &mValue.value() : nullptr; }
        auto get() -> T * { return mValue.has_value() ? &mValue.value() : nullptr; }

    private:
        std::optional<T> mValue;
    };

    template<typename T, uint32_t Usage = 0>
    using ReadOnly = SingleParameter<T, Usage | READING>;

    template<typename T, uint32_t Usage = 0>
    using WriteOnly = SingleParameter<T, Usage | WRITING>;

    template<typename T, uint32_t Usage = 0>
    using ReadWrite = SingleParameter<T, Usage | READING | WRITING>;

    template<typename T, size_t Count, uint32_t Usage>
    struct ArrayParameter : public Parameter<Usage> {
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

    template<typename Key, typename Value, uint32_t Usage = 0>
    struct MapParameter : public Parameter<Usage> {
        void set(const Key & key, const Value & value) { mValue[key] = value; }
        void reset() { mValue.reset(); }
        void reset(const Key & key) { mValue.erase(key); }
        auto get(const Key & key) const -> const Value * { return mValue.find(key) != mValue.end() ? &mValue.find(key)->second : nullptr; }
        auto get(const Key & key) -> Value * { return mValue.find(key) != mValue.end() ? &mValue.find(key)->second : nullptr; }

    private:
        std::unordered_map<Key, Value> mValue;
    };

    template<typename T, size_t Count, uint32_t Usage = 0>
    using ReadOnlyArray = ArrayParameter<T, Count, Usage | READING>;

    template<typename T, size_t Count, uint32_t Usage = 0>
    using WriteOnlyArray = ArrayParameter<T, Count, Usage | WRITING>;

    template<typename T, size_t Count, uint32_t Usage = 0>
    using ReadWriteArray = ArrayParameter<T, Count, Usage | READING | WRITING>;

    template<typename Key, typename Value, uint32_t Usage = 0>
    using ReadOnlyMap = MapParameter<Key, Value, Usage | READING>;

    template<typename Key, typename Value, uint32_t Usage = 0>
    using WriteOnlyMap = MapParameter<Key, Value, Usage | WRITING>;

    template<typename Key, typename Value, uint32_t Usage = 0>
    using ReadWriteMap = MapParameter<Key, Value, Usage | READING | WRITING>;

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

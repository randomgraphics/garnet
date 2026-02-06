#pragma once

#include "GNbase.h"
#include "gfx/image.h"
#include <functional>
#include <unordered_map>
#include <initializer_list>
#include <any>
#include <variant>

namespace GN::rg {

// Render graph: workflows are scheduled (possibly from multiple threads), then executed in topological order.
//
// - Action: contains the code/logic for an operation. Declares a set of predefined parameters (input and/or output).
// - Task: one Action + one arguments value (an action-defined struct, type-erased as std::any) to run that action.
// - Workflow: a sequence of tasks run in strict sequential order. A workflow can depend on completion of other workflows.
// - RenderGraph: schedule workflows (thread-safe); execute() runs all scheduled workflows in a topological order that satisfies dependencies.
//   After execute(), the graph is reset and all workflow pointers from schedule() are invalidated.

/// Artifact represents an atomic resource that can be used as input or output of a task.
struct GN_API Artifact : public RefCounter {
    struct Identification {
        const Guid & type;
        StrA         name;

        Identification(const Guid & type, const StrA & name) : type(type), name(name) {}

        Identification(const Identification & other) : type(other.type), name(other.name) {}

        Identification(Identification && other) : type(other.type), name(std::move(other.name)) {}
    };

    const Identification id;
    const uint64_t       sequence; ///< unique number of the artifact in the artifact database.

    virtual ~Artifact() {};

protected:
    /// Constructor
    Artifact(const Identification & id_, uint64_t seq) : id(id_), sequence(seq) {}
};

struct ArtifactDatabase {
    virtual void registerArtifactType(const Guid & type, std::function<AutoRef<Artifact>(const StrA & name, uint64_t sequence)> && creator) = 0;
    virtual AutoRef<Artifact> create(const Artifact::Identification &, bool returnExisting = false) = 0;
    virtual AutoRef<Artifact> search(const Artifact::Identification &) = 0;
    virtual AutoRef<Artifact> search(uint64_t) = 0;
    virtual bool erase(const Artifact::Identification &) = 0;
    virtual bool erase(uint64_t sequence) = 0;
};

/// Parameters for creating an artifact database
struct CreateArtifactDatabaseParams {
    // Empty for now, can be expanded later
};

/// Create a new artifact database instance
GN_API ArtifactDatabase * createArtifactDatabase(const CreateArtifactDatabaseParams & params);

/// Argument value for a task: either an artifact reference or a raw value of any type.
struct GN_API Argument {
    using Value = std::variant<AutoRef<Artifact>, std::any>;

    Value value;

    Argument() : value(std::in_place_type<std::any>) {}
    explicit Argument(AutoRef<Artifact> a) : value(std::move(a)) {}
    explicit Argument(std::any v) : value(std::move(v)) {}

    template <typename T>
    static Argument fromRaw(T && v) {
        Argument a;
        a.value = std::any(std::forward<T>(v));
        return a;
    }

    bool isArtifact() const { return std::holds_alternative<AutoRef<Artifact>>(value); }
    bool isRaw() const { return std::holds_alternative<std::any>(value); }

    Artifact * getArtifact() const {
        auto * p = std::get_if<AutoRef<Artifact>>(&value);
        return p ? p->get() : nullptr;
    }

    template <typename T>
    const T * getRaw() const {
        auto * p = std::get_if<std::any>(&value);
        if (!p) return nullptr;
        return std::any_cast<T>(p);
    }

    template <typename T>
    T * getRaw() {
        auto * p = std::get_if<std::any>(&value);
        if (!p) return nullptr;
        return std::any_cast<T>(p);
    }
};

/// Base class of all actions. An action holds the logic for an operation and declares its parameters (input/output).
struct GN_API Action : public Artifact {
    enum ExecutionResult {
        PASSED,  ///< the action executed successfully.
        FAILED,  ///< the action failed; dependents may be skipped.
        WARNING, ///< the action executed with warnings.
    };

    /// Execute the action with the given arguments. Arguments are an action-defined struct, passed type-erased as std::any.
    /// The implementation must std::any_cast to its own Arguments type (e.g. std::any_cast<const MyAction::Arguments&>(arguments)).
    virtual ExecutionResult execute(const std::any & arguments) = 0;

    enum ParameterUsage : uint32_t {
        OPTIONAL = 1 << 0,
        READING  = 1 << 1,
        WRITING  = 1 << 2,
    };

    /// Base class of all typed parameters (for use by concrete action implementations).
    struct Parameter {
        /// TBD
    };

    /// Represents a single parameter of an action.
    template <typename T, uint32_t Usage>
    struct SingleParameter : public Parameter {
        static constexpr bool OPTIONAL = (Usage & OPTIONAL) != 0;
        static constexpr bool REQUIRED = !(Usage & OPTIONAL);
        static constexpr bool READING  = (Usage & READING) != 0;
        static constexpr bool WRITING  = (Usage & WRITING) != 0;

        void set(const T & value) { mValue = value; }
        void reset() { mValue.reset(); }
        const T * get() const { return mValue.has_value() ? &mValue.value() : nullptr; }

    private:
        std::optional<T> mValue;
    };

    template <typename T, uint32_t Usage = 0>
    using ReadOnly = SingleParameter<T, Usage | READING>;

    template <typename T, uint32_t Usage = 0>
    using WriteOnly = SingleParameter<T, Usage | WRITING>;

    template <typename T, uint32_t Usage = 0>
    using ReadWrite = SingleParameter<T, Usage | READING | WRITING>;

    template <typename T, size_t Count, uint32_t Usage>
    struct ArrayParameter : public Parameter {
        static constexpr bool OPTIONAL = (Usage & OPTIONAL) != 0;
        static constexpr bool REQUIRED = !(Usage & OPTIONAL);
        static constexpr bool READING  = (Usage & READING) != 0;
        static constexpr bool WRITING  = (Usage & WRITING) != 0;

        void set(size_t index, const T & value) {
            if (index >= Count) GN_UNLIKELY {
                GN_ERROR(getLogger("GN.rg"))("ArrayParameter: index out of range");
                return;
            }
            mStorage[index] = value;
            mValues[index] = &mStorage[index].value();
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
        T * mValues[Count] = {};
        std::optional<T> mStorage[Count] = {};
    };

    template<typename T, size_t Count, uint32_t Usage = 0>
    using ReadOnlyArray = ArrayParameter<T, Count, Usage | READING>;

    template<typename T, size_t Count, uint32_t Usage = 0>
    using WriteOnlyArray = ArrayParameter<T, Count, Usage | WRITING>;

    template<typename T, size_t Count, uint32_t Usage = 0>
    using ReadWriteArray = ArrayParameter<T, Count, Usage | READING | WRITING>;

protected:
    /// Inherit constructor from Artifact
    using Artifact::Artifact;
};

/// A task is one action plus one arguments value (the action's Arguments struct, stored as std::any). Execution runs the action with that struct.
struct GN_API Task : public RefCounter {
    /// The action to run.
    AutoRef<Action> action;

    /// Arguments to pass to the action. Must be an instance of the action's own Arguments struct (e.g. Compose::Arguments).
    std::any arguments;

    /// Execute the action with the bound arguments. Returns the action's execution result.
    Action::ExecutionResult execute();

    virtual ~Task() = default;
};

/// A workflow is a sequence of tasks run in strict sequential order. It can depend on completion of other workflows.
/// The render graph runs workflows in a topological order that satisfies these dependencies.
struct GN_API Workflow : public RefCounter {
    /// Name for logging and debugging (need not be unique).
    StrA name;

    /// Tasks in this workflow; executed in order.
    DynaArray<AutoRef<Task>> tasks;

    /// Other workflows that must complete before this workflow runs.
    DynaArray<Workflow *> dependencies;

    Workflow() = default;
    virtual ~Workflow() = default;
};

/// Render graph: schedule workflows (thread-safe), then execute them in topological order.
struct GN_API RenderGraph {
    /// Parameters for execution.
    struct ExecutionParams {
        bool debug = false; ///< when true, generate debug statistics
    };

    /// Result of execute().
    struct ExecutionResult {
        Action::ExecutionResult result;
        StrA                    debugStats; ///< per-workflow/task stats when debug is true
    };

    /// Schedule a new workflow. Thread-safe; multiple threads may call schedule() in parallel.
    /// Returns a pointer to the scheduled workflow. The pointer is valid until execute() is called.
    /// After execute(), all pointers returned from schedule() are invalidated and the graph is reset.
    virtual Workflow * schedule() = 0;

    /// Run all scheduled workflows in a topological order that satisfies workflow dependencies.
    /// After completion, the graph is reset and ready for new scheduling.
    virtual ExecutionResult execute(const ExecutionParams & params) = 0;

    virtual ~RenderGraph() = default;
};

/// Parameters for creating a render graph
struct CreateRenderGraphParams {
    // Empty for now, can be expanded later
};

/// Create a new render graph instance
GN_API RenderGraph * createRenderGraph(const CreateRenderGraphParams & params);

// =============================================================================
// GPU artifact types
// =============================================================================

/// Texture represents a 2D/3D/cube texture with optional mipmap and array layers.
struct GN_API Texture : public Artifact {
    static inline const Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    /// Descriptor used when creating or declaring the texture (format, dimensions).
    struct Descriptor {
        gfx::img::PixelFormat format = gfx::img::PixelFormat::UNKNOWN();
        uint32_t              width  = 0;
        uint32_t              height = 0;
        uint32_t              depth  = 0;
        uint32_t              faces  = 0;  ///< 0 = default (6 for cubemap, 1 for others)
        uint32_t              levels = 0;  ///< 0 = full mipmap chain
    };

    /// Subresource range for binding a subset of the texture (mip levels, array layers, depth slices).
    struct Subresource {
        uint32_t mipLevel       = 0;
        uint32_t arrayLayer    = 0;
        uint32_t depthSlice    = 0;
        uint32_t numMipLevels  = (uint32_t) -1; ///< -1 means all mip levels
        uint32_t numArrayLayers = (uint32_t) -1; ///< -1 means all array layers
        uint32_t numDepthSlices = (uint32_t) -1; ///< -1 means all depth slices
    };

    /// Create a new instance of empty Texture The texture is not bound to any GPU resource yet. Must call reset() at least once for the texture to be valid to use.
    static AutoRef<Texture> create();

    virtual ~Texture() = default;

    /// Return the current texture descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create or recreate the underlying texture from the given descriptor. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

protected:
    using Artifact::Artifact;
};

/// Buffer represents a linear GPU buffer (vertex, index, constant, UAV, etc.).
struct GN_API Buffer : public Artifact {
    static inline const Guid TYPE = {0x2b91c74e, 0xa153, 0x4d8f, {0xb2, 0x0c, 0x1e, 0x5f, 0xa8, 0x33, 0xcc, 0x77}};

    /// Descriptor for buffer creation (size only for now; usage hints can be added later).
    struct Descriptor {
        uint64_t sizeInBytes = 0;
    };

    virtual ~Buffer() = default;

    /// Return the current buffer descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create or recreate the underlying buffer from the given descriptor. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

protected:
    using Artifact::Artifact;
};

/// Sampler represents GPU sampler state (filtering, addressing, LOD, anisotropy).
struct GN_API Sampler : public Artifact {
    static inline const Guid TYPE = {0x7be9c60e, 0xf783, 0x5c6f, {0x9f, 0xed, 0x08, 0xbd, 0xe5, 0x02, 0xaa, 0xee}};

    enum class Filter { POINT, LINEAR, ANISOTROPIC };
    enum class AddressMode { REPEAT, MIRROR_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER, MIRROR_CLAMP_TO_EDGE };

    /// Descriptor for sampler creation.
    struct Descriptor {
        Filter     filterMin     = Filter::LINEAR;
        Filter     filterMag     = Filter::LINEAR;
        Filter     filterMip     = Filter::LINEAR;
        AddressMode addressU     = AddressMode::REPEAT;
        AddressMode addressV    = AddressMode::REPEAT;
        AddressMode addressW    = AddressMode::REPEAT;
        float      mipLodBias   = 0.f;
        uint32_t   maxAnisotropy = 1;
        float      minLod       = 0.f;
        float      maxLod       = 0.f; ///< 0 often means "all mips"
    };

    // Create a new instance of Sampler. Must call reset() at least once for the sampler to be valid to use.
    AutoRef<Sampler> create();

    virtual ~Sampler() = default;

    /// Return the current buffer descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create or recreate the underlying buffer from the given descriptor. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

    protected:
    using Artifact::Artifact;
};

/// Composes one solid color and a set of textures into a single output texture.
/// Inputs: one color (set on the action) and up to MAX_INPUT_TEXTURES texture parameters.
/// Output: one texture (parameter "output").
struct GN_API Compose : public Action {
    struct Arguments {
        ReadOnly<Vector4f> color;
        ReadOnlyArray<AutoRef<Texture>, 8, OPTIONAL> textures;
    };

    virtual ~Compose() = default;

    /// Execute with arguments. The std::any must hold a const Compose::Arguments& or Compose::Arguments.
    Action::ExecutionResult execute(const std::any & arguments) override = 0;

protected:
    using Action::Action;
};

struct ComposeTask : public Task {
    AutoRef<Compose> action;
    Compose::Arguments arguments;
};

// struct Draw : public Action {
//     virtual ~Draw() = default;
// };

// struct Compute : public Action {
//     virtual ~Compute() = default;
// };

// struct LoadFromFile : public Action {
//     virtual ~LoadFromFile() = default;
// };

} // namespace GN::rg
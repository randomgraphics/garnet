#pragma once

#include "GNwin.h"
#include "gfx/image.h"
#include "gfx/gpures.h"
#include "gfx/primitive.h"
#include <functional>
#include <unordered_map>
#include <initializer_list>
#include <optional>
#include <type_traits>
#include <variant>

namespace GN::rg {

// struct RuntimeType {
//     const Guid & type();
// protected:
//     RuntimeType(const Guid & t) : mType(t) {}
// private:
//     const Guid & mType;
// };

// Render graph: workflows are scheduled (possibly from multiple threads), then executed in topological order.
//
// - Action: contains the code/logic for an operation. Declares a set of predefined parameters (input and/or output).
// - Task: one Action + one arguments value (an action-defined struct, type-erased as std::any) to run that action.
// - Workflow: a sequence of tasks run in strict sequential order. A workflow can depend on completion of other workflows.
// - RenderGraph: schedule workflows (thread-safe); submit() submits all scheduled workflows for async execution in a topological order that satisfies
// dependencies.
//   After submit(), the graph is reset and all workflow pointers from schedule() are invalidated.

/// Artifact represents an atomic resource that can be used as input or output of a task.
struct GN_API Artifact : public RefCounter {
    struct Identification {
        const Guid & type;
        StrA         name;

        Identification(const Guid & type, const StrA & name): type(type), name(name) {}

        Identification(const Identification & other): type(other.type), name(other.name) {}

        Identification(Identification && other): type(other.type), name(std::move(other.name)) {}
    };

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
    Artifact(const Identification & id_, uint64_t seq): id(id_), sequence(seq) {}
};

struct ArtifactDatabase {
    /// Parameters for creating an artifact database
    struct CreateParameters {
        // If true, then automatically register all built-in artifacts defined in this header. Or else, create an completely
        // empty database w/o any artifact type registered.
        bool autoAdmitAllBuiltInArtifacts = true;
    };

    /// Create a new artifact database instance
    static GN_API AutoRef<ArtifactDatabase> create(const CreateParameters & params);

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

    // A helper template to create and reset certain type of artifact in one function call.
    template<typename T, typename... CREATE_ARGS>
    AutoRef<T> spawnAndReset(const StrA & name, CREATE_ARGS &&... args) {
        auto a = spawn({T::TYPE, name});
        if (!a) return {};
        auto t = a->castTo<T>();
        if (!t) return {};
        if (!t->reset(std::forward<CREATE_ARGS>(args)...)) return {};
        return AutoRef<T>(t);
    }

    // A helper template to create and load certain type of artifacts in one function call
    template<typename T, typename... LOAD_ARGS>
    AutoRef<T> spawnAndLoad(const StrA & name, LOAD_ARGS &&... args) {
        auto a = spawn({T::TYPE, name});
        if (!a) return {};
        auto t = a->castTo<T>();
        if (!t) return {};
        if (!t->load(std::forward<LOAD_ARGS>(args)...)) return {};
        return AutoRef<T>(t);
    }
};

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
        static constexpr bool OPTIONAL = (Usage & OPTIONAL) != 0;
        static constexpr bool REQUIRED = !(Usage & OPTIONAL);
        static constexpr bool READING  = (Usage & READING) != 0;
        static constexpr bool WRITING  = (Usage & WRITING) != 0;
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
    static GN_API AutoRef<RenderGraph> create(const CreateParameters & params);

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

// =============================================================================
// GPU artifact types
// =============================================================================

/// GpuContext represents a GPU context (wrapper of D3D/Vulkan context) that exposes display properties.
struct GpuContext : public Artifact {
    static inline const Guid TYPE = {0x3c4d5e6f, 0x7a8b, 0x9c0d, {0x1e, 0x2f, 0x3a, 0x4b, 0x5c, 0x6d, 0x7e, 0x8f}};

    struct ResetParameters {
        /// The target window. If null, then create GPU context in headless mode.
        GN::win::Window * win = {};

        /// display width. If 0, use window width. Must be positive for headless mode.
        uint32_t width = 0;

        // display height. If 0, use window height. Must be positive for headless mode.
        uint32_t height = 0;
    };

    /// Create a new instance of GpuContext.
    static AutoRef<GpuContext> create();

    /// Destroy the GPU context.
    virtual ~GpuContext() = default;

    virtual bool reset(const ResetParameters &) = 0;

    /// Get display width and height
    virtual std::pair<uint32_t, uint32_t> dimension() const = 0;

protected:
    using Artifact::Artifact;
};

/// Texture represents a 2D/3D/cube texture with optional mipmap and array layers.
struct GN_API Texture : public Artifact {
    static inline const Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    /// Descriptor used when creating or declaring the texture (format, dimensions).
    struct Descriptor {
        gfx::img::PixelFormat format  = gfx::img::PixelFormat::UNKNOWN();
        uint32_t              width   = 1;
        uint32_t              height  = 1;
        uint32_t              depth   = 1; ///< 1 = 2D, >1 = 3D
        uint32_t              faces   = 1; ///< 1 = 2D, 6 = cubemap, >1 = array
        uint32_t              levels  = 0; ///< 0 = full mipmap chain, 1 = single level, >1 = mipmap chain with specified number of levels
        uint32_t              samples = 1; ///< 1 = no multisampling
    };

    struct SubresourceIndex {
        uint32_t mip   = 0; ///< index into mipmap chain
        uint32_t face  = 0; ///< index into array of faces
        uint32_t slice = 0; ///< index into depth slice
    };

    struct SubresourceRange {
        uint32_t numMipLevels   = (uint32_t) -1; ///< -1 means all mip levels
        uint32_t numArrayLayers = (uint32_t) -1; ///< -1 means all array layers
        uint32_t numDepthSlices = (uint32_t) -1; ///< -1 means all depth slices
    };

    /// Create a new instance of empty Texture The texture is not bound to any GPU resource yet. Must call reset() at least once for the texture to be valid to
    /// use.
    static AutoRef<Texture> create();

    virtual ~Texture() = default;

    /// Return the current texture descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create texture from the given descriptor. Discard old one. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

    /// Load texture from file. Discard old one. Returns true on success.
    virtual bool load(const StrA & filename) = 0;

protected:
    using Artifact::Artifact;
};

/// Backbuffer represents the a swapchain that can be present to screen.
struct GN_API Backbuffer : public Artifact {
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    /// Descriptor for backbuffer initialization.
    struct Descriptor {
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    /// Create a new instance of Backbuffer.
    static AutoRef<Backbuffer> create();

    virtual ~Backbuffer() = default;

    /// Initialize or reinitialize the backbuffer from the given descriptor. Returns true on success.
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
        Filter      filterMin     = Filter::LINEAR;
        Filter      filterMag     = Filter::LINEAR;
        Filter      filterMip     = Filter::LINEAR;
        AddressMode addressU      = AddressMode::REPEAT;
        AddressMode addressV      = AddressMode::REPEAT;
        AddressMode addressW      = AddressMode::REPEAT;
        float       mipLodBias    = 0.f;
        uint32_t    maxAnisotropy = 1;
        float       minLod        = 0.f;
        float       maxLod        = 0.f; ///< 0 often means "all mips"
    };

    // Create a new instance of Sampler. Must call reset() at least once for the sampler to be valid to use.
    static AutoRef<Sampler> create();

    virtual ~Sampler() = default;

    /// Return the current buffer descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create or recreate the underlying buffer from the given descriptor. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

protected:
    using Artifact::Artifact;
};

/// Buffer represents a GPU buffer (vertex, index, constant, storage, etc.).
struct GN_API Buffer : public Artifact {
    static inline const Guid TYPE = {0x1f2e3d4c, 0x5b6a, 0x7c8d, {0x9e, 0xaf, 0xb1, 0xc2, 0xd3, 0xe4, 0xf5, 0x06}};

    /// Buffer usage flags.
    enum Usage {
        VERTEX,       ///< Vertex buffer
        INDEX,        ///< Index buffer
        CONSTANT,     ///< Constant/uniform buffer
        STORAGE,      ///< Storage (RW) buffer
        INDIRECT,     ///< Indirect command buffer
        TRANSFER_SRC, ///< Can be used as transfer source
        TRANSFER_DST  ///< Can be used as transfer destination
    };

    /// Buffer descriptor for creation/reset.
    struct Descriptor {
        size_t size        = 0;      ///< Number of bytes for the buffer.
        Usage  usage       = VERTEX; ///< Usage flag.
        bool   cpuWritable = false;  ///< CPU can write (mapped buffer).
        bool   cpuReadable = false;  ///< CPU can read (mapped buffer).
    };

    /// Create a new instance of empty Buffer. The buffer is not bound to any GPU resource yet.
    /// Must call reset() at least once for the buffer to be valid to use.
    static AutoRef<Buffer> create();

    virtual ~Buffer() = default;

    /// Return the current buffer descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create or recreate the underlying buffer from the given descriptor. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

protected:
    using Artifact::Artifact;
};

/// Mesh represents a 3D geometry with vertex and index data.
struct GN_API Mesh : public Artifact {
    static inline const Guid TYPE = {0x8c9d4a1f, 0xb284, 0x5d7f, {0x9a, 0xfe, 0x19, 0xce, 0xf6, 0x13, 0xbb, 0xff}};

    /// Create a new instance of empty Mesh. The mesh is not bound to any GPU resource yet. Must call reset() or load() at least once for the mesh to be valid
    /// to use.
    static AutoRef<Mesh> create();

    virtual ~Mesh() = default;

    /// Load mesh from file. Discard old one. Returns true on success.
    virtual bool load(const StrA & filename) = 0;

protected:
    using Artifact::Artifact;
};

/// Render target is ether a subsurface of a texture or a back buffer
struct RenderTarget {
    std::variant<AutoRef<Texture>, AutoRef<Backbuffer>> target;
    Texture::SubresourceIndex                           sub; ///< only used for texture targets
};

struct ClearRenderTarget : public Action {
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
        struct ClearColor {
            struct {
                float r, g, b, a;
            };
            struct {
                uint32_t x, y, z, w;
            };
        };
        ReadOnly<ClearColor>    color;
        ReadWrite<RenderTarget> renderTarget;
    };
};

struct ClearDepthStencil : public Action {
    inline static constexpr Guid TYPE = {0xdb3ab1ef, 0xafc0, 0x4eca, {0x80, 0xfa, 0x49, 0xde, 0x23, 0x3c, 0xdf, 0x18}};
    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
        ReadOnly<float>              depth;
        ReadOnly<uint8_t>            stencil;
        ReadWrite<RenderTarget>      depthStencil;
    };
};

/// Composes one solid color and a set of textures into a single output texture.
/// Inputs: one color (set on the action) and up to MAX_INPUT_TEXTURES texture parameters.
/// Output: one texture (parameter "output").
struct Compose : public Action {
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    struct A : public Arguments {
        inline static constexpr Guid                 TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
        ReadOnly<AutoRef<Mesh>>                      mesh;
        ReadOnly<Vector4f>                           color;
        ReadOnlyArray<AutoRef<Texture>, 8, OPTIONAL> textures;
        ReadWriteArray<RenderTarget, 8, OPTIONAL>    renderTargets;
        ReadWrite<RenderTarget>                      depthStencil;
    };

protected:
    using Action::Action;
};

struct LoadTextureFromFile : public Action {
    inline static constexpr Guid TYPE = {0x825a7724, 0xfecb, 0x49e2, {0xb7, 0x1f, 0xfc, 0x9d, 0x3a, 0xa2, 0x8b, 0x11}};
    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x825a7724, 0xfecb, 0x49e2, {0xb7, 0x1f, 0xfc, 0x9d, 0x3a, 0xa2, 0x8b, 0x11}};
        ReadOnly<StrA>               filename; // Path to texture file
        WriteOnly<AutoRef<Texture>>  texture;  // Output texture resource
    };
};

struct PrepareBackbuffer : public Action {
    inline static constexpr Guid TYPE = {0x3e4f5a6b, 0x7c8d, 0x9e0f, {0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b}};
    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x3e4f5a6b, 0x7c8d, 0x9e0f, {0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b}};
        ReadWrite<Backbuffer>        backbuffer; // Backbuffer to prepare
    };
};

struct PresentBackbuffer : public Action {
    inline static constexpr Guid TYPE = {0x4f5a6b7c, 0x8d9e, 0x0f1a, {0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c}};
    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x4f5a6b7c, 0x8d9e, 0x0f1a, {0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c}};
        ReadOnly<Backbuffer>         backbuffer; // Backbuffer to present
    };
};

struct TextureReadback : public Action {
    inline static constexpr Guid TYPE = {0x5a6b7c8d, 0x9e0f, 0x1a2b, {0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d}};
    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x5a6b7c8d, 0x9e0f, 0x1a2b, {0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d}};
        ReadOnly<AutoRef<Texture>>   texture; ///< input texture
        WriteOnly<gfx::img::Image>   image;   ///< output image (will be cleared and filled with the texture content)
    };
};

/// Setup render states action for configuring GPU render pipeline state.
struct SetupRenderStates : public Action {
    inline static constexpr Guid TYPE = {0x7c8d9e0f, 0x1a2b, 0x3c4d, {0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e, 0x2f}};

    enum FillMode {
        FILL_SOLID = 0,
        FILL_WIREFRAME,
        FILL_POINT,
    };

    enum CullMode {
        CULL_NONE = 0,
        CULL_FRONT,
        CULL_BACK,
    };

    enum FrontFace {
        FRONT_CCW = 0,
        FRONT_CW,
    };

    enum ComparisonFunc {
        CMP_NEVER = 0,
        CMP_LESS,
        CMP_LESS_EQUAL,
        CMP_EQUAL,
        CMP_GREATER_EQUAL,
        CMP_GREATER,
        CMP_NOT_EQUAL,
        CMP_ALWAYS,
    };

    enum StencilOp {
        STENCIL_KEEP = 0,
        STENCIL_ZERO,
        STENCIL_REPLACE,
        STENCIL_INC_SAT,
        STENCIL_DEC_SAT,
        STENCIL_INVERT,
        STENCIL_INC,
        STENCIL_DEC,
    };

    enum BlendArg {
        BLEND_ZERO = 0,
        BLEND_ONE,
        BLEND_SRC_COLOR,
        BLEND_INV_SRC_COLOR,
        BLEND_SRC_ALPHA,
        BLEND_INV_SRC_ALPHA,
        BLEND_DEST_ALPHA,
        BLEND_INV_DEST_ALPHA,
        BLEND_DEST_COLOR,
        BLEND_INV_DEST_COLOR,
        BLEND_BLEND_FACTOR,
        BLEND_INV_BLEND_FACTOR,
    };

    enum BlendOp {
        BLEND_OP_ADD = 0,
        BLEND_OP_SUB,
        BLEND_OP_REV_SUB,
        BLEND_OP_MIN,
        BLEND_OP_MAX,
    };

    /// Viewport settings
    struct Viewport {
        float x        = 0.0f;
        float y        = 0.0f;
        float width    = 0.0f;
        float height   = 0.0f;
        float minDepth = 0.0f;
        float maxDepth = 1.0f;
    };

    /// Scissor rectangle
    struct ScissorRect {
        int32_t  x      = 0;
        int32_t  y      = 0;
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    /// Render state descriptor
    struct RenderStateDesc {
        // Rasterizer state
        std::optional<FillMode>  fillMode;    ///< fill mode (solid, wireframe, point)
        std::optional<CullMode>  cullMode;    ///< cull mode (none, front, back)
        std::optional<FrontFace> frontFace;   ///< front face winding (CCW, CW)
        std::optional<bool>      msaaEnabled; ///< MSAA enabled

        // Depth state
        std::optional<bool>           depthTestEnabled;  ///< enable depth testing
        std::optional<bool>           depthWriteEnabled; ///< enable depth writing
        std::optional<ComparisonFunc> depthFunc;         ///< depth comparison function

        // Stencil state
        std::optional<bool>           stencilEnabled;   ///< enable stencil testing
        std::optional<StencilOp>      stencilPassOp;    ///< stencil operation on pass
        std::optional<StencilOp>      stencilFailOp;    ///< stencil operation on fail
        std::optional<StencilOp>      stencilZFailOp;   ///< stencil operation on depth fail
        std::optional<ComparisonFunc> stencilFunc;      ///< stencil comparison function
        std::optional<uint8_t>        stencilRef;       ///< stencil reference value
        std::optional<uint8_t>        stencilReadMask;  ///< stencil read mask
        std::optional<uint8_t>        stencilWriteMask; ///< stencil write mask

        // Blend state
        std::optional<bool>     blendEnabled;  ///< enable blending
        std::optional<BlendArg> blendSrc;      ///< source blend factor
        std::optional<BlendArg> blendDst;      ///< destination blend factor
        std::optional<BlendOp>  blendOp;       ///< blend operation
        std::optional<BlendArg> blendAlphaSrc; ///< source alpha blend factor
        std::optional<BlendArg> blendAlphaDst; ///< destination alpha blend factor
        std::optional<BlendOp>  blendAlphaOp;  ///< alpha blend operation
        std::optional<Vector4f> blendFactors;  ///< blend factor color (RGBA)

        // Color write mask (per render target, 4 bits each: RGBA)
        std::optional<uint32_t> colorWriteMask; ///< color write mask (0xFFFFFFFF = all channels)

        // Viewport and scissor
        std::optional<Viewport>    viewport;    ///< viewport settings
        std::optional<ScissorRect> scissorRect; ///< scissor rectangle
    };

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x7c8d9e0f, 0x1a2b, 0x3c4d, {0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e, 0x2f}};
        ReadOnly<RenderStateDesc>    renderStates; ///< render state descriptor
    };
};

/// Base class for generic shader actions (draw and compute). Contains common shader resource binding definitions.
struct GenericShaderAction : public Action {
    struct ShaderResourceBinding {
        uint32_t set  = 0;
        uint32_t slot = 0;

        bool operator==(const ShaderResourceBinding & other) const { return set == other.set && slot == other.slot; }
    };

    struct BufferParameter {
        AutoRef<Buffer> buffer;
        uint32_t        offset = 0;
        uint32_t        size   = 0;
    };

    struct ImageParameter {
        AutoRef<Texture>          texture;
        gfx::img::PixelFormat     format = gfx::img::PixelFormat::UNKNOWN();
        Texture::SubresourceIndex subresourceIndex;
    };

    struct TextureParameter : public ImageParameter {
        AutoRef<Sampler>          sampler;
        Texture::SubresourceRange subresourceRange;
    };

protected:
    using Action::Action;
};

/// Generic draw action for quick GPU draw prototyping. It emphasizes ease of use and flexibility over extreme performance.
struct GenericDraw : public GenericShaderAction {
    inline static constexpr Guid TYPE = {0x6b7c8d9e, 0x0f1a, 0x2b3c, {0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e}};

    /// Draw parameters
    struct DrawParams {
        uint32_t vertexCount   = 0; ///< number of vertices to draw
        uint32_t instanceCount = 1; ///< number of instances to draw
        uint32_t firstVertex   = 0; ///< first vertex index
        uint32_t firstInstance = 0; ///< first instance index
    };

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x6b7c8d9e, 0x0f1a, 0x2b3c, {0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e}};

        ReadOnlyMap<GenericShaderAction::ShaderResourceBinding, GenericShaderAction::BufferParameter>  uniforms; ///< uniform buffers
        ReadOnlyMap<GenericShaderAction::ShaderResourceBinding, GenericShaderAction::TextureParameter> textures; ///< textures
        ReadOnlyMap<StrA, BufferParameter>                                                             vertices;
        ReadOnlyParameter<BufferParameter>                                                             indices;
        ReadOnly<DrawParams>                                                                           drawParams;    ///< draw parameters
        ReadWriteArray<RenderTarget, 8, OPTIONAL>                                                      renderTargets; ///< color render targets
        ReadWrite<RenderTarget, OPTIONAL>                                                              depthStencil; ///< depth/stencil render target (optional)
    };

    /// Shader stage description
    struct ShaderStageDesc {
        AutoRef<Blob> shaderBinary; ///< shader binary code
        StrA          entryPoint;   ///< entry point name
    };

    /// Reset the draw action with shader binaries for different stages.
    /// All parameters are optional - only provided stages will be set.
    virtual bool reset(const std::optional<ShaderStageDesc> & vs = {}, ///< vertex shader
                       const std::optional<ShaderStageDesc> & ds = {}, ///< domain shader
                       const std::optional<ShaderStageDesc> & hs = {}, ///< hull shader
                       const std::optional<ShaderStageDesc> & gs = {}, ///< geometry shader
                       const std::optional<ShaderStageDesc> & ps = {}  ///< pixel shader
                       ) = 0;
};

/// Generic compute action for quick GPU compute prototyping. It emphasizes ease of use and flexibility over extreme performance.
struct GenericCompute : public GenericShaderAction {
    inline static constexpr Guid TYPE = {0x5a6b7c8d, 0x9e0f, 0x1a2b, {0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d}};

    /// Dispatch dimensions (thread group counts)
    struct DispatchSize {
        uint32_t x = 1;
        uint32_t y = 1;
        uint32_t z = 1;
    };

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x5a6b7c8d, 0x9e0f, 0x1a2b, {0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d}};

        ReadOnlyMap<GenericShaderAction::ShaderResourceBinding, GenericShaderAction::BufferParameter>  uniforms; ///< uniform buffers
        ReadOnlyMap<GenericShaderAction::ShaderResourceBinding, GenericShaderAction::TextureParameter> textures; ///< textures
        ReadWriteMap<GenericShaderAction::ShaderResourceBinding, GenericShaderAction::BufferParameter> buffers;  ///< storage buffers
        ReadWriteMap<GenericShaderAction::ShaderResourceBinding, GenericShaderAction::ImageParameter>  images;   ///< storage images
        ReadOnly<DispatchSize>                                                                         groups;   ///< thread group counts
    };

    virtual bool reset(AutoRef<Blob> shaderBinary, const StrA & entryPoint) = 0;
};

} // namespace GN::rg

// Hash specialization for ShaderResourceBinding to use as key in std::unordered_map
namespace std {
template<>
struct hash<GN::rg::GenericShaderAction::ShaderResourceBinding> {
    size_t operator()(const GN::rg::GenericShaderAction::ShaderResourceBinding & binding) const {
        return std::hash<uint64_t> {}((static_cast<uint64_t>(binding.set) << 32) | binding.slot);
    }
};
} // namespace std
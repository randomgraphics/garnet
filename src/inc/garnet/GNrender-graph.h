#pragma once

#include "GNbase.h"
#include "gfx/image.h"
#include <functional>
#include <unordered_map>
#include <initializer_list>

namespace GN::rg {

// Render graph is a directed acyclic graph of tasks.
// Each task is an atomic self contained operation with deterministic input and output.
// Input and output are render artifacts managed by artifact database, including textures, buffers, GPU pipelines, pure CPU data blobs and etc.
//  all artifacts are uniquely identified by their name and type (or sequence number for faster lookup)
// The render graph has 2 main execution phases:
//  1. schedule phase:
//     - collecting all rendering operations and their dependencies.
//  2. execute phase:
//     - submit and execute all rendercing command lists

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
    virtual AutoRef<Artifact> create(const Artifact::Identification &) = 0;
    virtual AutoRef<Artifact> search(const Artifact::Identification &) = 0;
    virtual AutoRef<Artifact> search(uint64_t) = 0;
};

/// Parameters for creating an artifact database
struct CreateArtifactDatabaseParams {
    // Empty for now, can be expanded later
};

/// Create a new artifact database instance
GN_API ArtifactDatabase * createArtifactDatabase(const CreateArtifactDatabaseParams & params);


/// Base class of all actions
struct Action : public Artifact {
    struct Parameter {
        const Guid & type; // type of the parameter.
        StrA         name;
        bool         optional = false;
        bool         reading = false;
        bool         writing = false;

        /// Constructor for parameter with type, name, and options
        /// @param type The GUID type of the artifact this parameter references
        /// @param name The parameter name used for binding in action execution
        /// @param options String specifying parameter options:
        ///                'O' or 'o' = optional parameter (not required)
        ///                'R' or 'r' = reading access (input parameter)
        ///                'W' or 'w' = writing access (output parameter)
        Parameter(const Guid & type, const StrA & name, const char * options = nullptr);
    };

    enum ExecutionResult {
        PASSED,  ///< the action executed successfully w/o any issues.
        FAILED,  ///< the action failed. all other actions depending on this one will be skipped.
        WARNING, ///< the action executed successfully with some warnings.
    };

    virtual SafeArrayAccessor<const Parameter> parameters() const = 0;

    /// Execute the action with the given arguments
    virtual ExecutionResult execute(SafeArrayAccessor<Artifact> arguments) = 0;

protected:
    /// Inherit constructor from Artifact
    using Artifact::Artifact;
};

struct Task {
    struct Shard {
        AutoRef<Action> action;
        std::unordered_map<StrA, AutoRef<Artifact>> arguments;
        DynaArray<Task *> dependencies; // dependencies of the shard.
    };

    /// name of the task. doesn't have to be unique. for logging and debugging.
    StrA name;

    /// shards of the task. shards are always executed in strict sequential order.
    DynaArray<Shard> shards;

    /// global dependencies of the task. Each element in the array
    /// is the sequence number of the task that this task depends on.
    DynaArray<Task *> dependencies;

protected:
    /// Constructor for Task
    Task() = default;
    
    /// Destructor for Task
    ~Task() = default;
};

struct RenderGraph {
    /// Schedule a new task. Returns pointer to the task. MUlti thread safe.
    /// All task pointers are invalidated when execute() is called.
    virtual Task * schedule() = 0;

    /// Execute all scheduled tasks. Once the execution is complete,
    /// The render graph is reset to initial state and ready to schedule more tasks.
    virtual Action::ExecutionResult execute() = 0;

    virtual ~RenderGraph() = default;
};

/// Parameters for creating a render graph
struct CreateRenderGraphParams {
    // Empty for now, can be expanded later
};

/// Create a new render graph instance
GN_API RenderGraph * createRenderGraph(const CreateRenderGraphParams & params);

// /// Texture represents a traditional graphics texture with full mipmap chain
// struct Texture : public Artifact {
//     static inline const Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

//     struct Subresource {
//         uint32_t mipLevel = 0;
//         uint32_t arrayLayer = 0;
//         uint32_t depthSlice = 0;
//         uint32_t numMipLevels = (uint32_t)-1; // -1 means all mip levels.
//         uint32_t numArrayLayers = (uint32_t)-1; // -1 means all array layers.
//         uint32_t numDepthSlices = (uint32_t)-1; // -1 means all depth slices.
//     };

//     struct Parameter : public Action::Parameter {
//         // subresoufce range of the texture.
//         Subresource sub;
//         // If not UNKNOWN, defines the format of the sub resource.
//         gfx::img::PixelFormat format = gfx::img::PixelFormat::UNKNOWN;
//     };

//     virtual ~Texture() = default;
// };

// /// Buffer represents a traditional GPU buffer.
// struct Buffer : public Artifact {
//     static inline const Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

//     struct Parameter : public Action::Parameter {
//         uint64_t offsetInBytes = 0;
//         uint64_t sizeInBytes = (uint64_t)-1; // -1 means to the end of the buffer.
//     };

//     virtual ~Buffer() = default;
// };

// struct Draw : public Action {
//     virtual ~Draw() = default;
// };

// struct Compute : public Action {
//     virtual ~Compute() = default;
// };

// struct LoadFromFile : public Action {
//     virtual ~LoadFromFile=() = default;
// };

} // namespace GN::rg
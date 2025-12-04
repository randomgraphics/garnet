#pragma once

#include <garnet/GNbase.h>

namespace GN {
namespace engine {

// Render graph is a directed acyclic graph of tasks.
// Each task is an atomic self contained operation with deterministic input and output.
// Input and output are render artifacts managed by artifact database, including textures, buffers, GPU pipelines, pure CPU data blobs and etc.
//  all artifacts are uniquely identified by their name and type.
// The render graph has 3 main execution phases:
//  1. collect phase:
//     - collecting all rendering operations and their dependencies.
//  2. compile phase:
//     - compiling all rendering operations to generate rendering command lists.
//  3. execute phase:
//     - submit and execute all rendercing command lists

namespace rg {

/// Artifact represents an atomic resource that can be used as input or output of a task.
struct Artifact {
    enum Type {
        CPU_BLOB,
        GPU_TEXTURE,
        GPU_BUFFER,
        GPU_PIPELINE,
    };

    struct Identification {
        StrA name;
        Type type;
    };

    const Identification id;
    const uint64_t       number;

    /// Constructor
    Artifact(const StrA & name, Type type);

    virtual ~Artifact() = default;
};

struct ArtifactDatabase {
    virtual Artifact * create(const Artifact::Identification &) = 0;
    virtual Artifact * search(const Artifact::Identification &) = 0;
    virtual Artifact * search(uint64_t) = 0;
};

/// Base class of all tasks.
struct Task {
    StrA uniqueName;
    DynaArray<Artifact::Identification> inputs;
    DynaArray<Artifact::Identification> outputs;
    std::function<void(Task &)> executor;
};

struct RenderGraph {
    virtual void collect(const Task &) = 0;
    
    // this function will lock downe render graph that no more tasks can be added or altered during compliation.
    virtual void compile() = 0;

    virtual void execute() = 0;
};



} // namespace engine
} // namespace GN


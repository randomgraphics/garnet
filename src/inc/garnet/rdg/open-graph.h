#pragma once

#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace GN::rdg2 {

using GN::rdg::RttiBaseTypeIds;

/// A counter that never overflows.
///
/// This is internally a 128-bit counter. Even if you increment it a billion times per second,
/// it'll take over 5 sextillion (10^21) years to overflow, which is 700 billion times longer than
/// the age of our universe.
///
/// It is overkill for almost all practical use cases. Just create this class for fun and learning purposes.
struct NeverOverflowingCounter {
    uint64_t value0 = 0;
    uint64_t value1 = 0;

    static inline constexpr NeverOverflowingCounter OOO() { return NeverOverflowingCounter {0, 0}; }
    static inline constexpr NeverOverflowingCounter ONE() { return NeverOverflowingCounter {1, 0}; }
    static inline constexpr NeverOverflowingCounter INF() {
        return NeverOverflowingCounter {std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max()};
    }

    NeverOverflowingCounter & increment() {
        if (++value0 == 0) { ++value1; }
        return *this;
    }

    NeverOverflowingCounter & decrement() {
        if (--value0 == std::numeric_limits<uint64_t>::max()) { --value1; }
        return *this;
    }

    NeverOverflowingCounter & reset() {
        value0 = 0;
        value1 = 0;
        return *this;
    }

    bool operator==(const NeverOverflowingCounter & other) const { return value0 == other.value0 && value1 == other.value1; }

    bool operator!=(const NeverOverflowingCounter & other) const { return !(*this == other); }

    bool operator<(const NeverOverflowingCounter & other) const {
        if (value1 != other.value1) return value1 < other.value1;
        return value0 < other.value0;
    }

    bool operator>(const NeverOverflowingCounter & other) const { return other < *this; }

    bool operator<=(const NeverOverflowingCounter & other) const { return !(*this > other); }

    bool operator>=(const NeverOverflowingCounter & other) const { return !(*this < other); }
};

// ============================================================
// Base class of everything with a ID and name.
// ============================================================

/// The basic building block of the render graph module. Base class of everything that
/// needs reference counting and runtime type information.
struct Entity : public RefCounter, public GN::rdg::RuntimeType {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE();

    /// ID of the entity. Guaranteed to be unique within the process.
    const NeverOverflowingCounter id;

    /// Name of the entity. Optional. No uniqueness requirement.
    const StrA name;

    virtual ~Entity() = default;

protected:
    /// Constructor
    GN_API Entity(const GN::rdg::RuntimeType::TypeInfo & type, const StrA & name);
};

// ============================================================
// Artifact. Represents certain resource or data with a version
// ============================================================

struct Artifact : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

    /// The version of this artifact. Version ZERO is the initial version,
    /// indicating that the artifact is not yet published.
    NeverOverflowingCounter version = NeverOverflowingCounter::OOO();

    /// The content of the artifact.
    AutoRef<Entity> content = nullptr;

    bool publish(NeverOverflowingCounter newVersion, AutoRef<Entity> newContent) {
        if (newVersion <= version) {
            // can't roll back to a lower version
            return false;
        }
        version = newVersion;
        content = std::move(newContent);
        return true;
    }

    bool publish(AutoRef<Entity> newContent) {
        auto newVersion = version;
        newVersion.increment();
        return publish(newVersion, std::move(newContent));
    }

protected:
    using Entity::Entity;
};
using ArtifactPtr = AutoRef<Artifact>;

// ============================================================
// Arguments. Parameters passed to an action. Usually a collection of artifacts.
//
// The arguments are usually passed to the action as a single entity, but they can be
// split into multiple artifacts. For example, a shader action may have a vertex shader
// and a pixel shader as arguments.
// ============================================================

struct Arguments : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

protected:
    using Entity::Entity;
};
using ArgumentsPtr = AutoRef<Arguments>;

// ============================================================
// Action. Represents an operation that can be executed.
// ============================================================

struct Action : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

    /// The graph does not differentiate between successful and failed execution.
    /// That is the internal state of the action.
    ///
    /// For example, if an action failed on its first attempt, it may elect to
    /// retry by adding a child node to itself and re-execute it. Or it may elect to
    /// store the failed result somewhere and let its dependents to respond to the failure.
    virtual void execute(const Arguments & arguments) = 0;

protected:
    using Entity::Entity;
};
using ActionPtr = AutoRef<Action>;

// ============================================================
// Scheduling
// ============================================================

/// Lower numeric value means higher scheduling priority (runs earlier).
enum class SchedulingClass : uint8_t { Critical = 0, Normal = 1, Background = 2, Idle = 3 };

struct SchedulingHints {
    SchedulingClass schedulingClass = SchedulingClass::Normal;
    int32_t         priority        = 0;
};

// ============================================================
// Forward declarations
// ============================================================

struct Node;
using NodePtr = AutoRef<Node>;

template<typename T>
using ArrayContainer = DynaArray<T, size_t>;

// ============================================================
// Token
// ============================================================

/// Generic satisfiable prerequisite / milestone.
/// May be satisfied internally or externally.
struct Token : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

    bool                    satisfied = false;
    ArrayContainer<NodePtr> waiters;

    /// Constructor
    Token(const StrA & name): Entity(TYPE_INFO(), name) {}
};
using TokenPtr = AutoRef<Token>;

// ============================================================
// Published artifact identity (artifact + version counter)
// ============================================================

struct ArtifactVersion {
    ArtifactPtr             artifact;
    NeverOverflowingCounter version = NeverOverflowingCounter::OOO();

    bool operator==(const ArtifactVersion & other) const { return artifact == other.artifact && version == other.version; }

    bool operator!=(const ArtifactVersion & other) const { return !(*this == other); }
};

// ============================================================
// Dependencies / outputs
// ============================================================

enum class DependencyKind : uint8_t {
    /// Depends on a token.
    Token,

    /// Depends on a specific published version (artifact + version counter).
    ArtifactExactVersion,

    /// Depends on whatever version is latest at dependency resolution time (snapshot).
    LatestPublishedSnapshot,
};

struct Dependency {
    DependencyKind kind = DependencyKind::Token;

    TokenPtr                token    = nullptr;
    ArtifactPtr             artifact = nullptr;
    NeverOverflowingCounter version  = NeverOverflowingCounter::INF();

    static Dependency onToken(TokenPtr t) {
        Dependency d;
        d.kind  = DependencyKind::Token;
        d.token = std::move(t);
        return d;
    }

    static Dependency onArtifactVersion(ArtifactPtr art, NeverOverflowingCounter ver) {
        Dependency d;
        d.kind     = DependencyKind::ArtifactExactVersion;
        d.artifact = std::move(art);
        d.version  = ver;
        return d;
    }

    /// Snapshot semantics: resolved to currently published concrete version at insertion time.
    static Dependency onLatestPublishedSnapshot(ArtifactPtr art) {
        Dependency d;
        d.kind     = DependencyKind::LatestPublishedSnapshot;
        d.artifact = std::move(art);
        return d;
    }

    /// Resolve to the next published version at insertion time.
    static Dependency onNextPublishedVersion(ArtifactPtr art) {
        Dependency d;
        d.kind     = DependencyKind::ArtifactExactVersion;
        d.artifact = art;
        d.version  = art->version;
        d.version.increment();
        return d;
    }
};

// ============================================================
// Node state / completion model
// ============================================================

enum class NodeState : uint8_t { Created, Blocked, Ready, Running, Completed, Failed, Cancelled };

enum class CompletionPolicy : uint8_t {
    /// Graph completes the node when its own action and children (if any) are done per tryAutoComplete.
    Automatic,

    /// Graph never auto-completes the node. Host must explicitly complete it.
    Manual,

    /// Complete when own action is done (ignores children except as needed for other policies).
    WhenOwnActionCompletes,

    /// Complete when own action is done, children are sealed, and liveChildren == 0.
    WhenSubtreeCompletes,
};

// ============================================================
// Outputs (satisfied when node reaches terminal Completed via graph)
// ============================================================

enum class OutputKind : uint8_t { SatisfyToken, PublishArtifactVersion };

struct OutputSpec {
    OutputKind      kind = OutputKind::SatisfyToken;
    TokenPtr        token;
    ArtifactVersion artifactVersion;
};

// ============================================================
// Node description
// ============================================================

struct NodeDesc {
    StrA                       name                         = StrA::EMPTYSTR();
    StrA                       debugName                    = StrA::EMPTYSTR();
    ActionPtr                  action                       = nullptr;
    ArgumentsPtr               arguments                    = nullptr;
    ArrayContainer<Dependency> dependencies                 = {};
    ArrayContainer<OutputSpec> outputs                      = {};
    SchedulingHints            scheduling                   = {};
    NodePtr                    parent                       = nullptr;
    CompletionPolicy           completionPolicy             = CompletionPolicy::Automatic;
    bool                       autoSatisfyOutputsOnComplete = true;

    NodeDesc()                                   = default;
    NodeDesc(const NodeDesc & other)             = default;
    NodeDesc(NodeDesc && other)                  = default;
    NodeDesc & operator=(const NodeDesc & other) = default;
    NodeDesc & operator=(NodeDesc && other)      = default;
};

// ============================================================
// Node
// ============================================================

struct Node : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

    const NodeDesc & desc() const { return mDesc; }

    // For subtree completion policy.
    ArrayContainer<NodePtr> children;
    uint32_t                liveChildren   = 0;
    bool                    sealedChildren = false;

    TokenPtr completionToken = nullptr;

    uint32_t unresolvedDependencies = 0;

    NodeState state = NodeState::Created;

    // Own-action lifecycle.
    bool ownActionStarted  = false;
    bool ownActionFinished = false;

    /// Constructor
    Node(const StrA & name, const NodeDesc & desc): Entity(TYPE_INFO(), name), mDesc(desc) {}

    StrA effectiveDebugName() const { return mDesc.debugName.empty() ? name : mDesc.debugName; }

private:
    NodeDesc mDesc;
};

// ============================================================
// Execution API
// ============================================================

struct NodeExecutionResult {
    NodeState                       terminalState = NodeState::Completed;
    ArrayContainer<NodeDesc>        spawnedChildren;
    ArrayContainer<TokenPtr>        additionalSatisfiedTokens;
    ArrayContainer<ArtifactVersion> additionalPublishedVersions;
};

struct ReadyNode {
    NodePtr         node;
    StrA            debugName;
    ActionPtr       action;
    ArgumentsPtr    arguments;
    SchedulingHints scheduling;
};

// ============================================================
// Graph (abstract)
// ============================================================

class Graph : public RefCounter {
public:
    ~Graph() override = default;

    Graph(const Graph &)             = delete;
    Graph & operator=(const Graph &) = delete;

    GN_API static AutoRef<Graph> create();

    // --------------------------------------------------------
    // Object creation
    // --------------------------------------------------------

    virtual TokenPtr createToken(const StrA & name = StrA::EMPTYSTR()) = 0;

    virtual NodePtr addNode(const StrA & name, const NodeDesc & desc) = 0;

    virtual ArrayContainer<NodePtr> addNodes(ArrayContainer<NodeDesc> descs) = 0;

    // --------------------------------------------------------
    // Child scope control
    // --------------------------------------------------------

    /// Once sealed, a subtree-completing node may complete when its liveChildren hits 0.
    virtual void sealChildren(NodePtr node) = 0;

    // --------------------------------------------------------
    // Version / publication
    // --------------------------------------------------------

    virtual bool publishArtifactVersion(ArtifactVersion version) = 0;

    virtual std::optional<ArtifactVersion> getLatestPublishedVersion(ArtifactPtr artifact) const = 0;

    virtual TokenPtr getOrCreateArtifactVersionToken(ArtifactVersion version) = 0;

    // --------------------------------------------------------
    // External completion
    // --------------------------------------------------------

    virtual bool satisfyToken(TokenPtr token) = 0;

    // --------------------------------------------------------
    // Ready frontier
    // --------------------------------------------------------

    virtual bool hasReadyNode() const = 0;

    virtual bool tryPopReadyNode(ReadyNode & out) = 0;

    // --------------------------------------------------------
    // Completion / failure / cancellation
    // --------------------------------------------------------

    virtual bool completeNode(NodePtr node, const NodeExecutionResult & result = {}) = 0;

    virtual bool failNode(NodePtr node) = 0;

    virtual bool cancelNode(NodePtr node) = 0;

    /// For manual completion policy.
    virtual bool markNodeCompleted(NodePtr node) = 0;

protected:
    Graph() = default;
};

} // namespace GN::rdg2

#pragma once

#include <garnet/GNbase.h>
#include <garnet/rdg/rtti.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace GN::rgd2 {

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
    static inline constexpr NeverOverflowingCounter INF() { return NeverOverflowingCounter {std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max()}; }

    void increment() {
        if (++value0 == 0) {
            ++value1;
        }
    }

    void decrement() {
        if (--value0 == std::numeric_limits<uint64_t>::max()) {
            --value1;
        }
    }

    void reset() {
        value0 = 0;
        value1 = 0;
    }

    bool operator==(const NeverOverflowingCounter & other) const {
        return value0 == other.value0 && value1 == other.value1;
    }

    bool operator!=(const NeverOverflowingCounter & other) const {
        return !(*this == other);
    }

    bool operator<(const NeverOverflowingCounter & other) const {
        if (value1 != other.value1) return value1 < other.value1;
        return value0 < other.value0;
    }

    bool operator>(const NeverOverflowingCounter & other) const {
        return other < *this;
    }

    bool operator<=(const NeverOverflowingCounter & other) const {
        return !(*this > other);
    }

    bool operator>=(const NeverOverflowingCounter & other) const {
        return !(*this < other);
    }
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
    GN_API Entity(const TypeInfo & type, const StrA & name);
};

// ============================================================
// Artifact. Represents certain resource or data with a version
// ============================================================

struct Artifact : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

    /// The version of this artifact.
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

struct ArtifactVersion {

    /// The expected version of the artifact. Set to INF for the latest published version.
    NeverOverflowingCounter version = NeverOverflowingCounter::INF();
};

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

enum class SchedulingPriority : uint8_t { Critical, Normal, Background, Idle };

struct SchedulingHints {
    SchedulingPriority priority = SchedulingPriority::Normal;
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
struct Token : Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

    bool                    satisfied = false;
    ArrayContainer<NodePtr> waiters;

protected:
    using Entity::Entity;
};
using TokenPtr = AutoRef<Token>;

// ============================================================
// Dependencies / outputs
// ============================================================

enum class DependencyKind : uint8_t {
    /// Depends on a token.
    Token,

    /// Depends on a specific (or latest published) version of an artifact.
    Artifact,
};

struct Dependency {
    DependencyKind kind = DependencyKind::Token;

    TokenPtr   token = nullptr;
    ArtifactPtr      artifact {};

    static Dependency onToken(TokenPtr t) {
        Dependency d;
        d.kind  = DependencyKind::Token;
        d.token = t;
        return d;
    }

    static Dependency onArtifactVersion(ArtifactPtr artifact, uint64_t version) {
        Dependency d;
        d.kind            = DependencyKind::ArtifactVersion;
        d.artifactVersion = ArtifactVersion {artifact, version};
        return d;
    }

    // Snapshot semantics:
    // resolved to currently published concrete version at insertion time.
    static Dependency onLatestPublishedSnapshot(ArtifactPtr artifact) {
        Dependency d;
        d.kind           = DependencyKind::LatestPublishedSnapshot;
        d.latestArtifact = artifact;
        return d;
    }
};

enum class OutputKind : uint8_t { Token, PublishArtifactVersion };

struct OutputSpec {
    OutputKind kind = OutputKind::Token;

    TokenPtr         token = nullptr;
    ArtifactVersion artifactVersion {};

    static OutputSpec producesToken(TokenPtr t) {
        OutputSpec o;
        o.kind  = OutputKind::Token;
        o.token = t;
        return o;
    }

    static OutputSpec publishesArtifactVersion(ArtifactPtr artifact, uint64_t version) {
        OutputSpec o;
        o.kind            = OutputKind::PublishArtifactVersion;
        o.artifactVersion = ArtifactVersion {artifact, version};
        return o;
    }
};

// ============================================================
// Node state / completion model
// ============================================================

enum class NodeState : uint8_t { Created, Blocked, Ready, Running, Completed, Cancelled, Failed };

enum class CompletionPolicy : uint8_t {
    // Node completes when its own action finishes.
    // Children do not block parent completion.
    WhenOwnActionCompletes,

    // Node completes when:
    // - its own action is done (or there is no action)
    // - and all children are done
    //
    // This is the "node can be a group" mode.
    WhenSubtreeCompletes,

    // Graph never auto-completes the node.
    // Host/domain layer must explicitly complete it.
    Manual
};

// ============================================================
// Node description
// ============================================================

struct NodeDesc {
    std::string debugName;

    ActionPtr    action;
    ArgumentsPtr arguments;

    ArrayContainer<Dependency> dependencies;
    ArrayContainer<OutputSpec> outputs;

    SchedulingHints scheduling {};

    NodePtr           parent           = nullptr;
    CompletionPolicy  completionPolicy = CompletionPolicy::WhenOwnActionCompletes;

    // If true, graph creates a completion token for this node and satisfies it
    // when node reaches Completed.
    bool autoCreateCompletionToken = true;

    // If true, graph satisfies declared outputs automatically when node completes.
    bool autoSatisfyOutputsOnComplete = true;
};

// ============================================================
// Node
// ============================================================
//
// Unified concept:
// - leaf task
// - planner
// - compound node
// - former "group"
//
// A node may have:
// - own action
// - children
// - subtree completion semantics
//

struct Node : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

    ActionPtr    action;
    ArgumentsPtr arguments;

    SchedulingHints  scheduling {};
    CompletionPolicy completionPolicy = CompletionPolicy::WhenOwnActionCompletes;

    NodePtr              parent = nullptr;
    ArrayContainer<NodePtr> children;

    ArrayContainer<Dependency> originalDependencies;
    ArrayContainer<OutputSpec> outputs;

    TokenPtr completionToken              = nullptr;
    bool    autoSatisfyOutputsOnComplete = true;

    uint32_t  unresolvedDependencies = 0;
    NodeState state                  = NodeState::Created;

    // For subtree completion policy.
    uint32_t liveChildren   = 0;
    bool     sealedChildren = false;

    // Own-action lifecycle.
    bool ownActionStarted  = false;
    bool ownActionFinished = false;
};

// ============================================================
// Node execution result
// ============================================================

struct NodeExecutionResult {
    NodeState terminalState = NodeState::Completed;

    // Optional extra satisfactions beyond declared outputs.
    ArrayContainer<TokenPtr>         additionalSatisfiedTokens;
    ArrayContainer<ArtifactVersion>  additionalPublishedVersions;

    // Optional spawned children.
    ArrayContainer<NodeDesc> spawnedChildren;
};

// ============================================================
// Graph
// ============================================================

class Graph {
public:
    Graph()  = default;
    ~Graph() = default;

    Graph(const Graph &)             = delete;
    Graph & operator=(const Graph &) = delete;

    // --------------------------------------------------------
    // Object creation
    // --------------------------------------------------------

    TokenPtr createToken(std::string debugName = {}) {
        auto tok       = std::make_unique<Token>();
        tok->debugId   = ++m_nextTokenId;
        tok->debugName = std::move(debugName);

        TokenPtr out = tok.get();
        m_tokens.emplace_back(std::move(tok));
        return out;
    }

    NodePtr addNode(NodeDesc desc) {
        auto node                          = std::make_unique<Node>();
        node->debugId                      = ++m_nextNodeId;
        node->debugName                    = std::move(desc.debugName);
        node->action                       = std::move(desc.action);
        node->arguments                    = std::move(desc.arguments);
        node->scheduling                   = desc.scheduling;
        node->completionPolicy             = desc.completionPolicy;
        node->parent                       = desc.parent;
        node->originalDependencies         = std::move(desc.dependencies);
        node->outputs                      = std::move(desc.outputs);
        node->autoSatisfyOutputsOnComplete = desc.autoSatisfyOutputsOnComplete;
        node->state                        = NodeState::Created;

        if (desc.autoCreateCompletionToken) { node->completionToken = createToken("NodeCompletion:" + node->debugName); }

        NodePtr out = node.get();
        m_nodes.emplace_back(std::move(node));

        if (out->parent) {
            out->parent->children.push_back(out);
            ++out->parent->liveChildren;
        }

        out->unresolvedDependencies = resolveDependencies(*out);
        out->state                  = (out->unresolvedDependencies == 0) ? NodeState::Ready : NodeState::Blocked;

        if (out->state == NodeState::Ready) { pushReady(out); }

        return out;
    }

    ArrayContainer<NodePtr> addNodes(ArrayContainer<NodeDesc> descs) {
        ArrayContainer<NodePtr> out;
        out.reserve(descs.size());

        for (auto & d : descs) { out.push_back(addNode(std::move(d))); }

        return out;
    }

    // --------------------------------------------------------
    // Child scope control
    // --------------------------------------------------------

    // Once sealed, a subtree-completing node may complete when its liveChildren hits 0.
    void sealChildren(NodePtr node) {
        if (!node) return;

        node->sealedChildren = true;
        tryAutoComplete(node);
    }

    // --------------------------------------------------------
    // Version / publication
    // --------------------------------------------------------

    bool publishArtifactVersion(ArtifactVersion version) {
        auto & slot = findOrCreatePublishedSlot(version.artifact);

        if (slot.has_value() && slot->version >= version.version) return false;

        slot = version;

        TokenPtr versionToken = getOrCreateArtifactVersionToken(version);
        return satisfyToken(versionToken);
    }

    std::optional<ArtifactVersion> getLatestPublishedVersion(ArtifactPtr artifact) const {
        for (const auto & p : m_publishedArtifacts) {
            if (p.artifact == artifact) return p.version;
        }
        return std::nullopt;
    }

    TokenPtr getOrCreateArtifactVersionToken(ArtifactVersion version) {
        for (auto & p : m_artifactVersionTokens) {
            if (p.version == version) return p.token;
        }

        TokenPtr t = createToken("ArtifactVersion");
        m_artifactVersionTokens.push_back({version, t});
        return t;
    }

    // --------------------------------------------------------
    // External completion
    // --------------------------------------------------------

    bool satisfyToken(TokenPtr token) {
        if (!token || token->satisfied) return false;

        token->satisfied = true;

        for (NodePtr waiter : token->waiters) {
            if (!waiter) continue;
            if (waiter->state != NodeState::Blocked) continue;

            if (waiter->unresolvedDependencies > 0) {
                --waiter->unresolvedDependencies;
                if (waiter->unresolvedDependencies == 0) {
                    waiter->state = NodeState::Ready;
                    pushReady(waiter);
                }
            }
        }

        return true;
    }

    // --------------------------------------------------------
    // Ready frontier
    // --------------------------------------------------------

    bool hasReadyNode() const { return !m_ready.empty(); }

    bool tryPopReadyNode(ReadyNode & out) {
        while (!m_ready.empty()) {
            ReadyQueueEntry e = m_ready.top();
            m_ready.pop();

            NodePtr node = e.node;
            if (!node) continue;

            if (node->state != NodeState::Ready) continue;

            node->state            = NodeState::Running;
            node->ownActionStarted = true;

            out.node       = node;
            out.debugName  = node->debugName;
            out.action     = node->action;
            out.arguments  = node->arguments;
            out.scheduling = node->scheduling;
            return true;
        }

        return false;
    }

    // --------------------------------------------------------
    // Completion / failure / cancellation
    // --------------------------------------------------------

    bool completeNode(NodePtr node, const NodeExecutionResult & result = {}) {
        if (!node) return false;

        if (node->state != NodeState::Running && node->state != NodeState::Ready && node->state != NodeState::Blocked) { return false; }

        node->ownActionFinished = true;

        for (auto child : result.spawnedChildren) {
            child.parent = node;
            addNode(std::move(child));
        }

        if (result.terminalState == NodeState::Completed) {
            // Satisfy declared outputs and extras only when node truly reaches terminal Completed.
            // For subtree policy, that may happen now or later.
            node->state = NodeState::Running;
            m_pendingCompletionExtras.push_back({node, result});
            tryAutoComplete(node);
            return true;
        }

        node->state = result.terminalState;
        onNodeBecameTerminal(node);
        return true;
    }

    bool failNode(NodePtr node) {
        if (!node) return false;

        if (isTerminal(node->state)) return false;

        node->state             = NodeState::Failed;
        node->ownActionFinished = true;
        onNodeBecameTerminal(node);
        return true;
    }

    bool cancelNode(NodePtr node) {
        if (!node) return false;

        if (isTerminal(node->state)) return false;

        node->state             = NodeState::Cancelled;
        node->ownActionFinished = true;
        onNodeBecameTerminal(node);
        return true;
    }

    // For manual completion policy.
    bool markNodeCompleted(NodePtr node) {
        if (!node) return false;
        if (isTerminal(node->state)) return false;

        node->ownActionFinished = true;
        node->state             = NodeState::Completed;

        finalizeCompletion(node, nullptr);
        return true;
    }

private:
    struct PublishedArtifactEntry {
        ArtifactPtr                     artifact {};
        std::optional<ArtifactVersion> version;
    };

    struct ArtifactVersionTokenEntry {
        ArtifactVersion version {};
        TokenPtr         token = nullptr;
    };

    struct PendingCompletionExtras {
        NodePtr              node = nullptr;
        NodeExecutionResult result;
    };

    struct ReadyQueueEntry {
        NodePtr          node = nullptr;
        SchedulingHints hints {};
        uint64_t        enqueueOrdinal = 0;
    };

    struct ReadyQueueCompare {
        bool operator()(const ReadyQueueEntry & a, const ReadyQueueEntry & b) const {
            if (a.hints.schedulingClass != b.hints.schedulingClass) {
                return static_cast<int>(a.hints.schedulingClass) > static_cast<int>(b.hints.schedulingClass);
            }

            if (a.hints.priority != b.hints.priority) { return a.hints.priority < b.hints.priority; }

            return a.enqueueOrdinal > b.enqueueOrdinal;
        }
    };

private:
    static bool isTerminal(NodeState s) { return s == NodeState::Completed || s == NodeState::Cancelled || s == NodeState::Failed; }

    uint32_t resolveDependencies(Node & node) {
        uint32_t unresolved = 0;

        for (const Dependency & dep : node.originalDependencies) {
            TokenPtr token = nullptr;

            switch (dep.kind) {
            case DependencyKind::Token:
                token = dep.token;
                break;

            case DependencyKind::ArtifactVersion:
                token = getOrCreateArtifactVersionToken(dep.artifactVersion);
                break;

            case DependencyKind::LatestPublishedSnapshot: {
                auto latest = getLatestPublishedVersion(dep.latestArtifact);
                if (!latest.has_value()) {
                    // No published version yet => treat as satisfied by snapshot policy.
                    continue;
                }
                token = getOrCreateArtifactVersionToken(*latest);
                break;
            }
            }

            if (!token) continue;

            if (!token->satisfied) {
                ++unresolved;
                token->waiters.push_back(&node);
            }
        }

        return unresolved;
    }

    void pushReady(NodePtr node) {
        ReadyQueueEntry e;
        e.node           = node;
        e.hints          = node->scheduling;
        e.enqueueOrdinal = ++m_enqueueOrdinal;
        m_ready.push(e);
    }

    void tryAutoComplete(NodePtr node) {
        if (!node) return;

        if (node->completionPolicy == CompletionPolicy::Manual) return;

        const bool ownDone = (!node->action) || node->ownActionFinished;

        bool subtreeDone = true;
        if (node->completionPolicy == CompletionPolicy::WhenSubtreeCompletes) { subtreeDone = node->sealedChildren && (node->liveChildren == 0); }

        if (node->completionPolicy == CompletionPolicy::WhenOwnActionCompletes) {
            if (!ownDone) return;
        } else if (node->completionPolicy == CompletionPolicy::WhenSubtreeCompletes) {
            if (!ownDone || !subtreeDone) return;
        }

        if (node->state == NodeState::Completed) return;
        if (node->state == NodeState::Cancelled || node->state == NodeState::Failed) return;

        node->state = NodeState::Completed;
        finalizeCompletion(node, findPendingCompletionExtras(node));
    }

    void finalizeCompletion(NodePtr node, PendingCompletionExtras * pending) {
        if (node->completionToken) { satisfyToken(node->completionToken); }

        if (node->autoSatisfyOutputsOnComplete) {
            for (const OutputSpec & output : node->outputs) {
                switch (output.kind) {
                case OutputKind::Token:
                    satisfyToken(output.token);
                    break;
                case OutputKind::PublishArtifactVersion:
                    publishArtifactVersion(output.artifactVersion);
                    break;
                }
            }
        }

        if (pending) {
            for (TokenPtr t : pending->result.additionalSatisfiedTokens) { satisfyToken(t); }

            for (const ArtifactVersion & v : pending->result.additionalPublishedVersions) { publishArtifactVersion(v); }

            removePendingCompletionExtras(node);
        }

        onNodeBecameTerminal(node);
    }

    void onNodeBecameTerminal(NodePtr node) {
        if (!node || !node->parent) return;

        if (node->parent->liveChildren > 0) { --node->parent->liveChildren; }

        tryAutoComplete(node->parent);
    }

    PublishedArtifactEntry & findOrCreatePublishedSlot(ArtifactPtr artifact) {
        for (auto & e : m_publishedArtifacts) {
            if (e.artifact == artifact) return e;
        }

        m_publishedArtifacts.push_back({artifact, std::nullopt});
        return m_publishedArtifacts.back();
    }

    PendingCompletionExtras * findPendingCompletionExtras(NodePtr node) {
        for (auto & p : m_pendingCompletionExtras) {
            if (p.node == node) return &p;
        }
        return nullptr;
    }

    void removePendingCompletionExtras(NodePtr node) {
        for (size_t i = 0; i < m_pendingCompletionExtras.size(); ++i) {
            if (m_pendingCompletionExtras[i].node == node) {
                m_pendingCompletionExtras.erase(m_pendingCompletionExtras.begin() + static_cast<std::ptrdiff_t>(i));
                return;
            }
        }
    }

private:
    uint64_t m_nextNodeId     = 0;
    uint64_t m_nextTokenId    = 0;
    uint64_t m_enqueueOrdinal = 0;

    ArrayContainer<std::unique_ptr<Node>>  m_nodes;
    ArrayContainer<std::unique_ptr<Token>> m_tokens;

    ArrayContainer<PublishedArtifactEntry>    m_publishedArtifacts;
    ArrayContainer<ArtifactVersionTokenEntry> m_artifactVersionTokens;
    ArrayContainer<PendingCompletionExtras>   m_pendingCompletionExtras;

    std::priority_queue<ReadyQueueEntry, ArrayContainer<ReadyQueueEntry>, ReadyQueueCompare> m_ready;
};

} // namespace og
#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/open-graph.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

#include <cstdint>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace GN::rdg2 {

// ============================================================
// Action. Represents an operation that can be executed.
// ============================================================

namespace detail {

template<typename FUNC>
concept ZeroArgumentCallable = std::is_invocable_v<std::decay_t<FUNC> &>;

} // namespace detail

/// Unit of work the graph executes; implementations own how success, failure, and retries are represented.
struct Action : public Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

    template<detail::ZeroArgumentCallable FUNC>
    static AutoRef<Action> createFromLambda(const StrA & name, FUNC && f) {
        using F = std::decay_t<FUNC>;
        struct LambdaAction : public Action {
            F    func;
            void execute() override { func(); }
            LambdaAction(const StrA & n, F && fn): Action(Action::TYPE_INFO(), n), func(std::move(fn)) {}
        };
        return GN::referenceTo(static_cast<Action *>(new LambdaAction(name, F(std::forward<FUNC>(f)))));
    }

    /// The graph does not differentiate between successful and failed execution.
    /// That is the internal state of the action.
    ///
    /// For example, if an action failed on its first attempt, it may elect to
    /// retry by adding a child node to itself and re-execute it. Or it may elect to
    /// store the failed result somewhere and let its dependents to respond to the failure.
    virtual void execute() = 0;

protected:
    using Entity::Entity;
};
using ActionPtr = AutoRef<Action>;

// ============================================================
// Graph handles
// ============================================================

/// Dependency handle; satisfied when the conditions the graph associates with it are met.
///
/// The public type intentionally exposes only Entity identity/name/type. Concrete graph state
/// remains private to the open-graph implementation.
struct Token : public Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

protected:
    using Entity::Entity;
};
using TokenPtr = AutoRef<Token>;

/// Handle to a node in the graph (placement in the DAG, completion, tokens).
///
/// The public type intentionally exposes only Entity identity/name/type. Concrete graph state
/// remains private to the open-graph implementation.
struct Node : public Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

protected:
    using Entity::Entity;
};
using NodePtr = AutoRef<Node>;

// ============================================================
// Scheduling
// ============================================================

/// Coarse scheduling band. Lower numeric value means higher priority (scheduled earlier).
enum class SchedulingClass : uint8_t {
    CRITICAL   = 0, /// Run before normal interactive work.
    NORMAL     = 1, /// Default band for typical graph nodes.
    BACKGROUND = 2, /// Lower urgency than Normal.
    IDLE       = 3, /// Lowest urgency; run when little else is pending.
};

/// Fine-grained ordering hints for the scheduler within a scheduling class.
struct SchedulingHints {
    /// Which band this node belongs to.
    SchedulingClass schedulingClass = SchedulingClass::NORMAL;
    /// Secondary ordering within the same SchedulingClass; lower runs earlier.
    int32_t priority = 0;
};

// ============================================================
// Node description
// ============================================================

/// Description passed to Graph::addNode: what to run, what it depends on, and how it is scheduled.
struct NodeDesc {
    /// Human-readable name for debugging and logging; not required to be unique.
    StrA name = StrA::EMPTYSTR();

    /// (Optional) action invoked when this node runs.
    ActionPtr action = nullptr;

    /// Tokens that must be satisfied before this node may run.
    DynaArray<TokenPtr> dependencies = {};

    /// Scheduling band and intra-band priority.
    SchedulingHints scheduling = {};

    /// Optional parent node for hierarchical grouping or lifetime relationships.
    NodePtr parent = nullptr;

    /// If true, the node does not complete until completeNode() is called after its action finishes.
    bool manualComplete = false;

    NodeDesc(const StrA & name_): name(name_) {}

    /// Create a node and wrap a zero-argument callable as its action.
    template<detail::ZeroArgumentCallable FUNC>
    NodeDesc(const StrA & name_, FUNC && f): name(name_), action(Action::createFromLambda(name_, std::forward<FUNC>(f))) {}

    NodeDesc(const NodeDesc & other)             = default;
    NodeDesc(NodeDesc && other)                  = default;
    NodeDesc & operator=(const NodeDesc & other) = default;
    NodeDesc & operator=(NodeDesc && other)      = default;

    NodeDesc & setAction(ActionPtr act) {
        action = act;
        return *this;
    }

    NodeDesc & dependsOn(const TokenPtr & t) {
        if (t) (void) dependencies.append(t);
        return *this;
    }
};

// ============================================================
// Graph (abstract)
// ============================================================

using ArtifactPtr = AutoRef<Artifact>;

/// Abstract render-graph executor: artifacts, nodes, tokens, and completion/wait APIs.
class Graph : public RefCounter {
public:
    ~Graph() override = default;

    Graph(const Graph &)             = delete;
    Graph & operator=(const Graph &) = delete;

    GN_API static AutoRef<Graph> create();

    // --------------------------------------------------------
    // Graph status query
    // --------------------------------------------------------

    /// Result of a bounded wait on graph or token quiescence.
    enum class WaitResult : uint8_t {
        IDLE,   /// Condition met within the timeout (e.g. graph idle or token satisfied).
        BUSY,   /// Timed out or polled with zero timeout while work or the waited condition was still pending.
        FAILED, /// Unrecoverable error while waiting (implementation-defined).
    };

    /// Block until the graph has no pending work, or until \p timeout elapses.
    virtual WaitResult waitForIdle(std::chrono::milliseconds timeout = std::chrono::milliseconds::max()) const = 0;

    /// Non-blocking check: true iff waitForIdle(zero) would return IDLE.
    bool isIdle() const { return waitForIdle(std::chrono::milliseconds::zero()) == WaitResult::IDLE; }

    /// Wait until \p token is satisfied. Outside node execution this blocks until satisfied or graph shutdown.
    /// From inside an executing node, does not block: returns IDLE if already satisfied, otherwise BUSY.
    virtual WaitResult waitForToken(const TokenPtr & token) const = 0;

    // --------------------------------------------------------
    // Artifact management
    // --------------------------------------------------------

    /// Create a new artifact with no content and version equal to 0.
    virtual ArtifactPtr createArtifact(const StrA & name = StrA::EMPTYSTR()) = 0;

    /// Publish new content for an artifact, increase artifact version number by 1.
    virtual void publishArtifact(const ArtifactPtr & artifact, AutoRef<Entity> content) = 0;

    // --------------------------------------------------------
    // Node management
    // --------------------------------------------------------

    /// Add a new node to the graph for execution.
    virtual NodePtr addNode(const NodeDesc & desc) = 0;

    /// Manually complete a node whose action has already finished.
    ///
    /// This call is only needed for nodes with \c manualComplete set to true. Calling it before the node's action has
    /// finished, or before all of its children complete, is an error and does not complete the node.
    virtual void completeNode(const NodePtr & node) = 0;

    // --------------------------------------------------------
    // Token management
    // --------------------------------------------------------

    /// Get a token satisfied when a node completes.
    /// \param node The node to wait for.
    /// \return The token will be satisfied when the given node completes, automatically or manually.
    virtual TokenPtr getNodeCompletionToken(const NodePtr & node) = 0;

    /// Get an token satisfied when the given artifact version is published.
    /// \param artifact The artifact to wait for.
    /// \param version The version of the artifact to wait for. If set to OOO, then the next published version is used.
    /// \return The token satisfied when the given artifact version is published. Note that if you are asking an
    ///         an version that has been published, the returned token is satisfied immediately.
    virtual TokenPtr getArtifactVersionToken(const ArtifactPtr & artifact, NeverOverflowingCounter version) = 0;

    /// Get a token that will be satisfied when the next version of the given artifact is published.
    /// This is a convenience method for getArtifactVersionToken() with version set to OOO.
    TokenPtr getTokenForNextArtifactVersion(const ArtifactPtr & artifact) { return getArtifactVersionToken(artifact, NeverOverflowingCounter::OOO()); }

    /// Get an token that is satisfied when the given artifact is published at least once.
    /// This is a convenience method for getArtifactVersionToken() with version set to ONE.
    /// After this token is satisfied, it is safe to call Artifact::content() to retrieve the latest content of the artifact.
    TokenPtr getTokenToEnsureArtifactIsPublishedAtLeastOnce(const ArtifactPtr & artifact) {
        return getArtifactVersionToken(artifact, NeverOverflowingCounter::ONE());
    }

protected:
    Graph() = default;
};
using GraphPtr = AutoRef<Graph>;

} // namespace GN::rdg2

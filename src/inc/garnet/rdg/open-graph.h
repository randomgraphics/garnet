#pragma once

#include <garnet/GNbase.h>
#include <garnet/rdg/rtti.h>

#include <cstdint>
#include <chrono>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace GN::rdg2 {

// Unqualified RttiBaseTypeIds in GN_RDG_REGISTER_RUNTIME_TYPE (see rtti.h) must resolve in this namespace.
using GN::rdg::RttiBaseTypeIds;

/// A counter that never overflows.
///
/// This is internally a 128-bit counter. Even if you increment it a billion times per second,
/// it'll take over 5 sextillion (10^21) years to overflow, which is 700 billion times longer than
/// the age of our universe.
///
/// It is overkill for almost all practical use cases. Just create this class for fun and learning purposes.
struct NeverOverflowingCounter {
    /// Low 64 bits of the 128-bit counter.
    uint64_t value0 = 0;
    /// High 64 bits of the 128-bit counter.
    uint64_t value1 = 0;

    /// Zero: default for fresh counters (e.g. artifact version before the first publish).
    /// The same value is also used as a sentinel in APIs such as `Graph::getArtifactVersionToken`
    /// to mean "wait for the next published version" rather than a fixed version number.
    static inline constexpr NeverOverflowingCounter OOO() { return NeverOverflowingCounter {0, 0}; }

    /// Smallest positive value: use when you need an explicit first version (1) after zero,
    /// or a minimal non-zero bound in version logic.
    static inline constexpr NeverOverflowingCounter ONE() { return NeverOverflowingCounter {1, 0}; }

    /// Saturated maximum: use as an upper sentinel so this compares greater than any realistic
    /// version, e.g. open-ended ranges or "no ceiling" in ordering.
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
// Arguments. Parameters passed to an action. Usually a collection of artifacts.
//
// The arguments are usually passed to the action as a single entity, but they can be
// split into multiple artifacts. For example, a shader action may have a vertex shader
// and a pixel shader as arguments.
// ============================================================

/// Parameters passed into an action when it runs, usually as one or more artifacts (e.g. separate VS/PS shader objects).
struct Arguments : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

protected:
    using Entity::Entity;
};
using ArgumentsPtr = AutoRef<Arguments>;

// ============================================================
// Action. Represents an operation that can be executed.
// ============================================================

/// Unit of work the graph executes; implementations own how success, failure, and retries are represented.
struct Action : public Entity {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Entity);

    /// The graph does not differentiate between successful and failed execution.
    /// That is the internal state of the action.
    ///
    /// For example, if an action failed on its first attempt, it may elect to
    /// retry by adding a child node to itself and re-execute it. Or it may elect to
    /// store the failed result somewhere and let its dependents to respond to the failure.
    virtual void execute(Arguments & arguments) = 0;

protected:
    using Entity::Entity;
};
using ActionPtr = AutoRef<Action>;

// ============================================================
// Declare opaque types managed by the graph
// ============================================================

/// Opaque dependency handle; satisfied when the conditions the graph associates with it are met.
struct Token;
/// Opaque handle to a node in the graph (placement in the DAG, completion, tokens).
struct Node;
/// Opaque versioned payload slot; nodes publish and wait on artifact versions via the graph.
struct Artifact;

typedef struct Token *    TokenPtr;
typedef struct Node *     NodePtr;
typedef struct Artifact * ArtifactPtr;

// ============================================================
// Scheduling
// ============================================================

/// Coarse scheduling band. Lower numeric value means higher priority (scheduled earlier).
enum class SchedulingClass : uint8_t {
    Critical   = 0, /// Run before normal interactive work.
    Normal     = 1, /// Default band for typical graph nodes.
    Background = 2, /// Lower urgency than Normal.
    Idle       = 3, /// Lowest urgency; run when little else is pending.
};

/// Fine-grained ordering hints for the scheduler within a scheduling class.
struct SchedulingHints {
    /// Which band this node belongs to.
    SchedulingClass schedulingClass = SchedulingClass::Normal;
    /// Secondary ordering within the same SchedulingClass; lower runs earlier.
    int32_t priority = 0;
};

// ============================================================
// Node description
// ============================================================

template<typename T>
using ArrayContainer = DynaArray<T, size_t>;

/// Description passed to Graph::addNode: what to run, what it depends on, and how it is scheduled.
struct NodeDesc {
    /// Human-readable name for debugging and logging; not required to be unique.
    StrA name = StrA::EMPTYSTR();

    /// Action invoked when this node runs.
    ActionPtr action = nullptr;

    /// Inputs/parameters bound for this invocation of the action.
    ArgumentsPtr arguments = nullptr;

    /// Tokens that must be satisfied before this node may run.
    ArrayContainer<TokenPtr> dependencies = {};

    /// Scheduling band and intra-band priority.
    SchedulingHints scheduling = {};

    /// Optional parent node for hierarchical grouping or lifetime relationships.
    NodePtr parent = nullptr;

    /// If true, the node does not complete until satisfyNode() is called (e.g. waiting on external I/O or a GPU fence).
    bool manualComplete = false;

    NodeDesc()                                   = default;
    NodeDesc(const NodeDesc & other)             = default;
    NodeDesc(NodeDesc && other)                  = default;
    NodeDesc & operator=(const NodeDesc & other) = default;
    NodeDesc & operator=(NodeDesc && other)      = default;
};

// ============================================================
// Graph (abstract)
// ============================================================

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
    virtual WaitResult waitForToken(TokenPtr token) const = 0;

    // --------------------------------------------------------
    // Artifact management
    // --------------------------------------------------------

    /// Create a new artifact with no content and version equal to 0.
    virtual ArtifactPtr createArtifact(const StrA & name = StrA::EMPTYSTR()) = 0;

    /// Publish new content for an artifact, increase artifact version number by 1.
    virtual void publishArtifact(ArtifactPtr artifact, AutoRef<Entity> content) = 0;

    // --------------------------------------------------------
    // Node management
    // --------------------------------------------------------

    /// Add a new node to the graph for execution.
    virtual NodePtr addNode(const NodeDesc & desc) = 0;

    /// Mark a node as complete. Trigger all completion tokens associated with the given node.
    virtual void satisfyNode(NodePtr node) = 0;

    // --------------------------------------------------------
    // Token management
    // --------------------------------------------------------

    /// Get a token satisfied by certain node completes.
    /// \param node The node to wait for
    /// \return The token will be satisfied when the given node completes.
    virtual TokenPtr getNodeCompletionToken(NodePtr node) = 0;

    /// Get an token satisfied when the given artifact version is published.
    /// \param artifact The artifact to wait for.
    /// \param version The version of the artifact to wait for. If not provided (set to OOO), the next published version is used.
    /// \return The token that will be satisfied when the given artifact version is published.
    virtual TokenPtr getArtifactVersionToken(ArtifactPtr artifact, NeverOverflowingCounter version = NeverOverflowingCounter::OOO()) = 0;

protected:
    Graph() = default;
};

} // namespace GN::rdg2

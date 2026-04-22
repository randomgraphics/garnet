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
    virtual void execute(Arguments & arguments) = 0;

protected:
    using Entity::Entity;
};
using ActionPtr = AutoRef<Action>;

// ============================================================
// Declare opaque types managed by the graph
// ============================================================

struct Token;
struct Node;
struct Artifact;

typedef struct Token *    TokenPtr;
typedef struct Node *     NodePtr;
typedef struct Artifact * ArtifactPtr;

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
// Node description
// ============================================================

template<typename T>
using ArrayContainer = DynaArray<T, size_t>;

struct NodeDesc {
    StrA                     name         = StrA::EMPTYSTR();
    ActionPtr                action       = nullptr;
    ArgumentsPtr             arguments    = nullptr;
    ArrayContainer<TokenPtr> dependencies = {};
    SchedulingHints          scheduling   = {};
    NodePtr                  parent       = nullptr;

    NodeDesc()                                   = default;
    NodeDesc(const NodeDesc & other)             = default;
    NodeDesc(NodeDesc && other)                  = default;
    NodeDesc & operator=(const NodeDesc & other) = default;
    NodeDesc & operator=(NodeDesc && other)      = default;
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
    // Graph status query
    // --------------------------------------------------------

    enum class WaitResult : uint8_t {
        IDLE,
        BUSY,
        FAILED,
    };

    virtual WaitResult waitForIdle(std::chrono::milliseconds timeout = std::chrono::milliseconds::max()) const = 0;

    bool isIdle() const { return waitForIdle(std::chrono::milliseconds::zero()) == WaitResult::IDLE; }

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

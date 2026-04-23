#include "pch.h"
#include <garnet/GNrdg2.h>

#include <condition_variable>
#include <mutex>
#include <queue>

namespace GN::rdg2 {

/// Generates a process-wide unique entity id.
///
/// Thread-safe and monotonic, using NeverOverflowingCounter to avoid wraparound.
static NeverOverflowingCounter nextEntityNeverOverflowingId() {
    static std::mutex              m;
    static NeverOverflowingCounter counter = NeverOverflowingCounter::OOO();
    std::lock_guard<std::mutex>    lock(m);
    auto                           out = counter;
    counter.increment();
    return out;
}

/// Re-entrancy guard for calls that originate from inside node actions.
///
/// Some waits behave differently when called from within `Action::execute` to avoid deadlocks.
static thread_local int s_inGraphExecute = 0;

// ============================================================
// FOURCC / opaque
// ============================================================

/// A tiny 4-character tag used to validate opaque pointers at runtime.
///
/// This is used by `OpaqueBase::prompt()` to detect misuse (wrong type / dangling pointer).
struct FOURCC {
    /// Tag characters in fixed order.
    char ch0, ch1, ch2, ch3;
    /// Default constructs a zero tag (invalid/empty).
    constexpr FOURCC(): ch0(0), ch1(0), ch2(0), ch3(0) {}
    /// Copy constructor.
    constexpr FOURCC(const FOURCC & o): ch0(o.ch0), ch1(o.ch1), ch2(o.ch2), ch3(o.ch3) {}
    /// Constructs from explicit characters.
    constexpr FOURCC(char c0, char c1, char c2, char c3): ch0(c0), ch1(c1), ch2(c2), ch3(c3) {}
    /// Constructs from a 4-char string literal (plus '\0').
    constexpr FOURCC(const char (&s)[5]): ch0(s[0]), ch1(s[1]), ch2(s[2]), ch3(s[3]) {}
    /// Exact tag equality.
    bool operator==(const FOURCC & o) const { return ch0 == o.ch0 && ch1 == o.ch1 && ch2 == o.ch2 && ch3 == o.ch3; }
};

template<typename T>
struct OpaqueBase {
    /// Runtime type tag stored in the object.
    FOURCC tag;

    /// Stores the expected tag into the object instance.
    OpaqueBase(): tag(T::kTag) {}

    ~OpaqueBase() {
        // fill tag with garbage data.
        tag.ch0 = 0xb;
        tag.ch1 = 0xad;
        tag.ch2 = 0xbe;
        tag.ch3 = 0xef;
    }

    /// Validates an opaque pointer and returns a typed pointer on success.
    ///
    /// Returns nullptr on null input or if the tag doesn't match `T::kTag`.
    static const T * prompt(const void * p) {
        if (!p) return nullptr;
        auto t = (const T *) p;
        if (t->tag != T::kTag) return nullptr;
        return t;
    }

    /// Mutable overload of `prompt(const void*)`.
    static T * prompt(void * p) { return const_cast<T *>(prompt((const void *) p)); }
};

// ============================================================
// Token
// ============================================================

struct Node;

/// A synchronization primitive used to express dependencies between nodes/artifact versions.
///
/// A token can have multiple waiting nodes. When satisfied, it decrements the waiters'
/// dependency counters and may move them into the ready queue.
struct Token final : OpaqueBase<Token> {
    /// Opaque pointer validation tag for Token.
    static constexpr FOURCC kTag {"TOKE"};

    /// Debug/display name, useful for logging and profiling.
    const StrA name;

    /// Whether this token has been satisfied; satisfaction is sticky.
    bool satisfied = false;

    // One waiter entry per dependency edge; duplicates of the same node are allowed.
    /// Nodes blocked on this token. Duplicates represent multiple edges from the same node.
    ArrayContainer<Node *> waiters;

    /// Creates a token with a human-readable name.
    explicit Token(const StrA & n): name(n) {}
};

// ============================================================
// Artifact
// ============================================================

/// A named piece of data published into the graph with a monotonically increasing version.
///
/// Nodes can wait for "artifact reaches version >= X" via a token produced by
/// `getArtifactVersionToken`.
struct Artifact final : OpaqueBase<Artifact> {
    /// Opaque pointer validation tag for Artifact.
    static constexpr FOURCC kTag {"ARTI"};

    /// Debug/display name for the artifact.
    const StrA name;
    /// Current version counter (increments on every publish).
    NeverOverflowingCounter m_version = NeverOverflowingCounter::OOO();
    /// Latest published content for this artifact.
    AutoRef<Entity> m_content;
    // Pending: wait until m_version (after a publish) >= targetVersion.
    /// A pending waiter for a specific target version.
    struct Pending {
        /// Version threshold that must be reached (inclusive).
        NeverOverflowingCounter target;
        /// Token to satisfy when the version threshold is met.
        Token * token;
    };
    /// Wait list of version-threshold requests that haven't been satisfied yet.
    std::list<Pending> m_pending;

    /// Creates an artifact with a human-readable name.
    explicit Artifact(const StrA & n): name(n) {}
};

// ============================================================
// Node
// ============================================================

/// A runnable unit of work in the graph.
///
/// Nodes become Ready when all dependency tokens are satisfied, then they may be executed,
/// and finally Completed.
struct Node final : OpaqueBase<Node> {
    /// Opaque pointer validation tag for Node.
    static constexpr FOURCC kTag {"NODE"};

    /// Immutable description: action, arguments, dependencies and scheduling hints.
    NodeDesc desc;
    /// Number of unsatisfied dependency edges remaining.
    uint32_t unresolvedDependencies = 0;
    /// Current scheduling/execution lifecycle state.
    enum class State : uint8_t { Blocked, Ready, Running, Completed } state = State::Blocked;
    // Lazily created; satisfied by satisfyNode / pump.
    /// Token satisfied when the node reaches Completed (created on demand).
    Token * m_completion = nullptr;

    /// Constructs a node from a description (moves/copies as needed).
    explicit Node(NodeDesc d): desc(std::move(d)) {}
    ~Node() = default;
};

// ============================================================
// OpenGraphImpl
// ============================================================

/// A minimal single-worker "open graph" runtime.
///
/// This implementation executes at most one node at a time (`m_running` is 0/1) and uses
/// condition-variable based waiting for idle/token satisfaction.
class OpenGraphImpl final : public Graph {
public:
    OpenGraphImpl() = default;
    ~OpenGraphImpl() override {
        {
            std::unique_lock lock(m_mutex);
            m_stopping = true;
            m_cv.notify_all();
        }
        for (size_t i = 0; i < m_nodeRegistry.size(); ++i) { delete m_nodeRegistry[i]; }
        m_nodeRegistry.clear();
        for (size_t i = 0; i < m_artifactRegistry.size(); ++i) { delete m_artifactRegistry[i]; }
        m_artifactRegistry.clear();
        for (size_t i = 0; i < m_allTokens.size(); ++i) { delete m_allTokens[i]; }
        m_allTokens.clear();
    }

    /// Blocks until the graph has no runnable work and no outstanding non-terminal nodes.
    Graph::WaitResult waitForIdle(std::chrono::milliseconds timeout) const override;
    /// Blocks until a token becomes satisfied (or the graph is stopping).
    Graph::WaitResult waitForToken(TokenPtr token) const override;

    /// Creates a new artifact instance tracked by this graph.
    ArtifactPtr createArtifact(const StrA & name) override;
    /// Publishes new content, increments version, and satisfies any pending version waiters.
    void publishArtifact(ArtifactPtr artifact, AutoRef<Entity> content) override;

    /// Adds a node, collects dependency edges, and enqueues it if ready.
    NodePtr addNode(const NodeDesc & desc) override;
    /// Attempts to complete the node and/or drive execution forward.
    void satisfyNode(NodePtr node) override;

    /// Returns a token that is satisfied when the given node completes.
    TokenPtr getNodeCompletionToken(NodePtr node) override;
    /// Returns a token that is satisfied when the artifact reaches the requested version.
    ///
    /// If `version` is OOO(), it represents "next publish" relative to the current version.
    TokenPtr getArtifactVersionToken(ArtifactPtr artifact, NeverOverflowingCounter version) override;

private:
    /// Wakes all waiters on the graph condition variable.
    void notifyAll_() { m_cv.notify_all(); }
    /// Enqueues a ready node in the priority queue and wakes waiters.
    void pushReady_(Node * n);
    /// Runs ready nodes while possible (single-worker) and completes them.
    void pump_(std::unique_lock<std::mutex> & lock);
    /// Marks a token satisfied and potentially unblocks dependent nodes.
    void satisfyToken_(Token * t, std::unique_lock<std::mutex> & lock);
    /// Transitions a running node to completed and satisfies its completion token.
    bool tryCompleteNode_(Node * n, std::unique_lock<std::mutex> & lock);

    /// Protects all state below (nodes, tokens, queues, versions).
    mutable std::mutex m_mutex;
    /// Waiters sleep on this for token satisfaction, queue changes, or stopping.
    mutable std::condition_variable m_cv;
    /// Stop flag set during destruction to unblock waiters and prevent further work.
    bool m_stopping = false;
    /// Number of currently running workers; this implementation uses 0/1.
    mutable int m_running = 0;
    /// Count of nodes that haven't completed yet (used to detect idleness).
    mutable size_t m_nonTerminalNodes = 0;
    /// Monotonic enqueue ordinal used as a stable tie-breaker in the ready queue.
    mutable uint64_t m_enqueueOrdinal = 0;
    // Ownership of heap nodes / artifacts; tokens on nodes/artifacts and in m_artifactOrphans.
    /// Owns all heap-allocated nodes for this graph instance.
    ArrayContainer<Node *> m_nodeRegistry;
    /// Owns all heap-allocated artifacts for this graph instance.
    ArrayContainer<Artifact *> m_artifactRegistry;
    /// Owns all heap Token objects; freed in ~OpenGraphImpl (dep tokens, completion, artifact version).
    ArrayContainer<Token *> m_allTokens;
    // Ready queue: lower SchedulingClass and higher int priority first; stable tie-breaker.
    /// Priority-queue element for a runnable node.
    struct ReadyEntry {
        /// Node to execute.
        Node * node = nullptr;
        /// Cached scheduling hints to avoid re-reading node desc in compare.
        SchedulingHints hints {};
        /// Insertion order (increasing) to preserve stability for equal priorities.
        uint64_t ord = 0;
    };
    /// Comparator implementing the ready-queue ordering:
    /// - lower scheduling class first
    /// - higher priority first
    /// - earlier insertion order first
    struct ReadyCompare {
        bool operator()(const ReadyEntry & a, const ReadyEntry & b) const {
            if (a.hints.schedulingClass != b.hints.schedulingClass) {
                return static_cast<int>(a.hints.schedulingClass) > static_cast<int>(b.hints.schedulingClass);
            }
            if (a.hints.priority != b.hints.priority) { return a.hints.priority < b.hints.priority; }
            return a.ord > b.ord;
        }
    };
    /// Runnable nodes waiting for execution.
    std::priority_queue<ReadyEntry, std::vector<ReadyEntry>, ReadyCompare> m_ready;
};

// ============================================================
// OpenGraphImpl — helpers
// ============================================================

/// Adds a ready node to the execution queue.
void OpenGraphImpl::pushReady_(Node * n) {
    if (!n) { return; }
    ReadyEntry e;
    e.node  = n;
    e.hints = n->desc.scheduling;
    e.ord   = ++m_enqueueOrdinal;
    m_ready.push(e);
    notifyAll_();
}

/// Satisfies a token and unblocks any nodes that become dependency-free.
void OpenGraphImpl::satisfyToken_(Token * t, std::unique_lock<std::mutex> & lock) {
    (void) lock;
    if (!t || t->satisfied) { return; }
    t->satisfied = true;
    for (size_t i = 0; i < t->waiters.size(); ++i) {
        Node * w = t->waiters[i];
        if (!w) { continue; }
        if (w->state != Node::State::Blocked) { continue; }
        if (w->unresolvedDependencies > 0) { --w->unresolvedDependencies; }
        if (w->unresolvedDependencies == 0) {
            w->state = Node::State::Ready;
            pushReady_(w);
        }
    }
    notifyAll_();
}

/// Completes a node that is currently running.
///
/// Returns true if the node is now completed (already completed counts as true).
bool OpenGraphImpl::tryCompleteNode_(Node * n, std::unique_lock<std::mutex> & lock) {
    (void) lock;
    if (!n) { return false; }
    if (n->state == Node::State::Completed) { return true; }
    if (n->state != Node::State::Running) { return false; }
    n->state = Node::State::Completed;
    if (m_nonTerminalNodes > 0) { --m_nonTerminalNodes; }
    if (n->m_completion) { satisfyToken_(n->m_completion, lock); }
    return true;
}

/// Drives execution: runs ready nodes while no node is running and work exists.
///
/// This is single-threaded execution: it pops the highest-priority ready node, runs its action
/// outside the lock, then marks it completed.
void OpenGraphImpl::pump_(std::unique_lock<std::mutex> & lock) {
    while (m_running == 0 && !m_ready.empty() && !m_stopping) {
        ReadyEntry e = m_ready.top();
        m_ready.pop();
        Node * n = e.node;
        if (!n) { continue; }
        if (n->state != Node::State::Ready) { continue; }
        n->state        = Node::State::Running;
        m_running       = 1;
        Action *    act = n->desc.action.get();
        Arguments * arg = n->desc.arguments.get();
        lock.unlock();
        struct Scope {
            /// Reference to the thread-local nesting counter.
            int & d;
            /// Enters "in graph execute" scope.
            explicit Scope(int & d_): d(d_) { ++d; }
            /// Exits "in graph execute" scope.
            ~Scope() { --d; }
        } scope(s_inGraphExecute);
        /// Executes the node action (if present); user code may call back into the graph.
        if (act && arg) { act->execute(*arg); }
        lock.lock();
        m_running = 0;
        tryCompleteNode_(n, lock);
        if (m_stopping) { break; }
    }
    notifyAll_();
}

// ============================================================
// OpenGraphImpl — Graph
// ============================================================

/// Waits until the graph is idle (no running work, nothing ready, and no unfinished nodes).
///
/// - If called from inside a node action, returns BUSY to avoid deadlock.
/// - If timeout is zero, performs a non-blocking check.
Graph::WaitResult OpenGraphImpl::waitForIdle(std::chrono::milliseconds timeout) const {
    if (s_inGraphExecute > 0) { return Graph::WaitResult::BUSY; }
    auto * self          = const_cast<OpenGraphImpl *>(this);
    using steady_clock   = std::chrono::steady_clock;
    const auto deadline  = steady_clock::now() + timeout;
    const bool finiteCap = (timeout < std::chrono::milliseconds::max() && timeout.count() > 0);

    std::unique_lock lock(m_mutex);
    for (;;) {
        if (m_stopping) { return Graph::WaitResult::FAILED; }
        self->pump_(lock);
        if (m_stopping) { return Graph::WaitResult::FAILED; }
        if (m_running == 0 && m_ready.empty() && m_nonTerminalNodes == 0) { return Graph::WaitResult::IDLE; }
        if (timeout == std::chrono::milliseconds::zero()) { return Graph::WaitResult::BUSY; }
        if (finiteCap && steady_clock::now() >= deadline) { return Graph::WaitResult::BUSY; }
        if (finiteCap) {
            m_cv.wait_until(lock, deadline);
        } else {
            m_cv.wait(lock);
        }
    }
}

/// Waits for a specific token to be satisfied.
///
/// When called from inside node execution, it does not block; it only reports whether the
/// token is already satisfied.
Graph::WaitResult OpenGraphImpl::waitForToken(TokenPtr token) const {
    Token * t = Token::prompt(token);
    if (!t) { return Graph::WaitResult::FAILED; }
    if (s_inGraphExecute > 0) { return t->satisfied ? Graph::WaitResult::IDLE : Graph::WaitResult::BUSY; }
    std::unique_lock lock(m_mutex);
    for (;;) {
        if (m_stopping) { return Graph::WaitResult::FAILED; }
        if (t->satisfied) { return Graph::WaitResult::IDLE; }
        m_cv.wait(lock, [t, this] { return t->satisfied || m_stopping; });
    }
}

/// Allocates and registers an artifact owned by this graph.
ArtifactPtr OpenGraphImpl::createArtifact(const StrA & name) {
    std::lock_guard g(m_mutex);
    auto *          a = new Artifact(name);
    (void) m_artifactRegistry.append(a);
    return static_cast<ArtifactPtr>(a);
}

/// Publishes new artifact content, bumps the version, and satisfies pending version tokens.
void OpenGraphImpl::publishArtifact(ArtifactPtr ap, AutoRef<Entity> content) {
    auto * a = GN::rdg2::Artifact::prompt(ap);
    if (!a) { return; }
    std::unique_lock lock(m_mutex);
    a->m_version.increment();
    a->m_content = std::move(content);
    // for (size_t i = 0; i < a->m_pending.size();) {
    for (auto iter = a->m_pending.begin(); iter != a->m_pending.end();) {
        if (a->m_version >= iter->target) {
            auto pending = iter->token;
            iter         = a->m_pending.erase(iter);
            if (pending) { satisfyToken_(pending, lock); }
        } else {
            ++iter;
        }
    }
    notifyAll_();
}

/// Wires dependency edges from a node to its dependency tokens.
///
/// For each unsatisfied dependency token, this:
/// - increments the node's unresolved dependency counter
/// - adds the node into the token's waiter list
/// Finally sets the node's initial state (Ready vs Blocked).
static bool collectDeps(Node * n, const NodeDesc & d) {
    n->unresolvedDependencies = 0;
    for (size_t i = 0; i < d.dependencies.size(); ++i) {
        TokenPtr tok = d.dependencies[i];
        Token *  t   = static_cast<Token *>(tok);
        if (!t) { continue; }
        if (t->satisfied) { continue; }
        ++n->unresolvedDependencies;
        (void) t->waiters.append(n);
    }
    n->state = n->unresolvedDependencies == 0 ? Node::State::Ready : Node::State::Blocked;
    return true;
}

/// Adds a node to the graph and enqueues it if all dependencies are already satisfied.
NodePtr OpenGraphImpl::addNode(const NodeDesc & desc) {
    std::unique_lock lock(m_mutex);
    auto *           n = new Node(desc);
    if (!n) { return nullptr; }
    (void) collectDeps(n, desc);
    (void) m_nodeRegistry.append(n);
    ++m_nonTerminalNodes;
    if (n->state == Node::State::Ready) { pushReady_(n); }
    notifyAll_();
    return n;
}

/// Signals that a node may be complete and attempts to advance execution.
///
/// This does not directly satisfy dependencies; it ensures completion state is applied and may
/// run ready nodes if the worker is idle.
void OpenGraphImpl::satisfyNode(NodePtr node) {
    Node * n = Node::prompt(node);
    if (!n) { return; }
    std::unique_lock lock(m_mutex);
    if (m_stopping) { return; }
    if (n->state == Node::State::Completed) { return; }
    if (n->state == Node::State::Running) { tryCompleteNode_(n, lock); }
    if (m_running == 0) { pump_(lock); }
    notifyAll_();
}

/// Returns (and lazily creates) the completion token for a node.
TokenPtr OpenGraphImpl::getNodeCompletionToken(NodePtr node) {
    Node * n = Node::prompt(node);
    if (!n) { return nullptr; }
    std::lock_guard g(m_mutex);
    if (!n->m_completion) {
        n->m_completion = new Token("node completion");
        (void) m_allTokens.append(n->m_completion);
    }
    return n->m_completion;
}

/// Returns a token that becomes satisfied when an artifact reaches a target version.
///
/// `version == OOO()` is treated as "next version after current".
TokenPtr OpenGraphImpl::getArtifactVersionToken(ArtifactPtr ap, NeverOverflowingCounter version) {
    auto * a = Artifact::prompt(ap);
    if (!a) { return nullptr; }
    std::unique_lock lock(m_mutex);
    const bool       isNext = (version == NeverOverflowingCounter::OOO());
    Token *          t      = new Token("artifact version");
    (void) m_allTokens.append(t);
    NeverOverflowingCounter target = version;
    if (isNext) {
        target = a->m_version;
        target.increment();
    }
    if (a->m_version >= target) {
        satisfyToken_(t, lock);
    } else {
        Artifact::Pending p;
        p.target = target;
        p.token  = t;
        a->m_pending.insert(a->m_pending.end(), p);
    }
    return t;
}

/// Constructs an RDG entity with a unique id and human-readable name.
GN_API Entity::Entity(const GN::rdg::RuntimeType::TypeInfo & type, const StrA & name)
    : RefCounter(), RuntimeType(type), id(nextEntityNeverOverflowingId()), name(name) {}

/// Factory for creating a new open-graph implementation.
GN_API AutoRef<Graph> Graph::create() { return AutoRef<Graph>(new OpenGraphImpl()); }

} // namespace GN::rdg2

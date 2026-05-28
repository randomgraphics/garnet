#include "pch.h"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

namespace GN::rdg2 {

static GN::Logger * sLogger = GN::getLogger("GN.rdg2.open-graph");

class OpenGraphImpl;

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
    uint8_t ch0, ch1, ch2, ch3;

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

    virtual ~OpaqueBase() {
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

struct NodeImpl;

/// A synchronization primitive used to express dependencies between nodes/artifact versions.
///
/// A token can have multiple waiting nodes. When satisfied, it decrements the waiters'
/// dependency counters and may move them into the ready queue.
struct TokenImpl final : public Token, private OpaqueBase<TokenImpl> {
    GN_REGISTER_RUNTIME_TYPE(Token);

    /// Opaque pointer validation tag for Token.
    static constexpr FOURCC kTag {"TOKE"};

    /// Debug/display name, useful for logging and profiling.
    const StrA name;

    /// Whether this token has been satisfied; satisfaction is sticky.
    bool satisfied = false;

    // One waiter entry per dependency edge; duplicates of the same node are allowed.
    /// Nodes blocked on this token. Duplicates represent multiple edges from the same node.
    ArrayContainer<NodeImpl *> waiters;

    /// Creates a token with a human-readable name.
    TokenImpl(const StrA & n, Graph * graph): Token(TYPE_INFO(), n), name(n), mGraph(graph) {}

    WeakRef<Graph> graph() const { return mGraph; }

    bool hasValidTag() const { return tag == kTag; }

    /// Callable storage for artifact publish signal connections; sigslot keeps a pointer to this object.
    std::function<void(const Artifact::Content<> &)> artifactPublished;
    /// Disconnects the artifact signal when this token is destroyed or reused.
    Tether artifactPublishedTether;

private:
    /// Non-owning graph association. Handles outlive graphs; graph operations promote this before use.
    WeakRef<Graph> mGraph;
};

// ============================================================
// Node
// ============================================================

/// A runnable unit of work in the graph.
///
/// Nodes become Ready when all dependency tokens are satisfied, then they may be executed,
/// and finally Completed.
struct NodeImpl final : public Node, private OpaqueBase<NodeImpl> {
    GN_REGISTER_RUNTIME_TYPE(Node);

    /// Opaque pointer validation tag for Node.
    static constexpr FOURCC kTag {"NODE"};

    /// Immutable description: action, dependencies and scheduling hints.
    NodeDesc desc;

    /// Number of unsatisfied dependency edges remaining.
    uint32_t unresolvedDependencies = 0;

    /// Number of incomplete child nodes.
    uint32_t incompleteChildren = 0;

    /// Parent node (if this is a child); stored as a validated internal pointer.
    NodeImpl * parent = nullptr;

    /// Current scheduling/execution lifecycle state.
    enum class State : uint8_t { Blocked, Ready, Running, FinishedAction, Completed } state = State::Blocked;

    /// Constructs a node from a description (moves/copies as needed).
    NodeImpl(NodeDesc d, Graph * graph): Node(TYPE_INFO(), d.name), desc(std::move(d)), mGraph(graph) {}

    WeakRef<Graph> graph() const { return mGraph; }

    bool hasValidTag() const { return tag == kTag; }

private:
    friend class OpenGraphImpl;

    /// True once completeNode() has been accepted for a manual-complete node.
    bool manualCompletionRequested = false;

    // Lazily created; satisfied by completeNode / pump.
    /// Token satisfied when the node reaches Completed (created on demand).
    TokenImpl * mCompletion = nullptr;

    /// Non-owning graph association. Handles outlive graphs; graph operations promote this before use.
    WeakRef<Graph> mGraph;
};

// ============================================================
// OpenGraphImpl
// ============================================================

/// A minimal single-worker "open graph" runtime.
///
/// This implementation executes at most one node at a time (`mRunning` is 0/1) and uses
/// condition-variable based waiting for idle/token satisfaction.
class OpenGraphImpl final : public Graph {
public:
    OpenGraphImpl() = default;
    ~OpenGraphImpl() override {
        {
            std::unique_lock lock(mMutex);
            mStopping = true;
            mCv.notify_all();
        }
        mReady = {};
        mNodeRegistry.clear();
        mAllTokens.clear();
    }

    /// Blocks until the graph has no runnable work and no outstanding non-terminal nodes.
    Graph::WaitResult waitForIdle(std::chrono::milliseconds timeout) const override;
    /// Blocks until a token becomes satisfied (or the graph is stopping).
    Graph::WaitResult waitForToken(const TokenPtr & token) const override;

    /// Creates a standalone artifact instance.
    ArtifactPtr createArtifact(const StrA & name) override;
    /// Publishes new content through the standalone artifact API.
    void publishArtifact(const ArtifactPtr & artifact, AutoRef<Entity> content) override;

    /// Adds a node, collects dependency edges, and enqueues it if ready.
    NodePtr addNode(const NodeDesc & desc) override;
    /// Completes a manual node after its action has finished.
    void completeNode(const NodePtr & node) override;

    /// Returns a token that is satisfied when the given node completes.
    TokenPtr getNodeCompletionToken(const NodePtr & node) override;
    /// Returns a token that is satisfied when the artifact reaches the requested version.
    ///
    /// If `version` is OOO(), it represents "next publish" relative to the current version.
    TokenPtr getArtifactVersionToken(const ArtifactPtr & artifact, NeverOverflowingCounter version) override;

private:
    /// Wakes all waiters on the graph condition variable.
    void notifyAll_() { mCv.notify_all(); }
    /// Enqueues a ready node in the priority queue and wakes waiters.
    void pushReady_(NodeImpl & n);
    /// Runs ready nodes while possible (single-worker) and completes them.
    void pump_(std::unique_lock<std::mutex> & lock);
    /// Collects dependency edges for a newly-added node.
    bool collectDeps_(NodeImpl & n, const NodeDesc & d);
    /// Marks a token satisfied and potentially unblocks dependent nodes.
    void satisfyToken_(TokenImpl & t);
    /// Satisfies an artifact-version token from an artifact callback.
    void satisfyArtifactToken_(const AutoRef<TokenImpl> & t);
    /// Transitions a running node to completed and satisfies its completion token.
    bool tryCompleteNode_(NodeImpl & n, std::unique_lock<std::mutex> & lock);

    TokenImpl * validateToken_(const TokenPtr & token, const char * api) const;
    NodeImpl *  validateNode_(const NodePtr & node, const char * api) const;

    /// Protects all state below (nodes, tokens, queues).
    mutable std::mutex mMutex;
    /// Waiters sleep on this for token satisfaction, queue changes, or stopping.
    mutable std::condition_variable mCv;
    /// Stop flag set during destruction to unblock waiters and prevent further work.
    bool mStopping = false;
    /// Number of currently running workers; this implementation uses 0/1.
    mutable int mRunning = 0;
    /// Count of nodes that haven't completed yet (used to detect idleness).
    mutable size_t mNonTerminalNodes = 0;
    /// Monotonic enqueue ordinal used as a stable tie-breaker in the ready queue.
    mutable uint64_t mEnqueueOrdinal = 0;
    // Ownership of heap nodes and tokens.
    /// Owns all heap-allocated nodes for this graph instance.
    ArrayContainer<AutoRef<NodeImpl>> mNodeRegistry;
    /// Owns all heap Token objects; freed in ~OpenGraphImpl (dep tokens, completion, artifact version).
    ArrayContainer<AutoRef<TokenImpl>> mAllTokens;
    // Ready queue: lower SchedulingClass and higher int priority first; stable tie-breaker.
    /// Priority-queue element for a runnable node.
    struct ReadyEntry {
        /// Node to execute.
        NodeImpl * node = nullptr;
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
    std::priority_queue<ReadyEntry, std::vector<ReadyEntry>, ReadyCompare> mReady;
};

// ============================================================
// OpenGraphImpl — helpers
// ============================================================

/// Adds a ready node to the execution queue.
void OpenGraphImpl::pushReady_(NodeImpl & n) {
    ReadyEntry e;
    e.node  = &n;
    e.hints = n.desc.scheduling;
    e.ord   = ++mEnqueueOrdinal;
    mReady.push(e);
    notifyAll_();
}

/// Satisfies a token and unblocks any nodes that become dependency-free.
void OpenGraphImpl::satisfyToken_(TokenImpl & t) {
    if (t.satisfied) { return; }
    t.satisfied = true;
    for (size_t i = 0; i < t.waiters.size(); ++i) {
        NodeImpl * w = t.waiters[i];
        if (!w) { continue; }
        if (w->state != NodeImpl::State::Blocked) { continue; }
        if (w->unresolvedDependencies > 0) { --w->unresolvedDependencies; }
        if (w->unresolvedDependencies == 0) {
            w->state = NodeImpl::State::Ready;
            pushReady_(*w);
        }
    }
    notifyAll_();
}

/// Bridges standalone artifact notifications back into this graph's token dependency state.
void OpenGraphImpl::satisfyArtifactToken_(const AutoRef<TokenImpl> & t) {
    if (!t) { return; }
    std::unique_lock lock(mMutex);
    if (mStopping) { return; }
    satisfyToken_(*t);
}

/// Finalizes a node that has finished its own action, if possible.
///
/// A node becomes Completed iff:
/// - its own action has finished (state == FinishedAction), and
/// - manual completion has been requested when desc.manualComplete is true, and
/// - all of its children are Completed (incompleteChildren == 0).
///
/// Returns true if the node is now completed (already completed counts as true).
bool OpenGraphImpl::tryCompleteNode_(NodeImpl & n, std::unique_lock<std::mutex> & lock) {
    (void) lock;
    if (n.state == NodeImpl::State::Completed) { return true; }
    if (n.state != NodeImpl::State::FinishedAction) { return false; }
    if (n.desc.manualComplete && !n.manualCompletionRequested) { return false; }
    if (n.incompleteChildren != 0) { return false; }
    n.state = NodeImpl::State::Completed;
    if (mNonTerminalNodes > 0) { --mNonTerminalNodes; }
    if (n.mCompletion) { satisfyToken_(*n.mCompletion); }
    if (n.parent) {
        if (n.parent->incompleteChildren > 0) GN_LIKELY {
                --n.parent->incompleteChildren;
            }
        if (0 == n.parent->incompleteChildren) {
            // Parent may become completable now.
            (void) tryCompleteNode_(*n.parent, lock);
        }
    }
    return true;
}

/// Drives execution: runs ready nodes while no node is running and work exists.
///
/// This is single-threaded execution: it pops the highest-priority ready node, runs its action
/// outside the lock, then marks it completed.
void OpenGraphImpl::pump_(std::unique_lock<std::mutex> & lock) {
    while (mRunning == 0 && !mReady.empty() && !mStopping) {
        ReadyEntry e = mReady.top();
        mReady.pop();
        NodeImpl * n = e.node;
        if (!n) { continue; }
        if (n->state != NodeImpl::State::Ready) { continue; }
        n->state     = NodeImpl::State::Running;
        mRunning     = 1;
        Action * act = n->desc.action.get();
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
        if (act) { act->execute(); }
        lock.lock();
        mRunning = 0;
        // Mark the node's own work as finished; actual completion may be delayed by children.
        if (n->state == NodeImpl::State::Running) { n->state = NodeImpl::State::FinishedAction; }
        // Only try to complete the node when it is not manual completed.
        if (!n->desc.manualComplete) (void) tryCompleteNode_(*n, lock);
    }
    notifyAll_();
}

TokenImpl * OpenGraphImpl::validateToken_(const TokenPtr & token, const char * api) const {
    static auto * logger = GN::getLogger("GN.rdg2");
    if (!token) GN_UNLIKELY {
            GN_ERROR(logger)("{}: token is null", api);
            return nullptr;
        }
    auto * impl = RuntimeType::cast<TokenImpl>(token.get());
    if (!impl || !impl->hasValidTag()) GN_UNLIKELY {
            GN_ERROR(logger)("{}: token is not an open-graph token", api);
            return nullptr;
        }
    auto graph = impl->graph().promote();
    if (!graph) GN_UNLIKELY {
            GN_ERROR(logger)("{}: token's graph is already destroyed", api);
            return nullptr;
        }
    if (graph.get() != this) GN_UNLIKELY {
            GN_ERROR(logger)("{}: token belongs to a different graph", api);
            return nullptr;
        }
    return impl;
}

NodeImpl * OpenGraphImpl::validateNode_(const NodePtr & node, const char * api) const {
    static auto * logger = GN::getLogger("GN.rdg2");
    if (!node) GN_UNLIKELY {
            GN_ERROR(logger)("{}: node is null", api);
            return nullptr;
        }
    auto * impl = RuntimeType::cast<NodeImpl>(node.get());
    if (!impl || !impl->hasValidTag()) GN_UNLIKELY {
            GN_ERROR(logger)("{}: node is not an open-graph node", api);
            return nullptr;
        }
    auto graph = impl->graph().promote();
    if (!graph) GN_UNLIKELY {
            GN_ERROR(logger)("{}: node's graph is already destroyed", api);
            return nullptr;
        }
    if (graph.get() != this) GN_UNLIKELY {
            GN_ERROR(logger)("{}: node belongs to a different graph", api);
            return nullptr;
        }
    return impl;
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

    std::unique_lock lock(mMutex);
    for (;;) {
        if (mStopping) { return Graph::WaitResult::FAILED; }
        self->pump_(lock);
        if (mStopping) { return Graph::WaitResult::FAILED; }
        if (mRunning == 0 && mReady.empty() && mNonTerminalNodes == 0) { return Graph::WaitResult::IDLE; }
        if (timeout == std::chrono::milliseconds::zero()) { return Graph::WaitResult::BUSY; }
        if (finiteCap && steady_clock::now() >= deadline) { return Graph::WaitResult::BUSY; }
        if (finiteCap) {
            mCv.wait_until(lock, deadline);
        } else {
            mCv.wait(lock);
        }
    }
}

/// Waits for a specific token to be satisfied.
///
/// When called from inside node execution, it does not block; it only reports whether the
/// token is already satisfied.
Graph::WaitResult OpenGraphImpl::waitForToken(const TokenPtr & token) const {
    TokenImpl * t = validateToken_(token, "waitForToken");
    if (!t) { return Graph::WaitResult::FAILED; }
    if (s_inGraphExecute > 0) { return t->satisfied ? Graph::WaitResult::IDLE : Graph::WaitResult::BUSY; }
    auto *           self = const_cast<OpenGraphImpl *>(this);
    std::unique_lock lock(mMutex);
    for (;;) {
        if (mStopping) { return Graph::WaitResult::FAILED; }
        if (t->satisfied) { return Graph::WaitResult::IDLE; }
        self->pump_(lock); // drive ready nodes; token may be satisfied as a side effect
        if (mStopping) { return Graph::WaitResult::FAILED; }
        if (t->satisfied) { return Graph::WaitResult::IDLE; }
        mCv.wait(lock, [t, this] { return t->satisfied || mStopping; });
    }
}

/// Creates an artifact that is not owned by the graph runtime.
ArtifactPtr OpenGraphImpl::createArtifact(const StrA & name) { return Artifact::create(name); }

/// Publishes new artifact content through the standalone artifact primitive.
void OpenGraphImpl::publishArtifact(const ArtifactPtr & ap, AutoRef<Entity> content) {
    if (!ap) GN_UNLIKELY {
            GN_ERROR(sLogger)("publishArtifact: artifact is null.");
            return;
        }
    (void) ap->publish(std::move(content));
    notifyAll_();
}

/// Wires dependency edges from a node to its dependency tokens.
///
/// For each unsatisfied dependency token, this:
/// - increments the node's unresolved dependency counter
/// - adds the node into the token's waiter list
/// Finally sets the node's initial state (Ready vs Blocked).
bool OpenGraphImpl::collectDeps_(NodeImpl & n, const NodeDesc & d) {
    n.unresolvedDependencies = 0;
    for (size_t i = 0; i < d.dependencies.size(); ++i) {
        const TokenPtr & tok = d.dependencies[i];
        TokenImpl *      t   = validateToken_(tok, "addNode");
        if (!t) { continue; }
        if (t->satisfied) { continue; }
        ++n.unresolvedDependencies;
        (void) t->waiters.append(&n);
    }
    n.state = n.unresolvedDependencies == 0 ? NodeImpl::State::Ready : NodeImpl::State::Blocked;
    return true;
}

/// Adds a node to the graph and enqueues it if all dependencies are already satisfied.
NodePtr OpenGraphImpl::addNode(const NodeDesc & desc) {
    std::unique_lock lock(mMutex);
    auto             n = AutoRef<NodeImpl>(new NodeImpl(desc, this));
    if (!n) { return nullptr; }
    // Dependency tokens keep raw waiter links for hot-path efficiency; the registry
    // must own the node before any token records those links.
    (void) mNodeRegistry.append(n);
    n->parent = desc.parent ? validateNode_(desc.parent, "addNode") : nullptr;
    if (n->parent) { ++n->parent->incompleteChildren; }
    (void) collectDeps_(*n, desc);
    ++mNonTerminalNodes;
    if (n->state == NodeImpl::State::Ready) { pushReady_(*n); }
    notifyAll_();
    return n.staticCastTo<Node>();
}

/// Manually completes a node after its action and child nodes have finished.
///
/// completeNode() never treats a running action as finished. The action return path is the only
/// place that may transition Running -> FinishedAction.
void OpenGraphImpl::completeNode(const NodePtr & node) {
    NodeImpl * n = validateNode_(node, "completeNode");
    if (!n) { return; }
    std::unique_lock lock(mMutex);
    if (mStopping) { return; }
    if (n->state == NodeImpl::State::Completed) { return; }
    if (!n->desc.manualComplete) { return; }

    if (n->state != NodeImpl::State::FinishedAction) GN_UNLIKELY {
            GN_ERROR(GN::getLogger("GN.rdg2"))("completeNode: node '{}' action has not finished", n->name);
            return;
        }

    if (n->incompleteChildren != 0) GN_UNLIKELY {
            GN_ERROR(GN::getLogger("GN.rdg2"))("completeNode: node '{}' still has incomplete children", n->name);
            return;
        }

    n->manualCompletionRequested = true;
    (void) tryCompleteNode_(*n, lock);
    notifyAll_();
}

/// Returns (and lazily creates) the completion token for a node.
TokenPtr OpenGraphImpl::getNodeCompletionToken(const NodePtr & node) {
    NodeImpl * n = validateNode_(node, "getNodeCompletionToken");
    if (!n) { return nullptr; }
    std::lock_guard g(mMutex);
    if (!n->mCompletion) {
        auto token     = AutoRef<TokenImpl>(new TokenImpl("node completion", this));
        n->mCompletion = token.get();
        (void) mAllTokens.append(token);
    }
    return GN::referenceTo(static_cast<Token *>(n->mCompletion));
}

/// Returns a token that becomes satisfied when an artifact reaches a target version.
///
/// `version == OOO()` is treated as "next version after current".
TokenPtr OpenGraphImpl::getArtifactVersionToken(const ArtifactPtr & ap, NeverOverflowingCounter version) {
    if (!ap) GN_UNLIKELY {
            GN_ERROR(sLogger)("getArtifactVersionToken: artifact is null.");
            return nullptr;
        }

    auto t = AutoRef<TokenImpl>(new TokenImpl("artifact version", this));
    {
        std::unique_lock lock(mMutex);
        if (mStopping) { return nullptr; }
        (void) mAllTokens.append(t);
    }

    NeverOverflowingCounter target = version;
    if (target == NeverOverflowingCounter::OOO()) {
        target = ap->version();
        ++target;
    }

    t->artifactPublished = [token = t.get(), target](const Artifact::Content<> & content) {
        if (content.version < target) { return; }
        auto graph = token->graph().promote();
        if (!graph) { return; }
        static_cast<OpenGraphImpl *>(graph.get())->satisfyArtifactToken_(GN::referenceTo(token));
    };
    t->artifactPublishedTether = ap->sigPublished.connect(t->artifactPublished);
    if (ap->version() >= target) { satisfyArtifactToken_(t); }
    return t.staticCastTo<Token>();
}

/// Factory for creating a new open-graph implementation.
GN_API GraphPtr Graph::create() { return GraphPtr(new OpenGraphImpl()); }

} // namespace GN::rdg2

#include "pch.h"
#include <garnet/GNrdg2.h>

#include <condition_variable>
#include <mutex>
#include <queue>

namespace GN::rdg2 {

static NeverOverflowingCounter nextEntityNeverOverflowingId() {
    static std::mutex              m;
    static NeverOverflowingCounter counter = NeverOverflowingCounter::OOO();
    std::lock_guard<std::mutex>    lock(m);
    auto                           out = counter;
    counter.increment();
    return out;
}

static thread_local int s_inGraphExecute = 0;

// ============================================================
// FOURCC / opaque
// ============================================================

struct FOURCC {
    char ch0, ch1, ch2, ch3;
    constexpr FOURCC(): ch0(0), ch1(0), ch2(0), ch3(0) {}
    constexpr FOURCC(const FOURCC & o): ch0(o.ch0), ch1(o.ch1), ch2(o.ch2), ch3(o.ch3) {}
    constexpr FOURCC(char c0, char c1, char c2, char c3): ch0(c0), ch1(c1), ch2(c2), ch3(c3) {}
    constexpr FOURCC(const char (&s)[5]): ch0(s[0]), ch1(s[1]), ch2(s[2]), ch3(s[3]) {}
    bool operator==(const FOURCC & o) const { return ch0 == o.ch0 && ch1 == o.ch1 && ch2 == o.ch2 && ch3 == o.ch3; }
};

struct OpaqueBase {
    FOURCC tag;
    explicit OpaqueBase(const FOURCC & t): tag(t) {}
};

// ============================================================
// Token
// ============================================================

struct Node;

struct Token final : OpaqueBase {
    static constexpr FOURCC kTag {"TOKE"};

    const StrA name;
    bool       satisfied = false;
    // One waiter entry per dependency edge; duplicates of the same node are allowed.
    ArrayContainer<Node *> waiters;

    explicit Token(const StrA & n): OpaqueBase(kTag), name(n) {}
};

// ============================================================
// Artifact
// ============================================================

struct Artifact final : OpaqueBase {
    static constexpr FOURCC kTag {"ARTI"};

    const StrA              name;
    bool                    m_hasPublished = false;
    NeverOverflowingCounter m_version      = NeverOverflowingCounter::OOO();
    AutoRef<Entity>         m_content;
    // Pending: wait until m_version (after a publish) >= targetVersion.
    struct Pending {
        NeverOverflowingCounter target;
        Token *                 token;
    };
    ArrayContainer<Pending> m_pending;

    explicit Artifact(const StrA & n): OpaqueBase(kTag), name(n) {}
};

// ============================================================
// Node
// ============================================================

struct Node final : OpaqueBase {
    static constexpr FOURCC kTag {"NODE"};

    NodeDesc desc;
    uint32_t unresolvedDependencies                                         = 0;
    enum class State : uint8_t { Blocked, Ready, Running, Completed } state = State::Blocked;
    // Lazily created; satisfied by satisfyNode / pump.
    Token * m_completion = nullptr;

    static Node * n(NodePtr p) { return reinterpret_cast<Node *>(p); }

    explicit Node(NodeDesc d): OpaqueBase(kTag), desc(std::move(d)) {}
    ~Node() = default;
};

// ============================================================
// OpenGraphImpl
// ============================================================

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

    Graph::WaitResult waitForIdle(std::chrono::milliseconds timeout) const override;
    Graph::WaitResult waitForToken(TokenPtr token) const override;

    ArtifactPtr createArtifact(const StrA & name) override;
    void        publishArtifact(ArtifactPtr artifact, AutoRef<Entity> content) override;

    NodePtr addNode(const NodeDesc & desc) override;
    void    satisfyNode(NodePtr node) override;

    TokenPtr getNodeCompletionToken(NodePtr node) override;
    TokenPtr getArtifactVersionToken(ArtifactPtr artifact, NeverOverflowingCounter version) override;

private:
    void notifyAll_() { m_cv.notify_all(); }
    void pushReady_(Node * n);
    void pump_(std::unique_lock<std::mutex> & lock);
    void satisfyToken_(Token * t, std::unique_lock<std::mutex> & lock);
    void completeNodeAfterExecute_(Node * n, std::unique_lock<std::mutex> & lock);
    bool tryCompleteNode_(Node * n, std::unique_lock<std::mutex> & lock);

    bool idle_(const std::unique_lock<std::mutex> & lock) const {
        (void) lock;
        return m_running == 0 && m_ready.empty() && m_nonTerminalNodes == 0;
    }

    mutable std::mutex              m_mutex;
    mutable std::condition_variable m_cv;
    bool                            m_stopping         = false;
    mutable int                     m_running          = 0;
    mutable size_t                  m_nonTerminalNodes = 0;
    mutable uint64_t                m_enqueueOrdinal   = 0;
    // Ownership of heap nodes / artifacts; tokens on nodes/artifacts and in m_artifactOrphans.
    ArrayContainer<Node *>     m_nodeRegistry;
    ArrayContainer<Artifact *> m_artifactRegistry;
    /// Owns all heap Token objects; freed in ~OpenGraphImpl (dep tokens, completion, artifact version).
    ArrayContainer<Token *> m_allTokens;
    // Ready queue: lower SchedulingClass and higher int priority first; stable tie-breaker.
    struct ReadyEntry {
        Node *          node = nullptr;
        SchedulingHints hints {};
        uint64_t        ord = 0;
    };
    struct ReadyCompare {
        bool operator()(const ReadyEntry & a, const ReadyEntry & b) const {
            if (a.hints.schedulingClass != b.hints.schedulingClass) {
                return static_cast<int>(a.hints.schedulingClass) > static_cast<int>(b.hints.schedulingClass);
            }
            if (a.hints.priority != b.hints.priority) { return a.hints.priority < b.hints.priority; }
            return a.ord > b.ord;
        }
    };
    std::priority_queue<ReadyEntry, std::vector<ReadyEntry>, ReadyCompare> m_ready;
};

// ============================================================
// OpenGraphImpl — helpers
// ============================================================

void OpenGraphImpl::pushReady_(Node * n) {
    if (!n) { return; }
    ReadyEntry e;
    e.node  = n;
    e.hints = n->desc.scheduling;
    e.ord   = ++m_enqueueOrdinal;
    m_ready.push(e);
    notifyAll_();
}

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

void OpenGraphImpl::completeNodeAfterExecute_(Node * n, std::unique_lock<std::mutex> & lock) { (void) tryCompleteNode_(n, lock); }

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
            int & d;
            explicit Scope(int & d_): d(d_) { ++d; }
            ~Scope() { --d; }
        } scope(s_inGraphExecute);
        if (act && arg) { act->execute(*arg); }
        lock.lock();
        m_running = 0;
        tryCompleteNode_(n, lock);
        if (m_stopping) { break; }
    }
    notifyAll_();
}

// ============================================================
// Opaque pointer promotion
// ============================================================

static Token * promoteToken(TokenPtr p) {
    if (!p) { return nullptr; }
    return static_cast<Token *>(p);
}

static Artifact * promoteArtifact(ArtifactPtr p) {
    if (!p) { return nullptr; }
    return static_cast<Artifact *>(p);
}

// ============================================================
// OpenGraphImpl — Graph
// ============================================================

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

Graph::WaitResult OpenGraphImpl::waitForToken(TokenPtr token) const {
    if (s_inGraphExecute > 0) { return promoteToken(token) && promoteToken(token)->satisfied ? Graph::WaitResult::IDLE : Graph::WaitResult::BUSY; }
    Token * t = promoteToken(token);
    if (!t) { return Graph::WaitResult::FAILED; }
    std::unique_lock lock(m_mutex);
    for (;;) {
        if (m_stopping) { return Graph::WaitResult::FAILED; }
        if (t->satisfied) { return Graph::WaitResult::IDLE; }
        m_cv.wait(lock, [t, this] { return t->satisfied || m_stopping; });
    }
}

ArtifactPtr OpenGraphImpl::createArtifact(const StrA & name) {
    std::lock_guard g(m_mutex);
    auto *          a = new Artifact(name);
    (void) m_artifactRegistry.append(a);
    return static_cast<ArtifactPtr>(a);
}

void OpenGraphImpl::publishArtifact(ArtifactPtr ap, AutoRef<Entity> content) {
    auto * a = promoteArtifact(ap);
    if (!a) { return; }
    std::unique_lock lock(m_mutex);
    a->m_version.increment();
    a->m_content      = std::move(content);
    a->m_hasPublished = true;
    for (size_t i = 0; i < a->m_pending.size();) {
        if (a->m_version >= a->m_pending[i].target) {
            if (a->m_pending[i].token) { satisfyToken_(a->m_pending[i].token, lock); }
            a->m_pending.eraseIdx(i);
        } else {
            ++i;
        }
    }
    notifyAll_();
}

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

NodePtr OpenGraphImpl::addNode(const NodeDesc & desc) {
    std::unique_lock lock(m_mutex);
    auto *           n = new Node(desc);
    if (!n) { return nullptr; }
    (void) collectDeps(n, desc);
    (void) m_nodeRegistry.append(n);
    ++m_nonTerminalNodes;
    if (n->state == Node::State::Ready) { pushReady_(n); }
    notifyAll_();
    return reinterpret_cast<NodePtr>(n);
}

void OpenGraphImpl::satisfyNode(NodePtr node) {
    Node * n = Node::n(node);
    if (!n) { return; }
    std::unique_lock lock(m_mutex);
    if (m_stopping) { return; }
    if (n->state == Node::State::Completed) { return; }
    if (n->state == Node::State::Running) { tryCompleteNode_(n, lock); }
    if (m_running == 0) { pump_(lock); }
    notifyAll_();
}

TokenPtr OpenGraphImpl::getNodeCompletionToken(NodePtr node) {
    Node * n = Node::n(node);
    if (!n) { return nullptr; }
    std::lock_guard g(m_mutex);
    if (!n->m_completion) {
        n->m_completion = new Token("node completion");
        (void) m_allTokens.append(n->m_completion);
    }
    return static_cast<TokenPtr>(n->m_completion);
}

TokenPtr OpenGraphImpl::getArtifactVersionToken(ArtifactPtr ap, NeverOverflowingCounter version) {
    auto * a = promoteArtifact(ap);
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
        (void) a->m_pending.append(p);
    }
    return static_cast<TokenPtr>(t);
}

GN_API Entity::Entity(const GN::rdg::RuntimeType::TypeInfo & type, const StrA & name)
    : RefCounter(), RuntimeType(type), id(nextEntityNeverOverflowingId()), name(name) {}

GN_API AutoRef<Graph> Graph::create() { return AutoRef<Graph>(new OpenGraphImpl()); }

} // namespace GN::rdg2

#include "pch.h"
#include <garnet/GNrdg2.h>

#include <queue>

namespace GN::rdg2 {

static NeverOverflowingCounter nextEntityNeverOverflowId() {
    static std::mutex              m;
    static NeverOverflowingCounter counter = NeverOverflowingCounter::OOO();
    std::lock_guard<std::mutex>    lock(m);
    auto                           out = counter;
    counter.increment();
    return out;
}

struct FOURCC {
    char ch0;
    char ch1;
    char ch2;
    char ch3;

    constexpr FOURCC(): ch0(0), ch1(0), ch2(0), ch3(0) {}
    constexpr FOURCC(const FOURCC & other): ch0(other.ch0), ch1(other.ch1), ch2(other.ch2), ch3(other.ch3) {}
    constexpr FOURCC & operator=(const FOURCC & other) {
        ch0 = other.ch0;
        ch1 = other.ch1;
        ch2 = other.ch2;
        ch3 = other.ch3;
        return *this;
    }

    constexpr FOURCC(char c0, char c1, char c2, char c3): ch0(c0), ch1(c1), ch2(c2), ch3(c3) {}
    constexpr FOURCC(const char * s): ch0(s[0]), ch1(s[1]), ch2(s[2]), ch3(s[3]) {}
    constexpr FOURCC(const std::string_view & s): ch0(s[0]), ch1(s[1]), ch2(s[2]), ch3(s[3]) {}

    constexpr bool operator==(const FOURCC & other) const { return ch0 == other.ch0 && ch1 == other.ch1 && ch2 == other.ch2 && ch3 == other.ch3; }
    constexpr bool operator!=(const FOURCC & other) const { return !(*this == other); }
    constexpr bool operator<(const FOURCC & other) const {
        return ch0 < other.ch0 || (ch0 == other.ch0 && ch1 < other.ch1) || (ch0 == other.ch0 && ch1 == other.ch1 && ch2 < other.ch2) ||
               (ch0 == other.ch0 && ch1 == other.ch1 && ch2 == other.ch2 && ch3 < other.ch3);
    }
};

struct OpaquePointer {
    FOURCC tag;

    OpaquePointer(const FOURCC & tag_): tag(tag_) {}
};

/// Generic satisfiable prerequisite / milestone.
/// May be satisfied internally or externally.
struct TokenImpl : public OpaquePointer {
    static constexpr FOURCC TAG = {"TOKE"};

    static const TokenImpl * promote(const void * ptr) {
        if (!ptr) return nullptr;
        auto typedPtr = static_cast<const TokenImpl *>(ptr);
        if (typedPtr->tag != TAG) return nullptr;
        return typedPtr;
    }

    static TokenImpl * promote(void * ptr) { return const_cast<TokenImpl *>(promote(static_cast<const void *>(ptr))); }

    const StrA name;

    /// Constructor
    Token(const StrA & name_): OpaquePointer(TAG), name(name_) {}
};

struct ArtifactImpl : public OpaquePointer {
    static constexpr FOURCC TAG = {"ARTI"};

    static constexpr const ArtifactImpl * promote(const void * ptr) {
        if (!ptr) return nullptr;
        auto typedPtr = static_cast<const ArtifactImpl *>(ptr);
        if (typedPtr->tag != TAG) return nullptr;
        return typedPtr;
    }

    static ArtifactImpl * promote(void * ptr) { return const_cast<ArtifactImpl *>(promote(static_cast<const void *>(ptr))); }

    const StrA name;

    std::map<NeverOverflowingCounter, AutoRef<Entity>> versions;

    ArtifactImpl(const StrA & name_): OpaquePointer("ARTI"), name(name_) {}

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

    // delete all but the latest versions with reference count equal to 1.
    void purge() {
        if (versions.size() <= 1) return;
        auto latestVersion = versions.rbegin().base();
        for (auto it = versions.begin(); it != latestVersion;) {
            if (it->second->getref() == 1) {
                it = versions.erase(it);
            } else {
                ++it;
            }
        }
        GN_ASSERT(versions.size() == 1);
        GN_ASSERT(versions.begin() == latestVersion);
    }
};

// ============================================================
// Node State
// ============================================================

enum class NodeState : uint8_t { Created, Blocked, Ready, Running, Completed, Cancelled };

// ============================================================
// Node Execution Result
// ============================================================

struct NodeExecutionResult {
    ArrayContainer<NodeDesc>        spawnedChildren;
    ArrayContainer<TokenPtr>        satisfiedTokens;
    ArrayContainer<ArtifactVersion> publishedVersions;
};

// ============================================================
// Node
// ============================================================

struct Node : public OpaquePointer {
    static constexpr FOURCC TAG = {"NODE"};

    static const Node * promote(const void * ptr) {
        if (!ptr) return nullptr;
        auto typedPtr = static_cast<const Node *>(ptr);
        if (typedPtr->tag != TAG) return nullptr;
        return typedPtr;
    }

    static Node * promote(void * ptr) { return const_cast<Node *>(promote(static_cast<const void *>(ptr))); }

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

protected:
    /// Constructor
    Node(const NodeDesc & desc): Entity(TYPE_INFO(), desc.name), mDesc(desc) {}

private:
    NodeDesc mDesc;
};

struct ReadyNode {
    NodePtr         node;
    ActionPtr       action;
    ArgumentsPtr    arguments;
    SchedulingHints scheduling;
};

class OpenGraphImpl final : public Graph {
public:
    OpenGraphImpl()           = default;
    ~OpenGraphImpl() override = default;

    TokenPtr createToken(const StrA & name) override {
        auto tok = AutoRef<Token>(new Token(name));
        (void) m_tokens.append(tok);
        return tok;
    }

    NodePtr addNode(const NodeDesc & desc) override {
        // to be implemented
        (void) desc;
        return nullptr;
    }

    bool publishArtifact(ArtifactVersion version, AutoRef<Entity> content) override {
        // PublishedArtifactEntry & slot = findOrCreatePublishedSlot(version.artifact);

        // if (slot.version.has_value() && (*slot.version).version >= version.version) return false;

        // slot.version = version;

        // TokenPtr versionToken = getOrCreateArtifactVersionToken(version);
        // return satisfyToken(versionToken);
    }

    ArtifactVersion getLatestPublishedVersion(ArtifactPtr artifact) const override {
        // for (size_t i = 0; i < m_publishedArtifacts.size(); ++i) {
        //     const PublishedArtifactEntry & p = m_publishedArtifacts[i];
        //     if (p.artifact == artifact && p.version.has_value()) return *p.version;
        // }
        // return std::nullopt;
    }

    TokenPtr getOrCreateArtifactVersionToken(ArtifactVersion version) override {
        for (size_t i = 0; i < m_artifactVersionTokens.size(); ++i) {
            if (m_artifactVersionTokens[i].version == version) return m_artifactVersionTokens[i].token;
        }

        TokenPtr                  t = createToken("ArtifactVersion");
        ArtifactVersionTokenEntry entry;
        entry.version = version;
        entry.token   = t;
        (void) m_artifactVersionTokens.append(std::move(entry));
        return t;
    }

    bool satisfyToken(TokenPtr token) override {
        if (!token || token->satisfied) return false;

        token->satisfied = true;

        for (size_t wi = 0; wi < token->waiters.size(); ++wi) {
            NodePtr waiter = token->waiters[wi];
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

    bool hasReadyNode() const override { return !m_ready.empty(); }

    bool tryPopReadyNode(ReadyNode & out) override {
        while (!m_ready.empty()) {
            ReadyQueueEntry e = m_ready.top();
            m_ready.pop();

            NodePtr node = e.node;
            if (!node) continue;

            if (node->state != NodeState::Ready) continue;

            node->state            = NodeState::Running;
            node->ownActionStarted = true;

            out.node       = node;
            out.action     = node->desc().action;
            out.arguments  = node->desc().arguments;
            out.scheduling = node->desc().scheduling;
            return true;
        }

        return false;
    }

    bool completeNode(NodePtr node, const NodeExecutionResult & result) override {
        if (!node) return false;

        if (node->state != NodeState::Running && node->state != NodeState::Ready && node->state != NodeState::Blocked) { return false; }

        node->ownActionFinished = true;

        for (size_t i = 0; i < result.spawnedChildren.size(); ++i) {
            NodeDesc child = result.spawnedChildren[i];
            child.parent   = node;
            addNode(child.name, child);
        }

        if (result.terminalState == NodeState::Completed) {
            node->state = NodeState::Running;
            PendingCompletionExtras extra;
            extra.node   = node;
            extra.result = result;
            (void) m_pendingCompletionExtras.append(std::move(extra));
            tryAutoComplete(node);
            return true;
        }

        node->state = result.terminalState;
        onNodeBecameTerminal(node);
        return true;
    }

    bool failNode(NodePtr node) override {
        if (!node) return false;

        if (isTerminal(node->state)) return false;

        node->state             = NodeState::Failed;
        node->ownActionFinished = true;
        onNodeBecameTerminal(node);
        return true;
    }

    bool cancelNode(NodePtr node) override {
        if (!node) return false;

        if (isTerminal(node->state)) return false;

        node->state             = NodeState::Cancelled;
        node->ownActionFinished = true;
        onNodeBecameTerminal(node);
        return true;
    }

    bool markNodeCompleted(NodePtr node) override {
        if (!node) return false;
        if (isTerminal(node->state)) return false;

        node->ownActionFinished = true;
        node->state             = NodeState::Completed;

        finalizeCompletion(node, nullptr);
        return true;
    }

private:
    struct PublishedArtifactEntry {
        ArtifactPtr                    artifact {};
        std::optional<ArtifactVersion> version;
    };

    struct ArtifactVersionTokenEntry {
        ArtifactVersion version {};
        TokenPtr        token = nullptr;
    };

    struct PendingCompletionExtras {
        NodePtr             node = nullptr;
        NodeExecutionResult result;
    };

    struct ReadyQueueEntry {
        NodePtr         node = nullptr;
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

    static bool isTerminal(NodeState s) { return s == NodeState::Completed || s == NodeState::Cancelled || s == NodeState::Failed; }

    uint32_t resolveDependencies(NodePtr nodeRef) {
        uint32_t unresolved = 0;

        const ArrayContainer<Dependency> & deps = nodeRef->desc().dependencies;

        for (size_t i = 0; i < deps.size(); ++i) {
            const Dependency & dep   = deps[i];
            TokenPtr           token = nullptr;

            switch (dep.kind) {
            case DependencyKind::Token:
                token = dep.token;
                break;

            case DependencyKind::ArtifactExactVersion:
                token = getOrCreateArtifactVersionToken(ArtifactVersion {dep.artifact, dep.version});
                break;

            case DependencyKind::LatestPublishedSnapshot: {
                std::optional<ArtifactVersion> latest = getLatestPublishedVersion(dep.artifact);
                if (!latest.has_value()) { continue; }
                token = getOrCreateArtifactVersionToken(*latest);
                break;
            }
            }

            if (!token) continue;

            if (!token->satisfied) {
                ++unresolved;
                (void) token->waiters.append(nodeRef);
            }
        }

        return unresolved;
    }

    void pushReady(NodePtr node) {
        ReadyQueueEntry e;
        e.node           = node;
        e.hints          = node->desc().scheduling;
        e.enqueueOrdinal = ++m_enqueueOrdinal;
        m_ready.push(e);
    }

    void tryAutoComplete(NodePtr node) {
        if (!node) return;

        if (node->desc().completionPolicy == CompletionPolicy::Manual) return;

        const bool ownDone = (!node->desc().action) || node->ownActionFinished;

        const bool subtreeDone = node->sealedChildren && (node->liveChildren == 0);

        const CompletionPolicy policy = node->desc().completionPolicy;

        if (policy == CompletionPolicy::WhenOwnActionCompletes) {
            if (!ownDone) return;
        } else if (policy == CompletionPolicy::WhenSubtreeCompletes) {
            if (!ownDone || !subtreeDone) return;
        } else if (policy == CompletionPolicy::Automatic) {
            if (!ownDone) return;
            if (!node->children.empty() && !subtreeDone) return;
        }

        if (node->state == NodeState::Completed) return;
        if (node->state == NodeState::Cancelled || node->state == NodeState::Failed) return;

        node->state = NodeState::Completed;
        finalizeCompletion(node, findPendingCompletionExtras(node));
    }

    void finalizeCompletion(NodePtr node, PendingCompletionExtras * pending) {
        if (node->completionToken) { satisfyToken(node->completionToken); }

        if (node->desc().autoSatisfyOutputsOnComplete) {
            const ArrayContainer<OutputSpec> & outs = node->desc().outputs;
            for (size_t i = 0; i < outs.size(); ++i) {
                const OutputSpec & output = outs[i];
                switch (output.kind) {
                case OutputKind::SatisfyToken:
                    satisfyToken(output.token);
                    break;
                case OutputKind::PublishArtifactVersion:
                    publishArtifactVersion(output.artifactVersion);
                    break;
                }
            }
        }

        if (pending) {
            for (size_t ti = 0; ti < pending->result.additionalSatisfiedTokens.size(); ++ti) { satisfyToken(pending->result.additionalSatisfiedTokens[ti]); }

            for (size_t vi = 0; vi < pending->result.additionalPublishedVersions.size(); ++vi) {
                publishArtifactVersion(pending->result.additionalPublishedVersions[vi]);
            }

            removePendingCompletionExtras(node);
        }

        onNodeBecameTerminal(node);
    }

    void onNodeBecameTerminal(NodePtr node) {
        if (!node || !node->desc().parent) return;

        NodePtr parent = node->desc().parent;
        if (parent->liveChildren > 0) { --parent->liveChildren; }

        tryAutoComplete(parent);
    }

    PublishedArtifactEntry & findOrCreatePublishedSlot(ArtifactPtr artifact) {
        for (size_t i = 0; i < m_publishedArtifacts.size(); ++i) {
            if (m_publishedArtifacts[i].artifact == artifact) return m_publishedArtifacts[i];
        }

        PublishedArtifactEntry entry;
        entry.artifact = artifact;
        entry.version  = std::nullopt;
        (void) m_publishedArtifacts.append(std::move(entry));
        return m_publishedArtifacts[m_publishedArtifacts.size() - 1];
    }

    PendingCompletionExtras * findPendingCompletionExtras(NodePtr node) {
        for (size_t i = 0; i < m_pendingCompletionExtras.size(); ++i) {
            if (m_pendingCompletionExtras[i].node == node) return &m_pendingCompletionExtras[i];
        }
        return nullptr;
    }

    void removePendingCompletionExtras(NodePtr node) {
        for (size_t i = 0; i < m_pendingCompletionExtras.size(); ++i) {
            if (m_pendingCompletionExtras[i].node == node) {
                m_pendingCompletionExtras.eraseIdx(static_cast<size_t>(i));
                return;
            }
        }
    }

    uint64_t m_enqueueOrdinal = 0;

    ArrayContainer<NodePtr>  m_nodes;
    ArrayContainer<TokenPtr> m_tokens;

    ArrayContainer<PublishedArtifactEntry>    m_publishedArtifacts;
    ArrayContainer<ArtifactVersionTokenEntry> m_artifactVersionTokens;
    ArrayContainer<PendingCompletionExtras>   m_pendingCompletionExtras;

    std::priority_queue<ReadyQueueEntry, std::vector<ReadyQueueEntry>, ReadyQueueCompare> m_ready;
};

GN_API Entity::Entity(const GN::rdg::RuntimeType::TypeInfo & type, const StrA & name)
    : RefCounter(), RuntimeType(type), id(nextEntityNeverOverflowId()), name(name) {}

GN_API AutoRef<Graph> Graph::create() { return AutoRef<Graph>(new OpenGraphImpl()); }

} // namespace GN::rdg2

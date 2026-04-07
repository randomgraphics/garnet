#include "pch.h"
#include <garnet/rdg/open-graph.h>

#include <atomic>
#include <queue>

namespace GN::rdg2 {

static NeverOverflowingCounter nextEntityNeverOverflowId() {
    static std::atomic<uint64_t> counter {1};
    const uint64_t               v = counter.fetch_add(1, std::memory_order_relaxed);
    return NeverOverflowingCounter {v, 0};
}

namespace {

class OpenGraph final : public Graph {
public:
    OpenGraph()           = default;
    ~OpenGraph() override = default;

    TokenPtr createToken(const StrA & name) override;

    NodePtr addNode(const StrA & name, const NodeDesc & desc) override;

    ArrayContainer<NodePtr> addNodes(ArrayContainer<NodeDesc> descs) override;

    void sealChildren(NodePtr node) override;

    bool publishArtifactVersion(ArtifactVersion version) override;

    std::optional<ArtifactVersion> getLatestPublishedVersion(ArtifactPtr artifact) const override;

    TokenPtr getOrCreateArtifactVersionToken(ArtifactVersion version) override;

    bool satisfyToken(TokenPtr token) override;

    bool hasReadyNode() const override;

    bool tryPopReadyNode(ReadyNode & out) override;

    bool completeNode(NodePtr node, const NodeExecutionResult & result) override;

    bool failNode(NodePtr node) override;

    bool cancelNode(NodePtr node) override;

    bool markNodeCompleted(NodePtr node) override;

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

    uint32_t resolveDependencies(NodePtr node);

    void pushReady(NodePtr node);

    void tryAutoComplete(NodePtr node);

    void finalizeCompletion(NodePtr node, PendingCompletionExtras * pending);

    void onNodeBecameTerminal(NodePtr node);

    PublishedArtifactEntry & findOrCreatePublishedSlot(ArtifactPtr artifact);

    PendingCompletionExtras * findPendingCompletionExtras(NodePtr node);

    void removePendingCompletionExtras(NodePtr node);

    uint64_t m_enqueueOrdinal = 0;

    ArrayContainer<NodePtr>  m_nodes;
    ArrayContainer<TokenPtr> m_tokens;

    ArrayContainer<PublishedArtifactEntry>    m_publishedArtifacts;
    ArrayContainer<ArtifactVersionTokenEntry> m_artifactVersionTokens;
    ArrayContainer<PendingCompletionExtras>   m_pendingCompletionExtras;

    std::priority_queue<ReadyQueueEntry, std::vector<ReadyQueueEntry>, ReadyQueueCompare> m_ready;
};

TokenPtr OpenGraph::createToken(const StrA & name) {
    auto tok = AutoRef<Token>(new Token(name));
    (void) m_tokens.append(tok);
    return tok;
}

NodePtr OpenGraph::addNode(const StrA & name, const NodeDesc & desc) {
    auto node = AutoRef<Node>(new Node(name, desc));
    (void) m_nodes.append(node);

    NodePtr parent = desc.parent;
    if (parent) {
        (void) parent->children.append(node);
        ++parent->liveChildren;
    }

    node->unresolvedDependencies = resolveDependencies(node);
    node->state                  = (node->unresolvedDependencies == 0) ? NodeState::Ready : NodeState::Blocked;

    if (node->state == NodeState::Ready) { pushReady(node); }

    return node;
}

ArrayContainer<NodePtr> OpenGraph::addNodes(ArrayContainer<NodeDesc> descs) {
    ArrayContainer<NodePtr> out;
    (void) out.reserve(descs.size());

    for (size_t i = 0; i < descs.size(); ++i) {
        const NodeDesc & d = descs[i];
        (void) out.append(addNode(d.name, d));
    }

    return out;
}

void OpenGraph::sealChildren(NodePtr node) {
    if (!node) return;

    node->sealedChildren = true;
    tryAutoComplete(node);
}

bool OpenGraph::publishArtifactVersion(ArtifactVersion version) {
    PublishedArtifactEntry & slot = findOrCreatePublishedSlot(version.artifact);

    if (slot.version.has_value() && (*slot.version).version >= version.version) return false;

    slot.version = version;

    TokenPtr versionToken = getOrCreateArtifactVersionToken(version);
    return satisfyToken(versionToken);
}

std::optional<ArtifactVersion> OpenGraph::getLatestPublishedVersion(ArtifactPtr artifact) const {
    for (size_t i = 0; i < m_publishedArtifacts.size(); ++i) {
        const PublishedArtifactEntry & p = m_publishedArtifacts[i];
        if (p.artifact == artifact && p.version.has_value()) return *p.version;
    }
    return std::nullopt;
}

TokenPtr OpenGraph::getOrCreateArtifactVersionToken(ArtifactVersion version) {
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

bool OpenGraph::satisfyToken(TokenPtr token) {
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

bool OpenGraph::hasReadyNode() const { return !m_ready.empty(); }

bool OpenGraph::tryPopReadyNode(ReadyNode & out) {
    while (!m_ready.empty()) {
        ReadyQueueEntry e = m_ready.top();
        m_ready.pop();

        NodePtr node = e.node;
        if (!node) continue;

        if (node->state != NodeState::Ready) continue;

        node->state            = NodeState::Running;
        node->ownActionStarted = true;

        out.node       = node;
        out.debugName  = node->effectiveDebugName();
        out.action     = node->desc().action;
        out.arguments  = node->desc().arguments;
        out.scheduling = node->desc().scheduling;
        return true;
    }

    return false;
}

bool OpenGraph::completeNode(NodePtr node, const NodeExecutionResult & result) {
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

bool OpenGraph::failNode(NodePtr node) {
    if (!node) return false;

    if (isTerminal(node->state)) return false;

    node->state             = NodeState::Failed;
    node->ownActionFinished = true;
    onNodeBecameTerminal(node);
    return true;
}

bool OpenGraph::cancelNode(NodePtr node) {
    if (!node) return false;

    if (isTerminal(node->state)) return false;

    node->state             = NodeState::Cancelled;
    node->ownActionFinished = true;
    onNodeBecameTerminal(node);
    return true;
}

bool OpenGraph::markNodeCompleted(NodePtr node) {
    if (!node) return false;
    if (isTerminal(node->state)) return false;

    node->ownActionFinished = true;
    node->state             = NodeState::Completed;

    finalizeCompletion(node, nullptr);
    return true;
}

uint32_t OpenGraph::resolveDependencies(NodePtr nodeRef) {
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

void OpenGraph::pushReady(NodePtr node) {
    ReadyQueueEntry e;
    e.node           = node;
    e.hints          = node->desc().scheduling;
    e.enqueueOrdinal = ++m_enqueueOrdinal;
    m_ready.push(e);
}

void OpenGraph::tryAutoComplete(NodePtr node) {
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

void OpenGraph::finalizeCompletion(NodePtr node, PendingCompletionExtras * pending) {
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

void OpenGraph::onNodeBecameTerminal(NodePtr node) {
    if (!node || !node->desc().parent) return;

    NodePtr parent = node->desc().parent;
    if (parent->liveChildren > 0) { --parent->liveChildren; }

    tryAutoComplete(parent);
}

OpenGraph::PublishedArtifactEntry & OpenGraph::findOrCreatePublishedSlot(ArtifactPtr artifact) {
    for (size_t i = 0; i < m_publishedArtifacts.size(); ++i) {
        if (m_publishedArtifacts[i].artifact == artifact) return m_publishedArtifacts[i];
    }

    PublishedArtifactEntry entry;
    entry.artifact = artifact;
    entry.version  = std::nullopt;
    (void) m_publishedArtifacts.append(std::move(entry));
    return m_publishedArtifacts[m_publishedArtifacts.size() - 1];
}

OpenGraph::PendingCompletionExtras * OpenGraph::findPendingCompletionExtras(NodePtr node) {
    for (size_t i = 0; i < m_pendingCompletionExtras.size(); ++i) {
        if (m_pendingCompletionExtras[i].node == node) return &m_pendingCompletionExtras[i];
    }
    return nullptr;
}

void OpenGraph::removePendingCompletionExtras(NodePtr node) {
    for (size_t i = 0; i < m_pendingCompletionExtras.size(); ++i) {
        if (m_pendingCompletionExtras[i].node == node) {
            m_pendingCompletionExtras.eraseIdx(static_cast<size_t>(i));
            return;
        }
    }
}

} // namespace

GN_API Entity::Entity(const GN::rdg::RuntimeType::TypeInfo & type, const StrA & name)
    : RefCounter(), RuntimeType(type), id(nextEntityNeverOverflowId()), name(name) {}

GN_API AutoRef<Graph> Graph::create() { return AutoRef<Graph>(new OpenGraph()); }

} // namespace GN::rdg2

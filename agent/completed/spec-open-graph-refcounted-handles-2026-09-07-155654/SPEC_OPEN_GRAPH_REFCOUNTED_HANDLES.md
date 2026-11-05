# Spec: RDG2 Open Graph Refcounted Handles

## Problem

RDG2 open graph currently exposes `ArtifactPtr`, `TokenPtr`, and `NodePtr` as
raw opaque pointers. This hides implementation details, but lifetime is managed
by `OpenGraphImpl` raw-pointer registries. FOURCC tags provide a useful internal
validation layer, but raw public ownership still makes it too easy for callers
to keep handles after the graph has destroyed them.

## Goals

- Replace raw opaque public handles with typed refcounted handles.
- Preserve implementation hiding: public handle types expose only minimal
  utility/query methods.
- Make handle lifetime explicit through `AutoRef`.
- Let graph methods detect handles created by a different graph.
- Keep only the latest published artifact content inside the graph/artifact.
- Let callers keep older artifact content alive by holding their own
  `AutoRef<Entity>`.

## Non-Goals

- Do not expose open graph scheduling internals through public handle classes.
- Do not add historical artifact-content storage.
- Do not redesign graph execution, node scheduling, or token satisfaction
  semantics beyond what is required for handle ownership.
- Do not move graph operations onto handles in the first phase. Because handles
  keep a `WeakRef<Graph>`, later phases may move selected graph methods to
  `Artifact`, `Token`, or `Node` if that makes the public interface cleaner.

## Proposed Public API Shape

Replace raw pointer aliases with `AutoRef` aliases:

```cpp
struct Token : public Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);
protected:
    using Entity::Entity;
};
using TokenPtr = AutoRef<Token>;

struct Node : public Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);
protected:
    using Entity::Entity;
};
using NodePtr = AutoRef<Node>;

struct Artifact : public Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);
protected:
    using Entity::Entity;
};
using ArtifactPtr = AutoRef<Artifact>;
```

The public utility surface is intentionally empty for now:

- `Artifact`: no public methods beyond `Entity` initially.
- `Token`: no public methods beyond `Entity`.
- `Node`: no public methods beyond `Entity`.

`Artifact::version() const` was considered as a convenience query, but it does
not replace an existing public method. Do not add it unless a concrete call-site
needs to query an artifact version directly.

## Proposed Private API Shape

The concrete implementation types live in `src/core/rdg2/open-graph.cpp`:

- `TokenImpl final : public Token`
- `NodeImpl final : public Node`
- `ArtifactImpl final : public Artifact`

Private implementation state moves from the current `Token`, `Node`, and
`Artifact` structs into these implementation subclasses.

FOURCC validation should remain available as a private implementation detail in
the concrete implementation classes. It is an early guard for incoming handle
pointers/references. Runtime-type validation and graph-ownership checks are
added on top of it; they do not replace the FOURCC diagnostic path.

## Ownership Model

Public handles are refcounted. A handle remains alive while any caller or graph
internal structure holds an `AutoRef` to it.

The graph no longer owns public handles only through raw deletion registries.
Implementation registries should hold `AutoRef` values where the graph
intentionally owns the handle.

Use `AutoRef` wisely inside graph internals. `AutoRef` has memory and CPU
overhead, so graph-owned structures should hold enough refcounted references to
guarantee handle lifetime, but hot internal adjacency/queue links should prefer
raw pointers/references when another owner already prevents deletion while the
link can be used.

Concretely, the current implementation has these internal raw links that should
be reviewed individually:

- `Token::waiters` stores nodes waiting on a token.
- `Artifact::Pending::token` points to artifact-version tokens.
- `Node::parent` points to the parent node.
- `Node::mCompletion` points to the lazily-created completion token.
- `ReadyEntry::node` points to nodes queued for execution.

For each link, prefer a raw pointer/reference if the graph registry or another
nearby owning structure keeps the target alive and the target cannot be deleted
in the middle of using that link. Use `AutoRef` only when that lifetime guarantee
does not already exist or when ownership needs to cross an async/lifetime
boundary.

For function parameters, prefer references or const references over pointers
when the caller has already checked the null case and the callee does not expect
the incoming object to be null. Keep pointers for optional values, public
entry-point validation, or places where null is a meaningful state.

## Graph Association and Misuse Detection

Each public handle implementation should record the graph that created it. This
enables graph entry points to reject cross-graph misuse, such as publishing an
artifact created by another graph.

Each implementation handle stores a `WeakRef<Graph>` back to the graph that
created it. Handles do not keep their graph alive. A handle can remain alive
after graph destruction, but operations that require graph access fail when the
weak reference cannot be promoted to a strong `AutoRef<Graph>`.

Graph entry points should detect and log:

- Null handles.
- Handles whose internal FOURCC/type does not match the expected implementation
  type.
- Handles from a different graph.
- Stale handles whose creating graph no longer exists, if weak/raw graph
  association is selected.

Error returns should follow existing RDG2 conventions:

- return empty handle/content for failed queries or token creation;
- return `Graph::WaitResult::FAILED` for failed waits;
- log with `GN_ERROR`.

No cross-graph handle use case is currently known. Graph entry points should
reject handles created by another graph rather than delegating to the owning
graph.

## Artifact Content Semantics

An artifact stores exactly one graph-owned content reference: the latest
published `AutoRef<Entity>`.

When new content is published:

1. Artifact version increments.
2. The latest content reference is replaced.
3. The graph/artifact releases its reference to the previous content.
4. Pending version tokens whose targets are reached are satisfied.

Callers that need older content must keep their own `AutoRef<Entity>` returned
by `getArtifactContent()` or `getTypedArtifactContent<T>()`.

## Expected Public Call-Site Changes

- `Graph::createArtifact()` returns `ArtifactPtr` as `AutoRef<Artifact>`.
- `Graph::addNode()` returns `NodePtr` as `AutoRef<Node>`.
- `Graph::getNodeCompletionToken()` returns `TokenPtr` as `AutoRef<Token>`.
- `Graph::getArtifactVersionToken()` returns `TokenPtr` as `AutoRef<Token>`.
- `NodeDesc::dependencies` stores refcounted token handles.
- `NodeDesc::parent` stores a refcounted node handle.
- `VersionedArtifact` stores refcounted artifact and token handles.
- Samples/tests should not use raw pointer truth/lifetime assumptions.

## Resolved Decisions

- Keep `ArtifactPtr`, `TokenPtr`, and `NodePtr` names, but make them aliases to
  `AutoRef<Artifact>`, `AutoRef<Token>`, and `AutoRef<Node>`.
- Make `Artifact`, `Token`, and `Node` public `Entity` subclasses.
- Expose no additional public utility methods on `Artifact`, `Token`, or `Node`
  initially.
- Use `WeakRef<Graph>` as each handle's graph back-reference.
- Handles do not keep their graph alive. After graph destruction, operations
  that need graph access fail when weak-ref promotion fails.
- Reject cross-graph handles; no cross-graph handle use case is currently known.
- Keep FOURCC as a private implementation detail in concrete handle
  implementations.
- Use `AutoRef` internally only where ownership/lifetime requires it; prefer raw
  pointers for hot internal links when another owner guarantees the handle
  cannot be deleted while the link is used.
- Defer moving selected graph methods onto handle classes to a possible second
  phase after the lifetime refactor is complete.

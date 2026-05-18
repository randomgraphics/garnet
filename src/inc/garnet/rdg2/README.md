# Garnet RDG v2 — Design Document

This document captures the design of the `GN::rdg2` module, derived from the
analysis of v1's limitations. It is the authoritative design reference for all v2
implementation work. When in doubt, this document takes precedence over v1's
`README.md`.

> **Status:** Design phase. Not yet implemented. All decisions here are settled
> unless explicitly marked as open questions.

---

## Why v2

Two fundamental defects in v1 motivate a full redesign:

1. **Submission-based interface** — v1's `Submission` object forced all work to be
   declared upfront as a batch. This is hostile to open-ended, continuous workflows
   where passes and resources emerge incrementally as the frame (or background job)
   unfolds.

2. **External GPU2 dependency** — Instead of built-in GPU/API abstraction layer. v2
   now depends on externally maintained GNgpu2.h

---

## Architecture Overview

v2 uses three levels instead of four:

```
┌─────────────────────────────────────────────────────────────┐
│  Level 3 — Scene Management                                 │  ← drives full frame rendering
├─────────────────────────────────────────────────────────────┤
│  Level 2 — Rendering Scenarios                              │  ← global constants, shadow, lighting, skybox
├─────────────────────────────────────────────────────────────┤
│  Level 1 — Open Graph                (open-graph.h)         │  ← domain-independent task/data graph
└─────────────────────────────────────────────────────────────┘
```

---

## Execution Phases: Declare and Execute (No Compile)

Traditional RDG designs use three phases: **declare → compile → execute**. The
compile phase performs global analysis over the complete frame graph — dead node
culling, barrier placement, and transient resource aliasing — before any GPU work
begins. This requires the full graph to be known upfront, which is exactly the
batch-submission model v1 used and v2 discards.

v2 replaces that with two phases: **declare** and **execute**.

**Declare** — incrementally build the graph by calling `graph.addNode()`,
`graph.createArtifact()`, and `graph.getArtifactVersionToken()`. There is no
"close the graph" step. New nodes may be added at any time, including from inside
a running node's action (child nodes).

**Execute** — call `graph.waitForToken()` or `graph.waitForIdle()`. The graph
walks backward from the waited token, collects all unsatisfied ancestors in
dependency order, and runs them on the calling thread. Nothing executes
speculatively; only the subgraph reachable from the waited token runs.

What a traditional compile phase used to do, and where that responsibility now lives:

| Compile concern | v2 answer |
|---|---|
| Dead node culling | Free: only ancestors of the waited token execute |
| Barrier / layout placement | Explicit at L2; the caller places barriers |
| Transient resource aliasing | Lazy pool allocation at node execution time (see L2) |
| PSO compilation | On-demand inside `GpuRaster::seal()` during execution |

The tradeoff: global optimizations that require whole-graph visibility (e.g.,
optimal transient aliasing across an entire frame) are not available. In exchange
the graph is open, streaming, and incrementally buildable — which is the
architectural bet v2 makes.

---

## Level 1 — Open Graph (`open-graph.h`)

### Philosophy

A pure, generic, domain-independent open graph execution model. Zero GPU semantics.
Zero rendering concepts. Could in principle schedule any asynchronous or
heterogeneous workload. GPU-specific concerns (command buffers, fences, barriers,
queue submission) are encoded by L2 on top of L1 primitives — L1 never sees them.

### Core Primitives

#### Node

The fundamental unit of work. A node holds:
- An **action** — a general function/lambda, the logic to execute
- An **argument pack** — data/parameters passed to the action at execution time
- A **completion mode** — auto or manual (see below)
- An optional **parent** node

Nodes are pushed into the graph. A node with no parent is a **top-level node**,
visible to the rest of the graph via a token. A node with a parent is a **child
node**, invisible outside its parent scope.

#### Artifact

A named, versioned resource. Content is completely opaque to L1 — it is just a
slot with a monotonic revision counter.

- A newly created artifact is always at **revision 0** (no content published yet).
- Any **publish** action increments the revision by one and unblocks any node or
  waiter depending on that revision.
- Child nodes may publish artifact revisions. The artifact is the intended escape
  hatch for observing data produced inside a parent scope without exposing internal
  task structure.

#### Token

The single unified primitive for waiting and declaring dependencies. A token is a
handle that represents one of two things:

```
token  ←  node completion      (node reaches COMPLETED state)
token  ←  artifact @ revision  (artifact's revision counter reaches N)
```

Everything that can be waited on or declared as a dependency is expressed as a
token. The graph never distinguishes "waiting for a task" from "waiting for data"
at the edge level.

Tokens are produced by:
- `graph.push(...)` — returns a token for the pushed node's completion
- `artifact.publish(...)` — returns a token for the new revision

**Child nodes do not produce tokens visible outside their parent scope.** They are
invisible to the rest of the graph. Only top-level nodes produce externally usable
tokens.

### Node Lifecycle

```
PENDING → READY → RUNNING → FINISHED → COMPLETED
```

| State     | Meaning |
|-----------|---------|
| PENDING   | Pushed, dependencies not yet satisfied |
| READY     | All input tokens are COMPLETED, eligible to execute |
| RUNNING   | Action is currently executing |
| FINISHED  | Action has returned, but not yet COMPLETED (manual-complete only) |
| COMPLETED | Fully done; downstream nodes may now unblock |

Downstream nodes unblock on **COMPLETED** only, never on FINISHED.

### Auto vs Manual Completion

**Auto-complete node:** When the action returns, the node immediately transitions
FINISHED → COMPLETED and unblocks all dependents. Used for synchronous CPU work.

**Manual-complete node:** When the action returns, the node reaches FINISHED and
stays there. An explicit call to `graph.completeNode(node)` is required to
transition to COMPLETED. This call may come from any thread. Used for async work
whose completion is triggered by an external event (GPU fence, async I/O, etc.).

Example — two-phase GPU execution encoded as two L1 nodes:

```
node A (auto):    action = record + submit command buffer to GPU
                  completes immediately when submit() returns
                  → unblocks node B

node B (manual):  action = register GPU fence wait
                  action returns immediately (non-blocking)
                  GPU fence fires → external thread calls completeNode(B)
                  → unblocks downstream nodes
```

L1 knows nothing about GPU fences. It just sees two nodes.

### Parent / Child Nodes

When pushing a node, the caller may optionally specify a parent node. Rules:

- **`graph.push(..., parent=P)` is valid** if and only if P is not yet COMPLETED
  (P may be in PENDING, READY, RUNNING, or FINISHED state).
- **`graph.push(..., parent=P)` is an error** if P is already COMPLETED.
- Child nodes are **invisible** to the rest of the graph — they cannot be waited
  on and cannot be declared as dependencies by nodes outside the parent scope.
- The parent node is **COMPLETED** only when:
  1. The parent itself is FINISHED (action done, and `completeNode()` called if manual), AND
  2. All children and descendants are COMPLETED, AND
  3. No new children can be pushed (guaranteed by the parent being FINISHED,
     which closes the child-push window).

The parent/child relationship enables **structured concurrency** — a parent node
is a scope that owns a dynamic subtree of work. The outside world holds one token
for the entire scope.

**Both of the following push patterns are valid and identical from L1's perspective:**

Option A — parent's action pushes children (children pushed during RUNNING):
```cpp
// Inside bake_lightmap action:
for (auto& tile : tiles)
    graph.push(bake_tile, tileArgs, parent=thisNode);
graph.completeNode(thisNode);
```

Option B — external code pushes children before parent completes:
```cpp
Token bakeToken = graph.push(bake_lightmap, args);  // top-level
for (auto& tile : tiles)
    graph.push(bake_tile, tileArgs, parent=bakeToken.node());
graph.completeNode(bakeToken.node());
```

L1 enforces only the "parent must not be COMPLETED" invariant.

### Execution Model

**Lazy, demand-driven, single-threaded.**

The graph does not execute speculatively. Nothing runs until something is waited
on. The graph is internally threadless — all execution happens on the thread that
calls `wait()`.

```
wait(token T):
  1. Walk graph backwards from T, collect all ancestors not yet COMPLETED
     in dependency order. Stop at already-COMPLETED nodes.
  2. Loop until T is COMPLETED:
       a. Execute any READY node whose input tokens are all COMPLETED
          (call action synchronously on the calling thread)
       b. If node is auto-complete: transition to COMPLETED, signal its token
       c. If node is manual-complete: transition to FINISHED, wait for
          completeNode() call (sleep until woken by external thread)
       d. On each completeNode() call: re-scan for newly READY nodes, continue
  3. Return when T's token is COMPLETED
```

This means:
- **CPU parallelism** is not L1's responsibility. L2/L3 may push independent
  subgraphs and manage their own threads externally.
- **GPU parallelism** is encoded by L2 as manual-complete nodes that submit to
  multiple queues. L1 just waits for `completeNode()` to be called.
- The only concurrency primitive L1 needs internally is a condvar/futex for
  sleeping while waiting for external `completeNode()` calls.

### Artifact Revision as Progressive Refinement

Because `wait(artifact, min_revision)` unblocks as soon as the artifact reaches
that revision, consumers can declare coarse dependencies on partial results:

```cpp
// Frame lighting pass: "I need at least some lightmap data"
wait(lightmap_atlas, 1);    // unblocks on first completed tile

// Or wait for fully baked result
wait(lightmap_atlas, N*N);
```

Combined with the parent/child model, this enables background frameless workloads
that produce results progressively, consumed by frame-bound rendering without
either side needing to know the other's cadence.

### Drain Model — Frame-bound vs Frameless

L1 supports two natural drain patterns without any special-casing:

- **Frame-bound:** A `Present` node is the sink. The frame renderer calls
  `wait(present_token)` → everything upstream fires in topological order.
- **Frameless:** A background job (lightmap bake, radiosity, global shadow update)
  has its own top-level node or artifact. Whoever needs the result calls
  `wait(artifact, N)` → only the necessary subgraph executes.

Both are just `wait()` calls. L1 has no concept of "frame."

### Summary of L1 Rules

| Rule | Rationale |
|------|-----------|
| No GPU, shader, rendering, or async I/O concept at this level | L1 is domain-independent |
| No backend types in any public header | Backends are interchangeable |
| Child nodes are invisible outside parent scope | Encapsulation of internal task structure |
| `completeNode()` may be called from any thread | Required for external async completion |
| Execution is single-threaded, driven by `wait()` | Simplicity; real parallelism is GPU-side |
| Artifact content is opaque to L1 | L1 only tracks revision numbers |

---

## Level 2 — Rendering Scenarios

### Philosophy

A library of reusable, API-agnostic rendering scenario builders. This is where
GPU rendering policy lives: render loop management, framebuffer management,
shadow map rendering, forward direct lighting, skybox, and similar commonly-used
rendering scenarios.

Each scenario is a **subgraph builder** — it pushes a set of L1 nodes into the
graph that together implement the scenario. Scenarios maintain a shared contract
(resource binding layout, artifact naming conventions) so subgraphs from different
builders can interoperate within the same frame.

**No GPU-API-specific type (`VkBuffer`, `ID3D12Resource`, `MTLBuffer`, etc.) may
appear in any public header at this level.**

### GPU Abstraction Primitives

L2 exposes a set of API-agnostic GPU object types:

- `GpuContext` — device handle; owns GPU submission and completion tracking
- `Swapchain` — swapchain lifecycle and present
- `Texture` — GPU texture (2D, 3D, cube, array); transient variant available
- `Buffer` — GPU buffer; transient variant available
- `Sampler` — sampler state
- `GpuShader` — compiled shader binary
- `GpuRaster` — a raster render pass scope
- `GpuPayload` — opaque handle: both a unit of recorded GPU work and a GPU-side completion signal

Command buffers, fences, semaphores, and individual hardware queue handles are
**not part of the public API**. They are internal implementation details of
`GpuContext`. The only queue concept exposed is `GpuContext::Queue` — a three-tier enum
(Graphics / AsyncCompute / Transfer) that expresses intent; the backend maps it
to actual hardware queues.

### GpuPayload — Universal GPU Dependency Currency

`GpuPayload*` is the single type for GPU-side ordering. It is produced by two
sources, internally carries a fence and semaphore, and is consumed in two places:

| Source | What it represents |
|---|---|
| `GpuRaster::seal()` | Recorded raster pass; submit to GPU via `GpuContext::submit()` |
| `Swapchain::prepare()` | Swapchain image-ready signal (acquire semaphore) |

After `GpuContext::submit()` processes a payload, the payload's internal
fence/semaphore is associated with that submission's completion. The same pointer
is then used to declare downstream dependencies.

| Consumer | Meaning |
|---|---|
| `GpuContext::submit({.waitForGpu = {p}})` | GPU stalls until `p`'s semaphore signals |
| `Swapchain::present(p)` | Present executes on GPU after `p`'s semaphore signals |

### GpuContext::Queue — Queue Tier

`DebugMode`, `Verbosity`, and `Queue` are all direct nested enums of `GpuContext`.

```cpp
struct GpuContext : Entity {
    enum class DebugMode  { DISABLED, ENABLED };
    enum class Verbosity  { SILENCE, BRIEF, VERBOSE };
    enum class Queue {
        GRAPHICS,      // default: graphics pipeline + inline compute
        ASYNC_COMPUTE, // dedicated compute queue; runs concurrently with Graphics
        TRANSFER,      // dedicated DMA queue; copy operations only
    };
    // ...
};
```

Each tier maps to the most capable matching hardware queue. If a dedicated queue
is unavailable it falls back to Graphics. The backend resolves cross-queue
dependencies automatically:

| `waitForGpu` payload origin | Submitting queue | Backend inserts |
|---|---|---|
| Graphics | Graphics | Pipeline barrier (intra-queue) |
| AsyncCompute | Graphics | Semaphore (cross-queue) |
| Graphics | AsyncCompute | Semaphore (cross-queue) |
| Transfer | Graphics | Semaphore (cross-queue) |

### GpuContext

`GpuContext` owns all GPU submission. The `submit()` method accepts a target
queue tier, sealed `GpuPayload` payloads, optional GPU-side dependencies, and an
optional CPU callback fired when the GPU fence signals. `pump()` is called by the
L1 graph's wait loop to check pending fences and fire their callbacks without
blocking.

```cpp
struct GpuContext : Entity {
    struct SubmitParameters {
        StrA                        name;
        Queue                       queue = Queue::GRAPHICS; // target queue tier
        ArrayContainer<GpuPayload*> work;       // sealed GpuPayload objects to execute
        ArrayContainer<GpuPayload*> waitForGpu; // GPU-side deps; barrier or semaphore inserted automatically
        std::function<void()>       onComplete; // CPU callback when GPU fence signals
    };
    virtual void submit(const SubmitParameters &) = 0;
    virtual void pump()                           = 0;
};
```

Async compute example:

```cpp
GpuPayload * skinPayload = skinPass->seal();
ctx->submit({.queue = GpuContext::Queue::ASYNC_COMPUTE, .work = {skinPayload}});

GpuPayload * renderPayload = renderPass->seal();
ctx->submit({
    .queue      = GpuContext::Queue::GRAPHICS,
    .work       = {renderPayload},
    .waitForGpu = {frame.ready, skinPayload}, // cross-queue: backend inserts semaphore
});
swapchain->present(renderPayload);
```

### Swapchain

`prepare()` acquires the next frame image and returns a `Frame` containing the
renderable image view and a `GpuPayload*` (the swapchain acquire signal). Rendering
work that writes to the frame image must list this in `waitForGpu`.

`present(waitFor)` schedules the present to execute on GPU after `waitFor`
completes. For an empty frame with no rendering, pass the `GpuPayload*` from
`prepare()` directly.

```cpp
struct Swapchain : Entity {
    struct Frame {
        GpuResourceView view;  // render into this
        GpuPayload *       ready; // waitForGpu on this before writing to view
    };
    virtual Frame prepare()                  = 0;
    virtual void  present(GpuPayload * waitFor) = 0;
};
```

### GpuRaster

A raster render pass. Created from a `GpuContext` directly — no command buffer
needed. `seal()` finalizes recording and returns a `GpuPayload*` to be passed to
`GpuContext::submit()`.

### Two-Phase GPU Execution (L1 encoding)

Every `GpuContext::submit()` call is encoded as two L1 nodes internally:

```
node 1 (auto-complete):   allocate command buffer, record payloads, submit to queue
                          completes when vkQueueSubmit returns on CPU
                          → unblocks node 2

node 2 (manual-complete): registers onComplete as the fence-wait callback
                          action returns immediately; GPU fence fires
                          → onComplete called from pump() → satisfyNode(node2)
                          → unblocks downstream L1 nodes
```

L1 sees two plain nodes. L2 owns this encoding via `submit()` + `pump()`.
L3 and above never deal with it directly.

### Resource Lifetimes

L2 defines three resource lifetime categories:

| Category   | Lifetime | Examples |
|------------|----------|---------|
| Transient  | Ref-counted; freed when last holding GpuPayload completes on GPU | GBuffer textures, shadow map pingpong |
| Persistent | Explicitly owned; lives until destroyed | Lightmap atlas, global shadow map |
| Imported   | Owned externally, injected into the graph | Swapchain image, engine render targets |

**Transient allocation strategy:** A transient resource carries a ref-count. Each
`GpuRaster::seal()` increments the count for every transient resource it
references. GPU memory is allocated from a pool on the first `GpuContext::submit()`
that encounters the resource. Each `GpuPayload`'s `onComplete` callback decrements
the ref-count for its transients; when the count reaches zero the GPU memory is
returned to the pool. This provides aliasing without a compile phase: two transient
resources that are never live at the same time naturally reuse the same pool slot.

**Cross-submission transient lifetimes** are handled automatically: a transient
referenced by both a shadow pass and a lighting pass in separate `submit()` calls
lives until the last holder's `onComplete` fires, regardless of submission order.

Frame-bound subgraphs typically read from Persistent resources written by
frameless background jobs. L2 tracks cross-cadence read/write hazards via the
artifact revision system at L1: background jobs publish new revisions; frame
subgraphs declare minimum revision dependencies.

### Shared Contract Between Scenario Builders

All L2 scenario builders must follow a shared resource binding contract so their
subgraphs can interoperate within the same frame. (Specific binding layout TBD
during implementation — expected to follow a similar set-based layout to v1's
Level 3 contract.)

---

## Level 3 — Scene Management

### Philosophy

The top-level driver of full frame rendering. Uses L2 scenario builders as tools
to render an entire frame (or background workload). Responsible for:

- Frustum culling, LOD selection, draw call batching
- Composing scenario subgraphs into a coherent frame graph
- Managing the frame-bound / frameless workload boundary
- Driving `wait(present_token)` to flush the frame

### Relationship to L2

L3 uses L2 scenario builders. It does not bypass them. L3 has no direct knowledge
of `CommandBuffer` or `RenderPass` internals — those are L2 concerns.

---

## Key Differences from v1

| Concern | v1 | v2 |
|---------|----|----|
| Execution phases | Declare → compile → execute | Declare → execute (no compile phase) |
| Execution trigger | `Submission` batch flush | `wait(token)` demand-driven |
| Draw call granularity | One action node per draw | Draws are child nodes inside a `RenderPass` scope |
| Frameless workloads | Not supported | First-class via parent/child + artifact revisions |
| Command buffer | Hidden (caused complexity) | Fully internal; `GpuPayload*` is the only public handle |
| Barrier management | Implicit (fragile) | Explicit at L2; L3 may add inference on top |
| Transient resource aliasing | Compile-phase analysis | Lazy pool allocation at execution time |
| Resource lifetimes | Transient / imported | Transient / persistent / imported |
| Level count | 4 | 3 |

---

## Open Questions

- **Multi-queue (async compute + transfer):** In scope for v2 L2, or deferred?
  The two-phase node model supports it naturally but L2 needs to model queue
  ownership transfers explicitly.
- **Automatic barrier inference at L2:** Currently explicit. Should L2 offer an
  opt-in automatic mode for common cases, or leave that entirely to L3?
- **Shared binding contract details:** Exact descriptor set / root signature layout
  for L2 scenario interop — TBD during L2 implementation.
- **L1 thread safety scope:** `completeNode()` must be callable from any thread.
  What other L1 operations (if any) need to be thread-safe? Currently: push and
  wait are assumed to be single-threaded (called from the same owner thread).

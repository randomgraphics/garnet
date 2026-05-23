# Spec: RDG2 Render Graph Execution Layer

## Problem

RDG2 is currently a playground for graph implementation experiments. The current
open graph is one attempt, but this design is the next round: a render graph
execution model that may reuse useful ideas from open graph, but does not have
to build on top of it.

GPU-facing helpers such as shared shader constants and PBR shading can produce
useful `gpu2` resources and payloads, but the missing piece is a generic render
graph execution layer that turns declared rendering quests into ordered,
parallelizable execution and GPU submission.

The versioned artifact model originated from the need to connect long-lived
background producers with frame-scoped rendering. A background quest, such as a
compute lightmap updater, may gradually publish better data over time while the
main renderer consumes stable relics. Relics are therefore a first-class
synchronization and data-handoff mechanism, not just frame-local dependency
bookkeeping.

Concrete rendering helpers must remain clients of the render graph, not concepts
inside the graph. The graph should know about quests, artifact reads/writes,
relic selection, execution dependencies, and collected outputs. It should not
know what a skybox, PBR material, scene constants, or postprocess effect means.

## Goals

- Add a graph-native render-quest model for RDG2 take 2.
- Split processing into explicit phases:
  1. quest declaration,
  2. compile/build,
  3. execution,
  4. output gathering and GPU submission.
- Support both frame-scoped executions and long-lived background executions.
- Let each quest declare artifact inputs and outputs before it executes.
- Build dependency edges from those declarations instead of hard-coding pass
  order in concrete rendering helpers.
- Execute independent CPU-side quests in parallel when possible.
- Let each quest produce relic publications, diagnostics, and sealed
  `gpu2::GpuPayload` values.
- Gather produced GPU payloads and submit them through `gpu2::GpuContext` in a
  single ordered submit for the first implementation.
- Let background producers publish relics over time so frame rendering can
  consume stable relics without blocking on a full recompute.
- Keep SCC, PBR, skybox, and future effects as ordinary clients/adapters used to
  validate the layer.

## Non-Goals

- Do not redesign `gpu2` command recording or resource state tracking.
- Do not make RDG core aware of concrete effects, material models, lighting
  models, cameras, scene constants, or glTF.
- Do not replace `SharedShaderConstants` or `PbrShading` in the first phase.
- Do not require async compute or transfer queue splitting in the first version.
- Do not implement full transient GPU memory aliasing in the first version.
- Do not require speculative execution before an explicit execution request.

## Layering

```text
gpu2
  Low-level GPU resources, command payloads, submit/present.

rdg2 graph experiments
  Open graph and other graph implementations. These are references, not required
  foundations for this design.

rdg2 render graph take 2
  Generic render quest declarations, artifact access analysis, execution plan,
  CPU scheduling, output gathering, GPU payload submission.

concrete render helpers
  SCC, PBR, skybox, postprocess, shadow, debug draw, etc. These may be wrapped
  as render quests, but they are not RDG concepts.
```

## Terminology

Use gpu2 vocabulary as-is when referring to gpu2 concepts. In particular, keep
**GPU resource** and **GPU payload** for objects inherited from the gpu2 module.
RDG vocabulary should describe graph-level items: artifacts, relics, quests,
plans, and executions.

- **Artifact**: A named graph data item with a publication history. Quests read
  from and write to artifacts.
- **Relic**: One immutable published state of an artifact. It carries the stored
  `Entity` and the concrete version assigned by execution.
- **Quest**: A declared executable unit. It declares artifact reads/writes and
  later executes through a `QuestContext`.
- **Plan**: The compiled dependency graph generated from quest declarations. It
  contains quest order, dependency edges, symbolic relic selectors, and payload
  gathering order.
- **Execution**: An active running instance of a plan. Execution owns scheduling
  and may run independent ready quests in parallel. A frame execution is bounded
  by one frame; a background execution is long-lived or incremental.
- **Relic selector**: A symbolic version request such as current-known-good,
  next-published, or latest-at-execution-start. Outside execution, code should
  use selectors rather than concrete version numbers.
- **GPU resource**: A gpu2-owned resource such as a texture, buffer, shader,
  resource set, or render target view.
- **GPU payload**: A sealed gpu2 unit of GPU work submitted through
  `gpu2::GpuContext::submit()`.

Core sentence:

```text
An Execution runs a Plan made of Quests.
Quests read/write Artifacts.
Writes publish Relics.
```

## Public Object Model

Public retained RDG objects inherit `Entity` and use `AutoRef` references:

```cpp
struct Artifact : Entity {};
using ArtifactRef = AutoRef<Artifact>;

struct Relic : Entity {};
using RelicRef = AutoRef<Relic>;

struct Quest : Entity {};
using QuestRef = AutoRef<Quest>;

struct Plan : Entity {};
using PlanRef = AutoRef<Plan>;

struct Execution : Entity {};
using ExecutionRef = AutoRef<Execution>;
```

Use `XxxRef = AutoRef<Xxx>` for refcounted public objects. Reserve `Handle` for
repo/table-managed opaque IDs with explicit validation or generation semantics,
such as the existing repo handle system.

Do not use raw pointer handles for the public API. Value/generation handles can
be revisited later only if refcounted public objects become a real problem. Keep
internal compile bookkeeping such as dependency edges, topological indices, and
ready queues as lightweight value structs rather than `Entity` objects.

## Artifact Discipline

For the first implementation, using `Artifact` directly in multithreaded code is
acceptable if the API enforces clear ownership rules:

- `ArtifactRef` is a shared identity and may cross threads.
- `RelicRef` is immutable after publication and may cross threads.
- Only the execution system may resolve concrete relic versions, query latest
  relics, or publish new relics.
- Declaration, compilation, game-thread frame setup, and normal client code must
  treat artifacts as opaque immutable identities.
- Outside execution, code should not call `artifact->version()`,
  `artifact->latest()`, or `artifact->publish(...)`.
- Outside execution, concrete relic version numbers should not be used for
  ordinary reads. A queried concrete version can become stale immediately.
- If a declaration needs to describe which relic it wants, it should use a
  symbolic selector.

Example selectors:

```cpp
enum class RelicSelector {
    CURRENT_KNOWN_GOOD,
    LATEST_AT_COMPILE,
    LATEST_AT_EXECUTION_START,
    NEXT_PUBLISHED,
    REQUIRED_VERSION,
};
```

Concrete versions are resolved at the synchronization point chosen by execution.
Frame executions should usually capture stable relics before crossing to the
execution thread. Background executions may use latest/required selectors when
their producer/consumer policy needs that behavior.

## Phase 1: Quest Declaration

Declaration is pure description. It does not allocate transient GPU resources,
record commands, submit GPU work, publish relics, or call effect helpers.

A quest should be able to declare:

- a stable human-readable name;
- scheduling hints;
- artifact reads with symbolic relic selectors;
- artifact writes;
- optional explicit before/after quest dependencies;
- optional external GPU dependencies, such as swapchain acquire payloads;
- whether it is required as a final output root or can be culled if unused.

The public shape can start with working names like:

```cpp
enum class ArtifactAccess {
    READ,
    WRITE,
    READ_WRITE,
};

struct ArtifactUse {
    ArtifactRef   artifact;
    ArtifactAccess access = ArtifactAccess::READ;
    RelicSelector selector = RelicSelector::CURRENT_KNOWN_GOOD;
};

struct QuestDeclaration {
    StrA name;
    SchedulingHints scheduling;
    ArrayContainer<ArtifactUse> artifacts;
    ArrayContainer<QuestRef> explicitDependencies;
    bool outputRoot = false;
};

struct Quest : Entity {
    virtual QuestDeclaration declare() const = 0;
    virtual QuestResult execute(QuestContext &) = 0;
};
```

Exact type names may change during implementation, but the split between
declaration and execution should remain.

`QuestBuilder` may exist as an optional local helper for constructing
`QuestDeclaration`, but it is not part of the core quest lifecycle and should not
hold persistent state:

```cpp
QuestDeclaration MyQuest::declare() const {
    QuestBuilder q(name);
    q.read(sceneConstants);
    q.write(colorTarget);
    return q.done();
}
```

## Phase 2: Compile / Build

Compile consumes quest declarations and produces a plan.

Responsibilities:

- validate declarations;
- map artifact reads/writes to relic producers and consumers;
- detect missing producers for required reads unless the artifact is imported;
- reject ambiguous artifact use that cannot be ordered safely;
- detect cycles from artifact dependencies and explicit dependencies;
- cull quests that do not contribute to requested output roots;
- assign deterministic topological execution order;
- build ready groups that may execute in parallel;
- establish deterministic tie-breaking for equal-priority quests;
- construct a payload gathering order that respects artifact dependencies.

Initial dependency rules:

- A read depends on the latest prior write to the same artifact.
- A write depends on all prior reads and writes of that artifact unless the
  writer explicitly declares it discards previous relics.
- Multiple writes to the same artifact are ordered by declaration order unless
  explicit dependencies impose a stronger order.
- Explicit dependencies always add edges, even when no artifact dependency
  requires them.
- Imported artifacts start with already-published relics.

Compile should produce enough information for a dedicated render-quest executor.
It may borrow concepts from the current open graph, such as artifacts, tokens,
versions, or scheduling hints, but it should not be constrained by open graph's
API or execution model.

## Phase 3: Execution

Execution runs a plan. It is the layer that schedules quests, dispatches
independent ready quests in parallel when possible, and then gathers results in
the deterministic order recorded by the plan. Quests receive a context that
exposes only the artifacts and services they declared.

`QuestContext` should provide:

- read access to declared input relics;
- publication helpers for declared output artifacts;
- access to the `gpu2::GpuContext` for command object creation;
- a way to append sealed `gpu2::GpuPayload` outputs;
- diagnostics/logging helpers;
- per-execution scratch allocation if needed later.

Execution rules:

- Quests with no unsatisfied dependencies may execute concurrently with other
  ready quests.
- A quest must not publish undeclared output artifacts.
- A quest must not read undeclared input artifacts.
- A quest may produce zero, one, or many GPU payloads.
- A quest seals GPU payloads but does not submit them directly in the default
  path.
- CPU execution failure marks the execution failed; later policy may allow
  partial results, but the first implementation should prefer fail-fast
  diagnostics.

Mutable artifact operations live behind execution context:

```cpp
struct QuestContext {
    virtual RelicRef read(const ArtifactRef &) const = 0;
    virtual void publish(const ArtifactRef &, AutoRef<Entity>) = 0;
    virtual void emit(AutoRef<gpu2::GpuPayload>) = 0;
    virtual AutoRef<gpu2::GpuContext> gpu() const = 0;
};
```

The first coding slice may execute quests on one thread while preserving the
compiled dependency model. The execution design must still expose ready groups
or equivalent scheduling data so a later slice can add a worker pool without
changing quest declarations.

## Phase 4: Output Gathering And GPU Submission

After CPU quests finish, the graph gathers outputs from all executed quests.

For the first implementation:

- collect all non-null GPU payloads produced by executed quests;
- preserve compiled topological order, with stable declaration-order
  tie-breaking;
- include external GPU dependencies, such as swapchain acquire payloads, as
  submit dependencies;
- call `gpu2::GpuContext::submit()` once with the gathered GPU payload list;
- wire `onComplete` to complete the execution result;
- optionally call `Swapchain::present()` after the final submitted payload when
  a present target is supplied.

This single-submit model intentionally relies on `gpu2` submit-time resource
state tracking to emit barriers across payloads in the same batch.

Future versions may split gathered payloads by queue class, then submit multiple
batches with GPU-side dependencies. That is explicitly deferred until the single
submit path is correct.

## Long-Lived Background Executions

The render graph execution layer must support executions that are not naturally
frame-scoped. Examples include compute-driven lightmap baking, probe updates,
visibility data preparation, streaming texture transcoding, or any other
producer that gradually improves data consumed by regular frame rendering.

Background executions have different lifecycle rules from frame executions:

- they are started, paused, resumed, or cancelled by an owner outside the frame
  loop;
- they may execute incrementally across many frames;
- each step may emit GPU payloads and publish relics;
- they should not block frame rendering unless a caller explicitly waits for a
  required selector/version;
- frame executions should normally read stable relics selected at declaration,
  compile, or execution start according to the chosen selector;
- published relics provide the data handoff from background producers to frame
  renderers.

A useful initial model is:

```text
FrameExecution
  Declares output roots, compiles once, executes required quests, gathers payloads,
  and submits for one frame.

BackgroundExecution
  Owns a persistent plan or repeatedly declares small quest slices, executes when
  budget is available, publishes relics, and exposes progress/cancellation.
```

Background producers should not mutate relics that frame executions are already
using. They publish a new relic when a slice completes. Frame executions can then
consume that relic on a later frame. This mirrors the existing artifact model:
the graph keeps the latest published relic, while callers may hold older relics
alive if a frame already captured one.

Initial scheduling can be conservative:

- frame executions have priority over background executions;
- background executions run only when explicitly pumped or when a caller gives
  the scheduler a CPU/GPU budget;
- background GPU payloads may be gathered into their own submit batch rather
  than mixed into the frame batch;
- background quests may depend on frame-produced/imported artifacts, but this
  should be explicit and versioned through selectors.

Future scheduling can become richer, including budgeted async compute,
multi-queue submission, cancellation-aware worker pools, and progress reporting.
Those are not required for the first frame-rendering slice, but the API should
not make them impossible.

## Artifact And Relic Model

The execution layer should keep the artifact idea for logical graph data, and
use the existing `Artifact` class if it is practical after refactoring. If take 2
needs changes, refactor or extend that class rather than inventing a second
parallel concept.

Recommended initial model:

- each logical graph item is an artifact;
- every write publishes a new relic for that artifact;
- every read captures or resolves a relic according to its selector;
- frame reads of background-produced artifacts may request a stable known-good
  relic, latest-at-execution-start, next-published, or another selector;
- output roots are artifacts or quests requested by the caller;
- imported external artifacts are pre-published relics.

Keep API method names short. Prefer names like `publish`, `latest`, and `wait`
inside execution-owned APIs over longer phrases such as `publishRelic`,
`latestRelic`, or `waitForRelic`.

Potential relic entity types include:

- `GpuPayloadListRelic`;
- `GpuResourceSetRelic`;
- `TextureViewRelic`;
- `RasterTargetRelic`;
- helper-specific relic entities produced by clients.

Do not bake these concrete relic types into core dependency analysis. They are
ordinary `Entity` subclasses carried by artifact relics.

## Error Handling

Compile should fail before running quests when declarations are invalid.

Examples:

- missing producer for a required read;
- undeclared output publication in a quest result;
- explicit dependency references unknown quest;
- artifact dependency cycle;
- incompatible access modes that cannot be ordered;
- requested output root is unknown or culled.

Execution should fail when a quest reports failure or violates its declared
contract. The first implementation should skip GPU submission on failed CPU
execution unless a later design explicitly supports partial submissions.

Background execution failures should be reported on the background execution
object and should not fail unrelated frame executions. A frame execution only
fails because of a background producer when it explicitly requires a relic that
cannot be provided.

## Relationship To SCC And PBR

SCC and PBR are validation clients, not core concepts.

Useful first adapters:

- `SccQuest`: declares a `SceneConstants` or `Set0Resources` output, calls
  `SharedShaderConstants::takeSnapshot()`, publishes a relic containing the
  resource set, and emits the upload payloads returned by that snapshot.
- `PbrQuest`: declares reads of scene constants, material/asset inputs, and
  render target GPU resources, calls `PbrShading::getDrawParams()`, seals a
  raster payload, and emits it.

These adapters prove the graph is usable, but the render graph execution layer
must not depend on their types.

## Resolved Decisions

- Build the generic RDG-to-gpu2 execution layer before moving SCC or PBR between
  modules.
- Use a declaration/compile/execute/gather flow.
- Treat this as render graph take 2, not as an extension of the current open
  graph implementation.
- Reuse open graph concepts only when they prove useful for this design.
- Use `Entity` plus `AutoRef` references for retained public objects: Artifact,
  Relic, Quest, Plan, and Execution.
- Use `XxxRef` for `AutoRef<Xxx>` public references; reserve `Handle` for
  repo/table-managed opaque IDs.
- Keep concrete rendering helpers as ordinary quest clients.
- Gather GPU payloads and submit them once in the first implementation.
- Design for both frame-scoped executions and long-lived background producers.
- Treat artifacts as opaque immutable identities outside execution.
- Resolve concrete relic versions and publish relics only inside execution-owned
  APIs.
- Use artifact relics as the synchronization/data handoff between background
  producers and frame renderers.
- Defer async queue splitting and transient memory aliasing.
- Prefer a small, testable implementation that can later grow into a fuller
  mini-engine.

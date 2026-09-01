# Garnet RDG2 (`GN::rdg2`)

RDG2 is Garnet's experimental render-dependency-graph module. It sits above
`gpu2` and below engine rendering code. Its job is to represent declared work,
order that work through shared data, gather GPU payloads, and coordinate
submission and presentation without learning what a camera, material, light, or
scene means.

This README is the canonical RDG2 design and status document. Design changes
belong here once a real client requires them. Files under `agent/` track only
concrete implementation assignments and their verification steps.

## First Validated Client: E2 Visual Rendering

The first milestone was the E2 `VisualMoment` / `VisualDomain` render path. Its
implementation supplies RDG2's current requirements.

E2 captures a self-contained, immutable `VisualMoment` while the world is at a
consistent synchronization point. `VisualDomain` must render that snapshot
without reaching back into mutable world or form state. It adapts the snapshot
to FX2/gpu2 work and uses RDG2 for the outer frame lifecycle:

```text
World / VisualFacet tree
  -> immutable VisualMoment
  -> VisualDomain
       -> import moment as an RDG2 relic
       -> frame-begin quest: acquire backbuffer and await readiness
       -> visual-render quest: read moment, render, publish backbuffer
       -> frame-end quest: request presentation
  -> one gathered gpu2 submission
```

This integration requires and verifies that RDG2 can:

- represent the immutable visual moment as imported graph data;
- express acquire -> render -> present through declared artifact uses;
- reject a render stage whose required producer is missing;
- expose the acquired swapchain frame to rendering without E2 performing
  submission or presentation directly;
- gather FX2 and raster payloads in deterministic order;
- wait on the swapchain acquire payload, submit gathered work once, and present
  after the final payload;
- keep E2 responsible for interpreting the visual moment and choosing the frame
  workload.

The completed E2 refactor and its verification are recorded in
`agent/completed/E2_VISUAL_BACKEND.txt`.

## Module Boundary

```text
E2                         frame composition and VisualMoment interpretation
FX2                        graph-agnostic effects over gpu2
RDG2                       declared data/work ordering and execution
gpu2                       GPU resources, payloads, submit, and present
```

RDG2 uses `gpu2` concepts such as `GpuPayload`, `GpuContext`, and `Swapchain`,
but it remains graphics-API agnostic. It must not depend on E2 or attach engine
meanings to artifacts and quests. FX2 must not depend on RDG2; E2 is the adapter
that uses both.

## Public Layout

Clients include only `<garnet/GNrdg2.h>`.

- `artifact.h` contains shared graph-independent building blocks: `Entity`,
  `Artifact`, `Artifact::Relic`, and `TypedArtifact`.
- `closed-graph.h` contains the active declare -> compile -> execute API:
  `Quest`, `Plan`, `Execution`, and the frame lifecycle helpers.
- `open-graph.h` contains the earlier generic open DAG executor. It remains
  implemented and tested as reference material, but it is not the current E2
  integration path.

Implementation lives under `src/core/rdg2/`; module-local tests live under
`src/core/rdg2/test/`; samples live under `src/sample/rdg2/`.

## Artifact and Relic Model

An artifact is a stable logical identity for one stream of graph data. A publish
stores a new state and advances the artifact's monotonically increasing 128-bit
version. A relic is the value returned to a consumer:

```text
Artifact::Relic<T> = { AutoRef<T> value, Artifact::Version version }
```

Relics are small values; their content is a refcounted `Entity`. RDG consumers
treat a published relic as immutable. The artifact keeps the latest published
relic, while consumers may retain older relic values. Publishing empty content
is rejected and does not advance the version, so a failed producer leaves the
latest known-good content intact.

Artifact identity defines dependency identity. Two declarations that use the
same artifact communicate through the same logical data. Matching names or GPU
resource descriptions do not make two different artifacts the same resource.
An artifact containing references to other artifacts is ordinary data; it does
not create hidden dependency edges.

Artifacts are independent of either graph implementation. External producers
may publish imported content, while quests in a closed execution read and
publish through `QuestContext` so their accesses can be checked against the
compiled declaration.

## Closed Graph

The active API follows a fixed declare -> compile -> execute flow:

```text
An Execution runs a Plan made of Quests.
Quests read and write Artifacts.
Writes publish Relics.
```

### Quest declaration

A `Quest` returns a pure `QuestDeclaration` and later performs CPU-side command
recording through `QuestContext`. A declaration contains:

- a diagnostic name;
- artifact uses;
- optional explicit quest dependencies.

Each `ArtifactUse` specifies an artifact, an access mode, a selector, and an
optional root marker:

- `READ_ONLY` consumes an existing or earlier-produced relic;
- `DISCARD_WRITE` publishes a complete replacement and does not read the prior
  relic;
- `READ_WRITE` consumes the prior relic and publishes an updated relic.

The declaration is the complete mutable-data frontier for a closed execution.
Quest execution may use immutable values already owned by the quest, but it may
not discover undeclared mutable artifact reads or change the running plan's
topology.

### Compilation

`Plan::compile()` currently preserves declaration order as execution order. In
that order, a read binds to the latest earlier write of the same artifact. If
there is no earlier writer, compilation requires an already-published imported
relic. Explicit dependencies must currently refer to an earlier quest in the
same plan.

The current compiler validates null quests, null artifacts, missing producers,
and invalid explicit dependencies. It does not build a general dependency DAG,
reorder quests, or cull unused work.

The `root` marker identifies a required output but has no effect in the current
compiler because all supplied quests execute.

### Execution

`Execution::run()` currently runs synchronously on the calling thread. It:

1. executes quests in compiled order;
2. rejects undeclared reads and publications;
3. makes each successful quest's publications available to later quests;
4. gathers emitted GPU payloads and external wait payloads;
5. skips submission if CPU execution fails;
6. submits gathered work once; and
7. performs a requested present after the gathered work.

The returned execution is already terminal. GPU completion remains a gpu2
synchronization concern.

### Relic selectors

The API names three selector policies:

- `LATEST_AT_COMPILE`;
- `LATEST_AT_EXECUTION_START`;
- `NEXT_PUBLISHED`.

The current executor reads the latest committed relic when the quest runs; it
does not distinguish these selector values yet.

### Frame helpers

`createFrameBeginQuest()` prepares a swapchain frame, publishes a
`SwapchainFrameEntity` relic, and registers its ready payload as an external
submission dependency. `createFrameEndQuest()` reads the final backbuffer relic,
marks it as a root, and requests presentation. The execution owns actual submit
and present calls.

E2 inserts its visual-render quest between these helpers. That quest reads the
imported visual moment and acquired backbuffer, emits FX2/raster payloads, and
publishes the rendered backbuffer state for the frame-end quest.

## Open Graph

The open graph is a generic runtime DAG executor with `Action`, `Arguments`,
`Token`, `Node`, and `Graph`. Nodes move through blocked, ready, running,
finished-action, and completed states. Manual completion supports external work
such as GPU fences.

The E2 visual path uses the closed graph because its acquire -> render -> present
shape is known before execution.

## Design Rules Established by E2

- A closed plan consumes a sealed render-facing snapshot, not live simulation
  objects.
- E2 decides what work exists; RDG2 orders declared generic work.
- FX2 produces graph-agnostic effects and payloads; it does not declare quests.
- Graph edges come from declarations and explicit dependencies, never from
  arbitrary traversal of artifact contents.
- Runtime data may change what a declared quest does, but not the topology of a
  running closed plan.
- Logical data shares an artifact identity; concrete GPU resource-state tracking
  remains in gpu2.

## Current Status

Implemented and exercised:

- standalone thread-safe versioned artifacts and typed relic reads;
- the open graph reference implementation;
- generic configured quests through `Quest::create()`;
- first-slice closed-plan validation and declaration-order execution;
- staged quest publication and declared-access enforcement;
- gathered single-batch gpu2 submission;
- frame acquire and presentation helpers;
- the `closed-graph-hello-world` sample;
- CPU and Vulkan-backed closed-graph tests;
- the E2 visual backend using the closed frame plan for acquire, render,
  gathered submission, and presentation.

## Design Change Policy

New RDG2 design work starts with a concrete client requirement and acceptance
test recorded in an assignment under `agent/`. Once implemented, its settled
contract is documented here.

## Verification

Use the repository wrappers from an initialized Garnet environment:

```powershell
python.exe env\bin\build.py d
python.exe env\bin\cit.py -t
python.exe env\bin\cit.py -l
```

Relevant coverage is in `src/core/rdg2/test/closed-graph-test.cpp`,
`src/core/e2/test/visual-graph-test.cpp`, and the E2 and RDG2 samples.

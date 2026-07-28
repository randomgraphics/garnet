# Garnet RDG2 (`GN::rdg2`)

RDG2 is Garnet's rendering dependency graph module: it coordinates CPU-side
render work over versioned data and gathers GPU payloads for submission through
the gpu2 module. The core is intentionally generic — it schedules declared work
and knows nothing about concrete rendering effects, materials, cameras, or
scene formats.

```text
gpu2                     low-level GPU resources, command payloads, submit/present
rdg2                     graph designs: artifacts/relics, open graph, closed graph
concrete render helpers  SCC, PBR, skybox, ... — ordinary graph clients, not RDG concepts
```

## Module Layout

- Public include: `GNrdg2.h`. Sub-headers under `garnet/rdg2/` must not be
  included directly (enforced by an `#error` guard in each header).
- Implementation: `src/core/rdg2/`. Tests: `src/core/rdg2/test/` (picked up by
  `GNtest-internal`). Samples: `src/sample/rdg2/`.
- `artifact.h` — shared primitives: `Entity`, `NeverOverflowingCounter`,
  `Artifact`, `Artifact::Relic`, `TypedArtifact`.
- `open-graph.h` — generation 1: a generic open DAG executor. Implemented and
  tested; now reference material.
- `closed-graph.h` — generation 2: the active closed render graph design.
  Public API under iteration; a first executable slice is implemented.
- `shared-shader-constants.h`, `pbr.h` — concrete validation clients.

## Shared Primitives: Artifact, Version, Relic

The artifact is the basic resource of both graph generations. All graph data is
modeled as artifacts from the feature author's point of view; separate logical
needs (shadow-pass depth vs main-pass depth) are separate artifacts even when
their physical GPU storage may alias internally.

- `Artifact` is a named, versioned content slot. It derives from `Entity`
  (refcounted, runtime-typed, process-unique 128-bit id). `publish()` stores
  new content and increments the version, `content()` returns the latest
  relic, and `sigPublished` fires on every publish.
- `Artifact::Relic<C>` is one immutable published state: a small value struct
  pairing the stored content entity with its concrete version. A relic is not
  an `Entity`; it is copied by value and may cross threads freely. The content
  it carries is the refcounted entity.
- Failed work simply does not publish, so consumers keep reading the previous
  known-good relic.
- `TypedArtifact<T>` is a typed view that constrains publish/read to `T` (or
  derived) content.

Core sentence of the closed-graph design:

```text
An Execution runs a Plan made of Quests.
Quests read/write Artifacts.
Writes publish Relics.
```

## Generation 1 — Open Graph (`open-graph.h`), reference material

A generic, domain-independent open DAG executor: nodes wrap actions, become
ready when their dependency tokens are satisfied, and run ordered by scheduling
class, priority, then insertion order. Artifact version tokens let nodes wait
on published data; `manualComplete = true` holds a node in `FinishedAction`
until `satisfyNode()` is called, which encodes externally completed work such
as GPU fences. `Graph::create()` returns a single-worker implementation.

The open graph works and is exercised by tests and samples, but new investment
goes to the closed graph. Known gaps left as-is: multi-worker execution,
descriptor/resource binding paths, indexed draw coverage, and file-path shader
loading.

## Generation 2 — Closed Graph (`closed-graph.h`), active design

Render graph take 2: a declare → compile → execute → gather flow over the
artifact model.

- **Quest** — declared executable unit. `declare()` returns a pure
  `QuestDeclaration`; `execute(QuestContext &)` performs the CPU work.
  Pass-style quests fully described by their artifact uses plus an execute
  callable are created through `Quest::create()`; bespoke subclasses are for
  stateful producers and API-specific effect adapters.
- **ArtifactUse** — one artifact per use, with an access mode (`READ_ONLY`,
  `DISCARD_WRITE`, `READ_WRITE`), a symbolic `RelicSelector`
  (`LATEST_AT_COMPILE`, `LATEST_AT_EXECUTION_START`, `NEXT_PUBLISHED`), and an
  output-root flag that keeps a produced artifact alive even when nothing in
  the same execution reads it.
- **Plan** — the compiled dependency graph: validation, deterministic
  topological order, dead-quest culling, and internal resource lifetime /
  alias planning.
- **Execution** — a running plan instance. It schedules ready quests, gathers
  emitted GPU payloads in deterministic compiled order, and submits them in
  one `gpu2::GpuContext::submit()` batch in the first implementation.
- **QuestContext** — the only window a running quest has onto the graph.
  `read()` returns the relic the plan resolved for a declared use, `publish()`
  stages output relics, `emit()` collects sealed GPU payloads, and `gpu()`
  exposes the gpu2 context.

Key semantics:

- Dependencies come from artifact identity, never from matching descriptors.
  Quests that communicate through the same data must share the same
  `ArtifactRef`.
- Outside execution, artifacts are opaque immutable identities. Concrete relic
  versions are resolved only at execution-chosen synchronization points, and a
  resolved version stays pinned for the whole execution; mid-execution
  publishes become visible to later executions.
- Publication through `QuestContext` is staged: undeclared reads/writes are
  rejected, and output relics commit only when the producing GPU work reaches
  the policy-defined completion point. Failed executions drop their staged
  publications.
- Transient GPU storage and physical aliasing are compiler/backend concerns
  and never appear in quest declarations.
- The core stays API agnostic. API-specific effect implementations live in
  API-prefixed files (`vk-*.cpp`) with full-API-suffixed class names
  (`MyQuestVulkan`).

Status: the public API in `closed-graph.h` is under active design iteration.
Implemented so far: the generic `Quest::create()` factory; a first-slice
compiler (`Plan::compile` — declaration order kept as execution order, with
missing-producer and explicit-dependency validation); a synchronous sequential
executor (`Execution::run` — staged publication committed per quest, one
gathered submission, quest-requested present); and the frame begin/end quest
factories over a `SwapchainFrameEntity` relic. Not yet implemented:
dependency-driven ordering and culling, selector resolution beyond latest,
parallel execution, transient aliasing, and GPU-completion-tracked execution
status. The `PbrQuest` sketch at the bottom of the header is the parked
bespoke-adapter example. The `closed-graph-hello-world` sample renders a
solid-color empty frame through this path.

## Validation Clients

`SharedShaderConstants` (frame-level set0 constants; `takeSnapshot()` freezes
CPU state and returns the resource set plus upload payloads) and `PbrShading`
(PBR raster draw parameters) are concrete helpers used by tests and the
`simple-pbr` sample. They exist to validate graph designs; they must never
define RDG concepts, and the RDG core must not know their types.

## Direction

Work happens on `wip/rdg2/close-graph` and is deliberately design-first; the
detailed task plan returns to the assignment doc once the API settles. The
intended progression:

1. Settle the closed-graph public API shape (ongoing).
2. Declaration validation and compile: artifact dependency edges,
   missing-producer and cycle detection, deterministic ordering, dead-quest
   culling.
3. Execution: sequential first, with ready-group data preserved so a parallel
   worker path can be added without changing quest declarations.
4. Payload gathering and single-batch submission, then transient resource
   binding and aliasing behind the planner.
5. Long-lived background executions that publish relics for frame executions
   to consume through selectors.
6. SCC/PBR wrapped as ordinary quests to validate the full path.

## Further Reading

- `agent/RDG2_RENDER_GRAPH_EXECUTION.txt` — closed-graph design spec and
  iteration notes (authoritative for take 2).
- `src/core/rdg2/design-choices.md` — open vs closed graph white paper,
  including the required usage scenarios checklist that any design change must
  pass.
- `src/core/rdg2/TODO.md` — module implementation TODOs.
- `src/retired/rdg1/` — RDG v1, retired out of the active build; historical
  reference only.

# RDG2 Design Choices: Open Graphs, Closed Plans, and Dynamic Visibility

This note records the design reasoning behind the RDG2 graph experiments. It is
written as a design white paper rather than an API reference. The goal is to
explain the choices we have considered, where each choice works well, and where
it becomes the wrong tool.

The central tension is this:

```text
Real rendering work often discovers dependencies while it is running.
Graph planners want dependencies to be known before they execute work.
```

Examples include culling, streaming, shadow map selection, material variant
selection, and visibility-dependent postprocess work. A renderer might not know
which models, lights, or shadow maps matter until after culling. At the same
time, a graph compiler cannot correctly order work around an artifact that was
read but never declared.

RDG2 should make that tension explicit instead of hiding it behind artifact
contents or accidental reads.

## Engine Phase Model

The closed render graph lives inside a larger engine timeline. It does not own
the game world, and it should not require the game world to stop evolving while a
frame is rendered.

The intended frame-level model is:

```text
Game world:
  evolves continuously at its own pace

Frame boundary:
  sample/freeze the render-facing part of the world
  build a snapshot for one frame

Snapshot preprocessing:
  cull visible objects
  classify visible lights and shadow receivers
  choose LODs
  choose material/effect variants
  decide which render features are active for this snapshot

Closed render graph:
  compile a strict plan from the preprocessed snapshot
  execute the fixed set of render quests
  submit gathered GPU payloads
```

The snapshot is the boundary between game simulation and frame rendering. Work
done after the boundary is allowed to inspect and transform the snapshot, but it
does not read the live world directly. Likewise, changes to the live world after
the snapshot is taken do not affect the already-sealed frame.

This is a key distinction:

```text
Live world mutation affects future snapshots.
Snapshot preprocessing affects the current render plan.
Closed-plan execution consumes the finished snapshot plan.
```

Preprocessing may still affect rendering dependencies. For example, culling may
remove all transparent objects, LOD selection may choose a different mesh
artifact, distance-to-camera may select a simpler material effect, and light
classification may determine which shadow maps are needed. Those decisions are
valid inputs to the closed render graph only because they happen before the plan
is compiled.

Once preprocessing is complete, the engine has a strict static render set for
this frame:

```text
visible objects
selected LODs
selected materials/effects
visible lights
required shadow maps
required postprocess passes
selected render targets
```

That static render set is the input to closed-graph compilation. The closed graph
is then free to validate dependencies, build a plan, schedule independent work,
and gather GPU payloads without worrying that the live world will rewrite its
inputs halfway through execution.

This model makes sense for real-time rendering, but it depends on several
explicit design choices:

- The snapshot must contain enough render-facing data to avoid direct reads from
  the live world during graph execution.
- Culling and preprocessing may be implemented as an open graph, ad-hoc engine
  code, or an earlier closed plan, but their outputs must be sealed before the
  final closed render plan is compiled.
- Asset streaming and game simulation may continue in parallel, but newly
  completed work becomes eligible for a later snapshot unless the engine
  deliberately runs another preprocessing/compile cycle.
- If the presentation surface is unavailable or minimized, the engine can skip
  snapshot creation, preprocessing, closed-plan compilation, and GPU submission
  while leaving simulation, networking, audio, streaming, and other background
  systems free to continue.
- The engine must define what happens when a required render resource is missing
  from the snapshot: use a previous relic, use a fallback, skip the object/pass,
  or block before sealing the snapshot.
- If preprocessing itself needs long-running dynamic work, such as loading
  missing visible assets, that work belongs before the closed render plan or in
  an open/demand-driven graph, not as a hidden read during render execution.

This is not a global game-world lock. It is a render-facing snapshot contract.
The live world can move on; the frame renders the world as it was sampled and
prepared for that frame.

## Core Rule

Every piece of mutable graph state that can affect a quest's execution must be
part of the quest's declared dependency frontier.

The dependency frontier may be:

- a directly declared artifact;
- a compile-time-expanded composite artifact whose children are known before
  planning;
- a coarse declared store, such as a scene or render-resource table;
- an immutable relic/version captured by another artifact;
- or a second graph/plan compiled after a discovery phase.

What should not happen is an undeclared read:

```text
RenderQuest declares READ VisibleList
VisibleList contains ModelB
RenderQuest follows ModelB and reads latest ModelB content
LoadModelBQuest writes ModelB
```

In that situation the planner cannot know whether `LoadModelBQuest` must run
before `RenderQuest`. The dependency exists semantically, but it is absent from
the declaration. That makes the graph unsound.

## Required Usage Scenarios

This section is the compatibility checklist for RDG2 render-graph design. When
the API or implementation changes, verify that each scenario below is still
supported cleanly. This list should be maintained and expanded as new rendering
requirements appear.

The goal is to avoid designs that look simple in isolation but fail an important
real workload. For example, declaring raw `gpu2::GpuResourceView` values directly
is attractive, but it cannot by itself represent graph-provided transient
resources or physical aliasing between distinct logical resources.

### Frame Snapshot Boundary

The live game world evolves continuously. At a frame boundary, the engine samples
the render-facing state into a snapshot, preprocesses that snapshot, then feeds a
sealed render set to the closed render graph.

Expected behavior:

- simulation, streaming, networking, and other world systems can continue while
  the frame renders;
- changes to the live world after snapshot creation do not affect the sealed
  frame;
- snapshot preprocessing may affect render dependencies before the closed plan
  is compiled;
- closed-plan execution consumes the finished snapshot plan without reading the
  live world directly.

Examples of preprocessing that may shape the plan:

```text
cull visible objects
choose LODs
select material/effect variants
classify visible lights
determine required shadow maps
determine required postprocess passes
```

### Minimized Or Occluded Window

When a window is minimized, occluded, or has no presentable surface, the engine
should have a clear boundary where render work can be skipped.

Expected behavior:

```text
Window minimized:
  do not build a render snapshot
  do not compile or execute the closed render plan
  keep game/world/background systems running
```

Frame rendering is downstream of snapshot creation, so skipping presentation
does not require stopping the game.

### Fixed Postprocess Chain

Closed render graphs should handle fixed pass chains whose topology is known
before execution.

```text
SceneColor -> BloomPrefilter -> BloomBlur -> ToneMap -> Backbuffer
```

Expected behavior:

- per-frame values such as exposure, threshold, and render size may vary;
- the dependency shape remains known at compile time;
- the planner can validate order, gather payloads deterministically, and optimize
  temporary GPU resource lifetimes.

### Sealed Visible Draw Manifest

Culling can be regular closed-plan data production when it produces a sealed
manifest rather than new graph topology.

```text
CullQuest writes VisibleDrawManifest
MainRenderQuest reads VisibleDrawManifest
MainRenderQuest writes SceneColor/SceneDepth
```

Expected behavior:

- the manifest may contain many draw records and CPU-side render parameters;
- rendering consumes the manifest as immutable input for this execution;
- the renderer must not discover hidden mutable graph reads by chasing live model
  or material identities during execution.

### Feature-Flagged Frame Passes

Renderer settings can choose different frame plans before compilation.

```text
GBuffer -> SSAO -> Lighting -> TAA
```

Expected behavior:

- if SSAO is enabled for the frame, the plan declares it before compilation;
- if SSAO is disabled, the graph is compiled without it or with a known no-op
  replacement;
- the choice is dynamic at the frame level, but not after execution starts.

### Fixed Background Jobs

Closed graphs should support bounded background jobs when the job shape is known.

```text
GenerateMip0 -> GenerateMip1 -> GenerateMip2 -> PublishTextureResult
```

Expected behavior:

- the job may run asynchronously;
- the job's internal dependencies are fixed;
- consumers can use an older result, wait for the new result, or pick it up in a
  later snapshot.

### Culling As Data Vs Culling As Topology

Culling can be a regular quest only when its result is data inside an
already-declared dependency.

```text
Compile:
  CullQuest writes VisibleList
  RenderQuest reads VisibleList

Execute:
  CullQuest publishes visible draw data
  RenderQuest consumes that data
```

If culling decides which quests or mutable resources exist, then culling is a
discovery/planning phase, not merely data for a fixed plan.

```text
Cull finds visible objects A, B, C
Only then can the engine know it needs LoadB or ShadowMapForLightL
```

Expected behavior:

- data-only culling can be inside the closed plan;
- topology-changing culling must happen before closed-plan compilation or inside
  an open/dynamic graph.

### Visibility-Dependent Shadow Maps

The engine should support skipping shadow map work when no visible receivers need
that shadow map.

Requirement:

```text
Render a shadow map only when visible shadow receivers need it.
```

Acceptable strategies:

```text
Conditional quest:
  Cull -> Visibility
  ShadowMapQuest reads Visibility and may skip
  MainRenderQuest reads Visibility and ShadowMap

Phased plan:
  Cull/classify receivers
  Compile concrete plan with only needed shadow maps
  Execute concrete plan

Open graph:
  Cull node discovers receiver/light set
  It schedules only the needed shadow-map nodes
  Render waits on the concrete tokens created by discovery
```

Expected behavior:

- a fixed closed plan may keep a conditional shadow quest that no-ops;
- a phased/open design may omit the shadow work entirely;
- hidden artifact/resource reads must not be used to smuggle the dependency into
  the graph after planning.

### Logical Resource Identity

The graph needs logical GPU resource identity separate from physical GPU
allocation. The graph should not infer resource identity from matching
descriptions.

Example: pre-Z and main rendering share one logical scene depth resource.

```text
sceneDepth logical resource

PreZQuest:
  DISCARD_WRITE sceneDepth

MainRenderQuest:
  READ_ONLY sceneDepth
```

Expected behavior:

- both uses name the same logical resource;
- the planner knows `MainRenderQuest` depends on `PreZQuest`;
- the descriptor says what kind of texture can satisfy `sceneDepth`, but does
  not define sameness.

### Physical Resource Aliasing

Different logical resources may share the same physical GPU allocation when it is
safe.

```text
shadowDepth logical resource
sceneDepth logical resource

ShadowMapQuest:
  DISCARD_WRITE shadowDepth

PreZQuest:
  DISCARD_WRITE sceneDepth

MainRenderQuest:
  READ_ONLY sceneDepth
```

Expected behavior:

- `shadowDepth` and `sceneDepth` remain semantically independent because they are
  different logical resources;
- if both are transient, their lifetimes do not overlap, and their descriptors
  are compatible, the backend may map both to the same physical gpu2 texture;
- physical aliasing is not observable graph semantics.

```text
shadowDepth -> physical depth texture #12 during shadow pass
sceneDepth  -> physical depth texture #12 during main scene pass
```

### Concrete Imported Resources

Quests must be able to use concrete existing GPU resources such as material
textures owned by a PBR asset.

Expected behavior:

- concrete imported resources participate in dependency and barrier planning;
- imported resources are not transient graph allocations;
- the graph still reasons about the actual view range used by the quest.

### Graph-Provided Resources

Quests must be able to request logical GPU resources that the graph/framework
allocates or aliases.

Examples:

```text
scene depth
main color
gbuffer normal
shadow atlas
TAA history output
```

Expected behavior:

- the quest declares the logical resource and required properties;
- the graph backend resolves it to a concrete gpu2 resource/view during planning;
- multiple quests share the same resource by sharing logical identity, not by
  independently declaring matching descriptors.

### Partial Resource Overlap

The graph must reason about hazards at GPU view granularity.

Expected behavior:

```text
write texture T mip 0, layer 0
read  texture T mip 1, layer 0

no overlap -> no read/write hazard
```

```text
write buffer bytes [0, 1024)
read  buffer bytes [2048, 4096)

no overlap -> no read/write hazard
```

If two views overlap in texture subresource range or buffer byte range, the
planner must treat read/write combinations as hazards. If they do not overlap,
the planner should not serialize them merely because they refer to the same
resource object.

## Artifacts Containing Other Artifacts

An artifact may contain references to other artifacts as ordinary payload data.
For example, a visible-object list may contain model artifact identities. That
does not by itself create graph edges.

```text
Artifact containment is data.
Quest declaration is dependency.
```

Containment can become a dependency mechanism only if the design explicitly says
that a composite artifact is expanded during declaration or plan compilation.
That expansion is only valid when the contained set is known at compile time. If
the contained set is produced by `execute()`, it cannot retroactively modify a
plan that is already executing.

This keeps a simple invariant: graph edges come from declarations and explicit
graph operations, not from arbitrary payload traversal.

## Closed Graph / Closed Plan

A closed graph is the traditional declare-compile-plan-execute model.

```text
declare quests -> compile dependency plan -> execute fixed plan
```

After compilation, no new dependencies are discovered and no new quests are
inserted into that execution. A compiled `Plan` is a closed description of what
may run, what artifacts it may read/write, and how outputs are gathered.

Possible names:

- `ClosedGraph`, as the conceptual opposite of `OpenGraph`;
- `FrameGraph`, if the API should emphasize frame rendering;
- `FramePlan`, for the compiled result of a closed frame graph;
- `Plan`, for the current generic compiled representation.

The clearest vocabulary is:

```text
OpenGraph    = runtime-expanding dependency graph
ClosedGraph  = fixed dependency graph compiled before execution
Plan         = compiled instance of a ClosedGraph
Execution    = one run of a Plan
```

### Strengths

- Easy to validate. Every quest declares its artifact reads and writes before
  execution.
- Easy to schedule. The compiler can topologically sort, find independent work,
  and avoid runtime dependency surprises.
- Easier to reason about in tests. Given the same declarations and selected
  relics, the plan shape is deterministic.
- Good fit for GPU payload gathering and submission order.
- Good fit for future resource lifetime analysis and transient aliasing when a
  whole execution is visible.

### Costs

- A quest cannot discover new dependencies during `execute()`.
- Visibility cannot naturally remove or add work after planning.
- Runtime-discovered dependency sets require either coarse dependencies,
  conditional no-op quests, immutable snapshots, or a phase break.
- If used too strictly, it can force a frame to consume a render-state snapshot
  instead of reacting immediately to work that completed mid-frame.

### Restrictions

A closed render graph is intentionally restrictive. Those restrictions are what
make validation, scheduling, and resource planning tractable.

- All quests that may run in an execution must be known before the plan starts.
- All artifact read/write sets must be declared before execution.
- Quest execution may change artifact contents, but not the dependency topology
  of the running plan.
- Runtime-generated lists may be consumed as data, but they may not introduce
  new mutable artifact reads unless those reads are already covered by the
  declaration frontier.
- A quest may skip work at execution time, but the skipped quest still exists in
  the plan unless a prior discovery phase removed it before compilation.
- Newly published artifact versions may be ignored by an already-sealed
  execution if that execution selected an older relic or snapshot.
- A closed plan should not block on arbitrary "maybe needed" work that was not
  declared as part of the plan. If the dependency is conditional, the condition
  must be represented explicitly by a manifest, a conditional quest, or a phase
  break.

These rules mean a closed graph is best for bounded rendering work whose shape is
already known, or whose dynamic decisions can be represented as data rather than
new graph edges.

## Snapshot / Relic Manifest Model

The snapshot model solves late-bound object lists by sealing the concrete
versions to be consumed.

Instead of publishing:

```text
VisibleList = [ModelAArtifact, ModelBArtifact, ModelCArtifact]
```

culling publishes:

```text
VisibleList = [
  { object: A, renderable: ModelA version 7 },
  { object: B, renderable: ModelB version 3 },
  { object: C, renderable: ModelC version 12 },
]
```

The renderer does not chase `ModelBArtifact` and read its latest content. It
renders the sealed relic or draw record captured by the visible-list relic.

This means a `LoadModelBQuest` that later publishes version 4 does not affect the
current render. Version 4 becomes eligible for a later visible-list publication
or a later frame snapshot.

### Strengths

- No recompile is needed after culling.
- The closed plan remains sound: `RenderQuest` truly depends only on the visible
  manifest and any declared shared stores.
- The frame is internally consistent. It does not observe half of a model update
  or half of a resource table transition.
- Asset streaming and rendering can run concurrently without forcing the frame
  to wait for every possible asset update.

### Costs

- The frame consumes a render-facing snapshot. New work that completes during
  the frame may not appear until a later frame.
- The visible manifest has to be rich enough to prevent hidden reads.
- It is not ideal when the renderer must use a newly loaded object in the same
  execution that discovered the object was visible.

The important nuance is that this does not require snapshotting the entire game
world. It snapshots the render-facing state that this execution is allowed to
observe: transforms, renderable versions, material table versions, texture
residency table versions, and similar data.

## Coarse Dependency Model

Another closed-plan option is to declare a coarse stable store:

```text
RenderQuest reads VisibleListArtifact
RenderQuest reads SceneRenderStateArtifact
LoadModelBQuest writes SceneRenderStateArtifact
```

The planner then sees a dependency between loading and rendering through the
shared store.

This works only if the loading quest really writes a store that the renderer
declares. It does not help when loading is object-specific and the renderer
should wait only if that specific object is visible. The dependency becomes too
coarse and may serialize unrelated work.

### Strengths

- Simple declaration model.
- Preserves closed-plan validation.
- Good for small engines or systems where coarse invalidation is acceptable.

### Costs

- Can over-synchronize.
- Can cause visible latency when unrelated updates block rendering.
- Does not express "wait for B only if B is visible."

## Conditional Quest Model

A closed graph may include quests that no-op when their condition is false:

```text
CullQuest writes VisibilityArtifact
ShadowMapQuest reads VisibilityArtifact
ShadowMapQuest skips itself when no visible receivers need shadows
MainRenderQuest reads VisibilityArtifact and ShadowMapArtifact
```

This handles a shadow-pass example without changing graph topology. The shadow
quest is present in the plan but may do no work.

### Strengths

- Very easy to implement.
- Keeps one compile-plan-execute flow.
- Good when the skipped work is cheap to check and the resource planning cost is
  acceptable.

### Costs

- The planner cannot fully eliminate the quest ahead of time.
- Resource planning may remain conservative.
- The dependency graph says "shadow map may exist" even when this frame proves
  it does not.
- If many passes are conditional, the plan can become a large skeleton of mostly
  inactive work.

## Open Graph

An open graph allows nodes, artifacts, and dependencies to appear as execution
discovers facts.

```text
CullQuest executes
  discovers visible objects A, B, C
  schedules load/wait/render/shadow work for A, B, C
  publishes completion tokens or artifacts for downstream work
```

This is closer to a task runtime than a traditional frame graph. A parent node
can own a dynamic subtree of child work, and the outside world waits on a token
for the whole scope.

### Strengths

- Natural fit for visibility-dependent work.
- Natural fit for streaming, asset loading, hot reload, and other long-lived
  background producers.
- Does not require pretending that unknown work is known before execution.
- Can skip shadow maps, material preparation, skinning, or other passes when
  culling proves they are unnecessary.

### Costs

- Harder to validate globally. Dependencies may be created while work is already
  running.
- Harder to optimize with whole-frame analysis.
- Needs explicit semantics for continuations, parent/child scopes, cancellation,
  starvation, cycles, and when the graph is considered closed enough to present.
- More difficult to make deterministic than a closed plan.
- Resource aliasing and barrier planning become local/lazy rather than globally
  optimal unless additional analysis is introduced.

Open graph is the right tool when discovery changes the shape of work.

Closed graph is the right tool when discovery only changes data consumed by work
that was already declared.

## Phased Discovery and Closed Rendering

A hybrid model splits the frame into discovery and concrete rendering:

```text
Phase 1:
  discover/cull/classify visible objects, lights, and required passes

Phase 2:
  compile a closed plan from the discovered concrete set

Phase 3:
  execute that closed plan
```

For the shadow example:

```text
Discovery:
  cull visible receivers
  determine which lights need shadow maps

Compile:
  add ShadowMapQuest(light L) only when L has visible receivers
  make MainRenderQuest depend on the concrete shadow maps it uses

Execute:
  run the concrete plan
```

### Strengths

- Visibility can truly shape graph topology.
- The render execution itself can still be a closed plan.
- The planner can eliminate passes and allocate resources for the actual frame,
  not for the worst case.

### Costs

- There is a phase barrier between discovery and rendering.
- Compile cost moves into the frame unless plans are cached or incrementally
  reused.
- The engine needs clear rules for data lifetime between phases.
- Late work after the phase barrier still affects later frames unless the engine
  allows another discovery/compile loop.

This model is often a good compromise: use open or ad-hoc discovery to decide
what the frame needs, then use a closed plan to execute the concrete GPU work.

## Recommended RDG2 Direction

RDG2 should not force one graph model to solve every problem. The useful split
is:

```text
OpenGraph:
  dynamic task/data graph for discovery, streaming, loading, hot reload, and
  visibility-shaped work.

ClosedGraph / FramePlan:
  fixed dependency graph for deterministic frame execution, payload gathering,
  validation, and future resource planning.

Shared primitives:
  artifacts, relics, selectors, quest declarations, tokens, GPU payloads, and
  resource repositories.
```

The render path can then choose the appropriate strategy:

- Use a closed plan when all dependencies are known before execution.
- Use a relic manifest when culling produces data but not graph topology.
- Use conditional quests when the cost of inactive work is acceptable.
- Use phased discovery when visibility should shape the concrete frame plan.
- Use open graph when work discovery is inherently incremental or long-lived.

The design should avoid hidden dependencies in all cases. If a visible list
contains model identities, that is data. If the renderer must read those models'
latest mutable contents, the reads must be declared somehow or moved into a
later plan that can declare them.

## Design Invariants

These invariants should guide implementation choices:

- Any future design change must be checked against the Required Usage Scenarios
  section before it is considered acceptable.
- Quest declarations must describe all mutable state a quest may observe, unless
  that state is captured into an immutable relic consumed through a declared
  artifact.
- Artifact containment does not imply dependency edges.
- Runtime-generated manifests may contain artifact identities, but execution
  must not follow those identities to read latest mutable content unless the read
  is declared by another mechanism.
- A closed plan cannot gain new dependencies after execution begins.
- If culling changes data, it can be a regular quest in a closed plan.
- If culling changes graph topology, it is a discovery/planning step or an open
  graph operation.
- Asset streaming should generally publish new versions for future frames rather
  than mutating the state observed by an already sealed render execution.
- The graph model should make skipped work, delayed work, and future-frame work
  explicit rather than relying on accidental timing.

## Open Questions

- What public names should be used for the closed-plan API: `ClosedGraph`,
  `FrameGraph`, or only `Plan`/`Execution`?
- Should phased discovery be a first-class RDG2 API or a pattern built by client
  code using OpenGraph plus Plan compilation?
- How much conditional work is acceptable before a frame should switch to
  phased discovery?
- Should render-facing snapshots be explicit objects, or should they emerge from
  relic selectors and manifests?
- How should plan caching work when visibility produces similar but not identical
  frame plans across frames?
- What is the minimum continuation API needed for OpenGraph to express
  visibility-shaped work without making every quest a fully dynamic task?

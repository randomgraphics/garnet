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
- Let feature authors see all graph data as artifacts with declared usage, while
  the compiler/planner internally optimizes short-lived physical resources and
  potential aliasing.
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
- Do not expose transient allocation or aliasing as a feature-author concern.
  The first version may allocate conservatively, but the plan should still keep
  enough internal lifetime information to optimize physical resources later.
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
- **Execution-local resource**: An internal planner classification for physical
  GPU storage whose contents are only needed for a bounded interval inside one
  execution. Feature authors still see ordinary artifacts.
- **Physical GPU resource alias**: A plan/backend decision that maps multiple
  non-overlapping execution-local artifacts to the same compatible gpu2
  allocation. Alias decisions are not visible to quests; quests only name
  logical artifacts.
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

Public RDG2 interfaces should remain pure virtual wherever that is reasonable,
matching the existing RDG2 style. Concrete implementations should live behind
factories and internal implementation classes so callers see stable abstract
interfaces rather than backend or scheduler details.

The core render graph implementation should be API agnostic. It may gather and
submit `gpu2::GpuPayload` values, but it should not branch on Vulkan, D3D12,
Metal, OpenGL, or any concrete rendering effect. Backend-specific optimization
belongs in concrete client adapters or gpu2 backend code, not in artifact,
quest, plan, or execution dependency analysis.

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
};
```

Concrete versions are resolved at the synchronization point chosen by execution.
Frame executions should usually capture stable relics before crossing to the
execution thread. Background executions may use latest or next-published selectors when
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
- whether its outputs are required output roots that must be preserved even when
  not referenced by another quest in the same execution.

The public shape can start with working names like:

```cpp
enum class ArtifactAccess {
    READ_ONLY,     // read an existing relic selected by selector
    DISCARD_WRITE, // discard old contents, then write a complete replacement
    READ_WRITE,    // read existing relic, then publish an updated relic
};

struct ArtifactUse {
    ArrayContainer<ArtifactRef> artifacts;
    ArtifactAccess              access = ArtifactAccess::READ_ONLY;
    RelicSelector               selector = RelicSelector::CURRENT_KNOWN_GOOD;
};

struct QuestDeclaration {
    StrA name;
    SchedulingHints scheduling;
    ArrayContainer<ArtifactUse> artifactUses;
    ArrayContainer<QuestRef> explicitDependencies;
    // Root outputs are retained even when no other quest in this execution reads them.
    bool outputRoot = false;
};

struct Quest : Entity {
    virtual QuestDeclaration declare() const = 0;
    virtual QuestResult execute(QuestContext &) = 0;
};
```

Exact type names may change during implementation, but the split between
declaration and execution should remain.

Each `ArtifactUse` groups artifacts with the same access policy. This keeps
declarations compact for common cases such as a quest reading many input
textures with the same selector.

`DISCARD_WRITE` means write-only/full replacement. The quest promises it does
not need the prior contents of the artifact and will publish a complete
replacement. This creates no dependency on previous contents of the same
artifact.

`READ_WRITE` means the quest needs the existing relic and publishes an updated
relic. Use this for partial updates, accumulation, blending, append/update
operations, preserving parts of a render target, or any operation where old
artifact content affects the result.

Quest declarations should not expose physical lifetime policy. A feature author
declares reads and writes; the compiler decides which produced artifacts are
needed by downstream quests, which outputs must be preserved because they are
roots, and which physical resources can be recycled or aliased internally.

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

### Typed Feature Declarations Over The Generic Core

The core render graph API stays generic so the framework can compile, schedule,
validate, alias, and submit any graph shape. That does not mean feature authors
should normally hand-author anonymous arrays, maps, or string-keyed containers.
Engine layers built on top of RDG should provide strongly typed declaration
adapters that lower into the generic `QuestDeclaration` format.

In that model, `ArtifactUse` is the interchange representation consumed by the
compiler. A feature-specific declaration struct is the author-facing
representation. Each typed field describes one logical input or output, including
access mode, selector, multiplicity, and resource requirements. The framework
binds those fields to actual artifacts and relics when it builds and executes a
plan.

The bridge has three parts:

- **Typed declaration fields**: a feature declares named fields such as
  `sceneConstants`, `models`, `depth`, and `shadowMap`. These fields carry
  `READ_ONLY`, `DISCARD_WRITE`, or `READ_WRITE`, selector policy, and optional
  descriptor constraints. They do not need to contain concrete artifact
  instances when authored.
- **Lowering to generic uses**: the engine adapter enumerates the typed fields
  and emits `QuestDeclaration::artifactUses` for the core compiler. This is where
  descriptor constraints become planner-visible requirements for resource
  allocation and aliasing.
- **Bound execution view**: during execution the framework provides the same
  typed declaration shape, but with each field resolved to usable relics,
  provisional output resources, and helper methods such as `getRelic()` or
  `getRelics()`. The feature code reads named fields rather than searching a
  generic container.

Illustrative shadow-map feature:

```cpp
class ShadowMapFeature {
public:
    struct Declaration {
        TypedArtifactUse<SceneConstantsRelic> sceneConstants = {
            .access   = ArtifactAccess::READ_ONLY,
            .selector = RelicSelector::CURRENT_KNOWN_GOOD,
        };

        int lightIndex = 0;

        TypedArtifactUse<ModelRelicList> models = {
            .access   = ArtifactAccess::READ_ONLY,
            .selector = RelicSelector::CURRENT_KNOWN_GOOD,
        };

        TypedArtifactUse<DepthTargetRelic> depth = {
            .access       = ArtifactAccess::DISCARD_WRITE,
            .width        = 1024,
            .height       = 1024,
            .minDepthBits = 24,
        };

        TypedArtifactUse<ShadowMapRelic> shadowMap = {
            .access = ArtifactAccess::DISCARD_WRITE,
            .width  = 1024,
            .height = 1024,
            .format = ShadowMapFormat::FLOAT32,
        };
    };

    ArrayContainer<AutoRef<gpu2::GpuPayload>> execute(BoundDeclaration<Declaration> & decl) {
        auto sceneConstants = decl.sceneConstants.getRelic();
        auto models         = decl.models.getRelics();
        auto depth          = decl.depth.getRelic();
        auto shadowMap      = decl.shadowMap.getRelic();

        auto raster = decl.gpu()->createRasterPayload("shadow map");
        raster->begin({.color = shadowMap, .depth = depth});
        raster->setLight(sceneConstants, decl.lightIndex);

        for (auto model : models) {
            raster->draw(model);
        }

        return {raster->seal()};
    }
};
```

The sample above is intentionally feature-facing pseudo-code, not a required
core API. The key design target is that the feature author only names typed
inputs and outputs and writes the rendering logic. The framework performs the
generic work:

- resolve `READ_ONLY` fields to the selected relics;
- allocate or bind `DISCARD_WRITE` fields to fresh output relic/resource
  placeholders whose old contents are irrelevant;
- resolve `READ_WRITE` fields to prior contents plus a publishable update target;
- validate that execution only touches declared fields;
- lower typed fields to generic artifact dependencies and resource lifetime
  intervals;
- commit output relic publication only when the retained GPU work that produces
  those relics is accepted and reaches the policy-defined completion point.

Manual publication from feature code should be uncommon in this typed layer.
For GPU-produced outputs, publication is usually tied to gathered payload
submission/completion rather than the CPU moment when the payload is sealed. If a
payload is culled or dropped by policy, its provisional output relics are dropped
or recycled instead of being made visible as completed data.

## Phase 2: Compile / Build

Compile consumes quest declarations and produces a plan.

Responsibilities:

- validate declarations;
- map artifact reads/writes to relic producers and consumers;
- infer internal physical resource lifetimes from first use to last use;
- identify execution-local artifacts whose lifetimes do not overlap and whose
  framework-provided resource descriptors are alias-compatible;
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
- A discard-write access publishes a complete replacement relic for that
  artifact and does not depend on the previous relic.
- A read-write access depends on the latest prior write to the same artifact and
  publishes an updated relic.
- The planner preserves a published relic only when it is read by another
  retained quest, selected by an execution policy, imported/exported, or
  produced by an output root.
- Multiple writes to the same artifact are ordered by declaration order unless
  explicit dependencies impose a stronger order.
- Explicit dependencies always add edges, even when no artifact dependency
  requires them.
- Imported artifacts start with already-published relics.

Internal resource lifetime and alias planning rules:

- Feature authors do not declare artifacts as transient. They declare ordinary
  artifacts and their intended reads/writes; the compiler classifies
  execution-local physical storage internally.
- Logical artifacts stay distinct even when their physical resources may alias.
  For example, shadow-pass depth and main-pass depth should be separate
  artifacts even if the allocator later maps both to the same image allocation.
- Lifetime intervals are derived from the compiled order and all retained
  artifact reads and writes. One execution-local artifact can only alias another
  when their GPU lifetimes do not overlap.
- Resource descriptors are supplied by the artifact instance, framework factory,
  or concrete adapter rather than by every quest use. Descriptors must be
  compatible before aliasing. For textures this includes dimensions or
  allocation size, format/aspect, usage flags, sample count, tiling, memory
  class, and alignment. For buffers it includes size, usage flags, memory class,
  mapping/coherency requirements, and alignment.
- Aliasing must respect queue ownership and GPU-side execution overlap. If
  async queue splitting or overlapping submissions make two lifetimes ambiguous,
  the compiler/backend must keep them separate.
- Imported, exported, externally retained, cross-frame, selected latest-relic,
  or output-root artifacts are not alias candidates unless the framework can
  prove their physical storage is execution-local.
- Debug/validation modes may disable aliasing while preserving the same logical
  artifact behavior.

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

The gathered submission also owns the physical lifetime of execution-local GPU
resources. The first backend implementation may allocate one physical resource
per logical artifact, but the plan must expose enough internal lifetime and
descriptor information for an allocator to alias non-overlapping artifacts. A
later backend optimization can then bind multiple logical artifacts to one
physical image or buffer without changing quest declarations.

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

## Internal Resource Lifetime And Aliasing

Feature authors should not manage transient lifetimes. From their point of view,
all data is an artifact with intended usage: a quest declares the artifacts it
reads and writes, and execution provides usable instances when the quest runs.
The compiler and backend decide which artifacts are execution-local, how long
their physical resources must live, and whether memory can be reused.

This applies to pass-local depth buffers, temporary color targets, shadow-map
depth attachments, upload staging buffers, readback staging buffers, per-draw
scratch data, and other one-shot resources.

Two different logical needs should remain two different artifacts even when they
could reuse memory. For example:

```text
shadowDepthBuffer
  used only by the shadow pass

mainDepthBuffer
  used only by the main scene pass
```

These artifacts may have different descriptors. The shadow depth buffer might be
2048x2048 while the main depth buffer might match the backbuffer size. Because
their lifetimes do not overlap, the compiled plan may allow the backend to place
both artifacts in the same physical allocation if the allocation satisfies both
requirements.

The feature author does not request that aliasing. They simply declare writes to
`shadowDepthBuffer` and `mainDepthBuffer`. The planner sees that neither is an
output root, neither escapes the execution, and their physical lifetimes do not
overlap.

The same model applies to staging resources:

```text
meshUploadScratch
  host-visible buffer used by an upload quest

readbackScratch
  host-visible or readback buffer used by a later download quest
```

If their lifetimes and memory requirements are compatible, the internal
execution-local allocator may alias them. If not, it allocates separate
resources. Quest code must not observe or depend on the aliasing decision.

Aliasing is therefore a plan/backend optimization over logical artifacts:

```text
Logical artifact A  -> physical transient resource 7
Logical artifact B  -> physical transient resource 7
Logical artifact C  -> physical transient resource 8
```

The invariant is that artifacts describe meaning and dependencies, while gpu2
resources implement storage. Quests declare artifact requirements and use the
resources provided by execution. They do not manually pool, recycle, or share
temporary GPU resources.

Output roots are the user-facing preservation mechanism. If a quest produces an
artifact that must survive even when no retained quest reads it in the same
execution, the quest or caller marks that output as a root. Non-root writes can
be culled or have their physical storage recycled once the planner proves no
retained consumer needs them.

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

Concrete action/effect implementations may specialize per graphics API when that
is useful. Store those implementations in files prefixed with the API shorthand,
such as `vk-pbr-quest.cpp`, and name implementation classes with the full API
suffix, such as `PbrQuestVulkan`. Keep the public adapter interfaces abstract
and API neutral; the API-specific classes are private implementation choices.

## Resolved Decisions

- Build the generic RDG-to-gpu2 execution layer before moving SCC or PBR between
  modules.
- Keep public RDG2 APIs as pure virtual abstract interfaces where practical,
  with concrete implementations hidden behind factories.
- Keep the render graph execution implementation API agnostic; put
  Vulkan/D3D12/Metal-specific effect implementations in API-prefixed files and
  full-API-suffixed classes.
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
- Keep transient/aliasing decisions inside the compiler and planner; feature
  authors declare artifacts and root outputs, not physical resource lifetimes.
- Keep the core declaration API generic, while allowing engine/framework layers
  to expose strongly typed feature declaration structs that lower into generic
  artifact uses.
- Defer async queue splitting.
- Prefer a small, testable implementation that can later grow into a fuller
  mini-engine.

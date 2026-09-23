# Engine2 (`GN::e2`) Design

`engine2` is the experimental high-level engine layer above Garnet's lower-level
platform, GPU, and rendering systems. Its public API lives in
`src/inc/garnet/e2/` and is included through `garnet/GNengine2.h`.

The module now has a small concrete vertical slice under `src/core/e2/` while
the public headers continue to define the intended ownership model and major
engine roles.

## Goals

`engine2` models an application as a persistent universe containing one or more
worlds and functional subsystems called domains. `Universe` is the ultimate root
item of the module. A `World` represents a game world or level: it might be the
current interactive level, the next level being loaded by a background loader, or
another world kept alive for streaming, transition, preview, or editor workflows.
Worlds are logically independent from each other even when the lower engine
layers share physical resources such as textures, models, buffers, or compiled
GPU objects.

Simulation and rendering are intentionally independent. The world can evolve at
its own cadence, with or without a fixed timestep, and the visual layer observes
that evolution only through self-contained snapshots. At selected intervals, the
world generates a `VisualTableau` and forwards it to a `VisualDomain` for
rendering work.

The design aims to keep these concerns separate:

- `Universe`: ultimate root object, global lifetime context, multi-world owner,
  domain owner, and unique identifier source.
- `World`: game world or level that can be active, loading, staged, or otherwise
  managed independently of other worlds and of rendering.
- `Form`: structural simulation object for things that live in a world — the
  hierarchy, the spatial transform, and a flat list of facets.
- `Facet`: unit of state and behavior attached to a form; the extension point
  for domain-specific aspects such as visuals, audio, or physics.
- `VisualMoment`: one captured item that records its own rendering.
- `VisualTableau`: an opaque collection of moments for a scene snapshot.
- Domains: functional subsystems within the universe, such as rendering, audio,
  and operating-system integration.

## Public API Layout

Client code includes only:

```cpp
#include <garnet/GNengine2.h>
```

The subheaders under `src/inc/garnet/e2/` reject direct inclusion. This keeps the
module boundary explicit and lets the monolithic header control dependency order.
`GNengine2.h` currently includes `GNgpu2.h`, then the `e2` subheaders:

- `e2.h`: base types, references, units, and `OperatingDomain`.
- `spatial.h`: the coordinate system — absolute and local coordinate types, the
  physical scale, and the `spatial` rebasing/rotation utilities.
- `photometry.h`: photometric units used by lights.
- `visual.h`: camera, visual snapshot, and rendering domain interfaces.
- `world.h`: simulation forms, facets, and world interface.
- `universe.h`: global engine context and identifier generation.
- `simple.h`: the minimal `Simple` world used to exercise the workflow.

## Core Types

### `Universe`

`Universe` is the ultimate root object of the engine2 module. It is initialized
first and is expected to remain available for the lifetime of engine2 objects.
Conceptually, it can contain multiple worlds and multiple functional domains at
the same time. For example, the current playable level and the next level being
loaded in the background can both be represented as `World` objects within the
same universe. Those worlds run as separate logical simulations, but they may
refer to shared physical resources managed by lower layers. The current public
responsibility is to generate 64-bit identifier values that are unique within
the universe.

### `Being`

`Being` is the common base for named, reference-counted engine objects. It is
an e2-local alias of `GN::RCRT64` (defined in the base module and shared with
gpu2), kept as a separate name so e2 can later switch to a different id width
by changing only the alias. It combines:

- `RefCounter` ownership.
- `RuntimeType` metadata.
- immutable `id` and `name` fields.

e2 constructs beings with universe-generated ids via the `(type, id, name)`
constructor.

References use `GN::e2::Ref<T>`, an alias of `AutoRef<T>` constrained to
`Being`-derived targets when the target type is complete.

Derived public types use `GN_E2_DEFINE_A_BEING(baseType)` to inherit the base
constructor shape and register runtime type metadata.

### Coordinate System

Spatial state lives in `spatial.h` and splits by role:

- `WorldCoordinate`: an absolute coordinate measured in the smallest unit of a
  world, stored as a 128-bit signed integer (two 64-bit halves, exposed
  read-only through `hi()`/`lo()`). It supports full integer arithmetic but
  deliberately contains no floating-point code.
- `LocalCoordinate`: a coordinate in a local frame, stored as a signed 64-bit
  integer of world units. It represents extents, offsets, and camera-relative
  distances; rebasing into a local frame is what keeps physical-unit conversion
  precise.
- `PhysicalScale`: the physical size of one world unit, restricted to powers of
  ten — one unit = 10^exponent meters (`NANOMETER()` is exponent -9). The
  restriction keeps every conversion a pure multiply or divide by an
  exactly-representable constant. All physical-unit conversion lives here
  (`toMeters`/`fromMeters` and the centimeter variants): local quantities
  convert exactly, absolute coordinates convert approximately (double
  precision), and both directions saturate instead of overflowing — to
  +/-infinity toward physical units, to the int64 unit range toward units.
- `spatial`: stateless static utilities connecting the two coordinate types.
  `toLocal(from, to)` narrows a coordinate difference into a local frame
  (asserting it fits 64 bits), `toWorld(...)` widens or anchors local offsets
  back onto absolute coordinates, and `rotatedBy(...)` rotates world or local
  vectors in 32.32 fixed-point integer math.
- `WorldVector2/3/4` and `LocalVector2/3/4` are the vector aliases; `Rotation`
  is a `glm::quat`.

The design rule: absolute coordinates never pass through floating point, whose
53-bit mantissa would corrupt positions far from the origin. Quantities convert
to float meters only after rebasing into a local frame, so precision is
independent of where in the (128-bit) world the action happens.

### `World`

`World` is a continuously evolving game world or level. An active world is where
players, NPCs, scripts, and other forms interact, but a world can also be staged
or loaded in the background before it becomes the current gameplay world. Each
world is logically independent: its forms, scripts, update cadence, and visual
moments belong to that world. It owns the main game loop entry point through
`run()`, accepts new forms through `populate()`, and exposes
`snapshot()` for renderer-facing tableaux.

`populate()` and `snapshot()` are documented as callable from any
thread. Implementations therefore need a synchronization boundary between live
simulation mutation and snapshot capture.

### `Form` And `Facet`

`Form` represents an active presence in the world and owns the structural side
of the simulation: the parent/child hierarchy, the spatial transform (position
and rotation, parent-relative, with `worldPosition()`/`worldRotation()` composing
through the ancestor chain), and a flat list of facets. A form may be atomic or
composed from child forms. Its `live()` lets each attached facet live one
simulation moment, in attach order; the world's tree traversal covers child
forms. The public `Form`
is a sealed pure-virtual interface: `Form::create()` returns a new empty form
ready to receive facets, and the concrete implementation (hierarchy, transform,
and facet storage plus world-membership propagation) lives inside the engine.

`Facet` is the unit of state and behavior attached to a form, and the extension
point of the simulation: aspects a form has (visible, audible, physical, ...)
are expressed by attaching facets, not by subclassing `Form`. A facet belongs to
at most one form, never has children of its own, and receives world-lifecycle
notifications: `enterWorld()`/`leaveWorld()` fire when the owning form's world
membership changes, when the facet is added to a form already living in a world,
or when the owning form is destroyed while in one.

`VisualFacet` is the facet subtype that can contribute visual state for a
snapshot through `snapshot()`. Engine implementations discover visual
contributors with the internal `queryFacetsByType()` helper, using
`VisualFacet::TYPE_INFO()` as the requested runtime type. This keeps simulation
ownership in the form tree while allowing rendering to consume a snapshot built
from the visual facets exposed by that tree.

`Mold` is a reusable recipe for creating fresh form trees. The public `Mold`
interface is pure virtual; `Mold::create()` returns the engine-provided concrete
implementation.

### Domains And Visual Layer

Domains are functional subsystems owned by the universe. A domain provides a
specific engine service across worlds, such as rendering, sound and music, or
operating-system interaction.

Current and planned domain roles include:

- `VisualDomain`: rendering subsystem that consumes opaque `VisualTableau` snapshots.
- `PhysicalDomain`: physics and environmental simulation subsystem providing independent time evolution, queries, and lookahead predictions.
- Audio domain: sound and music subsystem. The public audio interfaces are not
  currently active in the headers.
- `OperatingDomain`: operating-system subsystem for windows, input, and related
  platform services.

The visual boundary separates a renderable item from its owning snapshot:

- `VisualMoment` is one item that knows how to record its own rendering.
- `VisualTableau` is the opaque collection of moments in a captured scene.
- `World::snapshot(parameters)` produces a `Ref<VisualTableau>`.
- `VisualDomain::renderFrame({.tableau = tableau})` consumes that opaque snapshot.

Each `renderFrame()` call clears the color and depth targets, renders the tableau,
and presents for a windowed domain. `RenderFrameParameters` supplies a linear RGBA
`clearColor` (default `(0.05, 0.06, 0.09, 1)`) and `clearDepth` (default 1) for that call;
omitted values do not inherit the previous frame's settings. Headless frames are
available through `readbackFrame()`.

The tableau's public interface exposes a factory and `add(moment)`, the operation
applications need to include environment or UI contributions. It exposes no child
lists, parent links, dependencies, graph edges, enumeration, or traversal callbacks.
Its organization and scheduling machinery stay inside E2. The current implementation
stores an insertion-ordered list and records serially: regular moments first in
insertion order, then environments, then overlays in descending logical Z order.
Relative order among environments, or among overlays with equal Z, is unspecified.
Future internal trees, graphs, and parallel execution do not require exposing
those structures to other modules.
A tableau is separate from `VisualMoment` and is not itself a renderable item.

`VisualMoment::record(RenderContext&)` is pure virtual. The context is borrowed
for that call and derives from `RuntimeType`, with no reference counting or object
identity. Its public methods expose the active gpu2 raster, prerequisite upload
scheduling, and `ssc()`: a read-only `fx2::SharedShaderConstants::Snapshot` with
prepared shader bindings. The domain already schedules that snapshot's uploads;
moments must not resubmit them. Target dimensions, viewport, and scissor are
available through `raster()` without duplicating those queries on the context.

Each moment provides its own draw logic; the domain's recording loop simply
invokes that operation. Built-in scene moments receive their own constants;
extension moments receive the first scene's constants, or a default camera/light
snapshot if the tableau has no scene moment. The selected environment supplies
lighting in either case. Environment moments receive their own prepared skybox
bindings and use the public context directly. The built-in scene renderer alone
accesses an internal context extension for reusable resource caches; those caches
and RDG2 scheduling remain private to E2.

`VisualMomentImpl` is the built-in scene moment containing captured cameras,
renderables, and lights. Simple-world snapshotting merges compatible scene
contributions and adds other facet-provided moments to the tableau independently.
`VisualFacet::snapshot()` returns a single self-contained visual moment; the
world's `snapshot()` assembles the collection. Later changes to the facet or its
owning form must not affect that moment. Any shared resources must remain
immutable for the moment's lifetime.

Complete composition before rendering, and keep the tableau and all referenced
moments unchanged until `renderFrame()` returns. Captured data must not refer back to
mutable world/form state. `VisualTableau::SnapshotParameters` carries the target
domain, observing cameras, and expected render-time shift for capture.

`VisualDomain::CreateParameters` supplies a universe and an optional OS domain;
no OS domain selects headless rendering. The domain interface contains no concrete
content types or content setters. `VisualMoment`, `VisualTableau`, `VisualOverlay`,
and `VisualEnvironment` are defined after `VisualDomain` in the public header.

### Physical Domain And Physics Integration

`PhysicalDomain` models the physical laws, dynamics, and continuous environmental
fields that govern living forms across a universe.

#### Architectural Principles: Singleton Engine & Data Ownership

- **Singleton Operating Engine**: Like `VisualDomain`, `PhysicalDomain` is a
  universe-level singleton service. It owns only the computational machinery:
  thread pools, Jolt job dispatchers, and compiled `gpu2` compute shader pipelines.
- **Zero Simulation Data Ownership**: `PhysicalDomain` holds **no** world simulation
  data (no bodies, colliders, islands, or velocities).
- **Data Owned by `World`, `Form`, and `Facet`**:
  - `Form` and `PhysicalFacet` own local physical attributes (mass, collision hulls, material tempers).
  - `World` owns the connected physical system representing the world's physical graph
    (broadphase BVH, island graphs, constraints, GPU particle/grid buffers), organized
    into a **`PhysicalTableau`** (analogous to `VisualTableau`).
  - A single `PhysicalDomain` seamlessly services multiple independent worlds (e.g., active level,
    background-loading level, editor preview) without cross-contamination.

#### Dual-Snapshot Symmetry & World Serialization

The engine establishes a profound reader-writer symmetry between rendering and physics:

1. **Physical Snapshot (Ingress Write: `PhysicalDomain -> World`)**:
   - The physical solver computes at its own high cadence (e.g. 200 Hz).
   - At the main world's logic cadence (e.g. 30 Hz / 60 Hz), the world requests a
     **`PhysicalSnapshot`** sampled/interpolated precisely at the world's simulation timestamp.
   - The world applies this snapshot to batch-update the positions and rotations of its `Form`s.
   - This is a **WRITE** operation to the world's state.
2. **Visual Snapshot (Egress Read: `World -> VisualDomain`)**:
   - `World::snapshot()` captures an immutable `VisualTableau` for the renderer.
   - This is a **READ** operation on the world's state.
3. **Serialization Invariant**:
   - Because applying a `PhysicalSnapshot` writes to `Form` transforms while capturing a
     `VisualTableau` reads from them, these phases **must be serialized** within each world step:
     `[Ingress Physical Write] -> [World/Gameplay live()] -> [Egress Visual Read]`.
   - This guarantees the renderer never observes half-updated, torn, or non-deterministic world state.

#### 128-bit Coordinate Rebasing to 32-bit Float

Garnet's 128-bit integer coordinates (`WorldCoordinate`) must be translated to the 32-bit
float coordinates (`glm::vec3` / `fiz::Vector3`) that physics solvers and GPUs require,
mirroring the camera-relative rebasing used in rendering. Crucially, `fiz` is **unit-agnostic**:
`e2` uses `PhysicalScale` to convert 128-bit integer world units into the target floating-point scalar
units (meters for terrestrial worlds, astronomical units for planetary orbits, or micrometers for micro-scale physics):

- **Ingress (`World -> fiz`)**: Each simulation zone establishes an integer 128-bit anchor
  $O_{sim}$. Coordinates are rebased in exact integer arithmetic and scaled into float units:
  $$\mathbf{P}_{fiz} = \text{scale.toPhysical}(\text{spatial::toLocal}(\mathbf{P}_{world}, \mathbf{O}_{sim}))$$
- **Egress (`fiz -> World Snapshot`)**: Simulated float coordinates convert back to local integer counts
  and re-anchor into 128-bit world space:
  $$\mathbf{P}'_{world} = \text{spatial::toWorld}(\text{scale.fromPhysical}(\mathbf{P}'_{fiz}), \mathbf{O}_{sim})$$
- **Floating Origin**: Anchoring locally to $O_{sim}$ guarantees maximum floating-point precision anywhere
  in a planetary or cosmic-scale world, completely eliminating float32 jitter at scale.

#### Integer Time, 100% Forward Determinism & $T$-Symmetry

- **Canonical Temporal Unit (`UnitOfTime`)**: `e2` inherits `UnitOfTime` directly from
  `GN::fiz::UnitOfTime` (`using UnitOfTime = GN::fiz::UnitOfTime;`, typed as `std::chrono::nanoseconds`).
  Simulations advance by discrete 64-bit integer nanosecond steps, eliminating floating-point
  time accumulation drift.
- **100% Strict Forward Determinism**: Given an initial physical state $S_0$ and a sequence of discrete
  integer timestamps, the simulation is guaranteed to reach the exact same, bit-identical state
  every single run. Force summation, contact sorting, and constraint resolution order are strictly stable
  across worker threads.
- **$T$-Symmetry (Time-Reversibility Where Physically Permissible)**: Physical laws in `fiz`
  strive for time-direction neutrality. Conservative systems (gravity, orbital mechanics, elastic
  springs, conservative XPBD constraints) support negative integer timesteps ($-\Delta t$) via
  symplectic integrators (e.g., Velocity Verlet), returning to identical initial states without
  history caches. Where physical laws themselves introduce thermodynamic dissipation (e.g. inelastic
  energy loss or plastic deformation), physical entropy is respected.
- **Time Scaling & Decoupled Cadence**:
  - `setTimeScale(scale)`: controls simulation pace (`1.0` normal, `0.1` slow-motion, `0.0` paused, negative for backward simulation).
  - `stepOnce()`: frame-by-frame debug step.
  - $\alpha$-state interpolation between consecutive physics steps ensures high-refresh displays (144+ Hz) render buttery smooth without jitter even when world logic ticks at 30 Hz.

#### Generic `PhysicalFacet` & Minimal Subclassing Rule

Rather than creating a separate `Facet` subclass for every individual physical effect
or solver provided by `fiz` (which leads to redundant class proliferation), `e2` favors
a **generic, unified `PhysicalFacet`**:

- **Subclassing Rule**: A `Facet` class is subclassed **if and only if** external
  application/gameplay code requires unique public methods or variables on that class
  (e.g., a `CharacterPhysicalFacet` exposing `jump()`, `walk()`, and `isGrounded()`).
- **Generic Physical Representation**: For standard physical presences (rigid solids,
  bouncy gels, cloth, hair, or fluid emitters), forms attach the generic `PhysicalFacet`,
  configured with appropriate descriptors and effect types from `fiz`.
- **Core Responsibilities**:
  - Exposes common application-facing controls: `applyForce()`, `applyImpulse()`,
    `velocity()`, `setVelocity()`, `setTemper()`.
  - Automatically handles world lifecycle (`enterWorld()` and `leaveWorld()`) by registering
    with the world's `PhysicalTableau`.
  - Coordinates bidirectional transform synchronization (`syncToForm()` / `syncFromForm()`)
    between `Form`'s spatial hierarchy and physical simulation states.

#### Query and Prediction Interfaces

- **Immediate Spatial Queries**: `raycast(...)`, `sweep(...)` (capsule, sphere, box), and `overlap(...)` tests.
- **Environmental Field Queries**: sampling water surface height and currents (`sampleWaterHeight`),
  buoyancy forces (`calculateBuoyancy`), and wind vectors.
- **Lookahead & Speculative Prediction**:
  - `predictTrajectory(...)`: analytical/kinematic projection of ballistic arcs and impact points.
  - `predictCollision(...)`: continuous collision detection (CCD) lookahead testing whether
    a body will collide within a future time window $\Delta t$.
  - `createSpeculativeSandbox()`: forks an isolated lightweight "ghost" simulation sandbox
    to step candidate actions ahead in time without mutating the live world state.

#### Architectural Separation with `GNfiz`

`PhysicalDomain`, `PhysicalTableau`, and `PhysicalFacet` in E2 define engine semantics,
world lifecycle, and spatial synchronization. Low-level, atomic, platform-specific algorithms
and solvers live entirely inside the standalone `GNfiz` module (`GN::fiz`). E2 consumes `fiz`,
but `fiz` remains completely independent of E2.

## Expected Data Flow

The intended frame-level flow across simulation, physics, and rendering is:

```text
Universe
  is the root item for engine2
  owns global lifetime context
  may contain multiple worlds and functional domains (VisualDomain, PhysicalDomain, OperatingDomain)

PhysicalDomain (Operating Engine)
  advances each World's PhysicalTableau at high frequency (e.g. 200 Hz) via Jolt / GPU compute
  operates on 32-bit float coordinates rebased from 128-bit simulation anchors
  publishes sampled PhysicalSnapshot instances at discrete integer timestamps

World::run()
  advances one logical world at its own cadence (e.g. 30 Hz or 60 Hz)

  1. Ingress Physical Write:
     queries PhysicalDomain for a PhysicalSnapshot at the world's current timestamp
     re-anchors float coordinates back to 128-bit world coordinates
     batch-updates Form transforms and PhysicalFacets (WRITE operation)

  2. World Gameplay Evolution:
     Form::live() advances attached facets in attach order
     gameplay logic, AI, and scripts react to updated physical positions
     gameplay can apply new impulses, forces, or velocities to PhysicalFacets

  3. Egress Visual Read:
     World::snapshot(parameters) captures an immutable VisualTableau (READ operation)
     briefly observes live state and queries each Form tree for VisualFacet objects
     re-bases positions against the observing camera in exact integer space
     forwards the sealed VisualTableau to VisualDomain for rendering work

VisualDomain::renderFrame({.tableau = tableau})
  consumes the immutable VisualTableau without blocking world evolution
  seals the tableau and moment references as an imported RDG2 relic
  builds the concrete closed frame plan (acquire -> render -> present)
  prepares shared resources and adapts each concrete scene task into FX2 effects
  invokes each moment's virtual record() in the tableau's internal order
  lets RDG2 gather payloads, submit, and present
```

The key rule is strict serialization on the World: `[Ingress Physical Write] -> [World live()] -> [Egress Visual Read]`.
This guarantees that rendering consumes captured tableaux and moments without observing partially updated or torn world
states, while allowing physical simulation, world logic, and rendering to execute at their own independent cadences.

## Scene, environment, and overlay moments

`createModelForm()` attaches a `ModelVisualFacet` containing an immutable FX2
`ModelScene`. Capture copies the model reference and the form's composed world
transform into the `VisualMoment`; rendering never reaches back into the live
form. A `VisualDomain` caches FX2 `ModelAsset` instances by scene identity, so
multiple moments reuse uploaded geometry and textures. Visibility is an atomic
facet property and hidden facets contribute no renderable to a new snapshot.

Each scene moment translates captured models into FX2 draws. The visual domain
invokes moments in the order maintained by the tableau implementation. RDG2 orders the outer frame
quests and handles payload submission and presentation. FX2 remains unaware of
worlds, visual moments, and render graphs.

`VisualOverlay` derives from `VisualMoment`. UI2's backend is one such task;
add it to the tableau after the world snapshot is captured. All overlays render
after regular moments and environments, regardless of insertion position.
`zOrder()` supplies an integer logical Z, independent of GPU depth: larger values
(farther away) render before smaller values (nearer). `setZOrder()` changes it
between frames; UI2 starts at zero. Equal-Z overlays have unspecified relative
order. An overlay records into the
shared raster target and returns any prerequisite upload payload. It owns its
draw resources, while the domain owns the GPU, targets, and frame plan. The domain
does not store an active overlay between frames. UI2 currently exposes finalized
ImGui draw data, so do not call `newFrame()` again until rendering that tableau returns.

`VisualEnvironment` also derives from `VisualMoment` and has two responsibilities:
provide reusable graphics resources and image-based lighting data to scene tasks,
and render the environment background. Create it with the universe, domain GPU,
and an immutable description containing skybox/IBL paths and exposure. Reuse the
moment across frames to retain its FX2 texture/resource cache; create a replacement
to change its description. Resource preparation occurs before drawing any tasks,
so an environment can light scene geometry regardless of insertion position.
Its skybox draws after all regular moments and before any overlays, with a depth
test that preserves the scene.

Multiple environments are allowed, with unspecified relative rendering order.
Each environment prepares its own resources and uses the first scene's first camera
for its background. Scene lighting uses one environment's resources; selection is
unspecified when several are present. Each scene task keeps its own camera/light
buffers and snapshot, sharing only the selected environment texture views. This
prevents later scene uploads from overwriting earlier cameras before the combined
raster executes. An environment created for a different GPU rejects the frame.
Without an environment moment, no skybox draw is recorded and indirect environment
radiance is zero.

Typical composition (after UI construction has been finalized):

```cpp
auto tableau = world->snapshot(snapshotParameters);
tableau->add(environmentMoment);
tableau->add(uiBackend);
visual->renderFrame({.tableau = tableau});
```

Use `VisualTableau::create(universe)` for a snapshot independent of world capture,
then add concrete moments. A new moment subclass implements `record()`; it does
not require adding a type-dispatch branch to the domain's recording loop. One-time
FX2/model uploads are retained if recording fails and replayed before the next
frame, then released after successful submission.

## Threading Model

The API already marks two `World` operations as thread-safe entry points:

- `populate(ArrayView<Ref<Form>>)`
- `snapshot(const VisualTableau::SnapshotParameters &)`.

The concrete implementation should treat `World` as the synchronization owner for
root form collection changes, recursive form-tree stepping, and visual snapshot
capture. `Form::live()` — which lets the form's facets live one simulation
moment — is driven by the world using whatever update cadence that world
chooses, while visual capture may
be requested from another thread by the rendering path.

## Visual Backend Composition

The rendering backend deliberately lives at the intersection of two lower-level
modules with narrower responsibilities:

```text
World / VisualFacet tree
  -> opaque VisualTableau (captured moments; private organization)
  -> VisualDomain implementation (prepare resources, invoke moment record methods)
       -> FX2 atomic effects (render-space constants and draw work)
       -> RDG2 closed plan (declared ordering, submit, and present)
  -> gpu2
```

FX2 is graph-agnostic. It accepts camera-relative float-meter positions and
produces resource sets, draw parameters, and sealed payloads; it never observes
a world, visual moment, artifact, or quest. RDG2 is domain-agnostic. It sees
stable tableau, SSC, and backbuffer artifacts plus four generic quests;
it never interprets cameras, lights, forms, or materials. E2 is
the adapter: the private tableau supplies the workload and order; scene moments perform exact
integer rebasing and translate captured data into FX2 operations.

The visual domain creates these artifacts and quests once during initialization.
Each frame publishes its tableau relic and compiles a new plan from this outer
skeleton, allowing frame-specific workload construction without recreating
long-lived artifact identities:

```text
frame-begin   DISCARD_WRITE backbuffer (acquire and await ready payload)
prepare-ssc   READ tableau, DISCARD_WRITE SSC (publish task snapshots + uploads)
render        READ tableau and SSC, READ_WRITE backbuffer (emit raster work)
frame-end     READ root backbuffer (request present)
```

Each call to `renderFrame()` publishes only new relics to those persistent artifacts;
it does not recreate graph identities. The tableau is published before
the first compilation, so execution reads one sealed relic and never follows
identities back into mutable simulation state. SSC preparation publishes immutable
FX2 resource snapshots for the scene and environment tasks, making their uploads
and consumer dependency explicit. An environment moment owns its FX2 constants
and lighting resources; the domain retains a zero-radiance fallback for tableaux that
omit an environment. The render quest republishes the same
physical swapchain-frame entity as a new relic: the new version represents the
semantic transition from acquired to rendered and provides the dependency edge
to presentation.

This is the first executable slice, not the final workload model. The current
moment implementation flattens renderables and lights captured from the form
tree. Future versions can give tableaux a richer private organization or sealed
asset/material manifests, perform discovery/culling before compilation, and generate a concrete
set of shadow, depth, and post-processing quests. Those changes belong in E2;
they do not require graph knowledge in FX2 or engine knowledge in RDG2.

## Relationship To Lower Layers

`engine2` is intentionally above GPU details. It includes `GNgpu2.h` through the
public module header, but the e2 interfaces do not expose concrete GPU resources.
The expected layering is:

```text
Application / sample
  -> GN::e2 world, forms, facets, cameras
  -> engine2 domains, such as VisualDomain or OperatingDomain
  -> gpu2 / renderer implementation
  -> platform graphics API
```

This keeps gameplay and world state independent from the active rendering backend.
Multiple worlds can still share physical resources through these lower layers.
For example, two worlds may reference the same texture or model asset without
sharing simulation state or visual moments.

## Current Implementation Status

`src/core/e2/` now contains a first concrete vertical slice that exercises the
whole world → visual-moment → render path:

- `simple-world.cpp`: the `Simple` namespace world. A `World` that advances its
  form trees at a fixed timestep on the caller's `run()` thread, plus two
  trivial visible objects built from facets on plain structural forms — a
  spinning box (a spin-behavior facet plus a box-mesh visual facet) and a point
  light (a single visual facet). The box is cast from a `Mold` recipe to
  exercise that workflow; the light is assembled directly. `populate()` and
  `snapshot()` are guarded by a single world mutex, which is the
  synchronization boundary between live simulation and snapshot capture. The
  sample runs `run()` on its own thread; the world itself does not own a
  thread.
- `form.cpp`: the engine's concrete `Form` implementation behind the sealed
  public interface — hierarchy, transform, and facet storage with their
  invariants — plus the `Form::create()` factory and the world-space transform
  composition helpers.
- `mold.cpp`: the official `Mold` recipe implementation. It stores a root form
  factory plus child molds and casts fresh form trees while rejecting recipe
  cycles.
- `os.cpp`: the official `OperatingDomain`, wrapping `GN::win` for the window,
  render surface, and event pump.
- `visual.cpp`: the official `Camera` and `VisualDomain`. The visual domain owns
  the gpu2 swapchain, depth buffer, box shaders, an FX2 shared-constants effect,
  a geometry cache, and persistent RDG2 artifacts/quests. `renderFrame()` publishes
  the tableau and its moment references as a new relic, compiles a frame-local
  plan, and executes it. A dedicated SSC quest prepares the environment resources
  and rebases each scene's positions against its first camera in exact integer
  space, then publishes FX2 camera/light snapshots and emits their upload payloads.
  The render quest consumes that SSC relic and invokes each moment's record method;
  RDG2 owns their gathered submission and presentation.
- `e2-internal.h`: private types shared across the implementation, most notably
  `VisualMomentImpl`, the concrete scene task (cameras + renderables + lights).
  `VisualTableauImpl` holds the snapshot organization separately from its moments.
- `vk-shaders/box.{vert,frag}`: a minimal lit shader that directly includes
  FX2's canonical `camera-ubo.h` and `scene-ubo.h` set-0 definitions, plus E2
  per-draw model/color push constants. The E2 build declares those FX2 headers
  as shader dependencies, so changing the SSC layout recompiles these shaders.

The factory functions `Form::create`,
`Simple::createWorld/createBox/createPointLight`, `OperatingDomain::create`,
`Camera::create`, `VisualTableau::create`, and `VisualDomain::create` are all implemented here. The
matching sample lives in `src/sample/e2/simple-world.cpp`, and
`test/simple-world-test.cpp` covers the CPU-side workflow headlessly: population,
capture contents in world space, independent-cadence advancement, facet
attach/ownership rules, facet live() dispatch, `enterWorld()`/`leaveWorld()`
notifications on every path, and transform composition through the parent chain.

`test/spatial-test.cpp` covers the coordinate system headlessly: 128-bit
`WorldCoordinate` arithmetic across the 64-bit boundary, `toLocal`/`toWorld`
rebasing, integer-space rotation, and physical-unit conversion including
rounding and saturation behavior. `test/visual-teardown-test.cpp` is a
GPU-required regression test that destroys the visual domain before the OS
domain and asserts the Vulkan validation layer reports no leaked objects.
`test/visual-graph-test.cpp` covers the E2-owned fixed frame-plan order and
verifies RDG2 rejects a render stage whose backbuffer has no acquire producer.
`test/visual-moment-test.cpp` covers opaque tableau composition, retained custom
facet contributions, polymorphic recording, independent scene cameras, failed
recording recovery, and environment removal.

The original smoke test, `test/e2-mock.cpp`, remains: it creates a `Universe`, a
mock `World`, a factory-created `Form`, and a mock `Facet` to verify the
runtime-type and reference patterns compile and that `populate()`/`run()` are
callable through the public interfaces.

## Development Notes

- Keep client code on the monolithic include: `#include <garnet/GNengine2.h>`.
- Follow `agent/skills/garnet-public-interface/SKILL.md`: public headers contain
  only cross-module contracts, with pure virtual behavioral interfaces preferred.
  Internal storage, caches, scheduling, and concrete types stay in module files.
- Use `GN::e2::Ref<T>` / `AutoRef<T>` for ownership.
- Derive public engine objects from `Being` or another direct e2 base type and
  register the direct parent with `GN_REGISTER_RUNTIME_TYPE(...)`.
- Put behavior and domain-specific state in `Facet` subtypes. `Form` is a
  sealed interface: create forms with `Form::create()` (or cast them from
  molds) and compose capabilities by attaching facets.
- Preserve the simulation/rendering split: live world state belongs to `World`
  and `Form`; renderer-facing state is captured from `VisualFacet` into a
  self-contained moments owned by an opaque `VisualTableau`.
- When implementing thread-safe world operations, document the synchronization
  invariant at the point where the lock, queue, or snapshot boundary is enforced.

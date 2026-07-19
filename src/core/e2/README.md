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
world generates a `VisualMoment` and forwards it to a `VisualDomain` for
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
- `VisualMoment`: self-contained world snapshot for rendering.
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

- `thing.h`: base types, references, units, identifiers, and `OperatingDomain`.
- `visual.h`: camera, visual snapshot, and rendering domain interfaces.
- `world.h`: simulation forms, facets, and world interface.
- `universe.h`: global engine context and identifier generation.

## Core Types

### `Universe`

`Universe` is the ultimate root object of the engine2 module. It is initialized
first and is expected to remain available for the lifetime of engine2 objects.
Conceptually, it can contain multiple worlds and multiple functional domains at
the same time. For example, the current playable level and the next level being
loaded in the background can both be represented as `World` objects within the
same universe. Those worlds run as separate logical simulations, but they may
refer to shared physical resources managed by lower layers. The current public
responsibility is to generate `UniqueIdentifier` values that are unique within
the universe.

### `Thing`

`Thing` is the common base for named, reference-counted engine objects. It
combines:

- `RefCounter` ownership.
- `RuntimeType` metadata.
- immutable `id` and `name` fields.

References use `GN::e2::Ref<T>`, an alias of `AutoRef<T>` constrained to
`Thing`-derived targets when the target type is complete.

Derived public types use `GN_E2_DEFINE_A_THING(baseType)` to inherit the base
constructor shape and register runtime type metadata.

### `World`

`World` is a continuously evolving game world or level. An active world is where
players, NPCs, scripts, and other forms interact, but a world can also be staged
or loaded in the background before it becomes the current gameplay world. Each
world is logically independent: its forms, scripts, update cadence, and visual
moments belong to that world. It owns the main game loop entry point through
`run()`, accepts new forms through `populate()`, and exposes
`captureVisualMoment()` for renderer-facing snapshots.

`populate()` and `captureVisualMoment()` are documented as callable from any
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
snapshot through `captureVisualMoment()`. Engine implementations discover visual
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

- `VisualDomain`: rendering subsystem that consumes `VisualMoment` snapshots.
- Audio domain: sound and music subsystem. The public audio interfaces are not
  currently active in the headers.
- `OperatingDomain`: operating-system subsystem for windows, input, and related
  platform services.

The visual domain is split into capture and consumption:

- `Camera` describes how the world is observed.
- `VisualMoment` represents captured visual state for one or more cameras.
- `VisualDomain` renders a `VisualMoment`.

`VisualMoment` is intended to be a self-contained snapshot of the world at a
certain point in time. `VisualMoment::CaptureParameters` carries the target
`VisualDomain`, camera list, and an expected render-time shift. That time shift
lets capture code account for the fact that rendering may occur after the
simulation snapshot is requested.

`VisualDomain::CreateParameters` depends on both `Universe` and
`OperatingDomain`, reflecting that rendering needs global engine context plus OS
resources such as windows, surfaces, keyboard, or mouse integration.

## Expected Data Flow

The intended frame-level flow is:

```text
Universe
  is the root item for engine2
  owns global lifetime context
  may contain multiple worlds and functional domains

World::run()
  advances one logical world at its own cadence
  may use a fixed timestep, variable timestep, script-driven updates, or another policy

World::captureVisualMoment(parameters)
  briefly freezes, synchronizes, or otherwise observes live state
  queries each root Form tree for VisualFacet objects by runtime type
  asks those VisualFacet objects to capture visual data
  returns a self-contained VisualMoment for a specific point in time

VisualDomain::render(moment)
  consumes the snapshot and talks to lower-level GPU/rendering systems
```

The key rule is that world evolution is independent of rendering. Rendering
should consume `VisualMoment`, not mutable simulation objects. This gives
implementations room to run simulation and rendering at different rates, and on
different threads, without letting the renderer observe partially updated world
state.

## Threading Model

The API already marks two `World` operations as thread-safe entry points:

- `populate(ArrayView<Ref<Form>>)`
- `captureVisualMoment(const VisualMoment::CaptureParameters &)`.

The concrete implementation should treat `World` as the synchronization owner for
root form collection changes, recursive form-tree stepping, and visual snapshot
capture. `Form::live()` — which lets the form's facets live one simulation
moment — is driven by the world using whatever update cadence that world
chooses, while visual capture may
be requested from another thread by the rendering path.

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
  `captureVisualMoment()` are guarded by a single world mutex, which is the
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
  the gpu2 swapchain, depth buffer, box shaders, a frame constants buffer, and a
  geometry cache; `render()` consumes a self-contained snapshot, emits a
  per-frame constants upload payload followed by the raster payload, and lets
  gpu2 queue ordering keep CPU frame preparation and GPU execution overlapped.
- `e2-internal.h`: private types shared across the implementation, most notably
  `VisualMomentImpl`, the concrete self-contained snapshot (cameras +
  renderables + lights) that worlds produce and the visual domain consumes.
- `vk-shaders/box.{vert,frag}`: a minimal lit shader (per-frame camera/light UBO,
  per-draw model/color push constants).

The factory functions `Form::create`,
`Simple::createWorld/createBox/createPointLight`, `OperatingDomain::create`,
`Camera::create`, and `VisualDomain::create` are all implemented here. The
matching sample lives in `src/sample/e2/simple-world.cpp`, and
`test/simple-world-test.cpp` covers the CPU-side workflow headlessly: population,
capture contents in world space, independent-cadence advancement, facet
attach/ownership rules, facet live() dispatch, `enterWorld()`/`leaveWorld()`
notifications on every path, and transform composition through the parent chain.

The original smoke test, `test/e2-mock.cpp`, remains: it creates a `Universe`, a
mock `World`, a factory-created `Form`, and a mock `Facet` to verify the
runtime-type and reference patterns compile and that `populate()`/`run()` are
callable through the public interfaces.

## Development Notes

- Keep client code on the monolithic include: `#include <garnet/GNengine2.h>`.
- Use `GN::e2::Ref<T>` / `AutoRef<T>` for ownership.
- Derive public engine objects from `Thing` or another direct e2 base type and
  register the direct parent with `GN_REGISTER_RUNTIME_TYPE(...)`.
- Put behavior and domain-specific state in `Facet` subtypes. `Form` is a
  sealed interface: create forms with `Form::create()` (or cast them from
  molds) and compose capabilities by attaching facets.
- Preserve the simulation/rendering split: live world state belongs to `World`
  and `Form`; renderer-facing state is captured from `VisualFacet` into a
  self-contained `VisualMoment`.
- When implementing thread-safe world operations, document the synchronization
  invariant at the point where the lock, queue, or snapshot boundary is enforced.

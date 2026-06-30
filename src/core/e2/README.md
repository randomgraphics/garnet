# Engine2 (`GN::e2`) Design

`engine2` is the experimental high-level engine layer above Garnet's lower-level
platform, GPU, and rendering systems. Its public API lives in
`src/inc/garnet/e2/` and is included through `garnet/GNengine2.h`.

The module is currently interface-first: the core implementation directory only
contains tests, while the public headers define the intended ownership model and
major engine roles.

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
- `Form`: simulation object interface for things that live in a world.
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
- `world.h`: simulation forms and world interface.
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

The base constructor and destructor are protected, so real use goes through a
derived universe type supplied by the implementation or tests.

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

### `Form`

`Form` represents an active presence in the world. It has two current
responsibilities:

- `update()`: advance internal simulation state.
- `captureVisualMoment()`: contribute visual state for a snapshot.

This keeps simulation ownership in the form while allowing rendering to consume a
snapshot built from one or more forms.

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
  asks relevant Form objects to capture visual data
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
form collection changes and visual snapshot capture. `Form::update()` is driven
by the world using whatever update cadence that world chooses, while visual
capture may be requested from another thread by the rendering path.

## Relationship To Lower Layers

`engine2` is intentionally above GPU details. It includes `GNgpu2.h` through the
public module header, but the e2 interfaces do not expose concrete GPU resources.
The expected layering is:

```text
Application / sample
  -> GN::e2 world, forms, cameras
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
  forms on a dedicated background simulation thread at a fixed timestep, plus two
  trivial forms — a slowly spinning box mesh and a point light. `populate()` and
  `captureVisualMoment()` are guarded by a single world mutex, which is the
  synchronization boundary between live simulation and snapshot capture.
- `os.cpp`: the official `OperatingDomain`, wrapping `GN::win` for the window,
  render surface, and event pump.
- `visual.cpp`: the official `Camera` and `VisualDomain`. The visual domain owns
  the gpu2 swapchain, depth buffer, box shaders, a per-frame uniform buffer, and a
  geometry cache; `render()` consumes a self-contained snapshot and draws it.
- `e2-internal.h`: private types shared across the implementation, most notably
  `VisualMomentImpl`, the concrete self-contained snapshot (cameras + renderables
  + lights) that worlds produce and the visual domain consumes.
- `vk-shaders/box.{vert,frag}`: a minimal lit shader (per-frame camera/light UBO,
  per-draw model/color push constants).

The factory functions `Simple::createWorld/createBox/createPointLight`,
`OperatingDomain::create`, `Camera::create`, and `VisualDomain::create` are all
implemented here. The matching sample lives in `src/sample/e2/simple-world.cpp`,
and `test/simple-world-test.cpp` covers the CPU-side workflow (population, capture
contents, and independent-cadence advancement) headlessly.

The original smoke test, `test/e2-mock.cpp`, remains: it creates mock `Universe`,
`World`, and `Form` types to verify the runtime-type and reference patterns
compile and that `populate()`/`run()` are callable through the public interfaces.

## Development Notes

- Keep client code on the monolithic include: `#include <garnet/GNengine2.h>`.
- Use `GN::e2::Ref<T>` / `AutoRef<T>` for ownership.
- Derive public engine objects from `Thing` or another direct e2 base type and
  register the direct parent with `GN_REGISTER_RUNTIME_TYPE(...)`.
- Preserve the simulation/rendering split: live world state belongs to `World`
  and `Form`; renderer-facing state is a self-contained `VisualMoment`.
- When implementing thread-safe world operations, document the synchronization
  invariant at the point where the lock, queue, or snapshot boundary is enforced.

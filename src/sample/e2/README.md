# E2 World Runtime sample

`GNsample-e2-world-runtime` renders immutable Prime snapshots while a separate
simulation thread runs DynamicsLaw followed by LifetimeLaw. Presentation uses
`basis::Platform` for window/events and `basis::Visual` for
rendering. It resolves the built-in box through `visual->assets()`, extracts a
pure-data `Visual::Tableau` from each PrimeView, and passes that data to Visual.
The sample records no GPU work and uses no RDG API. Visual internally draws
opaque white models with fixed Lambert lighting through gpu2/FX2.

The old `simple-world` sample remains suspended with SimpleWorld; this is the
active E2 sample.

```bash
source env/garnet.rc
build.py d --target GNsample-e2-world-runtime GNtest-internal
build/linux.gcc.d/bin/GNsample-e2-world-runtime
build/linux.gcc.d/bin/GNsample-e2-world-runtime --headless  # 600 ticks; t is an alias
build/linux.gcc.d/bin/GNsample-e2-world-runtime --headless --snapshot /tmp/e2-world.png
build/linux.gcc.d/bin/GNsample-e2-world-runtime --window-test  # windowed, exits after 600 ticks
```

The simulation uses a 10 ms timestep, up to 100 spawns/second and 1000 generated
bodies. Boxes fall onto a finite ground, collide with one another, and are removed
below -15 m. Random initial horizontal speed is 0..0.5 m/s, vertical speed zero.
The generator and its random state are committed World state. Headless mode advances
without wall-clock pacing; the renderer samples independently and checks retained
initial-state immutability, population bounds, and event output. The final output
reports ticks, rendered frames and observed events.

The local solver supports axis-aligned boxes without spin, uses substeps and
iterative contact resolution, and is not production physics or CCD. Final-tick
contact sets drive contact-begin counts, so within-tick transient impacts are not
part of that statistic. Presentation owns the camera and GPU resources. No renderer
code can mutate a Prime snapshot.

See [the canonical design](../../core/e2/README.md) and
[the active assignment](../../../agent/E2_WORLD_RUNTIME.txt) for runtime contracts,
implementation status and verification limitations.

Capabilities derive from `Facet` with nested `FacetValue` subclasses. Both use
`RuntimeType` and `RefCounter` without per-instance IDs or names; Forms and Laws
remain extensible `Being` subclasses.
`MotionFacet` requires `TransformFacet`; the runtime checks that requirement
before publishing a composition. TransformFacet parent links and local poses are versioned in Prime. Rendering
resolves world poses through the spatial ancestor chain; Form has no hierarchy.

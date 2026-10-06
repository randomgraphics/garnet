# Garnet Fiz (`GN::fiz`)

`fiz` provides standalone CPU rigid-body and soft-body simulation backed by Jolt
Physics. It has no E2, gpu2, FX2 or RDG2 dependency in its solver implementation.
Rendering and conversion from E2 world coordinates belong to callers.

This document describes the current code. The broader continuous-media roadmap
in `agent/FIZ_PHYSICS_MODULE.txt` is not an implemented API contract.

## Implemented API

Include the monolithic header; the subheaders reject direct inclusion:

```cpp
#include <garnet/GNfiz.h>
```

| Public subheader | Current functionality |
| --- | --- |
| `common.h` | `UnitOfTime`, `Scalar`, math types, `Transform`, motion/collision enums and `SimulationMode`. |
| `temper.h` | Density, restitution, friction, damping and compliance data, plus material presets. |
| `hull.h` | `Hull::createBox`, `createSphere`, `createCapsule`, `createCylinder`, `createConvex` and `createMesh`; typed shape inspection interfaces. |
| `solid.h` | `SolidEngine`, `SolidDesc`, rigid-body state/control and raycasts. |
| `gel.h` | `GelMesh`, `GelDesc`, `Gel`, `GelSolver`, soft-body surface extraction and rigid bodies in the same solver. |

Factories return owning `AutoRef` objects. `SolidEngine` owns its simulated solids;
`GelSolver` owns its gels and solids. Remove bodies through their owning solver.
A retained object reference is not an immutable physics snapshot; copy the
numerical state or surface data needed by another consumer. Keep the solver alive
while using its bodies, particularly Gel objects and solids owned by GelSolver.

## Rigid bodies

`SolidEngine` wraps a Jolt physics system and worker pool. Supported features are:

- Static, kinematic and dynamic bodies, with box, sphere, capsule, cylinder,
  convex and triangle-mesh hull factories. Mesh hulls are primarily for static scenery.
- Position/orientation, linear/angular velocity, force/torque, impulses, sleep
  control, AABB queries and runtime material updates through `Solid`.
- Gravity, body counts and nearest raycast queries through `SolidEngine`.
- Optional CCD through `SolidDesc::ccd = true`, mapped to Jolt's `LinearCast`
  motion quality. The default is discrete collision detection.

Mass is set by `SolidDesc::massOverride` when positive, otherwise calculated from
hull volume and `Temper::density` (with a positive fallback). `Temper` does not
have a `mass` field. Body pose is `SolidDesc::transform`, not a direct `position`
field. Hull creation is through `Hull` factories, not `BoxHull::create()`.

The GNfiz API does not currently expose joints, convex sweeps, trajectory
prediction, speculative simulation sandboxes or a GPU debris solver. Jolt's
underlying capabilities do not imply corresponding GNfiz interfaces.

## Soft bodies

`GelSolver` also uses a CPU Jolt physics system. It creates both soft gels and
rigid solids in one simulation for soft/rigid interaction; a separately created
`SolidEngine` is a separate simulation and does not exchange contacts with it.

`GelMesh` provides cube, tetrahedral sphere, hollow sphere and custom mesh
factories. Custom construction groups surface vertices first and remaps indices.
`GelDesc` configures XPBD edge-distance and tetrahedral-volume constraints,
solver iteration count, damping, friction, restitution and pneumatic pressure.
Nonzero pressure at creation omits tetrahedral-volume constraints to avoid
conflicting constraints. `setPressure()` updates pressure; it does not rebuild
the constraint set selected at creation.

`Gel::surfaceVertices()` creates a CPU `Blob` of deformed positions and normals,
in world space by default or center-of-mass space when requested.
`surfaceIndices()` supplies the matching surface topology. The gel sample reads
this CPU data and uploads it with gpu2 before rendering through FX2. This is not
a GPU compute solver or a zero-copy, zero-upload physics/rendering path.

Current implementation details callers should account for:

- Particle inverse masses come from `GelMesh::Vertex::invMass`.
  `Gel::mass()` reports rest volume times material density; the solver does not
  rescale particle inverse masses from that reported mass.
- Creation uses `GelDesc::linearVelocity` for particle velocities; per-vertex
  initial velocity and `GelDesc::angularVelocity` are not applied by the adapter.
- Edge and volume compliance come from `GelDesc`; individual mesh constraint
  compliance values are not passed through by the current adapter.
- GNfiz does not expose a soft-body CCD or self-collision configuration contract.
  The rigid-body CCD regression is not evidence of continuous soft-body collision.

## Time, scale and repeatability

`UnitOfTime` is `std::chrono::nanoseconds`, shared with E2. This makes duration
arithmetic exact at the API boundary. Solvers convert the duration to floating-point
seconds internally and subdivide positive steps to approximately 1/60 s or less.
There is no persistent fixed-step accumulator promising equivalence between
arbitrary groupings of calls to `step()`.

`Scalar` is `float`. Callers choose consistent length, mass, density, gravity and
velocity units. Default gravity and material values suit metre/kilogram-style
scenes, but unit choice and numerically suitable shape sizes remain the caller's
responsibility. GNfiz does not perform E2's 128-bit coordinate rebasing.

Stepping behavior differs by solver:

| Operation | `SolidEngine` | `GelSolver` |
| --- | --- | --- |
| Positive duration | Advance the Jolt simulation. | Advance the Jolt simulation. |
| Zero duration | No-op. | No-op. |
| Negative duration | Invert dynamic-body velocities, step forward by the magnitude, then invert back and correct the gravity position offset. | No-op; backward soft-body evolution is not implemented. |

The rigid negative-step path is tested for undamped translation and gravitational
freefall with numerical tolerances. It is not an exact inverse of arbitrary
collisions, friction, damping, sleeping or solver history, and does not guarantee
return to a bit-identical state. `SimulationMode` is currently stored and returned
by the solvers; changing it does not select a different integration algorithm or
automatically disable dissipative material properties.

Repeatability tests cover specific scenes in the current build. The rigid test
compares final positions, orientations and velocities bit-for-bit for 16 bodies
and 60 steps across ten additional single-worker runs. A separate 1/2/4-worker
check compares one body's final height within 0.01 units. The gel test compares
surface positions/normals across two single-worker runs. These checks do not
establish universal cross-platform or cross-thread-count bit identity.
`entityId` is exposed as caller metadata; the adapter does not implement its own
entity-ID-sorted contact/reduction pipeline.

## E2 boundary

The [E2 World runtime](../e2/README.md) shares the time type but its active
`DynamicsLaw` still uses a local AABB box solver. It does not invoke `SolidEngine`
or `GelSolver`. In particular, E2's demo has no CCD even though GNfiz rigid
bodies can enable it.

A future adapter must read tick-start Prime, rebase E2 coordinates into a suitable
local physical frame, solve and write final FacetValues through Slate. Mutable
Jolt continuation state needs explicit ownership and failure handling; retaining
a PrimeView does not version or roll back a live Jolt solver. E2 currently halts
on tick failure and does not promise automatic backend recovery.

## Standalone rigid-body example

This uses the current public API and needs no window or GPU:

```cpp
#include <garnet/GNfiz.h>

bool simulateFallingBox() {
    using namespace GN::fiz;

    SolidEngineDesc settings;
    settings.gravity = {0.0f, -9.81f, 0.0f};
    settings.numWorkerThreads = 1;
    auto engine = SolidEngine::create(settings);
    if (!engine) return false;

    SolidDesc floor;
    floor.hull = Hull::createBox({10.0f, 0.5f, 10.0f});
    floor.motionType = MotionType::STATIC;
    floor.layer = CollisionLayer::NON_MOVING;
    floor.transform.position = {0.0f, -0.5f, 0.0f};
    if (!engine->createSolid(floor)) return false;

    SolidDesc box;
    box.hull = Hull::createBox({0.5f, 0.5f, 0.5f});
    box.transform.position = {0.0f, 5.0f, 0.0f};
    box.massOverride = 5.0f;
    box.temper.friction = 0.5f;
    box.ccd = true;
    auto body = engine->createSolid(box);
    if (!body) return false;

    for (int i = 0; i < 120; ++i) engine->step(UnitOfTime {16'666'667});
    return body->position().y < box.transform.position.y;
}
```

## Samples and checks

Only two standalone fiz sample targets currently exist:

- `GNsample-fiz-solids`: interactive rigid-body scene and a finite 60-step test mode.
- `GNsample-fiz-gel`: interactive soft-body scene and a finite 180-step test mode
  checking freefall, impact, rebound and volume stability.

Both visualize CPU simulation using gpu2/FX2. The solver library itself is
headless; the visual samples still require a working GPU backend in test mode.

```bash
source env/garnet.rc
build.py d --target GNtest-internal GNsample-fiz-solids GNsample-fiz-gel
env/bin/cit.py -i '[fiz]'
DISPLAY= build/linux.gcc.d/bin/GNsample-fiz-solids t
DISPLAY= build/linux.gcc.d/bin/GNsample-fiz-gel t
# Omit t to run a sample interactively on a display.
```

Tests under `src/core/fiz/test/` cover:

| Test file | Actual coverage |
| --- | --- |
| `solid-test.cpp` | Hull volumes/bounds, body lifecycle, motion/impulses and raycast results. |
| `solid-collision-test.cpp` | Five-box stacking, relative restitution and a fast projectile against a thin wall with CCD. |
| `deterministic-forward-test.cpp` | The repeated-run and worker-count checks described above. |
| `t-reversal-symmetry-test.cpp` | Undamped translation and freefall reversal, with 0.05/0.1-unit tolerances. |
| `integer-time-test.cpp` | 3,600-step integer duration accumulation/cancellation and chrono unit conversion. |
| `scale-and-coordinate-test.cpp` | Kilometer- and centimeter-scale freefall; no E2 coordinate-rebasing test. |
| `gel-softbody-test.cpp` | Mesh generation/remapping, lifecycle, impact volume error within 0.5% for the tested cube, recovery, repeated surface output and inflatable pressure/rebound. |

There is no `query-prediction-test.cpp` and no test establishing the previously
planned orbital, arbitrary substep-equivalence or universal reversibility claims.
On 2026-10-05, the full Linux GCC debug build and Xvfb CIT passed (211 unit
tests and 185 internal cases / 58,778 assertions, including fiz). Both finite fiz
samples exited successfully: 60 rigid steps and 180 gel steps with all scenario
checks passing. The standalone C++ example above passed a compiler syntax check.
Windows and Android were not checked in this run. Detailed integration verification is recorded in
[`agent/E2_MASTER_PREPARATION.txt`](../../../agent/E2_MASTER_PREPARATION.txt).

## Not implemented

Cloth (`Weft`), hair/rods (`Strand`), smoke/fire (`Plume`/`Blaze`), particle fluids
(`Current`), ocean/shallow water (`Tide`) and a unified `Domain` remain roadmap
concepts. Their previously described public headers, solvers and sample targets
do not exist. GPU physics buffers shared directly with rendering, procedural
stream-out integration and automatic RDG2 imports are also future work.

# Garnet Fiz (`GN::fiz`)

> Note: In the Garnet repository layout, engine core modules live under `src/core/fiz/`.
> The canonical document is maintained at [`src/core/fiz/README.md`](../core/fiz/README.md).

`fiz` is Garnet's low-level physics, dynamics, and continuous media simulation
module. It occupies the same architectural layer as `gpu2` and `fx2`: atomic,
reusable, high-performance, and entirely independent of any specific world,
entity, or game-engine architecture.

## Architectural Role and Principles

- **Canonical Temporal Definition (`UnitOfTime`)**: `fiz` defines the engine's canonical
  discrete temporal unit as `using UnitOfTime = std::chrono::nanoseconds;`. Higher-level
  modules (`e2`) inherit this exact definition.
- **Physical Scale & Unit Agnosticism (`Scalar`)**: `fiz` operates on floating-point precision
  via `using Scalar = float;`. Crucially, `fiz` **does not hardcode SI meters or any specific physical dimension**.
  A scalar unit of `1.0` can represent a meter, an astronomical unit (AU) for planetary orbits,
  or a nanometer for molecular dynamics. The caller (`e2::PhysicalScale` or standalone client)
  defines the meaning of the unit.
- **100% Strict Forward Determinism**: Given an initial physical state $S_0$ and a sequence of discrete
  integer timestamps, the simulation is guaranteed to reach the exact same, bit-identical state
  every single run. Non-determinism is eliminated via:
  1. Strict IEEE 754 compliance (`/fp:precise` on MSVC, `-ffp-contract=off -fno-fast-math` on Clang/GCC).
  2. Deterministic parallel reduction: constraint solving, contact generation, and force summation
     are executed in a stable order keyed by immutable unique entity IDs, eliminating thread-scheduling races.
- **$T$-Symmetry (Time-Reversibility Where Physically Permissible)**: Physical laws in `fiz`
  strive for time-direction neutrality. Conservative systems (gravity, orbital mechanics, elastic
  springs, conservative XPBD constraints) support negative integer timesteps ($-\Delta t$) via
  symplectic integrators (e.g., Velocity Verlet), returning to identical initial states without
  history caches. Where physical laws themselves introduce thermodynamic dissipation (e.g. inelastic
  energy loss or plastic deformation), physical entropy is respected.
- **Zero E2 Dependency**: `fiz` does not include `<garnet/GNengine2.h>` and has
  no knowledge of `Universe`, `World`, `Form`, or `Facet`. It operates strictly
  on raw numerical coordinates, geometry buffers, and `gpu2` resources. 128-bit world coordinate
  rebasing is handled entirely at the E2 boundary.
- **Dual Consumption**:
  - **With E2**: consumed by `e2::PhysicalDomain` and `e2::PhysicalFacet` to
    provide world-lifecycle management, scene hierarchy synchronization, and
    gameplay integration.
  - **Standalone**: usable independently in offline baking tools, unit tests,
    dedicated headless servers, or lightweight non-E2 applications.
- **Zero CPU-GPU Readback for GPU Simulations**: flexible bodies, volumetric
  fluids, and ocean surfaces run directly via `gpu2` Compute Shaders. Their
  output buffers and 3D textures transition seamlessly into `rdg2` artifacts for
  rendering without round-trips to CPU host memory.

## Public API Layout

Client code includes only:

```cpp
#include <garnet/GNfiz.h>
```

Subheaders live under `src/inc/garnet/fiz/` and reject direct inclusion via the
`__GN_INSIDE_FIZ_H__` guard macro:

- `common.h`: canonical `UnitOfTime` (`std::chrono::nanoseconds`), scalar types, math types, error codes, and shared enumerations.
- `temper.h`: `Temper` material definitions (friction, restitution, mass, stiffness, damping).
- `hull.h`: collision shapes and bounding geometry (`Hull`, `BoxHull`, `SphereHull`, `ConvexHull`, `MeshHull`).
- `solid.h`: rigid-body dynamics (`Solid`, `SolidDesc`, `SolidEngine`).
- `gel.h`: soft and bouncy volumetric body simulation (`Gel`, `GelDesc`, `GelSolver`).
- `weft.h`: XPBD cloth simulation (`Weft`, `WeftDesc`, `WeftSolver`).
- `strand.h`: hair and rod dynamics (`Strand`, `StrandSolver`).
- `plume.h`: 3D grid-based smoke and fire solvers (`Plume`, `Blaze`).
- `tide.h`: ocean wave spectrum and shallow-water simulation (`Tide`, `Current`).
- `domain.h`: standalone physical simulation space (`Domain`).

## Physical Manifestations

`fiz` models physical reality through distinct fundamental manifestations of matter,
named in harmony with Garnet's evocative conceptual aesthetic:

### 1. Solid (`Solid` — Large-Scale Rigid Body Physics)

`Solid` represents impenetrable, rigid physical bodies governed by classical
Newtonian-Eulerian mechanics:

- **Collision Hulls (`Hull`)**: Box, Sphere, Capsule, Cylinder, Convex Mesh, and
  Static Triangle Mesh.
- **Physical Temper (`Temper`)**: density, mass, center of mass, linear/angular
  damping, friction, and restitution coefficients.
- **Solver Engine**: multithreaded CPU rigid-body engine backed by Jolt Physics,
  supporting continuous collision detection (CCD), island sleeping, and constraint
  joints.
- **Debris Acceleration**: optional lightweight GPU compute path for tens of
  thousands of non-interactive or particle-like rigid debris fragments.

### 2. Gel (`Gel` — Soft & Bouncy Volumetric Bodies)

`Gel` represents deformable, viscoelastic, and bouncy volumetric bodies
(such as rubber balls, cushions, silicone, gelatin, inflatables, and squishy organics):

- **Formulation**: Tetrahedral mesh **XPBD (Extended Position-Based Dynamics)**
  and Shape Matching solvers running on GPU Compute (`gpu2`).
- **Constitutive Constraints**:
  - **Elastic Recovery**: edge and Neo-Hookean volumetric strain constraints
    restoring the body's rest shape.
  - **Hydrostatic Incompressibility**: tetrahedral volume preservation preventing
    artificial volume collapse during impacts, creating natural squash-and-stretch bulging.
  - **Inflatables**: internal pneumatic overpressure constraints for balloons, tyres,
    and air-filled bouncy balls.
- **Bounciness & Damping**: configurable restitution, internal viscoelastic damping,
  and surface friction.
- **Collisions**: continuous collision with `Solid` hulls and dynamic self-collision.

### 3. Weft & Strand (Flexible Bodies: Cloth & Hair)

- **`Weft` (Cloth & Fabrics)**:
  - Formulated using **XPBD (Extended Position-Based Dynamics)** on GPU Compute.
  - Enforces distance, shear, and dihedral-angle bending constraints.
  - Aerodynamic lift and drag forces based on surface triangle normals.
  - Collision handled against dynamic spheres, capsules, and signed distance
    fields (SDFs).
- **`Strand` (Hair, Fur & Ropes)**:
  - Cosserat rod formulation / XPBD chain dynamics on GPU Compute.
  - Simulates guide strands with stretching, bending, and torsion constraints.
  - Tessellation/interpolation from guide strands to dense visual hair strands.

### 4. Plume & Blaze (Gaseous Phenomena: Smoke & Fire)

- **`Plume` (Smoke & Vapor)**:
  - 3D Eulerian grid simulation solving incompressible Navier-Stokes equations
    on `gpu2` Compute.
  - High-order MacCormack advection, Boussinesq thermal buoyancy, and vorticity
    confinement to preserve fine, turbulent swirling details.
- **`Blaze` (Fire & Combustion)**:
  - Coupled reaction model: fuel consumption, heat generation, expansion, and
    soot production.
  - Outputs 3D density and temperature volume textures directly imported into
    `rdg2` as relics for raymarched volumetric shading.

### 5. Current & Tide (Liquids: Water & Ocean)

- **`Current` (Small-Scale & Splash Fluid)**:
  - Lagrangian particle simulation using SPH (Smoothed Particle Hydrodynamics)
    or FLIP/PIC via GPU Compute.
  - Accelerated by GPU spatial hashing grid.
  - Rendered via screen-space fluid (SSF) or surface reconstruction.
- **`Tide` (Large-Scale Water & Ocean)**:
  - Tessendorf 2D IFFT wave spectrum (Phillips / JONSWAP wave spectra).
  - Dynamic displacement maps, folding foam maps, and normal maps computed on GPU.
  - 2D Shallow Water Equations (SWE) for near-shore shallow waves, boat wakes,
    and obstacle ripples.

## Shared Geometry & Procedural Object Data Flow

To maximize efficiency and eliminate redundant VRAM allocations, GPU-based physics
and visual rendering share geometry data seamlessly:

### 1. Zero-Copy Shared GPU Geometry Buffer
- **Unified Buffer Allocation**: For deformable volumetric bodies (`Gel`), cloth (`Weft`),
  hair rods (`Strand`), and ocean surfaces (`Tide`), the simulation compute shader writes
  updated vertex positions, normals, and tangents directly into a `gpu2::Buffer`
  (bound as an SSBO / Storage Buffer during compute dispatch).
- **Direct Rendering Consumption**: In `rdg2`, this exact same `gpu2::Buffer` is declared
  as a shared artifact. A GPU pipeline barrier (`Compute Write -> Vertex / Index / Shader Read`)
  transitions the resource, allowing raster passes to bind it immediately as a Vertex/Index
  Buffer or Storage Buffer for rendering.
- **Zero CPU Readback & Zero GPU Duplication**: Physics and rendering share the exact same
  underlying GPU memory without CPU involvement or duplicate GPU copies.

### 2. Procedural Objects: Stream-Out vs. Dual Evaluation
For procedural geometry (marching cubes, terrain tessellation, fractured meshes, procedural foliage/hair):
- **Stream-Out Architecture (Approach B, Default)**: The procedural generator runs **once**.
  The resulting vertices and indices are streamed out / stored into a unified GPU storage buffer
  first, and then passed simultaneously to both `VisualDomain` and `PhysicalDomain` (`GNfiz`).
  - **100% Bit-Identical Guaranteed**: The visible silhouette and physical collision boundaries
    match identically down to the last float.
  - **Optimal Compute**: Saves 50% procedural generation overhead by evaluating geometry once.
- **Dual Evaluation (Approach A, Opt-In)**: Rerunning procedural evaluation twice is reserved
  only as an explicit opt-in for trivial, purely analytical functions (e.g., mathematical planes or
  simple trigonometric ripples) where intermediate buffer memory would be wasteful compared to
  evaluating a simple formula inline.

## Standalone Usage Example


```cpp
#include <garnet/GNfiz.h>

using namespace GN::fiz;

// 1. Create a standalone physical domain
Domain::CreateParameters cp;
cp.gravity = {0.0f, -9.81f, 0.0f};
auto domain = Domain::create(cp);

// 2. Define collision shape and material properties
auto boxHull = BoxHull::create({1.0f, 1.0f, 1.0f});
Temper temper;
temper.mass = 5.0f;
temper.friction = 0.5f;

// 3. Spawn a rigid solid
SolidDesc solidDesc;
solidDesc.hull = boxHull;
solidDesc.temper = temper;
solidDesc.position = {0.0f, 10.0f, 0.0f};
auto box = domain->createSolid(solidDesc);

// 4. Advance simulation independently (integer nanosecond UnitOfTime)
domain->advance(UnitOfTime(5'000'000)); // +5 ms forward
auto currentTransform = box->transform();

// 5. True T-symmetric time reversal: advance backward in time with negative duration
domain->advance(UnitOfTime(-5'000'000)); // -5 ms backward, returns to identical initial state
```

## Standalone Sample Applications (`src/sample/fiz/`)

To validate and demonstrate each unique physical feature independently of `engine2`
(`e2`), dedicated standalone sample applications live under `src/sample/fiz/`:

1. **`GNsample-fiz-solids` (`solids.cpp`)**:
   - **Demonstrates**: Large-scale rigid body mechanics (thousands of falling boxes,
     spheres, capsules, and convex hulls stacking and interacting).
   - **Validates**: Jolt Physics multithreaded solving, continuous collision detection (CCD),
     friction, restitution, and sleeping performance without E2 involvement.
2. **`GNsample-fiz-gel` (`gel.cpp`)**:
   - **Demonstrates**: Soft & bouncy volumetric bodies (`fiz::Gel`).
   - **Validates**: XPBD tetrahedral volume constraints, squash-and-stretch bulging,
     viscoelastic damping, and high-restitution bouncing impacts against static and dynamic hulls.
3. **`GNsample-fiz-cloth` (`cloth.cpp`)**:
   - **Demonstrates**: GPU XPBD cloth physics (`fiz::Weft`).
   - **Validates**: Pinned and draped fabric, wind/aerodynamic drag interaction,
     and dynamic collision against moving solid obstacles.
4. **`GNsample-fiz-hair` (`hair.cpp`)**:
   - **Demonstrates**: GPU hair and strand chain dynamics (`fiz::Strand`).
   - **Validates**: Guide-strand bending/twisting constraints and visual interpolation
     under rotational and translational acceleration.
5. **`GNsample-fiz-smoke-fire` (`smoke-fire.cpp`)**:
   - **Demonstrates**: Real-time 3D Eulerian grid simulation (`fiz::Plume` & `fiz::Blaze`).
   - **Validates**: Buoyancy, vorticity confinement, combustion reactions, and direct
     RDG2 raymarched volumetric rendering.
6. **`GNsample-fiz-fluid-particles` (`fluid-particles.cpp`)**:
   - **Demonstrates**: Small-scale particle fluids (`fiz::Current`).
   - **Validates**: GPU SPH/FLIP particle simulation, neighbor search spatial hashing,
     and fluid splashing in dynamic containers.
7. **`GNsample-fiz-ocean` (`ocean.cpp`)**:
   - **Demonstrates**: Large-scale water and ocean waves (`fiz::Tide`).
   - **Validates**: GPU 2D IFFT Tessendorf wave spectrum, interactive shallow-water ripples,
     and dynamic foam map generation rendered with PBR water shaders.

## Deterministic Verification & Internal Unit Tests (`src/core/fiz/test/`)

To safeguard `fiz` behavior with zero shortcuts, an exhaustive battery of Catch2 unit
tests lives under `src/core/fiz/test/`, automatically executed by `GNtest-internal`:

1. **`deterministic-forward-test.cpp` (100% Strict Forward Determinism)**:
   - **Multi-Run Bit-Identical Repeatability**: Simulates a chaotic 500-body colliding and
     stacking scene for 1,000 steps across 10 repeated runs. Asserts that every position,
     quaternion, velocity, and contact impulse matches **bit-for-bit** across all 10 runs.
   - **Thread-Count Invariance**: Executes the identical 1,000-step simulation using 1, 2, 4,
     and 8 worker threads. Asserts bit-identical results, proving parallel work partitioning
     and constraint sorting are completely immune to thread-scheduling order.
   - **Substepping Consistency**: Asserts that stepping 10 $\times 5\text{ ms}$ sub-steps
     yields the bit-identical result to stepping $1 \times 50\text{ ms}$ with internal accumulator.
2. **`t-reversal-symmetry-test.cpp` ($T$-Symmetry & Reversibility)**:
   - **Conservative Motion Reversal**: Integrates harmonic springs, gravitational trajectories,
     and ballistic paths $+1{,}000$ steps forward, then $-1{,}000$ steps backward. Asserts that
     bodies return to initial positions within machine precision ($\epsilon$).
   - **Planetary Orbital Symplectic Test**: Simulates N-body gravitational orbits for 5,000 steps,
     verifying strict conservation of total energy and angular momentum.
3. **`integer-time-test.cpp` (Temporal Precision)**:
   - Validates that `UnitOfTime` (`std::chrono::nanoseconds`) arithmetic accumulates zero drift
     over $10^7$ steps, with exact algebraic cancellation: $+t + (-t) \equiv 0\text{ ns}$.
4. **`scale-and-coordinate-test.cpp` (Scale Agnosticism & Precision)**:
   - Validates numerical stability and solver convergence across extreme physical scales:
     planetary orbits ($1.0 = 1\text{ AU}$), human scale ($1.0 = 1\text{ m}$), and microscopic
     structures ($1.0 = 1\text{ nm}$).
   - Verifies that 128-bit coordinate rebasing against anchor $O_{sim}$ preserves sub-millimeter
     precision at coordinates $10^{18}$ units from world origin.
5. **`solid-collision-test.cpp` (Rigid Body Mechanics & CCD)**:
   - Verifies 20-box vertical stacking equilibrium without drift or explosive jitter.
   - Verifies static and kinetic friction thresholds against analytical inclined planes.
   - Verifies continuous collision detection (CCD) prevents high-speed bullet tunneling through thin walls.
6. **`gel-softbody-test.cpp` (Soft Body Incompressibility & Recovery)**:
   - Asserts hydrostatic tetrahedral volume preservation: total volume under extreme impact load
     remains constant within $\pm 0.5\%$.
   - Asserts complete elastic recovery to rest shape after compressive release.
7. **`query-prediction-test.cpp` (Lookahead & Spatial Queries)**:
   - Asserts raycast and convex sweep hits against dynamic bodies match exact ground truth.
   - Asserts `predictTrajectory()` predicts the exact impact point that standard simulation hits.
   - Asserts speculative ghost sandboxes can be created, stepped, and destroyed with zero side-effects
     on live world state.


# Assignment: Record collision fidelity policy and current backend capabilities

- Task: E2 World Runtime responsibility boundaries ([`../TASK.md`](../TASK.md))
- Assignment: 08
- Role: Codebase Analyst ([playbook](../../../harness/roles/codebase-analyst.harness.md))
- Status: complete
- Requirement revision: R6
- Owner: current coordinator invocation
- Dependencies: 07

## Outcome

Capture the user's contextual collision-fidelity policy and compare it with current E2 and GNfiz collision capabilities without selecting the implementation backend.

## Scope and boundaries

Read-only inspection of the public physics APIs, E2 solver and Jolt shape construction. No backend choice, code moves or tests.

## Confirmed user principles

- Visual and collision shapes usually may differ, especially for complex models.
- Terrain and other strict cases need collision geometry closely aligned with the visible surface to prevent penetration.
- Static objects may use precomputed detailed collision geometry tuned for fidelity and efficiency.
- Dynamic objects may use simpler collision proxies.
- Transform scale remains authoritative for all spatial properties, including collision detection; uniform scale is the optimized common path and non-uniform is supported as a special case.

## Current-code evidence

- E2 `BodyFacet::Value` represents AABB half-extents. `DynamicsLawImpl` rejects non-axis-aligned orientations/spin and performs AABB overlap/response; it does not support terrain triangle meshes or read Transform scale.
- GNfiz `Hull` exposes Box, Sphere, Capsule, Cylinder, Convex and indexed Triangle Mesh factories. Its public `MeshHull` comment says triangle meshes are primarily for static scenery.
- `SolidDesc` associates a Hull with `MotionType::{STATIC,KINEMATIC,DYNAMIC}`. GNfiz converts `MeshHull` to Jolt `MeshShapeSettings`; primitives and convex Hulls are also converted. Dynamic mesh use is not established as supported by the public docs and should not be assumed.
- `GNfiz` is independent of E2. Its README says E2's local `DynamicsLaw` does not invoke `SolidEngine` or `GelSolver`; an E2 adapter must rebase coordinates and handle mutable solver continuation state and failed World ticks.
- This API/source inventory does not verify terrain fidelity or performance inside E2.

## Design implications (not decisions)

- Collision geometry should be independently selected data rather than inferred from visual geometry in all cases.
- A strict terrain path likely needs detailed static mesh collision and an acceptance measure for surface deviation/penetration; no numeric tolerance is established.
- Dynamic collision proxies can remain simple and separately authored/baked; allowed shape families and quality targets are unspecified.
- Transform scale must be applied consistently to collider geometry. It may be applied during cooking/precomputation or backend shape construction; representation is not selected.
- A GNfiz integration is plausible for relevant shape support, but needs a World adapter and explicit Prime/Slate consistency policy. Current E2 AABB code cannot satisfy the stated terrain requirement.

## Progress and handoff

Complete 2026-11-02. Sources inspected: `src/inc/garnet/fiz/hull.h`, `solid.h`; `src/core/fiz/fiz-jolt-common.cpp`, `solid.cpp`, `src/core/fiz/README.md`; `src/core/e2/dynamics.cpp`, `src/inc/garnet/e2/dynamics.h`, `src/core/e2/README.md`. No tests or benchmarks run.

Next: capture any remaining principles, then ask about minimum strict-terrain behavior and whether physics backend integration is in scope. Keep the accepted collision split (detailed static, simpler dynamic) separate from the unresolved backend choice.

# Assignment: Trace Transform scale support and impact

- Task: E2 World Runtime responsibility boundaries ([`../TASK.md`](../TASK.md))
- Assignment: 06
- Role: Codebase Analyst ([playbook](../../../harness/roles/codebase-analyst.harness.md))
- Status: complete
- Requirement revision: R4
- Owner: current coordinator invocation
- Dependencies: 05

## Outcome

Trace the user's confirmed non-uniform Transform scale requirement through current storage, parent resolution, presentation and physics code; identify semantic decisions without choosing them.

## Scope and boundaries

Read-only source inspection. No code changes or tests.

## Findings

- `TransformFacet::Value` currently stores only `parent`, integer `position` and quaternion `orientation` in `src/inc/garnet/e2/dynamics.h`.
- `WorldTransform` likewise exposes only position and orientation. `resolvePose()` composes parent translations by rotating the child offset and composes quaternions; it cannot return scale or shear.
- `basis::Visual::Transform` already has a `glm::vec3 scale`, and the renderer multiplies translation, rotation and scale into its object matrix.
- `extractTableau()` sets render scale from `VisualFacet::Value::halfExtent`; Transform scale is not available. The visual field name is box-specific even though the renderer consumes a generic vector scale.
- The local solver resolves Transform pose for a body, then tests overlap using unmodified `BodyFacet::Value::halfExtent`. It explicitly limits solving to axis-aligned boxes with zero spin. It neither reads nor applies Transform scale.
- A rotated non-uniformly scaled parent followed by a child rotation can yield shear in the composed linear transform. A position/quaternion/three-component-scale representation cannot represent arbitrary shear exactly. This is a mathematical design constraint, not a required choice: the runtime may constrain hierarchy semantics, preserve a full matrix/basis, or define another contract.

## Open design questions

1. Is scale stored as local Transform data and composed through parents, or are supported world transforms limited to a no-shear TRS subset?
2. Are zero and negative scale valid, or must each component be positive/nonzero?
3. Does Transform scale affect physics colliders, or are visual and collision shapes independent data as the current separate Visual/Body values suggest?
4. Does adding scale to the derived world transform need to preserve the current integer-coordinate precision approach for translation?

## Progress and handoff

Complete 2026-11-02. Evidence locators: `src/inc/garnet/e2/dynamics.h` (`TransformFacet::Value`, `WorldTransform`, `VisualFacet::Value`); `src/core/e2/dynamics.cpp` (`resolvePose`, `DynamicsLawImpl`, `extractTableau`); `src/inc/garnet/e2/basis/visual.h` (`Visual::Transform`); `src/core/e2/visual.cpp` (object matrix). No tests run.

The user asked for an English explanation of “used by solving and presentation”; the coordinator should explain that DynamicsLaw reads/writes Transform values for body simulation while `extractTableau()` reads them to produce renderer input. The present solver ignores scale; the present renderer gets scale from VisualFacet instead.

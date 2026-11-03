# Assignment: Assess E2 built-ins individually

- Task: E2 World Runtime responsibility boundaries ([`../TASK.md`](../TASK.md))
- Assignment: 04
- Role: Codebase Analyst ([playbook](../../../harness/roles/codebase-analyst.harness.md))
- Status: complete
- Requirement revision: R2
- Owner: current coordinator invocation
- Dependencies: 03

## Outcome

Classify E2 built-ins individually using current active usage and contract evidence; identify the next user decision without assuming a reuse requirement.

## Scope and boundaries

Read-only inspection of public E2 symbols and active call sites in source, tests, samples, and adjacent presentation/physics contracts. No code moves and no final architecture decision.

## Assessment

| Capability | Current active usage | Provisional responsibility assessment | Candidate direction |
| --- | --- | --- | --- |
| `TransformFacet`, `WorldTransform`, `resolveWorldTransform`, `transformChildren` | E2 dynamics implementation, World Runtime sample, E2 spatial/dynamics tests; no other active module consumer found | A spatial hierarchy contract, not a World storage primitive. Current chain composes large integer world positions and quaternions but has no stored/resolved scale. The user has confirmed that independent non-uniform scale is required. | Keep spatial responsibility separate from local AABB physics. Decide whether E2 offers a standard spatial model; define hierarchy and collider-scale semantics before implementation. |
| `positionFromMeters`, `positionToMeters` | Dynamics implementation and dynamics tests; `PhysicalScale`/coordinate primitives live in E2 spatial facilities | General coordinate conversion helpers, but the current names and placement are tied to the demo solver file. | If retained, colocate with spatial/scale utilities, not DynamicsLaw. Verify conversion assumptions and reuse before promoting. |
| `MotionFacet` | Dynamics implementation, active World Runtime sample and dynamics tests | Velocity is broadly useful state, but the current public comment makes the Facet's presence select this particular Law's integration. That behavioral meaning is solver policy. | Do not make current solver's `MotionFacet` a generic World-runtime concept by default. Keep it beside a reusable physics extension if such an extension is confirmed; otherwise app/physics sample-owned. |
| `BodyFacet`, `DynamicsOptions`, `createDynamicsLaw` | Dynamics implementation, active sample and dynamics tests | The AABB shape/material/solver assumptions are specific to the current solver. DynamicsOptions also includes `destroyBelowY`, which crosses into lifetime/demo policy. | Candidate for an optional physics extension only if users need reuse; otherwise sample-owned. Separate solver options from any destruction threshold. |
| `ContactFacet`, `CollisionStatsFacet`, `CollisionEvent` | Dynamics implementation, active sample and dynamics tests | Current data and notification semantics are produced by the local solver (final-tick pair contacts, beginnings count, box-midpoint position). Generic relationships or collision events could differ. | Retain with the physics extension or sample until an independent shared contract is identified. Do not treat these concrete types as universal Event/Facet primitives. |
| `LifetimeFacet`, `LifetimeState`, `DestroyFormIntent`, `LifetimeOptions`, `createLifetimeLaw` | Dynamics implementation, active sample and dynamics tests | Population cap, spawn RNG/rate/region, falling-body deletion threshold and generated marker are demonstration/game rules. Generic `World` already has structural create/destroy operations. | Strong candidate for sample/gameplay ownership. No evidence currently requires a general E2 lifecycle service. |
| `VisualFacet`, `extractTableau` | Dynamics implementation, active sample and dynamics tests; suspended model test refers to a distinct `MeshVisualFacet` path | The renderer input is shape-agnostic: `Tableau::Object` carries mesh ID and transform; `Assets::registerMesh()` accepts caller meshes; the renderer uses the chosen mesh ID. Active sample only fills an absent ID with the built-in box. `VisualFacet::Value` has a `meshId` but calls its scale `halfExtent`, and current mold helpers create box compositions. | Per the user's R3 principle, retain simple lit white-model rendering and ensure mesh/shape selection comes from data. Keep the sample's box default local. Review `halfExtent` naming and box factories; do not infer that the visual API or World must be box-only. Keep the renderer and Facet-to-Tableau adapter as separable concerns. |
| `registerDynamicsFacets`, `createGroundMold`, `createBoxMold` | Active sample and E2 dynamics tests | Bundled registration and box/ground recipes encode one demo composition. | Sample/extension helper, not generic runtime API by current evidence. |
| Generic `World` create/destroy, Facet registration, Law/Intent/Event routing | E2 mock and extension tests, plus dynamics consumers | These are the extensibility mechanisms the prior runtime task implemented. | Preserve unless a separate requirement challenges the runtime contract; there is no present evidence that they cause the policy coupling. |

## Evidence limits and handoff

- Repository-wide active-source search found no non-E2 application consumer of the built-in dynamic APIs. E2 tests are implementation consumers, not independent product clients.
- `basis::Visual` is a renderer service accepting generic `Tableau` data; it does not require `VisualFacet`. The current box Facet/tableau adapter is an independent seam.
- Transform has meaningful transform-only tests and is used by both simulation and presentation, but current search still finds only E2 code/sample/tests. This supports separating it from physics; it does not prove universal demand.
- Suspended SimpleWorld/model-test references are excluded from active-consumer claims, as requested by prior task scope. Revisit if user changes Q4.
- `GNfiz` remains a separate API and solver. Its README says it is not called by E2 DynamicsLaw, and an adapter must address frame rebasing and mutable solver state on failed ticks. Replacing the current solver is not required by a cleanup decision.

Visual requirement update after handoff: user requires simple lit white models with data-driven shape selection; current Tableau/Assets can carry arbitrary meshes, while the sample supplies a box fallback. The E2 `VisualFacet` scale field's `halfExtent` name and box mold helpers remain potential cleanup points.

Next question: decide whether the E2 product contract wants to provide a common built-in spatial hierarchy/large-world coordinate model. This is the main ambiguous capability after the obvious demo/solver-specific groups have been classified provisionally.

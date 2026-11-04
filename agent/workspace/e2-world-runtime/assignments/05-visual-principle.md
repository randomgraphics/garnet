# Assignment: Capture visual-data constraints

- Task: E2 World Runtime responsibility boundaries ([`../TASK.md`](../TASK.md))
- Assignment: 05
- Role: Requirements Analyst ([playbook](../../../harness/roles/requirements-analyst.harness.md))
- Status: complete
- Requirement revision: R3
- Owner: current coordinator invocation
- Dependencies: 04

## Outcome

Record the user's presentation principle as a testable requirement and distinguish renderer capability from the sample's current box choice.

## Scope and boundaries

Read-only inspection of `VisualFacet`, `Tableau`, `Assets`, the E2 visual adapter/renderer and active sample. No rendering redesign or code edits.

## Confirmed user requirement

Keep visual implementation simple: lit white models are enough. Keep the world/presentation data pure and shape-agnostic; the visual contract must not require all World objects to be boxes.

## Evidence and acceptance implications

- `basis::Visual::Tableau::Object` carries `meshId` plus transform; `basis::Assets::registerMesh()` accepts caller geometry.
- `Visual::renderFrame()` resolves each object's requested mesh, with a box fallback for an empty ID.
- `extractTableau()` copies `VisualFacet::Value::meshId` and transforms to Tableau; the active sample sets missing IDs to the built-in box.
- The current value field named `halfExtent` is copied into `Tableau::Object::transform.scale`; `createGroundMold`/`createBoxMold` encode a specific demo composition.

For a future implementation, acceptance should demonstrate that two distinct mesh assets or other agreed shape data render through the same simple lit-white path without World core or the visual adapter constructing box geometry for every object. The sample may continue to choose boxes in its own data/setup.

## Progress and handoff

Complete 2026-11-02. R3 and its evidence are recorded in `../TASK.md`. No prototype was necessary to establish the current shape-data path. Next unresolved decision is spatial hierarchy/coordinate ownership, tracked as assignment 06.

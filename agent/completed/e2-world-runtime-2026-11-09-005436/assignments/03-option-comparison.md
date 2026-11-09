# Assignment: Compare E2 responsibility-boundary options

- Task: E2 World Runtime responsibility boundaries ([`../TASK.md`](../TASK.md))
- Assignment: 03
- Role: Exploration Researcher ([playbook](../../../harness/roles/exploration-researcher.harness.md))
- Status: complete
- Requirement revision: R1
- Owner: current coordinator invocation
- Dependencies: 02

## Outcome

Compare viable boundary choices using current Garnet ownership, caller and build evidence; recommend a next decision without selecting final API ownership.

## Scope and boundaries

Read-only design comparison for generic E2 World, spatial facilities, MVP physics, lifetime and visual extraction. No prototype or production code changes.

## Assignment context

See the classification matrix in [`01-boundary-inventory.md`](01-boundary-inventory.md), E2 contract in `src/core/e2/README.md`, GNfiz boundary in `src/core/fiz/README.md`, and target collection in `src/core/CMakeLists.txt`.

## Findings and options

| Option | Boundary | Benefits | Costs / risks | Evidence fit |
| --- | --- | --- | --- | --- |
| A. App/sample-owned MVP | Keep the generic World and likely spatial primitives in E2; move the local AABB solver, demo generation policy, demo Facets/events/molds and renderer extraction to the active sample (or sample-specific files). Move/retarget solver/lifetime tests with them. | Directly makes the engine World runtime independent of the demo; smallest product-model; avoids supporting these APIs as reusable engine features. | Active sample is currently one executable linked to GNcore; code/tests need a new owner. Reuse requires copy/adaptation until an extension is formalized. Visual extract may be distinct from physics and should not be bundled automatically. | Closest to README's “sample/gameplay code” wording and current only active callers. |
| B. Optional reusable E2 extension | Keep generic World/spatial contracts in core E2; move physics/lifetime and possibly presentation into a separately named extension with explicit public umbrella and build/link target. | Preserves reusable policies without making the generic runtime own them; clear dependency direction and optional consumption if the build target is truly separable. | Garnet currently collects `src/core` sources into one `GNcore-objects` object library and wraps them in GNcore; E2 CMake only adds files to that shared source list. A true independently linked optional extension requires build/export/test integration, not just moving files or namespaces. | Appropriate only if more than the demo needs a supported reusable extension. |
| C. Internal/API grouping only | Split `e2/dynamics.h` into spatial, physics, lifecycle and presentation headers/namespaces while retaining implementation in GNcore. | Low-risk organization, clearer includes, incremental migration surface; keeps current single library and all sample behavior. | Does not make physics/lifetime optional or remove policy from core runtime packaging; may be cosmetic if module ownership remains GNcore/E2. | Useful as a preparatory step or if user's goal is discoverability/organization rather than product boundary. |
| D. Replace MVP solver with GNfiz adapter | Put an E2 Law adapter around `fiz::SolidEngine`/`GelSolver`. | Reuses the maintained Jolt-backed physics module and its broader body/shape support. | This is solver replacement, not necessary for responsibility cleanup. GNfiz is explicitly independent of E2; an adapter must map coordinate frames and translate Prime/Slate. A live mutable Jolt solver is not versioned by Prime and cannot be rolled back with a failed tick; its README calls for explicit ownership/failure handling. | Technically possible, but high cost and not supported by the user's current goal absent an explicit physics upgrade requirement. |

## Recommendation

First decide whether current physics/lifetime/visual behaviors are meant to be supported for multiple consumers or only to demonstrate the World runtime. If sample-only, Option A is the narrowest way to honor the unopinionated-core intent. If reusable, develop Option B; Option C can be an intermediate migration step but is not by itself the boundary change. Keep spatial hierarchy/coordinate helpers as a separate classification decision rather than moving them with the solver. Do not infer that GNfiz should replace the local solver.

No disposable code prototype is needed for this decision: it is about intended reuse and API support, which a compilation prototype cannot answer. If the user chooses Option B, a later small build-target spike could test the cost before committing to the extension layout.

## Progress and handoff

Completed 2026-11-02. Evidence: `src/core/CMakeLists.txt` includes E2/FIZ source lists into the common `sources`; `GNcore-objects` compiles all production sources; `GNengine2.h` aggregates FX2/FIZ/E2; `src/core/fiz/README.md` documents E2's AABB solver as separate from GNfiz and details adapter state/rollback risks. No tests run.

Handoff: ask whether reusable support outside the sample is required for (1) the AABB physics/Law, (2) lifetime generation/destruction, (3) visual Tableau extraction, and whether these should share a package boundary. Do not treat them as one indivisible feature.

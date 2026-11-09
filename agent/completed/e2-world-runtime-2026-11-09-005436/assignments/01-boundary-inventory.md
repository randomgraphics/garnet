# Assignment: Audit E2 built-in responsibility boundaries

- Task: E2 World Runtime responsibility boundaries ([`../TASK.md`](../TASK.md))
- Assignment: 01
- Role: Codebase Analyst ([playbook](../../../harness/roles/codebase-analyst.harness.md))
- Status: complete
- Requirement revision: R0
- Owner: current coordinator invocation
- Dependencies: 00

## Outcome

Produce an evidence-backed inventory of current E2 public built-ins and their implementation/caller seams so the user can choose the intended scope before any API or module moves.

## Scope and boundaries

Inspect only E2 World runtime, spatial helpers, dynamics/lifetime, visual snapshot API, umbrella includes, build source discovery, active sample and tests. Do not modify code or decide the final architecture.

## Assignment context

The starting map is in [`TASK.md`](../TASK.md). Relevant files include `src/inc/garnet/GNengine2.h`, `src/inc/garnet/e2/dynamics.h`, `src/inc/garnet/e2/world.h`, `src/core/e2/world-runtime.cpp`, `src/core/e2/dynamics.cpp`, `src/core/e2/CMakeLists.txt`, E2 tests and `src/sample/e2/world-runtime.cpp`.

## Procedure

1. Enumerate public symbols and classify what each promises: generic spatial, solver-specific physics, contact/statistics, presentation, lifecycle/sample policy, or core protocol.
2. Trace each candidate to implementations, direct consumers, test ownership and build inclusion.
3. Note dependencies that complicate extraction and callers potentially affected; distinguish direct evidence from inference.
4. Return a compact matrix with files/symbols, current owner, consumers, evidence, candidate boundary questions and unknowns. Do not prescribe a final move.

## Deliverable and acceptance

Update this assignment with the sourced inventory and handoff. It is complete when every currently exported built-in from `dynamics.h` and every public factory/helper is accounted for, direct active callers are listed, and remaining gaps are explicit. This is read-only analysis; no tests required.

## Progress and handoff

Completed 2026-11-02 by read-only inspection. No code changes or tests.

| Public symbol/group | Current responsibility indicated by API/docs | Direct active consumers found | Boundary evidence / open issue |
| --- | --- | --- | --- |
| `TransformFacet`, `WorldTransform`, `resolveWorldTransform`, `transformChildren`, `positionFromMeters`, `positionToMeters` | Versioned spatial hierarchy, validation, coordinate conversion | `dynamics.cpp`, `world-dynamics-test.cpp`; active world sample consumes transform data through Tableau; transform-only test cases register TransformFacet directly | Transform-only tests show it can operate without the solver. It is currently declared/implemented in the combined dynamics files. Whether spatial primitives are universal E2 core facilities remains a user/design decision. |
| `MotionFacet` | World-space velocities; requires Transform; described as enabling DynamicsLaw integration | `dynamics.cpp`, `world-runtime.cpp`, `world-dynamics-test.cpp` | Its data model could be general motion state, but its current contract explicitly couples presence to the MVP solver. |
| `BodyFacet`, `DynamicsOptions`, `createDynamicsLaw` | AABB solver inputs/options and physics Law construction | `world-runtime.cpp`, `world-dynamics-test.cpp` | Strong solver-specific coupling; current implementation in `DynamicsLawImpl` in `dynamics.cpp`. |
| `ContactFacet`, `CollisionStatsFacet`, `CollisionEvent` | Persistent solver-produced contacts, contact-begin statistics/events | `world-runtime.cpp`, `world-dynamics-test.cpp` | Contact concepts may be reusable, but current semantics and writers are DynamicsLaw-specific; verify desired extensibility before relocation. |
| `LifetimeFacet`, `LifetimeState`, `DestroyFormIntent`, `LifetimeOptions`, `createLifetimeLaw` | Generated-population marker, persistent RNG/clock, destroy request and sample spawn/despawn rules | `world-runtime.cpp`, `world-dynamics-test.cpp` | README itself labels these as high-priority cleanup; current `LifetimeLawImpl` owns demo generation/removal policy. Generic Form removal already exists in World APIs, so no separate generic lifecycle requirement is established by current evidence. |
| `VisualFacet`, `extractTableau` | Box mesh/extents/color payload and visual snapshot extraction from Prime | `world-runtime.cpp`, `world-dynamics-test.cpp`; renderer implementation feeds the active sample | Presentation payload is part of the public dynamics header and its implementation; may be a presentation extension or app policy. Existing E2 model test is explicitly suspended/out-of-scope in source. |
| `registerDynamicsFacets`, `createGroundMold`, `createBoxMold` | Bundle registration and demo entity recipes | Active world sample and world dynamics tests | The names/behavior are demo-oriented; there is no generic requirement established for these factories. |

Additional seams:

- `src/core/e2/dynamics.cpp` combines coordinate helpers, transform resolution/validation, the local physics solver, lifetime generation/deletion, built-in registration, molds and presentation snapshot extraction. `src/core/e2/CMakeLists.txt` globs the E2 source folder, excluding only `simple-world.cpp`; extraction to a separately built module would need build/source ownership changes.
- The active sample and physics tests are the only direct call sites found for `registerDynamicsFacets`, `createDynamicsLaw`, `createLifetimeLaw`, and demo mold factories. Generic World extension tests do not depend on these types.
- `GNengine2.h` exports `GNfx2.h`, `GNfiz.h`, E2 contracts and `e2/dynamics.h` from one umbrella header. This establishes packaging aggregation, not the user's intended module/API boundary.
- Source comments label `SimpleWorld` and associated mesh/model test sources suspended. They were not treated as active migration consumers; confirm Q4 before expanding follow-up scope.

Conclusion: current API boundaries align poorly with the README's stated intent to separate world-runtime infrastructure from physics, rendering and demo lifecycle policy. Evidence supports several possible boundaries but does not determine whether Transform, Motion, contacts, or presentation contracts should remain E2 facilities. Recommendation: let the user choose whether to explore boundary options/prototypes before moving code; do not start extraction based on naming alone.

Handoff to assignment 02: present the decision below with this inventory and refine compatibility/scope based on the user's answer. No final module placement is accepted.

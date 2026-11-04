# Assignment: Record replaceable collision-detection Law boundary

- Task: E2 World Runtime responsibility boundaries ([`../TASK.md`](../TASK.md))
- Assignment: 09
- Role: Requirements Analyst / Architect ([requirements playbook](../../../harness/roles/requirements-analyst.harness.md), [decomposer playbook](../../../harness/roles/architect-decomposer.harness.md))
- Status: complete
- Requirement revision: R7
- Owner: current coordinator invocation
- Dependencies: 08

## Outcome

Capture the user's requirement for independent, alternative collision-detection Laws and identify result-routing/design constraints in the existing World runtime.

## Scope and boundaries

Requirements and architecture mapping only. No collision API, Law subtype or implementation strategy is approved here.

## Confirmed user principle

Collision detection can be an independent system (Law) in World, with multiple implementations such as AABB, OBB, octree-based and terrain-heightmap-optimized approaches.

## Existing World contract implications

- Each Law declares exact write types and accepted Intent/Event types; each concrete Facet has one writer Law.
- All Laws in a tick read the same immutable Prime[N]; Slate is write-only and earlier Law writes are not visible to later Laws.
- An Intent sent to an active later Law can be consumed in this tick. Sending to an earlier Law or self queues it for the next tick.
- Events appear only after publication and cannot serve as simulation input.
- Therefore, if collision detection and collision response are separate Laws but must interact in one tick, their result handoff requires a typed Intent/result contract (or another explicitly versioned architecture decision). The current `CollisionEvent` is not a simulation handoff.

## Taxonomy note and open design questions

The user's examples may describe different layers: AABB/OBB geometry or overlap approaches, octree spatial acceleration, and heightmap terrain representation/specialization. They may be independently selectable, composable, or selected per collision domain. Do not flatten these into one strategy interface without confirming desired combinations.

Open questions: which Law owns integration/response; when detection observes positions relative to integration; whether result data is a batch of pairs/contacts/penetrations; how static precomputed terrain data is versioned and invalidated by Transform changes; and whether multiple detector implementations are alternatives per World or may coexist for different Forms.

## Progress and handoff

Complete 2026-11-02. Evidence: `src/inc/garnet/e2/world.h` (`LawContract`, `LawContext`), `src/core/e2/world-runtime.cpp` (Law order and Intent routing), `src/core/e2/README.md` (shared-Prime/Slate and same-tick delivery policy). No tests run.

The confirmed principle is captured in R7 of `../TASK.md`. Wait for remaining user principles before asking the user to choose among the open pipeline/interface decisions.

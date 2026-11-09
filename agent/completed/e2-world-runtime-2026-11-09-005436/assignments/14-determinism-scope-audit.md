# Assignment: Audit determinism requirements against current runtime and GNfiz

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 14
- Role: Codebase Analyst ([playbook](../../../harness/roles/codebase-analyst.harness.md))
- Status: complete
- Requirement revision: R9
- Owner: current coordinator invocation
- Dependencies: 13

## Outcome

Map current deterministic mechanisms and evidence, and identify what “100% deterministic” changes in the R8 architecture.

## Current E2 evidence

- Law execution uses registration order; tick-start membership/state and accepted intents are latched before the tick executes.
- Prime storage uses ordered `std::map` columns/forms/state. Law iteration traverses Prime queries in FormId order.
- The MVP lifetime generator stores RNG state in versioned `LifetimeState` and uses a fixed xorshift algorithm rather than hidden mutable Law RNG state.
- Form IDs come from an atomic Universe counter. If concurrent callers reserve IDs, the exact order depends on input scheduling; reproducible evolution must define a stable ordering/recorded input sequence for such registrations.
- External intents are queued and delivered in order, so input ordering is part of the deterministic trace.

## GNfiz evidence

- `src/core/fiz/test/deterministic-forward-test.cpp` compares position, orientation and velocities bitwise across ten repeated 60-step runs with one worker for one 16-body box/sphere scene.
- The same test compares one body's final height across one, two and four workers with a 0.01 tolerance, not bitwise equality.
- `src/core/fiz/README.md` explicitly says current tests do not establish universal cross-platform or cross-thread-count bit identity.
- `SolidEngineDesc::numWorkerThreads == 0` selects automatic worker count. Exact replay therefore needs a defined worker configuration unless the backend is strengthened.
- Jolt-backed solver state is mutable and not part of Prime. The World halt-on-failure policy avoids automatic continuation after failure, but replay/restart equivalence and backend initialization still need explicit handling.

## Requirement interpretation

The user requires exact repeated evolution from the same initial state under identical supplied conditions. Deterministic comparison should include every committed Prime value and ordered Event/Intent outcome, not only macroscopic trajectories. “100%/完全相同” is interpreted as exact/bitwise equality of simulation state, not tolerance-level similarity.

The user wrote “给定同样的输出”; this assignment provisionally interprets it as the same external input/Intent/registration/timestep sequence. Clarify if intended literally.

## Open scope question

Does exactness need to hold only when build, platform, backend version and worker count are identical, or also across compilers/platforms/worker counts? The latter is a substantially stronger requirement and current GNfiz evidence does not meet it.

## Progress and handoff

Complete 2026-11-02. Read-only inspection of `src/core/e2/world-runtime.cpp`, `src/core/e2/dynamics.cpp`, `src/core/fiz/test/deterministic-forward-test.cpp`, `src/core/fiz/solid.cpp`, and `src/core/fiz/README.md`. No tests run.

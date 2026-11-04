# Assignment: Assess cross-platform and cross-thread determinism feasibility

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 15
- Role: Requirements analyst / codebase analyst
- Status: complete
- Requirement revision: R9
- Dependencies: 14

## User question

The user asks whether 100% deterministic evolution across platforms and thread counts is possible.

## Findings

- It is possible to target bitwise-identical state across a defined support matrix, including different worker counts, but it is not honest to promise universal determinism across arbitrary machines, compilers, backends, inputs, or versions. It depends on a deterministic simulation implementation and strict control of every state-affecting input and output ordering.
- Vendored dependency is Jolt 5.6.0 (`src/3rdparty/README.md`). `src/3rdparty/CMakeLists.txt` explicitly forces `CROSS_PLATFORM_DETERMINISTIC=ON`.
- Jolt's upstream documentation says this mode targets determinism across compiler, build configuration, OS, architecture, and word size; it notes approximately 8% overhead and requires matching source and compile defines. It also documents non-deterministic broadphase query results, multi-thread callback ordering, and active-body ordering; consumers must filter/order results when they affect state. [Jolt deterministic simulation documentation](https://github.com/jrouwe/JoltPhysics/blob/master/Docs/Architecture.md#deterministic-simulation)
- Existing GNfiz evidence remains narrower: repeated-run bitwise comparison is single-worker; worker-count comparison is tolerance-based. Therefore enabled backend support is promising evidence, not proof that GNfiz adapters and the E2 World meet the target.

## Contract recommendation

Treat cross-platform and cross-thread-count bitwise equality as a required design/acceptance target for the explicitly supported build matrix. Define the matrix (source revision, defines, numeric mode, OS/architecture/compiler families, backend version, and worker-count range) and verify it with replay/state-hash tests. Keep outside that contract arbitrary user callback side effects, unordered queries, unsynchronized concurrent API mutation, different input order, and differing configurations not listed in the matrix. If an output affects future simulation, normalize/sort it before committing it into Prime or emitting causally ordered Events.

This is a testable goal, not a universal mathematical guarantee. Any unsupported backend or implementation unable to meet the matrix must not silently claim deterministic compatibility.

## Architecture consequences

- Fix-step progression and externally submitted commands must enter a stable total order before advancing simulation. Concurrent object creation needs explicit stable command sequencing instead of relying on racing ID reservations.
- Laws need deterministic execution and commit phases; state changes and Events from parallel work must be merged in canonical order.
- All future-affecting RNG state/algorithm belongs in versioned/hashed state or deterministic inputs.
- Physics adapter must enable the vendored Jolt mode in every supported build and avoid exposing unordered Jolt results as ordered world state.
- Acceptance needs same replay run under worker-count variants and across each supported platform/compiler pair; compare complete Prime/state and event traces bitwise, not selected coordinates with tolerance.

## Progress and handoff

Complete 2026-11-02. Read-only inspection of vendored Jolt CMake configuration and upstream deterministic-simulation contract. No tests run. Proceed to renderer lifecycle decision and then include deterministic ordering/hash semantics in public contracts.

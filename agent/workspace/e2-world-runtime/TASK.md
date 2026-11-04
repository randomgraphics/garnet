# Task: E2 Extensible World Runtime

## Overview

- Branch: `wip/e2/world-runtime` in `garnet/1`.
- Status: implementation and local runtime/replay verification complete; the supported cross-platform matrix remains unverified.
- Completed: recovered and reorganized the legacy assignment, audited current E2/physics/rendering boundaries, recorded user principles R0–R7.
- Current step: record final API/runtime review, evidence and remaining portability boundary.
- Completed in this continuation: extracted public Spatial and data-driven Presentation; added optional GNfiz PhysicsFacet/Law; integrated GNfiz into the active sample; added uniform fast-path hull reuse, exact transformed box/convex/static-mesh baking, child affine output, sorted contact snapshots and World CollisionEvents, plus explicit unsupported-shape diagnostics.
- Remaining: run other operating systems/architectures/compilers to validate portable bitwise replay; static mesh/terrain fidelity remains dependent on caller-provided Hull data and acceptance tolerance.
- Latest verification: out-of-sandbox Debug build and headless Vulkan runs passed. Standard scenario Prime/event fingerprints `cd7bc7b6f6deef69` / `91ab6e4a218e90ba` matched two one-worker runs and one four-worker run. Affine/shear probe fingerprints `131d521e2f159359` / `a608b32402ec7aaa` matched one-worker and four-worker runs; it also checked persistent contact and angular orientation output. `git diff --check` passed.
- Blockers: none for implementation. Cross-operating-system bitwise equivalence is not verified on this Windows host; unsupported GNfiz Hull combinations log and fail explicitly.

---
## Goals

### Original outcome

Implement an extensible, versioned E2 World Runtime and demonstrate it through a multiple-body simulation and independent presentation. Include required GNfiz public-interface/backend modifications in scope when current physics APIs cannot satisfy the affine-transform and deterministic requirements. The runtime foundation is already present on this branch; historical implementation and verification details are in [`the archived assignment`](../../completed/e2-world-runtime-2026-10-05-215844/2026-10-05-215844-E2_WORLD_RUNTIME.txt).

### Current interpretation (requirements revision R8 — implementation goal)

Rewrite the E2 World Runtime as a fully generic, data-driven, cross-module extensible foundation, then build real World functionality through extensions. The E2 public interface must expose the contracts other modules need to add or derive Laws and related systems, Forms, Events, Prime/state types, and new World models. No world topology, visual form, physics policy, or gameplay rule is hard-coded into the generic core.

The requested end-to-end capability set is:

1. A simple lit white-model rendering system backed by GNgpu2 and GNfx2.
2. Dynamic World object/Form creation and deletion.
3. Physics backed by GNfiz, including gravity and collision.
4. A completely general low-level World core; concrete worlds are composed by extending Laws, Forms, Events, Prime/state and related public contracts.
5. A supported path for other modules to extend the runtime or construct different World models in the same way.

The previous runtime implementation is a starting point and evidence source, not a constraint on the rewrite. Keep established principles R3–R7: visual data is shape-agnostic with simple white lighting; Transform supports optimized uniform and supported non-uniform scale which affects collision; collision fidelity varies by use case and motion type; and collision detection can be an independently replaceable Law.

On 2026-11-02, the user selected **compare boundary options and use a disposable prototype if useful** before implementation. This authorizes research/prototyping, not a permanent code move or API break. A prototype is not currently necessary: the repository's build and dependency layout already exposes the principal tradeoffs.

The user confirmed an initial presentation principle: keep the visual system simple (lit white models are sufficient), while keeping it data-driven so world data does not force all objects to one shape, such as boxes. Preserve this as a constraint in any visual API/module proposal.

The user confirmed that Transform must support scaling, including non-uniform scale where `scale_x != scale_y != scale_z`. The common uniform case should be optimized, scale affects all spatial properties including collision, and hierarchy composition uses exact affine fallback if non-uniform scale creates shear.

The user refined the scale contract: non-uniform scale is required as a less-common special case, and implementation should optimize for the common uniform-scale path. Transform scale must affect every spatial property of a Form, including collision detection. Hierarchy semantics are resolved as unrestricted composition with a full affine fallback; individual collision Laws still choose exact shape handling or an explicit approximation/error path.

Collision fidelity is contextual: visual and collision geometry usually may differ, especially for complex objects. Terrain and other cases where penetration through the visible surface is unacceptable require collision geometry closely aligned with the visible model. Static objects can use precomputed collision geometry tuned for fidelity and performance; dynamic objects may use simpler proxies.

Collision detection can be an independent World Law with alternative implementations (examples include AABB, OBB, octree-based and heightmap-optimized approaches). Keep collision detection replaceable and separate from generic World storage. Whether these alternatives are peers or composable layers is not yet decided.

### Refinement R2 — no reuse commitment yet

The user chose to assess current behaviors individually without yet promising that any must remain reusable. Treat all placement suggestions below as provisional classification, not approval to move or delete APIs.

The initial requirements are now accepted as the implementation direction. Design details that affect public contracts, World extension lifecycle, Prime customization, renderer scheduling and collision-result routing remain open and must be resolved or kept explicitly provisional before implementation crosses those boundaries.

---
## Requirements and decisions

### Confirmed

- The previous assignment's Prime/Slate/Law runtime and falling-body demo are implemented on `wip/e2/world-runtime`; its last recorded checkpoint was awaiting user review/sign-off.
- The user now explicitly requests a World Runtime rewrite, using the generic engine core plus cross-module extension modules.
- The five requested capabilities are listed in the current interpretation above.
- The visual path should remain simple, with lit white models sufficient; shape must be selected through world/presentation data rather than imposed as a universal box form.
- Transform needs scale, including non-uniform scale (`scale_x != scale_y != scale_z`).
- Uniform scale is the common case and should receive the optimized code path; non-uniform scale remains supported as a special case.
- Transform scale affects all Form spatial properties, including collision detection.
- Visual and collision shapes do not generally need to match. Terrain and other strict cases need close visual/collision agreement to prevent visible penetration.
- Static models may use precomputed high-quality collision geometry balancing fidelity and efficiency; dynamic models may use simpler collision proxies.
- Collision detection may run as a distinct Law with alternative implementations, such as AABB-, OBB-, octree- or heightmap-based methods.
- E2 is documented as an early prototype, not a stable API baseline. `SimpleWorld` and mesh-viewer are explicitly out of scope for the previous runtime refactor; whether they remain excluded for this follow-up is not yet confirmed.

### Provisional assumptions

| ID | Assumption | Confidence | Evidence | Review trigger |
| --- | --- | --- | --- | --- |
| A1 | Treat this as a new follow-up task linked to, but distinct from, the historically implemented runtime assignment. | High | Prior assignment is in `agent/completed/`; current branch and code contain the delivered runtime. | User asks to revise or reopen the historical deliverable itself. |
| A2 | Keep implementation and public API changes behind a user-reviewed boundary decision. | High | User explicitly says the final solution is not fully determined and requests gradual clarification. | User explicitly delegates a reversible prototype or narrower implementation choice. |
| A3 | Keep archived historical verification as historical evidence; do not claim it verifies the current post-merge tree. | High | Archived assignment records earlier phases and revisions. | Fresh verification is performed. |

### Unknowns and decisions

| ID | Question | Impact | State |
| --- | --- | --- | --- |
| Q1 | Should rendering be registered as a World simulation Law, as a separate World presentation extension consuming immutable Prime snapshots, or remain an application-owned observer? | High: determines scheduling and public extension lifecycle. | Provisionally resolved: attach a presentation extension alongside World; it samples immutable Prime independently and is host-scheduled. GPU dependencies stay outside generic core. |
| Q2 | Does “extend Prime” mean domain-specific typed values/state in the default World (as currently possible), or must another module be able to supply a custom Prime implementation used by an E2 World? | High: affects runtime storage and compatibility. | Resolved: default Prime uses generic typed extension state; a different World model can supply its own World/Prime implementation. No injection hook into default Prime is required by current direction. |
| Q3 | Should the old falling-body demonstration become the new GNfiz-backed sample, or should compatibility be retained as a separate optional example? | Medium: affects sample/API migration. | Resolved provisionally: the active sample now uses GNfiz; the legacy AABB Law and Facets remain available as an optional demonstration policy. |
| Q4 | Are suspended `SimpleWorld` files and mesh-viewer required migration targets or only examples of another World model? | Medium: affects scope and compatibility. | Provisionally resolved: this task supplies public extension points and one active sample; it does not migrate suspended legacy consumers wholesale. |
| Q5 | How should Transform scale compose through rotated non-uniform parent chains where exact composition can introduce shear? | High: affects transform representation, rendering and collision. | Resolved: do not restrict hierarchy combinations; use the optimized uniform-scale path normally and fall back to full affine transforms when non-uniform scale/shear is present. |
| Q6 | What is the minimum strict-terrain collision fidelity measure and which static mesh/cooking workflow is expected? | High: determines physics data and acceptance. | Open; strict agreement is required in principle, numeric tolerance and cooking ownership are not. |
| Q7 | How should collision detection Laws hand results to a same-tick response/evolution Law under shared-Prime/write-only-Slate semantics? | High: affects causal pipeline and result contract. | Open; typed Intents can reach later Laws this tick; Events are post-publication only. |
| Q8 | Are AABB, OBB, octree and heightmap implementations peer alternatives or composable geometry/indexing/terrain layers? | Medium-high: affects the plugin contract. | Open; current examples span different technical layers. |
| Q9 | How should Prime publication and GNfiz's mutable solver state remain consistent when a Law or later candidate validation fails? | High: correctness of physics integration. | Open; World halts on failure, but backend state ownership/checkpoint boundaries still need a deliberate contract. |
| Q10 | What exact OS/architecture/compiler/backend matrix and worker-count range will E2 advertise for bitwise deterministic replay? | High: sets the verification and support commitment. | Same Windows Debug build produced identical 600-tick Prime fingerprints for two one-worker runs and a four-worker run. Jolt is built with cross-platform deterministic mode. Other OS/compiler/architecture combinations remain unverified and are not advertised as proven. |
| Q11 | Does hierarchical non-uniform scale need exact shear-capable affine composition, or may Spatial restrict/approximate rotated descendants? | High: affects transform storage, rendering, collision and public compatibility. | Resolved: unrestricted hierarchy with uniform fast path and full-affine fallback. Physics backends still need explicit affine collider handling/capability rules. |
| Q12 | How should the default GNfiz rigid-body Law handle a dynamic Form whose affine/shear transform changes during simulation? | High: determines physics extension scope and behavior when the backend cannot represent deformation. | Provisionally resolved: bake affine transforms for polygonal Box/Convex shapes and static triangle meshes; require an explicit convex proxy for dynamic concave meshes and non-uniform smooth primitives. Transform/custom Law contracts remain unrestricted. |

### Boundary notes

- Generic World stays unopinionated; optional Spatial, Presentation and GNfiz extension headers provide concrete capabilities.
- Transform and data-driven presentation APIs are separated from demo physics and lifecycle APIs.
- `GNengine2.h` remains a convenience umbrella for E2, FX2 and FIZ. A finer package/build-target split can be made independently if a downstream module needs it.

---
## Scope and constraints

### In scope

- Audit existing ownership and dependency seams for World runtime, spatial helpers, physics, visual data, sample lifecycle, public headers, tests and build wiring.
- Explore viable boundaries and, if useful, create isolated reversible prototypes after requirements refinement.
- Rewrite the generic E2 World public contracts and implementation only as needed to satisfy R8.
- Implement modular World systems for GNfiz-backed physics and simple data-driven GNgpu2/GNfx2 presentation.
- Migrate the active sample and document how external modules build new World behaviors/models.

### Exclusions pending user direction

- General-purpose production physics beyond the GNfiz capabilities and collision fidelity goals established here.
- Reintroducing suspended SimpleWorld or mesh-viewer wholesale without an accepted migration need.
- A complex material/PBR/texture renderer; simple lit white models are sufficient.

### Constraints

- Branch/worktree: `wip/e2/world-runtime`; repository: `D:\dev\garnet\1`.
- E2 is an early prototype; do not infer stable Garnet conventions from its current design.
- Preserve user changes; the GNfiz and harness edits, Spatial/Presentation split and runtime sample are all part of this task's working tree.
- Implementation and documentation changes are authorized by the user's R8 request. Commit and push are not requested.
- New harness contract: task-wide state belongs here; bounded invocation state belongs in `assignments/`; requirements and graph evolve as evidence arrives.

---
## Initial code inventory (R0 evidence)

- Generic World runtime implementation is in `src/core/e2/world-runtime.cpp`; extension and routing tests are in `src/core/e2/test/e2-mock.cpp` and `world-extension-test.cpp`.
- `src/inc/garnet/e2/dynamics.h` publicly declares `TransformFacet`, `MotionFacet`, `BodyFacet`, `ContactFacet`, `CollisionStatsFacet`, `VisualFacet`, `LifetimeFacet`, `LifetimeState`, `DestroyFormIntent`, physics/lifetime options and factory functions.
- `src/core/e2/dynamics.cpp` currently combines transform resolution/validation, `DynamicsLawImpl`, `LifetimeLawImpl`, built-in registration, demo molds and visual snapshot construction.
- The active sample `src/sample/e2/world-runtime.cpp` registers these built-ins and installs both Laws. `world-dynamics-test.cpp` tests both solver and lifetime policy; `world-runtime` tests in `e2-mock.cpp` exercise the generic runtime independently.
- `src/core/e2/CMakeLists.txt` discovers the E2 source folder as a whole and only excludes suspended `simple-world.cpp`. The umbrella `src/inc/garnet/GNengine2.h` aggregates `GNfx2.h`, `GNfiz.h` and `e2/dynamics.h`.
- `src/core/e2/README.md` contains both generic runtime contracts and MVP policy documentation, and explicitly labels built-in/demo cleanup as high priority.

Inventory is a starting map, not a classification decision. Assignment `01-boundary-inventory.md` owns the next bounded audit and must check direct callers/dependencies before proposing moves.

---
## Acceptance evidence

1. Generic E2 core contains no assumptions about object shape, world topology, gravity, collision, rendering or gameplay.
2. Public extension contracts let a separate module define and register custom Forms, Facets/Values, Prime/state data, Laws, Intents and Events; demonstrate a custom module using the public umbrella interface.
3. World supports dynamic Form creation/deletion through the runtime and extension Laws.
4. Physics extension uses GNfiz for gravity/collision and separates detection from response so implementations can be selected or composed.
5. Transform uniform and non-uniform scales affect rendering and collision; uniform scale gets the common optimized path.
6. Collision data supports separate visual/collision shapes, static high-fidelity precomputed shapes, dynamic simpler proxies, and a strict terrain path.
7. Rendering uses GNgpu2/GNfx2 for simple lit white models and chooses arbitrary mesh/shape from data, not a core box assumption.
8. Other modules can add systems or construct alternate World models without editing generic core implementation.

Each implementation assignment must define its focused acceptance evidence. Code-changing phases require review assignments for source/comment/doc/contract consistency. Tests/build/sample verification will be planned in the harness graph; no test commands have been run in this turn. Historical results are not current-tree verification.

---
## Decisions log

### R0 — 2026-11-02, task reframing

- Preserved the original runtime outcome as historical context.
- Reframed the active work as exploratory responsibility-boundary refinement and cleanup.
- Did not assume that every existing Facet is misplaced or prescribe a destination module.
- Source: current user request; repository state at `5eba8e7c284737bda8c74d7e3594f23d80b148e8`; prior task record linked above.

### R1 — 2026-11-02, exploration-first direction

- User selected comparing boundaries and using a disposable prototype if it can resolve a material uncertainty.
- No prototype is justified yet: source ownership, public API aggregation and build target structure already show the main costs; a code prototype would prematurely pick one boundary.
- Keep code moves behind the next user decision about reuse needs.
- Source: user reply to the first scope question; option comparison in assignment `03-option-comparison.md`.

### R2 — 2026-11-02, evaluate reuse per behavior

- User asked to evaluate each behavior independently and make no reuse commitment yet.
- Continue with per-capability evidence and provisional classification; do not bundle Physics, Lifetime and Presentation into one move.
- Source: user's reply to the second scope question; evidence and recommendations in `04-component-assessment.md`.

### R3 — 2026-11-02, data-driven shape requirement for visuals

- Keep presentation implementation simple; a lit white-model renderer is enough for the current goal.
- Keep shape choice data-driven. World content must not be constrained to boxes; arbitrary mesh selection through presentation data is acceptable.
- Current code can represent arbitrary mesh IDs in `VisualFacet::Value`/`Visual::Tableau::Object`, and `Assets` supports registering a mesh. The active sample chooses a box when no mesh ID is supplied. The `halfExtent` field name and box-oriented factories need review against the desired generic contract.
- Source: user's principle; API evidence in `src/inc/garnet/e2/dynamics.h`, `basis/asset.h`, `basis/visual.h`; adapter/default evidence in `src/core/e2/dynamics.cpp`, `src/core/e2/visual.cpp`, and `src/sample/e2/world-runtime.cpp`.

### R4 — 2026-11-02, non-uniform Transform scale

- Transform must support scale, including independent X/Y/Z values.
- The user has not yet specified scale composition through parent transforms, zero/negative scale validity, physics-collider inheritance, or whether shear from rotated non-uniform parents must be represented.
- Current implementation has no scale in `TransformFacet::Value` or `WorldTransform`; `basis::Visual::Transform` has scale, currently populated from box-named `VisualFacet::Value::halfExtent`. The AABB solver uses `BodyFacet::Value::halfExtent` without Transform scale.
- Source: direct user response; code evidence recorded in `assignments/06-transform-scale-impact.md`.

### R5 — 2026-11-02, scale path and spatial propagation

- Treat non-uniform scale as supported but less common; optimize the common uniform-scale path.
- Transform scale affects all spatial properties of a Form, explicitly including collision detection.
- This implies that presentation and collider calculations must consume the same authoritative Transform scale, subject to clarified shape/hierarchy semantics. It does not yet require every physics primitive or solver to support arbitrary shapes/rotations.
- Source: direct user statement; the current solver limitations and renderer data path are detailed in `assignments/06-transform-scale-impact.md`.

### R6 — 2026-11-02, contextual collision fidelity

- Visual and collision geometry usually need not be identical, particularly for complex models.
- Terrain and other strict cases should have collision geometry closely matching visible geometry to prevent other models penetrating the visible surface.
- Static models may use precomputed collision geometry selected to balance accuracy and efficiency; dynamic models may use simpler collision proxies.
- This does not select GNfiz, a collision mesh format, exact tolerances, or solver architecture.
- Source: direct user statement; current GNfiz/E2 capabilities and limits are in `assignments/08-collision-fidelity-inventory.md`.

### R7 — 2026-11-02, collision detection as independent Law

- Collision detection may be a standalone World Law and support multiple implementations.
- Examples supplied: AABB, OBB, octree-based and terrain-heightmap-optimized methods.
- This supports a replaceable collision-detection boundary separate from generic World storage and merits a distinct detection/result contract.
- Open technical distinction: AABB/OBB are geometry/bounding approaches, octrees are commonly spatial acceleration structures, and heightmaps are terrain data/specialization; they may compose rather than be exclusive Law choices. Confirm before locking the API.
- Current runtime Laws all read the same tick-start Prime and cannot read earlier Slate outputs. A later Law may receive a typed Intent in the same tick; Events are publication-only. Any detection-to-response contract must respect this model or propose a versioned requirement change.
- Source: direct user statement; current Law/Intent contract in `src/inc/garnet/e2/world.h` and tick semantics in `src/core/e2/README.md`.

### R8 — 2026-11-02, extensible runtime rewrite

- User authorized rewriting the World Runtime around five outcomes: simple lit white rendering backed by GNgpu2/GNfx2; dynamic Form creation/deletion; GNfiz-backed physics including gravity/collision; a generic core with no world-content assumptions; and cross-module extensions/custom World models through public interfaces.
- Existing principles R3–R7 remain accepted constraints, including arbitrary data-selected visual meshes, uniform-optimized/non-uniform Transform scale affecting collision, contextual static/dynamic collision fidelity, and independent replaceable collision-detection Laws.
- Implementation is authorized. Public API and architecture choices remain progressively refined; do not stall independent design work, but ask before crossing unresolved high-impact boundaries.
- Source: direct user request; current interface and extension-test evidence in `src/inc/garnet/e2/world.h` and `src/core/e2/test/world-extension-test.cpp`.

### R9 — 2026-11-02, Prime extensibility and deterministic evolution

- User agreed that domain data in the default World's generic Prime can be extended through typed world-state values, while a different module may implement its own World/Prime model. Do not add a custom Prime injection point to the default World without a demonstrated need.
- Prime publication and World evolution must be 100% deterministic: same initial state and same supplied conditions must produce exactly the same repeated evolution.
- Provisional interpretation: “same supplied conditions/output” refers to the same ordered external input/Intent/registration/timestep sequence. Confirm if “输出” was meant literally.
- Determinism domain: repeated runs and 1/4-worker sample runs now match bitwise on the current Windows Debug build; the supported cross-platform/compiler matrix remains unverified.
- The user asked whether 100% determinism across platforms and thread counts is possible. The vendored Jolt 5.6.0 is built with `CROSS_PLATFORM_DETERMINISTIC=ON`; its upstream contract describes cross-platform deterministic simulation under identical ordered mutations and matching source/defines, while explicitly excluding unordered query/callback surfaces unless callers sort/normalize their results. E2 normalizes contact output and now has World-level Prime/Event fingerprint evidence for worker counts 1 and 4 on Windows, but no cross-OS run.
- Source: direct user response; current E2 order/state/RNG and GNfiz repeatability evidence in `assignments/14-determinism-scope-audit.md`.

### R10 — 2026-11-02, provisional presentation lifecycle

- User said to proceed according to the current state after the renderer lifecycle and determinism questions. Adopt Assignment 12's recommendation as a reversible default: presentation is a distinct extension attached alongside World, samples retained immutable Prime snapshots on an independent cadence, and does not execute as a simulation Law.
- Keep GPU2/GNfx2 dependencies outside generic World core. The app/host advances presentation; generic extension lifetime must not alter simulation order or committed Prime data.
- This placement is provisional and may be revised if a generic extension lifecycle couples observers to tick advancement or impairs deterministic replay. See `assignments/16-renderer-lifecycle-decision.md`.
- Source: direct user instruction to proceed using current state; repository snapshot/render service evidence in `assignments/12-renderer-lifecycle-options.md`.

### R11 — 2026-11-02, unrestricted affine-capable Spatial hierarchy

- Do not impose code-level restrictions on non-uniform-scale hierarchy combinations.
- Optimize first for the no-scale/unit-scale case (translation + rotation only), then for uniform scale (translation + rotation + one scalar). Keep both common paths compact and avoid general matrix operations where possible.
- When non-uniform scale or hierarchy composition creates shear, fall back to a full affine representation/operations so composition can remain exact. This is a rare special path and must not impose material cost on rigid or uniform-scale cases.
- This is the Spatial contract; each physics Law still needs to consume the authoritative affine-transformed collider geometry, bake/cook it, or explicitly signal unsupported behavior. It must not silently ignore scale/shear.
- Source: direct user clarification resolving Q11; see `assignments/18-hierarchy-shear-options.md`.

### R12 — 2026-11-02, GNfiz extension in scope

- If GNfiz's current interface or backend prevents the World physics Law from satisfying the agreed transform/collision/determinism contract, changes and extensions to GNfiz are part of this task plan.
- Do not treat the current GNfiz API/backend boundary as immutable. Audit the required public shape/transform update API and backend implementation, then include design, implementation, and acceptance work as dedicated graph nodes if needed.
- This broadens implementation scope but does not predetermine whether dynamic shear uses a new exact solver, a body-local affine shape update, or an explicitly approximated proxy. Decide from backend feasibility and expected correctness; never silently drop affine scale.
- Vendored Jolt confirms the boundary: `ScaledShape` supports per-axis scale, but its docs explicitly state that scaling a rotated shape can introduce shear that Jolt does not support. Jolt has `BodyInterface::SetShape`, a dynamic scaling sample, and an extension path for custom `Shape` types; GNfiz currently lacks an equivalent shape-update API. Full affine collision therefore needs shape baking/custom Jolt shape or another collision Law, not just exposing the existing wrapper.
- Source: direct user instruction; extends the original R8 scope.

### R13 — 2026-11-02, autonomous reversible implementation choices

- For choices with limited structural impact or a straightforward correction path, select a reasonable default, record the rationale/revisit trigger, and continue implementation without waiting for user confirmation.
- Escalate only unresolved choices that materially change core structure, user outcomes, compatibility/support commitments, major cost or irreversible work and lack a clear evidence-based default.
- Prefer useful bounded implementation over waiting for a complex, perfect design. Record remaining details as open/deferred and resolve them when their dependency boundary is reached.
- This interaction rule is now part of the general harness in `agent/harness/requirement-refinement.harness.md`, `agent/harness/ASEH_DESIGN.harness.md`, and `agent/harness/roles/requirements-analyst.harness.md`.
- Source: direct user instruction.

### R14 — 2026-11-02, local TODO/log/fail-fast for deferred paths

- If a technical capability is unresolved but isolated to a path that is not currently needed, keep the uncertainty local with a specific TODO and capability check; continue implementing unrelated paths.
- When the unsupported path is exercised, log the feature and relevant input/configuration context. Return an explicit error or assert/throw if continued execution would be invalid; never silently claim support or silently approximate an exact requirement.
- Track the TODO in the task graph and resolve it when that capability becomes necessary.
- This principle is now in the generic requirements, ASEH and implementer harness docs. The GNfiz/Jolt adapter logs shape-construction failures and marks the affine/shear adapter TODO at its unsupported HullType path.
- Source: direct user instruction.

---
## Task graph

| ID | Bounded outcome | Role | Depends on | Status |
| --- | --- | --- | --- | --- |
| 00 | Reframe legacy task into an R0 charter and harness graph. | Architect/decomposer | — | Complete |
| 01 | Audit each public built-in Facet/Law/helper by responsibility, dependencies, callers, tests and build wiring; produce a sourced classification matrix without moving code. | Codebase analyst | 00 | Complete |
| 02 | Record the user's exploration-first preference and identify the next high-impact reuse question. | Requirements analyst | 01 | Complete |
| 03 | Compare viable module-boundary options against current build/dependency seams; recommend next decision without moving code. | Exploration researcher / architect | 02 | Complete |
| 04 | Assess current behaviors individually and identify which boundary requires the next user decision. | Codebase analyst / requirements analyst | 03 | Complete |
| 05 | Record and trace the confirmed visual principle against current data/API/render paths; revise visual acceptance constraints. | Requirements analyst / codebase analyst | 04 | Complete |
| 06 | Trace Transform scale support through current storage, hierarchy resolution, renderer adapter and AABB solver; record semantic unknowns. | Codebase analyst | 05 | Complete |
| 07 | Record the uniform-fast-path/non-uniform-fallback principle and scale propagation to collision detection. | Requirements analyst | 06 | Complete |
| 08 | Record contextual collision fidelity needs and inventory existing static/dynamic collision capabilities. | Requirements analyst / codebase analyst | 07 | Complete |
| 09 | Record replaceable collision-detection Law principle and map same-tick result-routing constraints. | Requirements analyst / architect | 08 | Complete |
| 10 | Translate R8 into an architecture contract and audit current public extension seams, including Prime and World model replacement. | Architect/decomposer | 09 | Complete |
| 11 | Clarify whether modules extend default Prime through typed state or supply custom Prime/World implementations (or both). | Requirements analyst | 10 | Complete |
| 12 | Recommend renderer lifecycle placement using current snapshot/event and GPU service contracts. | Exploration researcher | 10 | Complete |
| 13 | Record the accepted Prime extension boundary. | Requirements analyst | 11 | Complete |
| 14 | Audit deterministic properties and current FIZ evidence against the new exact-repeatability requirement. | Codebase analyst | 13 | Complete |
| 15 | Assess feasibility and contract for cross-platform/thread-count determinism against vendored Jolt guarantees and repo configuration. | Requirements analyst / codebase analyst | 14 | Complete |
| 16 | Adopt a provisional renderer lifecycle from the audited options, preserving simulation/presentation separation. | Requirements analyst / architect | 12, 15 | Complete |
| 17 | Audit spatial, visual and collision data boundaries; identify transform composition semantics that affect public APIs. | Codebase analyst / architect | 10, 13, 16 | Complete |
| 18 | Present concrete hierarchy-shear options and clarify whether Spatial preserves affine composition or uses constrained/approximate TRS semantics. | Requirements analyst | 17 | Complete |
| 19 | Verify GNfiz/Jolt capability for non-uniform scale/shear and static/dynamic collider recooking against the new affine contract. | Codebase analyst / architect | 18 | Complete |
| 20 | Audit which outcome requirements are clear enough to implement; add and review the first bounded GNfiz shape-update slice. | Requirements analyst / FIZ implementer / Reviewer | 15, 16, 19 | Complete; built and exercised by the GNfiz-backed sample |
| 21 | Prototype the confirmed Spatial fast/slow paths through hierarchy resolution, presentation and current AABB collision; record compatibility and open numeric limits. | Spatial implementer / reviewer | 17, 18, 20 | Complete; build verified |
| 21-review | Source review and build verification of Spatial propagation, legacy Tableau compatibility, AABB bounds and explicit failure paths. | Reviewer | 21 | Complete; the headless runtime uncovered and fixed an uninitialized sample origin |
| 21a | Extract reusable Spatial and data-driven Presentation capabilities from demo dynamics while preserving the existing generic World contracts. | Architect / implementer | 21-review | Complete; build verified |
| 21b | Design public contracts for optional shape/physics capabilities, deterministic ordering and simulation/presentation observation. | Architect/decomposer | 15, 16, 19, 20, 21a | Complete; first PhysicsFacet/GNfizLaw contracts compile |
| 22 | Map required GNfiz public API/backend extensions for affine shape updates, deterministic ordering and replay-safe ownership. | Architect/decomposer / FIZ analyst | 19, 21b | Complete; Solid::setHull added, backend state/replay limits remain acceptance work |
| 23 | Prototype GNfiz/Jolt affine collider updates; compare exact convex/mesh baking and unsupported proxy routes. | Exploration researcher / implementer | 22 | Complete; polygonal baking and dynamic shear/rotation exercised by headless probe |
| 24 | Select default GNfiz behavior for dynamic affine colliders from backend evidence; custom Laws remain unrestricted. | Requirements analyst | 23 | Complete provisionally: bake polygonal affine shapes; require proxy Hull for smooth non-uniform cases |
| 25 | Implement GNfiz hull replacement and safe affine shape cooking. | FIZ implementer | 24 | Complete; build verified; runtime exercised on the common rigid/uniform path |
| 26 | Integrate GNfiz-backed World Law, dynamic Form reconciliation, affine collider behavior and Prime/Slate publication. | World implementer | 25 | Complete; 600-tick uniform and affine/shear scenarios verified with 1/4-worker fingerprints |
| 27 | Migrate the active sample to dynamic Form creation/deletion, GNfiz gravity/collision and independent lit-mesh presentation. | Sample implementer | 21a, 26 | Integrated; build verified |
| 28 | Review extension APIs, collider-fidelity limits, determinism boundaries and run authorized final verification. | Reviewer / verifier | 23, 26, 27 | Complete for current Windows Debug target; cross-OS verification remains open |

The graph is intentionally open. Do not pre-create implementation tasks for unresolved terrain tolerances, alternative collision-layer composition or backend checkpointing until those paths are needed.

---
## PROGRESS

Last completed: 28 — reviewed supported GNfiz collider paths; fixed the active sample's uninitialized zero-origin argument; verified a 600-tick headless render/simulation run.
Verification: after `. env\\garnet.ps1`, `& .\\.pyvenv\\Scripts\\python.exe env\\bin\\build.py -C d --target GNsample-e2-world-runtime` succeeded outside the sandbox. Three standard runs (workers 1, 1, 4) produced Prime/event fingerprints `cd7bc7b6f6deef69` / `91ab6e4a218e90ba`; two affine/shear runs (workers 1, 4) produced `131d521e2f159359` / `a608b32402ec7aaa`. Frame counts varied as expected. `git diff --check` passed.
Affine probe: non-uniformly scaled parent, rotated child, dynamic angular motion, persistent contact and lit rendering passed 600 ticks. Remaining: cross-operating-system/compiler bitwise replay requires additional hosts; it is not evidenced by the current Windows runs. Static terrain fidelity depends on the caller supplying a matching high-quality Hull. No implementation blocker remains for the current platform target.

---
## References

- [New harness task contract](../../harness/task-workspace.harness.md)
- [Progressive requirement refinement](../../harness/requirement-refinement.harness.md)
- [E2 World Runtime design and TODO](../../../src/core/e2/README.md)
- [Prior implemented assignment and historical evidence](../../completed/e2-world-runtime-2026-10-05-215844/2026-10-05-215844-E2_WORLD_RUNTIME.txt)
- [E2 public dynamics API](../../../src/inc/garnet/e2/dynamics.h)
- [E2 dynamics implementation](../../../src/core/e2/dynamics.cpp)

---
## Archive state

Active. Do not archive until explicit user sign-off or archive request under the host policy.

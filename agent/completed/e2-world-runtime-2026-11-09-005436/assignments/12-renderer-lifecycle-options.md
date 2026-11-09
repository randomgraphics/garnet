# Assignment: Assess renderer lifecycle placement

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 12
- Role: Exploration Researcher ([playbook](../../../harness/roles/exploration-researcher.harness.md))
- Status: complete
- Requirement revision: R8
- Owner: current coordinator invocation
- Dependencies: 10

## Outcome

Compare the requested GNgpu2/GNfx2 renderer with the current Law/Prime/Platform design and recommend a lifecycle placement for the user to confirm.

## Evidence

- `Law::tick()` is part of ordered simulation; all Laws run at most once per World tick and receive tick-start Prime plus write-only Slate.
- `PrimeView` is immutable and retainable. `World::primeSnapshot()` lets readers sample committed versions independently of simulation.
- `basis::Visual` owns GPU context/asset/rendering work and accepts a plain `Tableau`; it does not own World simulation data.
- The active World sample performs simulation and presentation on separate cadences/threads, calls `extractTableau(prime, ...)`, then invokes `Visual::renderFrame(tableau)`.
- Events are published after successful simulation publication and are observational; they do not drive Laws.
- `GNengine2.h` aggregates FX2/FIZ/E2 public headers, but that packaging alone does not make rendering part of the World lifecycle.

## Options

1. **Rendering as a simulation Law:** Simple registration, but couples GPU/presentation work to the authoritative simulation tick and Law ordering. It risks blocking simulation on frame presentation and mixes simulation with observation.
2. **Registered World presentation extension:** World publicly owns/registers a rendering observer that consumes retained snapshots independently of Law dispatch. This supports the user's “other modules add World systems” direction while preserving simulation/presentation separation. It requires lifecycle/thread/error contracts and must not add GPU2/GNfx2 dependencies to generic World core.
3. **Application-owned observer/module:** Renderer remains a separate module consuming `PrimeView`/Events; the app owns scheduling and lifetime. This keeps core minimal, but does not provide World ownership for non-Law systems unless another extension hook is added.

## Recommendation

Prefer a **separate presentation extension attached alongside the World**, consuming immutable Prime snapshots at its own cadence. Do not make GPU rendering a simulation Law. Keep registration generic and keep GPU2/GNfx2 dependencies outside the World core.

## Adopted provisional default (R10)

After the user asked to proceed with the current direction, adopt the reversible option of a distinct presentation-extension lifecycle attached to a World, separate from ordered simulation Laws. The extension receives retained immutable snapshots and owns its frame cadence; E2 core only defines the generic extension/observer contract and does not link GPU2/GNfx2. The application or host controls when presentation advances. This keeps the requested extensible-system direction while preserving the standalone `Visual::renderFrame(Tableau)` service.

This is a provisional default, not a claim that every World must have presentation. Revisit only if public API design reveals that a generic extension lifecycle would couple read-only observers to simulation lifetime or compromise deterministic simulation.

## Progress and handoff

Complete 2026-11-02. Read-only inspection of `src/inc/garnet/e2/world.h`, `e2/basis/visual.h`, `e2/basis/platform.h`, `src/core/e2/visual.cpp`, `src/sample/e2/world-runtime.cpp`, and E2 design notes. No tests or prototype.

Renderer lifecycle placement is adopted provisionally as R10 after the user directed us to continue based on the current state.

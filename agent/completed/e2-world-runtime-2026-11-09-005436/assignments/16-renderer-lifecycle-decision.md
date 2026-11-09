# Assignment: Adopt provisional renderer lifecycle

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 16
- Role: Requirements analyst / architect
- Status: complete
- Requirement revision: R10
- Dependencies: 12, 15

## Decision

The user said to proceed according to the current state. Adopt the recommendation from Assignment 12 as a reversible default: a presentation extension is attached alongside a World, independently scheduled from simulation Laws, and consumes retained immutable Prime snapshots. The app/host decides when frames advance. Rendering is observational and cannot mutate simulation state through this contract.

The generic E2 core may expose a GPU-agnostic extension lifecycle, but must not depend on GNgpu2/GNfx2. A module implementing presentation may depend on those services. The existing `basis::Visual::renderFrame(Tableau)` remains a rendering service that can be driven by the extension.

## Why this default fits current evidence

- `PrimeView` is immutable and retainable; snapshots naturally support independent sampling cadence.
- `Law::tick()` participates in deterministic authoritative evolution. Rendering in a Law would couple frame timing and GPU stalls to simulation.
- `basis::Visual` already accepts pure-data `Tableau` and owns GPU/presentation resources separately from World state.
- This default can be replaced with app-owned observation without changing state/Law semantics if a generic extension lifecycle proves too coupled.

## Constraints and revisit trigger

- Extension registration/lifetime must not affect simulation tick order, RNG, Prime contents, or replay output.
- Observer callbacks must not receive mutable Slate or live mutable solver state.
- World destruction and outstanding retained snapshots need explicit lifetime semantics.
- Revisit if the generic extension interface entangles rendering cadence with tick advancement or introduces GPU dependencies to the World core.

## Progress and handoff

Complete 2026-11-02. Decision derived from previous repo audit and user's instruction to continue; no code/tests run. Proceed to public-contract design, keeping observer lifecycle separate from simulation Law lifecycle.

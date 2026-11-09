# Assignment: Translate R8 and audit current extension surfaces

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 10
- Role: Architect and Decomposer ([playbook](../../../harness/roles/architect-decomposer.harness.md))
- Status: complete
- Requirement revision: R8
- Owner: current coordinator invocation
- Dependencies: 09

## Outcome

Record the new implementation goal and identify what the current public interfaces already allow versus what remains missing for cross-module World/Prime extension.

## User goal

Rewrite E2 World Runtime around a generic core and modular capabilities: simple lit-white rendering using GNgpu2/GNfx2; dynamic object creation/deletion; GNfiz physics including gravity/collision; a core with no assumptions about world content; and public extension paths for other modules to add Laws, Forms, Events, Prime/state and new World models.

Carry R3–R7 visual, scale and collision principles forward. Implementation is authorized; commit/push are not requested.

## Existing public extension evidence

- `Facet`, `FacetValue`, `Form`, `PrimeView`, `Law`, `Intent` and `Event` are public polymorphic contracts in `src/inc/garnet/e2/world.h`.
- `Mold::FormFactory` allows an external module to create a custom `Form` subtype with a reserved ID.
- `World::addLaw`, `registerFacet`, `registerState`, `createForm`, `initializeState` and typed Prime/Slate methods support extensions in the default runtime.
- `src/core/e2/test/world-extension-test.cpp` deliberately includes only `garnet/GNengine2.h`, then defines a custom `Charge` Facet/Value, `AppForm`, `ChargeLaw` and custom world state. It demonstrates old Prime snapshots retaining polymorphic value types.
- `PrimeView` is an abstract public read contract, but default `PrimeImpl` storage is private and constructed by `World::create`. The default implementation has no public factory hook for a module-supplied Prime implementation.
- External modules can add typed world state (`FacetValue` + `registerState<T>()`), and can implement their own `PrimeView`/`World` subclasses, but the latter means supplying the full World implementation rather than augmenting the default World.

## Architecture consequence

R8's “extend Prime” phrase is materially ambiguous between adding domain data to the default versioned Prime, replacing/deriving the Prime implementation, or doing both. The current typed-state path already covers the first meaning for data, while custom Prime/World model implementations cover the second only at full interface implementation cost. Clarify before changing default storage/factory contracts.

## Progress and handoff

Complete 2026-11-02. Read-only source inspection only. Evidence: `src/inc/garnet/e2/world.h`, `src/core/e2/world-runtime.cpp`, `src/core/e2/test/world-extension-test.cpp`, and `src/core/e2/README.md`. No tests run. Assignment 11 asks the user to clarify desired Prime extension semantics; rendering lifecycle remains a separate later decision.

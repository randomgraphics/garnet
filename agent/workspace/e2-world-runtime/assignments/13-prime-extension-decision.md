# Assignment: Record Prime extension boundary

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 13
- Role: Requirements Analyst ([playbook](../../../harness/roles/requirements-analyst.harness.md))
- Status: complete
- Requirement revision: R8
- Owner: current coordinator invocation
- Dependencies: 11

## Outcome

Record the user's decision about extending default Prime versus implementing alternate World models.

## User decision

The user agreed with the recommendation: default World Prime remains generic and extensible through typed state values; modules that need a different World/Prime model may implement their own World and Prime contracts. Do not add a default-Prime injection hook without a concrete use case.

## Acceptance implications

- A public-only extension example should register a custom `FacetValue` world-state type and evolve it through a custom Law in the default World.
- Public contracts should remain sufficient for another module to implement its own `World` and `PrimeView` model without depending on private storage.
- Do not add domain-specific fields/methods to the generic default Prime.

## Progress and handoff

Complete 2026-11-02. User response recorded as R9 in `../TASK.md`. Determinism is a new cross-cutting requirement; the next assignment audits current order/state/FIZ guarantees before its environment scope is clarified.

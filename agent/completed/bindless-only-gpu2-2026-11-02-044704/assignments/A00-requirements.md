# Assignment: Refine bindless migration requirements

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A00
- Role: Requirements analyst (`../../../harness/roles/requirements-analyst.harness.md`)
- Status: complete
- Requirement revision: 0.2 user-confirmed
- Owner: coordinator
- Dependencies: none

## Outcome
Produce a clear requirement revision, decision record, and acceptance criteria for retiring bind-based Raster/CnC.

## Scope and boundaries
Own interpretation of the user request, scope uncertainty, and observable acceptance criteria. Do not design APIs or modify source. The user confirmed full bindless-only migration; preserve that decision accurately without extending its scope.

## Assignment context
Read the task charter, `agent/harness/requirement-refinement.harness.md`, `agent/harness/roles/requirements-analyst.harness.md`, and the initial evidence E-001/E-002.

## Procedure
1. Separate desired outcome from migration means.
2. Identify ambiguity, compatibility implications, and acceptance gaps.
3. Record proposed options and recommendation, keeping the human decision pending until it arrives.
4. Incorporate the received human answer as requirement revision 0.2.

## Deliverable and acceptance
Requirement revision and concise decision/acceptance record in this file and `TASK.md`; explicit links to user intent and source evidence.

## Progress and handoff
### Requirement refinement (rev 0.2 confirmed)

- **Outcome:** the user wants bind-based Raster/CnC retired, affected functionality moved to bindless, and the first task to exercise the full harness lifecycle, including multi-assignment tracking and verification/rework. Source: initial user request.
- **Confirmed constraints:** keep the task resumable in `agent/workspace/`; produce bounded assignments; report test evidence and any repair/retest loop; retain task files until explicit sign-off, per repo guidance.
- **Confirmed scope:** delete `GpuRaster` and `GpuCnC`, migrate transfer-only operations too, remove `bindless::Raster::recordBindBasedDraw`, and migrate transfers, FX2 ImGui and legacy SSC. Existing FX2 bindless material kernels need no rewrite.
- **Acceptance criteria:** retired public APIs, implementations and bound draw method are absent; all in-scope operations/callers work through bindless; focused GPU2/FX2 tests and builds cover raster, compute, transfer and ImGui; no stale API references remain; exact checks, failures, skips and repair/retest are recorded; independent audit finds no missed first-party callers.
- **Clarification exchange:** user asked whether any code still references bind-based CnC. Answered yes: old resource-table compute is used in `src/sample/gpu2/simple-compute.cpp` and GPU2 compute/CnC tests. `GpuCnC` is also used for transfer-only operations by `simple-copy.cpp`, FIZ geometry upload helpers, FX2 shared shader constants, and texture content upload. User then chose full GpuCnC removal with transfer migration and confirmed complete bindless-only including bound draw, ImGui and legacy SSC.

Requirement revision 0.2 and decisions D-001/D-002 are recorded in TASK.md. A10-A15 implementation assignments follow this confirmed scope.

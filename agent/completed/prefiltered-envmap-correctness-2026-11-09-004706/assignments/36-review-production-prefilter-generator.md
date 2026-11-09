# Assignment: Review the production prefilter generator change

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 36-review-production-prefilter-generator
- Role: Reviewer (`../../../harness/roles/reviewer.harness.md`)
- Status: complete; generator scope/fidelity passes, render parity outstanding
- Expected first checkpoint: inspect the production diff and report any scope or correctness issue within 15 minutes
- Verify: independently compare production implementation and output with assignment 30 evidence
- Commit: none; read-only review

## Outcome

Independently verify that the production generator now reproduces the strongest source-width-128 candidate without changing shader/renderer logic or unrelated environment inputs.

## Inputs

- `../TASK.md`
- `35-integrate-source128-prefilter.md`
- `26-pdf-lod-formula-audit.md` (corrected basis interpretation)
- `29-source-cube-width-128-probe.md`
- `30-hammersley-roles-at-source-128.md`
- `34-source-mip-chain-equivalence.md`
- `src/tool/texture-toolbox/gen-ibl-maps.py`
- Assignment 30 DDS/capture and assignment 35 candidate DDS/capture

## Scope and procedure

Read-only. Review the production diff against the pinned sampler behavior and assignment 30 evidence: source cube size, sample count/sequence roles, tangent frame, edge taps, N·L accumulation, PDF/LOD, output mip dimensions, and DDS layout. Recompute/check hashes and compare production candidate bytes with assignment 30. Confirm no shader, renderer, viewer, skybox/irradiance, or canonical asset changes were introduced. State residual mismatch and whether current visual parity is sufficient to proceed with canonical DDS integration; distinguish an engineering review from user sign-off.

## Deliverable

Record a focused review with pass/issues, exact evidence and hashes, any required corrections, and recommendation for the coordinator's next step. Do not edit files or commit.

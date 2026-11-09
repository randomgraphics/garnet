# Assignment: Isolate GGX sample sequence and count

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 15-sample-sequence-count-experiment
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; exact control settings unresolved; handoff to assignment 16
- Expected first checkpoint: report resolved upstream/Garnet sampling defaults and candidate-generation progress within 10 minutes; provide candidate/render handoff within 25 minutes
- Verify: run controlled GLB comparison with only sample sequence/count changed from assignment 10 candidate, and report hashes, command, exit status, image metrics, and visual result
- Commit: none; experimental candidate only, canonical DDS untouched

## Outcome

Test whether the remaining render difference is caused by Garnet's GGX sample sequence or sample count rather than source cubemap preparation.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `09-sampler-generator-difference.md`
- `10-cubemap-source-mips.md`
- Current `src/tool/texture-toolbox/gen-ibl-maps.py`
- Assignment 10 candidate DDS and controlled GLB render procedure

## Scope and procedure

First inspect the pinned Khronos sampler's invocation/settings and Garnet's current generator defaults. Preserve assignment 10's N·L weighting, PDF LOD, source cubemap mip chain/sampling, output roughness levels, and all render inputs. Change only the sequence and/or count needed for one controlled candidate; prefer matching the upstream sequence and count when they can be established. Write outputs under task evidence with clear names; never replace canonical `prefiltered.dds`. Run the same isolated GLB A/B capture and compare against the exact control capture. Record per-mip timing and any timeout/stall.

If exact upstream settings cannot be established, stop before running a multi-parameter sweep and report the specific missing evidence plus one recommended bounded probe. Do not change shader or rendering logic.

## Deliverable

Record upstream/Garnet settings, exact one-variable change, source/candidate/capture hashes, viewer command and exit status, image metrics, visual assessment, and whether this factor materially closes the gap. Report at the 10-minute checkpoint even if the full run is still in progress.


## Implementer findings
Status: blocked pending sampler control provenance; no generator or rendering code changed and no candidate/render was run. The pinned Khronos sampler report establishes Hammersley points, default 1024 samples, and zero LOD bias. However, the report explicitly says the checked-in `prefiltered-ggx.dds` has no recorded generation invocation/conversion provenance, may have been emitted by a wrapper or another version, and its actual sample count and LOD bias cannot be inferred from the DDS. Thus the control cannot be exactly sequence/count matched from available evidence.
Recommended bounded next probe: preserve assignment 10?s 512 samples and all cubemap, PDF-LOD, NdotL, output mip and viewer settings; change only the seeded random point sequence to the reported Hammersley sequence, stage one DDS, and perform the same GLB comparison. This isolates sequence and does not claim to reproduce the artifact?s unknown settings. For an exact upstream match, obtain the checked-in DDS generation command or wrapper/version and its actual sample-count and bias arguments.

## Handoff

Pinned sampler defaults are Hammersley, 1,024 samples, and zero LOD bias; the checked-in control DDS actual invocation is unknown, so exact artifact matching is not justified from current evidence. Manager selected a Hammersley-only sensitivity candidate at the existing 512 samples as assignment 16. No code or assets were changed in this assignment.

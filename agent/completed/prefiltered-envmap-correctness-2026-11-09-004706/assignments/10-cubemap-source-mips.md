# Assignment: Replace equirectangular LOD approximation with source cubemap mips

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 10-cubemap-source-mips
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; investigation findings retained as historical context
- Requirement revision: R9
- Owner: generator experiment worker
- Dependencies: 08-khronos-sampler-algorithm, 09-sampler-generator-difference, 07-source-lod-on-weighted

## Outcome
Test whether generating and sampling the same mipmapped source cubemap shape as the sampler closes the remaining GLB difference after NdotL weighting and PDF-derived source LOD.

## Scope and boundaries
Before coding, call Serena `initial_instructions`. Edit only `src/tool/texture-toolbox/gen-ibl-maps.py` plus this assignment and isolated candidate/evidence. Preserve NdotL weighting and current LOD equation; change only source preparation/sampling from equirectangular mip approximation to a cubemap mip chain. Hold sample sequence/count fixed. Do not edit shader, renderer, viewer, source environment assets, or canonical DDS until the A/B shows the candidate is accepted. Preserve unrelated/submodule state.

## Assignment context
Read `../TASK.md`, this playbook, assignments 08 and 09, and assignment 07 results. Khronos converts the panorama to a 1024-face float cubemap, generates cubemap mips, then samples the cube at PDF-derived LOD. The current Garnet experiment uses equirectangular mips with cube LOD + 2, which materially improves the render but remains different. Independent comparison ranks cubemap preparation/mip sampling as the leading residual difference. The exact CLI invocation for the checked-in control DDS remains unknown.

## Procedure
1. Read pinned Khronos conversion/cubemap mip source in assignment 08 and reconcile face axes/orientation, source width, mip filtering, and textureLod behavior with Garnet helpers.
2. Build a deterministic source cubemap at 1024 faces from the panorama, generate its mip chain, and sample bilinearly/trilinearly by direction and the existing computed PDF LOD. Document approximations, particularly seamless cross-face sampling.
3. Keep NdotL weighting, GGX samples/count, output roughness mips, and all other algorithm choices unchanged.
4. Generate an isolated candidate; check dimensions, finite values, and DDS layout.
5. Render GLB candidate and compare with exact sampler control and the prior source-LOD candidate; record hashes, commands, metrics, visual appearance.
6. Retain/reject based on evidence. If still different, hand back concrete next experiment; do not stack sample-count or Hammersley changes here.

## Deliverable and acceptance
Focused generator diff and reproducible candidate comparison evidence. Explain how cube mip source and sampling emulate the pinned sampler, report residual mismatch, and recommend next step. Canonical DDS remains untouched unless the manager accepts final integration.

## Progress and handoff
Resumed after user confirmed to continue. Manager independently verified base mip is close in R/B while higher project mips are much darker; record these metrics in the GLB candidate report. Next: implement the isolated real source-cubemap mip experiment.


## Performance checkpoint
The first full candidate attempt was stopped after about 17 minutes wall time. Process CPU time had advanced to 817.75 seconds, but no per-mip checkpoint or staged DDS was produced; stdout was buffered. The canonical DDS was untouched. The generator now batches output normals and emits flushed per-face mip checkpoints. A bounded scalar-equivalence and performance check is pending. Assignment 12 recommends 8x8 tiles, precomputed per-mip LOD plans, 2-4 workers, tiny synthetic equivalence and one-mip time/RSS measurement; use these recommendations before another full candidate.
The initial bounded synthetic check ran one zero-roughness mip (6 faces, 4x4, 16 samples) in 0.0183 s; output matched direct cube sampling within 2.4e-7 max absolute error. A separate copied scalar-reference harness disagreed and is not accepted as evidence until its mismatch is diagnosed. Assignment 12 recommendations received; full generation remains paused pending resolving equivalence and planned LOD grouping.


## Candidate result
Generated staged candidate SHA-256 `65E7EDFCF6F883751CD679FEC909DE94F7B26A3098E3998D83EC6817C58F3DF5`; GLB capture SHA-256 `76389B79BFCFC3A52D7783B2767F9804D93A0EAF62C4A4A2442B50C9AFAF5490`. Generation completed with per-mip elapsed times 70.0s, 25.2s, 6.7s, 1.5s, and 0.3s. Viewer exited 0 using the same isolated stage setup and `female_head_1.glb`; only staged `prefiltered.dds` was replaced. The capture is 1280x720. Against sampler-control capture, global RGB mean absolute error is (0.4724, 0.3009, 0.0663), RMS (2.6176, 1.6889, 0.3610); character region (x=460..899, y=169..638) mean absolute error is (2.1052, 1.3408, 0.2957), RMS (5.5258, 3.5654, 0.7622). The preceding equirectangular-source-LOD candidate had global mean absolute error (0.5127, 0.3591, 0.1206), RMS (2.7468, 1.9218, 0.5461), so this cubemap-source candidate improves the measured difference but remains non-identical. Visual inspection shows a close match with residual visible shading differences. Candidate remains isolated; canonical DDS untouched. Continue with a separate generator-only hypothesis.

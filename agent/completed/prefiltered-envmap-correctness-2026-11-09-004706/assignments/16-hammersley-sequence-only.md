# Assignment: Measure Hammersley sequence sensitivity

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 16-hammersley-sequence-only
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; evidence reviewed; no match
- Expected first checkpoint: candidate generation status within 10 minutes; final controlled render handoff within 25 minutes
- Verify: controlled GLB capture with only sequence changed from assignment 10; include hashes, command, exit status, image metrics, and visual result
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Measure whether switching Garnet's current seeded random GGX points to the pinned sampler's Hammersley sequence materially reduces the remaining render difference.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `10-cubemap-source-mips.md`
- `15-sample-sequence-count-experiment.md`
- Current `src/tool/texture-toolbox/gen-ibl-maps.py`
- Assignment 10 candidate DDS and the exact controlled GLB comparison procedure

## Scope and procedure

The control DDS provenance does not establish its actual sample count or LOD bias. Treat this as a sequence-sensitivity experiment, not exact artifact reproduction. Preserve assignment 10's 512 samples, N·L weighting, PDF-derived LOD, source cubemap mip chain/sampling, output roughness levels, and all viewer inputs. Change only the sample point sequence to Hammersley. Generate and render an evidence-only candidate; never replace canonical `prefiltered.dds`. Report per-mip generation timing and query the manager if the expected checkpoint is missed.

## Deliverable

Record source/candidate/capture hashes, exact sequence implementation, viewer invocation and exit status, image metrics, visual assessment, and whether this isolated change closes the gap. Do not change shader/rendering logic or run a sample-count sweep.


## Results
Classification: sequence-sensitivity experiment, not exact sampler reproduction. The evidence-only generator `evidence/gen-ibl-maps-hammersley-sequence-probe.py` differs from the assignment 10 generator only where `xi1, xi2` are created: seeded uniform RNG was replaced by existing `_hammersley_samples(512)`, which implements `(i/N, radicalInverse_VdC(i))`. Sample count remains 512; all N?L weights, PDF source LOD, cubemap source mip construction/sampling, roughness levels, output format and viewer inputs remain assignment 10 settings.
Generation command used Python importlib to load the evidence-only script, `load_hdr` on `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, `generate_prefiltered(pano, 128, 512, 4)`, and `write_dds_cubemap(..., 'rgba16f')`. Per-mip times: 36.9s, 13.1s, 3.2s, 0.7s, 0.1s. Candidate DDS SHA-256: `6DB958EF915BFA6CAABA2E65C824890839ECBD451BFE40BB7953315774E7D303`.
For GLB render, cloned the validated isolated stage to `%LOCALAPPDATA%\Temp\garnet-ibl-hammersley-candidate`, replaced only staged `prefiltered.dds` (staged hash matches candidate), set `GARNET_ROOT` to that stage, then ran `build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-cubemap-mips-hammersley-512.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>` from the stage. Exit status 0. Capture SHA-256: `6D6F8FBF1F0E0C51797DC4D0C44845D81E21B0A700393DC8B4C4D21A54150400`.
Against the verified sampler-control capture, full-frame RGB MAE is `(0.4501, 0.2855, 0.0545)` and RMS `(2.5209, 1.6406, 0.3295)`; character region x=460..899, y=169..638 MAE `(2.0057, 1.2721, 0.2429)` and RMS `(5.3217, 3.4634, 0.6956)`. Assignment 10 random-512 candidate had full-frame MAE `(0.4724, 0.3009, 0.0663)` and RMS `(2.6176, 1.6889, 0.3610)`. Hammersley at 512 modestly reduces error but remains visibly non-identical, so sequence alone does not close the gap. Canonical DDS untouched.

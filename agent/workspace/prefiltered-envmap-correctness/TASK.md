# Task: Correct prefiltered environment map generation

## Overview

- Branch/worktree: `wip/model-viewer-lighting`; checkpoint is committed in git history. Preserve earlier branch commits and unrelated edits.
- Status: paused at user's request; resume from the cubemap-source-mip experiment.
- Completed: valid GLB A/B, DDS audit, independent sampler report and comparison, two generator experiments.
- Current step: assignment 10, implement real source-cubemap mip preparation/sampling, interrupted before changes.
- Remaining: continue controlled experiments; accept the smallest correct generator fix and generated DDS; independently review and rerender.
- Latest verification: baseline GLB mismatch confirmed. NdotL-only candidate modestly improved. NdotL + equirectangular source-LOD candidate substantially improved but does not match control. `py_compile`, isolated candidate generation and viewer renders succeeded. Canonical DDS untouched.
- Clarification scan: no open question; user specified sampler DDS and `female_head_1.glb`.

Pause checkpoint: exact control DDS was copied to `assets/gltf-ibl-sampler-prefiltered-ggx.dds` for task-local reproducibility. Its SHA-256 matches the asset-tree source: `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`; size 1,047,700 bytes. Original submodule file remains in place. User explicitly requested committing this checkpoint; commit only the task workspace and generator experiment, excluding the unrelated submodule pointer change.

## Goal and constraints

Use `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds` as control and render `media/asset-foundry/model/misc/female_head_1.glb`. Compare against project `media/asset-foundry/image/envmap/qwantani_sunset_puresky/prefiltered.dds`, changing only prefiltered map. Diagnose DDS/generator differences, fix and repeat until renders match.

Final product changes may affect only prefiltered texture content and/or `src/tool/texture-toolbox/gen-ibl-maps.py`. Final shader/rendering logic must match baseline. Experimental generator code may remain in the current working tree as a checkpoint; canonical DDS is untouched. Preserve the modified `media/asset-foundry` submodule. Do not commit or push further work unless separately requested.

## Management workflow

The top-level management agent owns the graph, worker packets, dependencies, evidence, and next-step decisions. It keeps its context concise and delegates technical analysis, implementation, and review to bounded agents. Each worker reads this charter plus its own assignment, one role playbook, and linked evidence. Kickoff includes review of existing state and potential human clarification needs.

## Confirmed evidence

- Exact sampler source and staged DDS SHA-256: `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`.
- Project source and staged DDS SHA-256: `3C3E0B0B7528160DD0B5F5743BB164E2FFDD05691E8E8CC5ADDAC43865DDC51E`.
- Same GLB, viewer, stage, and non-prefiltered resources; viewer logs prefiltered-map load and PBR samples it. Baseline output differs; project map is glossier.
- DDS headers/layout match (128x128 RGBA16F, 6 faces, 5 mips); payload differs most at rougher mips.
- Independent Khronos-only report is in assignment 08; separate comparison is assignment 09. Baseline Garnet omitted NdotL weighting and PDF-derived source LOD. Khronos prepares a 1024-face cubemap and mip chain.
- NdotL-only experiment improved RGB MAE from `[0.8931,0.5669,0.1591]` to `[0.8662,0.5171,0.0837]`; still too glossy.
- NdotL + equirectangular-mip LOD approximation improved MAE to `[0.5127,0.3591,0.1206]` and RMS to `[2.7468,1.9218,0.5461]`; still not equal. Candidate hash `D93331D690117A42685D8B6A767AEB22B29DB07EA98E741B39427CAF876299F9`.
- Assignment 10's next hypothesis: build/sample a real source cubemap mip chain instead of approximating it with equirectangular mips. Hold NdotL weighting, source LOD equation, and sample sequence/count fixed.

## Resume instructions

1. Read this file, `assignments/manager.md`, `assignments/10-cubemap-source-mips.md`, and the completed assignments 08/09/07.
2. Confirm the current generator diff still contains only the NdotL and equirectangular source-LOD experiments; do not overwrite canonical `prefiltered.dds` without a successful comparison.
3. Dispatch assignment 10 to an implementer. Require Serena `initial_instructions` before coding.
4. Compare candidate against the exact sampler control using `female_head_1.glb`; then route next experiment or accepted fix/review.

## PROGRESS

Last completed: independent sampler/generator reports and source-LOD experiment.
In progress: paused by explicit user request.
Verification: project/control mismatch remains; no accepted final fix or canonical DDS update yet.

## TASK BREAKDOWN

Phase 0: valid GLB control and comparison — complete (assignments 01/02).
Phase 1: DDS and algorithm diagnosis — complete (assignments 03/04, 08/09).
Phase 2: isolated generator experiments — NdotL weighting and equirectangular LOD complete but insufficient (assignments 06/07); actual cubemap source mips pending (assignment 10).
Phase 3: integrate accepted generator/map fix, independent review, and repeat comparison — pending.

## References

- Harness coordination: `agent/harness/ASEH_DESIGN.harness.md`
- Task contract: `agent/harness/task-workspace.harness.md`
- Viewer: `src/tool/mesh-viewer/`
- Generator: `src/tool/texture-toolbox/gen-ibl-maps.py`
- Environment: `media/asset-foundry/image/envmap/qwantani_sunset_puresky/`
- Model: `media/asset-foundry/model/misc/female_head_1.glb`

## Paused asset publication checkpoint

- The sampler `.dds` control is retained at `assets/gltf-ibl-sampler-prefiltered-ggx.dds` with SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`.
- Pending asset-folder changes were committed on submodule branch `wip/envmap-assets` at `4a82892` and pushed to its Hugging Face remote.
- User requested removal of both `.ktx2` files; neither is in the published submodule commit. A local backup branch `backup/envmap-assets-pre-ktx2-removal` retains the pre-removal unpublished state only.
- Parent repo still needs its submodule pointer updated to `4a82892` and then pushed. Task remains paused; next engineering task is assignment 10.

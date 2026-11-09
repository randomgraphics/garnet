# Task: Correct environment-map prefilter generation

## Status

Complete and signed off by the user. The user confirmed the PBR lighting looks correct and requested final cleanup, archival, commit, and push. Assignments 43?51 are complete; assignment 51 regeneration received independent review.

## Scope and final result

The generator at `src/tool/texture-toolbox/gen-ibl-maps.py` now uses texel-center coordinates for cubemap source projection and output normals. The controlled female-head GLB comparison against the glTF sampler control reached 96.44% exactly matching RGB pixels, with maximum channel difference 3. The Qwantani `prefiltered.dds` SHA-256 is `14BCB2032B82475CF7A7730356FF1143BD9CD49EB6ECBF908699BA1343C1E4B0`.

All four DDS textures were regenerated for all three asset-foundry environment maps with the updated script. Skybox and irradiance outputs contain complete mip chains. Assignment 51 records layouts, hashes, and validation. The user-deleted sampler assets remain deleted; the verified control DDS copy remains in this task directory.

No shader or renderer workaround was retained. The final product change is generator logic and generated texture content.

## Cleanup and verification

Generated screenshots, candidate DDS files, logs, scripts, caches, and temporary staging were removed from the task workspace. Only task records and the control DDS remain. Python compilation, Black formatting, generated DDS validation, render comparison, and diff checks passed. Final code formatting, spelling, and documentation checks are being completed before publication.

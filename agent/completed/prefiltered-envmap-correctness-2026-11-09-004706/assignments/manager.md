# Assignment: Manage environment-map texture correction

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Role: Coordinator/router (`../../../harness/roles/coordinator-router.harness.md`)
- Status: complete; user signed off
- Owner: top-level management agent
- Commit: user requested final commit and push

## Final handoff

The generator fix uses texel-center cubemap coordinates for both source projection and output normals. The controlled female-head GLB render matches the sampler control exactly on 96.44% of RGB pixels, with maximum channel error 3. The updated script regenerated skybox, irradiance, prefiltered, and BRDF LUT DDS files for all three asset-foundry environment maps. All 12 outputs passed header, format, dimension, payload-size, finite-value, and hash checks. Independent review passed.

Task-local evidence images and intermediate files were removed. The verified sampler-control DDS copy remains. User-deleted sampler files remain deleted. User authorized archival, commit, and push after cleanup.

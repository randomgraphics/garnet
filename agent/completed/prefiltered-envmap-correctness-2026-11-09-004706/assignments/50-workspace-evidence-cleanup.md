# Assignment: Clean task workspace intermediates

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `50-workspace-evidence-cleanup`
- Role: Coordinator/router (`../../../harness/roles/coordinator-router.harness.md`)
- Status: complete
- Commit: none

## Intake review

Reviewed the task/workspace contract, task and manager files, assignments, evidence inventory, and Git status. No clarification was needed: retain durable reports and the control input; remove reproducible evidence outputs. The user deleted glTF sampler files from asset-foundry; those deletions were preserved.

## Result

Removed the entire task `evidence/` directory after capturing final/control render hashes and metrics in assignments 46 and 49. Removed all 29 PNG snapshots, 28 candidate DDS files, probe scripts, raw logs/comparison data, and Python caches (about 63 MiB). No PNG remains in the workspace. Kept all assignment reports and `assets/gltf-ibl-sampler-prefiltered-ggx.dds` (SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`) because its asset-foundry original was deleted. The user's sampler deletions remain in place; the canonical prefiltered DDS remains modified. No commit or push was made.

# Assignment: Manage prefiltered-map correction workflow

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: manager
- Role: Coordinator/router (`../../../harness/roles/coordinator-router.harness.md`)
- Status: paused by user; checkpoint being committed
- Requirement revision: R9
- Owner: top-level management agent
- Dependencies: none

## Outcome
Resume the task by coordinating bounded workers until the verified GLB comparison matches and the final product scope passes review.

## Scope and boundaries
Own task-wide graph/status, assignment routing, dependencies, evidence reconciliation, clarification scan, and final scope audit. Keep this context concise and coordination-focused; technical analysis and implementation belong to workers. Preserve unrelated user changes and the asset-foundry submodule. Do not commit/push additional work unless requested.

## Assignment context
Read `../TASK.md`, this playbook, current `git status`, and only the latest relevant handoff. The task is explicitly paused. Next node is `10-cubemap-source-mips.md`; the worker had not begun changes when interrupted. Current generator has experimental NdotL weighting plus equirectangular LOD; canonical DDS remains unchanged. User requested committing a checkpoint of context and status.

## Procedure
On resume: verify committed checkpoint/worktree state; dispatch task 10 to one implementer; reconcile result; choose next isolated experiment or accepted fix; dispatch independent review; rerun GLB A/B; update task status/evidence. Keep every worker packet limited to one outcome, one playbook, and linked evidence.

## Deliverable and acceptance
Current graph/status, concise worker packets, evidence-linked decisions, final review and comparison report. Do not mark complete until correct GLB renders match and final changes are only prefiltered map content/generator.

## Progress and handoff
Paused at user's explicit request. Active worker interrupted. Control DDS copy is in `assets/`; exact hash recorded in `TASK.md`. Generator currently contains NdotL weighting plus equirectangular source-LOD experiment; canonical map is untouched. Checkpoint commit includes task workspace and generator diff only, not asset-foundry submodule. Resume at assignment 10.

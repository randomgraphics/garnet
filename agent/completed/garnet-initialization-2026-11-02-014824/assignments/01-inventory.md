# Assignment: Inventory repository boundaries and build system

- Task: Garnet repository initialization and knowledge/skill refresh (`../TASK.md`)
- Assignment: 01-inventory
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete
- Requirement revision: 1
- Owner: current session
- Dependencies: none

## Outcome
Record the repository boundary, first-party directory inventory, build/test/CI entrypoints, platform variants, and excluded/generated areas with direct source evidence.

## Scope and boundaries
Inspect top-level files, CMake composition, scripts, CI, public include roots, module roots, test/sample/tool roots, and submodule metadata. Do not edit source or touch dirty submodules.

## Assignment context
Follow `agent/harness/initialization.harness.md`; preserve human edits and distinguish observation from inference.

## Procedure
1. Capture Git branch/status and top-level inventory.
2. Trace root CMake into source directories and test discovery.
3. Inspect CI matrix and build/CIT wrappers.
4. Record first-party/external/generated boundaries and platform coverage.

## Deliverable and acceptance
Inventory findings in the task knowledge coverage matrix with paths/sections as evidence; all top-level source roots accounted for.

## Progress and handoff
Repository identity confirmed as `agent-harness`; source/build/test/CI/platform roots and exclusions are recorded in `agent/knowledge/coverage.md` and `agent/knowledge/build-and-platforms.md`. No submodule content was changed.

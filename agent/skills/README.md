# Agent Skills

This directory contains reusable Garnet-specific skills shared by the harness
and AI coding agents. Agents are the primary maintainers; humans may also add,
edit, reorganize, or remove skills. Each skill is a self-contained folder with
a `SKILL.md` entrypoint and frontmatter describing when to use it.

The root `AGENTS.md` provides project-wide guidance. The generic task contract
is in `agent/harness/README.md`; this repository's path mapping is in
`agent/README.md`.

## Planning and task management

- `garnet-feature-workflow`: lifecycle for feature work.
- `garnet-execution-plan`: resumable plans, progress tracking, and stepwise execution.
- `garnet-assignment-tracking`: maintain task records in `agent/workspace/<task>/`.
- `garnet-agent-doc-archive`: archive signed-off records under `agent/completed/<task>-<date-time>/`.
- `garnet-git-workflow`: branches, commits, and repository workflow.

## Implementation guidance

- `garnet-public-interface`: public API boundaries and implementation-only declarations.
- `garnet-cpp-style`: C++ coding style and local conventions.
- `garnet-rdg-gpu2`: RDG and gpu2 module guidance.
- `garnet-submodules`: git submodule rules.

## Build and verification

- `garnet-build-test`: build, test, lint, and format through project wrappers.
- `garnet-code-coverage`: coverage builds and function-level reports.
- `garnet-android-docker`: Android build verification via Docker.
- `garnet-vulkan-verification`: Vulkan app, sample, and test verification under validation layers.
- `garnet-line-endings`: match new files to the hosting OS line endings.

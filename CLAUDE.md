# Claude Code Guidance for Garnet

Read `AGENTS.md` first. It is the primary repo guide: project layout,
build/test/format commands, coding rules, git workflow, and RDG/gpu2 module
conventions all live there.

## Skills

Repo-local skill notes live under `agent/skills/`. Each `<skill>/SKILL.md` has
frontmatter whose `description` states when it applies. Before starting a task
that matches one, read that SKILL.md and follow it:

- `garnet-feature-workflow` — multi-step feature work, especially RDG/gpu2.
- `garnet-build-test` — compiling, running CIT, formatting, verifying changes.
- `garnet-assignment-tracking` — creating/updating assignment files in
  `agent/` for resumable multi-step work.
- `garnet-agent-doc-archive` — archiving finished agent docs into
  `agent/completed/`; also where to look to learn how existing code came to be.
- `garnet-cpp-style` — editing C/C++ files, naming, comments, hot paths.
- `garnet-rdg-gpu2` — editing `src/core/rdg*`, `src/inc/garnet/rdg*`,
  `src/core/gpu2`, or `src/inc/garnet/gpu2`.
- `garnet-git-workflow` — committing, pushing, branching, task-step commits.
- `garnet-submodules` — adding, updating, or reviewing git submodules.
- `garnet-android-docker` — Android build verification via Docker.

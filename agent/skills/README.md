# Agent Skills

This directory contains repo-local skill notes shared by all AI coding agents
(Claude Code, Codex, etc.). They were originally converted from the repo's
consumed Cursor skills/rules and Claude rule files.

The root `AGENTS.md` is the primary guidance file. These skills are focused
references for common workflows. Each `<skill>/SKILL.md` starts with
frontmatter whose `description` says when to use it; read the matching skill
before starting a task it covers.

- `garnet-feature-workflow`: top-level lifecycle for feature work.
- `garnet-execution-plan`: detailed, resumable plans with an Overview, dedicated workspace, progress tracking, and stepwise execution.
- `garnet-build-test`: build, test, lint, and format through project wrappers.
- `garnet-assignment-tracking`: create and maintain assignment files in `agent/`.
- `garnet-agent-doc-archive`: archive finished agent docs into `agent/completed/`.
- `garnet-cpp-style`: C++ coding style and local conventions.
- `garnet-line-endings`: new files match the hosting OS (CRLF on Windows, LF on Linux).
- `garnet-rdg-gpu2`: RDG and gpu2 module guidance.
- `garnet-git-workflow`: git workflow for assignments, branches, and commits.
- `garnet-submodules`: git submodule rules.
- `garnet-android-docker`: Android build verification via Docker.

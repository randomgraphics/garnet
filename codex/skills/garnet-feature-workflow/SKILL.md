---
name: garnet-feature-workflow
description: General Garnet feature development workflow. Use when starting, planning, implementing, verifying, committing, merging, or finishing a multi-step feature in this repo, especially RDG/gpu2 work.
---

# Garnet Feature Workflow

Use this as the top-level workflow for feature work. Pull in the focused skills
as needed:

- `garnet-assignment-tracking` for multi-step or resumable work.
- `garnet-git-workflow` for branches, commits, pushes, and user edits.
- `garnet-build-test` for build, test, lint, and formatting commands.
- `garnet-rdg-gpu2` for RDG/gpu2 implementation rules.

## Start

- Inspect `git status` before changing files.
- Identify the base branch. RDG/gpu2 feature work starts from
  `feature/rdg/main` unless the user explicitly says otherwise.
- Create a work branch for substantial RDG/gpu2 work:
  `wip/gpu2/<topic>` or `wip/rdg2/<topic>`.
- For complex or resumable work, create or update
  `agent/ASSIGNMENT_<SCREAMING_SNAKE>.txt` before implementation.

## Plan

- Split the feature into small steps with concrete verification.
- Prefer steps that can be built, tested, sampled, or manually checked.
- Keep public API, implementation, samples, tests, and docs as separate
  concerns unless one small change naturally touches several of them.
- Record known gaps explicitly instead of hiding them in broad task wording.

## Implement

- Work with existing user/manual changes; do not revert them unless requested.
- Keep changes local to the feature and follow existing module patterns.
- Use repo-local conventions from `AGENTS.md` and the focused skills.
- Update call sites and tests in the same feature step when changing public
  interfaces.

## Verify

- Run the narrowest useful check first, then broader checks when risk warrants.
- Use project wrappers such as `env/bin/build.py`, `env/bin/cit.py`, and
  `env/bin/format-all-sources.py`; do not use raw CMake for normal work.
- If a check cannot run because of environment limits, record the exact command
  and failure mode in the handoff.

## Commit

- Commit only after verification for that logical step.
- Include all files that belong to the logical change, including user edits that
  are part of the same feature.
- Prefer one verified feature step per commit for assignment-style work.
- Push when the assignment or user request requires shared progress.

## Finish

- Re-check `git status`, branch, and recent log before final handoff.
- Summarize the feature result, verification, and remaining risks.
- When the user asks to merge accumulated work, squash merge to the target
  staging branch, push it, then delete completed work branches locally and
  remotely if requested.

---
name: garnet-git-workflow
description: Garnet git workflow for commits, PRs to master or staging branches, merges, and branch cleanup. Use when committing, pushing, preparing PRs, merging, or finishing merged work.
---

# Garnet Git Workflow

## User Edits

- Never revert manual/user changes unless the user explicitly asks.
- Before committing, run `git status`.
- Include all files that are part of the logical task, including user edits that
  belong with the change.

## Bug Fix Sessions

For ad-hoc debugging or bug fixes, do not commit or push unless the user asks.
Apply the fix, verify what you can, then summarize changed files and checks.

## Assignment Work

- Break work into verifiable steps.
- Prefer one logical task/step per commit.
- Verify before committing.
- Push after commits when the assignment workflow requires visible shared
  progress.
- Keep the assignment file's `PROGRESS` block in `agent/` synchronized.

## RDG/gpu2 Branches

`feature/rdg/main` is the staging branch for RDG/gpu2 development.

- Feature branches: `wip/gpu2/<topic>` or `wip/rdg2/<topic>` from
  `feature/rdg/main`.
- Small fixes may go directly to `feature/rdg/main` when appropriate.
- Do not branch RDG/gpu2 work from `master` unless explicitly directed.

## Pull Requests to Master

- When asked to submit work to `master`, verify and commit the logical change,
  push its source branch, and open or update a PR with `master` as the base.
  Check the final diff and describe the resulting behavior and verification.
- Respect required checks and reviews. Creating a PR does not itself authorize
  merging it; merge when requested or required by the assignment workflow.
- By default, ensure GitHub's **Automatically delete head branches** repository
  setting is enabled so GitHub deletes the PR source branch after a successful
  merge. This is a repository-wide setting, not a per-PR auto-merge option.
  Check that long-lived branches are protected against deletion before enabling
  it. If permissions prevent enabling it or verifying protection, report the
  limitation rather than claiming automatic cleanup is configured.
- Preserve long-lived branches such as `master`, `main`, and `feature/rdg/main`
  locally and remotely, even when they are the source of a merged PR. GitHub
  branch protection/rulesets should prevent their deletion; never bypass or
  weaken those protections for cleanup.

## Post-Merge Cleanup

These defaults apply to PRs targeting both `master` and staging branches.
Cleanup of a completed short-lived PR branch is part of the authorized merge
workflow and does not need a separate deletion request.

- Confirm the PR actually merged successfully before deleting branches; queued
  auto-merge or a closed, unmerged PR is not sufficient.
- Verify GitHub removed the remote source branch. If automatic deletion did not
  occur, delete it only after confirming it is short-lived, has no commits added
  since the merged PR head, and is not protected against deletion.
- Delete the local source branch by default only when it has no unmerged or
  unpushed work beyond the merged PR head and its worktree is completely clean:
  no staged, unstaged, untracked, or dirty-submodule changes. Inspect linked
  worktrees too; never discard changes or remove a worktree to force cleanup.
- Switch a clean checkout off the source branch before deleting it. Prefer
  `git branch -d`; after a squash/rebase merge, ancestry alone may not establish
  that the work merged. Use `-D` only after verifying the successful PR merge
  and that the local tip matches the merged PR head with no additional work.
- If cleanup cannot safely complete, retain the branch and report why. Always
  retain long-lived branches regardless of clean status or merge success.

GitHub reference: [Managing automatic branch deletion](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/configuring-pull-request-merges/managing-the-automatic-deletion-of-branches).


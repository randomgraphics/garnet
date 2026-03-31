---
name: interactive-bug-fix-session
description: Runs bug fixes and debugging as a human-in-the-loop session. Does not auto-commit or push; the human reviews diffs and decides when to commit, amend, or discard. Use when the user asks for interactive debugging, a bug fix session, human review before commit, or explicitly says not to commit yet.
---

# Interactive bug fix session (human involved)

## Default behavior

During an **interactive bug fix** or **debugging** session, treat the human as the owner of git history.

- **Do not** `git commit` or `git push` bug fixes unless the user **explicitly** asks (e.g. "commit this", "push the fix").
- **Do** apply code edits, explain the cause and fix, and show what changed (paths + short summary or diff highlights).
- **Do** end with a clear handoff: what was fixed, how to verify, and that the user can review, test, then commit when ready.

## When this applies

Use this workflow when the user signals:

- Interactive debugging, bug hunt, or "fix X" without asking for a commit
- "Don't commit yet", "let me review", "human decides", or similar
- A **session** focused on diagnosis and patch, not assignment-style "step done → commit"

If the user later says to commit, follow their instructions (message, scope, branch).

## Relationship to other rules

- **Assignment / task workflows** (e.g. commit-per-step for assignments) apply when the user is executing a structured assignment, not by default to ad-hoc bug fixes.
- **`do-not-revert-manual-changes`**: When the user *does* ask for a commit, still include their manual edits in that commit; do not revert them.

## Checklist for the agent

- [ ] Fix or investigate without assuming a commit at the end
- [ ] Summarize root cause + fix + verification steps
- [ ] Mention modified files; offer to show diff if useful
- [ ] **Only** commit/push if the user explicitly requests it

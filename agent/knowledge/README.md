# Garnet Code Knowledge Base

This directory is the version-controlled entrypoint to reusable Garnet codebase
knowledge. Start with [`catalog.md`](catalog.md), then follow its source links
and query the relevant code when a task needs more detail.

Agents are the primary maintainers: they should add or improve reusable,
evidence-backed knowledge as they explore and change the codebase. Human
contributors may also add, edit, move, or remove entries. The knowledge base is
shared project content, not an agent-private cache or read-only generated
artifact.

The knowledge base supports ASEH's code analysis and context builder. It does
not replace the source tree: implementation facts must be checked against the
current worktree, especially after local changes. See
[`../harness/ASEH_DESIGN.harness.md`](../harness/ASEH_DESIGN.harness.md) for the
retrieval, provenance, and freshness design.

## Contents and ownership

- `catalog.md` — concise, curated map of Garnet modules, public interfaces,
  build/test entrypoints, and authoritative docs.
- `ASEH_WORKLOAD_EXAMPLES.md` — Garnet-specific examples of refining ambiguous
  long-horizon engineering requests.
- `*.harness.md` beside source — module-specific explanations, invariants, and
  relationships that are useful to agents working in that code area.
- `*.harness.json` beside source — machine-readable module metadata only when
  a tool needs it; validate it against a schema under `agent/harness/`.
- `.cache/agent-knowledge/` — suggested ignored location for generated symbol,
  reference, dependency, and semantic indexes. Rebuild from the current
  worktree; do not treat cache content as authoritative or commit it by default.

Keep reusable facts and evidence-backed relationships here or beside their
owning code. Keep temporary findings and unverified hypotheses in
`agent/workspace/<task>/`; promote them only after checking their source and
reuse value. Avoid duplicating long source explanations: link to authoritative
docs and code instead.

Before editing knowledge, inspect the Git worktree and the exact target files.
Make focused changes that preserve useful human-authored explanations and
formatting. Do not regenerate curated files wholesale or overwrite concurrent
manual edits. If human and agent edits conflict, retain both pieces of evidence
and reconcile them explicitly. Make deletions reviewable in the normal Git diff;
remove an entry only when it is obsolete, duplicated, or contradicted, and
update its replacement or source links in the same change when applicable.

For curated entries, include enough provenance for another maintainer to
recheck the claim: source path and symbol/section, evidence type, last checked
commit or content version where useful, and whether the entry is confirmed,
provisional, or stale. Humans may override an entry, but changes should remain
traceable through Git history. Derived indexes and caches are managed
separately and must never overwrite curated Markdown or JSON.

## Query contract

An agent query should be answerable by module, symbol, API contract,
implementation, dependency, test, build target, platform, or concept. A useful
result includes:

- a concise answer limited to the requested scope;
- source path plus symbol, heading, or other stable anchor;
- current-worktree or indexed-commit freshness;
- evidence type and confidence, with conflicts or missing coverage called out.

Prefer exact text and symbol/reference search, language-server navigation,
dependency maps, and build/test metadata. Semantic search may help discover
candidates but cannot establish code facts on its own. If no executable query
backend is available, use this catalog and repository search tools directly;
do not imply that an index was consulted when it was not.

When code changes, refresh or invalidate derived entries for changed files and
known dependents. If curated knowledge disagrees with code, trust the current
code for implementation behavior, mark the knowledge stale, and create a
focused knowledge-update task when the correction is reusable. Derived updates
must not overwrite human- or agent-curated files.

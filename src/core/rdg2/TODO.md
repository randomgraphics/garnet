# RDG v2 — Implementation TODOs

## Resource Repo System

Resource registry that can lookup and create resources by certain Key type (such as resource's uri) to eliminate resource redundancy in memory. A working draft was built and later retired to `src/experimental/repo/` (kept as design reference, out of the active build). Revive from there when the RDG needs it.

## Standalone Versioned Artifact

The most useful part of the current open-graph system is the artifact/version
model: an artifact represents a piece of content with a monotonically increasing
version. Successful publish increments the version, failed work does not publish,
and users can either retrieve the latest known-good content immediately or wait
for a specific version to become available.

The first standalone version is implemented with no graph ownership. The base
artifact is untyped and owns the version counter, latest content, and waiters
directly. A separate typed wrapper enforces that publish/read use the same
content type, or a derived type, while still allowing the core artifact to store
`Entity`.

Important initial semantics to preserve:

- Start with one content stream and one monotonically increasing version.
- Reject empty content; rejected publish must not increment the version.
- Loading/creation failure should simply skip publish so the previous
  last-known-good content remains available.
- `wait(version)` should atomically return both wait status and the content
  snapshot that satisfied the wait, because callers usually wait and then read.
- Keep the API shape open enough to allow a future multi-stage model without
  committing to it now.

Possible future extension: split content into beta and stable tracks. Publishing
would create a beta version and wake waiters interested in new candidate content.
Systems could then trial the beta content with richer validation, such as shader
pipeline creation, material compatibility checks, or GPU validation passes. If
the candidate works, promote that version to stable; if it fails, drop it without
disturbing users that continue to rely on the stable content. There is no
immediate practical use case yet, so implement only the single-version model for
now while avoiding design choices that would make beta/stable promotion hard.

## RDG Building Blocks vs Graph Implementations

RDG should not be tied to one specific dependency graph design. The current
open graph is only one possible execution model, not the full boundary of what
the RDG module can provide. Future work should separate reusable building
blocks, such as versioned artifacts, typed artifact wrappers, resource repos,
tokens/wait results, and GPU payload coordination, from concrete graph
implementations.

This should make it possible to experiment with multiple dependency graph
systems in the same RDG module. For example, one graph might stay as a generic
open DAG executor, another might focus on frame-oriented render passes, and
another might be a lightweight artifact/task scheduler for asset streaming or
hot reload. Shared primitives should not assume a single owning graph unless
that ownership is truly part of their semantics.

## Sprite and Text

Migrate Sprite and Text renderer of gpu v1.

## Transient Resource Aliasing

Defer actual GPU buffer/texture allocation to node execution time. A transient
resource is allocated from a pool when the owning node runs, and returned to the
pool when all GpuPayload objects that reference it complete on GPU (ref-count
drops to zero). Each `GpuRaster::seal()` increments the ref-count for every
transient resource it captures; each payload's `onComplete` callback decrements
the count and returns GPU memory to the pool when it reaches zero.

This naturally handles aliasing without a global compile-phase analysis: two
transient resources that are never live at the same time will reuse the same pool
slot simply because one is returned before the other is allocated.

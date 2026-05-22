# RDG v2 — Implementation TODOs

## Resource Repo System

Resource registry that can lookup and create resources by certain Key type (such as resource's uri) to eliminate resource redundancy in memory. The basic system is there already. Need to apply it to the rest of the RDG.

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


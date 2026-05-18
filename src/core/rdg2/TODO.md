# RDG v2 — Implementation TODOs

## Typed artifact content
using std::any as part of public interface is not stable across DLL boundary. Consider using runtime type as the content.

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

## Raster DrawBundle and Payload optimization.

- Create a new class DrawBundle that is to record a series of draws for certain GpuRaster class. Multiple draw bundles
  can record draw calls in parallel. Once seal is called, information of all draw bundles are connected/packed together
  to become the final payload od the GpuRaster class.
- Refactor code to move most of the heavylifting logic from Payload's record-for-submit method back to draw() and seal(),
  the goal is to keep payload's record-for-submit as lightweight as possible.

## Shader Hot Reload

Add `GpuShader::reload()` for live shader reloading:

- Valid only for shaders created via `GpuShader::load()` (no-op for binary-blob `create()`).
- On success, invalidates all cached PSOs that reference the shader; they are rebuilt
  transparently on the next `GpuRaster::seal()` / submit.
- Must not be called while GPU work referencing the shader is in flight.

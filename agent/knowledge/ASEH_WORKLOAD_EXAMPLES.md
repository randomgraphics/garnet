# Garnet ASEH Workload Examples

These examples apply the generic progressive requirement-refinement process to
Garnet's current codebase. They are initial workload scenarios, not fixed
implementation contracts. Verify all repository details against the current
worktree before using them.

The general process is defined in
[`../harness/requirement-refinement.harness.md`](../harness/requirement-refinement.harness.md).

## Example 0: GPU-driven rendering pipeline

Initial request: "Implement a complete GPU-driven rendering pipeline,
including culling, indirect rendering, render-graph integration, resource
management, synchronization, cross-platform backends, and tests."

The harness treats "complete" and "cross-platform" as scope signals to
investigate, not an automatic requirement to implement every backend at once.
It maps current rendering, GPU, and resource APIs; determines which backends
and hardware paths exist; and identifies shared contracts and integration
boundaries. It separates a demonstrable vertical slice from later feature and
backend expansions, with each slice tied to measurable output and tests.

Likely human decisions include the first target workloads and platforms,
required fallback behavior, and the performance or feature-parity bar. The
harness can meanwhile prototype culling and indirect submission against an
existing backend, measure the result, and revise the dependency graph as
render-graph or synchronization constraints become clearer.

## Example 1: D3D12 backend for gpu2

Initial request: "Implement the gpu2 module based on D3D12, reusing the existing
generic gpu2 headers."

The harness treats header reuse as an intended compatibility goal and first
compares generic gpu2 contracts, the Vulkan implementation, D3D12 stubs, build
rules, tests, and factory dispatch. It creates a capability matrix for each
public interface and identifies semantics that D3D12 cannot provide directly
or that require different synchronization and resource lifetime behavior.

It should not widen or weaken the generic public API just to make the backend
easier to implement. If an existing contract has no sound D3D12 mapping, the
harness records the mismatch and prepares alternatives: preserve the contract
with an emulation cost, define an explicit unsupported result, or propose a
cross-backend API revision. It can implement and validate an isolated device
bring-up while that decision is pending.

Likely human decision: the minimum Windows/D3D12 feature level and whether
unsupported optional capabilities should fail, be emulated, or fall back to
another backend. Build-only validation and hardware-backed validation are
reported separately when suitable GPUs are unavailable.

## Example 2: Efficient generic skeletal animation

Initial request: "Implement a high-efficiency, general skeletal animation
system."

The harness first discovers the engine's asset, mesh, transform, rendering, and
GPU abstractions. It treats "general" and "efficient" as unresolved terms,
not permission to implement every animation feature. It inventories candidate
capabilities such as hierarchical skeletons, keyframe clips, blending, layered
poses, retargeting, and GPU skinning, then proposes a smallest useful vertical
slice.

Before choosing CPU or GPU execution, it builds representative workloads and
measures update time, skinning time, memory, bone counts, and character counts.
It records the hardware and build configuration so results are comparable.
The proposed contract includes explicit targets, for example a maximum p95
animation cost for a stated number of characters and bones on a named device
class; the harness does not invent a performance target and call it user
approved.

Likely human decision: which applications and device classes are the initial
target, and whether retargeting or authoring-tool integration is required in
the first release. Until then, the harness can build a prototype and benchmark
several execution strategies without committing to a public API.

## Example 3: Ray-tracing renderer with a traditional pipeline

Initial request: "Implement a ray-tracing renderer that also works with the
traditional rendering pipeline."

The harness separates possible meanings: a full path tracer, a real-time hybrid
renderer, or selected ray-traced effects alongside rasterization. It examines
RDG2, gpu2, current scene/material data, shader interfaces, and supported
hardware paths. A feature matrix records which rendering modes can run on each
backend and which quality or feature reductions are needed without ray tracing.

It tests whether both modes can consume the same scene and material contracts,
whether they share resource lifetime and frame scheduling, and how the
traditional path behaves when acceleration structures or ray queries are
unavailable. A small effect prototype can compare integration costs before
the harness commits to a renderer-wide architecture.

Likely human decision: whether the desired product is a path tracer or a
real-time hybrid renderer, and what minimum image-quality and performance goals
apply to the non-ray-tracing fallback. The harness can recommend a staged
hybrid demonstrator if it best preserves both paths, but must label that as a
proposal until accepted.

## Example 4: Android version that runs on phones

Initial request: "Make the engine Android-compatible and runnable on phones."

The harness inspects the existing Android build workflow, dependencies,
window/input layers, graphics backends, samples, and CI coverage. It separates
successful compilation from a packaged application that launches, renders,
handles input and lifecycle events, and meets a usable memory and frame-time
budget on real devices.

It drafts a support matrix for Android API level, ABI, graphics API, GPU/device
class, packaging, and validation method. It checks which entries can be covered
by the current build environment, an emulator, or physical devices. A build on
one emulator is not reported as proof of broad phone compatibility.

Likely human decision: minimum OS/API level, required device classes, and
whether the first milestone needs a sample app, a library package, or both. The
harness may continue by compiling a representative target and reporting
environment gaps while the support policy is pending.

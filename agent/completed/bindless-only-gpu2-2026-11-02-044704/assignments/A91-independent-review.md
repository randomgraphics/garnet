# A91: Independent bindless-only migration review

## Role

Reviewer/integrator. Follow `../../../harness/roles/reviewer.harness.md`.

## Objective and boundaries

Independently inspect the current source migration against requirement revision
0.2. Review API/backend removal, active callers, tests and CMake wiring, shader
declarations, descriptor/resource lifetime, heap-free transfer-only CnC, and
preserved shared raster/resource-table contracts. Do not edit implementation
source. Fresh C++ and GPU test execution belongs to A90 and remains pending.

## Progress

- Status: complete for read-only/static review; task remains in progress pending
  A90 fresh build and GPU tests, then user sign-off.
- Source review confirmed active first-party searches for `GpuRaster`,
  `GpuCnC`, `recordBindBasedDraw`, `RasterPsoFactory`, `GpuRasterPayload`, and
  `GpuRasterVulkan2` are clean. The only matches for retired naming are in
  `src/core/gpu2/MIP_LEVEL_RESEARCH.md`, which explicitly labels those notes as
  historical. Shared raster target/geometry/state types, bindless Raster,
  `passResources`, and the heap-backed resource table remain present.
- CnC inspection confirmed the bindless factory accepts a null heap only for
  transfer-only recordings (empty pass-resource declaration); compute paths
  require a heap. Sample/test shader bindings and pass-resource declarations
  were checked against their C++ set/index usage.
- ImGui inspection confirmed font and registered textures/samplers allocate
  heap indices, runtime shader descriptor set/binding decorations are remapped,
  push-constant layout matches the C++ structure, and recorded draw payloads
  retain descriptor leases through completion/discard. The regression test
  covers font and registered texture with a non-default heap set index.
- CMake/source check confirmed renamed CnC/Raster tests are present in the
  GPU2 test fragment, generated shader header dependencies are wired, and
  removed old shaders are not left in active wiring. Legacy FX2 SSC callers
  and tests are removed; bindless SSC caller/test paths remain.
- Checks run: `git diff --check --ignore-submodules=all` passed;
  `python env\bin\format-all-sources.py -dqn` passed; targeted `rg` and source/
  CMake inspections completed. No full build or GPU tests were run in this
  assignment. A90 records that the Windows MSBuild wrapper stalled at
  `Checking File Globs`; this is a validation limitation, not evidence of a
  source defect.

## Findings and handoff

No actionable defect found in the reviewed static scope. No repair assignment
is required based on this review. A90 must still produce fresh C++ build and
GPU test evidence; if that exposes failures, create bounded repair/retest nodes
and update TASK.md before sign-off.

## Verify / Commit

- Verify: `git diff --check --ignore-submodules=all`; `python env\bin\format-all-sources.py -dqn`; targeted source/CMake/shader audit.
- Commit: none; task instructions do not authorize commits.

## A92 review addendum

After fresh builds exposed a missed helper path, A91 reviewed A92's synchronous
`BufferVulkan::setContent` and `TextureVulkanBase::setContent` migration, test
compile fixes, and mesh-viewer quaternion include. No actionable behavior,
resource-state, or API contract issue was found. The synchronous submit/wait
semantics and tracked resource-state restoration are preserved. No additional
repair is recommended; fresh linked test/runtime evidence remains A90 work.

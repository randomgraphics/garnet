# Assignment: Migrate the small GPU2 CnC samples

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A12
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: active
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_sample_migration`
- Dependencies: A10/A11 implementations present; their build/test evidence remains pending

## Outcome
Migrate `simple-copy` to transfer-only bindless CnC and `simple-compute` to heap-indexed bindless compute, preserving each sample's observable copy/increment verification.

## Scope and boundaries
Own `src/sample/gpu2/simple-copy.cpp`, `src/sample/gpu2/simple-compute.cpp`, and `src/sample/gpu2/shaders/simple-compute.comp`. Do not migrate the larger alternating-pipeline `bindless.cpp`, tests, old backend deletion, Raster, or other modules in this slice. Keep changes local to these two sample stories; if shader/API constraints require broader changes, record the evidence and replan first.

## Assignment context
Read `../TASK.md`, A03 CnC contract, A10/A11 handoffs, the implementer playbook, and the linked GPU2/C++/build-test skills. Existing CnC sample shaders use `GpuResourceTable` per dispatch; bindless compute must resolve the storage buffer from the caller's descriptor index.

## Procedure
1. Migrate `simple-copy` operations to heap-free `bindless::CnC`, retaining payload submission and byte verification.
2. Migrate `simple-compute` to create/allocate a descriptor heap, pass the storage buffer's bindless index to the compute shader, and record through bindless CnC.
3. Update sample descriptions and shader comments to say bindless CnC.
4. Run formatting/diff checks, compile the shader with the project shader path if available, and attempt the narrow sample build/run only if the environment permits.
5. Record any assumptions/failures and update the next step from evidence; do not claim a GPU sample passed without running it.

## Deliverable and acceptance
Both examples compile against the public bindless CnC API; compute uses a descriptor index rather than a per-dispatch `GpuResourceTable`; copy retains no-heap transfer behavior; sample verification remains intact; exact build/shader/run evidence is recorded.

## Progress and handoff
Implementation complete for the assigned two samples:

- `simple-copy` now records a buffer copy through heap-free `bindless::CnC`.
- `simple-compute` allocates a storage-buffer heap descriptor, passes its slot to the shader, records through heap-backed bindless CnC, and declares the buffer in set 1 `passResources` for write-hazard tracking.
- First direct shader compile failed because GLSL requires `GL_EXT_nonuniform_qualifier` for descriptor-array indexing. Added the extension and `nonuniformEXT`; retry passed: `glslangValidator.exe -V -S comp -o build\windows.vc.d\simple-compute-check.spv src\sample\gpu2\shaders\simple-compute.comp` (exit 0).
- Formatting passed: `. env\garnet.ps1; python env\bin\format-all-sources.py -dqn` (exit 0). `git diff --check --ignore-submodules=all` passed. Targeted search found no `GpuCnC` in either sample; the only `GpuResourceTable` use is set 1 `passResources`, not per-dispatch resource binding.
- The samples were not built or run. The current Windows build path has the repeated MSBuild stall documented in A10/A11. Keep this assignment active pending a later compile/sample run; do not report acceptance yet.

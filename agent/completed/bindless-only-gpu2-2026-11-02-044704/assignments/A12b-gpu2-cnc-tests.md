# Assignment: Migrate GPU2 buffer CnC tests

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A12b
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: source migration complete; fresh build pending; keep open
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_cnc_tests`
- Dependencies: A10/A11 API implementations; A12 shader/sample correction reviewed

## Outcome
Move legacy buffer upload/copy/download and CnC lifecycle tests from `GpuCnC` to heap-free `bindless::CnC`, preserving the tested behavior and failure cases.

## Scope and boundaries
Own only `src/core/gpu2/test/gpu-cnc-test.cpp` and `src/core/gpu2/test/gpu-cnc-async-test.cpp`. Do not change image-transfer tests, mixed Raster/CnC tests, bindless tests, public API/backends, samples, or other modules. Use the existing heap-free semantics for transfer operations; don't create dummy heaps.

## Assignment context
Read `../TASK.md`, A03's rolling-backlog note, A10/A11 handoffs, A12's final shader retry notes, the implementer playbook, and GPU2/C++/build-test skills.

## Procedure
1. Inventory each old CnC test case and operation, distinguishing transfer/lifecycle coverage from compute validation.
2. Migrate the assigned cases to named `bindless::CnC::create(...)`; preserve all assertions and async future semantics.
3. Ensure tests that specifically need compute supply a heap, while transfer-only cases remain heap-free.
4. Run formatting/diff/source checks and attempt the focused internal test/build; document the existing MSBuild limitation if it persists.
5. Update TASK and this handoff based on evidence; leave mixed Raster/CnC tests for the Raster migration slice.

## Deliverable and acceptance
The two assigned files contain no `GpuCnC` references, retain buffer transfer/lifecycle/async assertions under bindless CnC, and have exact verification results. No old public backend/API deletion in this slice.

## Progress and handoff
Implemented the assigned GPU2 CnC tests in both named files. The upload, copy, download, cancellation, teardown, range, and lifecycle tests now use named `bindless::CnC` instances. Transfer and image-transfer cases use no descriptor heap. The null-shader compute validation case supplies a minimal descriptor heap, as required by the bindless compute API. The async image future cases in this file were also migrated because the assignment requires eliminating all legacy CnC references from both owned files; the separate image-transfer test file remains untouched.

Verification:
- `git diff --check -- src/core/gpu2/test/gpu-cnc-test.cpp src/core/gpu2/test/gpu-cnc-async-test.cpp` — passed.
- `rg -n "GpuCnC"` over both assigned files — no matches.
- `python env\bin\format-all-sources.py -dqn` — completed with no reported formatting issues.
- `python env\bin\cit.py -i "[gpu2][cnc]"` — existing Debug `GNtest-internal.exe` passed: 35 test cases, 2815 assertions. This executable predates these source edits; the run is baseline runtime evidence only and does not validate compilation of migrated test sources.
- An initial test-filter attempt using `GPU2/CnC` matched no cases; corrected to Catch2 tags above and reran successfully.

Fresh build remains pending. `python env\bin\build.py -C d` reached MSBuild `Checking File Globs` and stalled without further output; interrupted after 15 seconds. The focused runtime result is from the pre-existing Debug executable and is not compile evidence. Keep A12b open for a build retry.

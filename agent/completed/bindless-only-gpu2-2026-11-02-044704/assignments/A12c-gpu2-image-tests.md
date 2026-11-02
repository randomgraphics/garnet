# Assignment: Migrate GPU2 image CnC tests

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A12c
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: implemented; fresh compile pending
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_image_tests`
- Dependencies: A10/A11 implementations; A12b source migration

## Outcome
Move image upload/copy/download coverage in `cnc-image-transfer-test.cpp` to bindless-owned CnC types and recorders, retaining format/layout/invariant coverage.

## Scope and boundaries
Own only `src/core/gpu2/test/cnc-image-transfer-test.cpp`. Replace legacy `GpuCnC` sections and types with bindless CnC; where the same helper currently exercises both implementations, keep the bindless branch and remove the legacy branch. Preserve tests for padded/compressed regions, cube faces/mips, image copies, synchronous transfer helpers, and read-ready state. Do not edit GPU2 backend files, pure buffer tests, `gpu-cnc-raster-test.cpp`, samples, or external consumers.

## Assignment context
Read `../TASK.md`, A03 rolling-backlog note, A10/A11/A12b handoffs, the implementer playbook and GPU2/C++/build-test skills. `GpuCnC` operation/result structures now have bindless-owned equivalents; transfer-only bindless CnC does not require a heap.

## Procedure
1. Inventory each image test's old recorder/types and identify any unique behavior not already exercised by bindless coverage.
2. Migrate the file to heap-free bindless CnC and bindless operation/result types while preserving useful assertions.
3. Run formatting/diff/search checks and attempt the focused test/build; label existing-binary runs as baseline if they don't include current edits.
4. Update this handoff and TASK; use test/build findings to split the next slice.

## Deliverable and acceptance
The assigned file has no `GpuCnC` references or duplicate ordinary-vs-bindless sections, and preserves meaningful image transfer coverage through the bindless API. Exact source/static/runtime/build evidence is reported without claiming an unbuilt test passes.

## Progress and handoff
Progress: source migration complete. `cnc-image-transfer-test.cpp` now uses heap-free `bindless::CnC` creation and bindless-owned region types; the ordinary-vs-bindless duplicate sections were collapsed while retaining the original padded upload/copy/download/readback, faces/mips, compressed edge blocks, and depth readback assertions. No `GpuCnC` text remains in the assigned file.

Verification:
- Pass: `python env\bin\format-all-sources.py -dqn` (no diagnostics).
- Pass: `git diff --check --ignore-submodules=all` (no diagnostics).
- Pass, **baseline only**: `python env\bin\cit.py -i "[gpu2][cnc][image]"` ran the pre-existing `build\windows.vc.d\bin\Debug\GNtest-internal.exe`: 4 cases / 1620 assertions. It predates this source edit and does not validate the migration.
- Fresh build attempted: `python env\bin\build.py -C d`; stalled at MSBuild `Checking File Globs` after 30 seconds and was interrupted (exit 1). Changed C++ compilation and migrated runtime behavior remain unverified.

Status: implementation handoff complete, awaiting fresh build/test retry in later verification slice and user sign-off.

# Assignment A90: Build, test, and acceptance verification

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A90
- Role: Test/verifier
- Status: partial; fresh core and migrated test/sample translation units compile with Ninja Debug; linked test executable and GPU runtime remain blocked by third-party Assimp C1902
- Requirement revision: 0.2 user-confirmed
- Owner: `/root`
- Dependencies: A10-A17 source migrations; A91 independent source review

## Goal

Verify the completed bindless-only migration with the repository build/test workflow, focused GPU tests and affected headless samples. Record criterion-level results and create repair work if a source or runtime check fails.

## Scope and acceptance

- Fresh C++ build includes migrated GPU2/FX2 sources and renamed internal test files.
- Run focused CnC, Raster/RTT/hazard, ImGui, image-transfer and mixed upload/raster/compute tests against that fresh binary.
- Run the affected headless GPU2/RDG2 sample modes where available.
- Formatting, diff and active-source legacy-symbol searches pass.
- Record any unavailable or failing checks without calling stale binaries changed-source validation.

Verify: `python env\bin\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; `python env\bin\build.py -C d`; focused tests via `env\bin\cit.py` or the freshly built internal executable; headless sample invocations recorded in A12/A14 assignments.
Commit: `test(gpu2): record bindless-only verification`

## Progress and handoff

Passed:

- `python env\bin\format-all-sources.py -dqn` after source retirement.
- `git diff --check --ignore-submodules=all` after source retirement.
- Active-source search `rg -n "GpuCnC|GpuRaster|recordBindBasedDraw|GpuCncVulkan2|RasterPsoFactory|vk-gpu-cnc|vk-gpu-raster|gpu-cnc-test|gpu-raster-test" src --glob '!**/MIP_LEVEL_RESEARCH.md' --glob '!**/retired/**'` returns no matches. The only remaining mentions in `src` are explicitly historical excerpts in `src/core/gpu2/MIP_LEVEL_RESEARCH.md`.
- Standalone shader compiles are recorded in A12, A14c, A14d, and A15.
- Independent static review A91 found no actionable source defect; its exact search and contract review are recorded in `A91-independent-review.md`.
- `cmake -P build\windows.vc.d\CMakeFiles\VerifyGlobs.cmake` exits 0 in about 0.05 seconds.
- Final coordinator pass: `python env\bin\format-all-sources.py -dqn` and `git diff --check --ignore-submodules=all` both exit 0; the active-source legacy API search exits 1 with no matches (expected clean result). `git status --short --ignore-submodules=all` lists the task's modified/deleted/new files and succeeds.
- Recovery route: configured a separate ignored `build/windows.ninja.d` Debug build using Ninja under `VsDevCmd.bat` with project Debug flags using `/Z7`. `ninja -C build/windows.ninja.d -j8 src/core/GNcore-objects` passed after A92 rework, compiling all owned core sources including gpu2/FX2/FIZ/E2.
- Direct Ninja compilation passed for the migrated GPU2/FX2 test translation units: `cnc-buffer-test.cpp`, `cnc-buffer-async-test.cpp`, `cnc-image-transfer-test.cpp`, `cnc-raster-mixed-test.cpp`, `raster-target-test.cpp`, `bindless-cnc-test.cpp`, `bindless-raster-test.cpp`, `raster-hazard-test.cpp`, `rtt-test.cpp`, and `imgui-backend-test.cpp`. First pass exposed two source defects in mixed CnC compute designated-initializer ordering and a Catch assertion comparing a void pointer; both were repaired, then the two affected objects recompiled successfully.
- Direct Ninja compilation passed for changed sample/tool translation units: FIZ gel/solids, GPU2 hello-world/simple-copy/simple-compute/bindless, RDG2 clear/cubemap/open-graph triangle, and mesh-viewer main/scene renderer. The first pass exposed a missing direct `<glm/gtc/quaternion.hpp>` include in `scene-renderer.h`; after adding it and generating the RDG cube shader headers, all selected changed translation units compiled.

Pending / environment-limited:

- The repository wrapper's Visual Studio route remains unusable here: `python env\bin\build.py -C d` reran CMake configure/generate, then MSBuild printed `Checking File Globs` and returned no result after bounded polls; focused and direct MSBuild retries behaved the same.
- Direct Visual Studio 18 MSBuild attempts for `GNcore-objects.vcxproj` (`/t:Build` and `/t:ClCompile`, with `BuildProjectReferences=false`) also produced no compile result or diagnostics after bounded waits and were interrupted. A `/v:diag` log stopped after `ZERO_CHECK.vcxproj` completed `GetTargetPath`; the standalone VerifyGlobs script itself is fast.
- A fresh linked internal test executable and GPU runtime tests/samples remain unavailable. `ninja -C build/windows.ninja.d -j8 GNtest-internal` reaches the broad third-party build, where Assimp Debug commands append `/Zi` after `/Z7` and fail with `fatal error C1902: Program database manager mismatch`. This is a toolchain/dependency build failure; it does not invalidate the successful compile of owned core and selected migrated test/sample translation units. A12b/A12c old-executable test results remain baseline-only.
- Tried a separate Ninja Release configuration to avoid Debug-only Assimp flags. CMake's initial compiler ABI check still uses `/Zi` and fails with the same MSVC C1902, and the configured Release Assimp commands also append `/Zi` and fail identically. This route does not bypass the installed toolchain issue; no further broad retry is useful until the compiler/PDB manager installation or Assimp debug-info configuration changes.
- Local toolchain inventory found only VS 18 MSVC 14.51 and no installed `clang-cl`, LLVM, or second Visual Studio toolset. No alternative compiler path is available in this workspace.
- At the user's request, retried the exact full command `python .\env\bin\build.py d`. CMake configure/generate completed successfully in about five seconds; CMake/MSBuild then printed `Checking File Globs`. After several minutes the wrapper exited with code 1 and `[ERROR] cmake failed`, but emitted no underlying MSBuild diagnostic. A subsequent direct `cmake --build . -j8 --config Debug --verbose` showed `ZERO_CHECK` and `VerifyGlobs.cmake` completing successfully, then produced no further output during bounded waits; the user asked to pause build investigation and the run was interrupted. This build retry is inconclusive beyond the wrapper's nonzero exit.
- A final bounded retry, `python env\bin\build.py -C d --target GNcore-objects`, again printed only MSBuild `Checking File Globs` during a 30-second wait and was interrupted. No C++ compile outcome was produced.
- An initial Ninja configure outside the VS developer environment failed due to missing standard include environment (`stdint.h`); invoking Ninja through `VsDevCmd.bat` fixed this. A first full internal-test target attempt also encountered the third-party Assimp C1902 described above. These attempts informed the bounded verification route; they were not counted as project source failures.

Keep this assignment and the parent task active while trying a bounded alternative for linking/running fresh GPU tests. No commit is authorized.

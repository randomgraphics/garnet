# Task: Add Automated Code Quality Assessment to Harness Review

## Overview

- Branch/worktree: `wip/agent-harness` (existing worktree).
- Status: Complete and signed off by the user on 2026-11-02. Accepted scope and remaining platform limitations are recorded below.
- Completed: separated harness/skill/knowledge guidance; installed temporary Lizard; scanned 5,217 production and 1,005 test functions; used Microsoft's native MSVC coverage collector; inventoried Windows compile logs (93 translation units and 114 first-party headers); `cit.py -l` passed and all 202 unit tests passed.
- Current step: None. This baseline and Harness workflow have been signed off.
- Build output: collector settings, generated coverage, joined quality reports, and run logs live under each build variant's `output/config/`, `output/coverage/`, `output/quality/`, and `output/logs/`, respectively. These ignored `build*/` paths are independent per build directory.
- Future work outside this signed-off task: obtain a complete Windows compiled-code line denominator; diagnose the Windows Vulkan/internal-test failures; compare against Linux GCC coverage; adjust Docker validation only on Linux if needed.
- Latest verification: Windows assessment output is in `build/windows.vc.d/output/quality/`; with `src/sample/` excluded, 72.77% function execution among compiled-scope functions with executable-line mapping. The 69.26% Cobertura ratio covers only report-emitted lines and is diagnostic, not the whole-build denominator. Test failures are recorded but do not invalidate collected coverage evidence. No Docker used.
- Blockers: current host is Windows; WSL returned `Wsl/E_ACCESSDENIED`; Garnet's canonical gcovr coverage workflow remains Linux-only.

## Non-recoverable Resume Context

Keep only this compact task context in `agent/workspace/`. Generated reports,
raw data, logs, settings, file inventories, and helper scripts are reproducible
outputs and live under the matching ignored `build/<variant>/output/` folder.
The Windows outputs are local to this machine and will not move with the Git
workspace, so preserve the facts below for a cross-machine continuation.

- User decisions: current platform is Windows; do not use Docker here. Defer
  Docker-related changes until the Linux phase. Exclude `src/sample/` from
  coverage and the coverage/complexity join for now, but keep its CCN data in
  the standalone complexity distribution. Never score code unsupported by the
  current platform; label it `not assessed`.
- Baseline environment: Windows 11 x64, Visual Studio 18 Community,
  `Microsoft.CodeCoverage.Console` 18.10.0-preview, Lizard 1.24.3; assessment
  revision `69c9d4b1ed0e45ca04ce3eeb4ccb37c824a958bb`.
- Tests: lint passed; 202 unit tests passed. Full internal run aborted on a
  Vulkan depth-image layout validation failure (exit `2147483651`). Filtered
  internal run excluding `[gpu2]`: 102 passed, 8 failed, 2 skipped. Its
  coverage remains diagnostic evidence.
- Complexity: 5,217 production functions (5,073 CCN 1–10; 100 CCN 11–20; 44
  CCN >20), plus 1,005 test functions. Highest active-module hotspots include
  `VkBindlessPayload::recordForVulkanSubmit` (CCN 67),
  `VkBindlessCncPayload::recordForVulkanSubmit` (CCN 63),
  `GN::errno2str` (CCN 39, no mapped hits), `WindowMsw::handleMessage`
  (CCN 27, no mapped hits), and `GN::backtrace` (CCN 23, no mapped hits).
- Windows compile evidence: 93 first-party production translation units and
  114 first-party headers read during compile; `src/tool/d3dhook` is outside
  the configured root tool build and is not assessed on this Windows build.
- Windows coverage: after excluding `src/sample/`, 2,757 compiled-file-scope
  function definitions remain in the join. Function execution proxy is
  1,537/2,112 = 72.77%; 645 have no executable-line mapping in the collected
  reports. Cobertura report-corpus line ratio is 9,203/13,288 = 69.26%, but
  this is not a whole-build line-coverage percentage because some compiled
  modules are absent from runtime reports. The complete compiled-code line
  denominator remains to be collected.
- Windows outputs: `build/windows.vc.d/output/{config,coverage,logs,quality}/`.
  WSL failed with `Wsl/E_ACCESSDENIED`; no Linux GCC coverage run exists yet.

## Goals

Add quality assessment to code review centered on test coverage and per-function cyclomatic complexity. Keep generic review principles in `agent/harness/`; put Garnet-specific procedure and acceptance bands in `agent/skills/`; put evidence-backed current tooling capabilities and gaps in `agent/knowledge/`. Make quality distribution visible at function and module/platform level, and retain platform-specific follow-up without blocking current-platform integration when host policy permits.

## Design

- Generic harness states review independence, evidence, limitation recording, and follow-up lifecycle; it does not name metrics or thresholds.
- Garnet skill contains commands, changed-function test criteria, provisional complexity triage bands, report shape, and platform acceptance rules.
- Garnet knowledge records available coverage outputs, CI platform limits, and the absence of an integrated complexity collector.
- Coverage denominator is first-party code compiled on the current platform, including compiled code with no test hits. Code excluded by the platform build is `not assessed` and excluded; supported compiled code with no test hit counts as uncovered. Do not present a whole-build line percentage until executable-line denominators include compiled modules absent from the runtime report.
- Temporary scope: exclude `src/sample/` from coverage and combined coverage/complexity results; retain sample CCN in the standalone complexity distribution.

--- PROGRESS (resume from "Last completed" / "In progress") ---
Last completed: Windows diagnostic baseline and Harness workflow — complexity and native coverage distributions captured; platform/sample scope and limitations recorded; build outputs separated from concise resume context; user sign-off received.
In progress: None. Task complete; future work above is separate from this signed-off baseline.
--- END PROGRESS ---

## REFERENCE

- `agent/harness/ASEH_DESIGN.harness.md`
- `agent/harness/roles/reviewer.harness.md`
- `agent/harness/task-workspace.harness.md`
- `agent/skills/garnet-code-coverage/SKILL.md`
- `agent/skills/garnet-code-quality-assessment/SKILL.md`
- `agent/knowledge/code-quality-assessment.md`

## BUILD / CI

- Windows: `python env/bin/cit.py -l` passed; `python env/bin/cit.py -t` failed in GPU2 Vulkan validation; 202 unit tests passed; `python env/bin/cit.py -i '~[gpu2]'` reported 102 passed, 8 failed, 2 skipped.
- Coverage: native `Microsoft.CodeCoverage.Console` 18.10.0-preview collected Cobertura line data from unit and filtered internal tests. Full details and limitations are in the linked assessment evidence.
- Linux GCC and Docker follow-up have not run on this Windows host.

## WORKFLOW / CONVENTIONS

- Keep generic orchestration rules in `agent/harness/`.
- Keep finished task active and awaiting user sign-off; do not archive automatically.

## TASK BREAKDOWN

Phase 1: Define the metric assessment contract — complete
  Task 1.1: Specify automated coverage and complexity data, distribution reporting, hotspot ranking, and platform exception handling.
    Verify: Completed initial metric contract; its code-specific details are now maintained in Garnet skill/knowledge while generic evidence and follow-up principles remain in harness.
    Commit: "docs(harness): define review quality metrics (Phase 1.1)"

Phase 2: Integrate metrics into reviewer workflow — complete
  Task 2.1: Add reviewer steps and required handoff fields, then audit related workspace guidance for consistency.
    Verify: Reviewer/workspace contracts agree on host-specific evidence, limitations, and linked follow-ups; links resolve.
    Commit: "docs(harness): integrate quality assessment into review (Phase 2.1)"

Phase 3: Separate framework policy from Garnet-specific guidance — complete
  Task 3.1: Keep generic review principles in harness; move test commands, metric bands, pass criteria, and measured tooling facts into Garnet skill/knowledge and update indexes.
    Verify: Check harness has no Garnet-specific metric/tool thresholds; validate skill and knowledge links and audit review policy consistency.
    Commit: "docs(agent): separate review policy from Garnet quality checks (Phase 3.1)"

Phase 4: Complete cross-platform quality evidence
  Task 4.1: Triage low-coverage and high-complexity hotspots and add focused test/review follow-ups to the task graph.
    Verify: Every follow-up names source locations, target platform, reason, routing condition, and observable completion evidence.
    Commit: "docs(agent): track quality assessment follow-ups (Phase 4.1)"
  Task 4.2: Diagnose the Windows GPU2 Vulkan validation failure and the eight filtered-suite failures; record their dispositions without blocking diagnostic coverage analysis.
    Verify: Reproduce the focused internal test; determine whether the failure is environment/device-specific or a code/test defect and record any repair. Existing coverage evidence remains usable for paths reached by those runs.
    Commit: "test(gpu2): resolve Windows validation failure (Phase 4.2)"
  Task 4.3: Run supported Linux GCC coverage and combine function coverage with the Windows complexity baseline.
    Verify: `env/bin/code-coverage.py` produces `functions.md` and `coverage.json`; assessment records revision/platform and coverage-by-complexity distribution.
    Commit: "docs(agent): add Linux coverage to quality assessment (Phase 4.3)"
  Task 4.4: Adjust Docker coverage validation only if the Linux run shows it is needed.
    Verify: Run the Linux coverage workflow through the intended Docker path and capture report artifacts.
    Commit: "build(docker): support Linux quality assessment (Phase 4.4)"

## SUMMARY OF FILES TO TOUCH

- `agent/harness/ASEH_DESIGN.harness.md`
- `agent/harness/roles/reviewer.harness.md`
- `agent/harness/task-workspace.harness.md` (only if needed to align the review record contract)
- `agent/skills/garnet-code-quality-assessment/SKILL.md`
- `agent/skills/garnet-code-coverage/SKILL.md`
- `agent/skills/README.md`
- `agent/knowledge/code-quality-assessment.md`
- `agent/knowledge/README.md`
- `agent/knowledge/catalog.md`
- `agent/workspace/review-quality-metrics/TASK.md`
- `build/windows.vc.d/output/quality/evidence/windows-2026-11-01/ASSESSMENT.md` (local generated evidence, ignored by Git)

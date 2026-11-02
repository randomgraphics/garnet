---
name: garnet-code-quality-assessment
description: Assess Garnet code changes during review using tests, function-level coverage, and cyclomatic complexity. Use for review quality distributions, test adequacy, complexity hotspots, and platform verification gaps.
---

# Garnet Code-Quality Assessment

Use this procedure during code review after confirming the exact revision and
changed scope. Follow the generic review gate in
[`agent/harness/roles/reviewer.harness.md`](../../harness/roles/reviewer.harness.md);
this skill supplies Garnet-specific checks and acceptance guidance. Current
tooling facts and limitations are in
[`agent/knowledge/code-quality-assessment.md`](../../knowledge/code-quality-assessment.md).

## Collect test and coverage evidence

Initialize the environment and run the project wrappers appropriate to the
change and available platform:

```bash
source env/garnet.rc
env/bin/cit.py -l
env/bin/cit.py -t
env/bin/cit.py -c   # Linux GCC coverage build, tests, and report
```

On Windows, invoke Python scripts through `python.exe` and use the Windows
build/test paths described in
[`garnet-build-test`](../garnet-build-test/SKILL.md). Garnet's `build.py c` /
gcovr workflow itself remains Linux-only. MSVC has a separate native C++ path
through Visual Studio's `Microsoft.CodeCoverage.Console`; do not treat missing
coverage from the Linux-only build variant as 0%.

For a Linux GCC coverage run, inspect `build/linux.gcc.c/output/coverage/functions.md`
and `coverage.json`. The function report says whether each function executed
at least once; use HTML/summary output for line and branch coverage. Associate
the changed functions to source locations, then report changed-function and
module distributions alongside repository totals. Execution is evidence that a
path ran, not proof that assertions check its behavior. Read the relevant tests
and identify untested changed branches separately.

Calculate each platform's line-coverage percentage over executable lines in
first-party code compiled for that platform, including executable lines with
no test hits. Exclude code that the platform build does not compile. Do not
use only the lines emitted by runtime coverage reports as the whole-build
denominator: test runs can omit compiled modules that they never load. If the
collector cannot provide executable-line counts for those modules, mark the
whole-build line percentage unavailable and report a clearly scoped
diagnostic ratio. For function-level metrics, use the platform compile
inventory: compiled functions with no test hit count as uncovered; functions
outside the compile inventory are `not assessed` and excluded. Do not infer
platform incompatibility solely from missing coverage data.

For the current Garnet baseline, exclude `src/sample/` from coverage metrics
and the coverage × complexity matrix, even when sample files are compiled.
Keep sample functions in the separate complexity inventory so their CCN
distribution remains visible; do not label their coverage as 0% or as a
platform gap. Revisit this temporary scope when sample coverage is requested.

### Windows MSVC coverage

When Visual Studio's C++ workload is installed, locate
`Microsoft.CodeCoverage.Console.exe` under
`<VS install>\Common7\IDE\Extensions\Microsoft\CodeCoverage.Console\`.
Use a Debug build with matching PDBs and a C++ settings file that limits
instrumentation to the build's `bin\Debug` directory. For example:

```xml
<?xml version="1.0" encoding="utf-8"?>
<Configuration>
  <CodeCoverage>
    <ModulePaths>
      <IncludeDirectories>
        <Directory>D:\path\to\garnet\build\windows.vc.d\bin\Debug</Directory>
      </IncludeDirectories>
    </ModulePaths>
    <EnableDynamicNativeInstrumentation>True</EnableDynamicNativeInstrumentation>
    <EnableStaticNativeInstrumentation>False</EnableStaticNativeInstrumentation>
  </CodeCoverage>
</Configuration>
```

Collect the test executables separately so each result can be attributed to its
test scope:

```powershell
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$vsInstall = & $vswhere -latest -property installationPath
$coverage = Join-Path $vsInstall 'Common7\IDE\Extensions\Microsoft\CodeCoverage.Console\Microsoft.CodeCoverage.Console.exe'
$reportDir = 'build\windows.vc.d\output\coverage'
$qualityDir = 'build\windows.vc.d\output\quality'
$settings = 'build\windows.vc.d\output\config\coverage-msvc-settings.xml'
$null = New-Item -ItemType Directory -Force $reportDir, $qualityDir
$null = New-Item -ItemType Directory -Force (Split-Path $settings)
$env:DOTNET_COVERAGE_TELEMETRY_OPTOUT = '1'
& $coverage collect --settings $settings --output (Join-Path $reportDir 'coverage-unit-tests.cobertura.xml') --output-format cobertura build\windows.vc.d\bin\Debug\GNtest-unit-tests.exe
& $coverage collect --settings $settings --output (Join-Path $reportDir 'coverage-internal-no-gpu2.cobertura.xml') --output-format cobertura python env\bin\cit.py -i '~[gpu2]'
python build\windows.vc.d\output\quality\evidence\windows-2026-11-01\join_coverage_complexity.py --build-dir build\windows.vc.d --compiled-files build\windows.vc.d\output\quality\evidence\windows-2026-11-01\windows-compiled-production-files.txt --complexity-csv build\windows.vc.d\output\quality\evidence\windows-2026-11-01\complexity-production.csv
```

The joined markdown and JSON reports are written to
`build/windows.vc.d/output/quality/`. Use separate build directories for other
platform/compiler/configuration runs; each keeps independent coverage and
quality output under its own `output/` folder. Keep build-specific collector
settings in `output/config/`. The repository `/build*/` ignore rule excludes
these generated artifacts and settings from Git.

Visual Studio's native collector emits Cobertura line data. Map executable
Cobertura lines to Lizard function ranges to estimate function coverage, and
label the mapping as heuristic; do not claim it is identical to gcovr's
function records or branch coverage. Static native instrumentation requires
`/PROFILE`; the example explicitly uses dynamic instrumentation instead.
Record any failing/skipped tests with the report. If the full internal suite
aborts on a platform/API validation test, a tag-filtered run such as
`cit.py -i '~[gpu2]'` can produce a diagnostic partial report, but it cannot be
reported as a passing full suite.

Garnet's CI and `build.py c` are not wired to this Windows collector. Use it for
local MSVC assessment; keep the canonical Linux GCC coverage workflow as a
separate CI/platform result.

## Measure cyclomatic complexity

Use a configured C++-capable collector that reports cyclomatic complexity per
function, source location, and analyzed revision. Record the tool/version,
command, scope, exclusions, and raw output. Join results with coverage by stable
function/source identity, then summarize by module and platform. Include a
coverage-presence by complexity-band count matrix and link high-risk cells to
the affected functions.

The repository currently has no configured first-party complexity collector.
Until one is selected and wired into the environment, report complexity as
`unknown`, identify the missing collector as a tooling gap, and create a linked
follow-up to configure it. Do not describe a partial coverage-only report as a
complete two-metric assessment. Never estimate complexity by counting source
lines or branches manually and present it as a measured value.

## Review bands and acceptance

Use these initial reviewer bands to rank findings, not as a hidden CI gate:

| Metric | Band | Review treatment |
| --- | --- | --- |
| Function coverage | 0 executions | No test exercised the function; require a focused test when supported and reachable on the current platform. If incompatible with the host, label `not assessed`, exclude it from the current-platform quality score, and create a target-platform follow-up. |
| Function coverage | 1+ executions | Function ran at least once; inspect assertions and relevant branches before treating behavior as verified. |
| Cyclomatic complexity | 1–10 | Routine review; verify behavior and edge cases as usual. |
| Cyclomatic complexity | 11–20 | Elevated; inspect branch interactions and test each important decision path. Consider decomposition where it improves clarity. |
| Cyclomatic complexity | >20 | High-risk hotspot; require focused tests and a concrete rationale or decomposition plan before considering the review complete. |

These are review triage bands introduced by this skill; they are not existing
repository CI settings. Apply them to individual functions and changed code,
not just repository-wide averages. Prioritize functions that combine no test
execution with elevated complexity, then compare per-module distributions to
locate concentrated low-quality areas. Reviewers may justify exceptions with
specific evidence; record the rationale and follow-up instead of silently
waiving a hotspot.

For a review to pass on the current platform:

1. Relevant lint/style and test commands for the change pass on that platform,
   or their failures are resolved before approval.
2. Every changed function is covered by an appropriate test or has an explicit,
   evidence-backed reason why it cannot be exercised there. A function's
   execution count alone does not satisfy this criterion.
3. Complexity hotspots receive focused test/control-flow review and an explicit
   disposition. Do not claim the complexity criterion passed while the
   collector is unavailable; record it as unmeasured and link its tooling task.
4. A code path incompatible with the current platform is `not assessed`: do
   not assign it a coverage score, include it in a combined quality score, or
   treat its complexity as evidence of poor current-platform quality. This is
   nonblocking for the current platform. Record affected paths, target
   platform, incompatibility evidence, and a future task to add/run
   target-platform tests and repeat the quality review. Keep that task open
   until verified. A supported function missing from test coverage remains a
   0% coverage finding, not a platform exception.
5. A defect or reproducible verification failure on the current platform is not
   excused by a different platform's limitation.

## Review artifact

Attach a compact report to the review assignment with:

- revision, changed scope, platform, build/test commands and results;
- collector names/versions, commands, exclusions, and raw artifact links;
- line/branch summary and function execution counts by module, with changed
  functions called out;
- complexity distribution or explicit `unknown` status and tooling follow-up;
- coverage-status × complexity-band matrix (including `not assessed`) with
  links to risk functions; exclude `not assessed` rows from scored
  denominators;
- each platform exception, its evidence, and the linked future task ID;
- findings and their disposition.

Keep generated reports in the configured ignored build/report directory or
attach them to the task workspace; do not commit large generated coverage data.

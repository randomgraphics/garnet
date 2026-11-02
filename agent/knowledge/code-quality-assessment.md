# Garnet Code-Quality Assessment: Current Capabilities

## Evidence-backed facts

- The Linux GCC coverage workflow is implemented by `build.py c`,
  `env/bin/code-coverage.py`, and `env/bin/cit.py -c`; see
  [`build-and-platforms.md`](build-and-platforms.md) and the
  [`garnet-code-coverage` skill](../skills/garnet-code-coverage/SKILL.md).
- The generated `<build-variant>/output/coverage/functions.md` report includes per-module function
  execution counts/percentages and lists unexecuted functions. Its raw
  `coverage.json` provides each function's demangled name, source line, and
  `execution_count`. Function coverage here means executed at least once; it
  does not prove assertion quality or provide a per-function percentage.
- The Garnet `build.py c` and `code-coverage.py` entrypoints explicitly reject
  non-Linux hosts. Root CMake instruments with GNU-style `--coverage` and
  disables the option for MSVC. Coverage is only configured in the Linux GCC
  CI job; Windows, Linux Clang, and Android CI jobs do not produce a report.
  See `env/bin/build.py`, `env/bin/code-coverage.py`, root `CMakeLists.txt`, and
  [`build-and-platforms.md`](build-and-platforms.md).
- Windows coverage is technically possible through a separate MSVC/PDB-aware
  collector, but that is not the current Garnet gcov/gcovr backend and would
  need an explicit adapter and report-schema mapping before its metrics can be
  compared directly. Do not use the current Linux coverage command on Windows.
- The Windows host used for this assessment has Visual Studio 18 Community and
  `Microsoft.CodeCoverage.Console` 18.10.0-preview. Microsoft's collector
  supports native C++ and Cobertura output. A manual run succeeded against the
  existing Debug test binaries when dynamic native instrumentation was enabled
  and matching PDBs were present. This is not wired into Garnet's build/CIT/CI
  workflow. The evidence and command/settings are in
  the active task's non-recoverable Windows baseline summary in
  [`TASK.md`](../workspace/review-quality-metrics/TASK.md); generated detail
  stays in the ignored local build output.
- Generated coverage and quality outputs belong under the matching build
  variant's `output/coverage/` and `output/quality/` directories. Build-specific
  settings belong in `output/config/`; `.log` and `.exit` run records belong
  in `output/logs/`. The `/build*/` ignore rule
  keeps them out of Git and prevents different builds from overwriting one
  another.
- The current Garnet quality baseline temporarily excludes `src/sample/` from
  coverage percentages and the coverage/complexity join, while retaining its
  functions in the separate complexity inventory. See the assessment skill for
  the scope rule.
- The Windows Cobertura report is line-based. The assessment maps those lines
  onto Lizard function ranges to estimate function execution/line coverage;
  treat that as a review aid, not equivalent to gcovr's per-function records
  or branch coverage.
- A platform's line-coverage percentage uses only executable lines emitted by
  first-party code compiled for that platform as its denominator, including
  executable lines with no test hits. Other-platform code not selected by the
  current platform's compile is excluded. Do not divide by the repository-wide
  source inventory. A runtime coverage report can omit compiled modules that
  tests never load; those files must still be represented as uncovered in the
  denominator. When the report cannot provide their executable-line count,
  label the whole-build line percentage unavailable and report only a clearly
  scoped diagnostic ratio. For function metrics, derive scope from the actual
  platform compile inventory; functions in compiled code with no test hit are
  uncovered, while functions outside it are `not assessed`.
- Repository searches on 2026-11-01 found no first-party cyclomatic-complexity
  collector, configured complexity thresholds, or code-quality merge gate.
  The Windows baseline in the linked task used a temporary Lizard 1.24.3
  install; that collector is not integrated into Garnet's environment or CI.
  Do not infer that an absent report means zero complexity or a pass.

## Review implications

Use the current-platform coverage report first, then join it with complexity
results to locate quality hotspots. Associate report rows to the reviewed diff
by function name and source location. A test run may still provide useful
coverage data when some tests fail; record the failures and treat the coverage
as diagnostic evidence for the code paths that ran.

Classify coverage for each function as `covered`, `partial`, `0%`, `not
assessed`, or `no executable lines`. Use `not assessed` when the function's
platform/backend cannot be built or executed on the assessment host, and cite
the platform/build evidence. Do not infer platform incompatibility merely from
an absent row: distinguish code that is unsupported on this host from code
that is supported but was not built or exercised. Platform-incompatible code
has no current-platform quality score; do not include it in coverage or
combined quality denominators and do not count it as a low-quality region.
Keep its complexity inventory visible with the `not assessed` status and a
target-platform follow-up. For supported code that was built but never reached,
report 0% coverage; for incomplete test execution, label the result diagnostic
and retain whatever coverage evidence was collected.

The host-specific procedure and provisional review rubric are in
[`garnet-code-quality-assessment`](../skills/garnet-code-quality-assessment/SKILL.md).

## Sources and freshness

Microsoft documents native C++ collection in
[Microsoft.CodeCoverage.Console](https://learn.microsoft.com/en-us/visualstudio/test/microsoft-code-coverage-console-tool?view=vs-2022)
and [coverage configuration](https://learn.microsoft.com/en-us/visualstudio/test/customizing-code-coverage-analysis?view=visualstudio).

Verified from `env/bin/code-coverage.py`, `env/bin/cit.py`, root/core CMake,
`.circleci/config.yml`, repository-wide first-party search, the local Visual
Studio installation, and Microsoft's native C++ coverage documentation on
2026-11-01. Recheck these sources when coverage scripts, CI jobs, or analysis
dependencies change. These facts distinguish configured workflow from a
particular local assessment; they do not claim the full test suite passed.

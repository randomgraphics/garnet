#!/usr/bin/python3
"""Generate a function-level gcov coverage report from the coverage build variant.

The coverage variant (`build.py c`) compiles the project at -O0 with --coverage into its
own build folder, keeping the instrumentation cost off the daily debug build. This script
drives the whole cycle: build, clear stale counters, run the CIT test set, then turn the
resulting .gcda files into HTML, JSON and a function-level markdown report.

Reports land in <build-dir>/coverage, which the /build*/ gitignore rule already covers.
"""

import argparse, datetime, importlib, json, os, pathlib, platform, shutil, subprocess, sys
utils = importlib.import_module("garnet-utils")
cit = importlib.import_module("cit")

SCRIPT_DIR = pathlib.Path(os.path.realpath(__file__)).parent
ROOT_FOLDER = utils.get_root_folder()

# Only project sources belong in the report. Generated files (ut.cpp, features.h, SPIR-V
# headers) physically live under the build folder even though their paths contain src/, so
# the build exclusion has to come first. Vendored code is excluded by directory.
GCOVR_FILTERS = [r"(^|/)src/(core|inc|sample|test|tool)/"]
GCOVR_EXCLUDES = [
    r"(^|/)build[^/]*/",
    r"(^|/)src/3rdparty/",
    r"(^|/)src/test/3rdparty/",
]

def coverage_build_dir(build_root, use_clang):
    build_dir = pathlib.Path(build_root)
    if not build_dir.is_absolute():
        build_dir = ROOT_FOLDER / build_dir
    system = utils.BuildSystem(use_clang = use_clang)
    return build_dir / f"{system.build_dir()}{utils.COVERAGE_BUILD_VARIANTS[0]}"

def build_variant(build_root, use_clang):
    cmd = [sys.executable, str(SCRIPT_DIR / "build.py")]
    if use_clang: cmd += ["--clang"]
    cmd += ["-b", str(build_root), "c"]
    utils.logi("building coverage variant: " + " ".join(cmd))
    subprocess.run(cmd, check = True, cwd = ROOT_FOLDER)

def require_instrumented(build_dir):
    if not build_dir.is_dir():
        utils.rip(f"coverage build directory not found: {build_dir}\n"
                  f"        Run 'build.py c' first, or drop --no-build.")
    if next(build_dir.rglob("*.gcno"), None) is None:
        utils.rip(f"no .gcno files under {build_dir}.\n"
                  f"        That directory was not configured with GN_BUILD_CODE_COVERAGE=ON.\n"
                  f"        Re-run 'build.py c' (configure is required, -C alone is not enough).")

def clear_gcda(build_dir):
    # gcov accumulates into existing .gcda files. Without this every report would silently
    # fold in the results of all previous runs.
    removed = 0
    for gcda in build_dir.rglob("*.gcda"):
        gcda.unlink()
        removed += 1
    utils.logi(f"cleared {removed} stale .gcda file(s)")

def run_tests(test_args):
    """Run the CIT test set against the coverage variant.

    Returns the exception from the first failing binary, or None. Failures are reported
    rather than raised so the report still gets generated from the tests that did run; the
    caller turns a non-None result into a non-zero exit code.
    """
    args = argparse.Namespace(test_args = test_args)
    try:
        cit.run_all_tests(args, variants = utils.COVERAGE_BUILD_VARIANTS)
    except subprocess.CalledProcessError as err:
        return err
    return None

def find_gcovr():
    exe = shutil.which("gcovr")
    if exe is not None: return [exe]
    try:
        import gcovr # noqa: F401  probe only
        return [sys.executable, "-m", "gcovr"]
    except ImportError:
        utils.rip("gcovr not found. Activate the Garnet environment (source env/garnet.rc)\n"
                  "        or install it directly: pip install gcovr")

def run_gcovr(build_dir, out_dir, use_clang):
    cmd = find_gcovr() + [
        "--root", str(ROOT_FOLDER),
        str(build_dir),
    ]
    for pattern in GCOVR_FILTERS: cmd += ["--filter", pattern]
    for pattern in GCOVR_EXCLUDES: cmd += ["--exclude", pattern]
    if use_clang:
        # Clang emits gcov-incompatible notes; its own frontend has to decode them.
        if shutil.which("llvm-cov") is None:
            utils.rip("llvm-cov not found, which is required to decode clang coverage data.")
        cmd += ["--gcov-executable", "llvm-cov gcov"]
    cmd += [
        # GCC bug 68080 makes gcov emit negative hit counts on some lines and gcovr treats
        # that as a hard parse error, killing the whole report at the first affected file
        # (src/inc/garnet/base/smartptr.h in practice). warn_once_per_file keeps the
        # problem visible without flooding the log; "all" is deliberately not used because
        # it would also swallow unrelated parse errors.
        "--gcov-ignore-parse-errors=negative_hits.warn_once_per_file",
        "--html-details", str(out_dir / "index.html"),
        "--json", str(out_dir / "coverage.json"),
        "--json-summary", str(out_dir / "summary.json"),
        "--print-summary",
    ]
    utils.logi("running gcovr")
    subprocess.run(cmd, check = True, cwd = ROOT_FOLDER)

def module_of(relative_path):
    parts = pathlib.PurePosixPath(relative_path).parts
    # Group by src/<area>/<module>. A file sitting directly in src/<area> has only three
    # parts, so requiring four keeps it from becoming a one-file "module" of its own.
    if len(parts) >= 4 and "src" == parts[0]: return "/".join(parts[:3])
    if len(parts) >= 2: return "/".join(parts[:2])
    return relative_path

def git_describe():
    try:
        commit = subprocess.run(["git", "rev-parse", "--short", "HEAD"], cwd = ROOT_FOLDER,
                                capture_output = True, check = True).stdout.decode().strip()
        branch = subprocess.run(["git", "rev-parse", "--abbrev-ref", "HEAD"], cwd = ROOT_FOLDER,
                                capture_output = True, check = True).stdout.decode().strip()
        return f"{branch} @ {commit}"
    except (subprocess.CalledProcessError, FileNotFoundError):
        return "unknown"

def percent(covered, total):
    return 0.0 if not total else 100.0 * covered / total

def write_function_report(out_dir):
    """Turn gcovr's full JSON into the function-level markdown report.

    coverage.json carries one record per function with its demangled name, defining line
    and execution count. That granularity is the point of this report: gcovr's own HTML
    shows per-file percentages but never lists which functions were missed.
    """
    data = json.loads((out_dir / "coverage.json").read_text())
    summary = json.loads((out_dir / "summary.json").read_text())

    modules = {}
    uncovered = {}
    fn_total = fn_covered = 0
    for entry in data["files"]:
        path = entry["file"]
        module = module_of(path)
        bucket = modules.setdefault(module, {"fn_total": 0, "fn_covered": 0,
                                             "line_total": 0, "line_covered": 0})
        for line in entry.get("lines", []):
            bucket["line_total"] += 1
            if line.get("count", 0) > 0: bucket["line_covered"] += 1
        missed = []
        for fn in entry.get("functions", []):
            bucket["fn_total"] += 1
            fn_total += 1
            if fn.get("execution_count", 0) > 0:
                bucket["fn_covered"] += 1
                fn_covered += 1
            else:
                missed.append(fn)
        if missed: uncovered[path] = missed

    lines = []
    lines.append("# Garnet function-level coverage report")
    lines.append("")
    lines.append(f"- Generated (UTC): {datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%d %H:%M:%S')}")
    lines.append(f"- Revision: {git_describe()}")
    lines.append(f"- Source: {out_dir.parent.name} (`build.py c`, gcov, -O0)")
    lines.append("")
    lines.append("## Totals")
    lines.append("")
    lines.append("| metric | covered | total | % |")
    lines.append("| --- | --- | --- | --- |")
    for metric, label in (("line", "lines"), ("function", "functions"), ("branch", "branches")):
        total = summary.get(f"{metric}_total") or 0
        covered = summary.get(f"{metric}_covered") or 0
        pct = summary.get(f"{metric}_percent")
        pct = percent(covered, total) if pct is None else pct
        lines.append(f"| {label} | {covered} | {total} | {pct:.2f}% |")
    lines.append("")
    lines.append(f"Function records below are counted from coverage.json: {fn_covered}/{fn_total} "
                 f"({percent(fn_covered, fn_total):.2f}%) executed at least once.")
    lines.append("")
    lines.append("## Function coverage by module")
    lines.append("")
    lines.append("Sorted worst first.")
    lines.append("")
    lines.append("| module | functions | covered | % | lines % |")
    lines.append("| --- | --- | --- | --- | --- |")
    for module, bucket in sorted(modules.items(),
                                 key = lambda kv: percent(kv[1]["fn_covered"], kv[1]["fn_total"])):
        lines.append(f"| `{module}` | {bucket['fn_total']} | {bucket['fn_covered']} "
                     f"| {percent(bucket['fn_covered'], bucket['fn_total']):.2f}% "
                     f"| {percent(bucket['line_covered'], bucket['line_total']):.2f}% |")
    lines.append("")
    lines.append(f"## Uncovered functions ({sum(len(v) for v in uncovered.values())} in "
                 f"{len(uncovered)} files)")
    lines.append("")
    if not uncovered:
        lines.append("_Every function in scope was executed at least once._")
    for path in sorted(uncovered):
        lines.append(f"### `{path}`")
        lines.append("")
        for fn in sorted(uncovered[path], key = lambda f: f.get("lineno", 0)):
            name = fn.get("demangled_name") or fn.get("name") or "<unknown>"
            lines.append(f"- line {fn.get('lineno', '?')}: `{name}`")
        lines.append("")

    report = out_dir / "functions.md"
    report.write_text("\n".join(lines))
    utils.logi(f"function-level report: {report}")
    return percent(fn_covered, fn_total)

def main(argv = None):
    ap = argparse.ArgumentParser(description = __doc__,
                                 formatter_class = argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--no-build", action = "store_true", help="Skip the build step and report on the existing coverage build.")
    ap.add_argument("--clang", action = "store_true", help="Use the clang coverage build instead of gcc.")
    ap.add_argument("-b", dest = "build_dir", default = os.environ.get("GARNET_BUILD_DIR", "build"), help="Build output folder.")
    ap.add_argument("-o", dest = "output", default = None, help="Report output folder. Defaults to <coverage build dir>/coverage.")
    ap.add_argument("--fail-under", type = float, default = None, metavar = "PERCENT", help="Exit non-zero when function coverage falls below this percentage.")
    ap.add_argument("test_args", nargs = "*", help="Extra arguments forwarded to the test binaries.")
    args = ap.parse_args(argv)

    if "Linux" != platform.system():
        utils.rip(f"code coverage is only supported on Linux. Detected: {platform.system()}.")

    build_dir = coverage_build_dir(args.build_dir, args.clang)
    out_dir = pathlib.Path(args.output) if args.output else build_dir / "coverage"
    if not out_dir.is_absolute(): out_dir = ROOT_FOLDER / out_dir

    if not args.no_build:
        build_variant(args.build_dir, args.clang)

    require_instrumented(build_dir)
    out_dir.mkdir(parents = True, exist_ok = True)
    clear_gcda(build_dir)

    failure = run_tests(args.test_args)
    if failure is not None:
        utils.loge(f"a test binary failed: {failure}. Reporting coverage from the tests that did run.")

    run_gcovr(build_dir, out_dir, args.clang)
    function_percent = write_function_report(out_dir)

    print()
    utils.logi(f"HTML report      : {out_dir / 'index.html'}")
    utils.logi(f"function report  : {out_dir / 'functions.md'}")
    utils.logi(f"raw gcovr JSON   : {out_dir / 'coverage.json'}")
    utils.logi(f"function coverage: {function_percent:.2f}%")

    if args.fail_under is not None and function_percent < args.fail_under:
        utils.loge(f"function coverage {function_percent:.2f}% is below the required {args.fail_under:.2f}%.")
        return 1
    return 1 if failure is not None else 0

if __name__ == "__main__":
    sys.exit(main())

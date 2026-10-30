#!/usr/bin/python3
from pdb import run
import sys, subprocess, re, argparse, platform, pathlib, os
import importlib; utils = importlib.import_module("garnet-utils")

root_folder = utils.get_root_folder()

def run_all_tests(args, variants = None):
    utils.run_the_latest_binary("build/{variant}/bin/GNtest-unit-tests", args.test_args, check=True, cwd=root_folder, variants=variants)
    utils.run_the_latest_binary("build/{variant}/bin/GNtest-internal", args.test_args, check=True, cwd=root_folder, variants=variants)
    utils.run_the_latest_binary("build/{variant}/bin/GNsample-rdg2-open-graph-simple-triangle", ["t"], check=True, cwd=root_folder, variants=variants)
    # gpu2 samples double as smoke tests: they verify their results internally and exit non-zero on failure.
    utils.run_the_latest_binary("build/{variant}/bin/GNsample-gpu2-simple-copy", [], check=True, cwd=root_folder, variants=variants)
    utils.run_the_latest_binary("build/{variant}/bin/GNsample-gpu2-simple-compute", [], check=True, cwd=root_folder, variants=variants)

def run_style_check():
    print("Checking code styles...", end="", flush=True)

    # calling format-all-sources.py to verify that all sources are properly formatted.
    result= subprocess.run([sys.executable, pathlib.Path(os.path.realpath(__file__)).parent / "format-all-sources.py", "-dqn"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    err = result.stderr.decode("utf-8")
    if len(err) > 0:
        print(f"\nThe following changes are violating coding style standard:\n{err}")
        sys.exit(-1)

    # style check passed.
    print("OK")

def build_parser():
    ap = argparse.ArgumentParser()
    ap.add_argument("-l", action="store_true", help="Run code lint only. Skip rest.")
    ap.add_argument("-i", action="store_true", help="Run internal test only. Skip rest.")
    ap.add_argument("-u", action="store_true", help="Run unit test only. Skip rest.")
    ap.add_argument("-t", action="store_true", help="Run test only. Skip lint.")
    ap.add_argument("-c", "--coverage", action="store_true", help="Run tests against the coverage build variant and generate a function-level coverage report. Linux only.")
    ap.add_argument("test_args", nargs="*")
    return ap

def main(argv = None):
    args = build_parser().parse_args(argv)

    if args.coverage:
        # Delegating to a subprocess keeps code-coverage.py free to import this module for
        # the test list without creating an import cycle.
        script = pathlib.Path(os.path.realpath(__file__)).parent / "code-coverage.py"
        return subprocess.run([sys.executable, str(script)] + args.test_args, cwd=root_folder).returncode

    if args.l:
        run_style_check()
    elif args.i:
        utils.run_the_latest_binary("build/{variant}/bin/GNtest-internal", args.test_args, check=True, cwd=root_folder)
    elif args.u:
        utils.run_the_latest_binary("build/{variant}/bin/GNtest-unit-tests", args.test_args, check=True, cwd=root_folder)
    elif args.t:
        run_all_tests(args)
    else:
        # the default behavior when there are no arguments specified at command line.
        run_style_check()
        run_all_tests(args)
    return 0

if __name__ == "__main__":
    sys.exit(main())

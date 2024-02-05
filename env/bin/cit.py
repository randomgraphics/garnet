#!/usr/bin/python3
import sys, subprocess, re, argparse, platform, pathlib, os
import importlib; utils = importlib.import_module("garnet-utils")

def run_style_check():
    print("Checking code styles...", end="", flush=True)

    # calling format-all-sources.py to verify that all sources are properly formatted.
    result= subprocess.run([sys.executable, pathlib.Path(os.path.realpath(__file__)).parent / "format-all-sources.py", "-qn"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    err = result.stderr.decode("utf-8")
    if len(err) > 0:
        print(f"\nThe following changes are violating coding style standard:\n{err}")
        sys.exit(-1)

    # style check passed.
    print("OK")

# main
ap = argparse.ArgumentParser()
ap.add_argument("-l", action="store_true", help="Run code lint only. Skip test.")
ap.add_argument("-t", action="store_true", help="Run test only. Skip lint.")
ap.add_argument("test_args", nargs="*")
args = ap.parse_args()
if not args.t:
    run_style_check()
if not args.l:
    utils.run_the_latest_binary("build/{variant}/bin/GNtest-unit-tests", args.test_args)

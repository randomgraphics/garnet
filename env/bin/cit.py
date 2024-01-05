#!/usr/bin/python3
import sys, subprocess, re, argparse, platform
import importlib; utils = importlib.import_module("garnet-utils")

def run_style_check():
    print("Checking code styles...", end="")

    # get changes from git.
    root_dir = utils.get_root_folder()
    git_remote = subprocess.check_output(["git", "remote"], cwd=root_dir).decode(sys.stdout.encoding).strip()
    diff = subprocess.check_output(["git", "diff", "-U0", "--no-color", git_remote + "/main", "--", ":!*3rd-party*"], cwd=root_dir)

    # determine the clang-format-diff command line
    diff_script = root_dir / "dev/bin/clang-format-diff.py"
    system = platform.system()
    if "Windows" == system:
        clang_format = root_dir / "dev/bin/clang-format-14.exe"
        cmdline = ["python.exe", str(diff_script.absolute()), "-p1", "-binary", str(clang_format.absolute())]
    elif "Darwin" == system:
        cmdline = [str(diff_script.absolute()), "-p1", "-binary", "clang-format-mp-14"]
    else:
        cmdline = [str(diff_script.absolute()), "-p1", "-binary", "clang-format-14"]

    # check coding style of the diff
    format_diff = subprocess.check_output(cmdline, input=diff, cwd=root_dir).decode("utf-8")
    if len(format_diff) > 0:
        utils.rip(f"The following changes are violating coding style standard:\n{format_diff}")

    # Done
    print("OK.")

# main
ap = argparse.ArgumentParser()
ap.add_argument("-l", action="store_true", help="Run code lint only. Skip test.")
ap.add_argument("-t", action="store_true", help="Run test only. Skip lint.")
ap.add_argument("test_args", nargs="*")
args = ap.parse_args()
# if not args.t:
#     run_style_check()
if not args.l:
    utils.run_the_latest_binary("build/{variant}/bin/GNtest-unit-tests", args.test_args)

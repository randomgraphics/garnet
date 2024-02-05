#!/usr/bin/python3

import sys, pathlib, subprocess, importlib, argparse, platform, concurrent.futures, threading
utils = importlib.import_module("garnet-utils")

ap = argparse.ArgumentParser()
ap.add_argument("-n", "--dry-run", dest="n", action="store_true", help="Dry run. Print format violations to stderr, if any.")
ap.add_argument("-q", action="store_true", help="Quiet mode. Mute stdout.")
ap.add_argument("-d", action="store_true", help="Only process files that are different than the master/main branch")
args = ap.parse_args()

# get the root directory of the code base
root_dir = utils.get_root_folder()

# Gather all GIT managed source files
patterns = ["*.h", "*.hpp", "*.inl", "*.c", "*.cpp", "*.java", "*.glsl", "*.frag", "*.vert", "*.comp"]
if args.d:
     cmdline = ["git", "diff", "--name-only", "origin/master", "--"] + patterns
     all_files = subprocess.check_output(cmdline, cwd=root_dir).decode("utf-8").splitlines()
else:
     cmdline = ["git", "ls-files"] + patterns
     all_files = subprocess.check_output(cmdline, cwd=root_dir).decode("utf-8").splitlines()

# Remove all 3rd party sources
def is_our_source(x):
     if x.find("3rdparty") >= 0: return False
     if x.find("3rd-party") >= 0: return False
     if x.find("function-macros.h") >= 0: return False # ignore this file since it is script generated and is super long.
     return True
our_sources = [x for x in all_files if is_our_source(x)]

# Determine clang-format binary
system = platform.system()
if "Windows" == system:
     clang_format = str(root_dir / "env/bin/clang-format-14.exe")
else:
     clang_format = "clang-format-14"

# create a lock to serialize output
lock = threading.Lock()

# function to format one file
def format_one_file(x):
     cmdline = [clang_format, ("--dry-run" if args.n else "-i"), x]
     if not args.q:
          with lock: print(' '.join(cmdline))
     ret = subprocess.run(cmdline, cwd=root_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
     out = ret.stdout.decode("utf-8").strip()
     if not args.q and len(out) > 0:
          with lock: print(out)
     # always print errors to stderr
     err = ret.stderr.decode("utf-8").strip()
     if len(err) > 0:
          with lock: sys.stderr.write(err)

# run clang-format on all files in parallel
# for x in our_sources: format_one_file(x)
with concurrent.futures.ThreadPoolExecutor() as executor:
     futures = {executor.submit(format_one_file, x): x for x in our_sources}
     concurrent.futures.wait(futures)

#!/usr/bin/python3

import sys, pathlib, subprocess
import utils

# get the root directory of the code base
sdk_root_dir = utils.get_sdk_root_folder()
print(sdk_root_dir)

# Gather all GIT managed source files
all_files = subprocess.check_output(["git", "ls-files", "*.h", "*.hpp", "*.inl", "*.c", "*.cpp", "*.java", "*.glsl", "*.frag", "*.vert", "*.comp"], cwd=sdk_root_dir).decode("utf-8").splitlines()

# Remove all 3rd party sources
def is_our_source(x):
     if x.find("3rdparty") >= 0: return False
     if x.find("catch.hpp") >= 0: return False
     return True
our_sources = [x for x in all_files if is_our_source(x)]

# run clang-format-12 on all of them
for x in our_sources:
    cmdline = ["clang-format-12", "-i", x]
    print(' '.join(cmdline))
    subprocess.check_call(cmdline, cwd=sdk_root_dir)

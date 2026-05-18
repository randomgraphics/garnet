---
name: garnet-build-test
description: Build, test, lint, and format Garnet through the project wrappers. Use when compiling, running CIT, checking formatting, or verifying changes.
---

# Garnet Build, Test, and Format

Always use project wrappers, not raw CMake, for normal development.

## Environment

```bash
source env/garnet.rc
```

On Windows:

```powershell
. env\garnet.ps1
```

Run `.py` scripts through `python.exe` on Windows.

## Build

```bash
build.py d          # debug
build.py p          # profile / RelWithDebInfo
build.py r          # release
build.py c          # clean
build.py -c d       # configure only
build.py -C d       # build only
build.py --clang d  # Linux clang
build.py -a d       # Android
```

## CIT

```bash
env/bin/cit.py      # lint + tests
env/bin/cit.py -l   # lint/style only
env/bin/cit.py -t   # tests only
```

## Formatting

```bash
env/bin/format-all-sources.py -dqn  # dry-run check changed files
env/bin/format-all-sources.py -d    # format changed files
env/bin/format-all-sources.py       # format all tracked project sources
```


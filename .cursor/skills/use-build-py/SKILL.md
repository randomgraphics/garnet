---
name: use-build-py
description: Builds the garnet project using build.py instead of invoking cmake directly. Use when building the project, compiling, or when the user asks to build, make, or run cmake.
---

# Garnet build (build.py)

## When to use

Use `build.py` (or alias `b`) for any garnet build. Do not run cmake directly from the project root.

## Commands (run from repo root)

**Linux / WSL:** Source the environment first, then build.

```bash
source env/garnet.rc
build.py <variant> [options]
```

**Windows (PowerShell):** Use the garnet script and alias.

```powershell
. env\garnet.ps1
b <variant>
```

## Variants

| Short | Long     | CMake build type   |
|-------|----------|--------------------|
| `d`   | `debug`  | Debug              |
| `p`   | `profile`| RelWithDebInfo     |
| `r`   | `release`| Release            |

## Common options

- **Clang (Linux):** `build.py --clang d` (or `p`, `r`)
- **Android:** `build.py -a d` (or `-a p`, `-a r`). Requires `ANDROID_SDK_ROOT`, `ANDROID_NDK_ROOT`, `JAVA_HOME` (or use Android docker).

## Examples

```bash
source env/garnet.rc
build.py d              # Debug build (GCC on Linux)
build.py --clang d      # Debug with Clang
build.py -a d           # Android debug (needs NDK/SDK)
```

## Verification

Build output goes under `build/<system><suffix>/` (e.g. `build/linux.d/`). Success = cmake + compile finish without errors.

---
name: use-build-py
description: Builds the garnet project using build.py instead of invoking cmake directly. Use when building the project, compiling, or when the user asks to build, make, or run cmake.
---

# Use build.py to Build

## Rule

**Use `build.py` to build the project.** Do not invoke `cmake` (or `cmake --build`) directly.

The project provides `env/bin/build.py`, which configures and runs the build with the correct options, variants, and paths.

## How to Build

From the project root (garnet repo root):

```bash
./env/bin/build.py [variant] [target...]
```

Or:

```bash
python env/bin/build.py [variant] [target...]
```

- **variant**: e.g. `d`/`debug`, `r`/`release`, `p`/`profile`. Default or common is often `d` for debug.
- **target**: optional build target(s). Omit to build the default/all target.

## Do Not

- Do not run `cmake -S . -B build` or `cmake ..` to configure.
- Do not run `cmake --build <dir>` directly.
- Use `build.py` for any project build step so the same variant, generator, and paths are used consistently.

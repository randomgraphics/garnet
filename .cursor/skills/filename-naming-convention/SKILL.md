---
name: filename-naming-convention
description: Use lowercase words connected with hyphen for filenames. Use when creating or renaming source files, headers, scripts, or assets in the project, or when the user asks about file naming.
---

# Filename naming convention

## Rule

**Lowercase words connected with `-` (hyphen).**

- ✅ `solid-triangle.vert`, `solid-triangle-frag.spv.h`, `compile-shaders.py`, `simple-triangle.cpp`
- ❌ `solid_triangle.vert`, `SolidTriangle.vert`, `solidTriangle.vert`

## When to apply

- Creating new files under `src/` (e.g. rdg, sample, core).
- Renaming files to match the convention.
- Naming generated or referenced files in scripts (e.g. `compile_shaders.py` output names).

## Exceptions

- Third-party or vendored code keeps its original names.
- Extensions stay as-is (`.vert`, `.frag`, `.spv.h`, `.cpp`); only the base name follows the rule.

---
name: format-coding-style
description: Formats or checks C/C++ and project code style using env/bin/format-all-sources.py and clang-format. Use when the user asks to format code, fix style, run format check, or when lint fails due to formatting.
---

# Format / check coding style

## When to use

Use for formatting source files to project style or for checking style (e.g. when `cit.py -l` fails or the user asks to format code).

## Commands (run from repo root)

**Check only (no writes, fail if any file would change):**

```bash
env/bin/format-all-sources.py -dqn
```

- `-d`: only files different from origin/master  
- `-q`: quiet  
- `-n`: dry run (no writes; prints violations to stderr)

This is what `cit.py -l` runs. Use it to see which files would need formatting.

**Format in-place (fix style):**

```bash
env/bin/format-all-sources.py -d
```

- `-d`: only files different from origin/master (omit to format all project sources)
- Without `-n`: writes changes with clang-format `-i`.

**Format all tracked project sources (no -d):**

```bash
env/bin/format-all-sources.py
```

## Scope

- Includes: `*.h`, `*.hpp`, `*.inl`, `*.c`, `*.cpp`, and other listed extensions under git.
- Excludes: paths containing `3rdparty` or `3rd-party`.

## Lint vs format

- **Lint (style check):** `env/bin/cit.py -l` or `format-all-sources.py -dqn`. Use to verify CI style.
- **Format (fix):** `format-all-sources.py` or `format-all-sources.py -d`. Use after editing to fix style before committing.

## Verification

- Check: exit 0 and no stderr output = no violations.
- Format: exit 0; re-run `-dqn` to confirm clean.

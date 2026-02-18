---
name: run-cit
description: Runs garnet CI steps (lint and unit tests) via env/bin/cit.py. Use when the user asks to run CI, run tests, run lint, verify build, or after making code changes to validate.
---

# Run CIT (lint + test)

## When to use

Use `cit.py` to run lint and/or unit tests. Run after builds when a task requires "build+CI must pass" or "run cit".

## Commands (run from repo root)

**Full CI (lint + test):**

```bash
source env/garnet.rc   # Linux: ensures PATH and env
env/bin/cit.py
```

**Lint only (style check, no test run):**

```bash
env/bin/cit.py -l
```

**Test only (skip lint):**

```bash
env/bin/cit.py -t [test_args]
```

## What runs

- **Lint (`cit.py` or `cit.py -l`):** Runs `format-all-sources.py -dqn` (checks code style on changed files vs origin/master). Fails if any file would need formatting.
- **Test (`cit.py` or `cit.py -t`):** Runs `build/<variant>/bin/GNtest-unit-tests`. Uses the latest built variant.

## Prerequisites

- Lint: repo root, Python, format-all-sources.py (in env/bin). For "changed files" behavior, origin/master must exist.
- Test: a successful build must exist (e.g. `build.py d` already run) so `GNtest-unit-tests` is present.

## Verification

- Lint passes: exit code 0, "OK" or no style violations.
- Test passes: unit test binary runs and exits 0.

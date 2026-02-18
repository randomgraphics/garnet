---
name: use-submodule-shadow-copy
description: Ensures submodules are always used as shallow (shadow) copies with minimal history. Use when adding a new git submodule, updating .gitmodules, or when the user asks for shadow copy, shallow submodule, or minimal submodule clone.
---

# Submodule shadow copy (shallow)

## When to use

Use a **shadow copy** for every submodule: shallow clone, minimal history. Apply when adding or configuring submodules in this repo.

## Rule

**All submodules must be shallow.** Do not add or leave a submodule with full history.

## Adding a new submodule

1. Add the submodule as usual:
   ```bash
   git submodule add <url> <path>
   ```
2. **Immediately** set shallow in `.gitmodules` for that submodule:
   - Open `.gitmodules` and find the `[submodule "…"]` section for the new path.
   - Add `shallow = true` in that section (same level as `path` and `url`).

Example section after add:

```ini
[submodule "src/3rdparty/some-lib"]
	path = src/3rdparty/some-lib
	url = https://github.com/org/some-lib.git
	shallow = true
```

3. Optionally make the current clone shallow (if the repo was already cloned with full history):
   ```bash
   cd <submodule-path>
   git fetch --depth 1
   git checkout <desired-ref>
   cd -
   ```

## Existing submodules

- In `.gitmodules`, every `[submodule "…"]` section should contain `shallow = true`.
- If one is missing, add `shallow = true` so clone/update uses a shadow copy.

## Updating submodules

- `git submodule update --init --recursive` respects `shallow = true` in `.gitmodules` for initial clone.
- Do not run commands that explicitly fetch full history for a submodule (e.g. `git fetch --unshallow` in the submodule) unless the user asks.

## Verification

- `.gitmodules` has `shallow = true` for every submodule.
- After a fresh clone, `git submodule update --init --recursive` produces minimal submodule history.

---
name: garnet-submodules
description: Garnet submodule rules. Use when adding, updating, or reviewing git submodules.
---

# Garnet Submodules

All submodules must use shallow Git clones. Configure this in the parent
repository's `.gitmodules`; it does not create a separate "shadow copy".

When adding a submodule, update `.gitmodules` immediately:

```ini
[submodule "src/3rdparty/example"]
    path = src/3rdparty/example
    url = https://example.invalid/repo.git
    shallow = true
```

Rules:

- Every `.gitmodules` section needs `shallow = true`.
- Do not fetch full history unless the user asks.
- Owned or actively modified submodules should be developed on their own
  `wip/<topic>` branch.
- `garnet master` must only reference submodule `main`/`master` commits.

## Initialize a Fresh Clone or Worktree

After cloning the repository or adding a Git worktree, initialize dependencies
at the exact revisions recorded by the superproject:

```bash
git submodule update --init --recursive
```

Check that `git submodule status` shows no leading `-` (uninitialized) or `+`
(checked out at a different revision). A worktree may have empty submodule
directories when initialization is interrupted or its checkout is incomplete.
In that case, restore the pinned revisions explicitly:

```bash
git submodule update --init --checkout --force --jobs 1
git submodule status
```

Use `--recursive` on the first command when nested submodules are required.
Keep build output and generated files inside the worktree so the primary
checkout remains untouched. On remote hosts, confirm required SDK variables
such as `VULKAN_SDK` are present in the non-interactive SSH command environment;
interactive shell variables are not necessarily loaded for remote commands.

See [`agent/knowledge/coverage.md`](../../knowledge/coverage.md) for scan
scope and [`.gitmodules`](../../../.gitmodules) for the current submodule list.


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

See [`agent/knowledge/coverage.md`](../../knowledge/coverage.md) for scan
scope and [`.gitmodules`](../../../.gitmodules) for the current submodule list.


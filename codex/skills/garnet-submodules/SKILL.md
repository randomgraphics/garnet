---
name: garnet-submodules
description: Garnet submodule rules. Use when adding, updating, or reviewing git submodules.
---

# Garnet Submodules

All submodules must be shallow shadow copies.

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


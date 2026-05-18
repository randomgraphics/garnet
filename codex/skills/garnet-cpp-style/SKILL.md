---
name: garnet-cpp-style
description: Garnet C++ coding style and local conventions. Use when editing C/C++ files, adding headers, naming files, optimizing hot paths, or writing comments.
---

# Garnet C++ Style

- Keep edits small, buildable, and aligned with nearby code.
- Public API declarations need Doxygen comments.
- Comments explain why, invariants, ownership, API quirks, or tradeoffs. When
  code or build scripts use an unusual structure, workaround, ordering
  constraint, or non-obvious tradeoff, add a concise comment at that point that
  explains why it is necessary. Do not narrate obvious code.
- Enum values are `ALL_CAPS`.
- New project filenames use lowercase words with hyphens, e.g.
  `simple-triangle.cpp`.
- Public client code includes monolithic headers such as `GNrdg.h`, `GNrdg2.h`,
  `GNgpu2.h`, and `GNgfx.h`.
- In headers, include what declarations need and prefer forward declarations
  when a pointer/reference is enough.
- In `.cpp` files, avoid redundant includes already pulled in by the main
  included header.
- Preserve module-specific include ordering. In gpu2 Vulkan files,
  `vk-gpu-context.h` must precede headers that may include `rapid-vulkan.h`.
- Mark tiny header-defined functions and hot-path helpers `inline` when useful.
- Use `GN_UNLIKELY` for validation, error, and rare branches in hot code; use
  `GN_LIKELY` sparingly.

Example:

```cpp
if (!ptr) GN_UNLIKELY {
    GN_ERROR(sLogger)("invalid argument");
    return FAILED;
}
```


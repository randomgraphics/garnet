---
name: minimal-header-includes
description: Keeps C/C++ includes minimal by relying on transitive inclusion. If header A is included by header B and file C already includes B, C can use symbols from A without including A. Use when adding or reviewing #includes, editing headers or .cpp files, or when lint or review flags redundant includes.
---

# Minimal Header Inclusion

## Rule

**Transitive inclusion**: If header **A** is included by header **B**, and source file **C** already includes **B**, then **C** can reference everything from **A** without explicitly including **A**.

- Include only what you **directly** need.
- Do **not** add an include for a header that is already pulled in by another header you include.

## When to Apply

- Adding new `#include` directives in a `.cpp` or `.h` file.
- Reviewing or refactoring include lists.
- After lint or a code review flags redundant or unnecessary includes.

## Guidelines

1. **In a .cpp file**: Include the header that declares the types/functions you use directly. If that header already includes others (e.g. your project’s base headers), do not duplicate those includes.
2. **In a header**: Include only headers required for that header’s own declarations. Prefer forward declarations when a pointer/reference is enough to avoid pulling in heavy headers.
3. **Remove redundant includes**: If you remove the last use of a symbol from header X, remove `#include "X"` if X is only needed transitively via another included header.

## Example

- `foo.h` includes `bar.h`.
- `bar.h` includes `baz.h`.
- In `impl.cpp` you only use types from `bar.h` and `baz.h`.

Then `impl.cpp` should include `foo.h` (or `bar.h` if that’s the direct dependency), and **should not** add `#include "baz.h"` just because it uses symbols from `baz.h`—they are already available via `foo.h` → `bar.h` → `baz.h`.

## Exceptions

- If a symbol is only guaranteed by a particular standard or platform header (e.g. you rely on `<cstddef>` for `std::size_t`), including that header in the file that uses it can be acceptable for clarity or portability, even if it might come in transitively. Prefer the minimal set unless the team explicitly documents otherwise.
- When a header’s transitive includes are unstable or not part of the public contract, the team may choose to include the needed header explicitly for stability; follow project rules in that case.

---
name: use-gn-unlikely
description: Uses GN_UNLIKELY (and GN_LIKELY when appropriate) to hint branch prediction and reduce performance penalty of conditionals. Use when writing or editing C/C++ in this codebase, especially for error checks, parameter validation, and hot-path conditionals.
---

# Use GN_UNLIKELY for Branch Hints

## Rule

**Annotate the unlikely branch with `GN_UNLIKELY`** so the compiler can optimize for the common case and reduce branch misprediction cost.

Use **`GN_LIKELY`** on the branch that is expected to be taken in hot paths (e.g. success path after a quick check).

## When to Use GN_UNLIKELY

- **Error checks**: `if (!ptr) GN_UNLIKELY { ... }`, `if (result != ok) GN_UNLIKELY return FAILED;`
- **Parameter / precondition checks**: `if (index >= count) GN_UNLIKELY { ... }`, `if (!a) GN_UNLIKELY return;`
- **Validation failures**: conditions that indicate invalid input, null handles, or out-of-range values
- **Recovery or fallback paths**: branches that run only on failure or rare cases

## When to Use GN_LIKELY

- **Hot-path success**: `if (type == T::TYPE_ID) GN_LIKELY return static_cast<T*>(this);`
- **Expected-true conditions** in performance-sensitive code where the condition is almost always true

## Syntax

- Place the hint on the condition: `if (condition) GN_UNLIKELY { body }` or `if (condition) GN_UNLIKELY return x;`
- For single-statement bodies, the hint goes before the statement: `if (!p) GN_UNLIKELY return nullptr;`

## Example

**Before:**
```cpp
if (!a) {
    GN_ERROR(sLogger)("invalid arguments");
    return FAILED;
}
```

**After:**
```cpp
if (!a) GN_UNLIKELY {
    GN_ERROR(sLogger)("invalid arguments");
    return FAILED;
}
```

## Note

Macros are defined in `garnet/base/basicDefines.h` (typically included via project headers). Prefer `GN_UNLIKELY` on error/validation branches; use `GN_LIKELY` sparingly on the dominant branch in hot code.

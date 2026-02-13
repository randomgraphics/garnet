---
name: inline-small-functions
description: Marks small, hot-path functions as inline for best performance in C/C++. Use when writing or reviewing performance-sensitive code, getters/setters, one-liners, or when the user asks to optimize or inline functions.
---

# Inline Small Functions for Best Perf

## Rule

**Mark small functions as `inline`** so the compiler can eliminate call overhead and optimize across the call boundary.

- Prefer `inline` for: one-liners, trivial getters/setters, small predicates, hot-path helpers.
- In headers: use `inline` on function definitions that are defined in the header (so multiple TUs can include them without ODR issues).
- In .cpp: use `inline` on small functions that are only used in that TU, to encourage inlining.

## When to Use

1. **Header-defined functions**: Any function whose definition lives in a header should be `inline` (or in an anonymous namespace in the header) to avoid multiple-definition errors and to allow inlining.
2. **Small accessors**: Getters/setters that return a member or do a simple assignment are good candidates.
3. **Hot path**: Small functions called in loops or critical paths benefit from inlining.
4. **Tiny helpers**: Functions that are a few statements and called from few call sites.

## Examples

```cpp
// Header: inline so definition can appear in header and still inline at call sites
inline int Foo::getValue() const { return value_; }
inline void Foo::setValue(int v) { value_ = v; }

// Small predicate
inline bool isReady() const { return state_ == Ready; }
```

## Caveats

- Do **not** force `inline` on large or rarely-called functions; it can increase code size and hurt I-cache.
- The compiler may ignore `inline` for optimization; use it as a hint. For forced inlining, use compiler-specific attributes (e.g. `__attribute__((always_inline))`) only when profiling shows a need.
- In C++, member functions defined inside the class body are implicitly inline; no need to repeat `inline` there.

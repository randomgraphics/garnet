---
name: minimize-includes
description: Reduces redundant #include directives in C/C++ by omitting headers that are already included transitively. Use when editing .cpp/.cc/.cxx files, adding or reviewing includes, or when the user asks to minimize or clean up includes.
---

# Minimize Include Headers

## Rule

**Do not include a header B in a .cpp file if B is already included transitively** via another header A that the .cpp already includes.

- If `A.h` includes `B.h`, and the .cpp file includes `A.h`, then the .cpp file should **not** explicitly include `B.h`.
- Include only the minimal set of headers needed: prefer including the header that directly declares what you use; skip any header that another included header already pulls in.

## When Adding Includes

1. Include the header that declares the symbols you use (types, functions, etc.).
2. Do **not** add an include for a header that is already brought in by one of those includes.
3. When in doubt, include the "main" or "public" header for the feature you use; avoid adding its dependencies explicitly.

## When Cleaning Up a File

1. Remove any `#include` that is redundant: the same declarations are already available through another included header.
2. To verify, consider the include graph: if every symbol used from B is available via A (because A includes B), then remove `#include "B.h"` (or `#include <B.h>`).

## Example

**Before (redundant):**
```cpp
// myfile.cpp
#include "widget.h"   // widget.h already includes "gadget.h"
#include "gadget.h"  // redundant
```

**After (minimal):**
```cpp
// myfile.cpp
#include "widget.h"   // sufficient; gadget is available via widget.h
```

## Caveat

Transitive includes can change if a header (e.g. `A.h`) is refactored and stops including `B.h`. When removing an include, prefer removing one that is clearly redundant (e.g. the .cpp only uses types from A, and A's public API includes B). When a build fails after cleanup, re-add the include for the missing declaration.

---
name: garnet-public-interface
description: Design or review Garnet module APIs and public headers. Use when adding or changing exported classes, members, factories, or module boundaries, or when reviewing implementation details exposed through GNXXX.h and its included subheaders.
---

# Garnet Public Interfaces

A public header is a contract for callers in other modules that cannot see this
module's implementation. This includes each monolithic `GNXXX.h` and every
subheader it includes, even when a declaration is private or protected in C++.

## Decide what belongs in the contract

For each type, function, or data member, identify the external module that needs
to name it, call it, supply it, or implement it. Search actual callers before
changing an existing API. Module-local tests do not count as external consumers.
Keep only the declarations required for those interactions in public headers.

Move implementation-only types, members, storage, algorithms, caches, backend
adapters, and helpers into the module's `.cpp` files or private headers under
`src/core/<module>/`. Merely changing a public-header member to `private` does not
hide an implementation detail from the header. Do not add accessors for internal
state solely to replace exposed fields.

Public descriptors and value types are appropriate when another module actually
constructs or consumes that data. Preserve useful input/output schemas; this rule
does not require turning every vector or arithmetic value into a virtual object.

## Prefer abstract interfaces

Prefer pure virtual behavioral interfaces with factories returning owning refs.
Keep concrete classes and their data members in the implementation. Public
constructors, protected extension hooks, and inline/template definitions belong
in the header only when external construction, derivation, or C++ compilation
requires them. Explain non-obvious exceptions where they occur.

An opaque collection should expose only the operations its external users need.
Do not publish its storage, child/parent relationships, graph edges, traversal,
or scheduling machinery when these are the module's responsibility. A generic
consumer should accept the opaque contract without naming concrete contents.

## Review and verify

Check transitive includes as well as the top-level header. Update real callers
when narrowing a contract, and keep tests focused on observable results. Internal
tests may use a private module header when internal inspection is necessary;
do not retain public implementation details for test convenience.

Apply this to the requested change and related API cleanup. Avoid sweeping
unrelated module rewrites merely because the rule could also improve them.

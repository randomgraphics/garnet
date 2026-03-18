/// Unit tests for RuntimeType RTTI multi-inheritance:
///
///   A -\          /-> D
///        -> C ->
///   B -/          \-> E
///
/// A and B are RTTI roots (no RTTI bases). C has both A and B as RTTI bases.
/// D and E each have C as their RTTI base.

#include <catch2/catch_test_macros.hpp>
#include <garnet/rdg/rtti.h>

using namespace GN::rdg;

namespace {

struct A : RuntimeType {
    GN_RDG_REGISTER_RUNTIME_TYPE();
    int a = 0;

protected:
    A(const TypeInfo & ti): RuntimeType(ti) {}

public:
    A(): A(TYPE_INFO()) {}
};

struct B : RuntimeType {
    GN_RDG_REGISTER_RUNTIME_TYPE();
    int b = 0;

protected:
    B(const TypeInfo & ti): RuntimeType(ti) {}

public:
    B(): B(TYPE_INFO()) {}
};

/// C inherits from RuntimeType (so it can be held as RuntimeType*),
/// and from A and B as plain C++ mixins.
/// Its RTTI graph lists A and B as bases.
struct C : A, B {
    GN_RDG_REGISTER_RUNTIME_TYPE(A, B);

protected:
    explicit C(const TypeInfo & ti): A(ti), B(ti) {}

public:
    C(): A(TYPE_INFO()), B(TYPE_INFO()) {}
};

struct D : C {
    GN_RDG_REGISTER_RUNTIME_TYPE(C);
    int d = 0;

    D(): D(TYPE_INFO()) {}

protected:
    D(const TypeInfo & ti): C(ti) {}
};

struct E : C {
    GN_RDG_REGISTER_RUNTIME_TYPE(C);
    int e = 0;

    E(): E(TYPE_INFO()) {}

protected:
    E(const TypeInfo & ti): C(ti) {}
};

} // namespace

TEST_CASE("RTTI multi-inherit: C is kind of A and B", "[rdg][rtti]") {
    C c;
    CHECK(c.isKindOf<C>());
    CHECK(c.isKindOf<A>());
    CHECK(c.isKindOf<B>());
    CHECK_FALSE(c.isKindOf<D>());
    CHECK_FALSE(c.isKindOf<E>());
}

TEST_CASE("RTTI multi-inherit: D inherits A and B through C", "[rdg][rtti]") {
    D d;
    CHECK(d.isKindOf<D>());
    CHECK(d.isKindOf<C>());
    CHECK(d.isKindOf<A>());
    CHECK(d.isKindOf<B>());
    CHECK_FALSE(d.isKindOf<E>());
}

TEST_CASE("RTTI multi-inherit: E inherits A and B through C", "[rdg][rtti]") {
    E e;
    CHECK(e.isKindOf<E>());
    CHECK(e.isKindOf<C>());
    CHECK(e.isKindOf<A>());
    CHECK(e.isKindOf<B>());
    CHECK_FALSE(e.isKindOf<D>());
}

TEST_CASE("RTTI multi-inherit: A and B are unrelated", "[rdg][rtti]") {
    CHECK_FALSE(A::TYPE_INFO().isDerivedFrom(B::TYPE_INFO()));
    CHECK_FALSE(B::TYPE_INFO().isDerivedFrom(A::TYPE_INFO()));
}

TEST_CASE("RTTI multi-inherit: RuntimeType::cast through RuntimeType pointer", "[rdg][rtti]") {
    D   d;
    A * a = &d;

    CHECK(RuntimeType::cast<D>(a) != nullptr);
    CHECK(RuntimeType::cast<C>(a) != nullptr);
    CHECK(RuntimeType::cast<E>(a) == nullptr);

    E   e;
    B * b = &e;

    CHECK(RuntimeType::cast<E>(b) != nullptr);
    CHECK(RuntimeType::cast<C>(b) != nullptr);
    CHECK(RuntimeType::cast<D>(b) == nullptr);
}

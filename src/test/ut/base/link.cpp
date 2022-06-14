#include "../testCommon.h"

class LinkTest : public CxxTest::TestSuite {
public:
    void testConstructor() {
        using namespace GN;

        DoubleLink a;

        DoubleLink * null = NULL;

        TS_ASSERT_EQUALS(a.prev, null);
        TS_ASSERT_EQUALS(a.next, null);
    }

    void testInsertAfter() {
        using namespace GN;

        DoubleLink a, b, c;

        DoubleLink * null = NULL;

        // c->b->a
        TS_ASSERT(a.linkAfter(&c));
        TS_ASSERT(b.linkAfter(&c));

        // Expected failures
        TS_ASSERT(!a.linkAfter(NULL));
        TS_ASSERT(!a.linkAfter(&a));
        TS_ASSERT(!a.linkAfter(&c));

        TS_ASSERT_EQUALS(c.prev, null);
        TS_ASSERT_EQUALS(c.next, &b);

        TS_ASSERT_EQUALS(b.prev, &c);
        TS_ASSERT_EQUALS(b.next, &a);

        TS_ASSERT_EQUALS(a.prev, &b);
        TS_ASSERT_EQUALS(a.next, null);

        DoubleLink d, e;

        // e->d
        TS_ASSERT(d.linkAfter(&e));

        // e->d->c->b->a
        TS_ASSERT(!c.linkAfter(&e));
        TS_ASSERT(c.linkAfter(&d));

        TS_ASSERT_EQUALS(e.prev, null);
        TS_ASSERT_EQUALS(e.next, &d);

        TS_ASSERT_EQUALS(d.prev, &e);
        TS_ASSERT_EQUALS(d.next, &c);

        TS_ASSERT_EQUALS(c.prev, &d);
        TS_ASSERT_EQUALS(c.next, &b);
    }

    void testInsertBefore() {
        using namespace GN;

        DoubleLink a, b, c;

        DoubleLink * null = NULL;

        // a->b->c
        TS_ASSERT(a.linkBefore(&c));
        TS_ASSERT(b.linkBefore(&c));

        // Expected failures
        TS_ASSERT(!a.linkBefore(null));
        TS_ASSERT(!a.linkBefore(&a));
        TS_ASSERT(!a.linkBefore(&c));

        TS_ASSERT_EQUALS(a.prev, null);
        TS_ASSERT_EQUALS(a.next, &b);

        TS_ASSERT_EQUALS(b.prev, &a);
        TS_ASSERT_EQUALS(b.next, &c);

        TS_ASSERT_EQUALS(c.prev, &b);
        TS_ASSERT_EQUALS(c.next, null);

        DoubleLink d, e;

        // d->e
        TS_ASSERT(d.linkBefore(&e));

        // a->b->c->d->e
        TS_ASSERT(!c.linkBefore(&e));
        TS_ASSERT(c.linkBefore(&d));

        TS_ASSERT_EQUALS(c.prev, &b);
        TS_ASSERT_EQUALS(c.next, &d);

        TS_ASSERT_EQUALS(d.prev, &c);
        TS_ASSERT_EQUALS(d.next, &e);

        TS_ASSERT_EQUALS(e.prev, &d);
        TS_ASSERT_EQUALS(e.next, null);
    }

    void testDetach() {
        using namespace GN;

        DoubleLink a, b, c;

        DoubleLink * null = NULL;

        // a->b->c
        TS_ASSERT(b.linkBefore(&c));
        TS_ASSERT(a.linkBefore(&b));

        b.detach();

        // b should be alone.
        TS_ASSERT_EQUALS(b.prev, null);
        TS_ASSERT_EQUALS(b.next, null);

        // Now, it should be: a->c
        TS_ASSERT_EQUALS(a.prev, null);
        TS_ASSERT_EQUALS(a.next, &c);
        TS_ASSERT_EQUALS(c.prev, &a);
        TS_ASSERT_EQUALS(c.next, null);

        a.detach();

        // Both a and c should be alone.
        TS_ASSERT_EQUALS(a.prev, null);
        TS_ASSERT_EQUALS(a.next, null);
        TS_ASSERT_EQUALS(c.prev, null);
        TS_ASSERT_EQUALS(c.next, null);
    }

    void testLinkLoop() {
        using namespace GN;

        DoubleLink a, b, c;

        DoubleLink * null = NULL;

        // a->c
        TS_ASSERT(a.linkBefore(&c));
        TS_ASSERT_EQUALS(a.prev, null);
        TS_ASSERT_EQUALS(a.next, &c);
        TS_ASSERT_EQUALS(c.prev, &a);
        TS_ASSERT_EQUALS(c.next, null);

        // a->c->a
        TS_ASSERT(c.linkBefore(&a));
        TS_ASSERT_EQUALS(a.prev, &c);
        TS_ASSERT_EQUALS(a.next, &c);
        TS_ASSERT_EQUALS(c.prev, &a);
        TS_ASSERT_EQUALS(c.next, &a);

        // a->b->c->a
        TS_ASSERT(b.linkBefore(&c));
        TS_ASSERT_EQUALS(a.prev, &c);
        TS_ASSERT_EQUALS(a.next, &b);
        TS_ASSERT_EQUALS(b.prev, &a);
        TS_ASSERT_EQUALS(b.next, &c);
        TS_ASSERT_EQUALS(c.prev, &b);
        TS_ASSERT_EQUALS(c.next, &a);

        // detach a. should be b->c->b
        a.detach();
        TS_ASSERT_EQUALS(a.prev, null);
        TS_ASSERT_EQUALS(a.next, null);
        TS_ASSERT_EQUALS(b.prev, &c);
        TS_ASSERT_EQUALS(b.next, &c);
        TS_ASSERT_EQUALS(c.prev, &b);
        TS_ASSERT_EQUALS(c.next, &b);

        // detach b.
        b.detach();
        TS_ASSERT_EQUALS(b.prev, null);
        TS_ASSERT_EQUALS(b.next, null);
        TS_ASSERT_EQUALS(c.prev, null);
        TS_ASSERT_EQUALS(c.next, null);
    }
};

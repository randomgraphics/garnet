#include "../testCommon.h"

class ListTest : public CxxTest::TestSuite
{
public:

    void testConstructor()
    {
        using namespace GN;

        DoubleLink a;

        TS_ASSERT_EQUALS( a.prev, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( a.next, (DoubleLink*)NULL );
    }

    void testInsertAfter()
    {
        using namespace GN;

        DoubleLink a, b, c;

        // c->b->a
        TS_ASSERT( a.linkAfter( &c ) );
        TS_ASSERT( b.linkAfter( &c ) );

        // Expected failures
        TS_ASSERT( !a.linkAfter( NULL ) );
        TS_ASSERT( !a.linkAfter( &a ) );
        TS_ASSERT( !a.linkAfter( &c ) );

        TS_ASSERT_EQUALS( c.prev, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( c.next, &b );

        TS_ASSERT_EQUALS( b.prev, &c );
        TS_ASSERT_EQUALS( b.next, &a );

        TS_ASSERT_EQUALS( a.prev, &b );
        TS_ASSERT_EQUALS( a.next, (DoubleLink*)NULL );

        DoubleLink d, e;

        // e->d
        TS_ASSERT( d.linkAfter( &e ) );

        // e->d->c->b->a
        TS_ASSERT( !c.linkAfter( &e ) );
        TS_ASSERT( c.linkAfter( &d ) );

        TS_ASSERT_EQUALS( e.prev, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( e.next, &d );

        TS_ASSERT_EQUALS( d.prev, &e );
        TS_ASSERT_EQUALS( d.next, &c );

        TS_ASSERT_EQUALS( c.prev, &d );
        TS_ASSERT_EQUALS( c.next, &b );
    }

    void testInsertBefore()
    {
        using namespace GN;

        DoubleLink a, b, c;

        DynaArray<DoubleLink*> array;

        // a->b->c
        TS_ASSERT( a.linkBefore( &c ) );
        TS_ASSERT( b.linkBefore( &c ) );

        // Expected failures
        TS_ASSERT( !a.linkBefore( (DoubleLink*)NULL ) );
        TS_ASSERT( !a.linkBefore( &a ) );
        TS_ASSERT( !a.linkBefore( &c ) );

        TS_ASSERT_EQUALS( a.prev, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( a.next, &b );

        TS_ASSERT_EQUALS( b.prev, &a );
        TS_ASSERT_EQUALS( b.next, &c );

        TS_ASSERT_EQUALS( c.prev, &b );
        TS_ASSERT_EQUALS( c.next, (DoubleLink*)NULL );

        DoubleLink d, e;

        // d->e
        TS_ASSERT( d.linkBefore( &e ) );

        // a->b->c->d->e
        TS_ASSERT( !c.linkBefore( &e ) );
        TS_ASSERT( c.linkBefore( &d ) );

        TS_ASSERT_EQUALS( c.prev, &b );
        TS_ASSERT_EQUALS( c.next, &d );

        TS_ASSERT_EQUALS( d.prev, &c );
        TS_ASSERT_EQUALS( d.next, &e );

        TS_ASSERT_EQUALS( e.prev, &d );
        TS_ASSERT_EQUALS( e.next, (DoubleLink*)NULL );
    }

    void testDetach()
    {
        using namespace GN;

        DoubleLink a, b, c;

        DynaArray<DoubleLink*> array;

        // a->b->c
        TS_ASSERT( b.linkBefore( &c ) );
        TS_ASSERT( a.linkBefore( &b ) );

        b.detach();

        // b should be alone.
        TS_ASSERT_EQUALS( b.prev, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( b.next, (DoubleLink*)NULL );

        // Now, it should be: a->c
        TS_ASSERT_EQUALS( a.prev, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( a.next, &c );
        TS_ASSERT_EQUALS( c.prev, &a );
        TS_ASSERT_EQUALS( c.next, (DoubleLink*)NULL );

        a.detach();

        // Both a and c should be alone.
        TS_ASSERT_EQUALS( a.prev, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( a.next, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( c.prev, (DoubleLink*)NULL );
        TS_ASSERT_EQUALS( c.next, (DoubleLink*)NULL );
    }
};


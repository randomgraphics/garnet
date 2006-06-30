#include "../testCommon.h"

class StackArrayTest : public CxxTest::TestSuite
{
public:

    void testAppend()
    {
        using namespace GN;
        StackArray<int,2> a;
        a.append( 1 );
        a.append( 2 );
        a.append( 3 ); // should do nothing
        TS_ASSERT_EQUALS( 2, a.size() );
        TS_ASSERT_EQUALS( 1, a.front() );
        TS_ASSERT_EQUALS( 2, a.back() );
    }

    void testInsert()
    {
        using namespace GN;
        StackArray<int,4> a;
        a.insert( 0, 1 ); // 1 (insert at head)
        a.insert( 1, 3 ); // 1, 3 (insert at tail)
        a.insert( 1, 2 ); // 1, 2, 3 (insert at middle)
        a.insert( 100, 0 ); // invalid position, should do nothing
        a.insert( 3, 4 ); // 1, 2, 3, 4 (insert at tail)
        a.insert( 4, 5 ); // already full, should do nothing
        TS_ASSERT_EQUALS( 4, a.size() );
        TS_ASSERT_EQUALS( 1, a[0] );
        TS_ASSERT_EQUALS( 2, a[1] );
        TS_ASSERT_EQUALS( 3, a[2] );
        TS_ASSERT_EQUALS( 4, a[3] );
    }

    void testErase()
    {
        using namespace GN;
        StackArray<int,4> a;
        a.append( 1 );
        a.append( 2 );
        a.append( 3 );
        a.append( 4 );

        // erase middle: 1, 2, 4
        a.erase( 2 ); 
        TS_ASSERT_EQUALS( 3, a.size() );
        TS_ASSERT_EQUALS( 4, a[2] );

        // erase head: 2, 4
        a.erase( 0 ); 
        TS_ASSERT_EQUALS( 2, a.size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // erase tail: 2
        a.erase( 1 ); 
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // invalid position
        a.erase( 1 );
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // erase to empty
        a.erase( 0 );
        TS_ASSERT_EQUALS( 0, a.size() );
        
        // already empty
        a.erase( 0 );
        TS_ASSERT_EQUALS( 0, a.size() );
    }

    void testResize()
    {
        using namespace GN;
        StackArray<int,4> a;

        a.resize( 2 );
        TS_ASSERT_EQUALS( 2, a.size() );

        a.resize( 4 );
        TS_ASSERT_EQUALS( 4, a.size() );

        a.resize( 5 );
        TS_ASSERT_EQUALS( 4, a.size() );

        a.resize( 2 );
        TS_ASSERT_EQUALS( 2, a.size() );
    }
};

class DynaArrayTest : public CxxTest::TestSuite
{
public:

    void testDefautlCtor()
    {
        using namespace GN;
        DynaArray<int> a;

        TS_ASSERT( a.empty() );
        TS_ASSERT_EQUALS( (int*)NULL, a.cptr() );
        TS_ASSERT_EQUALS( 0, a.size() );
    }

    void testCopyCtor()
    {
        using namespace GN;
        DynaArray<int> a( 1 );
        DynaArray<int> b( a );
        DynaArray<int> c;

        TS_ASSERT_EQUALS( 1, b.size() );
        TS_ASSERT_DIFFERS( a.cptr(), b.cptr() );

        c = b;
        TS_ASSERT_EQUALS( 1, c.size() );
        TS_ASSERT_DIFFERS( b.cptr(), c.cptr() );
    }

    void testAppend()
    {
        using namespace GN;
        DynaArray<int> a;

        a.append( 1 );
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 1, a.front() );
        TS_ASSERT_EQUALS( 1, a.back() );

        a.append( 3 );
        TS_ASSERT_EQUALS( 2, a.size() );
        TS_ASSERT_EQUALS( 1, a.front() );
        TS_ASSERT_EQUALS( 3, a.back() );

        DynaArray<int> b;
        b.append( a );
        TS_ASSERT_EQUALS( 2, b.size() );
        TS_ASSERT_EQUALS( 1, b.front() );
        TS_ASSERT_EQUALS( 3, b.back() );
    }

    void testBeginEnd()
    {
        using namespace GN;
        DynaArray<int> a;

        a.append( 1 );
        a.append( 2 );

        int * b = a.begin();
        int * e = a.end();

        TS_ASSERT_EQUALS( 1, *b );
        TS_ASSERT_EQUALS( 2, e-b );
    }

    void testInsert()
    {
        using namespace GN;
        DynaArray<int> a;

        a.insert( 0, 3 ); // 3
        a.insert( 0, 1 ); // 1, 3
        a.insert( 1, 2 ); // 1, 2, 3
        a.insert( 100, 4 ); // should do nothing
        a.insert( 3, 4 ); // 1, 2, 3, 4
        a.insert( 4, 5 ); // 1, 2, 3, 4, 5

        TS_ASSERT_EQUALS( 5, a.size() );
        TS_ASSERT_EQUALS( 1, a[0] );
        TS_ASSERT_EQUALS( 2, a[1] );
        TS_ASSERT_EQUALS( 3, a[2] );
        TS_ASSERT_EQUALS( 4, a[3] );
        TS_ASSERT_EQUALS( 5, a[4] );
    }

    void testErase()
    {
        using namespace GN;
        DynaArray<int> a;

        a.append( 1 );
        a.append( 2 );
        a.append( 3 );
        a.append( 4 );
        a.append( 5 );

        a.erase( 100 ); // should do nothing
        TS_ASSERT_EQUALS( 5, a.size() );
        TS_ASSERT_EQUALS( 5, a[4] );

        a.erase( 4 ); // 1, 2, 3, 4
        TS_ASSERT_EQUALS( 4, a.size() );
        TS_ASSERT_EQUALS( 4, a[3] );

        a.erase( 3 ); // 1, 2, 3
        TS_ASSERT_EQUALS( 3, a.size() );
        TS_ASSERT_EQUALS( 3, a[2] );

        a.erase( 1 ); // 1, 3
        TS_ASSERT_EQUALS( 2, a.size() );
        TS_ASSERT_EQUALS( 1, a[0] );
        TS_ASSERT_EQUALS( 3, a[1] );

        a.erase( 0 ); // 3
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 3, a[0] );

        a.erase( 0 );
        TS_ASSERT_EQUALS( 0, a.size() );

        a.erase( 0 ); // this should do nothing
    }

    void testPopBack()
    {
        using namespace GN;
        DynaArray<int> a;
        a.append( 1 );
        a.append( 2 );
        a.popBack();
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 1, a.back() );

        a.popBack();
        TS_ASSERT( a.empty() );

        a.popBack(); // should do nothing
    }

    void testResize()
    {
        using namespace GN;
        DynaArray<int> a;

        a.resize( 2 );
        TS_ASSERT_EQUALS( 2, a.size() );

        a.resize( 4 );
        TS_ASSERT_EQUALS( 4, a.size() );
    }

    void testReserve()
    {
        using namespace GN;
        DynaArray<int> a;

        a.resize( 2 );
        TS_ASSERT_EQUALS( 2, a.size() );

        // reserve should not modify array size
        a.reserve( 1000 );
        TS_ASSERT_EQUALS( 2, a.size() );
    }
};

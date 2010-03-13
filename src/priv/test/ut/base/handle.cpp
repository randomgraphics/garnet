#include "../testCommon.h"


class HandleTest : public CxxTest::TestSuite
{
    struct TestItem
    {
        int * ptr;

        TestItem()
            : ptr( new int[100] )
        {
        }

        ~TestItem()
        {
            delete [] ptr;
        }
    };

public:

    void testCtor()
    {
        using namespace GN;

        HandleManager<TestItem> hm;

        size_t h = hm.NewHandle();
        TS_ASSERT( h );

        hm.Remove( h );

        TS_ASSERT( hm.Empty() );
    }

    void test1()
    {
        GN::HandleManager<int> hm;

        TS_ASSERT( hm.Empty() );
        TS_ASSERT_EQUALS( hm.Size(), 0 );
        TS_ASSERT_EQUALS( hm.First(), 0 );
        TS_ASSERT_EQUALS( hm.Next(0), 0 );

        size_t h1 = hm.Add( 1 );
        size_t h2 = hm.Add( 2 );
        size_t h3 = hm.Add( 3 );
        size_t h4 = hm.Add( 4 );
        size_t h5 = hm.Add( 5 );

        TS_ASSERT( !hm.Empty() );
        TS_ASSERT_EQUALS( hm.Size(), 5 );

        TS_ASSERT_EQUALS( hm.First(), h1 );
        TS_ASSERT_EQUALS( hm.Next(h1), h2 );
        TS_ASSERT_EQUALS( hm.Next(h2), h3 );
        TS_ASSERT_EQUALS( hm.Next(h3), h4 );
        TS_ASSERT_EQUALS( hm.Next(h4), h5 );
        TS_ASSERT_EQUALS( hm.Next(h5), 0 );
        TS_ASSERT_EQUALS( hm.Next(0), 0 );
        TS_ASSERT_EQUALS( hm.Next(100), 0 );

        TS_ASSERT_EQUALS( hm[h1], 1 );
        TS_ASSERT_EQUALS( hm[h2], 2 );
        TS_ASSERT_EQUALS( hm[h3], 3 );

        hm.Remove( 0 );
        hm.Remove( h1 );
        TS_ASSERT_EQUALS( hm.Size(), 4 );
        TS_ASSERT_EQUALS( hm.First(), h2 );
        TS_ASSERT_EQUALS( hm.Next(h2), h3 );

        hm.Remove( h3 );
        TS_ASSERT_EQUALS( hm.Size(), 3 );
        TS_ASSERT_EQUALS( hm.Next(h2), h4 );
        TS_ASSERT_EQUALS( hm.Next(h4), h5 );

        hm.Remove( h5 );
        TS_ASSERT_EQUALS( hm.Size(), 2 );
        TS_ASSERT_EQUALS( hm.Next(h4), 0 );

        // find
        TS_ASSERT_EQUALS( hm.Find( 1 ), 0 );
        TS_ASSERT_EQUALS( hm.Find( 2 ), h2 );

        // re-add
        h1 = hm.Add(1);
        TS_ASSERT( h1 );

        hm.Clear();
        TS_ASSERT( hm.Empty() );
        TS_ASSERT_EQUALS( hm.Size(), 0 );
        TS_ASSERT_EQUALS( hm.First(), 0 );
        TS_ASSERT_EQUALS( hm.Next(0), 0 );
    }

    void testCapacity()
    {
        GN::HandleManager<int> hm;
        hm.Reserve( 1 );
        TS_ASSERT_LESS_EQUALS( 1, hm.Capacity() );
        hm.Reserve( 1000 );
        TS_ASSERT_LESS_EQUALS( 1000, hm.Capacity() );
    }
};

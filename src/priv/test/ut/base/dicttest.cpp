#include "../testCommon.h"

class TestDict : public CxxTest::TestSuite
{
public:

    void test1()
    {
        typedef GN::Dictionary<int,int> Dict;

        Dict d;

        // empty
        TS_ASSERT( d.Empty() );
        TS_ASSERT_EQUALS( 0, d.Size() );

        // find and create
        d[0] = 1;
        d[1] = 2;

        // insert
        Dict::Iterator iter1, iter2;
        TS_ASSERT( d.Insert( 2, 3, &iter1 ) );
        TS_ASSERT_EQUALS( 2, iter1->Key() );
        TS_ASSERT_EQUALS( 3, iter1->Value() );
        TS_ASSERT( !d.Insert( 2, 4, &iter2 ) );
        TS_ASSERT_EQUALS( 2, iter2->Key() );
        TS_ASSERT_EQUALS( 3, iter2->Value() );
        TS_ASSERT_EQUALS( iter1, iter2 );

        TS_ASSERT( !d.Empty() );
        TS_ASSERT_EQUALS( 3, d.Size() );

        // find
        TS_ASSERT_EQUALS( 1, *d.Find(0) );
        TS_ASSERT_EQUALS( 2, *d.Find(1) );
        TS_ASSERT_EQUALS( 3, *d.Find(2) );

        // test iterator
        GN::DynaArray<int> keys;
        GN::DynaArray<int> values;
        for( Dict::Iterator i = d.Begin(); i != d.End(); ++i )
        {
            keys.Append( i->Key() );
            values.Append( i->Value() );
        }
        for( int i = 0; i < 3; ++i )
        {
            TS_ASSERT_EQUALS( i, keys[i] );
            TS_ASSERT_EQUALS( i+1, values[i] );
        }

        // remove
        d.Remove( 1 );
        TS_ASSERT( 2 == d.Size () );
        d.Remove( 4 );
        TS_ASSERT( 2 == d.Size () );

        // clear
        d.Clear();
        TS_ASSERT( d.Empty() );
        TS_ASSERT_EQUALS( 0, d.Size() );
    }
};

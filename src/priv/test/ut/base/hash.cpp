#include "../testCommon.h"

class StrHashTest : public CxxTest::TestSuite
{
    typedef GN::HashMap<GN::StrA, int, GN::StrA::Hash> StrHashMap;

public:

    void testHashMapSmoke()
    {
        using namespace GN;

        StrHashMap m(13);

        m.Insert( "abc", 1 );
        m.Insert( "abd", 2 );

        // find
        int * i;
        i = m.Find( "abc" );
        TS_ASSERT_DIFFERS( i, (int*)NULL );
        TS_ASSERT_EQUALS( *i, 1 );
        i = m.Find( "abcd" );
        TS_ASSERT_EQUALS( i, (int*)NULL );

        // erase
        m.Remove( "abe" );
        TS_ASSERT_EQUALS( m.Size(), 2 ); // erase non-existing item should have no effect.
        i = m.Find( "abc" );
        m.Remove( "abd" );
        TS_ASSERT_EQUALS( m.Size(), 1 ); // verify the one item is removed.
        TS_ASSERT_EQUALS( m.Find( "abd" ), (int*)NULL ); // verify correct item is erased.
        TS_ASSERT_EQUALS( m.Find( "abc" ), i ); // verify that erase operation does not affect other iterators.

        // erase the very last item in string map, would leave the map empty.
        m.Remove( "abc" );
        TS_ASSERT( m.Empty() );
        TS_ASSERT_EQUALS( m.Find( "abc" ), (int*)NULL );
    }

    void testEmptyMap()
    {
        using namespace GN;

        StrHashMap m;

        TS_ASSERT( m.Empty() );
        TS_ASSERT_EQUALS( 0, m.Size() );
    }

    void testEmptyMapIteration()
    {
        StrHashMap m;

        bool isempty = true;
        for( StrHashMap::KeyValuePair * i = m.First(); i != NULL; i = m.Next( i ) )
        {
            isempty = false;
        }
        TS_ASSERT( isempty );
    }

    void testNonEmptyMapIteration()
    {
        StrHashMap m;

        m.Insert( "a", 1 );

        int count = 0;
        for( const StrHashMap::KeyValuePair * i = m.First(); i != NULL; i = m.Next( i ) )
        {
            ++count;
        }
        TS_ASSERT( 1 == count );

        m.Insert( "b", 2 );

        TS_ASSERT( 2 == m["b"] );

        m["c"] = 3;

        count = 0;
        for( const StrHashMap::KeyValuePair * i = m.First(); i != NULL; i = m.Next( i ) )
        {
            ++count;
        }
        TS_ASSERT( 3 == count );
    }

    void testInsertAndFind()
    {
        using namespace GN;

        StrHashMap m;

        TS_ASSERT( m.Insert( "a", 1 ) );
        TS_ASSERT( !m.Insert( "a", 2 ) );
        TS_ASSERT( m.Insert( "b", 2 ) );

        TS_ASSERT_EQUALS( 1, *m.Find("a") );
        TS_ASSERT_EQUALS( 2, *m.Find("b") );
        TS_ASSERT( !m.Find("c") );
    }
};

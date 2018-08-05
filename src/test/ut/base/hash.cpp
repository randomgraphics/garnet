#include "../testCommon.h"

class StrHashTest : public CxxTest::TestSuite
{
    typedef GN::HashMap<GN::StrA, int, 128, GN::StrA::Hash> StrHashMap;

public:

    void testHashMapCrash()
    {
        // Hashmap used to crash when inserting values in specific order.

        using namespace GN;

        wchar_t keys[] = {
            70    ,
            80    ,
            83    ,
            58    ,
            32    ,
            48    ,
            46    ,
            51    ,
            57    ,
            40    ,
            114   ,
            101   ,
            115   ,
            49    ,
            102   ,
            111   ,
            104   ,
            108   ,
            112   ,
            41    ,
            55    ,
            50    ,
            54    ,
            53    ,
            56    ,
            69    ,
            67    ,
            47    ,
            88    ,
            66    ,
            84    ,
            78    ,
            36864 ,
            20986 ,
            52
        };

        HashMap<wchar_t,size_t,GN_ARRAY_COUNT(keys)> m;
        for( size_t i = 0; i < GN_ARRAY_COUNT(keys); ++i )
        {
            wchar_t key = keys[i];
            m[key] = i;
            TS_ASSERT( NULL != m.find(key) );
            TS_ASSERT_EQUALS( i, *m.find(key) );
        }
    }

    void testHashMapSmoke()
    {
        using namespace GN;

        StrHashMap m(13);

        m.insert( "abc", 1 );
        m.insert( "abd", 2 );

        // find
        int * i;
        i = m.find( "abc" );
        TS_ASSERT_DIFFERS( i, (int*)NULL );
        TS_ASSERT_EQUALS( *i, 1 );
        i = m.find( "abcd" );
        TS_ASSERT_EQUALS( i, (int*)NULL );

        // erase
        m.remove( "abe" );
        TS_ASSERT_EQUALS( m.size(), 2 ); // erase non-existing item should have no effect.
        i = m.find( "abc" );
        m.remove( "abd" );
        TS_ASSERT_EQUALS( m.size(), 1 ); // verify the one item is removed.
        TS_ASSERT_EQUALS( m.find( "abd" ), (int*)NULL ); // verify correct item is erased.
        TS_ASSERT_EQUALS( m.find( "abc" ), i ); // verify that erase operation does not affect other iterators.

        // erase the very last item in string map, would leave the map empty.
        m.remove( "abc" );
        TS_ASSERT( m.empty() );
        TS_ASSERT_EQUALS( m.find( "abc" ), (int*)NULL );
    }

    void testEmptyMap()
    {
        using namespace GN;

        StrHashMap m;

        TS_ASSERT( m.empty() );
        TS_ASSERT_EQUALS( 0, m.size() );
    }

    void testEmptyMapIteration()
    {
        StrHashMap m;

        bool isempty = true;
        for( StrHashMap::KeyValuePair * i = m.first(); i != NULL; i = m.next( i ) )
        {
            isempty = false;
        }
        TS_ASSERT( isempty );
    }

    void testNonEmptyMapIteration()
    {
        StrHashMap m;

        m.insert( "a", 1 );

        int count = 0;
        for( const StrHashMap::KeyValuePair * i = m.first(); i != NULL; i = m.next( i ) )
        {
            ++count;
        }
        TS_ASSERT( 1 == count );

        m.insert( "b", 2 );

        TS_ASSERT( 2 == m["b"] );

        m["c"] = 3;

        count = 0;
        for( const StrHashMap::KeyValuePair * i = m.first(); i != NULL; i = m.next( i ) )
        {
            ++count;
        }
        TS_ASSERT( 3 == count );
    }

    void testInsertAndFind()
    {
        using namespace GN;

        StrHashMap m;

        TS_ASSERT( m.insert( "a", 1 ) );
        TS_ASSERT( !m.insert( "a", 2 ) );
        TS_ASSERT( m.insert( "b", 2 ) );

        TS_ASSERT_EQUALS( 1, *m.find("a") );
        TS_ASSERT_EQUALS( 2, *m.find("b") );
        TS_ASSERT( !m.find("c") );
    }
};

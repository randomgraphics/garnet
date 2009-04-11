#include "../testCommon.h"

class StrHashTest : public CxxTest::TestSuite
{
    static bool strEqual( const GN::StrA & a, const GN::StrA & b )
    {
        return a == b;
    }

    typedef GN::HashMap< GN::StrA, int, GN::strHash<char>, strEqual > StrHashMap;

public:

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
        StrHashMap::Iterator e = m.end();
        for( StrHashMap::Iterator i = m.begin(); i != e; ++i )
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
        StrHashMap::ConstIterator e = m.end();
        for( StrHashMap::ConstIterator i = m.begin(); i != e; ++i )
        {
            ++count;
        }
        TS_ASSERT( 1 == count );

        m.insert( "b", 2 );

        TS_ASSERT( 2 == m["b"] );

        m["c"] = 3;

        count = 0;
        e = m.end();
        for( StrHashMap::ConstIterator i = m.begin(); i != e; i++ )
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

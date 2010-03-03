#include "../testCommon.h"

class StrHashTest : public CxxTest::TestSuite
{
    static bool equal( const GN::StrA & a, const GN::StrA & b )
    {
        return a == b;
    }

    static UInt64 hash( const GN::StrA & s ) { return s.Hash(); }

    typedef GN::HashMap< GN::StrA, int, hash, equal > StrHashMap;

public:

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
        StrHashMap::Iterator e = m.End();
        for( StrHashMap::Iterator i = m.Begin(); i != e; ++i )
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
        StrHashMap::ConstIterator e = m.End();
        for( StrHashMap::ConstIterator i = m.Begin(); i != e; ++i )
        {
            ++count;
        }
        TS_ASSERT( 1 == count );

        m.Insert( "b", 2 );

        TS_ASSERT( 2 == m["b"] );

        m["c"] = 3;

        count = 0;
        e = m.End();
        for( StrHashMap::ConstIterator i = m.Begin(); i != e; i++ )
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

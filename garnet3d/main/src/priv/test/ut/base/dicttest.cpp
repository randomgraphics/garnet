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

    static int kc;
    static int vc;

    // key type needs no default constructor
    struct KeyType
    {
        explicit KeyType( int )
        {
            ++kc;
        }

        KeyType( const KeyType & )
        {
            ++kc;
        }

        ~KeyType()
        {
            --kc;
        }

        bool operator<(const KeyType & ) const
        {
            return false;
        }
    };

    /// value type w/o default contructor
    struct ValueTypeNC
    {
        explicit ValueTypeNC( int )
        {
            ++vc;
        }

        ValueTypeNC( const ValueTypeNC & )
        {
            ++vc;
        }

        ~ValueTypeNC()
        {
            --vc;
        }
    };

    struct ValueType : public ValueTypeNC
    {
        ValueType() : ValueTypeNC( 100 )
        {
            ++vc;
        }

        ValueType( const ValueType & ) : ValueTypeNC(100)
        {
            ++vc;
        }

        ~ValueType()
        {
            --vc;
        }
    };

    void testObject()
    {
        using namespace GN;

        typedef Dictionary<KeyType,ValueTypeNC> Dict1;
        typedef Dictionary<KeyType,ValueType> Dict2;

        {
            Dict1 d1;
            Dict2 d2;

            KeyType k1(1);
            KeyType k2(2);
            ValueTypeNC v1(3);
            ValueTypeNC v2(4);
            ValueType v3;
            ValueType v4;

            d1.Insert( k1, v1 );
            d1.Insert( k2, v2 );

            d2.Insert( k1, v3 );
            d2[k2] = v4;
        }

        TS_ASSERT_EQUALS( 0, kc );
        TS_ASSERT_EQUALS( 0, vc );
    }
};

int TestDict::kc = 0;
int TestDict::vc = 0;

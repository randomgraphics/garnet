#include "../testCommon.h"

namespace GN
{
    static const unsigned int PRIMARY_ARRAY[] = {
        17,             /* 0 */
        37,             /* 1 */
        79,             /* 2 */
        163,            /* 3 */
        331,            /* 4 */
        673,            /* 5 */
        1361,           /* 6 */
        2729,           /* 7 */
        5471,           /* 8 */
        10949,          /* 9 */
        21911,          /* 10 */
        43853,          /* 11 */
        87719,          /* 12 */
        175447,         /* 13 */
        350899,         /* 14 */
        701819,         /* 15 */
        1403641,        /* 16 */
        2807303,        /* 17 */
        5614657,        /* 18 */
        11229331,       /* 19 */
        22458671,       /* 20 */
        44917381,       /* 21 */
        89834777,       /* 22 */
        179669557,      /* 23 */
        359339171,      /* 24 */
        718678369,      /* 25 */
        1437356741,     /* 26 */
        2147483647      /* 27 (largest signed int prime) */
    };                

    template<
        class  KEY,
        class  VALUE,
        size_t(*HASH_FUNC)( const KEY & ) >
    class HashMap
    {
    public:

        //@{

        HashMap()
            : mPrimIndex(0)
            , mCount(0)
            , mValues( PRIMARY_ARRAY[mPrimIndex] )
        {
        }

        //@}


        /// \name hash map operations
        //@{

        void clear()
        {
            mValues.clear();
            mValues.resize( PRIMARY_ARRAY[0] );
            mPrimIndex = 0;
            mCount = 0;
        }

        bool empty() const { return 0 == mCount; }

        const VALUE * find( const KEY & key ) const
        {
            const size_t N = PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mValues.size() );

            size_t k = HASH_FUNC(key) % N;

            const HashItem & hi = mValues[k];

            for(
                std::vector<PairType>::const_iterator i = hi.values.begin();
                i != hi.values.end();
                ++i )
            {
                if( i->first == key )
                {
                    // found!
                    GN_ASSERT( mCount > 0 );
                    return &i->second;
                }
            }

            // not found
            return 0;
        }

        bool insert( const KEY & key, const VALUE & value )
        {
            const size_t N = PRIMARY_ARRAY[mPrimIndex];
            
            GN_ASSERT( N == mValues.size() );

            size_t k = HASH_FUNC(key) % N;

            HashItem & hi = mValues[k];

            for(
                std::vector<PairType>::iterator i = hi.values.begin();
                i != hi.values.end();
                ++i )
            {
                if( i->first == key )
                {
                    // redundent key
                    return false;
                }
            }

            // insert new value
            hi.values.push_back( PairType(key,value) );
            ++mCount;
            if( mCount > (N*2) && (mPrimIndex+1) < GN_ARRAY_COUNT(PRIMARY_ARRAY) )
            {
                ++mPrimIndex;
                mValues.resize( PRIMARY_ARRAY[mPrimIndex] );
            }

            return true;
        }

        void remove( const KEY & key )
        {
            const size_t N = PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mValues.size() );

            size_t k = HASH_FUNC(key) % N;

            HashItem & hi = mValues[k];

            for(
                std::vector<PairType>::iterator i = hi.values.begin();
                i != hi.values.end();
                ++i )
            {
                if( i->first == key )
                {
                    // remove exisiting value
                    GN_ASSERT( mCount > 0 );
                    hi.values.remove( hi.values.begin() + i );
                    --mCount;
                    return;
                }
            }
        }

        void remove( const VALUE & )
        {
            GN_UNIMPL();
        }

        size_t size() const { return mCount; }

        //@}

        /// \name hash map operators
        //@{

        VALUE & operator[]( const KEY & key )
        {
            const size_t N = PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mValues.size() );

            size_t k = HASH_FUNC(key) % N;

            HashItem & hi = mValues[k];

            for(
                std::vector<PairType>::iterator i = hi.values.begin();
                i != hi.values.end();
                ++i )
            {
                if( i->first == key )
                {
                    // found!
                    GN_ASSERT( mCount > 0 );
                    return i->second;
                }
            }

            // not found, insert new value
            hi.values.push_back( PairType(key,VALUE()) );
            ++mCount;
            if( mCount > (N*2) && (mPrimIndex+1) < GN_ARRAY_COUNT(PRIMARY_ARRAY) )
            {
                ++mPrimIndex;
                mValues.resize( PRIMARY_ARRAY[mPrimIndex] );
            }
            return hi.values.back().second;
        }

        //@}

    private:

        typedef std::pair<KEY,VALUE> PairType;

        struct HashItem
        {
            std::vector<PairType> values;
        };

        size_t                mPrimIndex;
        size_t                mCount;
        std::vector<HashItem> mValues;
    };
}

class StrHashTest : public CxxTest::TestSuite
{
    typedef GN::HashMap< GN::StrA, int, GN::strHash<char> > StrHashMap;

public:

    void testEmptyMap()
    {
        using namespace GN;

        StrHashMap m;

        TS_ASSERT( m.empty() );
        TS_ASSERT_EQUALS( 0, m.size() );
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

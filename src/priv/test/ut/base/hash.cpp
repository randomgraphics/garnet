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
        size_t(*HASH_FUNC)( const KEY & ),
        size_t LOAD_FACTOR = 2 >
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

        ///
        /// hash map iterator class
        ///
        class Iterator
        {
            mutable const HashMap * mMap;
            mutable size_t          mIdx1;
            mutable size_t          mIdx2;

        public:

            ///
            /// ctor
            ///
            Iterator( const HashMap * m, size_t i1, size_t i2 )
                : mMap(m), mIdx1(i1), mIdx2(i2)
            {}

            /// copy constructor
            Iterator( const Iterator & it )
                : mMap(it.mMap), mIdx1(it.mIdx1), mIdx2(it.mIdx2)
            {
            }

            /// \name operators
            //@{

            VALUE & operator->()
            {
                GN_ASSERT( mIdx1 < mMap->mValues.size() );
                GN_ASSERT( mIdx2 < mMap->mValues[mIdx1].size() );

                mMap->mValues[mIdx1].values[mIdx2];
            }

            const VALUE & operator->() const
            {
                GN_ASSERT( mIdx1 < mMap->mValues.size() );
                GN_ASSERT( mIdx2 < mMap->mValues[mIdx1].size() );
                mMap->mValues[mIdx1].values[mIdx2];
            }

            bool operator==( const Iterator & rhs ) const
            {
                return  mMap == rhs.mMap
                    && mIdx1 == rhs.mIdx1
                    && mIdx2 == rhs.mIdx2;
            }

            bool operator!=( const Iterator & rhs ) const
            {
                return  mMap != rhs.mMap
                    || mIdx1 != rhs.mIdx1
                    || mIdx2 != rhs.mIdx2;
            }

            bool operator<( const Iterator & rhs ) const
            {
                if( mMap != rhs.mMap ) return mMap < rhs.mMap;
                if( mIdx1 != rhs.mIdx1 ) return mIdx1 < rhs.mIdx1;
                return mIdx2 < rhs.mIdx2;
            }

            const Iterator & operator=( const Iterator & rhs ) const
            {
                mMap  = rhs.mMap;
                mIdx1 = rhs.mIdx1;
                mIdx2 = rhs.mIdx2;
                return *this;
            }

            /// prefix plus operator
            const Iterator & operator++() const
            {
                GN_ASSERT( mIdx1 < mMap->mValues.size() );
                GN_ASSERT( mIdx2 < mMap->mValues[mIdx1].values.size() );

                ++mIdx2;
                if( mIdx2 < mMap->mValues[mIdx1].values.size() ) return *this;

                mIdx2 = 0;
                do {

                    ++mIdx1;

                    if( mIdx1 < mMap->mValues.size() && mIdx2 < mMap->mValues[mIdx1].values.size() )
                    {
                        return *this;
                    }
                } while( mIdx1 < mMap->mValues.size() );

                // reach the end of the hash map
                 mMap = 0;
                mIdx1 = 0;
                mIdx2 = 0;
                return *this;
            }

            /// suffix plus operator
            friend Iterator operator++( const Iterator & it, int )
            {
                Iterator ret( it );
                ++it;
                return ret;
            }

            //@}
        };

        ///
        /// constant iterator class
        ///
        typedef const Iterator ConstIterator;

        /// \name hash map operations
        //@{

        Iterator begin()
        {
            for( size_t idx1 = 0; idx1 < mValues.size(); ++idx1 )
            {
                if( mValues[idx1].values.size() > 0 )
                {
                    return Iterator( this, idx1, 0 );
                }
            }
            return Iterator( 0, 0, 0 );
        }

        ConstIterator begin() const
        {
            for( size_t idx1 = 0; idx1 < mValues.size(); ++idx1 )
            {
                if( mValues[idx1].values.size() > 0 )
                {
                    return Iterator( this, idx1, 0 );
                }
            }
            return Iterator( 0, 0, 0 );
        }

        void clear()
        {
            mValues.clear();
            mValues.resize( PRIMARY_ARRAY[0] );
            mPrimIndex = 0;
            mCount = 0;
        }

        bool empty() const { return 0 == mCount; }

        Iterator end()
        {
            return Iterator( 0, 0, 0 );
        }

        ConstIterator end() const
        {
            return Iterator( 0, 0, 0 );
        }

        const VALUE * find( const KEY & key ) const
        {
            const size_t N = PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mValues.size() );

            size_t k = compress( HASH_FUNC(key), N );

            const HashItem & hi = mValues[k];

            for(
                typename std::vector<PairType>::const_iterator i = hi.values.begin();
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

            size_t k = compress( HASH_FUNC(key), N );

            HashItem & hi = mValues[k];

            for(
                typename std::vector<PairType>::iterator i = hi.values.begin();
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
            if( mCount > (N*LOAD_FACTOR) && (mPrimIndex+1) < GN_ARRAY_COUNT(PRIMARY_ARRAY) )
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

            size_t k = compress( HASH_FUNC(key), N );

            HashItem & hi = mValues[k];

            for(
                typename std::vector<PairType>::iterator i = hi.values.begin();
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

            size_t k = compress( HASH_FUNC(key), N );

            HashItem & hi = mValues[k];

            for(
                typename std::vector<PairType>::iterator i = hi.values.begin();
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
            if( mCount > (N*LOAD_FACTOR) && (mPrimIndex+1) < GN_ARRAY_COUNT(PRIMARY_ARRAY) )
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

    private:

        /// compress arbitrary interger into range [0..N)
        static inline size_t compress( size_t i, size_t N )
        {
            return i % N;
        }

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
        m.insert( "c", 3 );

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

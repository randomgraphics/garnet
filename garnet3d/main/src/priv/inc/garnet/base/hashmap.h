#ifndef __GN_BASE_HASHMAP_H__
#define __GN_BASE_HASHMAP_H__
// *****************************************************************************
/// \file
/// \brief   Define hash map template
/// \author  chenli@@REDMOND (2008.9.10)
// *****************************************************************************

namespace GN
{
    extern const size_t HASH_MAP_PRIMARY_ARRAY[28];

    /// Hash map template class
    template<
        class  KEY,
        class  VALUE,
        UInt64(*HASH_FUNC)( const KEY & ),
        bool  (*EQUAL_FUNC)( const KEY &, const KEY & ),
        size_t LOAD_FACTOR = 2 >
    class HashMap
    {
    public:

        //@{

        HashMap()
            : mPrimIndex(0)
            , mCount(0)
            , mValues( HASH_MAP_PRIMARY_ARRAY[mPrimIndex] )
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
                GN_ASSERT( mIdx1 < mMap->mValues.Size() );
                GN_ASSERT( mIdx2 < mMap->mValues[mIdx1].Size() );

                mMap->mValues[mIdx1].values[mIdx2];
            }

            const VALUE & operator->() const
            {
                GN_ASSERT( mIdx1 < mMap->mValues.Size() );
                GN_ASSERT( mIdx2 < mMap->mValues[mIdx1].Size() );
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
                GN_ASSERT( mIdx1 < mMap->mValues.Size() );
                GN_ASSERT( mIdx2 < mMap->mValues[mIdx1].values.Size() );

                ++mIdx2;
                if( mIdx2 < mMap->mValues[mIdx1].values.Size() ) return *this;

                mIdx2 = 0;
                do {

                    ++mIdx1;

                    if( mIdx1 < mMap->mValues.Size() && mIdx2 < mMap->mValues[mIdx1].values.Size() )
                    {
                        return *this;
                    }
                } while( mIdx1 < mMap->mValues.Size() );

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
            for( size_t idx1 = 0; idx1 < mValues.Size(); ++idx1 )
            {
                if( mValues[idx1].values.Size() > 0 )
                {
                    return Iterator( this, idx1, 0 );
                }
            }
            return Iterator( 0, 0, 0 );
        }

        ConstIterator begin() const
        {
            for( size_t idx1 = 0; idx1 < mValues.Size(); ++idx1 )
            {
                if( mValues[idx1].values.Size() > 0 )
                {
                    return Iterator( this, idx1, 0 );
                }
            }
            return Iterator( 0, 0, 0 );
        }

        void Clear()
        {
            mValues.Resize( HASH_MAP_PRIMARY_ARRAY[0] );
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

        VALUE * find( const KEY & key ) const
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mValues.Size() );

            size_t k = mod( HASH_FUNC(key), N );

            const HashItem & hi = mValues[k];

            for(
                const PairType * p = hi.values.Begin();
                p != hi.values.End();
                ++p )
            {
                if( EQUAL_FUNC( p->first, key ) )
                {
                    // found!
                    GN_ASSERT( mCount > 0 );
                    return const_cast<VALUE*>(&p->second);
                }
            }

            // not found
            return 0;
        }

        bool insert( const KEY & key, const VALUE & value )
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mValues.Size() );

            size_t k = mod( HASH_FUNC(key), N );

            HashItem & hi = mValues[k];

            for(
                const PairType * p = hi.values.Begin();
                p != hi.values.End();
                ++p )
            {
                if( EQUAL_FUNC( p->first, key ) )
                {
                    // redundent key
                    return false;
                }
            }

            // insert new value
            hi.values.Append( PairType(key,value) );
            ++mCount;
            if( mCount > (N*LOAD_FACTOR) && (mPrimIndex+1) < GN_ARRAY_COUNT(HASH_MAP_PRIMARY_ARRAY) )
            {
                ++mPrimIndex;
                mValues.Resize( HASH_MAP_PRIMARY_ARRAY[mPrimIndex] );
            }

            return true;
        }

        void RemoveKey( const KEY & key )
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mValues.Size() );

            size_t k = mod( HASH_FUNC(key), N );

            HashItem & hi = mValues[k];

            for(
                PairType * p = hi.values.Begin();
                p != hi.values.End();
                ++p )
            {
                if( EQUAL_FUNC( p->first, key ) )
                {
                    // remove exisiting value
                    GN_ASSERT( mCount > 0 );
                    hi.values.EraseItem( p );
                    --mCount;
                    return;
                }
            }
        }

        void RemoveValue( const VALUE & )
        {
            GN_UNIMPL();
        }

        size_t Size() const { return mCount; }

        //@}

        /// \name hash map operators
        //@{

        VALUE & operator[]( const KEY & key )
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mValues.Size() );

            size_t k = mod( HASH_FUNC(key), N );

            HashItem & hi = mValues[k];

            for(
                PairType * p = hi.values.Begin();
                p != hi.values.End();
                ++p )
            {
                if( EQUAL_FUNC( p->first, key ) )
                {
                    // found!
                    GN_ASSERT( mCount > 0 );
                    return p->second;
                }
            }

            // not found, insert new value
            hi.values.Append( PairType(key,VALUE()) );
            ++mCount;
            if( mCount > (N*LOAD_FACTOR) && (mPrimIndex+1) < GN_ARRAY_COUNT(HASH_MAP_PRIMARY_ARRAY) )
            {
                ++mPrimIndex;
                mValues.Resize( HASH_MAP_PRIMARY_ARRAY[mPrimIndex] );
            }
            return hi.values.Back().second;
        }

        //@}

    private:

        typedef std::pair<KEY,VALUE> PairType;

        struct HashItem
        {
            DynaArray<PairType> values;
        };

        size_t                mPrimIndex;
        size_t                mCount;
        DynaArray<HashItem> mValues;

    private:

        /// mod interger into range [0..N)
        static inline size_t mod( UInt64 i, size_t N )
        {
            return (size_t)( i % N );
        }
    };

}


// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_HASHMAP_H__

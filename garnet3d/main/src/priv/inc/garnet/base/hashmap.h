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

    namespace HashMapUtils
    {
        template<typename T>
        struct DirectHash
        {
            UInt64 operator()( const T & t ) const
            {
                return (UInt64)t;
            }
        };

        template<typename T>
        struct EqualOperator
        {
            bool operator()( const T & a, const T & b ) const
            {
                return a == b;
            }
        };
    };

    /// Hash map template class
    template<
        class  KEY,
        class  VALUE,
        class  KEY_HASH_FUNC = HashMapUtils::DirectHash<KEY>,
        class  KEY_EQUAL_FUNC = HashMapUtils::EqualOperator<KEY>,
        size_t LOAD_FACTOR = 2 >
    class HashMap
    {
    public:

        /// the key-value pair type.
        struct KeyValuePair
        {
            const KEY key;
            VALUE     value;

        protected:

            KeyValuePair( const KEY & k, const VALUE & v )
                : key( k ), value( v )
            {
            }
        };

    public:

        /// \name public methods
        //@{


        explicit HashMap( size_t initialTableSize = 0 )
            : mKeyHashFunc( KEY_HASH_FUNC() )
            , mKeyEqualFunc( KEY_EQUAL_FUNC() )
            , mPrimIndex( GetInitialPrimIndex( initialTableSize ) )
            , mCount(0)
            , mTable( HASH_MAP_PRIMARY_ARRAY[mPrimIndex] )
        {
        }

        ~HashMap()
        {
            Clear();
        }

        void Clear()
        {
            // clear link table
            for( PairType * p = mLinkedItems.GetHead(); p != NULL; )
            {
                PairType * np = p->next;
                delete p;
                p = np;
            }
            memset( &mLinkedItems, 0, sizeof(mLinkedItems) );

            // clear hash table
            mTable.Clear();
            mTable.Resize( HASH_MAP_PRIMARY_ARRAY[0] );
            mPrimIndex = 0;
            mCount = 0;
        }

        bool Empty() const
        {
            return 0 == mCount;
        }

        VALUE * Find( const KEY & key ) const
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mTable.Size() );

            size_t k = mod( mKeyHashFunc(key), N );

            const HashItem & hi = mTable[k];

            for(
                const PairType * const * pp = hi.values.Begin();
                pp != hi.values.End();
                ++pp )
            {
                GN_ASSERT( *pp );
                if( mKeyEqualFunc( (*pp)->key, key ) )
                {
                    // found!
                    GN_ASSERT( mCount > 0 );
                    return const_cast<VALUE*>(&(*pp)->value);
                }
            }

            // not found
            return 0;
        }

        KeyValuePair * First()
        {
            return mLinkedItems.GetHead();
        }

        const KeyValuePair * First() const
        {
            return mLinkedItems.GetHead();
        }

        KeyValuePair * Insert( const KEY & key, const VALUE & value )
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mTable.Size() );

            size_t k = mod( mKeyHashFunc(key), N );

            HashItem & hi = mTable[k];

            // check for redundency
            for(
                const PairType * const * pp = hi.values.Begin();
                pp != hi.values.End();
                ++pp )
            {
                if( mKeyEqualFunc( (*pp)->key, key ) )
                {
                    // redundent item
                    return NULL;
                }
            }

            // create new pair item
            PairType * newPair = new PairType( key, value );

            // add to linked list
            mLinkedItems.Append( newPair );

            // add to hash table
            hi.values.Append( newPair );

            // adjust count
            ++mCount;
            if( mCount > (N*LOAD_FACTOR) && (mPrimIndex+1) < GN_ARRAY_COUNT(HASH_MAP_PRIMARY_ARRAY) )
            {
                ++mPrimIndex;
                mTable.Resize( HASH_MAP_PRIMARY_ARRAY[mPrimIndex] );
            }

            return newPair;
        }

        KeyValuePair * Next( const KeyValuePair * p )
        {
            if( NULL == p ) return NULL;

            PairType * pt = (PairType*)p;

            if( pt->owner != &mLinkedItems ) return NULL;

            return pt->next;
        }

        const KeyValuePair * Next( const KeyValuePair * p ) const
        {
            if( NULL == p && p->owner != &mLinkedItems ) return NULL;
            return ((const PairType*)p)->next;
        }

        void Remove( const KEY & key )
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mTable.Size() );

            size_t k = mod( mKeyHashFunc(key), N );

            HashItem & hi = mTable[k];

            for(
                PairType ** pp = hi.values.Begin();
                pp != hi.values.End();
                ++pp )
            {
                if( mKeyEqualFunc( (*pp)->key, key ) )
                {
                    // Found. Remove it.
                    GN_ASSERT( mCount > 0 );

                    // remove from linked list
                    mLinkedItems.Remove( (*pp) );

                    // delete the pair item
                    delete *pp;

                    // remove from hash table
                    // Note: this will change value of "*pp"
                    hi.values.EraseItem( pp );

                    // adjust count
                    --mCount;

                    // Done!
                    return;
                }
            }
        }

        size_t Size() const { return mCount; }

        //@}

        /// \name public operators
        //@{

        VALUE & operator[]( const KEY & key )
        {
            VALUE * p = Find( key );
            if( p ) return *p;

            return Insert( key, VALUE() )->value;
         }

        const VALUE & operator[]( const KEY & key ) const
        {
            VALUE * p = Find( key );

            GN_ASSERT( p );

            return *p;
         }

        //@}

    private:

        struct PairType : public KeyValuePair
        {
            void *     owner;
            PairType * prev;
            PairType * next;

            PairType( const KEY & k, const VALUE & v )
                : KeyValuePair( k, v )
                , owner(NULL)
                , prev(0)
                , next(0)
            {
            }
        };

        struct HashItem
        {
            DynaArray<PairType*> values;
        };

        KEY_HASH_FUNC              mKeyHashFunc;
        KEY_EQUAL_FUNC             mKeyEqualFunc;

        size_t                     mPrimIndex;
        size_t                     mCount;
        DynaArray<HashItem>        mTable;
        DoubleLinkedList<PairType> mLinkedItems;

    private:

        /// return index to primay number array
        static inline size_t GetInitialPrimIndex( size_t initialSize )
        {
            size_t i;

            for( i = 0; i < GN_ARRAY_COUNT(HASH_MAP_PRIMARY_ARRAY); ++i )
            {
                if( HASH_MAP_PRIMARY_ARRAY[i] >= initialSize ) return i;
            }

            return i - 1;
        }

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

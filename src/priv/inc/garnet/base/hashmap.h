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
        struct HashFunc_ToUInt64
        {
            uint64 operator()( const T & t ) const
            {
                return (uint64)t;
            }
        };

        template<typename T>
        struct HashFunc_MemoryHash
        {
            enum
            {
                N_64 = sizeof(T) / sizeof(uint64),
                TAIL = sizeof(T) % sizeof(uint64),
            };

            uint64 operator()( const T & t ) const
            {
                const uint64 * p64 = (const uint64*)&t;

                uint64 h = 5471;

                for( size_t i = 0; i < N_64; ++i, ++p64 )
                {
                    h = h * 33 + *p64;
                }

                const uint8 * p8 = (const uint8*)p64;
                uint64 tail = 0;
                for( size_t i = 0; i < TAIL; ++i, ++p8 )
                {
                    tail = (tail << 8) + *p8;
                }

                h = h * 33 + tail;

                return h;
            }
        };

        template<typename T>
        struct HashFunc_HashMethod
        {
            uint64 operator()( const T & t ) const
            {
                return t.hash();
            }
        };

        template<typename T>
        struct EqualFunc_Operator
        {
            bool operator()( const T & a, const T & b ) const
            {
                return a == b;
            }
        };

        template<typename T>
        struct EqualFunc_MemoryCompare
        {
            bool operator()( const T & a, const T & b ) const
            {
                return 0 == ::memcmp( &a, &b, sizeof(T) );
            }
        };
    };

    /// Hash map template class
    template<
        class  KEY,
        class  VALUE,
        class  KEY_HASH_FUNC = HashMapUtils::HashFunc_ToUInt64<KEY>,
        class  KEY_EQUAL_FUNC = HashMapUtils::EqualFunc_Operator<KEY>,
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
            , mPrimIndex( getInitialPrimIndex( initialTableSize ) )
            , mCount(0)
            , mTable( HASH_MAP_PRIMARY_ARRAY[mPrimIndex] )
        {
        }

        ~HashMap()
        {
            clear();
        }

        void clear()
        {
            // clear link table
            for( PairType * p = mLinkedItems.head(); p != NULL; )
            {
                PairType * np = p->next;
                delete p;
                p = np;
            }
            memset( &mLinkedItems, 0, sizeof(mLinkedItems) );

            // clear hash table
            mTable.clear();
            mTable.resize( HASH_MAP_PRIMARY_ARRAY[0] );
            mPrimIndex = 0;
            mCount = 0;
        }

        bool empty() const
        {
            return 0 == mCount;
        }

        VALUE * find( const KEY & key ) const
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mTable.size() );

            size_t k = mod( mKeyHashFunc(key), N );

            const HashItem & hi = mTable[k];

            for(
                const PairType * const * pp = hi.values.begin();
                pp != hi.values.end();
                ++pp )
            {
                GN_ASSERT( *pp );
                if( mKeyEqualFunc( (*pp)->key, key ) )
                {
                    // found!
                    GN_ASSERT( mCount > 0 );
                    return (VALUE*)&(*pp)->value;
                }
            }

            // not found
            return 0;
        }

        KeyValuePair * first()
        {
            return mLinkedItems.head();
        }

        const KeyValuePair * first() const
        {
            return mLinkedItems.head();
        }

        /// Insert new key and value into the map
        ///
        /// \param key, value
        ///     The new item's key and value
        /// \param pair
        ///     Optional. Could be NULL.
        ///     If insertion succeeds, return the newly inserted key-value pair;
        ///     if insertion failed because the key exists already, returns the
        ///     exsiting pair.
        /// \return
        ///     Return if insert succeded or not.
        bool insert( const KEY & key, const VALUE & value, KeyValuePair ** pair )
        {
            // Check for redundant insert
            {
                const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

                GN_ASSERT( N == mTable.size() );

                size_t k = mod( mKeyHashFunc(key), N );

                HashItem & hi = mTable[k];

                for(
                    PairType ** pp = hi.values.begin();
                    pp != hi.values.end();
                    ++pp )
                {
                    if( mKeyEqualFunc( (*pp)->key, key ) )
                    {
                        // redundent item
                        if( pair ) *pair = *pp;
                        return false;
                    }
                }

                // adjust primary index
                if( (mCount+1) > (N*LOAD_FACTOR) && (mPrimIndex+1) < GN_ARRAY_COUNT(HASH_MAP_PRIMARY_ARRAY) )
                {
                    ++mPrimIndex;
                    mTable.resize( HASH_MAP_PRIMARY_ARRAY[mPrimIndex] );
                }
            }

            // Note: mPrimaryIndex has changed, need re-hash.
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];
            GN_ASSERT( N == mTable.size() );
            size_t k = mod( mKeyHashFunc(key), N );
            HashItem & hi = mTable[k];

            // create new pair item
            PairType * newPair = new PairType( key, value );

            // add to linked list
            mLinkedItems.append( newPair );

            // add to hash table
            hi.values.append( newPair );

            // adjust count
            ++mCount;

            if( pair ) *pair = newPair;
            return true;
        }

        // return NULL, if insertion failed (like the key exists already)
        KeyValuePair * insert( const KEY & key, const VALUE & value )
        {
            KeyValuePair * result;
            if( insert( key, value, &result ) )
            {
                GN_ASSERT( result );
                return result;
            }
            else
            {
                return NULL;
            }
        }

        KeyValuePair * next( const KeyValuePair * p )
        {
            if( NULL == p ) return NULL;

            PairType * pt = (PairType*)p;

            if( pt->owner != &mLinkedItems ) return NULL;

            return pt->next;
        }

        const KeyValuePair * next( const KeyValuePair * p ) const
        {
            if( NULL == p && p->owner != &mLinkedItems ) return NULL;
            return ((const PairType*)p)->next;
        }

        void remove( const KEY & key )
        {
            const size_t N = HASH_MAP_PRIMARY_ARRAY[mPrimIndex];

            GN_ASSERT( N == mTable.size() );

            size_t k = mod( mKeyHashFunc(key), N );

            HashItem & hi = mTable[k];

            for(
                PairType ** pp = hi.values.begin();
                pp != hi.values.end();
                ++pp )
            {
                if( mKeyEqualFunc( (*pp)->key, key ) )
                {
                    // Found. remove it.
                    GN_ASSERT( mCount > 0 );

                    // remove from linked list
                    mLinkedItems.remove( (*pp) );

                    // delete the pair item
                    delete *pp;

                    // remove from hash table
                    // Note: this will change value of "*pp"
                    hi.values.erasePtr( pp );

                    // adjust count
                    --mCount;

                    // Done!
                    return;
                }
            }
        }

        size_t size() const { return mCount; }

        //@}

        /// \name public operators
        //@{

        VALUE & operator[]( const KEY & key )
        {
            VALUE * p = find( key );
            if( p ) return *p;

            return insert( key, VALUE() )->value;
         }

        const VALUE & operator[]( const KEY & key ) const
        {
            VALUE * p = find( key );

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
        static inline size_t getInitialPrimIndex( size_t initialSize )
        {
            size_t i;

            for( i = 0; i < GN_ARRAY_COUNT(HASH_MAP_PRIMARY_ARRAY); ++i )
            {
                if( HASH_MAP_PRIMARY_ARRAY[i] >= initialSize ) return i;
            }

            return i - 1;
        }

        /// mod interger into range [0..N)
        static inline size_t mod( uint64 i, size_t N )
        {
            return (size_t)( i % N );
        }
    };
}


// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_HASHMAP_H__

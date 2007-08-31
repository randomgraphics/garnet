#ifndef __GN_BASE_MEMPOOL_H__
#define __GN_BASE_MEMPOOL_H__
// *****************************************************************************
/// \file
/// \brief   memory pool classes
/// \author  chenli@@FAREAST (2007.8.31)
// *****************************************************************************

namespace GN
{
    ///
    /// Fix-sized raw memory pool, no ctor/dtor involved.
    ///
    template<
        size_t ITEM_SIZE,
        size_t ALIGNMENT = 8,
        size_t ITEMS_PER_BLOCK = 4096,
        size_t MAX_ITEMS = 0 >
    class FixSizedRawMemoryPool
    {
        // alignment must be 2^N
        GN_CASSERT( 0 == ( ( ALIGNMENT - 1 ) ^ ~((size_t)0) ) );

        template<size_t N,size_t A>
        struct Alignment
        {
            static const size_t VALUE = ( N + (A-1) ) & ~(A-1);
        };

        static const size_t ALIGNED_ITEM_SIZE = Alignment< Alignment<ITEM_SIZE,sizeof(void*)>::VALUE + sizeof(void*)*2, ALIGNMENT>::VALUE;

        union Item
        {
            UInt8  raw[ALIGNED_ITEM_SIZE];
            struct
            {
                UInt8  data[ITEM_SIZE];
                Item * prev; ///< points to previous item
                Item * next; ///< points to next item
            };
        };
        GN_CASSERT( sizeof(Item) == ALIGNED_ITEM_SIZE );

        struct Block
        {
            Item items[ITEMS_PER_BLOCK];
            Block * next; ///< points to next block
        };

        Block * mBlocks;
        Item  * mItems;
        Item  * mFreeItems;
        size_t  mItemCount;

    public:

        ///
        /// Default ctor.
        ///
        FixSizedRawMemoryPool()
            : mBlocks(0)
            , mItems(0)
            , mFreeItems(0)
            , mItemCount(0)
        {
        }

        ///
        /// Default ctor.
        ///
        ~FixSizedRawMemoryPool()
        {
            Block * p;
            while( mBlocks )
            {
                p = mBlocks;
                mBlocks = mBlocks->next;
                heapFree( p );
            }
        }

        ///
        /// Allocate raw memory for one item
        ///
        void * alloc()
        {
            if( MAX_ITEMS > 0 && mItemCount == MAX_ITEMS )
            {
                GN_ERROR(getLogger("FixSizedRawMemoryPool"))( "out of pool memory!" );
                return 0;
            }

            if( 0 == mFreeItems )
            {
                // no free items. create new block
                Block * b = (Block*)heapAlloc( sizeof(Block) );
                if( 0 == b )
                {
                    GN_ERROR(getLogger("FixSizedRawMemoryPool"))( "out of heap memory!" );
                    return 0;
                }

                // build free list
                for( size_t i = 0; i < ITEMS_PER_BLOCK; ++i )
                {
                    b->items[i].next = mFreeItems;
                    mFreeItems = &b->items[i];
                }

                // add to block list
                b->next = mBlocks;
                mBlocks = b;
            }

            // get one from free list.
            Item * p = mFreeItems;
            mFreeItems = mFreeItems->next;

            // insert to item list
            p->prev = 0;
            p->next = mItems;
            if( mItems ) mItems->prev = p;
            mItems = p;

            ++mItemCount;

            return p;
        }

        ///
        /// Deallocate
        ///
        void dealloc( void * p )
        {
            if( 0 == p ) return;

            if( 0 == mItemCount )
            {
                GN_ERROR(getLogger("FixSizedRawMemoryPool"))( "input pointer is not belong to this pool!" );
                return;
            }

            --mItemCount;

            Item * i = (Item*)p;

            // remove from item list
            if( i->prev ) i->prev->next = i->next;
            if( i->next ) i->next->prev = i->prev;
            if( i == mItems ) mItems = i->next;

            // add to free list
            i->next = mFreeItems;
            mFreeItems = i;
        }

        ///
        /// get first item in allocator
        ///
        void * getFirst() const { return mItems; }

        ///
        /// get next item in allocator
        ///
        void * getNext( void * p ) const { GN_ASSERT(p); return ((Item*)p)->next; }
    };

    ///
    /// object pool
    ///
    template<
        class  T,
        class  RAW_MEMORY_POOL = FixSizedRawMemoryPool<sizeof(T)> >
    class ObjectPool
    {
        RAW_MEMORY_POOL mRawMem;
        Mutex           mMutex;

        static void ctor( T * p )
        {
            GN_ASSERT( p );
            new (p) T;
        }

        static void dtor( T * p )
        {
            GN_ASSERT( p );
            p->T::~T();
        }

        T * doAlloc()
        {
            T * p = (T*)mRawMem.alloc();
            if( 0 == p ) return 0;

            // construct the object.
            ctor( p );

            // success
            return p;
        }

        void doDealloc( T * p )
        {
            if( 0 == p ) return;

            // destruct the object
            dtor( p );

            // free p
            mRawMem.dealloc( p );
        }

        void doFreeAll()
        {
            ScopeMutex<Mutex> lock( mMutex );

            T * p;
            while( NULL != ( p = (T*)mRawMem.getFirst() ) )
            {
                doDealloc( p );
            }
        }

    public:

        //@{

        ObjectPool() {}

        ~ObjectPool() { freeAll(); }

        //@}

        //@{
        T  * alloc() { return doAlloc(); }
        T  * allocUnconstructed() { return (T*)mRawMem.alloc(); }
        void dealloc( void * p ) { doDealloc( (T*)p ); }
        void freeAll() { doFreeAll(); }
        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_MEMPOOL_H__

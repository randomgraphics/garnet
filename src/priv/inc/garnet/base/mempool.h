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
        size_t INITIAL_ITEMS_PER_BLOCK = 32,
        size_t MAX_ITEMS = 0 >
    class FixSizedRawMemoryPool : public NoCopy
    {
        // alignment must be 2^N and can not be zero
        GN_CASSERT( ( 0 == ( (ALIGNMENT-1) & ALIGNMENT ) ) && ( ALIGNMENT > 0 ) );

        template<size_t N,size_t A>
        struct Alignment
        {
            static const size_t VALUE = ( N + (A-1) ) & ~(A-1);
        };

        static const size_t ALIGNED_ITEM_SIZE = Alignment< Alignment<ITEM_SIZE,sizeof(void*)>::VALUE + sizeof(void*)*3, ALIGNMENT>::VALUE;

        union Item
        {
            UInt8  raw[ALIGNED_ITEM_SIZE];
            struct
            {
                UInt8  data[ITEM_SIZE];
                void * pool; ///< pointer to the pool
                Item * prev; ///< points to previous item
                Item * next; ///< points to next item
            };
        };
        GN_CASSERT( sizeof(Item) == ALIGNED_ITEM_SIZE );

        struct Block
        {
            size_t  count; ///< number of items in item array
            Item  * items; ///< item array
            Block * next;  ///< points to next block
        };

        Block * mBlocks;
        Item  * mItems;
        Item  * mFreeItems;
        size_t  mItemCount;
        size_t  mNewBlockSize;

    public:

        ///
        /// Default ctor.
        ///
        FixSizedRawMemoryPool()
            : mBlocks(0)
            , mItems(0)
            , mFreeItems(0)
            , mItemCount(0)
            , mNewBlockSize(INITIAL_ITEMS_PER_BLOCK)
        {
        }

        ///
        /// Default ctor.
        ///
        ~FixSizedRawMemoryPool()
        {
            freeAll();
        }

        ///
        /// make sure a valid pointer belongs to this pool
        ///
        bool check( const void * p ) const
        {
            if( 0 == p ) return false;

            Item * i = (Item*)p;

            return i->pool == this;
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
                Block * b = (Block*)HeapAlloc( sizeof(Block) );
                if( 0 == b )
                {
                    GN_ERROR(getLogger("FixSizedRawMemoryPool"))( "out of heap memory!" );
                    return 0;
                }
                b->items = (Item*)HeapAlloc( sizeof(Item) * mNewBlockSize );
                if( 0 == b )
                {
                    GN_ERROR(getLogger("FixSizedRawMemoryPool"))( "out of heap memory!" );
                    HeapFree( b );
                    return 0;
                }
                b->count = mNewBlockSize;
                mNewBlockSize *= 2; // size of next block is doubled.

                // build free list
                for( size_t i = 0; i < b->count; ++i )
                {
                    b->items[i].pool = this;
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

            Item * i = (Item*)p;
            if( !check(p) )
            {
                GN_ERROR(getLogger("FixSizedRawMemoryPool"))( "invalid pointer!" );
                return;
            }

            if( 0 == mItemCount )
            {
                GN_ERROR(getLogger("FixSizedRawMemoryPool"))( "input pointer is not belong to this pool!" );
                return;
            }

            --mItemCount;

            // remove from item list
            if( i->prev ) i->prev->next = i->next;
            if( i->next ) i->next->prev = i->prev;
            if( i == mItems ) mItems = i->next;

            // add to free list
            i->next = mFreeItems;
            mFreeItems = i;
        }

        ///
        /// free all items
        ///
        void freeAll()
        {
            Block * p;
            while( mBlocks )
            {
                p = mBlocks;
                mBlocks = mBlocks->next;
                HeapFree( p->items );
                HeapFree( p );
            }
            mBlocks = 0;
            mItems = 0;
            mFreeItems = 0;
            mItemCount = 0;
        }

        ///
        /// get first item in allocator
        ///
        void * getFirst() const { return mItems; }

        ///
        /// get next item in allocator
        ///
        void * getNext( const void * p ) const { GN_ASSERT(p); return ((Item*)p)->next; }
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

        T * doAlloc()
        {
            T * p = (T*)mRawMem.alloc();
            if( 0 == p ) return 0;

            // construct the object, using defualt constructor
            new (p) T;

            // success
            return p;
        }

        void doDealloc( T * p )
        {
            if( 0 == p ) return;

            // destruct the object
            p->T::~T();

            // free p
            mRawMem.dealloc( p );
        }

        void doFreeAll()
        {
            ScopeMutex<Mutex> lock( mMutex );

            // destruct all objects
            for( T * p = (T*)mRawMem.getFirst(); 0 != p; p = (T*)mRawMem.getNext(p) )
            {
                p->T::~T();
            }

            // free memory
            mRawMem.freeAll();
        }

    public:

        //@{

        ObjectPool() {}

        ~ObjectPool() { deconstructAndFreeAll(); }

        //@}

        //@{
        T  * allocConstructed() { return doAlloc(); }
        T  * allocUnconstructed() { return (T*)mRawMem.alloc(); }
        void deconstructAndFree( void * p ) { doDealloc( (T*)p ); }
        void freeWithoutDeconstruct( void * p ) { mRawMem.dealloc( p ); }
        void deconstructAndFreeAll() { doFreeAll(); }
        bool check( const T * p ) const { return mRawMem.check( p ); }
        T  * getFirst() const { return (T*)mRawMem.getFirst(); }
        T  * getNext( const T * p ) const { return (T*)mRawMem.getNext(p); }
        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_MEMPOOL_H__

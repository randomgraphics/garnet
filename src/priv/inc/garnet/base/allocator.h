#ifndef __GN_BASE_ALLOCATOR_H__
#define __GN_BASE_ALLOCATOR_H__
// *****************************************************************************
/// \file    allocator.h
/// \brief   Common allocator classes
/// \author  chenlee (2006.5.31)
// *****************************************************************************

namespace GN
{
    ///
    /// STL compilant allocator that use garnet heap memory management routines.
    ///
    template<typename T>
    class StlAllocator
    {
        /// \cond NEVER
    public:
        typedef T                  value_type;
        typedef size_t             size_type;
        typedef ptrdiff_t          difference_type;
        typedef const value_type * const_pointer;
        typedef const value_type & const_reference;
        typedef value_type *       pointer;
        typedef value_type &       reference;

        template<class T2>
        struct rebind
        {
            typedef StlAllocator<T2> other;
        };

        StlAllocator() GN_NOTHROW() {}

        ~StlAllocator() GN_NOTHROW() {}

        StlAllocator( const StlAllocator<T> & ) GN_NOTHROW() {}

        template<class T2>
        StlAllocator( const StlAllocator<T2> & ) GN_NOTHROW() {}

        template<class T2>
        StlAllocator<T> & operator=( const StlAllocator<T2> & )
        {
            return *this;
        }

        pointer address( reference x ) const
        {
            return &x;
        }

        const_pointer address( const_reference x ) const
        {
            return &x;
        }

        pointer allocate( size_type count, const void * = 0 )
        {
            return (pointer)heapAlloc( count * sizeof(T) );
        }

        void deallocate( pointer ptr, size_type )
        {
            heapFree( ptr );
        }

        void construct( pointer ptr, const T & x )
        {
            new (ptr) T(x);
        }

        void destroy( pointer ptr )
        {
            ptr->T::~T();
        }

        size_type max_size() const GN_NOTHROW()
        {
            size_type count = (size_t)(-1) / sizeof(T);
            return ( 0 < count ? count : 1 );
        }
        /// \endcond
    };

    ///
    /// Standard allocator using global new and delete operator.
    ///
    /// Follow interface of this class, when defining customized allocator.
    ///
    template<class T>
    class StandardAllocator
    {
    public:

        ///
        /// Rebind to other type
        ///
        template<class T2>
        struct Rebind
        {
            ///
            /// allocator class suitable for class T2.
            ///
            typedef StlAllocator<T2> Other;
        };

        ///
        /// do allocation, call constructor(s) as well.
        ///
        static T * sAlloc( size_t count )
        {
            return new T[count];
        }

        ///
        /// do deallocation, call destructor(s) as well.
        ///
        static void sDealloc( T * p, size_t count )
        {
            ((void)(count)); // unused parameter
            delete [] p;
        }
    };

    ///
    /// Fixed sized memory allocator
    ///
    class FixedSizedMemoryAllocator
    {
    protected:

        // TODO: use big chunk of memory to increase performance and memory locality.

        ///
        /// Memory block header
        ///
        struct BlockHeader
        {
            // TODO: addd some checking pattern at head of block.
            BlockHeader * prev; ///< point to previous block. NULL means head block.
            BlockHeader * next; ///< point to next block. NULL means last block.
            BlockHeader * nextFree; ///< point to next free block.
            void * data() const { return (void*)( ((UInt8*)this) + (size_t)&((BlockHeader*)NULL)->nextFree ); } ///< return pointer to data area.
        };

        const size_t DATA_OFFSET; ///< offset of data area to the head of block.
        const size_t BLOCK_SIZE; ///< block size in bytes.

        BlockHeader * mAllBlocks; ///< point to first block
        BlockHeader * mFreeList; ///< point to first free block

    public:

        ///
        /// Default ctor.
        ///
        FixedSizedMemoryAllocator( size_t itemSize )
            : DATA_OFFSET( (size_t)&((BlockHeader*)NULL)->nextFree )
            , BLOCK_SIZE( (itemSize + DATA_OFFSET) > sizeof(BlockHeader)
                ? ( ( (itemSize + DATA_OFFSET) + (sizeof(void*)-1) ) & ~(sizeof(void*)-1) )
                : ( ( sizeof(BlockHeader) + (sizeof(void*)-1) ) & ~(sizeof(void*)-1) ) )
            , mAllBlocks(0)
            , mFreeList(0)
        {
            GN_ASSERT( BLOCK_SIZE >= sizeof(BlockHeader) && BLOCK_SIZE >= (DATA_OFFSET+itemSize) );
        }

        ///
        /// Default ctor.
        ///
        ~FixedSizedMemoryAllocator()
        {
            BlockHeader * p = mAllBlocks;
            while( mAllBlocks )
            {
                p = mAllBlocks;
                mAllBlocks = mAllBlocks->next;
                heapFree( p );
            }
        }

        ///
        /// Allocate raw memory for one block
        ///
        void * alloc()
        {
            BlockHeader * p;

            if( mFreeList )
            {
                // remove first block from free-list.
                p = mFreeList;
                mFreeList = mFreeList->nextFree;
            }
            else
            {
                // no free blocks. need to allocate a new block from heap
                p = (BlockHeader*)heapAlloc( BLOCK_SIZE );
                if( 0 == p ) return 0;

                // add to head of block list
                p->prev = 0;
                p->next = mAllBlocks;
                if( mAllBlocks ) mAllBlocks->prev = p;
                mAllBlocks = p;
            }

            return p->data();
        }

        ///
        /// Deallocate
        ///
        void dealloc( void * p )
        {
            if( 0 == p ) return;

            GN_ASSERT( (size_t)p > DATA_OFFSET );

            BlockHeader * b = (BlockHeader*)( ((UInt8*)p) - DATA_OFFSET );

            // TODO: check pattern at head of blcok. Make sure this is a valid memory block.

            // add to free list
            b->nextFree = mFreeList;
            mFreeList = b;
        }

        ///
        /// Shrink the allocator, free all unused blocks
        ///
        void shrink()
        {
            BlockHeader * p;
            while( mFreeList )
            {
                p = mFreeList;
                mFreeList = mFreeList->nextFree;

                // remove from all-blocks-list
                if( p->next ) p->next->prev = p->prev;
                if( p->prev )
                {
                    p->prev->next = p->next;
                }
                else
                {
                    // p must be the first block
                    mAllBlocks = p->next;
                }

                heapFree( p );
            }
        }
    };

    ///
    /// Fixed sized object allocator
    ///
    template<typename T>
    class FixedSizedObjectAllocator : public FixedSizedMemoryAllocator
    {
    public:

        ///
        /// Default ctor
        ///
        FixedSizedObjectAllocator() : FixedSizedMemoryAllocator( sizeof(T) ) {}

        ///
        /// Default ctor.
        ///
        ~FixedSizedObjectAllocator()
        {
            shrink();

            // call destructor of each one of remaining instances.
            BlockHeader * p = mAllBlocks;
            while( mAllBlocks )
            {
                p = mAllBlocks;
                mAllBlocks = mAllBlocks->next;
                ((T*)p->data())->~T();
                heapFree( p );
            }
        }
    };
}

// *****************************************************************************
//                           End of allocator.h
// *****************************************************************************
#endif // __GN_BASE_ALLOCATOR_H__

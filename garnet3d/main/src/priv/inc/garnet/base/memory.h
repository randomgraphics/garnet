#ifndef __GN_BASE_MEMORY_H__
#define __GN_BASE_MEMORY_H__
// *****************************************************************************
//! \file    memory.h
//! \brief   Basic memory management routines
//! \author  chenlee (2005.7.23)
// *****************************************************************************

#include <new>

//! \name macro to exception throw
//@{
#if GN_GCC
#define GN_THROW_BADALLOC() throw(std::bad_alloc)
#define GN_NOTHROW() throw()
#else
#define GN_THROW_BADALLOC()
#define GN_NOTHROW() throw()
#endif
//@}

namespace GN
{
    //!
    //! Allocate memory from heap. Can cross DLL boundary.
    //!
    void * memAlloc( size_t sizeInBytes );

    //!
    //! Re-allocate memory from heap. Can cross DLL boundary.
    //!
    void * memReAlloc( void *, size_t sizeInBytes );

    //!
    //! Free heap-allocated memory. Can cross DLL boundary.
    //!
    void memFree( void * );

    //!
    //! STL allocator that use garnet memory management routines.
    //!
    template<class T>
	class MemAllocator
	{
        //! \cond NEVER
    public:
    	typedef typename T         value_type;
    	typedef size_t             size_type;
    	typedef ptrdiff_t          difference_type;
    	typedef const value_type * const_pointer;
    	typedef const value_type & const_reference;
    	typedef value_type *       pointer;
    	typedef value_type &       reference;

    	template<class T2>
        struct rebind
        {
    		typedef MemAllocator<T2> other;
    	};

    	MemAllocator() GN_NOTHROW() {}

        ~MemAllocator() GN_NOTHROW() {}

    	MemAllocator( const MemAllocator<T> & ) GN_NOTHROW() {}

    	template<class T2>
    	MemAllocator( const MemAllocator<T2> & ) GN_NOTHROW() {}

    	template<class T2>
    	MemAllocator<T> & operator=( const MemAllocator<T2> & )
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
            return (pointer)memAlloc( count * sizeof(T) );
    	}

    	void deallocate( pointer ptr, size_type )
    	{
            memFree( ptr );
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
        //! \endcond
	};

    //!
    //! Fixed sized memory allocator
    //!
    class FixedSizedMemoryAllocator
    {
    protected:

        // TODO: use big chunk of memory to increase performance and memory locality.

        //!
        //! Memory block header
        //!
        struct BlockHeader
        {
            // TODO: addd some checking pattern at head of block.
            BlockHeader * prev; //!< point to previous block. NULL means head block.
            BlockHeader * next; //!< point to next block. NULL means last block.
            BlockHeader * nextFree; //!< point to next free block.
            void * data() const { return (void*)( ((uint8_t*)this) + (size_t)&((BlockHeader*)NULL)->nextFree ); } //!< return pointer to data area.
        };

        const size_t DATA_OFFSET; //!< offset of data area to the head of block.
        const size_t BLOCK_SIZE; //!< block size in bytes.

        BlockHeader * mAllBlocks; //!< point to first block
        BlockHeader * mFreeList; //!< point to first free block

    public:

        //!
        //! Default ctor.
        //!
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

        //!
        //! Default ctor.
        //!
        ~FixedSizedMemoryAllocator()
        {
            BlockHeader * p = mAllBlocks;
            while( mAllBlocks )
            {
                p = mAllBlocks;
                mAllBlocks = mAllBlocks->next;
                memFree( p );
            }
        }

        //!
        //! Allocate raw memory for one block
        //!
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
                p = (BlockHeader*)memAlloc( BLOCK_SIZE );
                if( 0 == p ) return 0;

                // add to head of block list
                p->prev = 0;
                p->next = mAllBlocks;
                if( mAllBlocks ) mAllBlocks->prev = p;
                mAllBlocks = p;
            }

            return p->data();
        }

        //!
        //! Deallocate
        //!
        void dealloc( void * p )
        {
            if( 0 == p ) return;

            GN_ASSERT( (size_t)p > DATA_OFFSET );

            BlockHeader * b = (BlockHeader*)( ((uint8_t*)p) - DATA_OFFSET );

            // TODO: check pattern at head of blcok. Make sure this is a valid memory block.

            // add to free list
            b->nextFree = mFreeList;
            mFreeList = b;
        }

        //!
        //! Shrink the allocator, free all unused blocks
        //!
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

                memFree( p );
            }
        }
    };

    //!
    //! Fixed sized object allocator
    //!
    template<typename T>
    class FixedSizedObjectAllocator : public FixedSizedMemoryAllocator
    {
    public:

        //!
        //! Default ctor
        //!
        FixedSizedObjectAllocator() : FixedSizedMemoryAllocator( sizeof(T) ) {}

        //!
        //! Default ctor.
        //!
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
                memFree( p );
            }
        }
    };
}

//! \name overloaded global new and delete operators
//@{
// TODO: more standard conforming implementation.
inline void * operator new( size_t s ) GN_THROW_BADALLOC() { return ::GN::memAlloc( s ); }
inline void * operator new[]( size_t s ) GN_THROW_BADALLOC() { return ::GN::memAlloc( s ); }
inline void operator delete( void* p ) GN_NOTHROW() { ::GN::memFree( p ); }
inline void operator delete[]( void* p ) GN_NOTHROW() { ::GN::memFree( p ); }
//@}

// *****************************************************************************
//                           End of memory.h
// *****************************************************************************
#endif // __GN_BASE_MEMORY_H__

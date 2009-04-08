#ifndef __GN_BASE_ALLOCATOR_H__
#define __GN_BASE_ALLOCATOR_H__
// *****************************************************************************
/// \file
/// \brief   Common allocator classes
/// \author  chenlee (2006.5.31)
// *****************************************************************************

#include <new> // for std::bad_alloc

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

        pointer allocate( size_type count )
        {
            void * p = heapAlloc( count * sizeof(T) );
            if( NULL == p )
            {
                throw std::bad_alloc();
            }
            return static_cast<pointer>(p);
        }

        void deallocate( pointer ptr, size_type )
        {
            heapFree( ptr );
        }

        void construct( pointer ptr, const_reference x )
        {
            new (ptr) T(x);
        }

        void construct( pointer ptr )
        {
            new (ptr) T;
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
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ALLOCATOR_H__

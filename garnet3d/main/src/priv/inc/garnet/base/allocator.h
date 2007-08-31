#ifndef __GN_BASE_ALLOCATOR_H__
#define __GN_BASE_ALLOCATOR_H__
// *****************************************************************************
/// \file
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
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ALLOCATOR_H__

#ifndef __GN_BASE_MEMORY_H__
#define __GN_BASE_MEMORY_H__
// *****************************************************************************
/// \file
/// \brief   Basic memory management routines
/// \author  chenlee (2005.7.23)
// *****************************************************************************

#include <new>

/// \name macro to exception throw
//@{
#if GN_GCC
#define GN_THROW_BADALLOC() throw(std::bad_alloc)
#define GN_NOTHROW() throw()
#else
#define GN_THROW_BADALLOC()
#define GN_NOTHROW() throw()
#endif
//@}

// *****************************************************************************
// Basic memory allocation/deallocation routines
// *****************************************************************************

namespace GN
{
    namespace HeapMemory
    {
        ///
        /// Allocate memory from heap. Can cross DLL boundary.
        ///
        GN_PUBLIC void * alloc( size_t sizeInBytes );

        ///
        /// Re-allocate memory from heap. Can cross DLL boundary.
        ///
        GN_PUBLIC void * realloc( void * ptr, size_t sizeInBytes );

        ///
        /// Allocate aligned memory from heap. Can cross DLL boundary
        ///
        GN_PUBLIC void * alignedAlloc( size_t sizeInBytes, size_t alignment );

        ///
        /// Re-allocate aligned memory from heap. Can cross DLL boundary
        ///
        GN_PUBLIC void * alignedRealloc( void * ptr, size_t sizeInBytes, size_t alignment );

        ///
        /// Free heap-allocated memory (aligned or unaligned). Can cross DLL boundary.
        ///
        GN_PUBLIC void dealloc( void * ptr );
    };
}

// *****************************************************************************
// Global new/delete operators
// *****************************************************************************

#if GN_ICL
#pragma warning(push)
#pragma warning(disable:522) // inline function is declared after being used.
#endif

/// \name overloaded global new and delete operators
//@{
// TODO: more standard conforming implementation.
inline void * operator new( size_t s ) GN_THROW_BADALLOC() { return ::GN::HeapMemory::alloc( s ); }
inline void * operator new[]( size_t s ) GN_THROW_BADALLOC() { return ::GN::HeapMemory::alloc( s ); }
inline void operator delete( void* p ) GN_NOTHROW() { ::GN::HeapMemory::dealloc( p ); }
inline void operator delete[]( void* p ) GN_NOTHROW() { ::GN::HeapMemory::dealloc( p ); }
//@}

#if GN_ICL
#pragma warning(pop)
#endif

// *****************************************************************************
// Other common memory management utilities
// *****************************************************************************

namespace GN
{
    ///
    /// enable CRT memory leak checking. Currently only work for MSVC compiler
    ///
    /// \param breakOnAllocID       Set allocation ID for "break-on-memory-allocation". Set to 0 to disable it.
    void enableCRTMemoryCheck( long breakOnAllocID = 0 );

    ///
    /// free heap memory pointer allocated using GN::HeapMemory functions
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeHeapDealloc( T * & ptr )
    {
        if( ptr )
        {
            HeapMemory::dealloc( ptr );
            ptr = 0;
        }
    }

    ///
    /// delete one object
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeDelete( T * & ptr )
    {
        if( ptr )
        {
            delete ptr;
            ptr = 0;
        }
    }

    ///
    /// delete object array
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeDeleteArray( T * & ptr )
    {
        if( ptr )
        {
            delete [] ptr;
            ptr = 0;
        }
    }

    ///
    /// Safe release COM interface
    ///
    template < typename T >
    GN_FORCE_INLINE void safeRelease( T * & ptr )
    {
        if( ptr )
        {
            ptr->Release();
            ptr = 0;
        }
    }

    ///
    /// Safe release RefCounter class
    ///
    template < typename T >
    GN_FORCE_INLINE void safeDecref( T * & ptr )
    {
        if( ptr )
        {
            ptr->decref();
            ptr = 0;
        }
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_MEMORY_H__

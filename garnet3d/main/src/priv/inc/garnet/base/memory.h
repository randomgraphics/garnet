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
    ///
    /// Allocate memory from heap. Can cross DLL boundary.
    ///
    GN_PUBLIC void * HeapAlloc( size_t sizeInBytes );

    ///
    /// Re-allocate memory from heap. Can cross DLL boundary.
    ///
    GN_PUBLIC void * HeapRealloc( void *, size_t sizeInBytes );

    ///
    /// Free heap-allocated memory. Can cross DLL boundary.
    ///
    GN_PUBLIC void HeapFree( void * );
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
inline void * operator new( size_t s ) GN_THROW_BADALLOC() { return ::GN::HeapAlloc( s ); }
inline void * operator new[]( size_t s ) GN_THROW_BADALLOC() { return ::GN::HeapAlloc( s ); }
inline void operator delete( void* p ) GN_NOTHROW() { ::GN::HeapFree( p ); }
inline void operator delete[]( void* p ) GN_NOTHROW() { ::GN::HeapFree( p ); }
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
    void EnableCRTMemoryCheck( long breakOnAllocID = 0 );

    ///
    /// general safe deallocation routine
    // ------------------------------------------------------------------------
    template < typename T, typename DEALLOC_FUNC >
    GN_FORCE_INLINE void SafeDealloc( T * & ptr )
    {
        if( ptr )
        {
            DEALLOC_FUNC( ptr );
            ptr = 0;
        }
    }

    ///
    /// free a C-style heap pointer
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void SafeHeapFree( T * & ptr )
    {
        if( ptr )
        {
            HeapFree( ptr );
            ptr = 0;
        }
    }

    ///
    /// delete one object
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void SafeDelete( T * & ptr )
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
    GN_FORCE_INLINE void SafeDeleteArray( T * & ptr )
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
    GN_FORCE_INLINE void SafeRelease( T * & ptr )
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
    GN_FORCE_INLINE void SafeDecref( T * & ptr )
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

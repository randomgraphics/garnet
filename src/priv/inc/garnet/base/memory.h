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

namespace GN
{
    ///
    /// Allocate memory from heap. Can cross DLL boundary.
    ///
    GN_PUBLIC void * heapAlloc( size_t sizeInBytes );

    ///
    /// Re-allocate memory from heap. Can cross DLL boundary.
    ///
    GN_PUBLIC void * heapRealloc( void *, size_t sizeInBytes );

    ///
    /// Free heap-allocated memory. Can cross DLL boundary.
    ///
    GN_PUBLIC void heapFree( void * );

    ///
    /// enable CRT memory leak checking. Currently only work for MSVC compiler
    ///
    /// \param breakOnAllocID       Set allocation ID for "break-on-memory-allocation". Set to 0 to disable it.
    void enableCRTMemoryCheck( long breakOnAllocID = 0 );
}

#if GN_ICL
#pragma warning(push)
#pragma warning(disable:522) // inline function is declared after being used.
#endif

/// \name overloaded global new and delete operators
//@{
// TODO: more standard conforming implementation.
inline void * operator new( size_t s ) GN_THROW_BADALLOC() { return ::GN::heapAlloc( s ); }
inline void * operator new[]( size_t s ) GN_THROW_BADALLOC() { return ::GN::heapAlloc( s ); }
inline void operator delete( void* p ) GN_NOTHROW() { ::GN::heapFree( p ); }
inline void operator delete[]( void* p ) GN_NOTHROW() { ::GN::heapFree( p ); }
//@}

#if GN_ICL
#pragma warning(pop)
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_MEMORY_H__

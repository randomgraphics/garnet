#ifndef __GN_BASE_MEMORY_H__
#define __GN_BASE_MEMORY_H__
// *****************************************************************************
//! \file    memory.h
//! \brief   Basic memory management routines
//! \author  chenlee (2005.7.23)
// *****************************************************************************

namespace GN
{
    //!
    //! Allocate memory from heap. Can cross DLL boundary.
    //!
    void * memAlloc( size_t sizeInBytes );

    //!
    //! Allocate typed memory from heap. Can cross DLL boundary.
    //!
    template<typename T>
    inline T * memAllocT( size_t elementCount )
    {
        return (T*)memAlloc( sizeof(T)*elementCount );
    }

    //!
    //! Re-allocate memory from heap. Can cross DLL boundary.
    //!
    void * memReAlloc( void *, size_t sizeInBytes );

    //!
    //! Re-allocate typed memory from heap. Can cross DLL boundary.
    //!
    template<typename T>
    inline T * memReAllocT( void * ptr, size_t elementCount )
    {
        return (T*)memReAlloc( ptr, sizeof(T)*elementCount );
    }

    //!
    //! Free heap-allocated memory. Can cross DLL boundary.
    //!
    void memFree( void * );
}

//! \name overloaded global new and delete operators
//@{
inline void * operator new( size_t s ) { return ::GN::memAlloc( s ); }
inline void * operator new[]( size_t s ) { return ::GN::memAlloc( s ); }
inline void operator delete( void* p ) { return ::GN::memFree( p ); }
inline void operator delete[]( void* p ) { return ::GN::memFree( p ); }
//@}

// *****************************************************************************
//                           End of memory.h
// *****************************************************************************
#endif // __GN_BASE_MEMORY_H__

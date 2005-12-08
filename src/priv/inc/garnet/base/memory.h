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
    void * memAlloc( size_t );

    //!
    //! Re-allocate memory from heap. Can cross DLL boundary.
    //!
    void * memReAlloc( void *, size_t );

    //!
    //! Free heap-allocated memory. Can cross DLL boundary.
    //!
    void memFree( void * );
}

// *****************************************************************************
//                           End of memory.h
// *****************************************************************************
#endif // __GN_BASE_MEMORY_H__


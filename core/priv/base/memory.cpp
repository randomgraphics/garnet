#include "pch.h"

namespace GN
{
    void * memAlloc( size_t sz )
    {
        #if GN_WINNT
        void * ptr = ::HeapAlloc( ::GetProcessHeap(), 0, sz );
        #else
        void * ptr = ::malloc( sz );
        #endif
        if ( 0 == ptr ) GN_ERROR( "out of memory!" );
        return ptr;
    }

    void * memReAlloc( void * ptr, size_t sz )
    {
        #if GN_WINNT
        ptr = ::HeapReAlloc( ::GetProcessHeap(), 0, ptr, sz );
        #else
        ptr = ::realloc( ptr, sz );
        #endif
        if ( 0 == ptr ) GN_ERROR( "out of memory!" );
        return ptr;
    }

    void memFree( void * ptr )
    {
        #if GN_WINNT
        ::HeapFree( ::GetProcessHeap(), 0, ptr );
        #else
        ::free( ptr );
        #endif
    }
}


#include "pch.h"
#include "garnet/GNinput.h"

// implement global singletons
GN_IMPLEMENT_CROSS_DLL_SINGLETON( GN::input::Input )

namespace GN
{
    // Global runtime assert behavior flag.
    GN_PUBLIC RuntimeAssertBehavior gRuntimeAssertBehavior = RAB_ASK_USER;

    static Logger * sHeapLogger = getLogger("GN.core.heapAllocation");

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC ProfilerManager & ProfilerManager::sGetGlobalInstance()
    {
        static GN::ProfilerManager sInstance;
        return sInstance;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC void * heapAlloc( size_t sz )
    {
        //#if GN_MSWIN
        //void * ptr = ::HeapAlloc( ::GetProcessHeap(), 0, sz );
        //#else
        void * ptr = ::malloc( sz );
        //#endif
        if ( 0 == ptr )
        {
            GN_ERROR(sHeapLogger)( "out of memory!" );
        }
        return ptr;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC void * heapRealloc( void * ptr, size_t sz )
    {
        //#if GN_MSWIN
        //ptr = ::HeapReAlloc( ::GetProcessHeap(), 0, ptr, sz );
        //#else
        ptr = ::realloc( ptr, sz );
        //#endif
        if ( 0 == ptr ) { GN_ERROR(sHeapLogger)( "out of memory!" ); }
        return ptr;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC void heapFree( void * ptr )
    {
        //#if GN_MSWIN
        //::HeapFree( ::GetProcessHeap(), 0, ptr );
        //#else
        ::free( ptr );
        //#endif
    }
}

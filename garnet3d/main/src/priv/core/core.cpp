#include "pch.h"
#include "garnet/GNcore.h"
#include "garnet/GNinput.h"

// implement global singletons
GN_IMPLEMENT_CROSS_DLL_SINGLETON( GN::PluginManager )
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
        if ( 0 == ptr ) GN_ERROR(sHeapLogger)( "out of memory!" );
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
        if ( 0 == ptr ) GN_ERROR(sHeapLogger)( "out of memory!" );
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

    namespace input
    {
        GN_PUBLIC Signal1<void,KeyEvent> & getSigKeyPress() { static Signal1<void,KeyEvent> s; return s; }
        GN_PUBLIC Signal1<void,wchar_t>  & getSigCharPress() { static Signal1<void,wchar_t> s; return s; }
        GN_PUBLIC Signal2<void,Axis,int> & getSigAxisMove() { static Signal2<void,Axis,int> s; return s; }
    }

    namespace gfx
    {
        struct Renderer;

        typedef Signal4<void, Renderer&, HandleType, UInt32, UInt32> SizeMoveSignal;

        GN_PUBLIC Signal1<void,Renderer&>  & gSigRendererDeviceLost() { static Signal1<void,Renderer&> s; return s; }
        GN_PUBLIC SizeMoveSignal           & getSigRendererWindowSizeMove() { static SizeMoveSignal s; return s; }
        GN_PUBLIC Signal1<void,Renderer&>  & getSigRendererWindowClose() { static Signal1<void,Renderer&> s; return s; }
    };
}

#include "pch.h"
#include "garnet/GNinput.h"

// implement global singletons
GN_IMPLEMENT_CROSS_DLL_SINGLETON( GN::input::Input )

namespace GN
{
    // Global runtime assert behavior flag.
    GN_PUBLIC RuntimeAssertBehavior gRuntimeAssertBehavior = RAB_ASK_USER;

    static Logger * sHeapLogger = getLogger("GN.core.heapAllocation");

    const char TAG[] = "garnet3d_memory";
    GN_CASSERT( sizeof(TAG) == 16 );

    struct MemoryHeader
    {
        char   tag[16];  // special memory tag.
        uint32 alignment;
        uint32 offset;
    };

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC ProfilerManager & ProfilerManager::sGetGlobalInstance()
    {
        static ProfilerManager sInstance = ProfilerManager();
        return sInstance;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC void * HeapMemory::alloc( size_t sz )
    {
        return HeapMemory::alignedAlloc( sz, 1 );
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC void * HeapMemory::realloc( void * ptr, size_t sz )
    {
        return HeapMemory::alignedRealloc( ptr, sz, 1 );
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC void * HeapMemory::alignedAlloc( size_t sizeInBytes, size_t alignment )
    {
        if( 0 == alignment ) alignment = 1;
        void * ptr = _aligned_malloc( sizeInBytes, alignment );
        if ( 0 == ptr ) GN_ERROR(sHeapLogger)( "out of memory!" );
        return ptr;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC void * HeapMemory::alignedRealloc( void * ptr, size_t sizeInBytes, size_t alignment )
    {
        if( 0 == alignment ) alignment = 1;
        ptr = _aligned_realloc( ptr, sizeInBytes, alignment );
        if ( 0 == ptr ) { GN_ERROR(sHeapLogger)( "out of memory!" ); }
        return ptr;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_PUBLIC void HeapMemory::dealloc( void * ptr )
    {
        return _aligned_free( ptr );
    }
}

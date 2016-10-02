#include "pch.h"
#include "garnet/GNinput.h"
#include <malloc.h>

// implement global singletons
GN_IMPLEMENT_CROSS_DLL_SINGLETON( GN::input::Input )

namespace GN
{
    static Logger * sHeapLogger = GetLogger("GN.core.heapAllocation");

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
    GN_API ProfilerManager & ProfilerManager::sGetGlobalInstance()
    {
        static ProfilerManager sInstance = ProfilerManager();
        return sInstance;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_API void * HeapMemory::Alloc( size_t sz )
    {
        return HeapMemory::AlignedAlloc( sz, 0 );
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_API void * HeapMemory::Realloc( void * ptr, size_t sz )
    {
        return HeapMemory::AlignedRealloc( ptr, sz, 0 );
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_API void * HeapMemory::AlignedAlloc( size_t sizeInBytes, size_t alignment )
    {
        if( 0 == alignment ) alignment = sizeof(size_t);
#if GN_POSIX
        void * ptr = memalign( alignment, sizeInBytes );
#else
        void * ptr = _aligned_malloc( sizeInBytes, alignment );
#endif
        if ( 0 == ptr )
        {
            GN_ERROR(sHeapLogger)( "out of memory!" );
        }
        return ptr;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_API void * HeapMemory::AlignedRealloc( void * ptr, size_t sizeInBytes, size_t alignment )
    {
        if( 0 == alignment ) alignment = sizeof(size_t);
#if GN_POSIX
        ptr = realloc( ptr, sizeInBytes );
#else
        ptr = _aligned_realloc( ptr, sizeInBytes, alignment );
        if ( 0 == ptr ) { GN_ERROR(sHeapLogger)( "out of memory!" ); }
#endif
        return ptr;
    }

    //
    //
    // -----------------------------------------------------------------------------
    GN_API void HeapMemory::Dealloc( void * ptr )
    {
#if GN_POSIX
        return ::free( ptr );
#else
        return _aligned_free( ptr );
#endif
    }
}

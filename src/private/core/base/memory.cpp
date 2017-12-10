#include "pch.h"

// TODO: more standard conforming implementation.
void * operator new( size_t s ) GN_THROW_BADALLOC() { return ::GN::HeapMemory::alloc( s ); }
void * operator new[]( size_t s ) GN_THROW_BADALLOC() { return ::GN::HeapMemory::alloc( s ); }
void operator delete( void* p ) GN_NOTHROW() { ::GN::HeapMemory::dealloc( p ); }
void operator delete[]( void* p ) GN_NOTHROW() { ::GN::HeapMemory::dealloc( p ); }

//
//
// -----------------------------------------------------------------------------
#if GN_MSVC
#include <crtdbg.h>
GN_API void GN::enableCRTMemoryCheck( long breakOnAllocID )
{
    int tmpDbgFlag;
    tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);

    if( 0 != breakOnAllocID )
    {
        _CrtSetBreakAlloc( breakOnAllocID );
    }
}
#else
GN_API void GN::enableCRTMemoryCheck( long ) {}
#endif

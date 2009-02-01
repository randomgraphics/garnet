#include "pch.h"

//
//
// -----------------------------------------------------------------------------
#if GN_MSVC
#include <crtdbg.h>
void GN::enableCRTMemoryCheck( long breakOnAllocID )
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
void GN::enableCRTMemoryCheck( long ) {}
#endif

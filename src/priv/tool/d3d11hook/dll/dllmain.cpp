#include "pch.h"
#include "interfacebase.h"

BOOL WINAPI DllMain( HINSTANCE, DWORD fdwReason, LPVOID ) {
	if ( fdwReason == DLL_PROCESS_ATTACH )
    {
        HookedClassFactory::sGetInstance().registerAll();
	} else if ( fdwReason == DLL_PROCESS_DETACH )
	{
        // TODO: cleanup.
	}
	return TRUE;
}

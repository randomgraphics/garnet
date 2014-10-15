#include "pch.h"

/*SDLLHook D3DHook =
{
    "d3d9.dll",
    false, NULL,    // Default hook disabled, NULL function pointer.
    {
        { "Direct3DCreate9",   MyCreate },
        { "Direct3DCreate9Ex", MyCreateEx },
        { NULL, NULL }
    }
};*/

BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    GN_UNUSED_PARAM(hModule);
    GN_UNUSED_PARAM(fdwReason);
    GN_UNUSED_PARAM(lpReserved);
    /*if ( fdwReason == DLL_PROCESS_ATTACH )  // When initializing....
    {
        hDLL = hModule;

        // We don't need thread notifications for what we're doing.  Thus,
        // get rid of them, thereby eliminating some of the overhead of
        // this DLL
        DisableThreadLibraryCalls( hModule );

        // Only hook the APIs if this is the right process.
        GetModuleFileName( GetModuleHandle( NULL ), Work, sizeof(Work) );
        PathStripPath( Work );

        if ( stricmp( Work, "ffxiiiimg.exe" ) == 0 )
            HookAPICalls( &D3DHook );
    }*/
    return TRUE;
}

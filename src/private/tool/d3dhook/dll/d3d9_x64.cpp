#include "pch.h"
#include "hooks_exports.h"

HINSTANCE mHinst = 0, mHinstDLL = 0;
extern "C" UINT_PTR mProcs[16] = {0};

LPCSTR mImportNames[] = {"D3DPERF_BeginEvent", "D3DPERF_EndEvent", "D3DPERF_GetStatus", "D3DPERF_QueryRepeatFrame", "D3DPERF_SetMarker", "D3DPERF_SetOptions", "D3DPERF_SetRegion", "DebugSetLevel", "DebugSetMute", "Direct3D9EnableMaximizedWindowedModeShim", "Direct3DCreate9", "Direct3DCreate9Ex", "Direct3DShaderValidatorCreate9", "PSGPError", "PSGPSampleTexture", (LPCSTR)16};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID )
{
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		mHinstDLL = LoadLibraryW( GetRealDllPath(L"d3d9.dll") );
		if ( !mHinstDLL )
			return ( FALSE );
		for ( int i = 0; i < 16; i++ )
			mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );
	} else if ( fdwReason == DLL_PROCESS_DETACH ) {
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" void D3DPERF_BeginEvent_wrapper();
extern "C" void D3DPERF_EndEvent_wrapper();
extern "C" void D3DPERF_GetStatus_wrapper();
extern "C" void D3DPERF_QueryRepeatFrame_wrapper();
extern "C" void D3DPERF_SetMarker_wrapper();
extern "C" void D3DPERF_SetOptions_wrapper();
extern "C" void D3DPERF_SetRegion_wrapper();
extern "C" void DebugSetLevel_wrapper();
extern "C" void DebugSetMute_wrapper();
extern "C" void Direct3D9EnableMaximizedWindowedModeShim_wrapper();
#if 0
extern "C" void Direct3DCreate9_wrapper();
extern "C" void Direct3DCreate9Ex_wrapper();
#else
extern "C" IDirect3D9 * WINAPI Direct3DCreate9_wrapper(UINT SDKVersion)
{
    return Direct3DCreate9Hook(SDKVersion);
}
extern "C" HRESULT WINAPI Direct3DCreate9Ex_wrapper(UINT SDKVersion, IDirect3D9Ex **ppD3D)
{
    return Direct3DCreate9ExHook(SDKVersion, ppD3D);
}
#endif
extern "C" void Direct3DShaderValidatorCreate9_wrapper();
extern "C" void PSGPError_wrapper();
extern "C" void PSGPSampleTexture_wrapper();
extern "C" void ExportByOrdinal16();

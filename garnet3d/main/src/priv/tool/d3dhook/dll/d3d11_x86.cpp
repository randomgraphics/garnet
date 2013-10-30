#include "pch.h"
#include "d3d11hooks_exports.h"

HINSTANCE mHinst = 0, mHinstDLL = 0;
UINT_PTR mProcs[47] = {0};

LPCSTR mImportNames[] = {"D3D11CoreCreateDevice", "D3D11CoreCreateLayeredDevice", "D3D11CoreGetLayeredDeviceSize", "D3D11CoreRegisterLayers", "D3D11CreateDevice", "D3D11CreateDeviceAndSwapChain", "D3DKMTCloseAdapter", "D3DKMTCreateAllocation", "D3DKMTCreateContext", "D3DKMTCreateDevice", "D3DKMTCreateSynchronizationObject", "D3DKMTDestroyAllocation", "D3DKMTDestroyContext", "D3DKMTDestroyDevice", "D3DKMTDestroySynchronizationObject", "D3DKMTEscape", "D3DKMTGetContextSchedulingPriority", "D3DKMTGetDeviceState", "D3DKMTGetDisplayModeList", "D3DKMTGetMultisampleMethodList", "D3DKMTGetRuntimeData", "D3DKMTGetSharedPrimaryHandle", "D3DKMTLock", "D3DKMTOpenAdapterFromHdc", "D3DKMTOpenResource", "D3DKMTPresent", "D3DKMTQueryAdapterInfo", "D3DKMTQueryAllocationResidency", "D3DKMTQueryResourceInfo", "D3DKMTRender", "D3DKMTSetAllocationPriority", "D3DKMTSetContextSchedulingPriority", "D3DKMTSetDisplayMode", "D3DKMTSetDisplayPrivateDriverFormat", "D3DKMTSetGammaRamp", "D3DKMTSetVidPnSourceOwner", "D3DKMTSignalSynchronizationObject", "D3DKMTUnlock", "D3DKMTWaitForSynchronizationObject", "D3DKMTWaitForVerticalBlankEvent", "D3DPerformance_BeginEvent", "D3DPerformance_EndEvent", "D3DPerformance_GetStatus", "D3DPerformance_SetMarker", "EnableFeatureLevelUpgrade", "OpenAdapter10", "OpenAdapter10_2"};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID ) {
	mHinst = hinstDLL;

    wchar_t dllpath[256];
    GetSystemDirectoryW(dllpath, _countof(dllpath));
    wcscat_s(dllpath, L"\\d3d11.dll");

	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		mHinstDLL = LoadLibraryW( dllpath );
		if ( !mHinstDLL )
			return ( FALSE );
		for ( int i = 0; i < 47; i++ )
			mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );
	} else if ( fdwReason == DLL_PROCESS_DETACH ) {
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" __declspec(naked) void __stdcall D3D11CoreCreateDevice_wrapper(){__asm{jmp mProcs[0*4]}}
extern "C" __declspec(naked) void __stdcall D3D11CoreCreateLayeredDevice_wrapper(){__asm{jmp mProcs[1*4]}}
extern "C" __declspec(naked) void __stdcall D3D11CoreGetLayeredDeviceSize_wrapper(){__asm{jmp mProcs[2*4]}}
extern "C" __declspec(naked) void __stdcall D3D11CoreRegisterLayers_wrapper(){__asm{jmp mProcs[3*4]}}
#if 0
extern "C" __declspec(naked) void __stdcall D3D11CreateDevice_wrapper(){__asm{jmp mProcs[4*4]}}
extern "C" __declspec(naked) void __stdcall D3D11CreateDeviceAndSwapChain_wrapper(){__asm{jmp mProcs[5*4]}}
#else
extern "C" HRESULT WINAPI
D3D11CreateDevice_wrapper(
    __in_opt IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    __in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    __out_opt ID3D11Device** ppDevice,
    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
    __out_opt ID3D11DeviceContext** ppImmediateContext )
{
    return D3D11CreateDeviceHook(
        pAdapter,
        DriverType,
        Software,
        Flags,
        pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);
}

extern "C" HRESULT WINAPI
D3D11CreateDeviceAndSwapChain_wrapper(
    __in_opt IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    __in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    __in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
    __out_opt IDXGISwapChain** ppSwapChain,
    __out_opt ID3D11Device** ppDevice,
    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
    __out_opt ID3D11DeviceContext** ppImmediateContext )
{
    return D3D11CreateDeviceAndSwapChainHook(
        pAdapter,
        DriverType,
        Software,
        Flags,
        pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext );
}
#endif
extern "C" __declspec(naked) void __stdcall D3DKMTCloseAdapter_wrapper(){__asm{jmp mProcs[6*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTCreateAllocation_wrapper(){__asm{jmp mProcs[7*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTCreateContext_wrapper(){__asm{jmp mProcs[8*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTCreateDevice_wrapper(){__asm{jmp mProcs[9*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTCreateSynchronizationObject_wrapper(){__asm{jmp mProcs[10*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTDestroyAllocation_wrapper(){__asm{jmp mProcs[11*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTDestroyContext_wrapper(){__asm{jmp mProcs[12*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTDestroyDevice_wrapper(){__asm{jmp mProcs[13*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTDestroySynchronizationObject_wrapper(){__asm{jmp mProcs[14*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTEscape_wrapper(){__asm{jmp mProcs[15*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTGetContextSchedulingPriority_wrapper(){__asm{jmp mProcs[16*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTGetDeviceState_wrapper(){__asm{jmp mProcs[17*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTGetDisplayModeList_wrapper(){__asm{jmp mProcs[18*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTGetMultisampleMethodList_wrapper(){__asm{jmp mProcs[19*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTGetRuntimeData_wrapper(){__asm{jmp mProcs[20*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTGetSharedPrimaryHandle_wrapper(){__asm{jmp mProcs[21*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTLock_wrapper(){__asm{jmp mProcs[22*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTOpenAdapterFromHdc_wrapper(){__asm{jmp mProcs[23*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTOpenResource_wrapper(){__asm{jmp mProcs[24*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTPresent_wrapper(){__asm{jmp mProcs[25*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTQueryAdapterInfo_wrapper(){__asm{jmp mProcs[26*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTQueryAllocationResidency_wrapper(){__asm{jmp mProcs[27*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTQueryResourceInfo_wrapper(){__asm{jmp mProcs[28*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTRender_wrapper(){__asm{jmp mProcs[29*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTSetAllocationPriority_wrapper(){__asm{jmp mProcs[30*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTSetContextSchedulingPriority_wrapper(){__asm{jmp mProcs[31*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTSetDisplayMode_wrapper(){__asm{jmp mProcs[32*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTSetDisplayPrivateDriverFormat_wrapper(){__asm{jmp mProcs[33*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTSetGammaRamp_wrapper(){__asm{jmp mProcs[34*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTSetVidPnSourceOwner_wrapper(){__asm{jmp mProcs[35*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTSignalSynchronizationObject_wrapper(){__asm{jmp mProcs[36*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTUnlock_wrapper(){__asm{jmp mProcs[37*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTWaitForSynchronizationObject_wrapper(){__asm{jmp mProcs[38*4]}}
extern "C" __declspec(naked) void __stdcall D3DKMTWaitForVerticalBlankEvent_wrapper(){__asm{jmp mProcs[39*4]}}
extern "C" __declspec(naked) void __stdcall D3DPerformance_BeginEvent_wrapper(){__asm{jmp mProcs[40*4]}}
extern "C" __declspec(naked) void __stdcall D3DPerformance_EndEvent_wrapper(){__asm{jmp mProcs[41*4]}}
extern "C" __declspec(naked) void __stdcall D3DPerformance_GetStatus_wrapper(){__asm{jmp mProcs[42*4]}}
extern "C" __declspec(naked) void __stdcall D3DPerformance_SetMarker_wrapper(){__asm{jmp mProcs[43*4]}}
extern "C" __declspec(naked) void __stdcall EnableFeatureLevelUpgrade_wrapper(){__asm{jmp mProcs[44*4]}}
extern "C" __declspec(naked) void __stdcall OpenAdapter10_wrapper(){__asm{jmp mProcs[45*4]}}
extern "C" __declspec(naked) void __stdcall OpenAdapter10_2_wrapper(){__asm{jmp mProcs[46*4]}}

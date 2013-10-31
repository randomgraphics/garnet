#include "pch.h"
#include "d3d11hooks_exports.h"

HINSTANCE mHinst = 0, mHinstDLL = 0;
extern "C" UINT_PTR mProcs[47] = {0};

LPCSTR mImportNames[] = {"D3D11CoreCreateDevice", "D3D11CoreCreateLayeredDevice", "D3D11CoreGetLayeredDeviceSize", "D3D11CoreRegisterLayers", "D3D11CreateDevice", "D3D11CreateDeviceAndSwapChain", "D3DKMTCloseAdapter", "D3DKMTCreateAllocation", "D3DKMTCreateContext", "D3DKMTCreateDevice", "D3DKMTCreateSynchronizationObject", "D3DKMTDestroyAllocation", "D3DKMTDestroyContext", "D3DKMTDestroyDevice", "D3DKMTDestroySynchronizationObject", "D3DKMTEscape", "D3DKMTGetContextSchedulingPriority", "D3DKMTGetDeviceState", "D3DKMTGetDisplayModeList", "D3DKMTGetMultisampleMethodList", "D3DKMTGetRuntimeData", "D3DKMTGetSharedPrimaryHandle", "D3DKMTLock", "D3DKMTOpenAdapterFromHdc", "D3DKMTOpenResource", "D3DKMTPresent", "D3DKMTQueryAdapterInfo", "D3DKMTQueryAllocationResidency", "D3DKMTQueryResourceInfo", "D3DKMTRender", "D3DKMTSetAllocationPriority", "D3DKMTSetContextSchedulingPriority", "D3DKMTSetDisplayMode", "D3DKMTSetDisplayPrivateDriverFormat", "D3DKMTSetGammaRamp", "D3DKMTSetVidPnSourceOwner", "D3DKMTSignalSynchronizationObject", "D3DKMTUnlock", "D3DKMTWaitForSynchronizationObject", "D3DKMTWaitForVerticalBlankEvent", "D3DPerformance_BeginEvent", "D3DPerformance_EndEvent", "D3DPerformance_GetStatus", "D3DPerformance_SetMarker", "EnableFeatureLevelUpgrade", "OpenAdapter10", "OpenAdapter10_2"};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID ) {
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		mHinstDLL = LoadLibraryW( GetRealDllPath(L"d3d11.dll") );
		if ( !mHinstDLL )
			return ( FALSE );
		for ( int i = 0; i < 47; i++ )
			mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );
	} else if ( fdwReason == DLL_PROCESS_DETACH ) {
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" void D3D11CoreCreateDevice_wrapper();
extern "C" void D3D11CoreCreateLayeredDevice_wrapper();
extern "C" void D3D11CoreGetLayeredDeviceSize_wrapper();
extern "C" void D3D11CoreRegisterLayers_wrapper();
#if 0
extern "C" void D3D11CreateDevice_wrapper();
extern "C" void D3D11CreateDeviceAndSwapChain_wrapper();
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

extern "C" void D3DKMTCloseAdapter_wrapper();
extern "C" void D3DKMTCreateAllocation_wrapper();
extern "C" void D3DKMTCreateContext_wrapper();
extern "C" void D3DKMTCreateDevice_wrapper();
extern "C" void D3DKMTCreateSynchronizationObject_wrapper();
extern "C" void D3DKMTDestroyAllocation_wrapper();
extern "C" void D3DKMTDestroyContext_wrapper();
extern "C" void D3DKMTDestroyDevice_wrapper();
extern "C" void D3DKMTDestroySynchronizationObject_wrapper();
extern "C" void D3DKMTEscape_wrapper();
extern "C" void D3DKMTGetContextSchedulingPriority_wrapper();
extern "C" void D3DKMTGetDeviceState_wrapper();
extern "C" void D3DKMTGetDisplayModeList_wrapper();
extern "C" void D3DKMTGetMultisampleMethodList_wrapper();
extern "C" void D3DKMTGetRuntimeData_wrapper();
extern "C" void D3DKMTGetSharedPrimaryHandle_wrapper();
extern "C" void D3DKMTLock_wrapper();
extern "C" void D3DKMTOpenAdapterFromHdc_wrapper();
extern "C" void D3DKMTOpenResource_wrapper();
extern "C" void D3DKMTPresent_wrapper();
extern "C" void D3DKMTQueryAdapterInfo_wrapper();
extern "C" void D3DKMTQueryAllocationResidency_wrapper();
extern "C" void D3DKMTQueryResourceInfo_wrapper();
extern "C" void D3DKMTRender_wrapper();
extern "C" void D3DKMTSetAllocationPriority_wrapper();
extern "C" void D3DKMTSetContextSchedulingPriority_wrapper();
extern "C" void D3DKMTSetDisplayMode_wrapper();
extern "C" void D3DKMTSetDisplayPrivateDriverFormat_wrapper();
extern "C" void D3DKMTSetGammaRamp_wrapper();
extern "C" void D3DKMTSetVidPnSourceOwner_wrapper();
extern "C" void D3DKMTSignalSynchronizationObject_wrapper();
extern "C" void D3DKMTUnlock_wrapper();
extern "C" void D3DKMTWaitForSynchronizationObject_wrapper();
extern "C" void D3DKMTWaitForVerticalBlankEvent_wrapper();
extern "C" void D3DPerformance_BeginEvent_wrapper();
extern "C" void D3DPerformance_EndEvent_wrapper();
extern "C" void D3DPerformance_GetStatus_wrapper();
extern "C" void D3DPerformance_SetMarker_wrapper();
extern "C" void EnableFeatureLevelUpgrade_wrapper();
extern "C" void OpenAdapter10_wrapper();
extern "C" void OpenAdapter10_2_wrapper();

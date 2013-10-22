#include <windows.h>
#include <dxgi.h>
#include <stdio.h>
#include "hookapi.h"
HINSTANCE mHinst = 0, mHinstDLL = 0;
extern "C" UINT_PTR mProcs[50] = {0};

LPCSTR mImportNames[] = {"CheckETWTLS", "CompatString", "CompatValue", "CreateDXGIFactory", "CreateDXGIFactory1", "D3DKMTCloseAdapter", "D3DKMTCreateAllocation", "D3DKMTCreateContext", "D3DKMTCreateDevice", "D3DKMTCreateSynchronizationObject", "D3DKMTDestroyAllocation", "D3DKMTDestroyContext", "D3DKMTDestroyDevice", "D3DKMTDestroySynchronizationObject", "D3DKMTEscape", "D3DKMTGetContextSchedulingPriority", "D3DKMTGetDeviceState", "D3DKMTGetDisplayModeList", "D3DKMTGetMultisampleMethodList", "D3DKMTGetRuntimeData", "D3DKMTGetSharedPrimaryHandle", "D3DKMTLock", "D3DKMTOpenAdapterFromHdc", "D3DKMTOpenResource", "D3DKMTPresent", "D3DKMTQueryAdapterInfo", "D3DKMTQueryAllocationResidency", "D3DKMTQueryResourceInfo", "D3DKMTRender", "D3DKMTSetAllocationPriority", "D3DKMTSetContextSchedulingPriority", "D3DKMTSetDisplayMode", "D3DKMTSetDisplayPrivateDriverFormat", "D3DKMTSetGammaRamp", "D3DKMTSetVidPnSourceOwner", "D3DKMTSignalSynchronizationObject", "D3DKMTUnlock", "D3DKMTWaitForSynchronizationObject", "D3DKMTWaitForVerticalBlankEvent", "DXGID3D10CreateDevice", "DXGID3D10CreateLayeredDevice", "DXGID3D10ETWRundown", "DXGID3D10GetLayeredDeviceSize", "DXGID3D10RegisterLayers", "DXGIDumpJournal", "DXGIReportAdapterConfiguration", "DXGIRevertToSxS", "OpenAdapter10", "OpenAdapter10_2", "SetAppCompatStringPointer"};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID ) {
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		mHinstDLL = LoadLibraryW( L"c:\\windows\\system32\\dxgi.dll" );
		if ( !mHinstDLL )
			return ( FALSE );
		for ( int i = 0; i < 50; i++ )
			mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );
	} else if ( fdwReason == DLL_PROCESS_DETACH ) {
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" void CheckETWTLS_wrapper();
extern "C" void CompatString_wrapper();
extern "C" void CompatValue_wrapper();

#if 0
extern "C" void CreateDXGIFactory_wrapper();
extern "C" void CreateDXGIFactory1_wrapper();
#else
extern "C" HRESULT WINAPI CreateDXGIFactory_wrapper(
    const IID & riid,
    void **ppFactory)
{
    return CreateDXGIFactoryHook(
        "CreateDXGIFactory",
        riid,
        ppFactory);
}

extern "C" HRESULT WINAPI CreateDXGIFactory1_wrapper(
    const IID & riid,
    void **ppFactory)
{
    return CreateDXGIFactoryHook(
        "CreateDXGIFactory1",
        riid,
        ppFactory);
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
extern "C" void DXGID3D10CreateDevice_wrapper();
extern "C" void DXGID3D10CreateLayeredDevice_wrapper();
extern "C" void DXGID3D10ETWRundown_wrapper();
extern "C" void DXGID3D10GetLayeredDeviceSize_wrapper();
extern "C" void DXGID3D10RegisterLayers_wrapper();
extern "C" void DXGIDumpJournal_wrapper();
extern "C" void DXGIReportAdapterConfiguration_wrapper();
extern "C" void DXGIRevertToSxS_wrapper();
extern "C" void OpenAdapter10_wrapper();
extern "C" void OpenAdapter10_2_wrapper();
extern "C" void SetAppCompatStringPointer_wrapper();

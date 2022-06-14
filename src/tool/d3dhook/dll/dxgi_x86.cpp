#include "pch.h"
#include "hooks_exports.h"

HINSTANCE mHinst = 0, mHinstDLL = 0;
UINT_PTR  mProcs[51] = {0};

LPCSTR      mImportNames[] = {"CompatString",
                         "CompatValue",
                         "CreateDXGIFactory",
                         "CreateDXGIFactory1",
                         "CreateDXGIFactory2",
                         "D3DKMTCloseAdapter",
                         "D3DKMTCreateAllocation",
                         "D3DKMTCreateContext",
                         "D3DKMTCreateDevice",
                         "D3DKMTCreateSynchronizationObject",
                         "D3DKMTDestroyAllocation",
                         "D3DKMTDestroyContext",
                         "D3DKMTDestroyDevice",
                         "D3DKMTDestroySynchronizationObject",
                         "D3DKMTEscape",
                         "D3DKMTGetContextSchedulingPriority",
                         "D3DKMTGetDeviceState",
                         "D3DKMTGetDisplayModeList",
                         "D3DKMTGetMultisampleMethodList",
                         "D3DKMTGetRuntimeData",
                         "D3DKMTGetSharedPrimaryHandle",
                         "D3DKMTLock",
                         "D3DKMTOpenAdapterFromHdc",
                         "D3DKMTOpenResource",
                         "D3DKMTPresent",
                         "D3DKMTQueryAdapterInfo",
                         "D3DKMTQueryAllocationResidency",
                         "D3DKMTQueryResourceInfo",
                         "D3DKMTRender",
                         "D3DKMTSetAllocationPriority",
                         "D3DKMTSetContextSchedulingPriority",
                         "D3DKMTSetDisplayMode",
                         "D3DKMTSetDisplayPrivateDriverFormat",
                         "D3DKMTSetGammaRamp",
                         "D3DKMTSetVidPnSourceOwner",
                         "D3DKMTSignalSynchronizationObject",
                         "D3DKMTUnlock",
                         "D3DKMTWaitForSynchronizationObject",
                         "D3DKMTWaitForVerticalBlankEvent",
                         "DXGID3D10CreateDevice",
                         "DXGID3D10CreateLayeredDevice",
                         "DXGID3D10ETWRundown",
                         "DXGID3D10GetLayeredDeviceSize",
                         "DXGID3D10RegisterLayers",
                         "DXGIDumpJournal",
                         "DXGIGetDebugInterface1",
                         "DXGIReportAdapterConfiguration",
                         "DXGIRevertToSxS",
                         "OpenAdapter10",
                         "OpenAdapter10_2",
                         "SetAppCompatStringPointer"};
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID) {
    mHinst = hinstDLL;
    if (fdwReason == DLL_PROCESS_ATTACH) {
        mHinstDLL = LoadLibraryW(GetRealDllPath(L"dxgi.dll"));
        if (!mHinstDLL) return (FALSE);
        for (int i = 0; i < 51; i++) mProcs[i] = (UINT_PTR) GetProcAddress(mHinstDLL, mImportNames[i]);
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        FreeLibrary(mHinstDLL);
    }
    return (TRUE);
}

extern "C" __declspec(naked) void __stdcall CompatString_wrapper() {
    __asm {jmp mProcs[0*4] }
}
extern "C" __declspec(naked) void __stdcall CompatValue_wrapper() {
    __asm {jmp mProcs[1*4] }
}
#if 0
extern "C" __declspec(naked) void __stdcall CreateDXGIFactory_wrapper(){__asm{jmp mProcs[2*4]}}
extern "C" __declspec(naked) void __stdcall CreateDXGIFactory1_wrapper(){__asm{jmp mProcs[3*4]}}
extern "C" __declspec(naked) void __stdcall CreateDXGIFactory2_wrapper(){__asm{jmp mProcs[4*4]}}
#else
extern "C" HRESULT WINAPI CreateDXGIFactory_wrapper(const IID & riid, void ** ppFactory) { return CreateDXGIFactoryHook("CreateDXGIFactory", riid, ppFactory); }
extern "C" HRESULT WINAPI CreateDXGIFactory1_wrapper(const IID & riid, void ** ppFactory) {
    return CreateDXGIFactoryHook("CreateDXGIFactory1", riid, ppFactory);
}
extern "C" HRESULT WINAPI CreateDXGIFactory2_wrapper(UINT flags, const IID & riid, void ** ppFactory) { return CreateDXGIFactory2Hook(flags, riid, ppFactory); }
#endif
extern "C" __declspec(naked) void __stdcall D3DKMTCloseAdapter_wrapper() {
    __asm {jmp mProcs[5*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTCreateAllocation_wrapper() {
    __asm {jmp mProcs[6*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTCreateContext_wrapper() {
    __asm {jmp mProcs[7*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTCreateDevice_wrapper() {
    __asm {jmp mProcs[8*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTCreateSynchronizationObject_wrapper() {
    __asm {jmp mProcs[9*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTDestroyAllocation_wrapper() {
    __asm {jmp mProcs[10*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTDestroyContext_wrapper() {
    __asm {jmp mProcs[11*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTDestroyDevice_wrapper() {
    __asm {jmp mProcs[12*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTDestroySynchronizationObject_wrapper() {
    __asm {jmp mProcs[13*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTEscape_wrapper() {
    __asm {jmp mProcs[14*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTGetContextSchedulingPriority_wrapper() {
    __asm {jmp mProcs[15*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTGetDeviceState_wrapper() {
    __asm {jmp mProcs[16*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTGetDisplayModeList_wrapper() {
    __asm {jmp mProcs[17*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTGetMultisampleMethodList_wrapper() {
    __asm {jmp mProcs[18*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTGetRuntimeData_wrapper() {
    __asm {jmp mProcs[19*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTGetSharedPrimaryHandle_wrapper() {
    __asm {jmp mProcs[20*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTLock_wrapper() {
    __asm {jmp mProcs[21*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTOpenAdapterFromHdc_wrapper() {
    __asm {jmp mProcs[22*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTOpenResource_wrapper() {
    __asm {jmp mProcs[23*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTPresent_wrapper() {
    __asm {jmp mProcs[24*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTQueryAdapterInfo_wrapper() {
    __asm {jmp mProcs[25*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTQueryAllocationResidency_wrapper() {
    __asm {jmp mProcs[26*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTQueryResourceInfo_wrapper() {
    __asm {jmp mProcs[27*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTRender_wrapper() {
    __asm {jmp mProcs[28*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTSetAllocationPriority_wrapper() {
    __asm {jmp mProcs[29*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTSetContextSchedulingPriority_wrapper() {
    __asm {jmp mProcs[30*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTSetDisplayMode_wrapper() {
    __asm {jmp mProcs[31*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTSetDisplayPrivateDriverFormat_wrapper() {
    __asm {jmp mProcs[32*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTSetGammaRamp_wrapper() {
    __asm {jmp mProcs[33*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTSetVidPnSourceOwner_wrapper() {
    __asm {jmp mProcs[34*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTSignalSynchronizationObject_wrapper() {
    __asm {jmp mProcs[35*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTUnlock_wrapper() {
    __asm {jmp mProcs[36*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTWaitForSynchronizationObject_wrapper() {
    __asm {jmp mProcs[37*4] }
}
extern "C" __declspec(naked) void __stdcall D3DKMTWaitForVerticalBlankEvent_wrapper() {
    __asm {jmp mProcs[38*4] }
}
extern "C" __declspec(naked) void __stdcall DXGID3D10CreateDevice_wrapper() {
    __asm {jmp mProcs[39*4] }
}
extern "C" __declspec(naked) void __stdcall DXGID3D10CreateLayeredDevice_wrapper() {
    __asm {jmp mProcs[40*4] }
}
extern "C" __declspec(naked) void __stdcall DXGID3D10ETWRundown_wrapper() {
    __asm {jmp mProcs[41*4] }
}
extern "C" __declspec(naked) void __stdcall DXGID3D10GetLayeredDeviceSize_wrapper() {
    __asm {jmp mProcs[42*4] }
}
extern "C" __declspec(naked) void __stdcall DXGID3D10RegisterLayers_wrapper() {
    __asm {jmp mProcs[43*4] }
}
extern "C" __declspec(naked) void __stdcall DXGIDumpJournal_wrapper() {
    __asm {jmp mProcs[44*4] }
}
extern "C" __declspec(naked) void __stdcall DXGIGetDebugInterface1_wrapper() {
    __asm {jmp mProcs[45*4] }
}
extern "C" __declspec(naked) void __stdcall DXGIReportAdapterConfiguration_wrapper() {
    __asm {jmp mProcs[46*4] }
}
extern "C" __declspec(naked) void __stdcall DXGIRevertToSxS_wrapper() {
    __asm {jmp mProcs[47*4] }
}
extern "C" __declspec(naked) void __stdcall OpenAdapter10_wrapper() {
    __asm {jmp mProcs[48*4] }
}
extern "C" __declspec(naked) void __stdcall OpenAdapter10_2_wrapper() {
    __asm {jmp mProcs[49*4] }
}
extern "C" __declspec(naked) void __stdcall SetAppCompatStringPointer_wrapper() {
    __asm {jmp mProcs[50*4] }
}

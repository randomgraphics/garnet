#include "pch.h"
#include "hooks_exports.h"

HINSTANCE mHinst = 0, mHinstDLL = 0;
UINT_PTR  mProcs[16] = {0};

LPCSTR      mImportNames[] = {"D3DPERF_BeginEvent",
                         "D3DPERF_EndEvent",
                         "D3DPERF_GetStatus",
                         "D3DPERF_QueryRepeatFrame",
                         "D3DPERF_SetMarker",
                         "D3DPERF_SetOptions",
                         "D3DPERF_SetRegion",
                         "DebugSetLevel",
                         "DebugSetMute",
                         "Direct3D9EnableMaximizedWindowedModeShim",
                         "Direct3DCreate9",
                         "Direct3DCreate9Ex",
                         "Direct3DShaderValidatorCreate9",
                         "PSGPError",
                         "PSGPSampleTexture",
                         (LPCSTR) 16};
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID) {
    mHinst = hinstDLL;
    if (fdwReason == DLL_PROCESS_ATTACH) {
        mHinstDLL = LoadLibraryW(GetRealDllPath(L"d3d9.dll"));
        if (!mHinstDLL) return (FALSE);
        for (int i = 0; i < 16; i++) mProcs[i] = (UINT_PTR) GetProcAddress(mHinstDLL, mImportNames[i]);
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        FreeLibrary(mHinstDLL);
    }
    return (TRUE);
}

extern "C" __declspec(naked) void __stdcall D3DPERF_BeginEvent_wrapper() {
    __asm {jmp mProcs[0*4] }
}
extern "C" __declspec(naked) void __stdcall D3DPERF_EndEvent_wrapper() {
    __asm {jmp mProcs[1*4] }
}
extern "C" __declspec(naked) void __stdcall D3DPERF_GetStatus_wrapper() {
    __asm {jmp mProcs[2*4] }
}
extern "C" __declspec(naked) void __stdcall D3DPERF_QueryRepeatFrame_wrapper() {
    __asm {jmp mProcs[3*4] }
}
extern "C" __declspec(naked) void __stdcall D3DPERF_SetMarker_wrapper() {
    __asm {jmp mProcs[4*4] }
}
extern "C" __declspec(naked) void __stdcall D3DPERF_SetOptions_wrapper() {
    __asm {jmp mProcs[5*4] }
}
extern "C" __declspec(naked) void __stdcall D3DPERF_SetRegion_wrapper() {
    __asm {jmp mProcs[6*4] }
}
extern "C" __declspec(naked) void __stdcall DebugSetLevel_wrapper() {
    __asm {jmp mProcs[7*4] }
}
extern "C" __declspec(naked) void __stdcall DebugSetMute_wrapper() {
    __asm {jmp mProcs[8*4] }
}
extern "C" __declspec(naked) void __stdcall Direct3D9EnableMaximizedWindowedModeShim_wrapper() {
    __asm {jmp mProcs[9*4] }
}
#if 0
extern "C" __declspec(naked) void __stdcall Direct3DCreate9_wrapper(){__asm{jmp mProcs[10*4]}}
extern "C" __declspec(naked) void __stdcall Direct3DCreate9Ex_wrapper(){__asm{jmp mProcs[11*4]}}
#else
extern "C" IDirect3D9 * WINAPI Direct3DCreate9_wrapper(UINT SDKVersion) { return Direct3DCreate9Hook(SDKVersion); }
extern "C" HRESULT WINAPI      Direct3DCreate9Ex_wrapper(UINT SDKVersion, IDirect3D9Ex ** ppD3D) { return Direct3DCreate9ExHook(SDKVersion, ppD3D); }
#endif
extern "C" __declspec(naked) void __stdcall Direct3DShaderValidatorCreate9_wrapper() {
    __asm {jmp mProcs[12*4] }
}
extern "C" __declspec(naked) void __stdcall PSGPError_wrapper() {
    __asm {jmp mProcs[13*4] }
}
extern "C" __declspec(naked) void __stdcall PSGPSampleTexture_wrapper() {
    __asm {jmp mProcs[14*4] }
}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal16() {
    __asm {jmp mProcs[15*4] }
}

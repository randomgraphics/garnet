#include "pch.h"
#include "hooks_exports.h"

HINSTANCE           mHinst = 0, mHinstDLL = 0;
extern "C" UINT_PTR mProcs[1] = {0};

LPCSTR      mImportNames[] = {"DWriteCreateFactory"};
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID) {
    mHinst = hinstDLL;
    if (fdwReason == DLL_PROCESS_ATTACH) {
        mHinstDLL = LoadLibraryW(GetRealDllPath(L"dwrite.dll"));
        if (!mHinstDLL) return (FALSE);
        for (int i = 0; i < 1; i++) mProcs[i] = (UINT_PTR) GetProcAddress(mHinstDLL, mImportNames[i]);
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        FreeLibrary(mHinstDLL);
    }
    return (TRUE);
}

// extern "C" void DWriteCreateFactory_wrapper();
extern "C" HRESULT WINAPI DWriteCreateFactory_wrapper(_In_ DWRITE_FACTORY_TYPE factoryType, _In_ REFIID iid, _Out_ IUnknown ** factory) {
    return DWriteCreateFactoryHook(factoryType, iid, factory);
}

#include "pch.h"
#include "hooks9.h"
#include "hooks_shared_impl.cpp"
#define INSIDE_D3D9_HOOK
#include "d3d9hooks_exports.h"
using namespace GN;

// *****************************************************************************
// DLL loading utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void * GetRealFunctionPtr(const wchar_t * dllName, const char * functionName)
{
    HMODULE dll;

    dll = ::LoadLibraryW(dllName);
    if (0 == dll)
    {
        HOOK_ERROR_LOG("Can't load dll: %S", dllName);
        return nullptr;
    }

    void * proc = ::GetProcAddress(dll, functionName);
    if(0 == proc)
    {
        HOOK_ERROR_LOG("Can't get proc address: dllName=%S, functionName=%s", dllName, functionName);
    }

    return proc;
}

//
//
// -----------------------------------------------------------------------------
static StrW GetRealDllPath(const wchar_t * dllName)
{
    StrW system32 = L"c:\\windows\\system32\\";
    StrW dllpath = system32 + dllName;
    return dllpath;
}

// *****************************************************************************
// D3D9 global functions
// *****************************************************************************

#define HOOK_ENABLED 0

typedef IDirect3D9 * (WINAPI * PFN_DIRECT3D_CREATE_9)(UINT SDKVersion);
typedef HRESULT (WINAPI * PFN_DIRECT3D_CREATE_9_EX)(UINT SDKVersion, IDirect3D9Ex **ppD3D);

//
//
// -----------------------------------------------------------------------------
D3D9_HOOK_API IDirect3D9 * WINAPI Direct3DCreate9Hook(UINT SDKVersion)
{
    calltrace::AutoTrace trace("Direct3DCreate9");

    PFN_DIRECT3D_CREATE_9 realFunc = (PFN_DIRECT3D_CREATE_9)GetRealFunctionPtr(
        GetRealDllPath(L"d3d9.dll").rawptr(), "Direct3DCreate9");
    if (nullptr == realFunc) return nullptr;

    IDirect3D9 * d3d9 = realFunc(SDKVersion);

#if HOOK_ENABLED
    if( nullptr != d3d9 && 1 == trace.getCurrentLevel() )
    {
        d3d9 = RealToHooked(d3d9);
    }
#endif

    return d3d9;
}

//
//
// -----------------------------------------------------------------------------
D3D9_HOOK_API HRESULT Direct3DCreate9ExHook(UINT SDKVersion, IDirect3D9Ex **ppD3D)
{
    calltrace::AutoTrace trace("Direct3DCreate9Ex");

    PFN_DIRECT3D_CREATE_9_EX realFunc = (PFN_DIRECT3D_CREATE_9_EX)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll").rawptr(),
        "Direct3DCreate9Ex");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(SDKVersion, ppD3D);

#if HOOK_ENABLED
    if( SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( ppD3D ) *ppD3D = RealToHooked(*ppD3D);
    }
#endif

    return hr;
}

// -----------------------------------------------------------------------------
// DLL Entry Point
// -----------------------------------------------------------------------------

BOOL WINAPI DllMain( HINSTANCE, DWORD fdwReason, LPVOID )
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
    {
        HookedClassFactory::sGetInstance().registerAll();
	} else if ( fdwReason == DLL_PROCESS_DETACH )
	{
        // TODO: cleanup.
	}
	return TRUE;
}

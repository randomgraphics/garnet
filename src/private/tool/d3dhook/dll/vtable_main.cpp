#include "pch.h"
#include "d3d11vtable.h"
#include "d3d9vtable.h"

#define INSIDE_HOOK_DLL
#include "hooks_exports.h"

using namespace GN;

// *****************************************************************************
// Misc. Local Utilities
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

// *****************************************************************************
// Global options
// *****************************************************************************

struct Options
{
    bool enabled; // global switch to enable/disable the hooked libarary.

    Options()
    {
        ZeroMemory(this, sizeof(*this));
        enabled = true;
    }
};
static Options g_options;

// *****************************************************************************
// Call Trace
// *****************************************************************************

namespace calltrace
{
    struct AutoFile
    {
        FILE * fp;
        bool   tried;

        AutoFile() : fp(nullptr), tried(false)
        {
        }

        ~AutoFile()
        {
            Close();
        }

        bool Open(const wchar_t * filename, const wchar_t * mode)
        {
            Close();
            int result = _wfopen_s(&fp, filename, mode);
            tried = true;
            return 0 == result;
        }

        void Close()
        {
            if(fp) fclose(fp), fp = 0;
            tried = false;
        }
    };

    AutoFile g_logFile;

    __declspec(thread) int g_level = 0;
    volatile LONG g_count = 0;

    bool g_callTraceEnabled = false;

    enum TraceTarget
    {
        DISABLED     = 0,
        TO_CONSOLE   = 1,
        TO_DEBUGGER  = 2,
        TO_FILE      = 4,
    };

    TraceTarget getTT()
    {
        if (IsWindow(GetConsoleWindow()))
        {
            return TO_CONSOLE;
        }
        else if(IsDebuggerPresent())
        {
            return TO_DEBUGGER;
        }
        else if (g_logFile.fp || (!g_logFile.tried && g_logFile.Open(L"d3dhook_trace_log.txt", L"wt")))
        {
            return TO_FILE;
        }
        else
        {
            return DISABLED;
        }
    }

    int doEnter(const wchar_t * text, TraceTarget tt)
    {
        if (DISABLED != tt)
        {
            wchar_t ident[256] = {};
            wchar_t buf[256] = {};

            for(int i = 0; i < g_level && i < countof(ident); ++i)
            {
                ident[i] = L' ';
            }
            swprintf_s(buf, L"{thread:%d}[%d]", GetCurrentThreadId(), InterlockedIncrement(&g_count));
            wcscat_s(buf, ident);
            wcscat_s(buf, text);
            wcscat_s(buf, L"\n");

            if (TO_CONSOLE & tt)
            {
                wprintf(L"%s", buf);
            }

            if (TO_DEBUGGER & tt)
            {
                OutputDebugStringW(buf);
            }

            if (TO_FILE & tt)
            {
                fwprintf(g_logFile.fp, L"%s", buf);
                fflush(g_logFile.fp);
            }
        }
        return ++g_level;
    }

    int enter(const wchar_t * text)
    {
        return doEnter(text, getTT());
    }

    int enter(const char * text)
    {
        TraceTarget tt = getTT();
        if (DISABLED != tt)
        {
            wchar_t textw[256];
            swprintf_s(textw, L"%S", text);
            return doEnter(textw, tt);
        }
        else
        {
            return doEnter(L"", tt);
        }
    }

    void leave()
    {
        --g_level;
    }
}

// *****************************************************************************
// DXGI global functions
// *****************************************************************************

typedef HRESULT (WINAPI * PFN_CREATE_DXGI_FACTORY)(const IID & riid, void **ppFactory);
typedef HRESULT (WINAPI * PFN_CREATE_DXGI_FACTORY2)(UINT flags, const IID & riid, void **ppFactory);

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
CreateDXGIFactoryHook(
    const char * funcName,
    const IID & riid,
    void **ppFactory
)
{
    GN_D3DHOOK_CALLTRACE(funcName);

    PFN_CREATE_DXGI_FACTORY realFunc = (PFN_CREATE_DXGI_FACTORY)GetRealFunctionPtr(
        GetRealDllPath(L"dxgi.dll"),
        funcName);
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(riid, ppFactory);

    if (g_options.enabled && SUCCEEDED(hr) && ppFactory)
    {
        RealToHooked11(riid, *ppFactory);
    }

    return hr;
}

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
CreateDXGIFactory2Hook(
    UINT flags,
    const IID & riid,
    void **ppFactory
)
{
    GN_D3DHOOK_CALLTRACE("CreateDXGIFactory2");

    PFN_CREATE_DXGI_FACTORY2 realFunc = (PFN_CREATE_DXGI_FACTORY2)GetRealFunctionPtr(
        GetRealDllPath(L"dxgi.dll"),
        "CreateDXGIFactory2");
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(flags, riid, ppFactory);

    if (g_options.enabled && SUCCEEDED(hr) && ppFactory)
    {
        RealToHooked11(riid, *ppFactory);
    }

    return hr;
}

// *****************************************************************************
// D3D11 global functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D3D11CreateDeviceHook(
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
    GN_D3DHOOK_CALLTRACE("D3D11CreateDevice");

    PFN_D3D11_CREATE_DEVICE realFunc = (PFN_D3D11_CREATE_DEVICE)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll"),
        "D3D11CreateDevice");
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        pAdapter,
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);

    if (g_options.enabled && SUCCEEDED(hr))
    {
        if (ppDevice) RealToHooked11(*ppDevice);
        if (ppImmediateContext) RealToHooked11(*ppImmediateContext);
    }

    return hr;
}

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D3D11CreateDeviceAndSwapChainHook(
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
    GN_D3DHOOK_CALLTRACE("D3D11CreateDeviceAndSwapChain");

    PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN realFunc = (PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll"),
        "D3D11CreateDeviceAndSwapChain");
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        pAdapter,
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);

    if( g_options.enabled && SUCCEEDED(hr) )
    {
        if( ppSwapChain ) RealToHooked11(*ppSwapChain);
        if( ppDevice ) RealToHooked11(*ppDevice);
        if( ppImmediateContext ) RealToHooked11(*ppImmediateContext);
    }

    return hr;
}

// *****************************************************************************
// D3D9 global functions
// *****************************************************************************

typedef IDirect3D9 * (WINAPI * PFN_DIRECT3D_CREATE_9)(UINT SDKVersion);
typedef HRESULT (WINAPI * PFN_DIRECT3D_CREATE_9_EX)(UINT SDKVersion, IDirect3D9Ex **ppD3D);

//
//
// -----------------------------------------------------------------------------
HOOK_API IDirect3D9 * WINAPI Direct3DCreate9Hook(UINT SDKVersion)
{
    GN_D3DHOOK_CALLTRACE("Direct3DCreate9");

    PFN_DIRECT3D_CREATE_9 realFunc = (PFN_DIRECT3D_CREATE_9)GetRealFunctionPtr(
        GetRealDllPath(L"d3d9.dll"),
        "Direct3DCreate9");
    if (nullptr == realFunc) return nullptr;

    IDirect3D9 * d3d9 = realFunc(SDKVersion);

    if( g_options.enabled && d3d9 )
    {
        RealToHooked9(d3d9);
    }

    return d3d9;
}

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI Direct3DCreate9ExHook(UINT SDKVersion, IDirect3D9Ex **ppD3D)
{
    GN_D3DHOOK_CALLTRACE("Direct3DCreate9Ex");

    PFN_DIRECT3D_CREATE_9_EX realFunc = (PFN_DIRECT3D_CREATE_9_EX)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll"),
        "Direct3DCreate9Ex");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(SDKVersion, ppD3D);

    if( g_options.enabled && SUCCEEDED(hr) && ppD3D && *ppD3D )
    {
        RealToHooked9(*ppD3D);
    }

    return hr;
}

// *****************************************************************************
// D2D1
// *****************************************************************************

typedef HRESULT (WINAPI * PFN_D2D1_CREATE_DEVICE)(
    _In_ IDXGIDevice *dxgiDevice,
    _In_opt_ CONST D2D1_CREATION_PROPERTIES *creationProperties,
    _Outptr_ ID2D1Device **d2dDevice);

typedef HRESULT (WINAPI * PFN_D2D1_CREATE_DEVICE_CONTEXT)(
    _In_ IDXGISurface *dxgiSurface,
    _In_opt_ CONST D2D1_CREATION_PROPERTIES *creationProperties,
    _Outptr_ ID2D1DeviceContext **d2dDeviceContext
);

typedef HRESULT (WINAPI * PFN_D2D1_CREATE_FACTORY)(
    _In_ D2D1_FACTORY_TYPE factoryType,
    _In_ REFIID riid,
    _In_opt_ CONST D2D1_FACTORY_OPTIONS *pFactoryOptions,
    _Out_ void **ppIFactory
);

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D2D1CreateDeviceHook(
    _In_ IDXGIDevice *dxgiDevice,
    _In_opt_ CONST D2D1_CREATION_PROPERTIES *creationProperties,
    _Outptr_ ID2D1Device **d2dDevice
)
{
    GN_D3DHOOK_CALLTRACE("D2D1CreateDevice");

    PFN_D2D1_CREATE_DEVICE realFunc = (PFN_D2D1_CREATE_DEVICE)GetRealFunctionPtr(
        GetRealDllPath(L"d2d1.dll"),
        "D2D1CreateDevice");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        dxgiDevice,
        creationProperties,
        d2dDevice);

    return hr;
}

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D2D1CreateDeviceContextHook(
    _In_ IDXGISurface *dxgiSurface,
    _In_opt_ CONST D2D1_CREATION_PROPERTIES *creationProperties,
    _Outptr_ ID2D1DeviceContext **d2dDeviceContext
)
{
    GN_D3DHOOK_CALLTRACE("D2D1CreateDeviceContext");

    PFN_D2D1_CREATE_DEVICE_CONTEXT realFunc = (PFN_D2D1_CREATE_DEVICE_CONTEXT)GetRealFunctionPtr(
        GetRealDllPath(L"d2d1.dll"),
        "D2D1CreateDeviceContext");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        dxgiSurface,
        creationProperties,
        d2dDeviceContext);

    return hr;
}

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D2D1CreateFactoryHook(
    _In_ D2D1_FACTORY_TYPE factoryType,
    _In_ REFIID riid,
    _In_opt_ CONST D2D1_FACTORY_OPTIONS *pFactoryOptions,
    _Out_ void **ppIFactory
)
{
    GN_D3DHOOK_CALLTRACE("D2D1CreateFactory");

    PFN_D2D1_CREATE_FACTORY realFunc = (PFN_D2D1_CREATE_FACTORY)GetRealFunctionPtr(
        GetRealDllPath(L"d2d1.dll"),
        "D2D1CreateFactory");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        factoryType,
        riid,
        pFactoryOptions,
        ppIFactory);

    return hr;
}

// *****************************************************************************
// DWRITE
// *****************************************************************************

typedef HRESULT (WINAPI * PFN_DWRITE_CREATE_FACTORY)(
    _In_ DWRITE_FACTORY_TYPE factoryType,
    _In_ REFIID iid,
    _Out_ IUnknown **factory
);

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI DWriteCreateFactoryHook(
    _In_ DWRITE_FACTORY_TYPE factoryType,
    _In_ REFIID iid,
    _Out_ IUnknown **factory
)
{
    GN_D3DHOOK_CALLTRACE("DWriteCreateFactory");

    PFN_DWRITE_CREATE_FACTORY realFunc = (PFN_DWRITE_CREATE_FACTORY)GetRealFunctionPtr(
        GetRealDllPath(L"dwrite.dll"),
        "DWriteCreateFactory");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        factoryType,
        iid,
        factory);

    return hr;
}

// -----------------------------------------------------------------------------
// DLL Entry Point
// -----------------------------------------------------------------------------

BOOL WINAPI DllMain( HINSTANCE, DWORD fdwReason, LPVOID )
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
    {
#if GN_PLATFORM_IS_STATIC
        g_options.enabled = fs::isFile("__d3dhook_enabled");
        calltrace::g_callTraceEnabled = fs::isFile("__d3dhook_call_trace_enabled");;
#else
        // fs::isFile() throws exceptions in non-static build.
#endif
        SetupD3D11HookedVTables();
        SetupD3D9HookedVTables();
	} else if ( fdwReason == DLL_PROCESS_DETACH )
	{
        // TODO: cleanup.
	}
	return TRUE;
}

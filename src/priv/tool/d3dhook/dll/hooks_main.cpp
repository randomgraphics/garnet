#include "pch.h"
#include "d2dhooks.h"
#include "d3d11hooks.h"
#include "d3d9hooks.h"
#define INSIDE_HOOK_DLL
#include "hooks_exports.h"
#include <unordered_map>

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
// InterfaceDesc
// *****************************************************************************

static const InterfaceDesc sAllInterfaces[] =
{
    { __uuidof(IUnknown), "IUnknown", L"IUnknown" },

#define DECLARE_D3D9_INTERFACE(x) { __uuidof(x), #x, L#x },
#include "d3d9interfaces.inl"

#define DECLARE_D3D11_INTERFACE(x) { __uuidof(x), #x, L#x },
#include "d3d11interfaces.inl"

#define DECLARE_D2D_INTERFACE(x) { __uuidof(x), #x, L#x },
#include "d2dinterfaces.inl"
};

const InterfaceDesc * InterfaceDesc::sGetDescFromIID(const IID & iid)
{
    for(size_t i = 0; i < _countof(sAllInterfaces); ++i)
    {
        const InterfaceDesc & id = sAllInterfaces[i];
        if (id.iid == iid) return &id;
    }
    return nullptr;
}

// *****************************************************************************
// Class Factory
// *****************************************************************************

HookedClassFactory HookedClassFactory::s_instance;

void HookedClassFactory::registerAll()
{
#include "d3d11factories.inl"
#include "d3d9factories.inl"
#include "d2dfactories.inl"
}

// *****************************************************************************
// Call Trace
// *****************************************************************************

namespace calltrace
{
    __declspec(thread) int g_level = 0;

    bool g_printCall = false;

    int enter(const wchar_t * text)
    {
        if (g_printCall)
        {
            wchar_t ident[256] = {};
            for(int i = 0; i < g_level && i < _countof(ident); ++i)
            {
                ident[i] = L' ';
            }

            wchar_t buf[256] = {};
            swprintf_s(buf, L"{%d}", GetCurrentThreadId());
            wcscat_s(buf, ident);
            wcscat_s(buf, text);
            wcscat_s(buf, L"\n");

            if (IsDebuggerPresent())
            {
                OutputDebugStringW(buf);
            }
        }
        return ++g_level;
    }

    int enter(const char * text)
    {
        if (g_printCall)
        {
            wchar_t textw[256];
            swprintf_s(textw, L"%S", text);
            return enter(textw);
        }
        else
        {
            return enter(L"");
        }
    }

    void leave()
    {
        --g_level;
    }
}

// *****************************************************************************
// UnknownBase Table
// *****************************************************************************

typedef std::unordered_map<intptr_t, GN::AutoComPtr<WeakUnknownRef>> ObjectMap;

struct ObjectTable
{
    CritSec   cs;
    ObjectMap objects;
};

static ObjectTable g_table;

//
//
// -----------------------------------------------------------------------------
static void UnknownBaseDestructNotif(UnknownBase * base, void *)
{
    IUnknown * realUnknown = base->GetRealObj(__uuidof(IUnknown));
    CritSec::AutoLock lock(g_table.cs);
    ObjectMap::const_iterator iter = g_table.objects.find((intptr_t)realUnknown);
    if( g_table.objects.end() != iter )
    {
        GN_ASSERT(!iter->second->promote());
        g_table.objects.erase(iter);
    }
    else
    {
        GN_UNEXPECTED();
    }
}

//
//
// -----------------------------------------------------------------------------
void UnknownBaseTable::add(IUnknown * realobj, UnknownBase * hooked)
{
    CritSec::AutoLock lock(g_table.cs);
    GN::AutoComPtr<IUnknown> realUnknown = Qi<IUnknown>(realobj);
    GN_ASSERT(g_table.objects.end() == g_table.objects.find((intptr_t)realUnknown.get()));
    AutoComPtr<WeakUnknownRef> ref;
    ref.set(new WeakUnknownRef);
    GN_ASSERT(hooked);
    ref->attach(hooked);
    hooked->setDestructNotif(UnknownBaseDestructNotif, nullptr);
    g_table.objects[(intptr_t)realUnknown.get()] = ref;
}

//
//
// -----------------------------------------------------------------------------
GN::AutoComPtr<UnknownBase>
UnknownBaseTable::get(IUnknown * realobj)
{
    CritSec::AutoLock lock(g_table.cs);

    GN::AutoComPtr<IUnknown> realUnknown = Qi<IUnknown>(realobj);

    ObjectMap::const_iterator iter = g_table.objects.find((intptr_t)realUnknown.get());
    if( iter == g_table.objects.end())
    {
        return nullptr;
    }
    else
    {
        return iter->second->promote();
    }
}

// *****************************************************************************
// D3D11 global functions
// *****************************************************************************

typedef HRESULT (WINAPI * PFN_CREATE_DXGI_FACTORY)(const IID & riid, void **ppFactory);

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
    calltrace::AutoTrace trace(funcName);

    PFN_CREATE_DXGI_FACTORY realFunc = (PFN_CREATE_DXGI_FACTORY)GetRealFunctionPtr(
        GetRealDllPath(L"dxgi.dll"),
        funcName);
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(riid, ppFactory);
    if( g_options.enabled && SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( ppFactory ) *ppFactory = RealToHooked11(riid, (IDXGIObject*)*ppFactory);
    }

    return hr;
}

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
    calltrace::AutoTrace trace("D3D11CreateDevice");

    PFN_D3D11_CREATE_DEVICE realFunc = (PFN_D3D11_CREATE_DEVICE)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll"),
        "D3D11CreateDevice");
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        HookedToReal(pAdapter),
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);

    if( g_options.enabled && SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( ppDevice ) *ppDevice = RealToHooked11(*ppDevice);
        if( ppImmediateContext ) *ppImmediateContext = RealToHooked11(*ppImmediateContext);
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
    calltrace::AutoTrace trace("D3D11CreateDeviceAndSwapChain");

    PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN realFunc = (PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll"),
        "D3D11CreateDeviceAndSwapChain");
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        HookedToReal(pAdapter),
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);

    if( g_options.enabled && SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( ppSwapChain ) *ppSwapChain = RealToHooked11(*ppSwapChain);
        if( ppDevice ) *ppDevice = RealToHooked11(*ppDevice);
        if( ppImmediateContext ) *ppImmediateContext = RealToHooked11(*ppImmediateContext);
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
    calltrace::AutoTrace trace("Direct3DCreate9");

    PFN_DIRECT3D_CREATE_9 realFunc = (PFN_DIRECT3D_CREATE_9)GetRealFunctionPtr(
        GetRealDllPath(L"d3d9.dll"),
        "Direct3DCreate9");
    if (nullptr == realFunc) return nullptr;

    IDirect3D9 * d3d9 = realFunc(SDKVersion);

    if( g_options.enabled && nullptr != d3d9 && 1 == trace.getCurrentLevel() )
    {
        d3d9 = RealToHooked9(d3d9);
    }

    return d3d9;
}

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI Direct3DCreate9ExHook(UINT SDKVersion, IDirect3D9Ex **ppD3D)
{
    calltrace::AutoTrace trace("Direct3DCreate9Ex");

    PFN_DIRECT3D_CREATE_9_EX realFunc = (PFN_DIRECT3D_CREATE_9_EX)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll"),
        "Direct3DCreate9Ex");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(SDKVersion, ppD3D);

    if( g_options.enabled && SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( ppD3D ) *ppD3D = RealToHooked9(*ppD3D);
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
    calltrace::AutoTrace trace("D2D1CreateDevice");

    PFN_D2D1_CREATE_DEVICE realFunc = (PFN_D2D1_CREATE_DEVICE)GetRealFunctionPtr(
        GetRealDllPath(L"d2d1.dll"),
        "D2D1CreateDevice");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        1 == trace.getCurrentLevel() ? HookedToReal(dxgiDevice) : dxgiDevice,
        creationProperties,
        d2dDevice);

    if( g_options.enabled && SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( d2dDevice ) *d2dDevice = RealToHooked_D2D(*d2dDevice);
    }

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
    calltrace::AutoTrace trace("D2D1CreateDeviceContext");

    PFN_D2D1_CREATE_DEVICE_CONTEXT realFunc = (PFN_D2D1_CREATE_DEVICE_CONTEXT)GetRealFunctionPtr(
        GetRealDllPath(L"d2d1.dll"),
        "D2D1CreateDeviceContext");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        1 == trace.getCurrentLevel() ? HookedToReal(dxgiSurface) : dxgiSurface,
        creationProperties,
        d2dDeviceContext);

    if( g_options.enabled && SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( d2dDeviceContext ) *d2dDeviceContext = RealToHooked_D2D(*d2dDeviceContext);
    }

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
    calltrace::AutoTrace trace("D2D1CreateFactory");

    PFN_D2D1_CREATE_FACTORY realFunc = (PFN_D2D1_CREATE_FACTORY)GetRealFunctionPtr(
        GetRealDllPath(L"d2d1.dll"),
        "D2D1CreateFactory");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        factoryType,
        riid,
        pFactoryOptions,
        ppIFactory);

    if( g_options.enabled && SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( ppIFactory ) *ppIFactory = RealToHooked_D2D(riid, (IUnknown*)*ppIFactory);
    }

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
    calltrace::AutoTrace trace("DWriteCreateFactory");

    PFN_DWRITE_CREATE_FACTORY realFunc = (PFN_DWRITE_CREATE_FACTORY)GetRealFunctionPtr(
        GetRealDllPath(L"dwrite.dll"),
        "DWriteCreateFactory");

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        factoryType,
        iid,
        factory);

    if( g_options.enabled && SUCCEEDED(hr) && 1 == trace.getCurrentLevel() )
    {
        if( factory ) *factory = RealToHooked_D2D(iid, (IUnknown*)*factory);
    }

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

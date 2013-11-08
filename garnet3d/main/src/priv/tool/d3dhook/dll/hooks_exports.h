#ifdef INSIDE_HOOK_DLL
#define HOOK_API __declspec(dllexport)
#else
#define HOOK_API __declspec(dllimport)
#endif

// *****************************************************************************
// DXGI
// *****************************************************************************

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI CreateDXGIFactoryHook(
    const char * funcName,
    const IID & riid,
    void **ppFactory
);

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI CreateDXGIFactory2Hook(
    UINT flag,
    const IID & riid,
    void **ppFactory
);

// *****************************************************************************
// D3D11
// *****************************************************************************

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
    __out_opt ID3D11DeviceContext** ppImmediateContext );

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
    __out_opt ID3D11DeviceContext** ppImmediateContext );

// *****************************************************************************
// D3D9
// *****************************************************************************

// -----------------------------------------------------------------------------
HOOK_API IDirect3D9 * WINAPI Direct3DCreate9Hook(UINT SDKVersion);

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI Direct3DCreate9ExHook(UINT SDKVersion, IDirect3D9Ex **ppD3D);

// *****************************************************************************
// D2D1
// *****************************************************************************

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D2D1CreateDeviceHook(
    _In_ IDXGIDevice *dxgiDevice,
    _In_opt_ CONST D2D1_CREATION_PROPERTIES *creationProperties,
    _Outptr_ ID2D1Device **d2dDevice
);

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D2D1CreateDeviceContextHook(
    _In_ IDXGISurface *dxgiSurface,
    _In_opt_ CONST D2D1_CREATION_PROPERTIES *creationProperties,
    _Outptr_ ID2D1DeviceContext **d2dDeviceContext
);

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D2D1CreateFactoryHook(
    _In_ D2D1_FACTORY_TYPE factoryType,
    _In_ REFIID riid,
    _In_opt_ CONST D2D1_FACTORY_OPTIONS *pFactoryOptions,
    _Out_ void **ppIFactory
);

// *****************************************************************************
// DWRITE
// *****************************************************************************

// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI DWriteCreateFactoryHook(
    _In_ DWRITE_FACTORY_TYPE factoryType,
    _In_ REFIID iid,
    _Out_ IUnknown **factory
);

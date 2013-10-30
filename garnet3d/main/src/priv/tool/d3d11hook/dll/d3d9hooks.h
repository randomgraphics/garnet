// script generated file. Do _NOT_ edit.

#include "hooks9.h"

// ==============================================================================
// IDirect3D9
// ==============================================================================
class Direct3D9Hook : public HookBase<IDirect3D9>
{

protected:

    Direct3D9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3D9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3D9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3D9Hook * typedPtr = (Direct3D9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void* pInitializeFunction);
    NullPtr<void (Direct3D9Hook::*)(void* &)> _RegisterSoftwareDevice_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, void*)> _RegisterSoftwareDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetAdapterCount();
    NullPtr<void (Direct3D9Hook::*)()> _GetAdapterCount_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(UINT)> _GetAdapterCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier);
    NullPtr<void (Direct3D9Hook::*)(UINT &, DWORD &, D3DADAPTER_IDENTIFIER9* &)> _GetAdapterIdentifier_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, DWORD, D3DADAPTER_IDENTIFIER9*)> _GetAdapterIdentifier_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter, D3DFORMAT Format);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DFORMAT &)> _GetAdapterModeCount_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(UINT, UINT, D3DFORMAT)> _GetAdapterModeCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DFORMAT &, UINT &, D3DDISPLAYMODE* &)> _EnumAdapterModes_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DFORMAT, UINT, D3DDISPLAYMODE*)> _EnumAdapterModes_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DDISPLAYMODE* &)> _GetAdapterDisplayMode_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DDISPLAYMODE*)> _GetAdapterDisplayMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DDEVTYPE &, D3DFORMAT &, D3DFORMAT &, BOOL &)> _CheckDeviceType_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, BOOL)> _CheckDeviceType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DDEVTYPE &, D3DFORMAT &, DWORD &, D3DRESOURCETYPE &, D3DFORMAT &)> _CheckDeviceFormat_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DDEVTYPE, D3DFORMAT, DWORD, D3DRESOURCETYPE, D3DFORMAT)> _CheckDeviceFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DDEVTYPE &, D3DFORMAT &, BOOL &, D3DMULTISAMPLE_TYPE &, DWORD* &)> _CheckDeviceMultiSampleType_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DDEVTYPE, D3DFORMAT, BOOL, D3DMULTISAMPLE_TYPE, DWORD*)> _CheckDeviceMultiSampleType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DDEVTYPE &, D3DFORMAT &, D3DFORMAT &, D3DFORMAT &)> _CheckDepthStencilMatch_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT)> _CheckDepthStencilMatch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DDEVTYPE &, D3DFORMAT &, D3DFORMAT &)> _CheckDeviceFormatConversion_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT)> _CheckDeviceFormatConversion_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DDEVTYPE &, D3DCAPS9* &)> _GetDeviceCaps_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DDEVTYPE, D3DCAPS9*)> _GetDeviceCaps_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter);
    NullPtr<void (Direct3D9Hook::*)(UINT &)> _GetAdapterMonitor_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HMONITOR, UINT)> _GetAdapterMonitor_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
    NullPtr<void (Direct3D9Hook::*)(UINT &, D3DDEVTYPE &, HWND &, DWORD &, D3DPRESENT_PARAMETERS* &, IDirect3DDevice9** &)> _CreateDevice_pre_ptr;
    NullPtr<void (Direct3D9Hook::*)(HRESULT, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, IDirect3DDevice9**)> _CreateDevice_post_ptr;

};

// ==============================================================================
// IDirect3DDevice9
// ==============================================================================
class Direct3DDevice9Hook : public HookBase<IDirect3DDevice9>
{

protected:

    Direct3DDevice9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DDevice9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DDevice9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DDevice9Hook * typedPtr = (Direct3DDevice9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE TestCooperativeLevel();
    NullPtr<void (Direct3DDevice9Hook::*)()> _TestCooperativeLevel_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT)> _TestCooperativeLevel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetAvailableTextureMem();
    NullPtr<void (Direct3DDevice9Hook::*)()> _GetAvailableTextureMem_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(UINT)> _GetAvailableTextureMem_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EvictManagedResources();
    NullPtr<void (Direct3DDevice9Hook::*)()> _EvictManagedResources_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT)> _EvictManagedResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D9** ppD3D9);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3D9** &)> _GetDirect3D_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3D9**)> _GetDirect3D_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS9* pCaps);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DCAPS9* &)> _GetDeviceCaps_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DCAPS9*)> _GetDeviceCaps_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, D3DDISPLAYMODE* &)> _GetDisplayMode_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, D3DDISPLAYMODE*)> _GetDisplayMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS * pParameters);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DDEVICE_CREATION_PARAMETERS * &)> _GetCreationParameters_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DDEVICE_CREATION_PARAMETERS *)> _GetCreationParameters_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, IDirect3DSurface9* &)> _SetCursorProperties_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, IDirect3DSurface9*)> _SetCursorProperties_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetCursorPosition(int X, int Y, DWORD Flags);
    NullPtr<void (Direct3DDevice9Hook::*)(int &, int &, DWORD &)> _SetCursorPosition_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(int, int, DWORD)> _SetCursorPosition_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE ShowCursor(BOOL bShow);
    NullPtr<void (Direct3DDevice9Hook::*)(BOOL &)> _ShowCursor_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(BOOL, BOOL)> _ShowCursor_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** ppSwapChain);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DPRESENT_PARAMETERS* &, IDirect3DSwapChain9** &)> _CreateAdditionalSwapChain_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DPRESENT_PARAMETERS*, IDirect3DSwapChain9**)> _CreateAdditionalSwapChain_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** ppSwapChain);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, IDirect3DSwapChain9** &)> _GetSwapChain_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, IDirect3DSwapChain9**)> _GetSwapChain_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetNumberOfSwapChains();
    NullPtr<void (Direct3DDevice9Hook::*)()> _GetNumberOfSwapChains_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(UINT)> _GetNumberOfSwapChains_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DPRESENT_PARAMETERS* &)> _Reset_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DPRESENT_PARAMETERS*)> _Reset_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
    NullPtr<void (Direct3DDevice9Hook::*)(CONST RECT* &, CONST RECT* &, HWND &, CONST RGNDATA* &)> _Present_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*)> _Present_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, D3DBACKBUFFER_TYPE &, IDirect3DSurface9** &)> _GetBackBuffer_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface9**)> _GetBackBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, D3DRASTER_STATUS* &)> _GetRasterStatus_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, D3DRASTER_STATUS*)> _GetRasterStatus_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetDialogBoxMode(BOOL bEnableDialogs);
    NullPtr<void (Direct3DDevice9Hook::*)(BOOL &)> _SetDialogBoxMode_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, BOOL)> _SetDialogBoxMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, DWORD &, CONST D3DGAMMARAMP* &)> _SetGammaRamp_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(UINT, DWORD, CONST D3DGAMMARAMP*)> _SetGammaRamp_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, D3DGAMMARAMP* &)> _GetGammaRamp_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(UINT, D3DGAMMARAMP*)> _GetGammaRamp_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, UINT &, DWORD &, D3DFORMAT &, D3DPOOL &, IDirect3DTexture9** &, HANDLE* &)> _CreateTexture_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DTexture9**, HANDLE*)> _CreateTexture_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, UINT &, UINT &, DWORD &, D3DFORMAT &, D3DPOOL &, IDirect3DVolumeTexture9** &, HANDLE* &)> _CreateVolumeTexture_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DVolumeTexture9**, HANDLE*)> _CreateVolumeTexture_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, DWORD &, D3DFORMAT &, D3DPOOL &, IDirect3DCubeTexture9** &, HANDLE* &)> _CreateCubeTexture_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DCubeTexture9**, HANDLE*)> _CreateCubeTexture_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, DWORD &, DWORD &, D3DPOOL &, IDirect3DVertexBuffer9** &, HANDLE* &)> _CreateVertexBuffer_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer9**, HANDLE*)> _CreateVertexBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, DWORD &, D3DFORMAT &, D3DPOOL &, IDirect3DIndexBuffer9** &, HANDLE* &)> _CreateIndexBuffer_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DIndexBuffer9**, HANDLE*)> _CreateIndexBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, D3DFORMAT &, D3DMULTISAMPLE_TYPE &, DWORD &, BOOL &, IDirect3DSurface9** &, HANDLE* &)> _CreateRenderTarget_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface9**, HANDLE*)> _CreateRenderTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, D3DFORMAT &, D3DMULTISAMPLE_TYPE &, DWORD &, BOOL &, IDirect3DSurface9** &, HANDLE* &)> _CreateDepthStencilSurface_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface9**, HANDLE*)> _CreateDepthStencilSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DSurface9* &, CONST RECT* &, IDirect3DSurface9* &, CONST POINT* &)> _UpdateSurface_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DSurface9*, CONST RECT*, IDirect3DSurface9*, CONST POINT*)> _UpdateSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DBaseTexture9* &, IDirect3DBaseTexture9* &)> _UpdateTexture_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DBaseTexture9*, IDirect3DBaseTexture9*)> _UpdateTexture_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DSurface9* &, IDirect3DSurface9* &)> _GetRenderTargetData_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DSurface9*, IDirect3DSurface9*)> _GetRenderTargetData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, IDirect3DSurface9* &)> _GetFrontBufferData_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, IDirect3DSurface9*)> _GetFrontBufferData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DSurface9* &, CONST RECT* &, IDirect3DSurface9* &, CONST RECT* &, D3DTEXTUREFILTERTYPE &)> _StretchRect_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DSurface9*, CONST RECT*, IDirect3DSurface9*, CONST RECT*, D3DTEXTUREFILTERTYPE)> _StretchRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DSurface9* &, CONST RECT* &, D3DCOLOR &)> _ColorFill_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DSurface9*, CONST RECT*, D3DCOLOR)> _ColorFill_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, D3DFORMAT &, D3DPOOL &, IDirect3DSurface9** &, HANDLE* &)> _CreateOffscreenPlainSurface_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, D3DFORMAT, D3DPOOL, IDirect3DSurface9**, HANDLE*)> _CreateOffscreenPlainSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, IDirect3DSurface9* &)> _SetRenderTarget_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, IDirect3DSurface9*)> _SetRenderTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, IDirect3DSurface9** &)> _GetRenderTarget_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, IDirect3DSurface9**)> _GetRenderTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DSurface9* &)> _SetDepthStencilSurface_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DSurface9*)> _SetDepthStencilSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DSurface9** &)> _GetDepthStencilSurface_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DSurface9**)> _GetDepthStencilSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE BeginScene();
    NullPtr<void (Direct3DDevice9Hook::*)()> _BeginScene_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT)> _BeginScene_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EndScene();
    NullPtr<void (Direct3DDevice9Hook::*)()> _EndScene_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT)> _EndScene_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, CONST D3DRECT* &, DWORD &, D3DCOLOR &, float &, DWORD &)> _Clear_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, CONST D3DRECT*, DWORD, D3DCOLOR, float, DWORD)> _Clear_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DTRANSFORMSTATETYPE &, CONST D3DMATRIX* &)> _SetTransform_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*)> _SetTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DTRANSFORMSTATETYPE &, D3DMATRIX* &)> _GetTransform_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DTRANSFORMSTATETYPE, D3DMATRIX*)> _GetTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE TransformType, CONST D3DMATRIX* Value);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DTRANSFORMSTATETYPE &, CONST D3DMATRIX* &)> _MultiplyTransform_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*)> _MultiplyTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetViewport(CONST D3DVIEWPORT9* pViewport);
    NullPtr<void (Direct3DDevice9Hook::*)(CONST D3DVIEWPORT9* &)> _SetViewport_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, CONST D3DVIEWPORT9*)> _SetViewport_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT9* pViewport);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DVIEWPORT9* &)> _GetViewport_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DVIEWPORT9*)> _GetViewport_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetMaterial(CONST D3DMATERIAL9* pMaterial);
    NullPtr<void (Direct3DDevice9Hook::*)(CONST D3DMATERIAL9* &)> _SetMaterial_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, CONST D3DMATERIAL9*)> _SetMaterial_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL9* pMaterial);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DMATERIAL9* &)> _GetMaterial_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DMATERIAL9*)> _GetMaterial_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetLight(DWORD Index, CONST D3DLIGHT9* pLight);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, CONST D3DLIGHT9* &)> _SetLight_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, CONST D3DLIGHT9*)> _SetLight_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLight(DWORD Index, D3DLIGHT9* pLight);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, D3DLIGHT9* &)> _GetLight_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, D3DLIGHT9*)> _GetLight_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE LightEnable(DWORD Index, BOOL Enable);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, BOOL &)> _LightEnable_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, BOOL)> _LightEnable_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD Index, BOOL* pEnable);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, BOOL* &)> _GetLightEnable_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, BOOL*)> _GetLightEnable_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD Index, CONST float* pPlane);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, CONST float* &)> _SetClipPlane_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, CONST float*)> _SetClipPlane_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD Index, float* pPlane);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, float* &)> _GetClipPlane_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, float*)> _GetClipPlane_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DRENDERSTATETYPE &, DWORD &)> _SetRenderState_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DRENDERSTATETYPE, DWORD)> _SetRenderState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DRENDERSTATETYPE &, DWORD* &)> _GetRenderState_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DRENDERSTATETYPE, DWORD*)> _GetRenderState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DSTATEBLOCKTYPE &, IDirect3DStateBlock9** &)> _CreateStateBlock_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DSTATEBLOCKTYPE, IDirect3DStateBlock9**)> _CreateStateBlock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE BeginStateBlock();
    NullPtr<void (Direct3DDevice9Hook::*)()> _BeginStateBlock_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT)> _BeginStateBlock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9** ppSB);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DStateBlock9** &)> _EndStateBlock_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DStateBlock9**)> _EndStateBlock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus);
    NullPtr<void (Direct3DDevice9Hook::*)(CONST D3DCLIPSTATUS9* &)> _SetClipStatus_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, CONST D3DCLIPSTATUS9*)> _SetClipStatus_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS9* pClipStatus);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DCLIPSTATUS9* &)> _GetClipStatus_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DCLIPSTATUS9*)> _GetClipStatus_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, IDirect3DBaseTexture9** &)> _GetTexture_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, IDirect3DBaseTexture9**)> _GetTexture_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, IDirect3DBaseTexture9* &)> _SetTexture_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, IDirect3DBaseTexture9*)> _SetTexture_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, D3DTEXTURESTAGESTATETYPE &, DWORD* &)> _GetTextureStageState_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, D3DTEXTURESTAGESTATETYPE, DWORD*)> _GetTextureStageState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, D3DTEXTURESTAGESTATETYPE &, DWORD &)> _SetTextureStageState_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, D3DTEXTURESTAGESTATETYPE, DWORD)> _SetTextureStageState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, D3DSAMPLERSTATETYPE &, DWORD* &)> _GetSamplerState_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, D3DSAMPLERSTATETYPE, DWORD*)> _GetSamplerState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &, D3DSAMPLERSTATETYPE &, DWORD &)> _SetSamplerState_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD, D3DSAMPLERSTATETYPE, DWORD)> _SetSamplerState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD* pNumPasses);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD* &)> _ValidateDevice_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD*)> _ValidateDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST PALETTEENTRY* &)> _SetPaletteEntries_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST PALETTEENTRY*)> _SetPaletteEntries_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, PALETTEENTRY* &)> _GetPaletteEntries_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, PALETTEENTRY*)> _GetPaletteEntries_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT PaletteNumber);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &)> _SetCurrentTexturePalette_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT)> _SetCurrentTexturePalette_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCurrentTexturePalette(UINT * PaletteNumber);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT * &)> _GetCurrentTexturePalette_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT *)> _GetCurrentTexturePalette_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetScissorRect(CONST RECT* pRect);
    NullPtr<void (Direct3DDevice9Hook::*)(CONST RECT* &)> _SetScissorRect_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, CONST RECT*)> _SetScissorRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetScissorRect(RECT* pRect);
    NullPtr<void (Direct3DDevice9Hook::*)(RECT* &)> _GetScissorRect_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, RECT*)> _GetScissorRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL bSoftware);
    NullPtr<void (Direct3DDevice9Hook::*)(BOOL &)> _SetSoftwareVertexProcessing_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, BOOL)> _SetSoftwareVertexProcessing_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing();
    NullPtr<void (Direct3DDevice9Hook::*)()> _GetSoftwareVertexProcessing_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(BOOL)> _GetSoftwareVertexProcessing_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetNPatchMode(float nSegments);
    NullPtr<void (Direct3DDevice9Hook::*)(float &)> _SetNPatchMode_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, float)> _SetNPatchMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual float STDMETHODCALLTYPE GetNPatchMode();
    NullPtr<void (Direct3DDevice9Hook::*)()> _GetNPatchMode_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(float)> _GetNPatchMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DPRIMITIVETYPE &, UINT &, UINT &)> _DrawPrimitive_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DPRIMITIVETYPE, UINT, UINT)> _DrawPrimitive_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DPRIMITIVETYPE &, INT &, UINT &, UINT &, UINT &, UINT &)> _DrawIndexedPrimitive_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT)> _DrawIndexedPrimitive_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DPRIMITIVETYPE &, UINT &, CONST void* &, UINT &)> _DrawPrimitiveUP_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DPRIMITIVETYPE, UINT, CONST void*, UINT)> _DrawPrimitiveUP_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DPRIMITIVETYPE &, UINT &, UINT &, UINT &, CONST void* &, D3DFORMAT &, CONST void* &, UINT &)> _DrawIndexedPrimitiveUP_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DPRIMITIVETYPE, UINT, UINT, UINT, CONST void*, D3DFORMAT, CONST void*, UINT)> _DrawIndexedPrimitiveUP_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &, UINT &, IDirect3DVertexBuffer9* &, IDirect3DVertexDeclaration9* &, DWORD &)> _ProcessVertices_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT, UINT, IDirect3DVertexBuffer9*, IDirect3DVertexDeclaration9*, DWORD)> _ProcessVertices_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
    NullPtr<void (Direct3DDevice9Hook::*)(CONST D3DVERTEXELEMENT9* &, IDirect3DVertexDeclaration9** &)> _CreateVertexDeclaration_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, CONST D3DVERTEXELEMENT9*, IDirect3DVertexDeclaration9**)> _CreateVertexDeclaration_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DVertexDeclaration9* &)> _SetVertexDeclaration_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DVertexDeclaration9*)> _SetVertexDeclaration_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DVertexDeclaration9** &)> _GetVertexDeclaration_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DVertexDeclaration9**)> _GetVertexDeclaration_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFVF(DWORD FVF);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD &)> _SetFVF_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD)> _SetFVF_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFVF(DWORD* pFVF);
    NullPtr<void (Direct3DDevice9Hook::*)(DWORD* &)> _GetFVF_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, DWORD*)> _GetFVF_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader);
    NullPtr<void (Direct3DDevice9Hook::*)(CONST DWORD* &, IDirect3DVertexShader9** &)> _CreateVertexShader_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, CONST DWORD*, IDirect3DVertexShader9**)> _CreateVertexShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9* pShader);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DVertexShader9* &)> _SetVertexShader_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DVertexShader9*)> _SetVertexShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVertexShader(IDirect3DVertexShader9** ppShader);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DVertexShader9** &)> _GetVertexShader_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DVertexShader9**)> _GetVertexShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST float* &, UINT &)> _SetVertexShaderConstantF_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST float*, UINT)> _SetVertexShaderConstantF_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, float* &, UINT &)> _GetVertexShaderConstantF_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, float*, UINT)> _GetVertexShaderConstantF_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST int* &, UINT &)> _SetVertexShaderConstantI_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST int*, UINT)> _SetVertexShaderConstantI_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, int* &, UINT &)> _GetVertexShaderConstantI_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, int*, UINT)> _GetVertexShaderConstantI_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST BOOL* &, UINT &)> _SetVertexShaderConstantB_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST BOOL*, UINT)> _SetVertexShaderConstantB_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, BOOL* &, UINT &)> _GetVertexShaderConstantB_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, BOOL*, UINT)> _GetVertexShaderConstantB_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, IDirect3DVertexBuffer9* &, UINT &, UINT &)> _SetStreamSource_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, IDirect3DVertexBuffer9*, UINT, UINT)> _SetStreamSource_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, IDirect3DVertexBuffer9** &, UINT* &, UINT* &)> _GetStreamSource_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, IDirect3DVertexBuffer9**, UINT*, UINT*)> _GetStreamSource_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT StreamNumber, UINT Setting);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT &)> _SetStreamSourceFreq_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT)> _SetStreamSourceFreq_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, UINT* &)> _GetStreamSourceFreq_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, UINT*)> _GetStreamSourceFreq_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9* pIndexData);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DIndexBuffer9* &)> _SetIndices_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DIndexBuffer9*)> _SetIndices_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer9** ppIndexData);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DIndexBuffer9** &)> _GetIndices_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DIndexBuffer9**)> _GetIndices_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader);
    NullPtr<void (Direct3DDevice9Hook::*)(CONST DWORD* &, IDirect3DPixelShader9** &)> _CreatePixelShader_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, CONST DWORD*, IDirect3DPixelShader9**)> _CreatePixelShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9* pShader);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DPixelShader9* &)> _SetPixelShader_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DPixelShader9*)> _SetPixelShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPixelShader(IDirect3DPixelShader9** ppShader);
    NullPtr<void (Direct3DDevice9Hook::*)(IDirect3DPixelShader9** &)> _GetPixelShader_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, IDirect3DPixelShader9**)> _GetPixelShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST float* &, UINT &)> _SetPixelShaderConstantF_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST float*, UINT)> _SetPixelShaderConstantF_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, float* &, UINT &)> _GetPixelShaderConstantF_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, float*, UINT)> _GetPixelShaderConstantF_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST int* &, UINT &)> _SetPixelShaderConstantI_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST int*, UINT)> _SetPixelShaderConstantI_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, int* &, UINT &)> _GetPixelShaderConstantI_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, int*, UINT)> _GetPixelShaderConstantI_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST BOOL* &, UINT &)> _SetPixelShaderConstantB_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST BOOL*, UINT)> _SetPixelShaderConstantB_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, BOOL* &, UINT &)> _GetPixelShaderConstantB_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, BOOL*, UINT)> _GetPixelShaderConstantB_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST float* &, CONST D3DRECTPATCH_INFO* &)> _DrawRectPatch_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST float*, CONST D3DRECTPATCH_INFO*)> _DrawRectPatch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &, CONST float* &, CONST D3DTRIPATCH_INFO* &)> _DrawTriPatch_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT, CONST float*, CONST D3DTRIPATCH_INFO*)> _DrawTriPatch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DeletePatch(UINT Handle);
    NullPtr<void (Direct3DDevice9Hook::*)(UINT &)> _DeletePatch_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, UINT)> _DeletePatch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);
    NullPtr<void (Direct3DDevice9Hook::*)(D3DQUERYTYPE &, IDirect3DQuery9** &)> _CreateQuery_pre_ptr;
    NullPtr<void (Direct3DDevice9Hook::*)(HRESULT, D3DQUERYTYPE, IDirect3DQuery9**)> _CreateQuery_post_ptr;

};

// ==============================================================================
// IDirect3DStateBlock9
// ==============================================================================
class Direct3DStateBlock9Hook : public HookBase<IDirect3DStateBlock9>
{

protected:

    Direct3DStateBlock9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DStateBlock9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DStateBlock9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DStateBlock9Hook * typedPtr = (Direct3DStateBlock9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice);
    NullPtr<void (Direct3DStateBlock9Hook::*)(IDirect3DDevice9** &)> _GetDevice_pre_ptr;
    NullPtr<void (Direct3DStateBlock9Hook::*)(HRESULT, IDirect3DDevice9**)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Capture();
    NullPtr<void (Direct3DStateBlock9Hook::*)()> _Capture_pre_ptr;
    NullPtr<void (Direct3DStateBlock9Hook::*)(HRESULT)> _Capture_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Apply();
    NullPtr<void (Direct3DStateBlock9Hook::*)()> _Apply_pre_ptr;
    NullPtr<void (Direct3DStateBlock9Hook::*)(HRESULT)> _Apply_post_ptr;

};

// ==============================================================================
// IDirect3DSwapChain9
// ==============================================================================
class Direct3DSwapChain9Hook : public HookBase<IDirect3DSwapChain9>
{

protected:

    Direct3DSwapChain9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DSwapChain9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DSwapChain9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DSwapChain9Hook * typedPtr = (Direct3DSwapChain9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
    NullPtr<void (Direct3DSwapChain9Hook::*)(CONST RECT* &, CONST RECT* &, HWND &, CONST RGNDATA* &, DWORD &)> _Present_pre_ptr;
    NullPtr<void (Direct3DSwapChain9Hook::*)(HRESULT, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*, DWORD)> _Present_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFrontBufferData(IDirect3DSurface9* pDestSurface);
    NullPtr<void (Direct3DSwapChain9Hook::*)(IDirect3DSurface9* &)> _GetFrontBufferData_pre_ptr;
    NullPtr<void (Direct3DSwapChain9Hook::*)(HRESULT, IDirect3DSurface9*)> _GetFrontBufferData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    NullPtr<void (Direct3DSwapChain9Hook::*)(UINT &, D3DBACKBUFFER_TYPE &, IDirect3DSurface9** &)> _GetBackBuffer_pre_ptr;
    NullPtr<void (Direct3DSwapChain9Hook::*)(HRESULT, UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface9**)> _GetBackBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRasterStatus(D3DRASTER_STATUS* pRasterStatus);
    NullPtr<void (Direct3DSwapChain9Hook::*)(D3DRASTER_STATUS* &)> _GetRasterStatus_pre_ptr;
    NullPtr<void (Direct3DSwapChain9Hook::*)(HRESULT, D3DRASTER_STATUS*)> _GetRasterStatus_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE* pMode);
    NullPtr<void (Direct3DSwapChain9Hook::*)(D3DDISPLAYMODE* &)> _GetDisplayMode_pre_ptr;
    NullPtr<void (Direct3DSwapChain9Hook::*)(HRESULT, D3DDISPLAYMODE*)> _GetDisplayMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice);
    NullPtr<void (Direct3DSwapChain9Hook::*)(IDirect3DDevice9** &)> _GetDevice_pre_ptr;
    NullPtr<void (Direct3DSwapChain9Hook::*)(HRESULT, IDirect3DDevice9**)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters);
    NullPtr<void (Direct3DSwapChain9Hook::*)(D3DPRESENT_PARAMETERS* &)> _GetPresentParameters_pre_ptr;
    NullPtr<void (Direct3DSwapChain9Hook::*)(HRESULT, D3DPRESENT_PARAMETERS*)> _GetPresentParameters_post_ptr;

};

// ==============================================================================
// IDirect3DResource9
// ==============================================================================
class Direct3DResource9Hook : public HookBase<IDirect3DResource9>
{

protected:

    Direct3DResource9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DResource9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DResource9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DResource9Hook * typedPtr = (Direct3DResource9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice);
    NullPtr<void (Direct3DResource9Hook::*)(IDirect3DDevice9** &)> _GetDevice_pre_ptr;
    NullPtr<void (Direct3DResource9Hook::*)(HRESULT, IDirect3DDevice9**)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
    NullPtr<void (Direct3DResource9Hook::*)(REFGUID, CONST void* &, DWORD &, DWORD &)> _SetPrivateData_pre_ptr;
    NullPtr<void (Direct3DResource9Hook::*)(HRESULT, REFGUID, CONST void*, DWORD, DWORD)> _SetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
    NullPtr<void (Direct3DResource9Hook::*)(REFGUID, void* &, DWORD* &)> _GetPrivateData_pre_ptr;
    NullPtr<void (Direct3DResource9Hook::*)(HRESULT, REFGUID, void*, DWORD*)> _GetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid);
    NullPtr<void (Direct3DResource9Hook::*)(REFGUID)> _FreePrivateData_pre_ptr;
    NullPtr<void (Direct3DResource9Hook::*)(HRESULT, REFGUID)> _FreePrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew);
    NullPtr<void (Direct3DResource9Hook::*)(DWORD &)> _SetPriority_pre_ptr;
    NullPtr<void (Direct3DResource9Hook::*)(DWORD, DWORD)> _SetPriority_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWORD STDMETHODCALLTYPE GetPriority();
    NullPtr<void (Direct3DResource9Hook::*)()> _GetPriority_pre_ptr;
    NullPtr<void (Direct3DResource9Hook::*)(DWORD)> _GetPriority_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PreLoad();
    NullPtr<void (Direct3DResource9Hook::*)()> _PreLoad_pre_ptr;
    NullPtr<void (Direct3DResource9Hook::*)()> _PreLoad_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D3DRESOURCETYPE STDMETHODCALLTYPE GetType();
    NullPtr<void (Direct3DResource9Hook::*)()> _GetType_pre_ptr;
    NullPtr<void (Direct3DResource9Hook::*)(D3DRESOURCETYPE)> _GetType_post_ptr;

};

// ==============================================================================
// IDirect3DVertexDeclaration9
// ==============================================================================
class Direct3DVertexDeclaration9Hook : public HookBase<IDirect3DVertexDeclaration9>
{

protected:

    Direct3DVertexDeclaration9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DVertexDeclaration9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DVertexDeclaration9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DVertexDeclaration9Hook * typedPtr = (Direct3DVertexDeclaration9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice);
    NullPtr<void (Direct3DVertexDeclaration9Hook::*)(IDirect3DDevice9** &)> _GetDevice_pre_ptr;
    NullPtr<void (Direct3DVertexDeclaration9Hook::*)(HRESULT, IDirect3DDevice9**)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDeclaration(D3DVERTEXELEMENT9* pElement, UINT* pNumElements);
    NullPtr<void (Direct3DVertexDeclaration9Hook::*)(D3DVERTEXELEMENT9* &, UINT* &)> _GetDeclaration_pre_ptr;
    NullPtr<void (Direct3DVertexDeclaration9Hook::*)(HRESULT, D3DVERTEXELEMENT9*, UINT*)> _GetDeclaration_post_ptr;

};

// ==============================================================================
// IDirect3DVertexShader9
// ==============================================================================
class Direct3DVertexShader9Hook : public HookBase<IDirect3DVertexShader9>
{

protected:

    Direct3DVertexShader9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DVertexShader9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DVertexShader9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DVertexShader9Hook * typedPtr = (Direct3DVertexShader9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice);
    NullPtr<void (Direct3DVertexShader9Hook::*)(IDirect3DDevice9** &)> _GetDevice_pre_ptr;
    NullPtr<void (Direct3DVertexShader9Hook::*)(HRESULT, IDirect3DDevice9**)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFunction(void* pFunction, UINT* pSizeOfData);
    NullPtr<void (Direct3DVertexShader9Hook::*)(void* &, UINT* &)> _GetFunction_pre_ptr;
    NullPtr<void (Direct3DVertexShader9Hook::*)(HRESULT, void*, UINT*)> _GetFunction_post_ptr;

};

// ==============================================================================
// IDirect3DPixelShader9
// ==============================================================================
class Direct3DPixelShader9Hook : public HookBase<IDirect3DPixelShader9>
{

protected:

    Direct3DPixelShader9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DPixelShader9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DPixelShader9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DPixelShader9Hook * typedPtr = (Direct3DPixelShader9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice);
    NullPtr<void (Direct3DPixelShader9Hook::*)(IDirect3DDevice9** &)> _GetDevice_pre_ptr;
    NullPtr<void (Direct3DPixelShader9Hook::*)(HRESULT, IDirect3DDevice9**)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFunction(void* pFunction, UINT* pSizeOfData);
    NullPtr<void (Direct3DPixelShader9Hook::*)(void* &, UINT* &)> _GetFunction_pre_ptr;
    NullPtr<void (Direct3DPixelShader9Hook::*)(HRESULT, void*, UINT*)> _GetFunction_post_ptr;

};

// ==============================================================================
// IDirect3DBaseTexture9
// ==============================================================================
class Direct3DBaseTexture9Hook : public HookBase<IDirect3DBaseTexture9>
{
    Direct3DResource9Hook & _Direct3DResource9;

protected:

    Direct3DBaseTexture9Hook(UnknownBase & unknown, Direct3DResource9Hook & Direct3DResource9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DResource9(Direct3DResource9)
    {
    }

    ~Direct3DBaseTexture9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DResource9Hook * Direct3DResource9 = (Direct3DResource9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DResource9), realobj);
        if (nullptr == Direct3DResource9) return nullptr;

        try
        {
            return new Direct3DBaseTexture9Hook(unknown, *Direct3DResource9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DBaseTexture9Hook * typedPtr = (Direct3DBaseTexture9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice)
    {
        return _Direct3DResource9.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
    {
        return _Direct3DResource9.SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
    {
        return _Direct3DResource9.GetPrivateData(refguid, pData, pSizeOfData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid)
    {
        return _Direct3DResource9.FreePrivateData(refguid);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew)
    {
        return _Direct3DResource9.SetPriority(PriorityNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetPriority()
    {
        return _Direct3DResource9.GetPriority();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PreLoad()
    {
        return _Direct3DResource9.PreLoad();
    }

    // -----------------------------------------------------------------------------
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType()
    {
        return _Direct3DResource9.GetType();
    }

    // -----------------------------------------------------------------------------
    virtual DWORD STDMETHODCALLTYPE SetLOD(DWORD LODNew);
    NullPtr<void (Direct3DBaseTexture9Hook::*)(DWORD &)> _SetLOD_pre_ptr;
    NullPtr<void (Direct3DBaseTexture9Hook::*)(DWORD, DWORD)> _SetLOD_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWORD STDMETHODCALLTYPE GetLOD();
    NullPtr<void (Direct3DBaseTexture9Hook::*)()> _GetLOD_pre_ptr;
    NullPtr<void (Direct3DBaseTexture9Hook::*)(DWORD)> _GetLOD_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWORD STDMETHODCALLTYPE GetLevelCount();
    NullPtr<void (Direct3DBaseTexture9Hook::*)()> _GetLevelCount_pre_ptr;
    NullPtr<void (Direct3DBaseTexture9Hook::*)(DWORD)> _GetLevelCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
    NullPtr<void (Direct3DBaseTexture9Hook::*)(D3DTEXTUREFILTERTYPE &)> _SetAutoGenFilterType_pre_ptr;
    NullPtr<void (Direct3DBaseTexture9Hook::*)(HRESULT, D3DTEXTUREFILTERTYPE)> _SetAutoGenFilterType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE GetAutoGenFilterType();
    NullPtr<void (Direct3DBaseTexture9Hook::*)()> _GetAutoGenFilterType_pre_ptr;
    NullPtr<void (Direct3DBaseTexture9Hook::*)(D3DTEXTUREFILTERTYPE)> _GetAutoGenFilterType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GenerateMipSubLevels();
    NullPtr<void (Direct3DBaseTexture9Hook::*)()> _GenerateMipSubLevels_pre_ptr;
    NullPtr<void (Direct3DBaseTexture9Hook::*)()> _GenerateMipSubLevels_post_ptr;

};

// ==============================================================================
// IDirect3DTexture9
// ==============================================================================
class Direct3DTexture9Hook : public HookBase<IDirect3DTexture9>
{
    Direct3DResource9Hook & _Direct3DResource9;
    Direct3DBaseTexture9Hook & _Direct3DBaseTexture9;

protected:

    Direct3DTexture9Hook(UnknownBase & unknown, Direct3DResource9Hook & Direct3DResource9, Direct3DBaseTexture9Hook & Direct3DBaseTexture9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DResource9(Direct3DResource9)
        , _Direct3DBaseTexture9(Direct3DBaseTexture9)
    {
    }

    ~Direct3DTexture9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DResource9Hook * Direct3DResource9 = (Direct3DResource9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DResource9), realobj);
        if (nullptr == Direct3DResource9) return nullptr;

        Direct3DBaseTexture9Hook * Direct3DBaseTexture9 = (Direct3DBaseTexture9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DBaseTexture9), realobj);
        if (nullptr == Direct3DBaseTexture9) return nullptr;

        try
        {
            return new Direct3DTexture9Hook(unknown, *Direct3DResource9, *Direct3DBaseTexture9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DTexture9Hook * typedPtr = (Direct3DTexture9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice)
    {
        return _Direct3DResource9.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
    {
        return _Direct3DResource9.SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
    {
        return _Direct3DResource9.GetPrivateData(refguid, pData, pSizeOfData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid)
    {
        return _Direct3DResource9.FreePrivateData(refguid);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew)
    {
        return _Direct3DResource9.SetPriority(PriorityNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetPriority()
    {
        return _Direct3DResource9.GetPriority();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PreLoad()
    {
        return _Direct3DResource9.PreLoad();
    }

    // -----------------------------------------------------------------------------
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType()
    {
        return _Direct3DResource9.GetType();
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetLOD(DWORD LODNew)
    {
        return _Direct3DBaseTexture9.SetLOD(LODNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetLOD()
    {
        return _Direct3DBaseTexture9.GetLOD();
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetLevelCount()
    {
        return _Direct3DBaseTexture9.GetLevelCount();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
    {
        return _Direct3DBaseTexture9.SetAutoGenFilterType(FilterType);
    }

    // -----------------------------------------------------------------------------
    D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE GetAutoGenFilterType()
    {
        return _Direct3DBaseTexture9.GetAutoGenFilterType();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GenerateMipSubLevels()
    {
        return _Direct3DBaseTexture9.GenerateMipSubLevels();
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLevelDesc(UINT Level, D3DSURFACE_DESC * pDesc);
    NullPtr<void (Direct3DTexture9Hook::*)(UINT &, D3DSURFACE_DESC * &)> _GetLevelDesc_pre_ptr;
    NullPtr<void (Direct3DTexture9Hook::*)(HRESULT, UINT, D3DSURFACE_DESC *)> _GetLevelDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel);
    NullPtr<void (Direct3DTexture9Hook::*)(UINT &, IDirect3DSurface9** &)> _GetSurfaceLevel_pre_ptr;
    NullPtr<void (Direct3DTexture9Hook::*)(HRESULT, UINT, IDirect3DSurface9**)> _GetSurfaceLevel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
    NullPtr<void (Direct3DTexture9Hook::*)(UINT &, D3DLOCKED_RECT* &, CONST RECT* &, DWORD &)> _LockRect_pre_ptr;
    NullPtr<void (Direct3DTexture9Hook::*)(HRESULT, UINT, D3DLOCKED_RECT*, CONST RECT*, DWORD)> _LockRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnlockRect(UINT Level);
    NullPtr<void (Direct3DTexture9Hook::*)(UINT &)> _UnlockRect_pre_ptr;
    NullPtr<void (Direct3DTexture9Hook::*)(HRESULT, UINT)> _UnlockRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddDirtyRect(CONST RECT* pDirtyRect);
    NullPtr<void (Direct3DTexture9Hook::*)(CONST RECT* &)> _AddDirtyRect_pre_ptr;
    NullPtr<void (Direct3DTexture9Hook::*)(HRESULT, CONST RECT*)> _AddDirtyRect_post_ptr;

};

// ==============================================================================
// IDirect3DVolumeTexture9
// ==============================================================================
class Direct3DVolumeTexture9Hook : public HookBase<IDirect3DVolumeTexture9>
{
    Direct3DResource9Hook & _Direct3DResource9;
    Direct3DBaseTexture9Hook & _Direct3DBaseTexture9;

protected:

    Direct3DVolumeTexture9Hook(UnknownBase & unknown, Direct3DResource9Hook & Direct3DResource9, Direct3DBaseTexture9Hook & Direct3DBaseTexture9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DResource9(Direct3DResource9)
        , _Direct3DBaseTexture9(Direct3DBaseTexture9)
    {
    }

    ~Direct3DVolumeTexture9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DResource9Hook * Direct3DResource9 = (Direct3DResource9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DResource9), realobj);
        if (nullptr == Direct3DResource9) return nullptr;

        Direct3DBaseTexture9Hook * Direct3DBaseTexture9 = (Direct3DBaseTexture9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DBaseTexture9), realobj);
        if (nullptr == Direct3DBaseTexture9) return nullptr;

        try
        {
            return new Direct3DVolumeTexture9Hook(unknown, *Direct3DResource9, *Direct3DBaseTexture9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DVolumeTexture9Hook * typedPtr = (Direct3DVolumeTexture9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice)
    {
        return _Direct3DResource9.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
    {
        return _Direct3DResource9.SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
    {
        return _Direct3DResource9.GetPrivateData(refguid, pData, pSizeOfData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid)
    {
        return _Direct3DResource9.FreePrivateData(refguid);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew)
    {
        return _Direct3DResource9.SetPriority(PriorityNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetPriority()
    {
        return _Direct3DResource9.GetPriority();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PreLoad()
    {
        return _Direct3DResource9.PreLoad();
    }

    // -----------------------------------------------------------------------------
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType()
    {
        return _Direct3DResource9.GetType();
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetLOD(DWORD LODNew)
    {
        return _Direct3DBaseTexture9.SetLOD(LODNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetLOD()
    {
        return _Direct3DBaseTexture9.GetLOD();
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetLevelCount()
    {
        return _Direct3DBaseTexture9.GetLevelCount();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
    {
        return _Direct3DBaseTexture9.SetAutoGenFilterType(FilterType);
    }

    // -----------------------------------------------------------------------------
    D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE GetAutoGenFilterType()
    {
        return _Direct3DBaseTexture9.GetAutoGenFilterType();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GenerateMipSubLevels()
    {
        return _Direct3DBaseTexture9.GenerateMipSubLevels();
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLevelDesc(UINT Level, D3DVOLUME_DESC * pDesc);
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(UINT &, D3DVOLUME_DESC * &)> _GetLevelDesc_pre_ptr;
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(HRESULT, UINT, D3DVOLUME_DESC *)> _GetLevelDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVolumeLevel(UINT Level, IDirect3DVolume9** ppVolumeLevel);
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(UINT &, IDirect3DVolume9** &)> _GetVolumeLevel_pre_ptr;
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(HRESULT, UINT, IDirect3DVolume9**)> _GetVolumeLevel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE LockBox(UINT Level, D3DLOCKED_BOX* pLockedVolume, CONST D3DBOX* pBox, DWORD Flags);
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(UINT &, D3DLOCKED_BOX* &, CONST D3DBOX* &, DWORD &)> _LockBox_pre_ptr;
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(HRESULT, UINT, D3DLOCKED_BOX*, CONST D3DBOX*, DWORD)> _LockBox_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnlockBox(UINT Level);
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(UINT &)> _UnlockBox_pre_ptr;
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(HRESULT, UINT)> _UnlockBox_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddDirtyBox(CONST D3DBOX* pDirtyBox);
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(CONST D3DBOX* &)> _AddDirtyBox_pre_ptr;
    NullPtr<void (Direct3DVolumeTexture9Hook::*)(HRESULT, CONST D3DBOX*)> _AddDirtyBox_post_ptr;

};

// ==============================================================================
// IDirect3DCubeTexture9
// ==============================================================================
class Direct3DCubeTexture9Hook : public HookBase<IDirect3DCubeTexture9>
{
    Direct3DResource9Hook & _Direct3DResource9;
    Direct3DBaseTexture9Hook & _Direct3DBaseTexture9;

protected:

    Direct3DCubeTexture9Hook(UnknownBase & unknown, Direct3DResource9Hook & Direct3DResource9, Direct3DBaseTexture9Hook & Direct3DBaseTexture9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DResource9(Direct3DResource9)
        , _Direct3DBaseTexture9(Direct3DBaseTexture9)
    {
    }

    ~Direct3DCubeTexture9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DResource9Hook * Direct3DResource9 = (Direct3DResource9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DResource9), realobj);
        if (nullptr == Direct3DResource9) return nullptr;

        Direct3DBaseTexture9Hook * Direct3DBaseTexture9 = (Direct3DBaseTexture9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DBaseTexture9), realobj);
        if (nullptr == Direct3DBaseTexture9) return nullptr;

        try
        {
            return new Direct3DCubeTexture9Hook(unknown, *Direct3DResource9, *Direct3DBaseTexture9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DCubeTexture9Hook * typedPtr = (Direct3DCubeTexture9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice)
    {
        return _Direct3DResource9.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
    {
        return _Direct3DResource9.SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
    {
        return _Direct3DResource9.GetPrivateData(refguid, pData, pSizeOfData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid)
    {
        return _Direct3DResource9.FreePrivateData(refguid);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew)
    {
        return _Direct3DResource9.SetPriority(PriorityNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetPriority()
    {
        return _Direct3DResource9.GetPriority();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PreLoad()
    {
        return _Direct3DResource9.PreLoad();
    }

    // -----------------------------------------------------------------------------
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType()
    {
        return _Direct3DResource9.GetType();
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetLOD(DWORD LODNew)
    {
        return _Direct3DBaseTexture9.SetLOD(LODNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetLOD()
    {
        return _Direct3DBaseTexture9.GetLOD();
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetLevelCount()
    {
        return _Direct3DBaseTexture9.GetLevelCount();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
    {
        return _Direct3DBaseTexture9.SetAutoGenFilterType(FilterType);
    }

    // -----------------------------------------------------------------------------
    D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE GetAutoGenFilterType()
    {
        return _Direct3DBaseTexture9.GetAutoGenFilterType();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GenerateMipSubLevels()
    {
        return _Direct3DBaseTexture9.GenerateMipSubLevels();
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLevelDesc(UINT Level, D3DSURFACE_DESC * pDesc);
    NullPtr<void (Direct3DCubeTexture9Hook::*)(UINT &, D3DSURFACE_DESC * &)> _GetLevelDesc_pre_ptr;
    NullPtr<void (Direct3DCubeTexture9Hook::*)(HRESULT, UINT, D3DSURFACE_DESC *)> _GetLevelDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCubeMapSurface(D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface);
    NullPtr<void (Direct3DCubeTexture9Hook::*)(D3DCUBEMAP_FACES &, UINT &, IDirect3DSurface9** &)> _GetCubeMapSurface_pre_ptr;
    NullPtr<void (Direct3DCubeTexture9Hook::*)(HRESULT, D3DCUBEMAP_FACES, UINT, IDirect3DSurface9**)> _GetCubeMapSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE LockRect(D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
    NullPtr<void (Direct3DCubeTexture9Hook::*)(D3DCUBEMAP_FACES &, UINT &, D3DLOCKED_RECT* &, CONST RECT* &, DWORD &)> _LockRect_pre_ptr;
    NullPtr<void (Direct3DCubeTexture9Hook::*)(HRESULT, D3DCUBEMAP_FACES, UINT, D3DLOCKED_RECT*, CONST RECT*, DWORD)> _LockRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnlockRect(D3DCUBEMAP_FACES FaceType, UINT Level);
    NullPtr<void (Direct3DCubeTexture9Hook::*)(D3DCUBEMAP_FACES &, UINT &)> _UnlockRect_pre_ptr;
    NullPtr<void (Direct3DCubeTexture9Hook::*)(HRESULT, D3DCUBEMAP_FACES, UINT)> _UnlockRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddDirtyRect(D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect);
    NullPtr<void (Direct3DCubeTexture9Hook::*)(D3DCUBEMAP_FACES &, CONST RECT* &)> _AddDirtyRect_pre_ptr;
    NullPtr<void (Direct3DCubeTexture9Hook::*)(HRESULT, D3DCUBEMAP_FACES, CONST RECT*)> _AddDirtyRect_post_ptr;

};

// ==============================================================================
// IDirect3DVertexBuffer9
// ==============================================================================
class Direct3DVertexBuffer9Hook : public HookBase<IDirect3DVertexBuffer9>
{
    Direct3DResource9Hook & _Direct3DResource9;

protected:

    Direct3DVertexBuffer9Hook(UnknownBase & unknown, Direct3DResource9Hook & Direct3DResource9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DResource9(Direct3DResource9)
    {
    }

    ~Direct3DVertexBuffer9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DResource9Hook * Direct3DResource9 = (Direct3DResource9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DResource9), realobj);
        if (nullptr == Direct3DResource9) return nullptr;

        try
        {
            return new Direct3DVertexBuffer9Hook(unknown, *Direct3DResource9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DVertexBuffer9Hook * typedPtr = (Direct3DVertexBuffer9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice)
    {
        return _Direct3DResource9.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
    {
        return _Direct3DResource9.SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
    {
        return _Direct3DResource9.GetPrivateData(refguid, pData, pSizeOfData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid)
    {
        return _Direct3DResource9.FreePrivateData(refguid);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew)
    {
        return _Direct3DResource9.SetPriority(PriorityNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetPriority()
    {
        return _Direct3DResource9.GetPriority();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PreLoad()
    {
        return _Direct3DResource9.PreLoad();
    }

    // -----------------------------------------------------------------------------
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType()
    {
        return _Direct3DResource9.GetType();
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
    NullPtr<void (Direct3DVertexBuffer9Hook::*)(UINT &, UINT &, void** &, DWORD &)> _Lock_pre_ptr;
    NullPtr<void (Direct3DVertexBuffer9Hook::*)(HRESULT, UINT, UINT, void**, DWORD)> _Lock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Unlock();
    NullPtr<void (Direct3DVertexBuffer9Hook::*)()> _Unlock_pre_ptr;
    NullPtr<void (Direct3DVertexBuffer9Hook::*)(HRESULT)> _Unlock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc(D3DVERTEXBUFFER_DESC * pDesc);
    NullPtr<void (Direct3DVertexBuffer9Hook::*)(D3DVERTEXBUFFER_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (Direct3DVertexBuffer9Hook::*)(HRESULT, D3DVERTEXBUFFER_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// IDirect3DIndexBuffer9
// ==============================================================================
class Direct3DIndexBuffer9Hook : public HookBase<IDirect3DIndexBuffer9>
{
    Direct3DResource9Hook & _Direct3DResource9;

protected:

    Direct3DIndexBuffer9Hook(UnknownBase & unknown, Direct3DResource9Hook & Direct3DResource9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DResource9(Direct3DResource9)
    {
    }

    ~Direct3DIndexBuffer9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DResource9Hook * Direct3DResource9 = (Direct3DResource9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DResource9), realobj);
        if (nullptr == Direct3DResource9) return nullptr;

        try
        {
            return new Direct3DIndexBuffer9Hook(unknown, *Direct3DResource9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DIndexBuffer9Hook * typedPtr = (Direct3DIndexBuffer9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice)
    {
        return _Direct3DResource9.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
    {
        return _Direct3DResource9.SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
    {
        return _Direct3DResource9.GetPrivateData(refguid, pData, pSizeOfData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid)
    {
        return _Direct3DResource9.FreePrivateData(refguid);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew)
    {
        return _Direct3DResource9.SetPriority(PriorityNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetPriority()
    {
        return _Direct3DResource9.GetPriority();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PreLoad()
    {
        return _Direct3DResource9.PreLoad();
    }

    // -----------------------------------------------------------------------------
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType()
    {
        return _Direct3DResource9.GetType();
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
    NullPtr<void (Direct3DIndexBuffer9Hook::*)(UINT &, UINT &, void** &, DWORD &)> _Lock_pre_ptr;
    NullPtr<void (Direct3DIndexBuffer9Hook::*)(HRESULT, UINT, UINT, void**, DWORD)> _Lock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Unlock();
    NullPtr<void (Direct3DIndexBuffer9Hook::*)()> _Unlock_pre_ptr;
    NullPtr<void (Direct3DIndexBuffer9Hook::*)(HRESULT)> _Unlock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc(D3DINDEXBUFFER_DESC * pDesc);
    NullPtr<void (Direct3DIndexBuffer9Hook::*)(D3DINDEXBUFFER_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (Direct3DIndexBuffer9Hook::*)(HRESULT, D3DINDEXBUFFER_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// IDirect3DSurface9
// ==============================================================================
class Direct3DSurface9Hook : public HookBase<IDirect3DSurface9>
{
    Direct3DResource9Hook & _Direct3DResource9;

protected:

    Direct3DSurface9Hook(UnknownBase & unknown, Direct3DResource9Hook & Direct3DResource9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DResource9(Direct3DResource9)
    {
    }

    ~Direct3DSurface9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DResource9Hook * Direct3DResource9 = (Direct3DResource9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DResource9), realobj);
        if (nullptr == Direct3DResource9) return nullptr;

        try
        {
            return new Direct3DSurface9Hook(unknown, *Direct3DResource9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DSurface9Hook * typedPtr = (Direct3DSurface9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice)
    {
        return _Direct3DResource9.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
    {
        return _Direct3DResource9.SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
    {
        return _Direct3DResource9.GetPrivateData(refguid, pData, pSizeOfData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid)
    {
        return _Direct3DResource9.FreePrivateData(refguid);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew)
    {
        return _Direct3DResource9.SetPriority(PriorityNew);
    }

    // -----------------------------------------------------------------------------
    DWORD STDMETHODCALLTYPE GetPriority()
    {
        return _Direct3DResource9.GetPriority();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PreLoad()
    {
        return _Direct3DResource9.PreLoad();
    }

    // -----------------------------------------------------------------------------
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType()
    {
        return _Direct3DResource9.GetType();
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetContainer(REFIID riid, void** ppContainer);
    NullPtr<void (Direct3DSurface9Hook::*)(REFIID, void** &)> _GetContainer_pre_ptr;
    NullPtr<void (Direct3DSurface9Hook::*)(HRESULT, REFIID, void**)> _GetContainer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc(D3DSURFACE_DESC * pDesc);
    NullPtr<void (Direct3DSurface9Hook::*)(D3DSURFACE_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (Direct3DSurface9Hook::*)(HRESULT, D3DSURFACE_DESC *)> _GetDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
    NullPtr<void (Direct3DSurface9Hook::*)(D3DLOCKED_RECT* &, CONST RECT* &, DWORD &)> _LockRect_pre_ptr;
    NullPtr<void (Direct3DSurface9Hook::*)(HRESULT, D3DLOCKED_RECT*, CONST RECT*, DWORD)> _LockRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnlockRect();
    NullPtr<void (Direct3DSurface9Hook::*)()> _UnlockRect_pre_ptr;
    NullPtr<void (Direct3DSurface9Hook::*)(HRESULT)> _UnlockRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDC(HDC * phdc);
    NullPtr<void (Direct3DSurface9Hook::*)(HDC * &)> _GetDC_pre_ptr;
    NullPtr<void (Direct3DSurface9Hook::*)(HRESULT, HDC *)> _GetDC_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReleaseDC(HDC hdc);
    NullPtr<void (Direct3DSurface9Hook::*)(HDC &)> _ReleaseDC_pre_ptr;
    NullPtr<void (Direct3DSurface9Hook::*)(HRESULT, HDC)> _ReleaseDC_post_ptr;

};

// ==============================================================================
// IDirect3DVolume9
// ==============================================================================
class Direct3DVolume9Hook : public HookBase<IDirect3DVolume9>
{

protected:

    Direct3DVolume9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DVolume9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DVolume9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DVolume9Hook * typedPtr = (Direct3DVolume9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice);
    NullPtr<void (Direct3DVolume9Hook::*)(IDirect3DDevice9** &)> _GetDevice_pre_ptr;
    NullPtr<void (Direct3DVolume9Hook::*)(HRESULT, IDirect3DDevice9**)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
    NullPtr<void (Direct3DVolume9Hook::*)(REFGUID, CONST void* &, DWORD &, DWORD &)> _SetPrivateData_pre_ptr;
    NullPtr<void (Direct3DVolume9Hook::*)(HRESULT, REFGUID, CONST void*, DWORD, DWORD)> _SetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
    NullPtr<void (Direct3DVolume9Hook::*)(REFGUID, void* &, DWORD* &)> _GetPrivateData_pre_ptr;
    NullPtr<void (Direct3DVolume9Hook::*)(HRESULT, REFGUID, void*, DWORD*)> _GetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid);
    NullPtr<void (Direct3DVolume9Hook::*)(REFGUID)> _FreePrivateData_pre_ptr;
    NullPtr<void (Direct3DVolume9Hook::*)(HRESULT, REFGUID)> _FreePrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetContainer(REFIID riid, void** ppContainer);
    NullPtr<void (Direct3DVolume9Hook::*)(REFIID, void** &)> _GetContainer_pre_ptr;
    NullPtr<void (Direct3DVolume9Hook::*)(HRESULT, REFIID, void**)> _GetContainer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc(D3DVOLUME_DESC * pDesc);
    NullPtr<void (Direct3DVolume9Hook::*)(D3DVOLUME_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (Direct3DVolume9Hook::*)(HRESULT, D3DVOLUME_DESC *)> _GetDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE LockBox(D3DLOCKED_BOX * pLockedVolume, CONST D3DBOX* pBox, DWORD Flags);
    NullPtr<void (Direct3DVolume9Hook::*)(D3DLOCKED_BOX * &, CONST D3DBOX* &, DWORD &)> _LockBox_pre_ptr;
    NullPtr<void (Direct3DVolume9Hook::*)(HRESULT, D3DLOCKED_BOX *, CONST D3DBOX*, DWORD)> _LockBox_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnlockBox();
    NullPtr<void (Direct3DVolume9Hook::*)()> _UnlockBox_pre_ptr;
    NullPtr<void (Direct3DVolume9Hook::*)(HRESULT)> _UnlockBox_post_ptr;

};

// ==============================================================================
// IDirect3DQuery9
// ==============================================================================
class Direct3DQuery9Hook : public HookBase<IDirect3DQuery9>
{

protected:

    Direct3DQuery9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DQuery9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DQuery9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DQuery9Hook * typedPtr = (Direct3DQuery9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice);
    NullPtr<void (Direct3DQuery9Hook::*)(IDirect3DDevice9** &)> _GetDevice_pre_ptr;
    NullPtr<void (Direct3DQuery9Hook::*)(HRESULT, IDirect3DDevice9**)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D3DQUERYTYPE STDMETHODCALLTYPE GetType();
    NullPtr<void (Direct3DQuery9Hook::*)()> _GetType_pre_ptr;
    NullPtr<void (Direct3DQuery9Hook::*)(D3DQUERYTYPE)> _GetType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWORD STDMETHODCALLTYPE GetDataSize();
    NullPtr<void (Direct3DQuery9Hook::*)()> _GetDataSize_pre_ptr;
    NullPtr<void (Direct3DQuery9Hook::*)(DWORD)> _GetDataSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Issue(DWORD dwIssueFlags);
    NullPtr<void (Direct3DQuery9Hook::*)(DWORD &)> _Issue_pre_ptr;
    NullPtr<void (Direct3DQuery9Hook::*)(HRESULT, DWORD)> _Issue_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetData(void* pData, DWORD dwSize, DWORD dwGetDataFlags);
    NullPtr<void (Direct3DQuery9Hook::*)(void* &, DWORD &, DWORD &)> _GetData_pre_ptr;
    NullPtr<void (Direct3DQuery9Hook::*)(HRESULT, void*, DWORD, DWORD)> _GetData_post_ptr;

};

// ==============================================================================
// IDirect3D9Ex
// ==============================================================================
class Direct3D9ExHook : public HookBase<IDirect3D9Ex>
{
    Direct3D9Hook & _Direct3D9;

protected:

    Direct3D9ExHook(UnknownBase & unknown, Direct3D9Hook & Direct3D9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3D9(Direct3D9)
    {
    }

    ~Direct3D9ExHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3D9Hook * Direct3D9 = (Direct3D9Hook *)unknown.GetHookedParent(__uuidof(IDirect3D9), realobj);
        if (nullptr == Direct3D9) return nullptr;

        try
        {
            return new Direct3D9ExHook(unknown, *Direct3D9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3D9ExHook * typedPtr = (Direct3D9ExHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void* pInitializeFunction)
    {
        return _Direct3D9.RegisterSoftwareDevice(pInitializeFunction);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetAdapterCount()
    {
        return _Direct3D9.GetAdapterCount();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
    {
        return _Direct3D9.GetAdapterIdentifier(Adapter, Flags, pIdentifier);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
    {
        return _Direct3D9.GetAdapterModeCount(Adapter, Format);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
    {
        return _Direct3D9.EnumAdapterModes(Adapter, Format, Mode, pMode);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
    {
        return _Direct3D9.GetAdapterDisplayMode(Adapter, pMode);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
    {
        return _Direct3D9.CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
    {
        return _Direct3D9.CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
    {
        return _Direct3D9.CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
    {
        return _Direct3D9.CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
    {
        return _Direct3D9.CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
    {
        return _Direct3D9.GetDeviceCaps(Adapter, DeviceType, pCaps);
    }

    // -----------------------------------------------------------------------------
    HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter)
    {
        return _Direct3D9.GetAdapterMonitor(Adapter);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
    {
        return _Direct3D9.CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    }

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetAdapterModeCountEx(UINT Adapter, CONST D3DDISPLAYMODEFILTER* pFilter);
    NullPtr<void (Direct3D9ExHook::*)(UINT &, CONST D3DDISPLAYMODEFILTER* &)> _GetAdapterModeCountEx_pre_ptr;
    NullPtr<void (Direct3D9ExHook::*)(UINT, UINT, CONST D3DDISPLAYMODEFILTER*)> _GetAdapterModeCountEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EnumAdapterModesEx(UINT Adapter, CONST D3DDISPLAYMODEFILTER* pFilter, UINT Mode, D3DDISPLAYMODEEX* pMode);
    NullPtr<void (Direct3D9ExHook::*)(UINT &, CONST D3DDISPLAYMODEFILTER* &, UINT &, D3DDISPLAYMODEEX* &)> _EnumAdapterModesEx_pre_ptr;
    NullPtr<void (Direct3D9ExHook::*)(HRESULT, UINT, CONST D3DDISPLAYMODEFILTER*, UINT, D3DDISPLAYMODEEX*)> _EnumAdapterModesEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetAdapterDisplayModeEx(UINT Adapter, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation);
    NullPtr<void (Direct3D9ExHook::*)(UINT &, D3DDISPLAYMODEEX* &, D3DDISPLAYROTATION* &)> _GetAdapterDisplayModeEx_pre_ptr;
    NullPtr<void (Direct3D9ExHook::*)(HRESULT, UINT, D3DDISPLAYMODEEX*, D3DDISPLAYROTATION*)> _GetAdapterDisplayModeEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDeviceEx(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface);
    NullPtr<void (Direct3D9ExHook::*)(UINT &, D3DDEVTYPE &, HWND &, DWORD &, D3DPRESENT_PARAMETERS* &, D3DDISPLAYMODEEX* &, IDirect3DDevice9Ex** &)> _CreateDeviceEx_pre_ptr;
    NullPtr<void (Direct3D9ExHook::*)(HRESULT, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, D3DDISPLAYMODEEX*, IDirect3DDevice9Ex**)> _CreateDeviceEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetAdapterLUID(UINT Adapter, LUID * pLUID);
    NullPtr<void (Direct3D9ExHook::*)(UINT &, LUID * &)> _GetAdapterLUID_pre_ptr;
    NullPtr<void (Direct3D9ExHook::*)(HRESULT, UINT, LUID *)> _GetAdapterLUID_post_ptr;

};

// ==============================================================================
// IDirect3DDevice9Ex
// ==============================================================================
class Direct3DDevice9ExHook : public HookBase<IDirect3DDevice9Ex>
{
    Direct3DDevice9Hook & _Direct3DDevice9;

protected:

    Direct3DDevice9ExHook(UnknownBase & unknown, Direct3DDevice9Hook & Direct3DDevice9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DDevice9(Direct3DDevice9)
    {
    }

    ~Direct3DDevice9ExHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DDevice9Hook * Direct3DDevice9 = (Direct3DDevice9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DDevice9), realobj);
        if (nullptr == Direct3DDevice9) return nullptr;

        try
        {
            return new Direct3DDevice9ExHook(unknown, *Direct3DDevice9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DDevice9ExHook * typedPtr = (Direct3DDevice9ExHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE TestCooperativeLevel()
    {
        return _Direct3DDevice9.TestCooperativeLevel();
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetAvailableTextureMem()
    {
        return _Direct3DDevice9.GetAvailableTextureMem();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EvictManagedResources()
    {
        return _Direct3DDevice9.EvictManagedResources();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D9** ppD3D9)
    {
        return _Direct3DDevice9.GetDirect3D(ppD3D9);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS9* pCaps)
    {
        return _Direct3DDevice9.GetDeviceCaps(pCaps);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
    {
        return _Direct3DDevice9.GetDisplayMode(iSwapChain, pMode);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS * pParameters)
    {
        return _Direct3DDevice9.GetCreationParameters(pParameters);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
    {
        return _Direct3DDevice9.SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetCursorPosition(int X, int Y, DWORD Flags)
    {
        return _Direct3DDevice9.SetCursorPosition(X, Y, Flags);
    }

    // -----------------------------------------------------------------------------
    BOOL STDMETHODCALLTYPE ShowCursor(BOOL bShow)
    {
        return _Direct3DDevice9.ShowCursor(bShow);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** ppSwapChain)
    {
        return _Direct3DDevice9.CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** ppSwapChain)
    {
        return _Direct3DDevice9.GetSwapChain(iSwapChain, ppSwapChain);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetNumberOfSwapChains()
    {
        return _Direct3DDevice9.GetNumberOfSwapChains();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
    {
        return _Direct3DDevice9.Reset(pPresentationParameters);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
    {
        return _Direct3DDevice9.Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
    {
        return _Direct3DDevice9.GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
    {
        return _Direct3DDevice9.GetRasterStatus(iSwapChain, pRasterStatus);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetDialogBoxMode(BOOL bEnableDialogs)
    {
        return _Direct3DDevice9.SetDialogBoxMode(bEnableDialogs);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
    {
        return _Direct3DDevice9.SetGammaRamp(iSwapChain, Flags, pRamp);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
    {
        return _Direct3DDevice9.GetGammaRamp(iSwapChain, pRamp);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
    {
        return _Direct3DDevice9.CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
    {
        return _Direct3DDevice9.CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
    {
        return _Direct3DDevice9.CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
    {
        return _Direct3DDevice9.CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
    {
        return _Direct3DDevice9.CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
    {
        return _Direct3DDevice9.CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
    {
        return _Direct3DDevice9.CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
    {
        return _Direct3DDevice9.UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
    {
        return _Direct3DDevice9.UpdateTexture(pSourceTexture, pDestinationTexture);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
    {
        return _Direct3DDevice9.GetRenderTargetData(pRenderTarget, pDestSurface);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
    {
        return _Direct3DDevice9.GetFrontBufferData(iSwapChain, pDestSurface);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
    {
        return _Direct3DDevice9.StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
    {
        return _Direct3DDevice9.ColorFill(pSurface, pRect, color);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
    {
        return _Direct3DDevice9.CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
    {
        return _Direct3DDevice9.SetRenderTarget(RenderTargetIndex, pRenderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
    {
        return _Direct3DDevice9.GetRenderTarget(RenderTargetIndex, ppRenderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
    {
        return _Direct3DDevice9.SetDepthStencilSurface(pNewZStencil);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
    {
        return _Direct3DDevice9.GetDepthStencilSurface(ppZStencilSurface);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE BeginScene()
    {
        return _Direct3DDevice9.BeginScene();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EndScene()
    {
        return _Direct3DDevice9.EndScene();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
    {
        return _Direct3DDevice9.Clear(Count, pRects, Flags, Color, Z, Stencil);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
    {
        return _Direct3DDevice9.SetTransform(State, pMatrix);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
    {
        return _Direct3DDevice9.GetTransform(State, pMatrix);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE TransformType, CONST D3DMATRIX* Value)
    {
        return _Direct3DDevice9.MultiplyTransform(TransformType, Value);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetViewport(CONST D3DVIEWPORT9* pViewport)
    {
        return _Direct3DDevice9.SetViewport(pViewport);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT9* pViewport)
    {
        return _Direct3DDevice9.GetViewport(pViewport);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetMaterial(CONST D3DMATERIAL9* pMaterial)
    {
        return _Direct3DDevice9.SetMaterial(pMaterial);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL9* pMaterial)
    {
        return _Direct3DDevice9.GetMaterial(pMaterial);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
    {
        return _Direct3DDevice9.SetLight(Index, pLight);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetLight(DWORD Index, D3DLIGHT9* pLight)
    {
        return _Direct3DDevice9.GetLight(Index, pLight);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE LightEnable(DWORD Index, BOOL Enable)
    {
        return _Direct3DDevice9.LightEnable(Index, Enable);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD Index, BOOL* pEnable)
    {
        return _Direct3DDevice9.GetLightEnable(Index, pEnable);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD Index, CONST float* pPlane)
    {
        return _Direct3DDevice9.SetClipPlane(Index, pPlane);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD Index, float* pPlane)
    {
        return _Direct3DDevice9.GetClipPlane(Index, pPlane);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
    {
        return _Direct3DDevice9.SetRenderState(State, Value);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
    {
        return _Direct3DDevice9.GetRenderState(State, pValue);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
    {
        return _Direct3DDevice9.CreateStateBlock(Type, ppSB);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE BeginStateBlock()
    {
        return _Direct3DDevice9.BeginStateBlock();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9** ppSB)
    {
        return _Direct3DDevice9.EndStateBlock(ppSB);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
    {
        return _Direct3DDevice9.SetClipStatus(pClipStatus);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
    {
        return _Direct3DDevice9.GetClipStatus(pClipStatus);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
    {
        return _Direct3DDevice9.GetTexture(Stage, ppTexture);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
    {
        return _Direct3DDevice9.SetTexture(Stage, pTexture);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
    {
        return _Direct3DDevice9.GetTextureStageState(Stage, Type, pValue);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
    {
        return _Direct3DDevice9.SetTextureStageState(Stage, Type, Value);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
    {
        return _Direct3DDevice9.GetSamplerState(Sampler, Type, pValue);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
    {
        return _Direct3DDevice9.SetSamplerState(Sampler, Type, Value);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD* pNumPasses)
    {
        return _Direct3DDevice9.ValidateDevice(pNumPasses);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
    {
        return _Direct3DDevice9.SetPaletteEntries(PaletteNumber, pEntries);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
    {
        return _Direct3DDevice9.GetPaletteEntries(PaletteNumber, pEntries);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT PaletteNumber)
    {
        return _Direct3DDevice9.SetCurrentTexturePalette(PaletteNumber);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetCurrentTexturePalette(UINT * PaletteNumber)
    {
        return _Direct3DDevice9.GetCurrentTexturePalette(PaletteNumber);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetScissorRect(CONST RECT* pRect)
    {
        return _Direct3DDevice9.SetScissorRect(pRect);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetScissorRect(RECT* pRect)
    {
        return _Direct3DDevice9.GetScissorRect(pRect);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL bSoftware)
    {
        return _Direct3DDevice9.SetSoftwareVertexProcessing(bSoftware);
    }

    // -----------------------------------------------------------------------------
    BOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing()
    {
        return _Direct3DDevice9.GetSoftwareVertexProcessing();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetNPatchMode(float nSegments)
    {
        return _Direct3DDevice9.SetNPatchMode(nSegments);
    }

    // -----------------------------------------------------------------------------
    float STDMETHODCALLTYPE GetNPatchMode()
    {
        return _Direct3DDevice9.GetNPatchMode();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
    {
        return _Direct3DDevice9.DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
    {
        return _Direct3DDevice9.DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
    {
        return _Direct3DDevice9.DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
    {
        return _Direct3DDevice9.DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
    {
        return _Direct3DDevice9.ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
    {
        return _Direct3DDevice9.CreateVertexDeclaration(pVertexElements, ppDecl);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
    {
        return _Direct3DDevice9.SetVertexDeclaration(pDecl);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
    {
        return _Direct3DDevice9.GetVertexDeclaration(ppDecl);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetFVF(DWORD FVF)
    {
        return _Direct3DDevice9.SetFVF(FVF);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFVF(DWORD* pFVF)
    {
        return _Direct3DDevice9.GetFVF(pFVF);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
    {
        return _Direct3DDevice9.CreateVertexShader(pFunction, ppShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9* pShader)
    {
        return _Direct3DDevice9.SetVertexShader(pShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetVertexShader(IDirect3DVertexShader9** ppShader)
    {
        return _Direct3DDevice9.GetVertexShader(ppShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
    {
        return _Direct3DDevice9.SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
    {
        return _Direct3DDevice9.GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
    {
        return _Direct3DDevice9.SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
    {
        return _Direct3DDevice9.GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
    {
        return _Direct3DDevice9.SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
    {
        return _Direct3DDevice9.GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
    {
        return _Direct3DDevice9.SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
    {
        return _Direct3DDevice9.GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT StreamNumber, UINT Setting)
    {
        return _Direct3DDevice9.SetStreamSourceFreq(StreamNumber, Setting);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting)
    {
        return _Direct3DDevice9.GetStreamSourceFreq(StreamNumber, pSetting);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9* pIndexData)
    {
        return _Direct3DDevice9.SetIndices(pIndexData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer9** ppIndexData)
    {
        return _Direct3DDevice9.GetIndices(ppIndexData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
    {
        return _Direct3DDevice9.CreatePixelShader(pFunction, ppShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9* pShader)
    {
        return _Direct3DDevice9.SetPixelShader(pShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPixelShader(IDirect3DPixelShader9** ppShader)
    {
        return _Direct3DDevice9.GetPixelShader(ppShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
    {
        return _Direct3DDevice9.SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
    {
        return _Direct3DDevice9.GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
    {
        return _Direct3DDevice9.SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
    {
        return _Direct3DDevice9.GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
    {
        return _Direct3DDevice9.SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
    {
        return _Direct3DDevice9.GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
    {
        return _Direct3DDevice9.DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
    {
        return _Direct3DDevice9.DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE DeletePatch(UINT Handle)
    {
        return _Direct3DDevice9.DeletePatch(Handle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
    {
        return _Direct3DDevice9.CreateQuery(Type, ppQuery);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetConvolutionMonoKernel(UINT width, UINT height, float* rows, float* columns);
    NullPtr<void (Direct3DDevice9ExHook::*)(UINT &, UINT &, float* &, float* &)> _SetConvolutionMonoKernel_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, UINT, UINT, float*, float*)> _SetConvolutionMonoKernel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ComposeRects(IDirect3DSurface9* pSrc, IDirect3DSurface9* pDst, IDirect3DVertexBuffer9* pSrcRectDescs, UINT NumRects, IDirect3DVertexBuffer9* pDstRectDescs, D3DCOMPOSERECTSOP Operation, int Xoffset, int Yoffset);
    NullPtr<void (Direct3DDevice9ExHook::*)(IDirect3DSurface9* &, IDirect3DSurface9* &, IDirect3DVertexBuffer9* &, UINT &, IDirect3DVertexBuffer9* &, D3DCOMPOSERECTSOP &, int &, int &)> _ComposeRects_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, IDirect3DSurface9*, IDirect3DSurface9*, IDirect3DVertexBuffer9*, UINT, IDirect3DVertexBuffer9*, D3DCOMPOSERECTSOP, int, int)> _ComposeRects_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PresentEx(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
    NullPtr<void (Direct3DDevice9ExHook::*)(CONST RECT* &, CONST RECT* &, HWND &, CONST RGNDATA* &, DWORD &)> _PresentEx_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*, DWORD)> _PresentEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGPUThreadPriority(INT* pPriority);
    NullPtr<void (Direct3DDevice9ExHook::*)(INT* &)> _GetGPUThreadPriority_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, INT*)> _GetGPUThreadPriority_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetGPUThreadPriority(INT Priority);
    NullPtr<void (Direct3DDevice9ExHook::*)(INT &)> _SetGPUThreadPriority_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, INT)> _SetGPUThreadPriority_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE WaitForVBlank(UINT iSwapChain);
    NullPtr<void (Direct3DDevice9ExHook::*)(UINT &)> _WaitForVBlank_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, UINT)> _WaitForVBlank_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckResourceResidency(IDirect3DResource9** pResourceArray, UINT32 NumResources);
    NullPtr<void (Direct3DDevice9ExHook::*)(IDirect3DResource9** &, UINT32 &)> _CheckResourceResidency_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, IDirect3DResource9**, UINT32)> _CheckResourceResidency_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetMaximumFrameLatency(UINT MaxLatency);
    NullPtr<void (Direct3DDevice9ExHook::*)(UINT &)> _SetMaximumFrameLatency_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, UINT)> _SetMaximumFrameLatency_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetMaximumFrameLatency(UINT* pMaxLatency);
    NullPtr<void (Direct3DDevice9ExHook::*)(UINT* &)> _GetMaximumFrameLatency_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, UINT*)> _GetMaximumFrameLatency_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceState(HWND hDestinationWindow);
    NullPtr<void (Direct3DDevice9ExHook::*)(HWND &)> _CheckDeviceState_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, HWND)> _CheckDeviceState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetEx(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage);
    NullPtr<void (Direct3DDevice9ExHook::*)(UINT &, UINT &, D3DFORMAT &, D3DMULTISAMPLE_TYPE &, DWORD &, BOOL &, IDirect3DSurface9** &, HANDLE* &, DWORD &)> _CreateRenderTargetEx_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface9**, HANDLE*, DWORD)> _CreateRenderTargetEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurfaceEx(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage);
    NullPtr<void (Direct3DDevice9ExHook::*)(UINT &, UINT &, D3DFORMAT &, D3DPOOL &, IDirect3DSurface9** &, HANDLE* &, DWORD &)> _CreateOffscreenPlainSurfaceEx_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, UINT, UINT, D3DFORMAT, D3DPOOL, IDirect3DSurface9**, HANDLE*, DWORD)> _CreateOffscreenPlainSurfaceEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilSurfaceEx(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage);
    NullPtr<void (Direct3DDevice9ExHook::*)(UINT &, UINT &, D3DFORMAT &, D3DMULTISAMPLE_TYPE &, DWORD &, BOOL &, IDirect3DSurface9** &, HANDLE* &, DWORD &)> _CreateDepthStencilSurfaceEx_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, DWORD, BOOL, IDirect3DSurface9**, HANDLE*, DWORD)> _CreateDepthStencilSurfaceEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ResetEx(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX * pFullscreenDisplayMode);
    NullPtr<void (Direct3DDevice9ExHook::*)(D3DPRESENT_PARAMETERS* &, D3DDISPLAYMODEEX * &)> _ResetEx_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, D3DPRESENT_PARAMETERS*, D3DDISPLAYMODEEX *)> _ResetEx_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDisplayModeEx(UINT iSwapChain, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation);
    NullPtr<void (Direct3DDevice9ExHook::*)(UINT &, D3DDISPLAYMODEEX* &, D3DDISPLAYROTATION* &)> _GetDisplayModeEx_pre_ptr;
    NullPtr<void (Direct3DDevice9ExHook::*)(HRESULT, UINT, D3DDISPLAYMODEEX*, D3DDISPLAYROTATION*)> _GetDisplayModeEx_post_ptr;

};

// ==============================================================================
// IDirect3DSwapChain9Ex
// ==============================================================================
class Direct3DSwapChain9ExHook : public HookBase<IDirect3DSwapChain9Ex>
{
    Direct3DSwapChain9Hook & _Direct3DSwapChain9;

protected:

    Direct3DSwapChain9ExHook(UnknownBase & unknown, Direct3DSwapChain9Hook & Direct3DSwapChain9, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _Direct3DSwapChain9(Direct3DSwapChain9)
    {
    }

    ~Direct3DSwapChain9ExHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DSwapChain9Hook * Direct3DSwapChain9 = (Direct3DSwapChain9Hook *)unknown.GetHookedParent(__uuidof(IDirect3DSwapChain9), realobj);
        if (nullptr == Direct3DSwapChain9) return nullptr;

        try
        {
            return new Direct3DSwapChain9ExHook(unknown, *Direct3DSwapChain9, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DSwapChain9ExHook * typedPtr = (Direct3DSwapChain9ExHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
    {
        return _Direct3DSwapChain9.Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFrontBufferData(IDirect3DSurface9* pDestSurface)
    {
        return _Direct3DSwapChain9.GetFrontBufferData(pDestSurface);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
    {
        return _Direct3DSwapChain9.GetBackBuffer(iBackBuffer, Type, ppBackBuffer);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
    {
        return _Direct3DSwapChain9.GetRasterStatus(pRasterStatus);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE* pMode)
    {
        return _Direct3DSwapChain9.GetDisplayMode(pMode);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** ppDevice)
    {
        return _Direct3DSwapChain9.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
    {
        return _Direct3DSwapChain9.GetPresentParameters(pPresentationParameters);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(UINT* pLastPresentCount);
    NullPtr<void (Direct3DSwapChain9ExHook::*)(UINT* &)> _GetLastPresentCount_pre_ptr;
    NullPtr<void (Direct3DSwapChain9ExHook::*)(HRESULT, UINT*)> _GetLastPresentCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPresentStats(D3DPRESENTSTATS* pPresentationStatistics);
    NullPtr<void (Direct3DSwapChain9ExHook::*)(D3DPRESENTSTATS* &)> _GetPresentStats_pre_ptr;
    NullPtr<void (Direct3DSwapChain9ExHook::*)(HRESULT, D3DPRESENTSTATS*)> _GetPresentStats_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDisplayModeEx(D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation);
    NullPtr<void (Direct3DSwapChain9ExHook::*)(D3DDISPLAYMODEEX* &, D3DDISPLAYROTATION* &)> _GetDisplayModeEx_pre_ptr;
    NullPtr<void (Direct3DSwapChain9ExHook::*)(HRESULT, D3DDISPLAYMODEEX*, D3DDISPLAYROTATION*)> _GetDisplayModeEx_post_ptr;

};

// ==============================================================================
// IDirect3D9ExOverlayExtension
// ==============================================================================
class Direct3D9ExOverlayExtensionHook : public HookBase<IDirect3D9ExOverlayExtension>
{

protected:

    Direct3D9ExOverlayExtensionHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3D9ExOverlayExtensionHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3D9ExOverlayExtensionHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3D9ExOverlayExtensionHook * typedPtr = (Direct3D9ExOverlayExtensionHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckDeviceOverlayType(UINT Adapter, D3DDEVTYPE DevType, UINT OverlayWidth, UINT OverlayHeight, D3DFORMAT OverlayFormat, D3DDISPLAYMODEEX* pDisplayMode, D3DDISPLAYROTATION DisplayRotation, D3DOVERLAYCAPS* pOverlayCaps);
    NullPtr<void (Direct3D9ExOverlayExtensionHook::*)(UINT &, D3DDEVTYPE &, UINT &, UINT &, D3DFORMAT &, D3DDISPLAYMODEEX* &, D3DDISPLAYROTATION &, D3DOVERLAYCAPS* &)> _CheckDeviceOverlayType_pre_ptr;
    NullPtr<void (Direct3D9ExOverlayExtensionHook::*)(HRESULT, UINT, D3DDEVTYPE, UINT, UINT, D3DFORMAT, D3DDISPLAYMODEEX*, D3DDISPLAYROTATION, D3DOVERLAYCAPS*)> _CheckDeviceOverlayType_post_ptr;

};

// ==============================================================================
// IDirect3DDevice9Video
// ==============================================================================
class Direct3DDevice9VideoHook : public HookBase<IDirect3DDevice9Video>
{

protected:

    Direct3DDevice9VideoHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DDevice9VideoHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DDevice9VideoHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DDevice9VideoHook * typedPtr = (Direct3DDevice9VideoHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetContentProtectionCaps(CONST GUID* pCryptoType, CONST GUID* pDecodeProfile, D3DCONTENTPROTECTIONCAPS* pCaps);
    NullPtr<void (Direct3DDevice9VideoHook::*)(CONST GUID* &, CONST GUID* &, D3DCONTENTPROTECTIONCAPS* &)> _GetContentProtectionCaps_pre_ptr;
    NullPtr<void (Direct3DDevice9VideoHook::*)(HRESULT, CONST GUID*, CONST GUID*, D3DCONTENTPROTECTIONCAPS*)> _GetContentProtectionCaps_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateAuthenticatedChannel(D3DAUTHENTICATEDCHANNELTYPE ChannelType, IDirect3DAuthenticatedChannel9** ppAuthenticatedChannel, HANDLE* pChannelHandle);
    NullPtr<void (Direct3DDevice9VideoHook::*)(D3DAUTHENTICATEDCHANNELTYPE &, IDirect3DAuthenticatedChannel9** &, HANDLE* &)> _CreateAuthenticatedChannel_pre_ptr;
    NullPtr<void (Direct3DDevice9VideoHook::*)(HRESULT, D3DAUTHENTICATEDCHANNELTYPE, IDirect3DAuthenticatedChannel9**, HANDLE*)> _CreateAuthenticatedChannel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCryptoSession(CONST GUID* pCryptoType, CONST GUID* pDecodeProfile, IDirect3DCryptoSession9** ppCryptoSession, HANDLE* pCryptoHandle);
    NullPtr<void (Direct3DDevice9VideoHook::*)(CONST GUID* &, CONST GUID* &, IDirect3DCryptoSession9** &, HANDLE* &)> _CreateCryptoSession_pre_ptr;
    NullPtr<void (Direct3DDevice9VideoHook::*)(HRESULT, CONST GUID*, CONST GUID*, IDirect3DCryptoSession9**, HANDLE*)> _CreateCryptoSession_post_ptr;

};

// ==============================================================================
// IDirect3DAuthenticatedChannel9
// ==============================================================================
class Direct3DAuthenticatedChannel9Hook : public HookBase<IDirect3DAuthenticatedChannel9>
{

protected:

    Direct3DAuthenticatedChannel9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DAuthenticatedChannel9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DAuthenticatedChannel9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DAuthenticatedChannel9Hook * typedPtr = (Direct3DAuthenticatedChannel9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCertificateSize(UINT* pCertificateSize);
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(UINT* &)> _GetCertificateSize_pre_ptr;
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(HRESULT, UINT*)> _GetCertificateSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCertificate(UINT CertifacteSize, BYTE* ppCertificate);
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(UINT &, BYTE* &)> _GetCertificate_pre_ptr;
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(HRESULT, UINT, BYTE*)> _GetCertificate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE NegotiateKeyExchange(UINT DataSize, VOID* pData);
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(UINT &, VOID* &)> _NegotiateKeyExchange_pre_ptr;
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(HRESULT, UINT, VOID*)> _NegotiateKeyExchange_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Query(UINT InputSize, CONST VOID* pInput, UINT OutputSize, VOID* pOutput);
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(UINT &, CONST VOID* &, UINT &, VOID* &)> _Query_pre_ptr;
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(HRESULT, UINT, CONST VOID*, UINT, VOID*)> _Query_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Configure(UINT InputSize, CONST VOID* pInput, D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT* pOutput);
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(UINT &, CONST VOID* &, D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT* &)> _Configure_pre_ptr;
    NullPtr<void (Direct3DAuthenticatedChannel9Hook::*)(HRESULT, UINT, CONST VOID*, D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT*)> _Configure_post_ptr;

};

// ==============================================================================
// IDirect3DCryptoSession9
// ==============================================================================
class Direct3DCryptoSession9Hook : public HookBase<IDirect3DCryptoSession9>
{

protected:

    Direct3DCryptoSession9Hook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~Direct3DCryptoSession9Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new Direct3DCryptoSession9Hook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        Direct3DCryptoSession9Hook * typedPtr = (Direct3DCryptoSession9Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCertificateSize(UINT* pCertificateSize);
    NullPtr<void (Direct3DCryptoSession9Hook::*)(UINT* &)> _GetCertificateSize_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT, UINT*)> _GetCertificateSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCertificate(UINT CertifacteSize, BYTE* ppCertificate);
    NullPtr<void (Direct3DCryptoSession9Hook::*)(UINT &, BYTE* &)> _GetCertificate_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT, UINT, BYTE*)> _GetCertificate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE NegotiateKeyExchange(UINT DataSize, VOID* pData);
    NullPtr<void (Direct3DCryptoSession9Hook::*)(UINT &, VOID* &)> _NegotiateKeyExchange_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT, UINT, VOID*)> _NegotiateKeyExchange_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EncryptionBlt(IDirect3DSurface9* pSrcSurface, IDirect3DSurface9* pDstSurface, UINT DstSurfaceSize, VOID* pIV);
    NullPtr<void (Direct3DCryptoSession9Hook::*)(IDirect3DSurface9* &, IDirect3DSurface9* &, UINT &, VOID* &)> _EncryptionBlt_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT, IDirect3DSurface9*, IDirect3DSurface9*, UINT, VOID*)> _EncryptionBlt_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DecryptionBlt(IDirect3DSurface9* pSrcSurface, IDirect3DSurface9* pDstSurface, UINT SrcSurfaceSize, D3DENCRYPTED_BLOCK_INFO* pEncryptedBlockInfo, VOID* pContentKey, VOID* pIV);
    NullPtr<void (Direct3DCryptoSession9Hook::*)(IDirect3DSurface9* &, IDirect3DSurface9* &, UINT &, D3DENCRYPTED_BLOCK_INFO* &, VOID* &, VOID* &)> _DecryptionBlt_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT, IDirect3DSurface9*, IDirect3DSurface9*, UINT, D3DENCRYPTED_BLOCK_INFO*, VOID*, VOID*)> _DecryptionBlt_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSurfacePitch(IDirect3DSurface9* pSrcSurface, UINT* pSurfacePitch);
    NullPtr<void (Direct3DCryptoSession9Hook::*)(IDirect3DSurface9* &, UINT* &)> _GetSurfacePitch_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT, IDirect3DSurface9*, UINT*)> _GetSurfacePitch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE StartSessionKeyRefresh(VOID* pRandomNumber, UINT RandomNumberSize);
    NullPtr<void (Direct3DCryptoSession9Hook::*)(VOID* &, UINT &)> _StartSessionKeyRefresh_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT, VOID*, UINT)> _StartSessionKeyRefresh_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FinishSessionKeyRefresh();
    NullPtr<void (Direct3DCryptoSession9Hook::*)()> _FinishSessionKeyRefresh_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT)> _FinishSessionKeyRefresh_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetEncryptionBltKey(VOID* pReadbackKey, UINT KeySize);
    NullPtr<void (Direct3DCryptoSession9Hook::*)(VOID* &, UINT &)> _GetEncryptionBltKey_pre_ptr;
    NullPtr<void (Direct3DCryptoSession9Hook::*)(HRESULT, VOID*, UINT)> _GetEncryptionBltKey_post_ptr;

};


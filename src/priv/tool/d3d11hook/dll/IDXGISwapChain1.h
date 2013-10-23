// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;
DXGISwapChainHook & _DXGISwapChain;

protected:

DXGISwapChain1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, DXGISwapChainHook & DXGISwapChain, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIDeviceSubObject(DXGIDeviceSubObject)
    , _DXGISwapChain(DXGISwapChain)
{
    Construct(); 
}

~DXGISwapChain1Hook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
    if (nullptr == DXGIObject) return nullptr;

    DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
    if (nullptr == DXGIDeviceSubObject) return nullptr;

    DXGISwapChainHook * DXGISwapChain = (DXGISwapChainHook *)unknown.GetHookedObj(__uuidof(IDXGISwapChain));
    if (nullptr == DXGISwapChain) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGISwapChain1Hook(unknown, *DXGIObject, *DXGIDeviceSubObject, *DXGISwapChain, realobj);
        result->AddRef();
        return result;
    }
    catch(std::bad_alloc&)
    {
        GN_ERROR(GN::getLogger("GN.d3d11hook"))("Out of memory.");
        return nullptr;
    }
}

// ==============================================================================
// Calling to base interfaces
// ==============================================================================
public:

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID Name,
    /* [in] */ UINT DataSize,
    _In_reads_bytes_(DataSize)  const void * pData)
{
    return _DXGIObject.SetPrivateData(Name, DataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID Name,
    _In_  const IUnknown * pUnknown)
{
    return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID Name,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_(*pDataSize)  void * pData)
{
    return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetParent(
    _In_  REFIID riid,
    _Out_  void ** ppParent)
{
    return _DXGIObject.GetParent(riid, ppParent);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetDevice(
    _In_  REFIID riid,
    _Out_  void ** ppDevice)
{
    return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Present(
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT Flags)
{
    return _DXGISwapChain.Present(SyncInterval, Flags);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetBuffer(
    /* [in] */ UINT Buffer,
    _In_  REFIID riid,
    _Out_  void ** ppSurface)
{
    return _DXGISwapChain.GetBuffer(Buffer, riid, ppSurface);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetFullscreenState(
    /* [in] */ BOOL Fullscreen,
    _In_opt_  IDXGIOutput * pTarget)
{
    return _DXGISwapChain.SetFullscreenState(Fullscreen, pTarget);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetFullscreenState(
    _Out_opt_  BOOL * pFullscreen,
    _Out_opt_  IDXGIOutput ** ppTarget)
{
    return _DXGISwapChain.GetFullscreenState(pFullscreen, ppTarget);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_SWAP_CHAIN_DESC * pDesc)
{
    return _DXGISwapChain.GetDesc(pDesc);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE ResizeBuffers(
    /* [in] */ UINT BufferCount,
    /* [in] */ UINT Width,
    /* [in] */ UINT Height,
    /* [in] */ DXGI_FORMAT NewFormat,
    /* [in] */ UINT SwapChainFlags)
{
    return _DXGISwapChain.ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE ResizeTarget(
    _In_  const DXGI_MODE_DESC * pNewTargetParameters)
{
    return _DXGISwapChain.ResizeTarget(pNewTargetParameters);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetContainingOutput(
    _Out_  IDXGIOutput ** ppOutput)
{
    return _DXGISwapChain.GetContainingOutput(ppOutput);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats)
{
    return _DXGISwapChain.GetFrameStatistics(pStats);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetLastPresentCount(
    _Out_  UINT * pLastPresentCount)
{
    return _DXGISwapChain.GetLastPresentCount(pLastPresentCount);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc1(
    _Out_  DXGI_SWAP_CHAIN_DESC1 * pDesc);
NullPtr<void (DXGISwapChain1Hook::*)(_Out_  DXGI_SWAP_CHAIN_DESC1 * &)> _GetDesc1_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  DXGI_SWAP_CHAIN_DESC1 *)> _GetDesc1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFullscreenDesc(
    _Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pDesc);
NullPtr<void (DXGISwapChain1Hook::*)(_Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC * &)> _GetFullscreenDesc_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC *)> _GetFullscreenDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetHwnd(
    _Out_  HWND * pHwnd);
NullPtr<void (DXGISwapChain1Hook::*)(_Out_  HWND * &)> _GetHwnd_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  HWND *)> _GetHwnd_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetCoreWindow(
    _In_  REFIID refiid,
    _Out_  void ** ppUnk);
NullPtr<void (DXGISwapChain1Hook::*)(_In_  REFIID, _Out_  void ** &)> _GetCoreWindow_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _In_  REFIID, _Out_  void **)> _GetCoreWindow_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE Present1(
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT PresentFlags,
    _In_  const DXGI_PRESENT_PARAMETERS * pPresentParameters);
NullPtr<void (DXGISwapChain1Hook::*)(/* [in] */ UINT &, /* [in] */ UINT &, _In_  const DXGI_PRESENT_PARAMETERS * &)> _Present1_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, /* [in] */ UINT, /* [in] */ UINT, _In_  const DXGI_PRESENT_PARAMETERS *)> _Present1_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE IsTemporaryMonoSupported();
NullPtr<void (DXGISwapChain1Hook::*)()> _IsTemporaryMonoSupported_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(BOOL)> _IsTemporaryMonoSupported_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetRestrictToOutput(
    _Out_  IDXGIOutput ** ppRestrictToOutput);
NullPtr<void (DXGISwapChain1Hook::*)(_Out_  IDXGIOutput ** &)> _GetRestrictToOutput_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  IDXGIOutput **)> _GetRestrictToOutput_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetBackgroundColor(
    _In_  const DXGI_RGBA * pColor);
NullPtr<void (DXGISwapChain1Hook::*)(_In_  const DXGI_RGBA * &)> _SetBackgroundColor_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _In_  const DXGI_RGBA *)> _SetBackgroundColor_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetBackgroundColor(
    _Out_  DXGI_RGBA * pColor);
NullPtr<void (DXGISwapChain1Hook::*)(_Out_  DXGI_RGBA * &)> _GetBackgroundColor_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  DXGI_RGBA *)> _GetBackgroundColor_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetRotation(
    _In_  DXGI_MODE_ROTATION Rotation);
NullPtr<void (DXGISwapChain1Hook::*)(_In_  DXGI_MODE_ROTATION &)> _SetRotation_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _In_  DXGI_MODE_ROTATION)> _SetRotation_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetRotation(
    _Out_  DXGI_MODE_ROTATION * pRotation);
NullPtr<void (DXGISwapChain1Hook::*)(_Out_  DXGI_MODE_ROTATION * &)> _GetRotation_pre_ptr;
NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  DXGI_MODE_ROTATION *)> _GetRotation_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
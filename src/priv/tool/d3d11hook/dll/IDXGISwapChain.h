// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;

protected:

DXGISwapChainHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIDeviceSubObject(DXGIDeviceSubObject)
{
    Construct(); 
}

~DXGISwapChainHook() {}

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

    try
    {
        IUnknown * result = (UnknownBase*)new DXGISwapChainHook(unknown, *DXGIObject, *DXGIDeviceSubObject, realobj);
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
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE Present(
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT Flags);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, /* [in] */ UINT &)> _Present_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, /* [in] */ UINT)> _Present_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetBuffer(
    /* [in] */ UINT Buffer,
    _In_  REFIID riid,
    _Out_  void ** ppSurface);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, _In_  REFIID, _Out_  void ** &)> _GetBuffer_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, _In_  REFIID, _Out_  void **)> _GetBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetFullscreenState(
    /* [in] */ BOOL Fullscreen,
    _In_opt_  IDXGIOutput * pTarget);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ BOOL &, _In_opt_  IDXGIOutput * &)> _SetFullscreenState_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ BOOL, _In_opt_  IDXGIOutput *)> _SetFullscreenState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(
    _Out_opt_  BOOL * pFullscreen,
    _Out_opt_  IDXGIOutput ** ppTarget);
NullPtr<void (DXGISwapChainHook::*)(_Out_opt_  BOOL * &, _Out_opt_  IDXGIOutput ** &)> _GetFullscreenState_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_opt_  BOOL *, _Out_opt_  IDXGIOutput **)> _GetFullscreenState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_SWAP_CHAIN_DESC * pDesc);
NullPtr<void (DXGISwapChainHook::*)(_Out_  DXGI_SWAP_CHAIN_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  DXGI_SWAP_CHAIN_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ResizeBuffers(
    /* [in] */ UINT BufferCount,
    /* [in] */ UINT Width,
    /* [in] */ UINT Height,
    /* [in] */ DXGI_FORMAT NewFormat,
    /* [in] */ UINT SwapChainFlags);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, /* [in] */ UINT &, /* [in] */ UINT &, /* [in] */ DXGI_FORMAT &, /* [in] */ UINT &)> _ResizeBuffers_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, /* [in] */ UINT, /* [in] */ UINT, /* [in] */ DXGI_FORMAT, /* [in] */ UINT)> _ResizeBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ResizeTarget(
    _In_  const DXGI_MODE_DESC * pNewTargetParameters);
NullPtr<void (DXGISwapChainHook::*)(_In_  const DXGI_MODE_DESC * &)> _ResizeTarget_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _In_  const DXGI_MODE_DESC *)> _ResizeTarget_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(
    _Out_  IDXGIOutput ** ppOutput);
NullPtr<void (DXGISwapChainHook::*)(_Out_  IDXGIOutput ** &)> _GetContainingOutput_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  IDXGIOutput **)> _GetContainingOutput_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats);
NullPtr<void (DXGISwapChainHook::*)(_Out_  DXGI_FRAME_STATISTICS * &)> _GetFrameStatistics_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  DXGI_FRAME_STATISTICS *)> _GetFrameStatistics_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(
    _Out_  UINT * pLastPresentCount);
NullPtr<void (DXGISwapChainHook::*)(_Out_  UINT * &)> _GetLastPresentCount_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  UINT *)> _GetLastPresentCount_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
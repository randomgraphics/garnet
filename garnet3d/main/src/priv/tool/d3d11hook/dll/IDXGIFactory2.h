// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIFactoryHook & _DXGIFactory;
DXGIFactory1Hook & _DXGIFactory1;

protected:

DXGIFactory2Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIFactoryHook & DXGIFactory, DXGIFactory1Hook & DXGIFactory1, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIFactory(DXGIFactory)
    , _DXGIFactory1(DXGIFactory1)
{
    Construct(); 
}

~DXGIFactory2Hook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
    if (nullptr == DXGIObject) return nullptr;

    DXGIFactoryHook * DXGIFactory = (DXGIFactoryHook *)unknown.GetHookedObj(__uuidof(IDXGIFactory));
    if (nullptr == DXGIFactory) return nullptr;

    DXGIFactory1Hook * DXGIFactory1 = (DXGIFactory1Hook *)unknown.GetHookedObj(__uuidof(IDXGIFactory1));
    if (nullptr == DXGIFactory1) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIFactory2Hook(unknown, *DXGIObject, *DXGIFactory, *DXGIFactory1, realobj);
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
HRESULT STDMETHODCALLTYPE EnumAdapters(
    /* [in] */ UINT Adapter,
    _Out_  IDXGIAdapter ** ppAdapter)
{
    return _DXGIFactory.EnumAdapters(Adapter, ppAdapter);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE MakeWindowAssociation(
    HWND WindowHandle,
    UINT Flags)
{
    return _DXGIFactory.MakeWindowAssociation(WindowHandle, Flags);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetWindowAssociation(
    _Out_  HWND * pWindowHandle)
{
    return _DXGIFactory.GetWindowAssociation(pWindowHandle);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateSwapChain(
    _In_  IUnknown * pDevice,
    _In_  DXGI_SWAP_CHAIN_DESC * pDesc,
    _Out_  IDXGISwapChain ** ppSwapChain)
{
    return _DXGIFactory.CreateSwapChain(pDevice, pDesc, ppSwapChain);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateSoftwareAdapter(
    /* [in] */ HMODULE Module,
    _Out_  IDXGIAdapter ** ppAdapter)
{
    return _DXGIFactory.CreateSoftwareAdapter(Module, ppAdapter);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE EnumAdapters1(
    /* [in] */ UINT Adapter,
    _Out_  IDXGIAdapter1 ** ppAdapter)
{
    return _DXGIFactory1.EnumAdapters1(Adapter, ppAdapter);
}
// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE IsCurrent()
{
    return _DXGIFactory1.IsCurrent();
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE IsWindowedStereoEnabled();
NullPtr<void (DXGIFactory2Hook::*)()> _IsWindowedStereoEnabled_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(BOOL)> _IsWindowedStereoEnabled_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSwapChainForHwnd(
    _In_  IUnknown * pDevice,
    _In_  HWND hWnd,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pFullscreenDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Out_  IDXGISwapChain1 ** ppSwapChain);
NullPtr<void (DXGIFactory2Hook::*)(_In_  IUnknown * &, _In_  HWND &, _In_  const DXGI_SWAP_CHAIN_DESC1 * &, _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * &, _In_opt_  IDXGIOutput * &, _Out_  IDXGISwapChain1 ** &)> _CreateSwapChainForHwnd_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  IUnknown *, _In_  HWND, _In_  const DXGI_SWAP_CHAIN_DESC1 *, _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *, _In_opt_  IDXGIOutput *, _Out_  IDXGISwapChain1 **)> _CreateSwapChainForHwnd_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSwapChainForCoreWindow(
    _In_  IUnknown * pDevice,
    _In_  IUnknown * pWindow,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Out_  IDXGISwapChain1 ** ppSwapChain);
NullPtr<void (DXGIFactory2Hook::*)(_In_  IUnknown * &, _In_  IUnknown * &, _In_  const DXGI_SWAP_CHAIN_DESC1 * &, _In_opt_  IDXGIOutput * &, _Out_  IDXGISwapChain1 ** &)> _CreateSwapChainForCoreWindow_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  IUnknown *, _In_  IUnknown *, _In_  const DXGI_SWAP_CHAIN_DESC1 *, _In_opt_  IDXGIOutput *, _Out_  IDXGISwapChain1 **)> _CreateSwapChainForCoreWindow_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetSharedResourceAdapterLuid(
    _In_  HANDLE hResource,
    _Out_  LUID * pLuid);
NullPtr<void (DXGIFactory2Hook::*)(_In_  HANDLE &, _Out_  LUID * &)> _GetSharedResourceAdapterLuid_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HANDLE, _Out_  LUID *)> _GetSharedResourceAdapterLuid_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE RegisterStereoStatusWindow(
    _In_  HWND WindowHandle,
    _In_  UINT wMsg,
    _Out_  DWORD * pdwCookie);
NullPtr<void (DXGIFactory2Hook::*)(_In_  HWND &, _In_  UINT &, _Out_  DWORD * &)> _RegisterStereoStatusWindow_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HWND, _In_  UINT, _Out_  DWORD *)> _RegisterStereoStatusWindow_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE RegisterStereoStatusEvent(
    _In_  HANDLE hEvent,
    _Out_  DWORD * pdwCookie);
NullPtr<void (DXGIFactory2Hook::*)(_In_  HANDLE &, _Out_  DWORD * &)> _RegisterStereoStatusEvent_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HANDLE, _Out_  DWORD *)> _RegisterStereoStatusEvent_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE UnregisterStereoStatus(
    _In_  DWORD dwCookie);
NullPtr<void (DXGIFactory2Hook::*)(_In_  DWORD &)> _UnregisterStereoStatus_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(_In_  DWORD)> _UnregisterStereoStatus_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE RegisterOcclusionStatusWindow(
    _In_  HWND WindowHandle,
    _In_  UINT wMsg,
    _Out_  DWORD * pdwCookie);
NullPtr<void (DXGIFactory2Hook::*)(_In_  HWND &, _In_  UINT &, _Out_  DWORD * &)> _RegisterOcclusionStatusWindow_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HWND, _In_  UINT, _Out_  DWORD *)> _RegisterOcclusionStatusWindow_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE RegisterOcclusionStatusEvent(
    _In_  HANDLE hEvent,
    _Out_  DWORD * pdwCookie);
NullPtr<void (DXGIFactory2Hook::*)(_In_  HANDLE &, _Out_  DWORD * &)> _RegisterOcclusionStatusEvent_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HANDLE, _Out_  DWORD *)> _RegisterOcclusionStatusEvent_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE UnregisterOcclusionStatus(
    _In_  DWORD dwCookie);
NullPtr<void (DXGIFactory2Hook::*)(_In_  DWORD &)> _UnregisterOcclusionStatus_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(_In_  DWORD)> _UnregisterOcclusionStatus_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSwapChainForComposition(
    _In_  IUnknown * pDevice,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Outptr_  IDXGISwapChain1 ** ppSwapChain);
NullPtr<void (DXGIFactory2Hook::*)(_In_  IUnknown * &, _In_  const DXGI_SWAP_CHAIN_DESC1 * &, _In_opt_  IDXGIOutput * &, _Outptr_  IDXGISwapChain1 ** &)> _CreateSwapChainForComposition_pre_ptr;
NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  IUnknown *, _In_  const DXGI_SWAP_CHAIN_DESC1 *, _In_opt_  IDXGIOutput *, _Outptr_  IDXGISwapChain1 **)> _CreateSwapChainForComposition_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
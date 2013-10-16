// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;

protected:

DXGIFactoryHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
{
    unknown.AddInterface<IDXGIFactory>(this, realobj);
    Construct(); 
}

~DXGIFactoryHook() {}

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
    _DXGIObject.SetPrivateData(Name, DataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID Name,
    _In_  const IUnknown * pUnknown)
{
    _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID Name,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_(*pDataSize)  void * pData)
{
    _DXGIObject.GetPrivateData(Name, pDataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetParent(
    _In_  REFIID riid,
    _Out_  void ** ppParent)
{
    _DXGIObject.GetParent(riid, ppParent);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE EnumAdapters(
    /* [in] */ UINT Adapter,
    _Out_  IDXGIAdapter ** ppAdapter);
NullPtr<void (DXGIFactoryHook::*)(/* [in] */ UINT &, _Out_  IDXGIAdapter ** &)> _EnumAdapters_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, /* [in] */ UINT, _Out_  IDXGIAdapter **)> _EnumAdapters_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE MakeWindowAssociation(
    HWND WindowHandle,
    UINT Flags);
NullPtr<void (DXGIFactoryHook::*)(HWND &, UINT &)> _MakeWindowAssociation_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, HWND, UINT)> _MakeWindowAssociation_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetWindowAssociation(
    _Out_  HWND * pWindowHandle);
NullPtr<void (DXGIFactoryHook::*)(_Out_  HWND * &)> _GetWindowAssociation_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, _Out_  HWND *)> _GetWindowAssociation_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSwapChain(
    _In_  IUnknown * pDevice,
    _In_  DXGI_SWAP_CHAIN_DESC * pDesc,
    _Out_  IDXGISwapChain ** ppSwapChain);
NullPtr<void (DXGIFactoryHook::*)(_In_  IUnknown * &, _In_  DXGI_SWAP_CHAIN_DESC * &, _Out_  IDXGISwapChain ** &)> _CreateSwapChain_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, _In_  IUnknown *, _In_  DXGI_SWAP_CHAIN_DESC *, _Out_  IDXGISwapChain **)> _CreateSwapChain_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSoftwareAdapter(
    /* [in] */ HMODULE Module,
    _Out_  IDXGIAdapter ** ppAdapter);
NullPtr<void (DXGIFactoryHook::*)(/* [in] */ HMODULE &, _Out_  IDXGIAdapter ** &)> _CreateSoftwareAdapter_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, /* [in] */ HMODULE, _Out_  IDXGIAdapter **)> _CreateSoftwareAdapter_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
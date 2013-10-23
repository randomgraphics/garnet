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
    Construct(); 
}

~DXGIFactoryHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
    if (nullptr == DXGIObject) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIFactoryHook(unknown, *DXGIObject, realobj);
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
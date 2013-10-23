// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;

protected:

DXGIAdapterHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
{
    Construct(); 
}

~DXGIAdapterHook() {}

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
        IUnknown * result = (UnknownBase*)new DXGIAdapterHook(unknown, *DXGIObject, realobj);
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
virtual HRESULT STDMETHODCALLTYPE EnumOutputs(
    /* [in] */ UINT Output,
    _Out_  IDXGIOutput ** ppOutput);
NullPtr<void (DXGIAdapterHook::*)(/* [in] */ UINT &, _Out_  IDXGIOutput ** &)> _EnumOutputs_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, /* [in] */ UINT, _Out_  IDXGIOutput **)> _EnumOutputs_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_ADAPTER_DESC * pDesc);
NullPtr<void (DXGIAdapterHook::*)(_Out_  DXGI_ADAPTER_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, _Out_  DXGI_ADAPTER_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(
    _In_  REFGUID InterfaceName,
    _Out_  LARGE_INTEGER * pUMDVersion);
NullPtr<void (DXGIAdapterHook::*)(_In_  REFGUID, _Out_  LARGE_INTEGER * &)> _CheckInterfaceSupport_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, _In_  REFGUID, _Out_  LARGE_INTEGER *)> _CheckInterfaceSupport_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
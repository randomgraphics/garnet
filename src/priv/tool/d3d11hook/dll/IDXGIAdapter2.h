// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIAdapterHook & _DXGIAdapter;
DXGIAdapter1Hook & _DXGIAdapter1;

protected:

DXGIAdapter2Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIAdapterHook & DXGIAdapter, DXGIAdapter1Hook & DXGIAdapter1, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIAdapter(DXGIAdapter)
    , _DXGIAdapter1(DXGIAdapter1)
{
    Construct(); 
}

~DXGIAdapter2Hook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
    if (nullptr == DXGIObject) return nullptr;

    DXGIAdapterHook * DXGIAdapter = (DXGIAdapterHook *)unknown.GetHookedObj(__uuidof(IDXGIAdapter));
    if (nullptr == DXGIAdapter) return nullptr;

    DXGIAdapter1Hook * DXGIAdapter1 = (DXGIAdapter1Hook *)unknown.GetHookedObj(__uuidof(IDXGIAdapter1));
    if (nullptr == DXGIAdapter1) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIAdapter2Hook(unknown, *DXGIObject, *DXGIAdapter, *DXGIAdapter1, realobj);
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
HRESULT STDMETHODCALLTYPE EnumOutputs(
    /* [in] */ UINT Output,
    _Out_  IDXGIOutput ** ppOutput)
{
    return _DXGIAdapter.EnumOutputs(Output, ppOutput);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_ADAPTER_DESC * pDesc)
{
    return _DXGIAdapter.GetDesc(pDesc);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(
    _In_  REFGUID InterfaceName,
    _Out_  LARGE_INTEGER * pUMDVersion)
{
    return _DXGIAdapter.CheckInterfaceSupport(InterfaceName, pUMDVersion);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetDesc1(
    _Out_  DXGI_ADAPTER_DESC1 * pDesc)
{
    return _DXGIAdapter1.GetDesc1(pDesc);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc2(
    _Out_  DXGI_ADAPTER_DESC2 * pDesc);
NullPtr<void (DXGIAdapter2Hook::*)(_Out_  DXGI_ADAPTER_DESC2 * &)> _GetDesc2_pre_ptr;
NullPtr<void (DXGIAdapter2Hook::*)(HRESULT, _Out_  DXGI_ADAPTER_DESC2 *)> _GetDesc2_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
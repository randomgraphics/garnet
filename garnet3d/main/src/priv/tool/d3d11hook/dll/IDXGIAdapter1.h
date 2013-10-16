// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIAdapterHook & _DXGIAdapter;

protected:

DXGIAdapter1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIAdapterHook & DXGIAdapter, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIAdapter(DXGIAdapter)
{
    unknown.AddInterface<IDXGIAdapter1>(this, realobj);
    Construct(); 
}

~DXGIAdapter1Hook() {}

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
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE EnumOutputs(
    /* [in] */ UINT Output,
    _Out_  IDXGIOutput ** ppOutput)
{
    _DXGIAdapter.EnumOutputs(Output, ppOutput);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_ADAPTER_DESC * pDesc)
{
    _DXGIAdapter.GetDesc(pDesc);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(
    _In_  REFGUID InterfaceName,
    _Out_  LARGE_INTEGER * pUMDVersion)
{
    _DXGIAdapter.CheckInterfaceSupport(InterfaceName, pUMDVersion);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc1(
    _Out_  DXGI_ADAPTER_DESC1 * pDesc);
NullPtr<void (DXGIAdapter1Hook::*)(_Out_  DXGI_ADAPTER_DESC1 * &)> _GetDesc1_pre_ptr;
NullPtr<void (DXGIAdapter1Hook::*)(HRESULT, _Out_  DXGI_ADAPTER_DESC1 *)> _GetDesc1_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
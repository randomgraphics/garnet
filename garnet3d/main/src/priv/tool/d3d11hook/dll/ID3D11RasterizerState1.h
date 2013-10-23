// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceChildHook & _D3D11DeviceChild;
D3D11RasterizerStateHook & _D3D11RasterizerState;

protected:

D3D11RasterizerState1Hook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11RasterizerStateHook & D3D11RasterizerState, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11DeviceChild(D3D11DeviceChild)
    , _D3D11RasterizerState(D3D11RasterizerState)
{
    Construct(); 
}

~D3D11RasterizerState1Hook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
    if (nullptr == D3D11DeviceChild) return nullptr;

    D3D11RasterizerStateHook * D3D11RasterizerState = (D3D11RasterizerStateHook *)unknown.GetHookedObj(__uuidof(ID3D11RasterizerState));
    if (nullptr == D3D11RasterizerState) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11RasterizerState1Hook(unknown, *D3D11DeviceChild, *D3D11RasterizerState, realobj);
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
void STDMETHODCALLTYPE GetDevice(
    _Out_  ID3D11Device ** ppDevice)
{
    return _D3D11DeviceChild.GetDevice(ppDevice);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_( *pDataSize )  void * pData)
{
    return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_( DataSize )  const void * pData)
{
    return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE GetDesc(
    _Out_  D3D11_RASTERIZER_DESC * pDesc)
{
    return _D3D11RasterizerState.GetDesc(pDesc);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetDesc1(
    _Out_  D3D11_RASTERIZER_DESC1 * pDesc);
NullPtr<void (D3D11RasterizerState1Hook::*)(_Out_  D3D11_RASTERIZER_DESC1 * &)> _GetDesc1_pre_ptr;
NullPtr<void (D3D11RasterizerState1Hook::*)(_Out_  D3D11_RASTERIZER_DESC1 *)> _GetDesc1_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
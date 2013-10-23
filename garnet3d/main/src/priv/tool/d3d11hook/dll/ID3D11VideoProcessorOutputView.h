// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceChildHook & _D3D11DeviceChild;
D3D11ViewHook & _D3D11View;

protected:

D3D11VideoProcessorOutputViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ViewHook & D3D11View, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11DeviceChild(D3D11DeviceChild)
    , _D3D11View(D3D11View)
{
    Construct(); 
}

~D3D11VideoProcessorOutputViewHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
    if (nullptr == D3D11DeviceChild) return nullptr;

    D3D11ViewHook * D3D11View = (D3D11ViewHook *)unknown.GetHookedObj(__uuidof(ID3D11View));
    if (nullptr == D3D11View) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11VideoProcessorOutputViewHook(unknown, *D3D11DeviceChild, *D3D11View, realobj);
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
void STDMETHODCALLTYPE GetResource(
    _Out_  ID3D11Resource ** ppResource)
{
    return _D3D11View.GetResource(ppResource);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetDesc(
    _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc);
NullPtr<void (D3D11VideoProcessorOutputViewHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (D3D11VideoProcessorOutputViewHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC *)> _GetDesc_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
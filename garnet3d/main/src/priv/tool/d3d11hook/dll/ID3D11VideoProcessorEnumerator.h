// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

D3D11VideoProcessorEnumeratorHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11DeviceChild(D3D11DeviceChild)
{
    Construct(); 
}

~D3D11VideoProcessorEnumeratorHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
    if (nullptr == D3D11DeviceChild) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11VideoProcessorEnumeratorHook(unknown, *D3D11DeviceChild, realobj);
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
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorContentDesc(
    _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc);
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * &)> _GetVideoProcessorContentDesc_pre_ptr;
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC *)> _GetVideoProcessorContentDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckVideoProcessorFormat(
    _In_  DXGI_FORMAT Format,
    _Out_  UINT * pFlags);
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_In_  DXGI_FORMAT &, _Out_  UINT * &)> _CheckVideoProcessorFormat_pre_ptr;
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _In_  DXGI_FORMAT, _Out_  UINT *)> _CheckVideoProcessorFormat_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorCaps(
    _Out_  D3D11_VIDEO_PROCESSOR_CAPS * pCaps);
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_CAPS * &)> _GetVideoProcessorCaps_pre_ptr;
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _Out_  D3D11_VIDEO_PROCESSOR_CAPS *)> _GetVideoProcessorCaps_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorRateConversionCaps(
    _In_  UINT TypeIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps);
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_In_  UINT &, _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * &)> _GetVideoProcessorRateConversionCaps_pre_ptr;
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _In_  UINT, _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS *)> _GetVideoProcessorRateConversionCaps_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorCustomRate(
    _In_  UINT TypeIndex,
    _In_  UINT CustomRateIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate);
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_In_  UINT &, _In_  UINT &, _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * &)> _GetVideoProcessorCustomRate_pre_ptr;
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _In_  UINT, _In_  UINT, _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE *)> _GetVideoProcessorCustomRate_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorFilterRange(
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange);
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_In_  D3D11_VIDEO_PROCESSOR_FILTER &, _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE * &)> _GetVideoProcessorFilterRange_pre_ptr;
NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _In_  D3D11_VIDEO_PROCESSOR_FILTER, _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE *)> _GetVideoProcessorFilterRange_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
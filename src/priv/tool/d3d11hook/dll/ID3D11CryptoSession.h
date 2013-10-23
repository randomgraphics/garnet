// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

D3D11CryptoSessionHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11DeviceChild(D3D11DeviceChild)
{
    Construct(); 
}

~D3D11CryptoSessionHook() {}

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
        IUnknown * result = (UnknownBase*)new D3D11CryptoSessionHook(unknown, *D3D11DeviceChild, realobj);
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
virtual void STDMETHODCALLTYPE GetCryptoType(
    _Out_  GUID * pCryptoType);
NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  GUID * &)> _GetCryptoType_pre_ptr;
NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  GUID *)> _GetCryptoType_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetDecoderProfile(
    _Out_  GUID * pDecoderProfile);
NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  GUID * &)> _GetDecoderProfile_pre_ptr;
NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  GUID *)> _GetDecoderProfile_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetCertificateSize(
    _Out_  UINT * pCertificateSize);
NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  UINT * &)> _GetCertificateSize_pre_ptr;
NullPtr<void (D3D11CryptoSessionHook::*)(HRESULT, _Out_  UINT *)> _GetCertificateSize_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetCertificate(
    _In_  UINT CertificateSize,
    _Out_writes_bytes_(CertificateSize)  BYTE * pCertificate);
NullPtr<void (D3D11CryptoSessionHook::*)(_In_  UINT &, _Out_writes_bytes_(CertificateSize)  BYTE * &)> _GetCertificate_pre_ptr;
NullPtr<void (D3D11CryptoSessionHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_(CertificateSize)  BYTE *)> _GetCertificate_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetCryptoSessionHandle(
    _Out_  HANDLE * pCryptoSessionHandle);
NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  HANDLE * &)> _GetCryptoSessionHandle_pre_ptr;
NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  HANDLE *)> _GetCryptoSessionHandle_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private:
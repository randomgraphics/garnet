// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetCryptoType, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  GUID *, pCryptoType)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetDecoderProfile, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  GUID *, pDecoderProfile)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetCertificateSize, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pCertificateSize)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetCertificate, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  UINT, CertificateSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_(CertificateSize)  BYTE *, pCertificate)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetCryptoSessionHandle, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  HANDLE *, pCryptoSessionHandle)))

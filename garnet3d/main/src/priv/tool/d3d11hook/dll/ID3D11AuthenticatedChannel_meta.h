// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetCertificateSize, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pCertificateSize)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetCertificate, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  UINT, CertificateSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_(CertificateSize)  BYTE *, pCertificate)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetChannelHandle, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  HANDLE *, pChannelHandle)))

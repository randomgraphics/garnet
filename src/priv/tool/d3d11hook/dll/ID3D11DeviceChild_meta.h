// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetDevice, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11Device **, ppDevice)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  REFGUID, guid),
    DEFINE_METHOD_PARAMETER(_Inout_  UINT *, pDataSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_opt_( *pDataSize )  void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  REFGUID, guid),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_opt_( DataSize )  const void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateDataInterface, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  REFGUID, guid),
    DEFINE_METHOD_PARAMETER(_In_opt_  const IUnknown *, pData)))

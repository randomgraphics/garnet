// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  REFGUID, Name),
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_(DataSize)  const void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateDataInterface, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  REFGUID, Name),
    DEFINE_METHOD_PARAMETER(_In_  const IUnknown *, pUnknown)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  REFGUID, Name),
    DEFINE_METHOD_PARAMETER(_Inout_  UINT *, pDataSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_(*pDataSize)  void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetParent, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  REFIID, riid),
    DEFINE_METHOD_PARAMETER(_Out_  void **, ppParent)))

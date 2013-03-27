// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(REFGUID, Name),
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, DataSize),
    DEFINE_METHOD_PARAMETER(const void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateDataInterface, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(REFGUID, Name),
    DEFINE_METHOD_PARAMETER(const IUnknown *, pUnknown)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(REFGUID, Name),
    DEFINE_METHOD_PARAMETER(UINT *, pDataSize),
    DEFINE_METHOD_PARAMETER(void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetParent, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(REFIID, riid),
    DEFINE_METHOD_PARAMETER(void **, ppParent)))

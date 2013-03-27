// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetDevice, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(ID3D11Device **, ppDevice)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(REFGUID, guid),
    DEFINE_METHOD_PARAMETER(UINT *, pDataSize),
    DEFINE_METHOD_PARAMETER(void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(REFGUID, guid),
    DEFINE_METHOD_PARAMETER(UINT, DataSize),
    DEFINE_METHOD_PARAMETER(const void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateDataInterface, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(REFGUID, guid),
    DEFINE_METHOD_PARAMETER(const IUnknown *, pData)))

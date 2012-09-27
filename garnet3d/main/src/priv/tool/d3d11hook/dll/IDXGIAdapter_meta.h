// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, EnumOutputs, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, Output),
    DEFINE_METHOD_PARAMETER(IDXGIOutput **, ppOutput)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDesc, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(DXGI_ADAPTER_DESC *, pDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CheckInterfaceSupport, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(REFGUID, InterfaceName),
    DEFINE_METHOD_PARAMETER(LARGE_INTEGER *, pUMDVersion)))

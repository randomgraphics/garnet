// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetDevice(
    ID3D11Device ** ppDevice);
NullPtr<void (D3D11DeviceChildHook::*)(ID3D11Device ** &)> _GetDevice_pre_ptr;
NullPtr<void (D3D11DeviceChildHook::*)(ID3D11Device **)> _GetDevice_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    REFGUID guid,
    UINT * pDataSize,
    void * pData);
NullPtr<void (D3D11DeviceChildHook::*)(REFGUID, UINT * &, void * &)> _GetPrivateData_pre_ptr;
NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, REFGUID, UINT *, void *)> _GetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    REFGUID guid,
    UINT DataSize,
    const void * pData);
NullPtr<void (D3D11DeviceChildHook::*)(REFGUID, UINT &, const void * &)> _SetPrivateData_pre_ptr;
NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, REFGUID, UINT, const void *)> _SetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    REFGUID guid,
    const IUnknown * pData);
NullPtr<void (D3D11DeviceChildHook::*)(REFGUID, const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, REFGUID, const IUnknown *)> _SetPrivateDataInterface_post_ptr;

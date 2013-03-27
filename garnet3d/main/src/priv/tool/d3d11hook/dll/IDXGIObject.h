// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    REFGUID Name,
    /* [in] */ UINT DataSize,
    const void * pData);
NullPtr<void (DXGIObjectHook::*)(REFGUID, /* [in] */ UINT &, const void * &)> _SetPrivateData_pre_ptr;
NullPtr<void (DXGIObjectHook::*)(HRESULT, REFGUID, /* [in] */ UINT, const void *)> _SetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    REFGUID Name,
    const IUnknown * pUnknown);
NullPtr<void (DXGIObjectHook::*)(REFGUID, const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
NullPtr<void (DXGIObjectHook::*)(HRESULT, REFGUID, const IUnknown *)> _SetPrivateDataInterface_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    REFGUID Name,
    UINT * pDataSize,
    void * pData);
NullPtr<void (DXGIObjectHook::*)(REFGUID, UINT * &, void * &)> _GetPrivateData_pre_ptr;
NullPtr<void (DXGIObjectHook::*)(HRESULT, REFGUID, UINT *, void *)> _GetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetParent(
    REFIID riid,
    void ** ppParent);
NullPtr<void (DXGIObjectHook::*)(REFIID, void ** &)> _GetParent_pre_ptr;
NullPtr<void (DXGIObjectHook::*)(HRESULT, REFIID, void **)> _GetParent_post_ptr;

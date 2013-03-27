// script generated file. DO NOT edit.
#include "pch.h"
#include "d3d11hook.h"
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::SetPrivateData(
    REFGUID Name,
    /* [in] */ UINT DataSize,
    const void * pData)
{
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(Name, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(Name, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, Name, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::SetPrivateDataInterface(
    REFGUID Name,
    const IUnknown * pUnknown)
{
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(Name, pUnknown); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(Name, pUnknown);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, Name, pUnknown); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::GetPrivateData(
    REFGUID Name,
    UINT * pDataSize,
    void * pData)
{
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(Name, pDataSize, pData); }
    HRESULT ret = GetRealObj()->GetPrivateData(Name, pDataSize, pData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, Name, pDataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::GetParent(
    REFIID riid,
    void ** ppParent)
{
    if (_GetParent_pre_ptr._value) { (this->*_GetParent_pre_ptr._value)(riid, ppParent); }
    HRESULT ret = GetRealObj()->GetParent(riid, ppParent);
    if (_GetParent_post_ptr._value) { (this->*_GetParent_post_ptr._value)(ret, riid, ppParent); }
    return ret;
}


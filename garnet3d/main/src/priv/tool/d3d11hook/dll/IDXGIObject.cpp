// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::SetPrivateData(
    _In_  REFGUID Name,
    /* [in] */ UINT DataSize,
    _In_reads_bytes_(DataSize)  const void * pData)
{
    calltrace::AutoTrace trace(L"DXGIObjectHook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(Name, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(Name, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, Name, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::SetPrivateDataInterface(
    _In_  REFGUID Name,
    _In_  const IUnknown * pUnknown)
{
    calltrace::AutoTrace trace(L"DXGIObjectHook::SetPrivateDataInterface");
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(Name, pUnknown); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(Name, pUnknown);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, Name, pUnknown); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::GetPrivateData(
    _In_  REFGUID Name,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_(*pDataSize)  void * pData)
{
    calltrace::AutoTrace trace(L"DXGIObjectHook::GetPrivateData");
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(Name, pDataSize, pData); }
    HRESULT ret = GetRealObj()->GetPrivateData(Name, pDataSize, pData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, Name, pDataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::GetParent(
    _In_  REFIID riid,
    _Out_  void ** ppParent)
{
    calltrace::AutoTrace trace(L"DXGIObjectHook::GetParent");
    if (_GetParent_pre_ptr._value) { (this->*_GetParent_pre_ptr._value)(riid, ppParent); }
    HRESULT ret = GetRealObj()->GetParent(riid, ppParent);
    if (_GetParent_post_ptr._value) { (this->*_GetParent_post_ptr._value)(ret, riid, ppParent); }
    return ret;
}


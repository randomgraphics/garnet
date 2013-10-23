// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceChildHook::GetDevice(
    _Out_  ID3D11Device ** ppDevice)
{
    calltrace::AutoTrace trace(L"D3D11DeviceChildHook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    GetRealObj()->GetDevice(ppDevice);
    if (ppDevice) *ppDevice = RealToHooked( *ppDevice );
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ppDevice); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceChildHook::GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_( *pDataSize )  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceChildHook::GetPrivateData");
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(guid, pDataSize, pData); }
    HRESULT ret = GetRealObj()->GetPrivateData(guid, pDataSize, pData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, guid, pDataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceChildHook::SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_( DataSize )  const void * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceChildHook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(guid, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(guid, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, guid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceChildHook::SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceChildHook::SetPrivateDataInterface");
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(guid, pData); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(guid, pData);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, guid, pData); }
    return ret;
}


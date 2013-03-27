// script generated file. DO NOT edit.
#include "pch.h"
#include "d3d11hook.h"
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceSubObjectHook::GetDevice(
    REFIID riid,
    void ** ppDevice)
{
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(riid, ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(riid, ppDevice);
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, riid, ppDevice); }
    return ret;
}


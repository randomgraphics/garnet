// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurface1Hook::GetDC(
    /* [in] */ BOOL Discard,
    _Out_  HDC * phdc)
{
    calltrace::AutoTrace trace(L"DXGISurface1Hook::GetDC");
    if (_GetDC_pre_ptr._value) { (this->*_GetDC_pre_ptr._value)(Discard, phdc); }
    HRESULT ret = GetRealObj()->GetDC(Discard, phdc);
    if (_GetDC_post_ptr._value) { (this->*_GetDC_post_ptr._value)(ret, Discard, phdc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurface1Hook::ReleaseDC(
    _In_opt_  RECT * pDirtyRect)
{
    calltrace::AutoTrace trace(L"DXGISurface1Hook::ReleaseDC");
    if (_ReleaseDC_pre_ptr._value) { (this->*_ReleaseDC_pre_ptr._value)(pDirtyRect); }
    HRESULT ret = GetRealObj()->ReleaseDC(pDirtyRect);
    if (_ReleaseDC_post_ptr._value) { (this->*_ReleaseDC_post_ptr._value)(ret, pDirtyRect); }
    return ret;
}


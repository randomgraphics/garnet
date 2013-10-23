// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11BlendState1Hook::GetDesc1(
    _Out_  D3D11_BLEND_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11BlendState1Hook::GetDesc1");
    if (_GetDesc1_pre_ptr._value) { (this->*_GetDesc1_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc1(pDesc);
    if (_GetDesc1_post_ptr._value) { (this->*_GetDesc1_post_ptr._value)(pDesc); }
}


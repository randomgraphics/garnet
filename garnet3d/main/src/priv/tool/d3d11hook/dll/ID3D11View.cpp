// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ViewHook::GetResource(
    _Out_  ID3D11Resource ** ppResource)
{
    calltrace::AutoTrace trace(L"D3D11ViewHook::GetResource");
    if (_GetResource_pre_ptr._value) { (this->*_GetResource_pre_ptr._value)(ppResource); }
    GetRealObj()->GetResource(ppResource);
    if (ppResource) *ppResource = RealToHooked( *ppResource );
    if (_GetResource_post_ptr._value) { (this->*_GetResource_post_ptr._value)(ppResource); }
}


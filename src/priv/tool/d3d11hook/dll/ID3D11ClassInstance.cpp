// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ClassInstanceHook::GetClassLinkage(
    _Out_  ID3D11ClassLinkage ** ppLinkage)
{
    calltrace::AutoTrace trace(L"D3D11ClassInstanceHook::GetClassLinkage");
    if (_GetClassLinkage_pre_ptr._value) { (this->*_GetClassLinkage_pre_ptr._value)(ppLinkage); }
    GetRealObj()->GetClassLinkage(ppLinkage);
    if (ppLinkage) *ppLinkage = RealToHooked( *ppLinkage );
    if (_GetClassLinkage_post_ptr._value) { (this->*_GetClassLinkage_post_ptr._value)(ppLinkage); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ClassInstanceHook::GetDesc(
    _Out_  D3D11_CLASS_INSTANCE_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11ClassInstanceHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ClassInstanceHook::GetInstanceName(
    _Out_writes_opt_(*pBufferLength)  LPSTR pInstanceName,
    _Inout_  SIZE_T * pBufferLength)
{
    calltrace::AutoTrace trace(L"D3D11ClassInstanceHook::GetInstanceName");
    if (_GetInstanceName_pre_ptr._value) { (this->*_GetInstanceName_pre_ptr._value)(pInstanceName, pBufferLength); }
    GetRealObj()->GetInstanceName(pInstanceName, pBufferLength);
    if (_GetInstanceName_post_ptr._value) { (this->*_GetInstanceName_post_ptr._value)(pInstanceName, pBufferLength); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ClassInstanceHook::GetTypeName(
    _Out_writes_opt_(*pBufferLength)  LPSTR pTypeName,
    _Inout_  SIZE_T * pBufferLength)
{
    calltrace::AutoTrace trace(L"D3D11ClassInstanceHook::GetTypeName");
    if (_GetTypeName_pre_ptr._value) { (this->*_GetTypeName_pre_ptr._value)(pTypeName, pBufferLength); }
    GetRealObj()->GetTypeName(pTypeName, pBufferLength);
    if (_GetTypeName_post_ptr._value) { (this->*_GetTypeName_post_ptr._value)(pTypeName, pBufferLength); }
}


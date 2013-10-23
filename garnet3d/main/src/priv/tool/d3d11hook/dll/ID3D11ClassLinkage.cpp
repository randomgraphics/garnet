// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11ClassLinkageHook::GetClassInstance(
    _In_  LPCSTR pClassInstanceName,
    _In_  UINT InstanceIndex,
    _Out_  ID3D11ClassInstance ** ppInstance)
{
    calltrace::AutoTrace trace(L"D3D11ClassLinkageHook::GetClassInstance");
    if (_GetClassInstance_pre_ptr._value) { (this->*_GetClassInstance_pre_ptr._value)(pClassInstanceName, InstanceIndex, ppInstance); }
    HRESULT ret = GetRealObj()->GetClassInstance(pClassInstanceName, InstanceIndex, ppInstance);
    if (ppInstance) *ppInstance = RealToHooked( *ppInstance );
    if (_GetClassInstance_post_ptr._value) { (this->*_GetClassInstance_post_ptr._value)(ret, pClassInstanceName, InstanceIndex, ppInstance); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11ClassLinkageHook::CreateClassInstance(
    _In_  LPCSTR pClassTypeName,
    _In_  UINT ConstantBufferOffset,
    _In_  UINT ConstantVectorOffset,
    _In_  UINT TextureOffset,
    _In_  UINT SamplerOffset,
    _Out_  ID3D11ClassInstance ** ppInstance)
{
    calltrace::AutoTrace trace(L"D3D11ClassLinkageHook::CreateClassInstance");
    if (_CreateClassInstance_pre_ptr._value) { (this->*_CreateClassInstance_pre_ptr._value)(pClassTypeName, ConstantBufferOffset, ConstantVectorOffset, TextureOffset, SamplerOffset, ppInstance); }
    HRESULT ret = GetRealObj()->CreateClassInstance(pClassTypeName, ConstantBufferOffset, ConstantVectorOffset, TextureOffset, SamplerOffset, ppInstance);
    if (ppInstance) *ppInstance = RealToHooked( *ppInstance );
    if (_CreateClassInstance_post_ptr._value) { (this->*_CreateClassInstance_post_ptr._value)(ret, pClassTypeName, ConstantBufferOffset, ConstantVectorOffset, TextureOffset, SamplerOffset, ppInstance); }
    return ret;
}


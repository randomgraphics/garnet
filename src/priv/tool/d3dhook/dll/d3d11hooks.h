#pragma once
#include "hooks.h"
#include "d3d/d3d11_1.h"
#include "d3d/dxgidebug.h"

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class INPUT_TYPE>
inline IUnknown * RealToHooked11(const IID & realIId, INPUT_TYPE * realobj)
{
    // -----------------------------------------------------------------------------
    // {CF9120C7-4E7A-493A-96AA-0C33583803F6}
    /// GUID that is used to attach hooked object pointer to real interface.
    static const GUID HOOKED_OBJECT_GUID =
    { 0xcf9120c7, 0x4e7a, 0x493a, { 0x96, 0xaa, 0xc, 0x33, 0x58, 0x38, 0x3, 0xf6 } };

    if (nullptr == realobj)
    {
        return nullptr;
    }

    if (IsHooked(realobj))
    {
        // Expecting a realobj, not a hooked object.
        HOOK_ASSERT(false);
        return realobj;
    }

    GN::AutoComPtr<UnknownBase> base;
    GN::AutoComPtr<WeakUnknownRef> unknownRef;
    UINT size = (UINT)sizeof(unknownRef);
    HRESULT hr = realobj->GetPrivateData(
        HOOKED_OBJECT_GUID,
        &size,
        &unknownRef);
    if (SUCCEEDED(hr))
    {
        base = unknownRef->promote();
    }

    if (!base)
    {
        base = UnknownBase::sCreateNew(realobj);
        unknownRef.set(new WeakUnknownRef());
        unknownRef->attach(base);
        realobj->SetPrivateDataInterface(HOOKED_OBJECT_GUID, unknownRef);
    }

    IUnknown * hooked;
    if (SUCCEEDED(base->QueryInterface(realIId, (void**)&hooked)))
    {
        HOOK_ASSERT(hooked);
        realobj->Release();
        return hooked;
    }
    else
    {
        // Fall back to real object.
        return realobj;
    }
}

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class REAL_INTERFACE>
inline REAL_INTERFACE * RealToHooked11(REAL_INTERFACE * realobj)
{
    return (REAL_INTERFACE*)RealToHooked11(__uuidof(REAL_INTERFACE), realobj);
}

#include "d3d11hooks.inl"

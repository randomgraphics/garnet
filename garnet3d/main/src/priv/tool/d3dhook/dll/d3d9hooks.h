#pragma once
#include "hooks.h"
#include "d3d/d3d9.h"

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class INPUT_TYPE>
inline IUnknown * RealToHooked9(const IID & realiid, INPUT_TYPE * realobj)
{
    if (nullptr == realobj)
    {
        return nullptr;
    }

    if (IsHooked(realobj))
    {
        // Expecting a realobj, not a hooked object.
        GN_UNEXPECTED();
        return realobj;
    }

    GN::AutoComPtr<UnknownBase> base = UnknownBaseTable::get(realobj);
    if (!base)
    {
        base = UnknownBase::sCreateNew(realobj);
        UnknownBaseTable::add(realobj, base);
    }
    if (!base)
    {
        HOOK_ERROR_LOG("Failed to create new UnknownBase instance.");
        return realobj;
    }

    IUnknown * hooked;
    if (SUCCEEDED(base->QueryInterface(realiid, (void**)&hooked)))
    {
        GN_ASSERT(hooked);
        realobj->Release();
        return hooked;
    }
    else
    {
        return realobj;
    }
}

#if 0
// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object, using
/// GetPrivateData and SetPrivateData
template<>
inline IUnknown * RealToHooked9<IDirect3DResource9>(const IID & realiid, IDirect3DResource9 * realobj)
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
        GN_UNEXPECTED();
        return realobj;
    }

    GN::AutoComPtr<UnknownBase> base;
    WeakUnknownRef * unknownRef;
    DWORD size = (DWORD)sizeof(unknownRef);
    HRESULT hr = realobj->GetPrivateData(HOOKED_OBJECT_GUID, &unknownRef, &size);
    if (SUCCEEDED(hr))
    {
        base = unknownRef->promote();
    }

    if (!base)
    {
        base = UnknownBase::sCreateNew(realobj);
        unknownRef = new WeakUnknownRef();
        unknownRef->attach(base);
        realobj->SetPrivateData(HOOKED_OBJECT_GUID, unknownRef, size, D3DSPD_IUNKNOWN);
        unknownRef->AddRef(); // SetPrivateData() does not increase refcount.
    }

    IUnknown * hooked;
    if (SUCCEEDED(base->QueryInterface(realiid, (void**)&hooked)))
    {
        GN_ASSERT(hooked);
        realobj->Release();
        return hooked;
    }
    else
    {
        // Fall back to real object.
        return realobj;
    }
}
#endif

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class REAL_INTERFACE>
inline REAL_INTERFACE * RealToHooked9(REAL_INTERFACE * realobj)
{
    return (REAL_INTERFACE*)RealToHooked9(__uuidof(REAL_INTERFACE), realobj);
}

#include "d3d9hooks.inl"

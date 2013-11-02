#pragma once
#include "hooks.h"
#include "d3d/d2d1_1.h"
#include "d3d/dwrite.h"

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class INPUT_TYPE>
inline IUnknown * RealToHooked_D2D(const IID & realiid, INPUT_TYPE * realobj)
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

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class REAL_INTERFACE>
inline REAL_INTERFACE * RealToHooked_D2D(REAL_INTERFACE * realobj)
{
    return (REAL_INTERFACE*)RealToHooked_D2D(__uuidof(REAL_INTERFACE), realobj);
}

#include "d2dhooks.inl"

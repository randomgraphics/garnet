#pragma once
#include "hooks.h"
#include "d3d/d3d9.h"

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class INPUT_TYPE>
inline IUnknown * RealToHooked9(const IID & realIId, INPUT_TYPE * realObj)
{
    (realIId);
    (realObj);
    return nullptr;
}

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class REAL_INTERFACE>
inline REAL_INTERFACE * RealToHooked9(REAL_INTERFACE * realobj)
{
    return (REAL_INTERFACE*)RealToHooked9(__uuidof(REAL_INTERFACE), realobj);
}

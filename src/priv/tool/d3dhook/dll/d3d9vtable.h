#pragma once
#include "vtable.h"
#include "d3d/d3d9.h"

/// update real vtable to hooked vtable
template<typename T> void RealToHooked9(T * p);

/// update real vtable to hooked vtable based on interface ID
void RealToHooked9(const IID & iid, void * p);

/// setup D3D9 vtables
void SetupD3D9HookedVTables();

#include "d3d9vtable.inl"

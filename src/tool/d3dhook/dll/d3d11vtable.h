#pragma once
#include "vtable.h"
#include "d3d/d3d11_1.h"
#include "d3d/dxgidebug.h"

/// update real vtable to hooked vtable
template<typename T>
void RealToHooked11(T * p);

/// update real vtable to hooked vtable based on interface ID
void RealToHooked11(const IID & iid, void * p);

/// setup D3D11 vtables
void SetupD3D11HookedVTables();

#include "d3d11vtable.inl"

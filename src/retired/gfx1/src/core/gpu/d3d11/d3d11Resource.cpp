#include "pch.h"
#include "d3d11Resource.h"
#include "d3d11Gpu.h"

GN::gfx::D3D11Resource::D3D11Resource(D3D11Gpu & r): mGpu(r) { mGpu.insertResource(this); }

GN::gfx::D3D11Resource::~D3D11Resource() { mGpu.removeResource(this); }

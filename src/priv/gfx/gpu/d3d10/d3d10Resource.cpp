#include "pch.h"
#include "d3d10Resource.h"
#include "d3d10Gpu.h"

GN::gfx::D3D10Resource::D3D10Resource( D3D10Gpu & r )
    : mGpu(r)
{
    mGpu.insertResource( this );
}

GN::gfx::D3D10Resource::~D3D10Resource()
{
    mGpu.removeResource( this );
}

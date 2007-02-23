#include "pch.h"
#include "d3d10Resource.h"
#include "d3d10Renderer.h"

GN::gfx::D3D10Resource::D3D10Resource( D3D10Renderer & r )
    : mRenderer(r)
{
    mRenderer.insertResource( this );
}

GN::gfx::D3D10Resource::~D3D10Resource()
{
    mRenderer.removeResource( this );
}

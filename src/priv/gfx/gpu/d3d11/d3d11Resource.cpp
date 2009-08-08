#include "pch.h"
#include "d3d11Resource.h"
#include "d3d11Renderer.h"

GN::gfx::D3D11Resource::D3D11Resource( D3D11Renderer & r )
    : mRenderer(r)
{
    mRenderer.insertResource( this );
}

GN::gfx::D3D11Resource::~D3D11Resource()
{
    mRenderer.removeResource( this );
}

#include "pch.h"
#include "d3d9Resource.h"
#include "d3d9Renderer.h"

GN::gfx::D3D9Resource::D3D9Resource( D3D9Renderer & r )
    : mRenderer(r)
{
    mRenderer.insertResource( this );
}

GN::gfx::D3D9Resource::~D3D9Resource()
{
    mRenderer.removeResource( this );
}

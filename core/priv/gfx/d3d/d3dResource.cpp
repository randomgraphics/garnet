#include "pch.h"
#include "d3dResource.h"
#include "d3dRenderer.h"

GN::gfx::D3DResource::D3DResource( D3DRenderer & r )
    : mRenderer(r)
{
    mRenderer.insertResource( this );
}

GN::gfx::D3DResource::~D3DResource()
{
    mRenderer.removeResource( this );
}

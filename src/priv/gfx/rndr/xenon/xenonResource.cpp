#include "pch.h"
#include "xenonResource.h"
#include "xenonRenderer.h"

GN::gfx::XenonResource::XenonResource( XenonRenderer & r )
    : mRenderer(r)
{
    mRenderer.insertResource( this );
}

GN::gfx::XenonResource::~XenonResource()
{
    mRenderer.removeResource( this );
}

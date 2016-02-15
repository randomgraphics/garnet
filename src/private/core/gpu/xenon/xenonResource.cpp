#include "pch.h"
#include "xenonResource.h"
#include "xenonGpu.h"

GN::gfx::XenonResource::XenonResource( XenonGpu & r )
    : mGpu(r)
{
    mGpu.insertResource( this );
}

GN::gfx::XenonResource::~XenonResource()
{
    mGpu.removeResource( this );
}

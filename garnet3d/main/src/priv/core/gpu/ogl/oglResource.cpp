#include "pch.h"
#include "oglResource.h"
#include "oglGpu.h"

GN::gfx::OGLResource::OGLResource( OGLGpu & r )
    : mGpu(r)
{
    mGpu.insertResource( this );
}

GN::gfx::OGLResource::~OGLResource()
{
    mGpu.removeResource( this );
}

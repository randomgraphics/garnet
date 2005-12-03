#include "pch.h"
#include "d3dResource.h"
#include "d3dRenderer.h"

GN::gfx::D3DResource::D3DResource( D3DRenderer & r )
    : mRenderer(r), mState(UNINITIALIZED)
{
    mRenderer.insertResource( this );
}

GN::gfx::D3DResource::~D3DResource()
{
    GN_ASSERT( UNINITIALIZED == mState );
    mRenderer.removeResource( this );
}

bool GN::gfx::D3DResource::switchState( ResourceState newState )
{
    if( mState == newState ) return true;

    switch( newState )
    {
        case UNINITIALIZED:
            break;

        case CREATED:
            break;

        case READY:
            break;

        default:
            GN_UNEXPECTED();
    }

    // sucess
    return true;
}

bool GN::gfx::D3DResource::deviceCreate()
{
    GN_ASSERT( UNINITIALIZED == mState );
    mState = CREATED;
    return true;
}

bool GN::gfx::D3DResource::deviceRestore()
{
    GN_ASSERT( CREATED == mState );
    mState = READY;
    return true;
}

void GN::gfx::D3DResource::deviceDispose()
{
    mState = ( READY == mState || CREATED == mState ) ? CREATED : UNINITIALIZED;
}

void GN::gfx::D3DResource::deviceDestroy()
{
    mState = UNINITIALIZED;
}


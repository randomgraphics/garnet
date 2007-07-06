#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9RenderStateBlock");

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// public method
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9RenderStateBlock::D3D9RenderStateBlock( D3D9GraphicsSystem & gs )
{
    mDevice = gs.d3ddev();

    if( 0 == mDevice )
    {
        GN_UNEXPECTED();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::setRenderState( D3DRENDERSTATETYPE type, DWORD value )
{
    GN_UNUSED_PARAM( type );
    GN_UNUSED_PARAM( value );
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::setSamplerState( size_t stage, D3DSAMPLERSTATETYPE type, DWORD value )
{
    GN_UNUSED_PARAM( stage );
    GN_UNUSED_PARAM( type );
    GN_UNUSED_PARAM( value );
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::setTextureState( size_t stage, D3DTEXTURESTAGESTATETYPE type, DWORD value )
{
    GN_UNUSED_PARAM( stage );
    GN_UNUSED_PARAM( type );
    GN_UNUSED_PARAM( value );
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::apply( const D3D9RenderStateBlock * last ) const
{
    GN_UNUSED_PARAM( last );
    GN_UNIMPL();
}

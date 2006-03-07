#include "pch.h"
#include "d3dRenderer.h"
#include "d3dTexture.h"
#include "garnet/GNd3d.h"

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::supportTextureFormat(
    TexType type, BitField usage, ClrFmt format ) const
{
    GN_GUARD;

    return D3D_OK == checkD3DDeviceFormat(
        texUsage2D3DUsage(usage),
        texType2D3DResourceType(type),
        d3d::clrFmt2D3DFormat(format) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::D3DRenderer::createTexture( const TextureDesc & desc,
                                     const TextureLoader & loader )
{
    GN_GUARD;

    AutoRef<D3DTexture> p( new D3DTexture(*this) );
    p->setLoader( loader );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::D3DRenderer::createTextureFromFile( File & file )
{
    GN_GUARD;

    AutoRef<D3DTexture> p( new D3DTexture(*this) );
    if( p->initFromFile(file) ) return p.detach();
    return 0;

    GN_UNGUARD;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::textureDeviceRestore()
{
    //GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // reset texture parameters
    for( size_t i = 0; i < MAX_TEXTURE_STAGES; ++i )
    {
        TexParameters & tp = mTexParameters[i];
        tp.min = tp.mag = tp.mip = D3DTEXF_FORCE_DWORD;
        tp.s = tp.t = tp.r = tp.q = D3DTADDRESS_FORCE_DWORD;
    }

    // successful
    return true;

    //GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::applyTexture() const
{
    GN_GUARD_SLOW;

    PIXPERF_BEGIN_EVENT( 0, "GN::gfx::D3DRenderer::applyTexture()" );

    GN_ASSERT( getDirtyTextureStages() > 0 );

    const Texture * const * texlist = getCurrentTextures();

    size_t n = min<size_t>( getDirtyTextureStages(), getCaps(CAPS_MAX_TEXTURE_STAGES) );

    // apply texture list
    for( UINT i = 0; i < n; ++i )
    {
        const Texture * tex = texlist[i];

        if( 0 == tex )
        {
            mDevice->SetTexture( i, NULL );
        }
        else
        {
            const D3DTexture * p = safeCast<const D3DTexture*>(tex);

            mDevice->SetTexture( i, p->getD3DTexture() );

            // update texture parameters
            updateTextureFilters( i, p->getD3DFilters() );
            updateTextureWraps( i, p->getD3DWraps() );
        }
    }

    clearDirtyTextureStages();

    PIXPERF_END_EVENT();

    GN_UNGUARD_SLOW;
}


//
//
// ------------------------------------------------------------------------
GN_INLINE void
GN::gfx::D3DRenderer::updateTextureFilters( size_t stage, const D3DTEXTUREFILTERTYPE * filters ) const
{
    GN_ASSERT( stage < MAX_TEXTURE_STAGES );

    // update texture filters

    if( *filters != mTexParameters[stage].min )
    {
        mTexParameters[stage].min = *filters;
        GN_DX_CHECK( mDevice->SetSamplerState( (UINT)stage, D3DSAMP_MINFILTER, *filters ) );
    }
    ++filters;
    if( *filters != mTexParameters[stage].mag )
    {
        mTexParameters[stage].mag = *filters;
        GN_DX_CHECK( mDevice->SetSamplerState( (UINT)stage, D3DSAMP_MAGFILTER, *filters ) );
    }
    ++filters;
    if( *filters != mTexParameters[stage].mip )
    {
        mTexParameters[stage].mip = *filters;
        GN_DX_CHECK( mDevice->SetSamplerState( (UINT)stage, D3DSAMP_MIPFILTER, *filters ) );
    }
}

//
//
// ------------------------------------------------------------------------
GN_INLINE void
GN::gfx::D3DRenderer::updateTextureWraps( size_t stage, const D3DTEXTUREADDRESS * strq ) const
{
    GN_ASSERT( stage < MAX_TEXTURE_STAGES );

    if( *strq != mTexParameters[stage].s )
    {
        mTexParameters[stage].s = *strq;
        mDevice->SetSamplerState( (UINT)stage, D3DSAMP_ADDRESSU, *strq );
    }
    ++strq;
    if( *strq != mTexParameters[stage].t )
    {
        mTexParameters[stage].t = *strq;
        mDevice->SetSamplerState( (UINT)stage, D3DSAMP_ADDRESSV, *strq );
    }
    ++strq;
    if( *strq != mTexParameters[stage].r )
    {
        mTexParameters[stage].r = *strq;
        mDevice->SetSamplerState( (UINT)stage, D3DSAMP_ADDRESSW, *strq );
    }
}

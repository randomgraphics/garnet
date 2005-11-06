//
//
// ------------------------------------------------------------------------
GN_INLINE void
GN::gfx::D3DRenderer::updateTextureFilters( uint32_t stage, const D3DTEXTUREFILTERTYPE * filters )
{
    GN_ASSERT( stage < RenderStateBlockDesc::MAX_STAGES );

    // update texture filters

    if( *filters != mTexParameters[stage].min )
    {
        mTexParameters[stage].min = *filters;
        GN_DX_CHECK( mDevice->SetSamplerState( stage, D3DSAMP_MINFILTER, *filters ) );
    }
    ++filters;
    if( *filters != mTexParameters[stage].mag )
    {
        mTexParameters[stage].mag = *filters;
        GN_DX_CHECK( mDevice->SetSamplerState( stage, D3DSAMP_MAGFILTER, *filters ) );
    }
    ++filters;
    if( *filters != mTexParameters[stage].mip )
    {
        mTexParameters[stage].mip = *filters;
        GN_DX_CHECK( mDevice->SetSamplerState( stage, D3DSAMP_MIPFILTER, *filters ) );
    }
}

//
//
// ------------------------------------------------------------------------
GN_INLINE void
GN::gfx::D3DRenderer::updateTextureWraps( uint32_t stage, const D3DTEXTUREADDRESS * strq )
{
    GN_ASSERT( stage < RenderStateBlockDesc::MAX_STAGES );

    if( *strq != mTexParameters[stage].s )
    {
        mTexParameters[stage].s = *strq;
        mDevice->SetSamplerState( stage, D3DSAMP_ADDRESSU, *strq );
    }
    ++strq;
    if( *strq != mTexParameters[stage].t )
    {
        mTexParameters[stage].t = *strq;
        mDevice->SetSamplerState( stage, D3DSAMP_ADDRESSV, *strq );
    }
    ++strq;
    if( *strq != mTexParameters[stage].r )
    {
        mTexParameters[stage].r = *strq;
        mDevice->SetSamplerState( stage, D3DSAMP_ADDRESSW, *strq );
    }
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::setD3DRenderState(
    D3DRENDERSTATETYPE type, DWORD newval )
{
#if GN_DEBUG

    if( 0 == (D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD oldval;
        GN_DX_CHECK( getDevice()->GetRenderState( type, &oldval ) );
        if( oldval == newval ) return;
    }

#endif

   GN_DX_CHECK( getDevice()->SetRenderState( type, newval ) );
}

//
//
// ----------------------------------------------------------------------------
#if GN_XENON
GN_INLINE void GN::gfx::D3DRenderer::setD3DTextureState( UINT, D3DTEXTURESTAGESTATETYPE, DWORD )
{
    // NOTE: there's no texture stage state on xenon.
}
#else
GN_INLINE void GN::gfx::D3DRenderer::setD3DTextureState(
    UINT stage, D3DTEXTURESTAGESTATETYPE type, DWORD newval )
{
#if GN_DEBUG

    if( 0 == (D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD oldval;
        GN_DX_CHECK( getDevice()->GetTextureStageState( stage, type, &oldval ) );
        if( oldval == newval ) return;
    }

#endif

    GN_DX_CHECK( getDevice()->SetTextureStageState( stage, type, newval ) );
}
#endif

//
//
// ----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::setD3DSamplerState(
    UINT stage, D3DSAMPLERSTATETYPE type, DWORD newval )
{
#if GN_DEBUG

    if( 0 == (D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD oldval;
        GN_DX_CHECK( getDevice()->GetSamplerState( stage, type, &oldval ) );
        if( oldval == newval ) return;
    }

#endif

    GN_DX_CHECK( getDevice()->SetSamplerState( stage, type, newval ) );
}

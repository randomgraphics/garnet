//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9Renderer::setD3DRenderState(
    D3DRENDERSTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;

#if GN_DEBUG_BUILD
    if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD old;
        GN_DX9_CHECK( mDevice->GetRenderState( type, &old ) );
        if( old != value )
        {
            GN_DX9_CHECK( mDevice->SetRenderState( type, value ) );
        }
    }
    else
#endif

    GN_DX9_CHECK( mDevice->SetRenderState( type, value ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9Renderer::setD3DSamplerState(
    UINT stage, D3DSAMPLERSTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;

    GN_ASSERT( stage < MAX_TEXTURE_STAGES );

#if GN_DEBUG_BUILD
    if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD old;
        GN_DX9_CHECK( mDevice->GetSamplerState( stage, type, &old ) );
        if( old != value )
        {
            GN_DX9_CHECK( mDevice->SetSamplerState( stage, type, value ) );
        }
    }
    else
#endif

    GN_DX9_CHECK( mDevice->SetSamplerState( stage, type, value ) );

    GN_UNGUARD_SLOW;
}

#if !GN_XENON

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9Renderer::setD3DTextureState(
    UINT stage, D3DTEXTURESTAGESTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;

    GN_ASSERT( stage < MAX_TEXTURE_STAGES );

#if GN_DEBUG_BUILD
    if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD old;
        GN_DX9_CHECK( mDevice->GetTextureStageState( stage, type, &old ) );
        if( value != old )
        {
            GN_DX9_CHECK( mDevice->SetTextureStageState( stage, type, value ) );
        }
    }
    else
#endif

    GN_DX9_CHECK( mDevice->SetTextureStageState( stage, type, value ) );

    GN_UNGUARD_SLOW;
}

#endif // !GN_XENON

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::setD3DRenderState(
    D3DRENDERSTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;
    GN_ASSERT( 0 <= type && type < MAX_D3D_RENDER_STATES );
    StateValue<DWORD> & s = mRenderStates[type];
    if( value != s.value || !s.initialized )
    {
        GN_DX_CHECK( mDevice->SetRenderState( type, value ) );
        s.initialized = true;
        s.value = value;
    }
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::setD3DSamplerState(
    UINT stage, D3DSAMPLERSTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;
    GN_ASSERT(
        0 <= stage && stage < MAX_TEXTURE_STAGES &&
        0 <= type && type < MAX_D3D_SAMPLER_STATES );
    StateValue<DWORD> & s = mSamplerStates[stage][type];
    if( value != s.value || !s.initialized )
    {
        GN_DX_CHECK( mDevice->SetSamplerState( stage, type, value ) );
        s.value = value;
        s.initialized = true;
    }
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::setD3DTextureState(
    UINT stage, D3DTEXTURESTAGESTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;
    GN_ASSERT(
        0 <= stage && stage < MAX_TEXTURE_STAGES &&
        0 <= type && type < MAX_D3D_TEXTURE_STATES );
    StateValue<DWORD> & s = mTextureStates[stage][type];
    if( value != s.value || !s.initialized )
    {
        GN_DX_CHECK( mDevice->SetTextureStageState( stage, type, value ) );
        s.value = value;
        s.initialized = true;
    }
    GN_UNGUARD_SLOW;
}

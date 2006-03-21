//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3D9Renderer::setD3DRenderState(
    D3DRENDERSTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;
    GN_ASSERT( 0 <= type && type < MAX_D3D_RENDER_STATES );

    StateValue<DWORD> & s = mRenderStates[type];

#if GN_DEBUG
    if( s.initialized && !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD d;
        GN_DX9_CHECK( mDevice->GetRenderState( type, &d ) );
        GN_ASSERT( d == s.value );
    }
#endif

    if( value != s.value || !s.initialized )
    {
        GN_DX9_CHECK( mDevice->SetRenderState( type, value ) );
        s.initialized = true;
        s.value = value;
#if GN_DEBUG
        if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
        {
            DWORD d;
            GN_DX9_CHECK( mDevice->GetRenderState( type, &d ) );
            GN_ASSERT( d == s.value );
        }
#endif
    }
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE DWORD GN::gfx::D3D9Renderer::getD3DRenderState( D3DRENDERSTATETYPE type )
{
    GN_GUARD_SLOW;

    GN_ASSERT(
        0 <= type && type < MAX_D3D_RENDER_STATES &&
        mRenderStates[type].initialized );

#if GN_DEBUG
    if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD d;
        GN_DX9_CHECK( mDevice->GetRenderState( type, &d ) );
        GN_ASSERT( d == mRenderStates[type].value );
    }
#endif

    return mRenderStates[type].value;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3D9Renderer::setD3DSamplerState(
    UINT stage, D3DSAMPLERSTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;

    GN_ASSERT(
        stage < MAX_TEXTURE_STAGES &&
        0 <= type && type < MAX_D3D_SAMPLER_STATES );

    StateValue<DWORD> & s = mSamplerStates[stage][type];

#if GN_DEBUG
    if( s.initialized && !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD d;
        GN_DX9_CHECK( mDevice->GetSamplerState( stage, type, &d ) );
        GN_ASSERT( d == s.value );
    }
#endif

    if( value != s.value || !s.initialized )
    {
        GN_DX9_CHECK( mDevice->SetSamplerState( stage, type, value ) );
        s.initialized = true;
        s.value = value;

#if GN_DEBUG
        if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
        {
            DWORD d;
            GN_DX9_CHECK( mDevice->GetSamplerState( stage, type, &d ) );
            GN_ASSERT( d == s.value );
        }
#endif
    }
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE DWORD GN::gfx::D3D9Renderer::getD3DSamplerState( UINT stage, D3DSAMPLERSTATETYPE type )
{
    GN_GUARD_SLOW;

    GN_ASSERT(
        stage < MAX_TEXTURE_STAGES &&
        0 <= type && type < MAX_D3D_SAMPLER_STATES &&
        mSamplerStates[stage][type].initialized );

#if GN_DEBUG
    if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD d;
        GN_DX9_CHECK( mDevice->GetSamplerState( stage, type, &d ) );
        GN_ASSERT( d == mSamplerStates[stage][type].value );
    }
#endif

    return mSamplerStates[stage][type].value;

    GN_UNGUARD_SLOW;
}

#if !GN_XENON

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3D9Renderer::setD3DTextureState(
    UINT stage, D3DTEXTURESTAGESTATETYPE type, DWORD value )
{
    GN_GUARD_SLOW;
    GN_ASSERT(
        stage < MAX_TEXTURE_STAGES &&
        0 <= type && type < MAX_D3D_TEXTURE_STATES );

    StateValue<DWORD> & s = mTextureStates[stage][type];

#if GN_DEBUG
    if( s.initialized && !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD d;
        GN_DX9_CHECK( mDevice->GetTextureStageState( stage, type, &d ) );
        GN_ASSERT( d == s.value );
    }
#endif

    if( value != s.value || !s.initialized )
    {
        GN_DX9_CHECK( mDevice->SetTextureStageState( stage, type, value ) );
        s.initialized = true;
        s.value = value;
#if GN_DEBUG
        if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
        {
            DWORD d;
            GN_DX9_CHECK( mDevice->GetTextureStageState( stage, type, &d ) );
            GN_ASSERT( d == s.value );
        }
#endif
    }
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE DWORD GN::gfx::D3D9Renderer::getD3DTextureState( UINT stage, D3DTEXTURESTAGESTATETYPE type )
{
    GN_GUARD_SLOW;

    GN_ASSERT(
        stage < MAX_TEXTURE_STAGES &&
        0 <= type && type < MAX_D3D_SAMPLER_STATES &&
        mTextureStates[stage][type].initialized );

#if GN_DEBUG
    if( !(D3DCREATE_PUREDEVICE & getBehavior()) )
    {
        DWORD d;
        GN_DX9_CHECK( mDevice->GetTextureStageState( stage, type, &d ) );
        GN_ASSERT( d == mTextureStates[stage][type].value );
    }
#endif

    return mTextureStates[stage][type].value;

    GN_UNGUARD_SLOW;
}

#endif // !GN_XENON

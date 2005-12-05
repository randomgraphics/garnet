//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::applyDrawState()
{
    GN_GUARD_SLOW;

    if( 0 == mDrawState.dirtyFlags.u32 ) return;

    if( mDrawState.dirtyFlags.vtxBinding )
    {
        applyVtxBinding( mDrawState.vtxBinding );
    }

    if( mDrawState.dirtyFlags.vtxBufs )
    {
        applyVtxBuffers();
    }

    applyShader(
        mDrawState.vtxShader.get(), !!mDrawState.dirtyFlags.vtxShader,
        mDrawState.pxlShader.get(), !!mDrawState.dirtyFlags.pxlShader );

    // switch state buffer, and clear dirty flags
    mDrawState.dirtyFlags.u32 = 0;

    // apply render parameters
    if( !getRpDirtySet().empty() ) applyRenderParameters();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::resizeAutoDepthBuffer( const Vector2<uint32_t> & sz )
{
    GN_GUARD_SLOW;

    // Check surface size
    if( sz.x > mAutoDepthSize.x || sz.y > mAutoDepthSize.y || NULL == mAutoDepth )
    {
        // release old buffer
        safeRelease( mAutoDepth );

        // create a new buffer
        AutoComPtr<IDirect3DSurface9> newSurf;
        GN_DX_CHECK_R( mDevice->CreateDepthStencilSurface(
            sz.x, sz.y,
            mPresentParameters.AutoDepthStencilFormat,
            mPresentParameters.MultiSampleType, mPresentParameters.MultiSampleQuality,
            mPresentParameters.Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL,
            &mAutoDepth, 0 ) );
    }

    // update D3D depth surface
    GN_DX_CHECK_R( mDevice->SetDepthStencilSurface( mAutoDepth ) );

    GN_UNGUARD_SLOW;
}

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

    if( mDrawState.dirtyFlags.vtxShader )
    {
        applyVtxShader( mDrawState.vtxShader.get() );
    }

    if( mDrawState.dirtyFlags.pxlShader )
    {
        applyPxlShader( mDrawState.pxlShader.get() );
    }

    // switch state buffer, and clear dirty flags
    mDrawState.dirtyFlags.u32 = 0;

    GN_UNGUARD_SLOW;
}

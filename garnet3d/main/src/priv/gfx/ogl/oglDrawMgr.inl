//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::updateDrawState( size_t baseVtx )
{
    GN_GUARD_SLOW;

    if( 0 == mCurrentDrawState.dirtyFlags.u32 ) return;

    if( mCurrentDrawState.dirtyFlags.vtxBinding )
    {
        updateVtxBinding();
    }

    if( mCurrentDrawState.dirtyFlags.vtxBuf )
    {
        updateVtxBufState( baseVtx );
    }

    if( mCurrentDrawState.dirtyFlags.vtxShader || mCurrentDrawState.dirtyFlags.pxlShader )
    {
        updateShaderState();
    }

    // clear dirty flags
    mCurrentDrawState.dirtyFlags.u32 = 0;

    // replicate to last state
    mLastDrawState = mCurrentDrawState;

    GN_UNGUARD_SLOW;
}

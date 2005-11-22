//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::updateVtxBufState()
{
    GN_GUARD_SLOW;

    if( 0 == mVtxBufState.dirtyFlags.u32 ) return;

    GN_ASSERT( mVtxBindings.validHandle( mVtxBufState.vtxBinding ) );
    GN_ASSERT( mVtxBindings[mVtxBufState.vtxBinding].decl );

    if( mVtxBufState.dirtyFlags.vtxBinding )
    {
        GN_DX_CHECK( mDevice->SetVertexDeclaration( mVtxBindings[mVtxBufState.vtxBinding].decl ) );
    }

    if( mVtxBufState.dirtyFlags.vtxBufs )
    {
        updateVtxBufs();
    }

    // switch state buffer, and clear dirty flags
    mVtxBufState.dirtyFlags.u32 = 0;

    GN_UNGUARD_SLOW;
}

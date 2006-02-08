//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::applyVtxBinding( uint32_t id )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mVtxBindings.validHandle( id ) );
    GN_ASSERT( mVtxBindings[id].decl );

    GN_DX_CHECK( mDevice->SetVertexDeclaration( mVtxBindings[id].decl ) );

    GN_UNGUARD_SLOW;
}

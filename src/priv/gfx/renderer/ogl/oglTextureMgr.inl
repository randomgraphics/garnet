//
//
// ----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::chooseClientTextureStage( uint32_t stage ) const
{
    GN_GUARD_SLOW;
    GN_ASSERT( stage < getCaps(CAPS_MAX_TEXTURE_STAGES) );
    GN_ASSERT( glClientActiveTextureARB );
    GN_OGL_CHECK( glClientActiveTextureARB( GL_TEXTURE0_ARB + stage ) );
    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::chooseTextureStage( uint32_t stage ) const
{
    GN_GUARD_SLOW;
    GN_ASSERT( stage < getCaps(CAPS_MAX_TEXTURE_STAGES) );
    GN_ASSERT( glActiveTextureARB );
    GN_OGL_CHECK( glActiveTextureARB( GL_TEXTURE0_ARB + stage ) );
    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::disableTextureStage( uint32_t stage ) const
{
    GN_GUARD_SLOW;

    chooseTextureStage( stage );

    GN_OGL_CHECK( glDisable(GL_TEXTURE_2D) );
    GN_OGL_CHECK( glBindTexture(GL_TEXTURE_2D, 0) );

    if( GLEW_EXT_texture3D )
    {
        GN_OGL_CHECK( glDisable( GL_TEXTURE_3D_EXT ) );
        GN_OGL_CHECK( glBindTexture( GL_TEXTURE_3D_EXT, 0 ) );
    }

    if( GLEW_ARB_texture_cube_map )
    {
        GN_OGL_CHECK( glDisable( GL_TEXTURE_CUBE_MAP_ARB ) );
        GN_OGL_CHECK( glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, 0 ) );
    }

    GN_UNGUARD_SLOW;
}

/*
 * Setup individual render state
 */

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_ALPHA_TEST( GN::gfx::RenderStateValue val )
{
    if( val )
        glEnable( GL_ALPHA_TEST );
    else
        glDisable( GL_ALPHA_TEST );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_ALPHA_FUNC( GN::gfx::RenderStateValue val )
{
    // FIXME : 允许用户修改reference value
    GN_OGL_CHECK( glAlphaFunc( sRsv2OGL[val], 0.0f ) );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_BLENDING( GN::gfx::RenderStateValue val )
{
    if( val )
    {
        glEnable( GL_BLEND );
    }
    else
    {
        glDisable( GL_BLEND );
    }
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_BLEND_SRC( GN::gfx::RenderStateValue )
{
    GN_ASSERT_EX( 0, "your program should not reach here!" );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_BLEND_DST( GN::gfx::RenderStateValue )
{
    GN_ASSERT_EX( 0, "your program should not reach here!" );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_CULL_MODE( GN::gfx::RenderStateValue val )
{
    if( GN::gfx::RSV_CULL_NONE == val )
    {
        glDisable( GL_CULL_FACE );
    }
    else
    {
        glEnable( GL_CULL_FACE );
        GN_OGL_CHECK( glCullFace( sRsv2OGL[val] ) );
    }
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_FOG( GN::gfx::RenderStateValue val )
{
    if( val )
        glEnable( GL_FOG );
    else
        glDisable( GL_FOG );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE void
sSet_LIGHTING( GN::gfx::RenderStateValue val )
{
    if( val )
        glEnable( GL_LIGHTING );
    else
        glDisable( GL_LIGHTING );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_DEPTH_TEST( GN::gfx::RenderStateValue val )
{
    if( val )
        glEnable( GL_DEPTH_TEST );
    else
        glDisable( GL_DEPTH_TEST );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_DEPTH_FUNC( GN::gfx::RenderStateValue val )
{
    GN_OGL_CHECK( glDepthFunc( sRsv2OGL[val] ) );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_DEPTH_WRITE( GN::gfx::RenderStateValue val )
{
    GN_OGL_CHECK( glDepthMask( (GLboolean)val ) );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_FILL_MODE( GN::gfx::RenderStateValue val )
{
    GN_OGL_CHECK( glPolygonMode( GL_FRONT_AND_BACK, sRsv2OGL[val] ) );
}


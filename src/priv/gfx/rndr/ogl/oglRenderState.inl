/*
 * Setup individual render state
 */

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_ALPHA_TEST( int val )
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
void sSet_ALPHA_FUNC( int )
{
    GN_ASSERT_EX( 0, "your program should not reach here!" );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_ALPHA_REF( int )
{
    GN_ASSERT_EX( 0, "your program should not reach here!" );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_BLENDING( int val )
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
void sSet_BLEND_SRC( int )
{
    GN_ASSERT_EX( 0, "your program should not reach here!" );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_BLEND_DST( int )
{
    GN_ASSERT_EX( 0, "your program should not reach here!" );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_COLOR0_WRITE( int val )
{
    GLboolean r = !!( val & 0x1 );
    GLboolean g = !!( val & 0x2 );
    GLboolean b = !!( val & 0x4 );
    GLboolean a = !!( val & 0x8 );
    glColorMask( r, g, b, a );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_CULL_MODE( int val )
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
void sSet_DEPTH_TEST( int val )
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
void sSet_DEPTH_FUNC( int val )
{
    GN_OGL_CHECK( glDepthFunc( sRsv2OGL[val] ) );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_DEPTH_WRITE( int val )
{
    GN_OGL_CHECK( glDepthMask( (GLboolean)val ) );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_FILL_MODE( int val )
{
    GN_OGL_CHECK( glPolygonMode( GL_FRONT_AND_BACK, sRsv2OGL[val] ) );
}

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void sSet_FOG( int val )
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
sSet_LIGHTING( int val )
{
    if( val )
        glEnable( GL_LIGHTING );
    else
        glDisable( GL_LIGHTING );
}

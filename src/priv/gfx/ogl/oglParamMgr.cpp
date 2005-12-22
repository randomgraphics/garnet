#include "pch.h"
#include "oglRenderer.h"

//
// matrix computation
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::gfx::OGLRenderer::composePerspectiveMatrix(
    Matrix44f & result,
    float fovy,
    float ratio,
    float znear,
    float zfar ) const
{
    return result.perspectiveOGL( fovy, ratio, znear, zfar );
}
//
GN::Matrix44f &
GN::gfx::OGLRenderer::composeOrthoMatrix(
    Matrix44f & result,
    float left,
    float bottom,
    float width,
    float height,
    float znear,
    float zfar ) const
{
    return result.orthoOGL(
        left,
        left+width,
        bottom,
        bottom+height,
        znear,
        zfar );
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::paramDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    setAllRpDirty();

    // success
    return true;

    GN_UNGUARD;
}


// *****************************************************************************
// private function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::applyRenderParameters()
{
    GN_GUARD;

    const std::set<RenderParameterType> & dirtySet = getRpDirtySet();

    GN_ASSERT( !dirtySet.empty() );

    std::set<RenderParameterType>::const_iterator i, e = dirtySet.end();

    for( i = dirtySet.begin(); i != e; ++i )
    {
        const RenderParameterValue & rpv = getRenderParameter( *i );

        switch( *i )
        {
            case RPT_TRANSFORM_WORLD :
            case RPT_TRANSFORM_VIEW :
                {
                    GN_ASSERT( 16 == rpv.count );
                    const Matrix44f * world = (const Matrix44f * )getRenderParameter( RPT_TRANSFORM_WORLD ).valueFloats;
                    const Matrix44f * view = (const Matrix44f * )getRenderParameter( RPT_TRANSFORM_VIEW ).valueFloats;
                    glMatrixMode( GL_MODELVIEW );
                    glLoadMatrixf( Matrix44f::transpose(*view**world)[0] );
                }
                break;

            case RPT_TRANSFORM_PROJ :
                {
                    GN_ASSERT( 16 == rpv.count );
                    const Matrix44f * proj = (const Matrix44f * )rpv.valueFloats;
                    glMatrixMode( GL_PROJECTION );
                    glLoadMatrixf( Matrix44f::transpose(*proj)[0] );
                }
                break;

            case RPT_TRANSFORM_VIEWPORT :
                {
                    GN_ASSERT( 4 == rpv.count );
                    GLint x = (GLint)( rpv.valueFloats[0] * mCurrentRTSize.x );
                    GLint y = (GLint)( rpv.valueFloats[1] * mCurrentRTSize.y );
                    GLsizei w = (GLsizei)( rpv.valueFloats[2] * mCurrentRTSize.x );
                    GLsizei h = (GLsizei)( rpv.valueFloats[3] * mCurrentRTSize.y );
                    glViewport( x, y, w, h );
                    GNGFX_INFO( "Update viewport to: x(%d), y(%d), w(%d), h(%d)", x, y, w, h );
                }
                break;

            case RPT_LIGHT0_POSITION :
                // FIXME:  transform light position from world space to object space before
                //         sending into OpenGL pipeline.
                GN_ASSERT( 4 == rpv.count );
                glLightfv( GL_LIGHT0, GL_POSITION, rpv.valueFloats );
                break;

            case RPT_LIGHT0_DIFFUSE :
                GN_ASSERT( 4 == rpv.count );
                glLightfv( GL_LIGHT0, GL_DIFFUSE, rpv.valueFloats );
                break;

            case RPT_MATERIAL_DIFFUSE :
                GN_ASSERT( rpv.count == 4 );
                GN_OGL_CHECK( glColor4fv( rpv.valueFloats ) );
                GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, rpv.valueFloats ) );
                break;

            case RPT_MATERIAL_SPECULAR :
                GN_ASSERT( rpv.count == 4 );
                GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, rpv.valueFloats ) );
                break;

            default:
                GNGFX_ERROR( "Invalid render parameter type : %d", *i );
                GN_UNEXPECTED();
        }
    }

    GN_OGL_CHECK(;);

    // clear dirty set
    clearRpDirtySet();

    GN_UNGUARD;
}

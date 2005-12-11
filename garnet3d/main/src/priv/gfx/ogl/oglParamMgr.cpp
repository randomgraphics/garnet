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
            case RPT_LIGHT0_POSITION :
                GN_DO_ONCE( GNGFX_ERROR( "do not support render parameter %d", *i ) );
                break;

            case RPT_LIGHT0_DIFFUSE :
                GN_DO_ONCE( GNGFX_ERROR( "do not support render parameter %d", *i ) );
                break;

            case RPT_MATERIAL_DIFFUSE :
                GN_ASSERT( rpv.count == 4 );
                if( isParameterCheckEnabled() )
                {
                    if( rpv.count != 4 )
                    {
                        GNGFX_ERROR( "diffuse color must be 4-D float vector." );
                    }
                }
                GN_OGL_CHECK( glColor4fv( rpv.valueFloats ) );
                GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, rpv.valueFloats ) );
                break;

            case RPT_MATERIAL_SPECULAR :
                GN_DO_ONCE( GNGFX_ERROR( "do not support render parameter %d", *i ) );
                break;

            case RPT_TRANSFORM_WORLD :
                GN_DO_ONCE( GNGFX_ERROR( "do not support render parameter %d", *i ) );
                break;

            case RPT_TRANSFORM_VIEW :
                GN_DO_ONCE( GNGFX_ERROR( "do not support render parameter %d", *i ) );
                break;

            case RPT_TRANSFORM_PROJ :
                GN_DO_ONCE( GNGFX_ERROR( "do not support render parameter %d", *i ) );
                break;

            case RPT_TRANSFORM_VIEWPORT :
                GN_DO_ONCE( GNGFX_ERROR( "do not support render parameter %d", *i ) );
                break;

            default:
                GNGFX_ERROR( "Invalid render parameter type : %d", *i );
                GN_UNEXPECTED();
        }
    }

    // clear dirty set
    clearRpDirtySet();

    GN_UNGUARD;
}

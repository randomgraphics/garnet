#include "pch.h"
#include "oglRenderTargetMgr.h"
#include "oglTexture.h"
#include "oglGpu.h"

//static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");

// *****************************************************************************
// local functions
// *****************************************************************************

/*
//
// -----------------------------------------------------------------------------
static inline void
sCopyFrameBufferTo( const GN::gfx::RenderTargetTexture & rtt )
{
    using namespace GN;
    using namespace GN::gfx;

    GN_ASSERT( rtt.texture );

    const OGLTexture * tex = safeCastPtr<const OGLTexture>( rtt.texture );

    // get texture size
    uint32 sx, sy;
    tex->getMipSize<uint32>( rtt.level, &sx, &sy );

    // copy framebuffer to current (old) render target texture
    GLint currentTexID;
    switch( tex->getDesc().dim )
    {
        case TEXDIM_CUBE :
            GN_ASSERT( sx == sy );
            GN_OGL_CHECK(
                glGetIntegerv( GL_TEXTURE_BINDING_CUBE_MAP_ARB, &currentTexID ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_CUBE_MAP_ARB,
                    tex->getOGLTexture() ) );
            GN_OGL_CHECK(
                glCopyTexSubImage2D(
                    OGLTexture::sCubeface2OGL( rtt.face ), 0,
                    0, 0, 0, 0, sx, sx ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, currentTexID ) );
            break;

        case TEXDIM_2D :
            GN_OGL_CHECK(
                glGetIntegerv( GL_TEXTURE_BINDING_2D, &currentTexID ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_2D,
                    tex->getOGLTexture() ) );
            GN_OGL_CHECK(
                glCopyTexSubImage2D( GL_TEXTURE_2D, 0,
                    0, 0, 0, 0, sx, sy ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_2D, currentTexID ) );
            break;

        case TEXDIM_1D :
            GN_ASSERT( 1 == sy );
            GN_OGL_CHECK(
                glGetIntegerv( GL_TEXTURE_BINDING_1D, &currentTexID ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_1D,
                    tex->getOGLTexture() ) );
            GN_OGL_CHECK(
                glCopyTexSubImage1D( GL_TEXTURE_1D, 0, 0, 0, 0, sx ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_1D, currentTexID ) );
            break;

        default:
            GN_ERROR(sLogger)( "invalid texture type!" );
            return;
    }
}*/

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRTMgrCopyFrame::bind(
    const RenderTargetDesc & newrt,
    const RenderTargetDesc & oldrt,
    bool                     skipDirtyCheck )
{
    GN_GUARD_SLOW;

    GN_UNUSED_PARAM( newrt );
    GN_UNUSED_PARAM( oldrt );
    GN_UNUSED_PARAM( skipDirtyCheck );

    /*

    // bind color buffers
    uint32 count = min( (uint32)RenderContext::NUM_COLOR_RENDER_TARGETS, mGpu.caps().maxRenderTargets ) );
    for( uint32 i = 0; i < count; ++i )
    {
        const RenderTargetTexture * oldc = i < oldrt.count ? oldrt.crts[i] : NULL;
        const RenderTargetTexture * newc = i < newrt.count ? newrt.crts[i] : NULL;

        if( skipDirtyCheck || // if no dirty check, then do it.
            oldc != newc && // if oldc and newc are both NULL, then do nothing.
            // program reaches here, means at least one surface is _NOT_ null. So if one of them is NULL
            // or content of the two surfaces are different, we have to do the binding.
            ( NULL == oldc || NULL == newc || *oldc != *newc ) )
        {
            if( oldc )
            {
                GN_ASSERT( oldc->texture );
                sCopyFrameBufferTo( *oldc );
            }

            // update render target size
            if( 0 == i )
            {
                uint32 oldw = mWidth;
                uint32 oldh = mHeight;
                if( newc && newc->texture )
                {
                    newc->texture->getMipSize<uint32>( newc->level, &mWidth, &mHeight );
                }
                else
                {
                    // use default back buffer size
                    mWidth = mGpu.getDispDesc().width;
                    mHeight = mGpu.getDispDesc().height;
                }

                renderTargetSizeChanged = ( oldw != mWidth || oldh != mHeight );
            }
        }
    }

    // bind depth buffer
    const RenderTargetTexture & oldz = oldrt.zbuffer;
    const RenderTargetTexture & newz = newrt.zbuffer;
    if( oldz.texture && ( oldz != newz || skipDirtyCheck ) )
    {
        sCopyFrameBufferTo( oldz );
    }

    GN_OGL_CHECK( ; );

    */

    return true;

    GN_UNGUARD_SLOW;
}

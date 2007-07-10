#include "pch.h"
#include "oglRenderTargetMgr.h"
#include "oglTexture.h"
#include "oglRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN_INLINE void
sCopyFrameBufferTo( const GN::gfx::RenderTargetTexture & rtt )
{
    using namespace GN;
    using namespace GN::gfx;

    GN_ASSERT( rtt.texture );

    const OGLTexture * tex = safeCastPtr<const OGLTexture>( rtt.texture );

    // get texture size
    UInt32 sx, sy;
    tex->getMipSize<UInt32>( rtt.level, &sx, &sy );

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
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRTMgrCopyFrame::bind(
    const RenderTargetDesc & oldDesc,
    const RenderTargetDesc & newDesc,
    bool forceRebind,
    bool & needRebindViewport )
{
    GN_GUARD_SLOW;

    GN_ASSERT( oldDesc.valid() );
    GN_ASSERT( newDesc.valid() );

    needRebindViewport = false;

    // bind color buffers
    UInt32 count = min( (UInt32)newDesc.count, mRenderer.getCaps( CAPS_MAX_RENDER_TARGETS ) );
    if( 0 == count ) count = 1;
    for( UInt32 i = 0; i < count; ++i )
    {
        const RenderTargetTexture * oldc = i < oldDesc.count ? &oldDesc.cbuffers[i] : 0;
        const RenderTargetTexture * newc = i < newDesc.count ? &newDesc.cbuffers[i] : 0;

        if( forceRebind || // if "force" rebinding, then do it.
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
                UInt32 oldw = mWidth;
                UInt32 oldh = mHeight;
                if( newc && newc->texture )
                {
                    newc->texture->getMipSize<UInt32>( newc->level, &mWidth, &mHeight );
                }
                else
                {
                    // use default back buffer size
                    mWidth = mRenderer.getDispDesc().width;
                    mHeight = mRenderer.getDispDesc().height;
                }

                needRebindViewport = ( oldw != mWidth || oldh != mHeight );
            }
        }
    }

    // bind depth buffer
    const RenderTargetTexture & oldz = oldDesc.zbuffer;
    const RenderTargetTexture & newz = newDesc.zbuffer;
    if( oldz.texture && ( oldz != newz || forceRebind ) )
    {
        sCopyFrameBufferTo( oldz );
    }

    GN_OGL_CHECK( ; );

    GN_UNGUARD_SLOW;
}

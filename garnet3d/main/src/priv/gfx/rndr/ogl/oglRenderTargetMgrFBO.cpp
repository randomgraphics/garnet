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
static inline void sAttachRTT2FBO( const GN::gfx::RenderTargetTexture & rtt, GLenum attachpoint )
{
    using namespace GN;
    using namespace GN::gfx;

    GN_ASSERT( rtt.texture );

    const OGLTexture * tex = safeCast<const OGLTexture*>( rtt.texture );

    switch( tex->getDesc().dim )
    {
        case TEXDIM_1D :
            GN_OGL_CHECK( glFramebufferTexture1DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_1D,
                tex->getOGLTexture(),
                (GLint)rtt.level ) );
            break;

        case TEXDIM_2D :
            GN_OGL_CHECK( glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_2D,
                tex->getOGLTexture(),
                (GLint)rtt.level ) );
            break;

        case TEXDIM_3D :
            GN_OGL_CHECK( glFramebufferTexture3DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_2D,
                tex->getOGLTexture(),
                (GLint)rtt.level,
                (GLint)rtt.slice ) );
            break;

        case TEXDIM_CUBE :
            GN_OGL_CHECK( glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                (GLenum)( GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + rtt.face ),
                tex->getOGLTexture(),
                (GLint)rtt.level ) );
            break;

        default :
            GN_UNEXPECTED();
    }
}

// *****************************************************************************
// init/quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRTMgrFBO::init()
{
    GN_ASSERT( usable() );

    GN_ASSERT( 0 == mFbo );

    // create frame buffer object
    GN_OGL_CHECK_RV( glGenFramebuffersEXT( 1, &mFbo ), false );

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRTMgrFBO::quit()
{
    if( mAutoZ )
    {
        GN_OGL_CHECK( glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, 0 ) );
        GN_OGL_CHECK( glDeleteRenderbuffersEXT( 1, &mAutoZ ) );
        mAutoZ = 0;
    }

    if( mFbo )
    {
        GN_OGL_CHECK( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 ) );
        GN_OGL_CHECK( glDeleteFramebuffersEXT( 1, &mFbo ) );
        mFbo = 0;
    }

    clear();
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRTMgrFBO::bind(
    const RenderTargetDesc & oldDesc,
    const RenderTargetDesc & newDesc,
    bool forceRebind,
    bool & needRebindViewport )
{
    GN_GUARD_SLOW;

    // make sure no OGL errors before this function
    GN_OGL_CHECK( ; );

    GN_UNUSED_PARAM( oldDesc );
    GN_UNUSED_PARAM( forceRebind );

    GN_ASSERT( oldDesc.valid() );
    GN_ASSERT( newDesc.valid() );

    needRebindViewport = false;

    // special case for render to back buffer
    if( 0 == newDesc.count && 0 == newDesc.zbuffer.texture )
    {
        if( oldDesc.count > 0 || oldDesc.zbuffer.texture || forceRebind )
        {
            // unbind FBO, render to back buffer
            GN_OGL_CHECK( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 ) );
            GN_OGL_CHECK( glDrawBuffer( GL_BACK ) );
            GN_OGL_CHECK( glReadBuffer( GL_BACK ) );

            // update render target size
            UInt32 oldw = mWidth, oldh = mHeight;
            mWidth = mRenderer.getDispDesc().width;
            mHeight = mRenderer.getDispDesc().height;

            // rebind viewport, if render target size changes.
            needRebindViewport = oldw != mWidth || oldh != mHeight;
        }
        return;
    }

    // bind FBO, if last context is "render-to-back-buffer"
    if( 0 == oldDesc.count && 0 == oldDesc.zbuffer.texture )
    {
        GN_OGL_CHECK( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFbo ) );
    }

    // store old RT size
    UInt32 oldw = mWidth, oldh = mHeight;

    if( 0 == newDesc.count )
    {
        // render to depth texture only. So take depth texture size as render target size.
        GN_ASSERT( newDesc.zbuffer.texture );
        newDesc.zbuffer.texture->getMipSize<UInt32>( newDesc.cbuffers[0].level, &mWidth, &mHeight );
        if( oldDesc.count > 0 || 0 == oldDesc.zbuffer.texture )
        {
            GN_OGL_CHECK( glDrawBuffer( GL_NONE ) );
            GN_OGL_CHECK( glReadBuffer( GL_NONE ) );
        }
    }
    else
    {
        // take color buffer 0 size as render target size.
        GN_ASSERT( newDesc.cbuffers[0].texture );
        newDesc.cbuffers[0].texture->getMipSize<UInt32>( newDesc.cbuffers[0].level, &mWidth, &mHeight );

        if( oldDesc.count != newDesc.count )
        {
            static GLenum buffers[] =
            {
                GL_COLOR_ATTACHMENT0_EXT,
                GL_COLOR_ATTACHMENT1_EXT,
                GL_COLOR_ATTACHMENT2_EXT,
                GL_COLOR_ATTACHMENT3_EXT,
                GL_COLOR_ATTACHMENT4_EXT,
                GL_COLOR_ATTACHMENT5_EXT,
                GL_COLOR_ATTACHMENT6_EXT,
                GL_COLOR_ATTACHMENT7_EXT,
                GL_COLOR_ATTACHMENT8_EXT,
                GL_COLOR_ATTACHMENT9_EXT,
                GL_COLOR_ATTACHMENT10_EXT,
                GL_COLOR_ATTACHMENT11_EXT,
                GL_COLOR_ATTACHMENT12_EXT,
                GL_COLOR_ATTACHMENT13_EXT,
                GL_COLOR_ATTACHMENT14_EXT,
                GL_COLOR_ATTACHMENT15_EXT,
            };
            GN_ASSERT( newDesc.count <= 16 );
            GN_OGL_CHECK( glDrawBuffersARB( newDesc.count, buffers ) );
        }
    }

    // rebind viewport, if render target size changes.
    needRebindViewport = oldw != mWidth || oldh != mHeight;

    // bind color buffers
    for( GLenum i = 0; i < newDesc.count; ++i )
    {
        sAttachRTT2FBO( newDesc.cbuffers[i], GL_COLOR_ATTACHMENT0_EXT + i );
    }

    // bind depth buffer
    if( newDesc.zbuffer.texture )
    {
        // attach depth texture to FBO
        if( newDesc.zbuffer.texture != oldDesc.zbuffer.texture || forceRebind )
        {
            sAttachRTT2FBO( newDesc.zbuffer, GL_DEPTH_ATTACHMENT_EXT );
        }
    }
    else
    {
        // generate auto Z buffer
        if( mWidth > mAutoZSize.x || mHeight > mAutoZSize.y )
        {
            // delete old z buffer
            if( mAutoZ ) GN_OGL_CHECK( glDeleteRenderbuffersEXT( 1, &mAutoZ ) );

            // create new z buffer
            GN_OGL_CHECK_DO( glGenRenderbuffersEXT( 1, &mAutoZ ),
                GN_UNEXPECTED();
                GN_ERROR(sLogger)( L"fail to generate automatic z buffer" );
                return; );

            // update size of auto z buffer
            mAutoZSize.set( mWidth, mHeight );

            // initialize new z buffer
            // TODO: choose appropriate depth format
            GN_OGL_CHECK( glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mAutoZ ) );
            GN_OGL_CHECK( glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, mWidth, mHeight ) );

            GN_OGL_CHECK( glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mAutoZ ) );
        }
        else if( oldDesc.zbuffer.texture || forceRebind )
        {
            GN_OGL_CHECK( glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mAutoZ ) );
        }
    }

    GN_OGL_CHECK( ; );

    // verify completness of frame buffer
#if GN_DEBUG_BUILD
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    GN_ASSERT( GL_FRAMEBUFFER_COMPLETE_EXT == status );
#endif

    GN_UNGUARD_SLOW;
}

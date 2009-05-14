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

    const OGLTexture * tex = (const OGLTexture*)rtt.texture.get();

    switch( tex->getOGLTarget() )
    {
        case GL_TEXTURE_1D :
            GN_OGL_CHECK( glFramebufferTexture1DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_1D,
                tex->getOGLTexture(),
                (GLint)rtt.level ) );
            break;

        case GL_TEXTURE_2D :
            GN_OGL_CHECK( glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_2D,
                tex->getOGLTexture(),
                (GLint)rtt.level ) );
            break;

        case GL_TEXTURE_3D_EXT :
            GN_OGL_CHECK( glFramebufferTexture3DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_2D,
                tex->getOGLTexture(),
                (GLint)rtt.level,
                (GLint)rtt.slice ) );
            break;

        case GL_TEXTURE_CUBE_MAP_ARB :
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

//
//
// -----------------------------------------------------------------------------
static inline bool
sCheckRenderTargetDesc( const GN::gfx::OGLRenderTargetDesc & desc )
{
    for( size_t i = 0; i < desc.crts.size(); ++i )
    {
        if( !desc.crts[i].texture )
        {
            GN_ERROR(sLogger)( "NULL color render targets in render target array is not allowed." );
            return false;
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
static inline bool
sIsRenderingToBackBuffer( const GN::gfx::OGLRenderTargetDesc & desc )
{
    return 0 == desc.crts.size() && 0 == desc.dsrt.texture;
}

//
//
// -----------------------------------------------------------------------------
static inline bool
sIsRenderingToDepthTextureOnly( const GN::gfx::OGLRenderTargetDesc & desc )
{
    return 0 == desc.crts.size() && 0 != desc.dsrt.texture;
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
bool GN::gfx::OGLRTMgrFBO::bind(
    const OGLRenderTargetDesc & oldrt,
    const OGLRenderTargetDesc & newrt,
    bool                        skipDirtyCheck,
    bool                      & renderTargetSizeChanged )
{
    GN_GUARD_SLOW;

    // make sure no OGL errors before this function
    GN_OGL_CHECK( ; );

    // render target size is unchanged by default.
    renderTargetSizeChanged = false;

    // make new render target description is valid.
    GN_ASSERT( sCheckRenderTargetDesc( oldrt ) );
    if( !sCheckRenderTargetDesc( newrt ) ) return false;

    // special case for render to back buffer
    if( sIsRenderingToBackBuffer( newrt ) )
    {
        if( skipDirtyCheck || !sIsRenderingToBackBuffer( oldrt ) )
        {
            // disable FBO, render to back buffer
            GN_OGL_CHECK( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 ) );
            GN_OGL_CHECK( glDrawBuffer( GL_BACK ) );
            GN_OGL_CHECK( glReadBuffer( GL_BACK ) );

            // update render target size
            UInt32 oldw = mWidth, oldh = mHeight;
            mWidth = mRenderer.getDispDesc().width;
            mHeight = mRenderer.getDispDesc().height;

            // rebind viewport, if render target size changes.
            renderTargetSizeChanged = oldw != mWidth || oldh != mHeight;
        }
        return true;
    }

    // enable FBO, if last context is "render-to-back-buffer"
    if( skipDirtyCheck || sIsRenderingToBackBuffer( oldrt ) )
    {
        GN_OGL_CHECK( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFbo ) );
    }

    // store old RT size
    UInt32 oldw = mWidth, oldh = mHeight;

    if( 0 == newrt.crts.size() )
    {
        // render to depth texture only. So take depth texture size as render target size.

        GN_ASSERT( sIsRenderingToDepthTextureOnly( newrt ) );

        newrt.dsrt.texture->getMipSize<UInt32>( newrt.crts[0].level, &mWidth, &mHeight );

        if( skipDirtyCheck || !sIsRenderingToDepthTextureOnly( oldrt ) )
        {
            GN_OGL_CHECK( glDrawBuffer( GL_NONE ) );
            GN_OGL_CHECK( glReadBuffer( GL_NONE ) );
        }
    }
    else
    {
        // Render to both color and depth texture.
        // Take size of color buffer 0 as render target size.

        GN_ASSERT( newrt.crts[0].texture );
        newrt.crts[0].texture->getMipSize<UInt32>( newrt.crts[0].level, &mWidth, &mHeight );

        // setup color buffer attachments
        if( skipDirtyCheck || oldrt.crts.size() != newrt.crts.size() )
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
            GN_ASSERT( newrt.crts.size() <= 16 );
            GN_OGL_CHECK( glDrawBuffersARB( newrt.crts.size(), buffers ) );
        }
    }

    // update render target size changed flag
    renderTargetSizeChanged = oldw != mWidth || oldh != mHeight;

    // bind color buffers
    for( GLenum i = 0; i < newrt.crts.size(); ++i )
    {
        if( skipDirtyCheck || newrt.crts[i] != oldrt.crts[i] )
        {
            sAttachRTT2FBO( newrt.crts[i], GL_COLOR_ATTACHMENT0_EXT + i );
        }
    }

    // bind depth buffer
    if( newrt.dsrt.texture )
    {
        // attach depth texture to FBO
        if( skipDirtyCheck || newrt.dsrt.texture != oldrt.dsrt.texture )
        {
            sAttachRTT2FBO( newrt.dsrt, GL_DEPTH_ATTACHMENT_EXT );
        }
    }
    else if( mWidth > mAutoZSize.x || mHeight > mAutoZSize.y )
    {
        // bind to auto-z buffer, with larger size.

        // delete old z buffer
        if( mAutoZ ) GN_OGL_CHECK( glDeleteRenderbuffersEXT( 1, &mAutoZ ) );

        // create new z buffer
        GN_OGL_CHECK_DO( glGenRenderbuffersEXT( 1, &mAutoZ ),
            GN_UNEXPECTED();
            GN_ERROR(sLogger)( L"fail to generate automatic z buffer" );
            return false; );

        // update size of auto z buffer
        mAutoZSize.set( mWidth, mHeight );

        // initialize new z buffer
        // TODO: choose appropriate depth format
        GN_OGL_CHECK( glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mAutoZ ) );
        GN_OGL_CHECK( glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, mWidth, mHeight ) );

        GN_OGL_CHECK( glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mAutoZ ) );
    }
    else if( skipDirtyCheck || oldrt.dsrt.texture )
    {
        // bind current auto-Z buffer.
        GN_OGL_CHECK( glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mAutoZ ) );
    }

    GN_OGL_CHECK( ; );

    // verify completness of frame buffer
#if GN_BUILD_DEBUG
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    GN_ASSERT( GL_FRAMEBUFFER_COMPLETE_EXT == status );
#endif

    return true;

    GN_UNGUARD_SLOW;
}

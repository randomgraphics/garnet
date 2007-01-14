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
static inline void sAttachRTT2FBO( GLenum fbo, const GN::gfx::RenderTargetTexture & rtt, GLenum attachpoint )
{
    using namespace GN;
    using namespace GN::gfx;

    GN_ASSERT( fbo );

    GN_ASSERT( rtt.texture );

    const OGLTexture * tex = safeCast<const OGLTexture*>( rtt.texture );

    switch( tex->getDesc().type )
    {
        case TEXTYPE_1D :
            GN_OGL_CHECK( glFramebufferTexture1DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_1D,
                tex->getOGLTexture(),
                rtt.level ) );
            break;

        case TEXTYPE_2D :
            GN_OGL_CHECK( glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_2D,
                tex->getOGLTexture(),
                rtt.level ) );
            break;

        case TEXTYPE_3D :
            GN_OGL_CHECK( glFramebufferTexture3DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_2D,
                tex->getOGLTexture(),
                rtt.level,
                rtt.slice ) );
            break;

        case TEXTYPE_CUBE :
            GN_OGL_CHECK( glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER_EXT,
                attachpoint,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + rtt.face,
                tex->getOGLTexture(),
                rtt.level ) );
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

    // bind this frame buffer object
    GN_OGL_CHECK_RV( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFbo ), false );

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

    GN_UNUSED_PARAM( oldDesc );
    GN_UNUSED_PARAM( forceRebind );

    GN_ASSERT( oldDesc.valid() );
    GN_ASSERT( newDesc.valid() );

    needRebindViewport = false;

    // special case for render to back buffer
    if( 0 == newDesc.count )
    {
        // render to back buffer
        GN_OGL_CHECK( glDrawBuffer( GL_BACK ) );

        // get back buffer size
        mWidth = mRenderer.getDispDesc().width;
        mHeight = mRenderer.getDispDesc().height;
    }
    else
    {
        // render to texture(s)
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

        // get render target size
        GN_ASSERT( newDesc.cbuffers[0].texture );
        newDesc.cbuffers[0].texture->getMipSize<UInt32>( newDesc.cbuffers[0].level, &mWidth, &mHeight );
    }

    // bind color buffers
    for( GLenum i = (0==newDesc.count)?1:0; i < newDesc.count; ++i )
    {
        sAttachRTT2FBO( mFbo, newDesc.cbuffers[i], GL_COLOR_ATTACHMENT0_EXT + i );
        needRebindViewport = true;
    }

    // bind depth buffer
    if( newDesc.zbuffer.texture )
    {
        // attach depth texture to FBO
        sAttachRTT2FBO( mFbo, newDesc.zbuffer, GL_DEPTH_ATTACHMENT_EXT );
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

            // initialize new z buffer
            // TODO: choose appropriate depth format
            GN_OGL_CHECK( glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mAutoZ ) );
            GN_OGL_CHECK( glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, mWidth, mHeight ) );
        }

        // attach auto z buffer to FBO
        GN_OGL_CHECK( glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mAutoZ ) );
    }

    GN_OGL_CHECK( ; );

    // verify completness of frame buffer
    GN_ASSERT( GL_FRAMEBUFFER_COMPLETE_EXT == glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) );

    GN_UNGUARD_SLOW;
}

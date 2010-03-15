#include "pch.h"
#include "oglRenderTargetMgr.h"
#include "oglTexture.h"
#include "oglGpu.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.OGL");

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

    const OGLTexture * tex = (const OGLTexture*)rtt.texture.Get();

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

// *****************************************************************************
// init/quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRTMgrFBO::Init()
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
void GN::gfx::OGLRTMgrFBO::Quit()
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

    Clear();
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRTMgrFBO::bind(
    const RenderTargetDesc & oldrt,
    const RenderTargetDesc & newrt,
    bool                     skipDirtyCheck )
{
    GN_GUARD_SLOW;

    // make sure no OGL errors before this function
    GN_OGL_CHECK( ; );

    // make new render target description is valid.
    GN_ASSERT( oldrt.valid() );
    if( !newrt.valid() ) return false;

    // check for redundancy
    if( !skipDirtyCheck && oldrt == newrt )
    {
        return true;
    }

    // special case for render to back buffer
    if( 0 == newrt.colortargets.Size() && 0 == newrt.depthstencil.texture )
    {
        // disable FBO, render to back buffer
        GN_OGL_CHECK( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 ) );
        GN_OGL_CHECK( glDrawBuffer( GL_BACK ) );
        GN_OGL_CHECK( glReadBuffer( GL_BACK ) );

        // update render target size
        mRenderTargetSize.x = mGpu.getDispDesc().width;
        mRenderTargetSize.y = mGpu.getDispDesc().height;

        return true;
    }

    // enable FBO
    GN_OGL_CHECK( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFbo ) );

    // setup color buffers
    if( newrt.colortargets.Size() > 0 )
    {
        // setup color buffer attachments
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
        GN_ASSERT( newrt.colortargets.Size() <= 16 );
        GN_OGL_CHECK( glDrawBuffersARB( (GLsizei)newrt.colortargets.Size(), buffers ) );

        // bind color buffers
        for( GLenum i = 0; i < newrt.colortargets.Size(); ++i )
        {
            sAttachRTT2FBO( newrt.colortargets[i], GL_COLOR_ATTACHMENT0_EXT + i );
        }

        // update color render target size
        newrt.colortargets[0].texture->getMipSize<UInt32>(
            newrt.colortargets[0].level,
            &mRenderTargetSize.x,
            &mRenderTargetSize.y );
    }
    else
    {
        // depth only rendering
        GN_ASSERT( newrt.depthstencil.texture );

        GN_OGL_CHECK( glDrawBuffer( GL_NONE ) );
        GN_OGL_CHECK( glReadBuffer( GL_NONE ) );

        // update color render target size
        newrt.depthstencil.texture->getMipSize<UInt32>(
            newrt.depthstencil.level,
            &mRenderTargetSize.x,
            &mRenderTargetSize.y );
    }

    // bind depth buffer
    if( newrt.depthstencil.texture )
    {
        sAttachRTT2FBO( newrt.depthstencil, GL_DEPTH_ATTACHMENT_EXT );
    }
    else
    {
        if( mRenderTargetSize.x > mAutoZSize.x || mRenderTargetSize.y > mAutoZSize.y )
        {
            //
            // Current auto-z buffer is smaller than color render targets. Need to enlarge it.
            //
            UInt32 newWidth  = math::GetMax( mRenderTargetSize.x, mAutoZSize.x );
            UInt32 newHeight = math::GetMax( mRenderTargetSize.y, mAutoZSize.y );

            // delete old z buffer
            if( mAutoZ )
            {
                GN_OGL_CHECK( glDeleteRenderbuffersEXT( 1, &mAutoZ ) );
                mAutoZSize.Set( 0, 0 );
            }

            // create new z buffer
            GN_OGL_CHECK_DO( glGenRenderbuffersEXT( 1, &mAutoZ ),
                GN_UNEXPECTED();
                GN_ERROR(sLogger)( L"fail to generate automatic z buffer" );
                return false; );

            // create a new z buffer as large as current render target
            // TODO: choose appropriate depth format
            GN_OGL_CHECK( glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mAutoZ ) );
            GN_OGL_CHECK( glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, newWidth, newHeight ) );

            // update size of auto z buffer
            mAutoZSize.Set( newWidth, newHeight );
        }

        // bind auto-Z buffer to OpenGL
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

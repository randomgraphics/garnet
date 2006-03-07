#include "pch.h"
#include "oglRenderer.h"
#include "oglTexture.h"

#if GN_MSVC
#pragma warning(disable:4100) // unused parameters
#endif

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::renderTargetDeviceCreate()
{
    GN_GUARD;

    // check multiple render target support
    if( getCaps(CAPS_MAX_RENDER_TARGETS) > MAX_RENDER_TARGETS )
    {
        GN_ERROR( "Sorry, we currently do not support more then %d simultaneous render targets.", MAX_RENDER_TARGETS );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::renderTargetDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // make sure MRT caps does not exceed maximum allowance value
    GN_ASSERT( getCaps(CAPS_MAX_RENDER_TARGETS) <= MAX_RENDER_TARGETS );

    // store old RT data
    RenderTargetTextureDesc oldRT[MAX_RENDER_TARGETS], oldDepth;
    for( int i = 0; i < MAX_RENDER_TARGETS; ++i )
    {
        oldRT[i] = mCurrentRTs[i];
    }
    oldDepth = mCurrentDepth;

    //set default render target
    mCurrentRTSize.set( getDispDesc().width, getDispDesc().height );
    for( int i = 0; i < MAX_RENDER_TARGETS; ++i )
    {
        mCurrentRTs[i].tex = 0;
    }
    setViewport( getViewport() );

    // (re)apply render targets
    for( size_t i = 0; i < getCaps(CAPS_MAX_RENDER_TARGETS); ++i )
    {
        setRenderTarget( i, oldRT[i].tex, oldRT[i].level, oldRT[i].face );
    }
    setRenderDepth( oldDepth.tex, oldDepth.level, oldDepth.face );

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::setRenderTarget(
    size_t index, const Texture * tex, size_t level, TexFace face )
{
    GN_GUARD_SLOW;

    if( isParameterCheckEnabled() )
    {
        if( index >= getCaps(CAPS_MAX_RENDER_TARGETS) )
        {
            GN_ERROR( "invalid render target index!" );
            return;
        }
        // make sure target texture is a RTT
        if( tex && !(TEXUSAGE_RENDER_TARGET & tex->getDesc().usage) )
        {
            GN_ERROR( "Only texture with TEXUSAGE_RENDER_TARGET usage can be used as render target." );
            return;
        }
    }
    GN_ASSERT( index < getCaps(CAPS_MAX_RENDER_TARGETS) );
    GN_ASSERT( !tex || (TEXUSAGE_RENDER_TARGET & tex->getDesc().usage) );

    RenderTargetTextureDesc & rttd = mCurrentRTs[index];

    // skip redundant call
    if( rttd.equal( tex, level, face ) ) return;

    if( rttd.tex )
    {
        const OGLTexture * gltex = safeCast<const OGLTexture*>(rttd.tex);

        // get texture size
        Vector3<uint32_t> mipsize = gltex->getMipSize( level );

        GLint oldtex;

        // copy framebuffer to current render target texture
        TexType tt = rttd.tex->getDesc().type;
        if( TEXTYPE_CUBE == tt )
        {
            GN_ASSERT( mipsize.x == mipsize.y );
            GN_OGL_CHECK( glGetIntegerv( GL_TEXTURE_BINDING_CUBE_MAP_ARB, &oldtex ) );
            GN_OGL_CHECK( glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, gltex->getOGLTexture() ) );
            GN_OGL_CHECK( glCopyTexImage2D(
                OGLTexture::sCubeface2OGL(face), (GLint)level,
                gltex->getOGLInternalFormat(), 0, 0, mipsize.x, mipsize.x, 0 ) );
            GN_OGL_CHECK( glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, oldtex ) );
        }
        else if( TEXTYPE_2D == tt || TEXTYPE_1D == tt )
        {
            GN_OGL_CHECK( glGetIntegerv( GL_TEXTURE_BINDING_2D, &oldtex ) );
            GN_OGL_CHECK( glBindTexture( GL_TEXTURE_2D, gltex->getOGLTexture() ) );
            GN_OGL_CHECK( glCopyTexImage2D(
                GL_TEXTURE_2D, (GLint)level,
                gltex->getOGLInternalFormat(), 0, 0, mipsize.x, mipsize.y, 0 ) );
            GN_OGL_CHECK( glBindTexture( GL_TEXTURE_2D, oldtex ) );
        }
        else
        {
            GN_ERROR( "invalid texture type!" );
            return;
        }
    }

    // update render target data member
    if( tex ) tex->incref();
    if( rttd.tex ) rttd.tex->decref();
    rttd.tex = tex;
    rttd.level = level;
    rttd.face = face;

    // update render target size
    if( 0 == index )
    {
        if( tex )
        {
            tex->getMipSize( level, &mCurrentRTSize.x, &mCurrentRTSize.y );
        }
        else
        {
            // use default back buffer size
            mCurrentRTSize.x = getDispDesc().width;
            mCurrentRTSize.y = getDispDesc().height;
        }

        // Because viewport is using relative coordinates based on render target size,
        // so here we have to re-apply the viewport.
        setViewport( getViewport() );
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::setRenderDepth(
    const Texture * tex, size_t level, TexFace face )
{
    GN_GUARD_SLOW;

    // make sure target texture is a depth texture
    if( isParameterCheckEnabled() )
    {
        if( tex && !(TEXUSAGE_DEPTH & tex->getDesc().usage) )
        {
            GN_ERROR( "Only texture with TEXUSAGE_DEPTH usage can be used as depth texture." );
            return;
        }
    }
    GN_ASSERT( !tex || (TEXUSAGE_DEPTH & tex->getDesc().usage) );

    // skip redundant call
    if( mCurrentDepth.equal( tex, level, face ) ) return;

    if( mCurrentDepth.tex )
    {
        const OGLTexture * gltex = safeCast<const OGLTexture*>(mCurrentDepth.tex);

        // get texture size
        GLint sx, sy;
        GN_OGL_CHECK( glGetTexLevelParameteriv(
                gltex->getOGLTarget(), (GLint)level, GL_TEXTURE_WIDTH, &sx ) );
        GN_OGL_CHECK( glGetTexLevelParameteriv(
                gltex->getOGLTarget(), (GLint)level, GL_TEXTURE_HEIGHT, &sy ) );

        // copy framebuffer to current render target texture
        TexType tt = mCurrentDepth.tex->getDesc().type;
        if( TEXTYPE_CUBE == tt )
        {
            GN_ASSERT( sx == sy );
            GN_OGL_CHECK( glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, gltex->getOGLTexture() ) );
            GN_OGL_CHECK( glCopyTexImage2D(
                OGLTexture::sCubeface2OGL(face), (GLint)level,
                gltex->getOGLInternalFormat(), 0, 0, sx, sx, 0 ) );
        }
        else if( TEXTYPE_2D == tt || TEXTYPE_1D == tt )
        {
            GN_OGL_CHECK( glBindTexture( GL_TEXTURE_2D, gltex->getOGLTexture() ) );
            GN_OGL_CHECK( glCopyTexImage2D(
                GL_TEXTURE_2D, (GLint)level,
                gltex->getOGLInternalFormat(), 0, 0, sx, sy, 0 ) );
        }
        else
        {
            GN_ERROR( "invalid texture type!" );
            return;
        }
    }

    // update render target data member
    if( tex ) tex->incref();
    if( mCurrentDepth.tex ) mCurrentDepth.tex->decref();
    mCurrentDepth.tex = tex;
    mCurrentDepth.level = level;
    mCurrentDepth.face = face;

    GN_UNGUARD_SLOW;
}

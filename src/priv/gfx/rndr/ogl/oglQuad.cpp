#include "pch.h"
#include "oglQuad.h"
#include "oglRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLQuad::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLQuad, () );

    // create vertex buffer
    GN_ASSERT( !mVtxBuf );
    mVtxBuf = new QuadVertex[MAX_QUADS*4];
    if( !mVtxBuf )
    {
        GN_ERROR(sLogger)( "out of memory!" );
        return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLQuad::quit()
{
    GN_GUARD;

    safeDeleteArray( mVtxBuf );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLQuad::drawQuads(
    BitFields options,
    const float * positions, size_t posStride,
    const float * texcoords, size_t texStride,
    const UInt32 * colors, size_t clrStride,
    size_t count )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( 0 == positions )
    {
        GN_ERROR(sLogger)( "Positions can't be NULL!" );
        return;
    }

    if( 0 == posStride )
    {
        GN_ERROR(sLogger)( "Position stride can't be zero!" );
        return;
    }

    GN_ASSERT( mNextQuad < MAX_QUADS );

    // handle large amount of array
    while( count + mNextQuad > MAX_QUADS )
    {
        size_t n = MAX_QUADS - mNextQuad;
        GN_ASSERT( n > 0 );
        drawQuads( options, positions, posStride, texcoords, texStride, colors, clrStride, n );
        positions = (const float*)( ((const UInt8*)positions) + n * posStride * 4 );
        if( texcoords )
            texcoords = (const float*)( ((const UInt8*)texcoords) + n * texStride * 4 );
        if( colors )
            colors = (const UInt32*)( ((const UInt8*)colors) + n * clrStride * 4 );
        count -= n;
    }

#define BGRA2RGBA( x ) ( \
          ( (x)&0xFF000000) | \
          (((x)&0x00FF0000)>>16) | \
          ( (x)&0x0000FF00) | \
          (((x)&0x000000FF)<<16) )

    // fill vertex data
    GN_ASSERT( mVtxBuf );
    for( size_t i = 0; i < count*4; ++i )
    {
        QuadVertex & v = mVtxBuf[i];

        v.p.set( positions[0], positions[1], positions[2] );
        positions = (const float*)( ((const UInt8*)positions) + posStride );

        if( texcoords )
        {
            v.t.set( texcoords[0], texcoords[1] );
            texcoords = (const float*)( ((const UInt8*)texcoords) + texStride );
        }

        if( colors )
        {
            v.c = BGRA2RGBA(*colors);
            colors = (const UInt32*)( ((const UInt8*)colors) + clrStride );
        }
        else v.c = 0xFFFFFFFF;
    }

    // determine attributes that need to be restored.
    GLbitfield attribs = GL_TEXTURE_BIT;
    if( !(DQ_USE_CURRENT_RS & options ) )
        attribs |= GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BITS | GL_ENABLE_BIT;
    if( !(DQ_USE_CURRENT_VS & options ) )
        attribs |= GL_TRANSFORM_BIT | GL_ENABLE_BIT;
    if( !(DQ_USE_CURRENT_PS & options ) )
        attribs |= GL_ENABLE_BIT;

    // push OGL attributes
    GN_OGL_CHECK( glPushAttrib( attribs ) );
    GN_OGL_CHECK( glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT ) );

    // apply render states
    if( !( DQ_USE_CURRENT_RS & options ) )
    {
        if( DQ_OPAQUE & options )
        {
            glDisable( GL_BLEND );
            glDisable( GL_ALPHA_TEST );
        }
        else
        {
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glEnable( GL_ALPHA_TEST );
            glAlphaFunc( GL_GREATER, 0.0f );
        }
        glDepthMask( !!(DQ_UPDATE_DEPTH & options) );
        if( (DQ_DEPTH_ENABLE|DQ_UPDATE_DEPTH) & options )
        {
            glEnable( GL_DEPTH_TEST );
        }
        else
        {
            glDisable( GL_DEPTH_TEST );
        }
        glDisable( GL_CULL_FACE );
    }

    // apply vertex shader
    if( !( DQ_USE_CURRENT_VS & options ) )
    {
        // disable ARB program
        if( GLEW_ARB_vertex_program )
        {
            glDisable( GL_VERTEX_PROGRAM_ARB );
        }

        // disable NV program
        if( GLEW_NV_vertex_program )
        {
            glDisable( GL_VERTEX_PROGRAM_NV );
        }

        // setup OGL matrices
        if( DQ_WINDOW_SPACE & options )
        {
            GLdouble vp[4];
            GN_OGL_CHECK( glGetDoublev( GL_VIEWPORT, vp ) );

            // position is in screen space (0,0)->(width,height)
            GN_OGL_CHECK( glMatrixMode( GL_PROJECTION ) );
            GN_OGL_CHECK( glPushMatrix() );
            GN_OGL_CHECK( glLoadIdentity() );
            GN_OGL_CHECK( glOrtho( 0, vp[2], vp[3], 0, 0, 1 ) );

            GN_OGL_CHECK( glMatrixMode( GL_MODELVIEW ) );
            GN_OGL_CHECK( glPushMatrix() );
            GN_OGL_CHECK( glLoadIdentity() );
        }
        else
        {
            // position is in unit space (0,0)->(1,1)
            GN_OGL_CHECK( glMatrixMode( GL_PROJECTION ) );
            GN_OGL_CHECK( glPushMatrix() );
            GN_OGL_CHECK( glLoadIdentity() );
            GN_OGL_CHECK( glOrtho( 0, 1, 1, 0, 0, 1 ) );

            GN_OGL_CHECK( glMatrixMode( GL_MODELVIEW ) );
            GN_OGL_CHECK( glPushMatrix() );
            GN_OGL_CHECK( glLoadIdentity() );
        }
    }

    if( !( DQ_USE_CURRENT_PS & options ) )
    {
        // disable ARB program
        if( GLEW_ARB_fragment_program )
        {
            glDisable( GL_FRAGMENT_PROGRAM_ARB );
        }

        // disable NV program
        if( GLEW_NV_vertex_program )
        {
            glDisable( GL_FRAGMENT_PROGRAM_NV );
        }

        glDisable( GL_LIGHTING );
        mRenderer.chooseTextureStage( 0 );
        GN_OGL_CHECK( glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE ) );
    }

    // disable VBO
    GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 ) );

    // apply vertex binding
    mRenderer.chooseClientTextureStage( 0 );
    GN_OGL_CHECK( glInterleavedArrays( GL_T2F_C4UB_V3F, sizeof(QuadVertex), mVtxBuf ) );

    // do draw
    GN_OGL_CHECK( glDrawArrays( GL_QUADS, 0, (GLsizei)count*4 ) );

    if( !( DQ_USE_CURRENT_VS & options ) )
    {
        // restore OGL matrices
        GN_OGL_CHECK( glMatrixMode( GL_PROJECTION ) );
        GN_OGL_CHECK( glPopMatrix() );
        GN_OGL_CHECK( glMatrixMode( GL_MODELVIEW ) );
        GN_OGL_CHECK( glPopMatrix() );
    }

    // restore OGL attributes
    GN_OGL_CHECK( glPopClientAttrib() );
    GN_OGL_CHECK( glPopAttrib() );

    // update mNextQuad
    mNextQuad += count;
    GN_ASSERT( mNextQuad <= MAX_QUADS );
    if( MAX_QUADS == mNextQuad ) mNextQuad = 0;

    GN_UNGUARD_SLOW;
}

#include "pch.h"
#include "oglQuad.h"
#include "oglRenderer.h"

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
        GN_ERROR( "out of memory!" );
        quit(); return selfOK();
    }

    // success
    return selfOK();

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
    const float * positions, size_t posStride,
    const float * texcoords, size_t texStride,
    size_t count, uint32_t options )
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( 0 == positions || 0 == texcoords )
    {
        GN_ERROR( "NULL parameter(s)!" );
        return;
    }

    if( 0 == posStride || 0 == texStride )
    {
        GN_ERROR( "stride can't be zero!" );
        return;
    }

    GN_ASSERT( mNextQuad < MAX_QUADS );

    // handle large amount of array
    while( count + mNextQuad > MAX_QUADS )
    {
        size_t n = MAX_QUADS - mNextQuad;
        GN_ASSERT( n > 0 );
        drawQuads( positions, posStride, texcoords, texStride, n, options );
        positions = (const float*)( ((const uint8_t*)positions) + n * posStride * 4 );
        texcoords = (const float*)( ((const uint8_t*)texcoords) + n * texStride * 4 );
        count -= n;
    }

    // fill vertex data
    GN_ASSERT( mVtxBuf );
    if( DQ_3D_POSITION & options )
    {
        for( size_t i = 0; i < count; ++i )
        {
            QuadVertex * q = mVtxBuf+i*4;
            for( size_t i = 0; i < 4; ++i )
            {
                QuadVertex & v = q[i];
                v.p.set( positions[0], positions[1], positions[2] );
                v.t.set( texcoords[0], texcoords[1] );
                positions = (const float*)( ((const uint8_t*)positions) + posStride );
                texcoords = (const float*)( ((const uint8_t*)texcoords) + texStride );
            }
        }
    }
    else
    {
        for( size_t i = 0; i < count; ++i )
        {
            QuadVertex * q = mVtxBuf+i*4;
            for( size_t i = 0; i < 4; ++i )
            {
                QuadVertex & v = q[i];
                v.p.set( positions[0], positions[1], 0 );
                v.t.set( texcoords[0], texcoords[1] );
                positions = (const float*)( ((const uint8_t*)positions) + posStride );
                texcoords = (const float*)( ((const uint8_t*)texcoords) + texStride );
            }
        }
    }

    // determine attributes that need to be restored.
    GLbitfield attribs = GL_TEXTURE_BIT;
    if( !(DQ_USE_CURRENT_RS & options ) )
        attribs |= GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BITS | GL_ENABLE_BIT;
    if( !(DQ_USE_CURRENT_VS & options ) )
        attribs |= GL_TRANSFORM_BIT;
    if( !(DQ_USE_CURRENT_PS & options ) )
        attribs |= GL_CURRENT_BIT;

    // push OGL attributes
    GN_OGL_CHECK( glPushAttrib( attribs ) );
    GN_OGL_CHECK( glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT ) );

    // apply render states
    if( !( DQ_USE_CURRENT_RS & options ) )
    {
        if( DQ_OPAQUE & options ) glDisable( GL_BLEND ); else glEnable( GL_BLEND );
        glDepthMask( !!(DQ_ENABLE_DEPTH_WRITE & options) );
        glEnable( GL_DEPTH_TEST );
        glDisable( GL_CULL_FACE );
        glDisable( GL_LIGHTING );
    }

    // apply vertex shader 
    if( !( DQ_USE_CURRENT_VS & options ) )
    {
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
        // setup material color
        static Vector4f white(1,1,1,1);
        GN_OGL_CHECK( glColor4fv( white ) );
    }

    // apply texture states
    mRenderer.chooseTextureStage( 0 );
    GN_OGL_CHECK( glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE ) );

    // apply vertex binding
    GN_OGL_CHECK( glInterleavedArrays( GL_T2F_V3F, sizeof(QuadVertex), mVtxBuf ) );

    // do draw
    GN_OGL_CHECK( glDrawArrays( GL_QUADS, 0, count*4 ) );

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

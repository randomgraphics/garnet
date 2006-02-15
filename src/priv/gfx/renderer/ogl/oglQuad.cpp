#include "pch.h"
#include "oglQuad.h"
#include "oglRenderer.h"

struct OGLQuadVertex
{
    GN::Vector4f p;
    GN::Vector2f t;
    GN::Vector2f _; // padding to 32 bytes.
};

struct OGLQuadStruct
{
    OGLQuadVertex v[4];
};

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

    // create vertex binding
    GN_CASSERT( sizeof(OGLQuadVertex) == 32 );
    VtxFmtDesc vfd;
    vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT4 );
    vfd.addAttrib( 0, 16, VTXSEM_TEX0, FMT_FLOAT2 );
    vfd.streams[0].stride = 32;
    mVtxBinding = mRenderer.createVtxBinding( vfd );
    if( 0 == mVtxBinding ) { quit(); return selfOK(); }

    // create vertex buffer
    GN_ASSERT( !mVtxBuf );
    mVtxBuf = mRenderer.createVtxBuf( sizeof(OGLQuadStruct) * MAX_QUADS, true );
    if( 0 == mVtxBuf ) { quit(); return selfOK(); }

    // create index buffer
    GN_ASSERT( !mIdxBuf );
    mIdxBuf = mRenderer.createIdxBuf( MAX_QUADS * 6 );
    if( 0 == mIdxBuf ) { quit(); return selfOK(); }

    // fill index buffer
    uint16_t * ibData = (uint16_t*)mIdxBuf->lock( 0, 0, 0 );
    if( 0 == ibData ) { quit(); return selfOK(); }
    for( uint16_t i = 0; i < MAX_QUADS; ++i )
    {
        ibData[i*6+0] = i*4+0;
        ibData[i*6+1] = i*4+1;
        ibData[i*6+2] = i*4+2;
        ibData[i*6+3] = i*4+0;
        ibData[i*6+4] = i*4+2;
        ibData[i*6+5] = i*4+3;
    }
    mIdxBuf->unlock();

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

    safeDecref( mVtxBuf );
    safeDecref( mIdxBuf );

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
        positions = (const float*)( ((const uint8_t*)positions) + n * posStride );
        texcoords = (const float*)( ((const uint8_t*)texcoords) + n * texStride );
        count -= n;
    }

    // lock vertex buffer
    OGLQuadStruct * vbData;
    if( 0 == mNextQuad )
    {
        vbData = (OGLQuadStruct*)mVtxBuf->lock( 0, 0, LOCK_DISCARD );
    }
    else
    {
        vbData = (OGLQuadStruct*)mVtxBuf->lock(
            sizeof(OGLQuadStruct)*mNextQuad,
            sizeof(OGLQuadStruct)*count,
            LOCK_NO_OVERWRITE );
    }
    if( 0 == vbData ) return;

    // fill vertex data
    if( DQ_3D_POSITION & options )
    {
        for( size_t i = 0; i < count; ++i )
        {
            OGLQuadStruct & q = vbData[i];
            for( size_t i = 0; i < 4; ++i )
            {
                OGLQuadVertex & v = q.v[i];
                v.p.set( positions[0], positions[1], positions[2], 1 );
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
            OGLQuadStruct & q = vbData[i];
            for( size_t i = 0; i < 4; ++i )
            {
                OGLQuadVertex & v = q.v[i];
                v.p.set( positions[0], positions[1], 0, 1 );
                v.t.set( texcoords[0], texcoords[1] );
                positions = (const float*)( ((const uint8_t*)positions) + posStride );
                texcoords = (const float*)( ((const uint8_t*)texcoords) + texStride );
            }
        }
    }

    // unlock the buffer
    mVtxBuf->unlock();

    // apply buffer states
    mRenderer.bindVtxBinding( mVtxBinding );
    mRenderer.bindVtxBufs( &mVtxBuf, 0, 1 );
    mRenderer.bindIdxBuf( mIdxBuf );

    // apply render states
    if( !( DQ_USE_CURRENT_RS & options ) )
    {
        const int statePairs[] =
        {
            RS_BLENDING     , ( DQ_OPAQUE & options ) ? RSV_FALSE : RSV_TRUE,
            RS_DEPTH_TEST   , RSV_TRUE,
            RS_DEPTH_WRITE  , RSV_FALSE,
            RS_CULL_MODE    , RSV_CULL_NONE,
        };
        mRenderer.setRenderStates( statePairs, sizeof(statePairs)/sizeof(statePairs[0])/2 );
    }

    // apply vertex shader 
    if( !( DQ_USE_CURRENT_VS & options ) )
    {
        mRenderer.bindVtxShader( 0 );

        // push OGL attributes
        GN_OGL_CHECK( glPushAttrib( GL_TRANSFORM_BIT ) );

        // setup OGL matrices
        if( DQ_WINDOW_SPACE & options )
        {
            GLdouble vp[4];
            GN_OGL_CHECK( glGetDoublev( GL_VIEWPORT, vp ) );

            // position is in screen space (0,0)->(width,height)
            GN_OGL_CHECK( glMatrixMode( GL_PROJECTION ) );
            GN_OGL_CHECK( glPushMatrix() );
            GN_OGL_CHECK( glLoadIdentity() );
            GN_OGL_CHECK( glOrtho( 0, vp[2], 0, vp[3], 0, 1 ) );

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
        mRenderer.bindPxlShader( 0 );

        // setup material color
        static Vector4f white(1,1,1,1);
        mRenderer.setMaterialDiffuse( white );
    }

    // apply texture states
    // TODO: texture states is meaningless for programmable pipeline.
    mRenderer.setTextureState( 0, TS_COLOROP, TSV_ARG0 );
    mRenderer.setTextureState( 0, TS_COLORARG0, TSV_TEXTURE_COLOR );
    mRenderer.setTextureState( 0, TS_ALPHAOP, TSV_ARG0 );
    mRenderer.setTextureState( 0, TS_ALPHAARG0, TSV_TEXTURE_ALPHA );
    mRenderer.setTextureState( 1, TS_COLOROP, TSV_DISABLE );
    mRenderer.setTextureState( 1, TS_ALPHAOP, TSV_DISABLE );

    // draw
    mRenderer.drawIndexed(
        TRIANGLE_LIST,
        count * 2,     // numPrim
        mNextQuad * 4, // startVtx
        0,             // minVtxIdx
        count * 4,     // numVtx
        0 );           // startIdx

    if( !( DQ_USE_CURRENT_VS & options ) )
    {
        // restore OGL matrices
        GN_OGL_CHECK( glMatrixMode( GL_PROJECTION ) );
        GN_OGL_CHECK( glPopMatrix() );
        GN_OGL_CHECK( glMatrixMode( GL_MODELVIEW ) );
        GN_OGL_CHECK( glPopMatrix() );

        // restore OGL attributes
        GN_OGL_CHECK( glPopAttrib() );
    }

    // update mNextQuad
    mNextQuad += count;
    GN_ASSERT( mNextQuad <= MAX_QUADS );
    if( MAX_QUADS == mNextQuad ) mNextQuad = 0;

    GN_UNGUARD_SLOW;
}

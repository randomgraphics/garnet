#include "pch.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SpriteRenderer::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::SpriteRenderer, () );

    // create vertex buffer
    mVtxBuf.attach( mRenderer.createVtxBuf( VTXBUF_SIZE, true, false ) );
    if( !mVtxBuf ) return failure();

    // create index buffer
    mIdxBuf.attach( mRenderer.createIdxBuf16( MAX_INDICES, false, false ) );
    if( !mIdxBuf ) return failure();
    DynaArray<UInt16> indices( MAX_INDICES );
    for( size_t i = 0; i < MAX_SPRITES_PER_BATCH; ++i )
    {
        indices[i*6+0] = i * 4 + 0;
        indices[i*6+1] = i * 4 + 1;
        indices[i*6+2] = i * 4 + 2;
        indices[i*6+3] = i * 4 + 0;
        indices[i*6+4] = i * 4 + 2;
        indices[i*6+5] = i * 4 + 3;
    }
    mIdxBuf->update( 0, MAX_INDICES, indices.cptr() );

    // create pending vertex buffer
    mPendingVertices = (SpriteVertex)heapAlloc( VTXBUF_SIZE );
    if( NULL == mPendingVertices ) return failure();

    // initialize other member variables
    mPrivateContext.vtxbufs[0] = mVtxBuf;
    mPrivateContext.idxbuf = mIdxBuf;
    mDrawBegun = false;
    mNumPendingSprites = 0;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SpriteRenderer::quit()
{
    GN_GUARD;

    heapFree( mPendingVertices );
    mVtxBuf.clear();
    mIdxBuf.clear();
    mPrivateContext.resetToDefault();
    mEnvironmentContext.resetToDefault();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SpriteRenderer::drawBegin( Texture * texture, BitFields options )
{
    if( mDrawBegun )
    {
        GN_ERROR(sLogger)( "SpriteRenderer::drawBegin() can not be called consequtively w/o drawEnd() in between" );
        return;
    }

    if( options & USE_COSTOM_CONTEXT )
    {
        mEnvironmentContext = mRenderer.getContext();
        mEnvironmentContext.vtxbufs[0] = mVtxBuf;
        mEnvironmentContext.idxbuf = mIdxBuf;
        mEnvironmentContext.textures[0].set( texture );
        mEffectiveContext = &mEnvironmentContext;
    }
    else
    {
        mPrivateContext.textures[0].set( texture );
        mPrivateContext.blendEnabled = options & OPAQUE_SPRITE;
        mPrivateContext.depthTest = options & ENABLE_DEPTH_TEST;
        mPrivateContext.depthWrite = options & ENABLE_DEPTH_WRITE;
        mEffectiveContext = &mPrivateContext;
    }

    mDrawBegun = true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SpriteRenderer::drawEnd()
{
    if( !mDrawBegun )
    {
        GN_ERROR(sLogger)( "SpriteRenderer::drawEnd\() can not be called consequtively w/o drawBegin() in between" );
        return;
    }

    mDrawBegun = false;
    mNumPendingSprites = 0;
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::SpriteRenderer::drawTextured(
    float z,
    float x1,
    float y1,
    float x2,
    float y2,
    float u1,
    float v1,
    float u2,
    float v2 )
{
    if( !mDrawBegun )
    {
        GN_ERROR(sLogger)( "Must be called between drawBegin() and drawEnd()." );
        return;
    }

    if( mNumPendingSprites == MAX_SPRITES_PER_BATCH )
    {
        drawEnd();
        drawBegin( mContext.textures[0].get(), mOptions );
    }

    GN_ASSERT( mNumPendingSprites < MAX_SPRITES_PER_BATCH );

    // fill vertex buffer
    SpriteVertex * p = &mPendingVertices[mNumPendingSprites*4];

    p[0].pos.set( x1, y1, z );
    p[0].tex.set( u1, v1 );

    p[1].pos.set( x1, y2, z );
    p[1].tex.set( u1, v2 );

    p[2].pos.set( x2, y2, z );
    p[2].tex.set( u2, v2 );

    p[3].pos.set( x2, y1, z );
    p[3].tex.set( u2, v1 );

    // prepare for next sprite
    ++mNumPendingSprites;
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::SpriteRenderer::drawSolid(
    UInt32 rgba,
    float  z,
    float  x1,
    float  y1,
    float  x2,
    float  y2 )
{
    if( !mDrawBegun )
    {
        GN_ERROR(sLogger)( "Must be called between drawBegin() and drawEnd()." );
        return;
    }

    if( mNumPendingSprites == MAX_SPRITES_PER_BATCH )
    {
        drawEnd();
        drawBegin( mContext.textures[0].get(), mOptions );
    }

    GN_ASSERT( mNumPendingSprites < MAX_SPRITES_PER_BATCH );

    // fill vertex buffer
    SpriteVertex * p = &mPendingVertices[mNumPendingSprites*4];

    p[0].pos.set( x1, y1, z );
    p[0].clr = rgba;

    p[1].pos.set( x1, y2, z );
    p[1].clr = rgba;

    p[2].pos.set( x2, y2, z );
    p[2].clr = rgba;

    p[3].pos.set( x2, y1, z );
    p[3].clr = rgba;

    // prepare for next Sprite
    ++mNumPendingSprites;
}

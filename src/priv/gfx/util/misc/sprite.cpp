#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

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

    enum
    {
        MAX_VERTICES = MAX_SPRITES * 4,
        MAX_INDICES  = MAX_SPRITES * 6,
        VTXBUF_SIZE  = MAX_VERTICES * sizeof(SpriteVertex)
    };

    // create vertex buffer
    mVtxBuf.attach( mRenderer.createVtxBuf( VTXBUF_SIZE, true, false ) );
    if( !mVtxBuf ) return failure();

    // create index buffer
    mIdxBuf.attach( mRenderer.createIdxBuf16( MAX_INDICES, false, false ) );
    if( !mIdxBuf ) return failure();
    DynaArray<UInt16> indices( MAX_INDICES );
    for( UInt16 i = 0; i < MAX_SPRITES; ++i )
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
    mSprites = (Sprite*)heapAlloc( VTXBUF_SIZE );
    if( NULL == mSprites ) return failure();

    // initialize other member variables
    mPrivateContext.vtxbufs[0] = mVtxBuf;
    mPrivateContext.idxbuf = mIdxBuf;
    mDrawBegun = false;
    mNextPendingSprite = mSprites;
    mNextFreeSprite = mSprites;

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

    heapFree( mSprites );
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
    mOptions   = options;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SpriteRenderer::drawEnd()
{
    if( !mDrawBegun )
    {
        GN_ERROR(sLogger)( "SpriteRenderer::drawEnd() can not be called consequtively w/o drawBegin() in between" );
        return;
    }

    size_t numPendingSprites = mNextFreeSprite - mNextPendingSprite;
    if( numPendingSprites > 0 )
    {
        size_t firstPendingSpriteOffset = mNextPendingSprite - mSprites;

        mVtxBuf->update(
            firstPendingSpriteOffset * sizeof(Sprite),
            numPendingSprites * sizeof(Sprite),
            mNextPendingSprite,
            mSprites == mNextPendingSprite ? SURFACE_UPDATE_DISCARD : SURFACE_UPDATE_NO_OVERWRITE );

        mRenderer.bindContext( *mEffectiveContext );

        mRenderer.drawIndexed(
            TRIANGLE_LIST,
            numPendingSprites * 6,        // numidx
            firstPendingSpriteOffset * 4, // basevtx,
            0,                            // startvtx
            numPendingSprites * 4,        // numvtx
            0 );                          // startidx
    }

    mDrawBegun = false;
    if( mNextFreeSprite == mSprites + MAX_SPRITES ) mNextFreeSprite = mSprites; // rewind next-free pointer if needed.
    mNextPendingSprite = mNextFreeSprite;
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

    if( mNextFreeSprite == mSprites + MAX_SPRITES )
    {
        drawEnd();
        drawBegin( mEffectiveContext->textures[0].get(), mOptions );
    }

    GN_ASSERT( mNextFreeSprite < mSprites + MAX_SPRITES );

    // fill vertex buffer
    mNextFreeSprite->v[0].pos.set( x1, y1, z );
    mNextFreeSprite->v[0].tex.set( u1, v1 );

    mNextFreeSprite->v[1].pos.set( x1, y2, z );
    mNextFreeSprite->v[1].tex.set( u1, v2 );

    mNextFreeSprite->v[2].pos.set( x2, y2, z );
    mNextFreeSprite->v[2].tex.set( u2, v2 );

    mNextFreeSprite->v[3].pos.set( x2, y1, z );
    mNextFreeSprite->v[3].tex.set( u2, v1 );

    // prepare for next sprite
    ++mNextFreeSprite;
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

    if( mNextFreeSprite == mSprites + MAX_SPRITES )
    {
        drawEnd();
        drawBegin( mEffectiveContext->textures[0].get(), mOptions );
    }

    GN_ASSERT( mNextFreeSprite < mSprites + MAX_SPRITES );

    mNextFreeSprite->v[0].pos.set( x1, y1, z );
    mNextFreeSprite->v[0].clr = rgba;

    mNextFreeSprite->v[1].pos.set( x1, y2, z );
    mNextFreeSprite->v[1].clr = rgba;

    mNextFreeSprite->v[2].pos.set( x2, y2, z );
    mNextFreeSprite->v[2].clr = rgba;

    mNextFreeSprite->v[3].pos.set( x2, y1, z );
    mNextFreeSprite->v[3].clr = rgba;

    // prepare for next Sprite
    ++mNextFreeSprite;
}

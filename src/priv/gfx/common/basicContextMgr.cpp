#include "pch.h"
#include "basicRenderer.h"

template<class T>
static void sClearAutoRefArray( GN::AutoRef<T> data[], size_t count )
{
    GN_ASSERT( data && count );
    for( size_t i = 0; i < count; ++i ) data[i].clear();
}

template<class T>
static void sUpdateAutoRefArray( GN::AutoRef<T> dst[], T * const src[], size_t count )
{
    GN_ASSERT( dst && src && count );
    for( size_t i = 0; i < count; ++i ) dst[i].set( src[i] );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::contextClear()
{
    GN_GUARD;
    sClearAutoRefArray( mResourceHolder.shaders, NUM_SHADER_TYPES );
    sClearAutoRefArray( mResourceHolder.colorBuffers, MAX_RENDER_TARGETS );
    mResourceHolder.depthBuffer.clear();
    sClearAutoRefArray( mResourceHolder.textures, MAX_TEXTURE_STAGES );
    sClearAutoRefArray( mResourceHolder.vtxBufs, MAX_VERTEX_STREAMS );
    mResourceHolder.idxBuf.clear();
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::holdContextState( const ContextState & state )
{
    GN_GUARD;

    GN_ASSERT( 0xFFFFFFFF == state.flags.u32 );

    sUpdateAutoRefArray( mResourceHolder.shaders, state.shaders, NUM_SHADER_TYPES );

    for( size_t i = 0; i < state.numColorBuffers; ++i ) mResourceHolder.colorBuffers[i].set( state.colorBuffers[i].texture );

    mResourceHolder.depthBuffer.set( state.depthBuffer.texture );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::holdContextData( const ContextData & data )
{
    GN_GUARD;

    GN_ASSERT( 0xFFFFFFFF == data.flags.u32 );

    sUpdateAutoRefArray( mResourceHolder.textures, data.textures, data.numTextures );

    for( size_t i = 0; i < data.numVtxBufs; ++i ) mResourceHolder.vtxBufs[i].set( data.vtxBufs[i].buffer );

    mResourceHolder.idxBuf.set( data.idxBuf );

    GN_UNGUARD;
}

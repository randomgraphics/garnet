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

    for( int i = 0; i < NUM_SHADER_TYPES; ++i )
    {
        if( state.flags.shaderBit(i) ) mResourceHolder.shaders[i].set( state.shaders[i] );
    }

    if( state.flags.renderTargets )
    {
        for( size_t i = 0; i < state.renderTargets.numColorBuffers; ++i )
            mResourceHolder.colorBuffers[i].set( state.renderTargets.colorBuffers[i].texture );
        mResourceHolder.depthBuffer.set( state.renderTargets.depthBuffer.texture );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::holdContextData( const ContextData & data )
{
    GN_GUARD;

    if( data.flags.textures )
        sUpdateAutoRefArray( mResourceHolder.textures, data.textures, data.numTextures );

    if( data.flags.vtxBufs )
        for( size_t i = 0; i < data.numVtxBufs; ++i )
            mResourceHolder.vtxBufs[i].set( data.vtxBufs[i].buffer );

    if( data.flags.idxBuf ) mResourceHolder.idxBuf.set( data.idxBuf );

    GN_UNGUARD;
}

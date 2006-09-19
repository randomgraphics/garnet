#include "pch.h"
#include "basicRenderer.h"

#define UPDATE_AUTOREF_ARRAY( array, newCount, newData, field ) \
{ \
    GN_ASSERT( newData && newCount <= array.MAX_COUNT ); \
    for( size_t i = 0; i < newCount; ++i ) array.data[i].set( newData[i]field ); \
    for( size_t i = newCount; i < newCount; ++i ) array.data[i].clear(); \
    array.count = newCount; \
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::holdContextResources( const RendererContext & context )
{
    GN_GUARD_SLOW;

    for( int i = 0; i < NUM_SHADER_TYPES; ++i )
    {
        if( context.flags.shaderBit(i) ) mResourceHolder.shaders.data[i].set( context.shaders[i] );
    }

    if( context.flags.colorBuffers )
    {
        UPDATE_AUTOREF_ARRAY(
            mResourceHolder.colorBuffers,
            context.numColorBuffers,
            context.colorBuffers,
            .texture );
    }

    if( context.flags.depthBuffer )
    {
        mResourceHolder.depthBuffer.set( context.depthBuffer.texture );
    }

    if( context.flags.textures )
    {
        UPDATE_AUTOREF_ARRAY(
            mResourceHolder.textures,
            context.numTextures,
            context.textures, );
    }

    if( context.flags.vtxBufs )
    {
        UPDATE_AUTOREF_ARRAY(
            mResourceHolder.vtxBufs,
            context.numVtxBufs,
            context.vtxBufs,
            .buffer );
    }

    if( context.flags.idxBuf ) mResourceHolder.idxBuf.set( context.idxBuf );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::clearContextResources()
{
    GN_GUARD;
    mResourceHolder.shaders.clear();
    mResourceHolder.colorBuffers.clear();
    mResourceHolder.depthBuffer.clear();
    mResourceHolder.textures.clear();
    mResourceHolder.vtxBufs.clear();
    mResourceHolder.idxBuf.clear();
    GN_UNGUARD;
}

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

    if( context.flags.renderTargets )
    {
        const RenderTargetDesc & rtd = context.renderTargets;
        
        UPDATE_AUTOREF_ARRAY(
            mResourceHolder.cbuffers,
            rtd.count,
            rtd.cbuffers,
            .texture );

        mResourceHolder.zbuffer.set( rtd.zbuffer.texture );
    }

    if( context.flags.textures )
    {
        UPDATE_AUTOREF_ARRAY(
            mResourceHolder.textures,
            context.numTextures,
            context.textures, );
    }

    if( context.flags.vtxbufs )
    {
        UPDATE_AUTOREF_ARRAY(
            mResourceHolder.vtxbufs,
            context.numVtxBufs,
            context.vtxbufs,
            .buffer );
    }

    if( context.flags.idxbuf ) mResourceHolder.idxbuf.set( context.idxbuf );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::clearContextResources()
{
    GN_GUARD;
    mResourceHolder.shaders.clear();
    mResourceHolder.cbuffers.clear();
    mResourceHolder.zbuffer.clear();
    mResourceHolder.textures.clear();
    mResourceHolder.vtxbufs.clear();
    mResourceHolder.idxbuf.clear();
    GN_UNGUARD;
}

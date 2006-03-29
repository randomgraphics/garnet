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
void GN::gfx::BasicRenderer::holdContextReference( const RendererContext & context )
{
    GN_GUARD_SLOW;

    for( int i = 0; i < NUM_SHADER_TYPES; ++i )
    {
        if( context.flags.shaderBit(i) ) mResourceHolder.shaders[i].set( context.shaders[i] );
    }

    if( context.flags.renderTargets )
    {
        for( size_t i = 0; i < context.renderTargets.numColorBuffers; ++i )
            mResourceHolder.colorBuffers[i].set( context.renderTargets.colorBuffers[i].texture );
        mResourceHolder.depthBuffer.set( context.renderTargets.depthBuffer.texture );
    }

    if( context.flags.textures )
    {
        sUpdateAutoRefArray( mResourceHolder.textures, context.textures, context.numTextures );
    }

    if( context.flags.vtxBufs )
    {
        for( size_t i = 0; i < context.numVtxBufs; ++i )
            mResourceHolder.vtxBufs[i].set( context.vtxBufs[i].buffer );
    }

    if( context.flags.idxBuf ) mResourceHolder.idxBuf.set( context.idxBuf );

    GN_UNGUARD_SLOW;
}

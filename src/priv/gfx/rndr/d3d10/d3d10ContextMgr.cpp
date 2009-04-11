#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10RenderTargetMgr.h"
#include "d3d10StateObject.h"
#include "d3d10Shader.h"
#include "d3d10Texture.h"
#include "d3d10VtxLayout.h"
#include "d3d10Buffer.h"

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::contextInit()
{
    GN_GUARD;

    mSOMgr = new D3D10StateObjectManager( *this );
    if( 0 == mSOMgr ) return false;

    // create render target manager
    mRTMgr = new D3D10RTMgr( *this );
    if( !mRTMgr->init() ) return false;

    // bind default context
    rebindContext();

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::contextQuit()
{
    GN_GUARD;

    mDevice->ClearState();

    mContext.clear();

    safeDelete( mRTMgr );

    safeDelete( mSOMgr );

    mInputLayouts.clear();

    GN_UNGUARD;
}

// *****************************************************************************
// from BasicRenderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::bindContextImpl( const RendererContext & newContext, bool skipDirtyCheck )
{
    PIXPERF_FUNCTION_EVENT();

    //
    // Parameter check
    //
    if( paramCheckEnabled() )
    {
        // TODO: verify data in new context
        // TODO: make sure all fields in current context are valid.
    }

    if( !bindContextRenderTarget( newContext, skipDirtyCheck ) ) return false;
    if( !bindContextShader( newContext, skipDirtyCheck ) ) return false;
    if( !bindContextState( newContext, skipDirtyCheck ) ) return false;
    if( !bindContextResource( newContext, skipDirtyCheck ) ) return false;

    return true;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D10Renderer::bindContextRenderTarget(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    //
    // bind render targets
    //
    bool needRebindViewport = false;
    if( !mRTMgr->bind(
            newContext.crts,
            newContext.dsrt,
            mContext.crts,
            newContext.dsrt,
            skipDirtyCheck,
            needRebindViewport ) )
    {
        return false;
    }

    // bind viewport
    const Vector2<UInt32> & rtsize = mRTMgr->getRenderTargetSize();
    const Rect<UInt32> & newvp = newContext.viewport;
    D3D10_VIEWPORT d3dvp;
    d3dvp.MinDepth = 0.0f;
    d3dvp.MaxDepth = 1.0f;
    if( 0 == newvp.x && 0 == newvp.y && 0 == newvp.w && 0 == newvp.h )
    {
        d3dvp.TopLeftX = 0;
        d3dvp.TopLeftY = 0;
        d3dvp.Width    = rtsize.x;
        d3dvp.Height   = rtsize.y;
    }
    else
    {
        UInt32 l = newvp.x;
        UInt32 t = newvp.y;
        UInt32 r = newvp.x + newvp.w;
        UInt32 b = newvp.y + newvp.h;

        math::clamp<UInt32>( l, 0, rtsize.width );
        math::clamp<UInt32>( t, 0, rtsize.height );
        math::clamp<UInt32>( r, 0, rtsize.width );
        math::clamp<UInt32>( b, 0, rtsize.height );

        d3dvp.TopLeftX = l;
        d3dvp.TopLeftY = t;
        d3dvp.Width    = r - l;
        d3dvp.Height   = b - t;
    }
    mDevice->RSSetViewports( 1, &d3dvp );

    // update scissor (same as viewport)
    D3D10_RECT scissor = {
        (long)( d3dvp.TopLeftX ),
        (long)( d3dvp.TopLeftY ),
        (long)( d3dvp.TopLeftX+d3dvp.Width ),
        (long)( d3dvp.TopLeftY+d3dvp.Height ),
    };
    mDevice->RSSetScissorRects( 1, &scissor );

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D10Renderer::bindContextShader(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    //
    // bind shaders
    //
    if( newContext.gpuProgram )
    {
        D3D10GpuProgram * newProg = (D3D10GpuProgram*)newContext.gpuProgram.get();
        D3D10GpuProgram * oldProg = (D3D10GpuProgram*)mContext.gpuProgram.get();

        // apply shader
        if( skipDirtyCheck || newProg != oldProg )
        {
            newProg->apply();
        }

        // apply uniforms
        const SysMemUniform * const * uniforms = (const SysMemUniform * const *)newContext.uniforms.cptr();
        newProg->applyUniforms( uniforms, newContext.uniforms.size(), skipDirtyCheck );
    }
    else if( skipDirtyCheck || (NULL != mContext.gpuProgram) )
    {
        mDevice->VSSetShader( NULL );
        mDevice->GSSetShader( NULL );
        mDevice->PSSetShader( NULL );
    }
    else
    {
        // Both old and new program are NULL. Do nothing
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D10Renderer::bindContextState(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    GN_UNUSED_PARAM( newContext );
    GN_UNUSED_PARAM( skipDirtyCheck );
    GN_UNIMPL_WARNING();
    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D10Renderer::bindContextResource(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    //
    // bind vertex format
    //
    if( skipDirtyCheck || mContext.vtxfmt != newContext.vtxfmt )
    {
        if( 0 == newContext.vtxfmt.numElements )
        {
            mDevice->IASetInputLayout( NULL );
        }
        else
        {
            AutoComPtr<ID3D10InputLayout> & layout = mInputLayouts[newContext.vtxfmt];

            if( NULL == layout )
            {
                layout.attach( createD3D10InputLayout( *mDevice, newContext.vtxfmt ) );
                if( NULL == layout ) return false;
            }

            mDevice->IASetInputLayout( layout );
        }
    }

    ///
    /// bind vertex buffers
    ///
    ID3D10Buffer * buf[RendererContext::MAX_VERTEX_BUFFERS];
    UINT           strides[RendererContext::MAX_VERTEX_BUFFERS];
    UINT           offsets[RendererContext::MAX_VERTEX_BUFFERS];
    for( UINT i = 0; i < RendererContext::MAX_VERTEX_BUFFERS; ++i )
    {
        const AutoRef<VtxBuf> & vb = newContext.vtxbufs[i];
        buf[i]     = vb ? safeCastPtr<const D3D10VtxBuf>(vb.get())->getD3DBuffer() : NULL;
        strides[i] = newContext.strides[i];
        offsets[i] = 0;
    }
    mDevice->IASetVertexBuffers( 0, RendererContext::MAX_VERTEX_BUFFERS, buf, strides, offsets );

    //
    // bind index buffer
    //
    if( skipDirtyCheck || newContext.idxbuf != mContext.idxbuf )
    {
        if( newContext.idxbuf )
        {
            const D3D10IdxBuf * ib = (const D3D10IdxBuf*)newContext.idxbuf.get();

            const IdxBufDesc & ibdesc = ib->getDesc();

            mDevice->IASetIndexBuffer(
                ib->getD3DBuffer(),
                ibdesc.bits32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
                0 );
        }
        else
        {
            mDevice->IASetIndexBuffer( NULL, DXGI_FORMAT_R16_UINT, 0 );
        }
    }

    //
    // bind textures
    //
    /*
    UINT maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);
    UINT numTex = min<UINT>( (UINT)newContext.numTextures, maxStages );
    UINT stage;
    for( stage = 0; stage < numTex; ++stage )
    {
        const Texture * tex = newContext.textures[stage];
        if( tex != mContext.textures[stage] ||
            stage > mContext.numTextures ||
            !skipDirtyCheck )
        {
            if( tex )
            {
                safeCastPtr<const D3D10Texture>(tex)->bind( stage );
            }
            else
            {
                mDevice->SetTexture( stage, NULL );
            }
        }
    }
    // clear unused stages
    numTex = min<UINT>( (UINT)mContext.numTextures, maxStages );
    for( ; stage < numTex; ++stage )
    {
        mDevice->SetTexture( stage, 0 );
    }
    */

    return true;
}

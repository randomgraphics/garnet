#include "pch.h"
#include "basicRenderer.h"

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::drawGeometry(
    const RenderingParameters & params, const RenderingGeometry * geomList, size_t count )
{
    GN_GUARD_SLOW;

    if( params.vtxShader.used ) bindVtxShader( params.vtxShader.value );
    if( params.pxlShader.used ) bindPxlShader( params.pxlShader.value );
    if( params.renderStateBlock.used ) bindRenderStateBlock( params.renderStateBlock.value );
    for( uint32_t i = 0; i < MAX_TEXTURE_STAGES; ++i )
    {
        const RenderingParameters::Parameter<AutoRef<Texture> > & t = params.textures[i];
        if( t.used ) bindTexture( i, t.value );
    }
    for( uint32_t i = 0; i < MAX_RENDER_TARGETS; ++i )
    {
        if( params.renderTargets[i].used )
        {
            const RenderingParameters::RenderTargetDesc & rt = params.renderTargets[i].value;
            setRenderTarget( i, rt.texture, rt.level, rt.face );
        }
    }
    if( params.renderDepth.used )
    {
        const RenderingParameters::RenderTargetDesc & rt = params.renderDepth.value;
        setRenderDepth( rt.texture, rt.level, rt.face );
    }
    #define APPLY_FFP( name ) if( params.name.used ) set##name( params.name.value )
    APPLY_FFP( TransformWorld );
    APPLY_FFP( TransformView );
    APPLY_FFP( TransformProj );
    APPLY_FFP( Viewport );
    APPLY_FFP( Light0Pos );
    APPLY_FFP( Light0Diffuse );
    APPLY_FFP( MaterialDiffuse );
    APPLY_FFP( MaterialSpecular );
    APPLY_FFP( TextureStates );
    #undef APPLY_FFP

    GN_UNIMPL_WARNING();

    for( size_t i = 0; i < count; ++i )
    {
        const RenderingGeometry & g = geomList[i];

        bindVtxBinding( g.vtxBinding );
        bindVtxBufs( (const VtxBuf**)g.vtxBufs, 0, g.numVtxBufs );
        bindIdxBuf( g.idxBuf );

        if( g.idxBuf )
        {
            drawIndexed( g.prim, g.numPrim, g.startVtx, g.minVtxIdx, g.numVtx, g.startIdx );
        }
        else
        {
            draw( g.prim, g.numPrim, g.startVtx );
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::drawTextA(
    const char * s, int x, int y, const Vector4f & c )
{
    GN_GUARD_SLOW;

    if( strEmpty(s) ) return;

    size_t len = strLen(s);
    wchar_t * ws = 0;

    // allocate buffer in stack
    ws = (wchar_t*)alloca( sizeof(wchar_t) * (len+1) );

    // convert mbs -> unicode
    size_t wlen = mbstowcs( ws, s, len );
    if( static_cast<size_t>(-1L) == wlen )
    {
        GNGFX_ERROR( "fail to convert multi-byte string to unicode string!" );
        return;
    }
    ws[wlen] = 0;

    // call UNICODE version of drawText()
    drawTextW( ws, x, y, c );

    GN_UNGUARD_SLOW;
}

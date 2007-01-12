namespace GN { namespace gfx
{
    // *************************************************************************
    // resource management methods
    // *************************************************************************

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createVS( ShadingLanguage lang,
                        const StrA &    code,
                        const StrA &    hints )
    {
        GN_GUARD;
        return createShader( SHADER_VS, lang, code, hints );
        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createPS( ShadingLanguage lang,
                        const StrA &    code,
                        const StrA &    hints )
    {
        GN_GUARD;
        return createShader( SHADER_PS, lang, code, hints );
        GN_UNGUARD;
    }

    // *************************************************************************
    // context update helpers
    // *************************************************************************

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::contextUpdateBegin()
    {
        GN_ASSERT( !mHelperContextUpdateBegun );
        mHelperContextUpdateBegun = true;
        mHelperContext.clearToNull();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::contextUpdateEnd()
    {
        GN_ASSERT( mHelperContextUpdateBegun );
        mHelperContextUpdateBegun = false;
        if( mHelperContext.flags.u32 ) setContext( mHelperContext );
    }

#define _GNGFX_CONTEXT_UPDATE_BEGIN() \
    bool immediate = !mHelperContextUpdateBegun; if( immediate ) { contextUpdateBegin(); } else void(0)

#define _GNGFX_CONTEXT_UPDATE_END() \
    if( immediate ) { contextUpdateEnd(); } else void(0)

#define _GNGFX_CONTEXT_UPDATE( contextMethod ) \
    _GNGFX_CONTEXT_UPDATE_BEGIN(); \
    mHelperContext.contextMethod; \
    _GNGFX_CONTEXT_UPDATE_END();

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShader( ShaderType type, const Shader * shader )
    {
        _GNGFX_CONTEXT_UPDATE( setShader( type, shader ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShaders( const Shader * vs, const Shader * ps, const Shader * gs )
    {
        _GNGFX_CONTEXT_UPDATE( setShaders( vs, ps, gs ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVS( const Shader * s )
    {
        _GNGFX_CONTEXT_UPDATE( setVS( s ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setPS( const Shader * s )
    {
        _GNGFX_CONTEXT_UPDATE( setPS( s ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setGS( const Shader * s )
    {
        _GNGFX_CONTEXT_UPDATE( setGS( s ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderStateBlock( const RenderStateBlockDesc & rsb )
    {
        _GNGFX_CONTEXT_UPDATE( setRenderStateBlock( rsb ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderState( RenderState state, SInt32 value )
    {
        _GNGFX_CONTEXT_UPDATE( setRenderState( state, value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderTargets( const RenderTargetDesc & desc )
    {
        _GNGFX_CONTEXT_UPDATE( setRenderTargets( desc ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    void Renderer::setDrawToBackBuf()
    {
        _GNGFX_CONTEXT_UPDATE( setDrawToBackBuf() );
    }

    //
    //
    // -------------------------------------------------------------------------
    void Renderer::setDrawToTexture(
        UInt32 count,
        const Texture * rt0,
        const Texture * rt1,
        const Texture * rt2,
        const Texture * rt3,
        const Texture * z,
        MsaaType aa )
    {
        _GNGFX_CONTEXT_UPDATE( setDrawToTexture( count, rt0, rt1, rt2, rt3, z, aa ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setViewport( const Rectf & viewport )
    {
        _GNGFX_CONTEXT_UPDATE( setViewport( viewport ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setViewport( float left, float top, float width, float height )
    {
        _GNGFX_CONTEXT_UPDATE( setViewport( left, top, width, height ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setWorld( const Matrix44f & value )
    {
        _GNGFX_CONTEXT_UPDATE( setWorld( value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setView( const Matrix44f & value )
    {
        _GNGFX_CONTEXT_UPDATE( setView( value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setProj( const Matrix44f & value )
    {
        _GNGFX_CONTEXT_UPDATE( setProj( value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureStateBlock( const TextureStateBlockDesc & value )
    {
        _GNGFX_CONTEXT_UPDATE( setTextureStateBlock( value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureState( size_t stage, TextureState state, TextureStateValue value )
    {
        _GNGFX_CONTEXT_UPDATE( setTextureState( stage, state, value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTexture( size_t stage, const Texture * tex )
    {
        _GNGFX_CONTEXT_UPDATE( setTexture( stage, tex ) )
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextures( const Texture * const texlist[], size_t start, size_t count )
    {
        _GNGFX_CONTEXT_UPDATE( setTextures( texlist, start, count ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxFmt( VtxFmtHandle h )
    {
        _GNGFX_CONTEXT_UPDATE( setVtxFmt( h ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxBuf( size_t index, const VtxBuf * buffer, size_t stride )
    {
        _GNGFX_CONTEXT_UPDATE( setVtxBuf( index, buffer, stride ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setIdxBuf( const IdxBuf * idxBuf )
    {
        _GNGFX_CONTEXT_UPDATE( setIdxBuf( idxBuf ) );
    }

#undef _GNGFX_CONTEXT_UPDATE
#undef _GNGFX_CONTEXT_UPDATE_BEGIN
#undef _GNGFX_CONTEXT_UPDATE_END
}}

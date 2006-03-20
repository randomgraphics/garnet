namespace GN { namespace gfx
{
    // *************************************************************************
    // resource management methods
    // *************************************************************************

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createVtxShader( ShadingLanguage lang,
                               const StrA &    code,
                               const StrA &    entry )
    {
        GN_GUARD;
        return createShader( VERTEX_SHADER, lang, code, entry );
        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createPxlShader( ShadingLanguage lang,
                               const StrA &    code,
                               const StrA &    entry )
    {
        GN_GUARD;
        return createShader( PIXEL_SHADER, lang, code, entry );
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
        GN_ASSERT( !mHelperUpdateBegun );
        mHelperUpdateBegun = true;
        mHelperContextState.clearToNull();
        mHelperContextData.clearToNull();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::contextUpdateEnd()
    {
        GN_ASSERT( mHelperUpdateBegun );
        mHelperUpdateBegun = false;
        if( mHelperContextState.flags.u32 ) setContextState( mHelperContextState );
        if( mHelperContextData.flags.u32 ) setContextData( mHelperContextData );
    }

#define _GNGFX_CONTEXT_UPDATE_BEGIN() \
    bool immediate = !mHelperUpdateBegun; if( immediate ) { contextUpdateBegin(); } else void(0)

#define _GNGFX_CONTEXT_UPDATE_END() \
    if( immediate ) { contextUpdateEnd(); } else void(0)

#define _GNGFX_CONTEXT_UPDATE_STATE( contextMethod ) \
    _GNGFX_CONTEXT_UPDATE_BEGIN(); \
    mHelperContextState.contextMethod; \
    _GNGFX_CONTEXT_UPDATE_END();

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShader( ShaderType type, const Shader * shader )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setShader( type, shader ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShaders( const Shader * const shaders[] )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setShaders( shaders ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShaders( const Shader * vtxShader, const Shader * pxlShader )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setShaders( vtxShader, pxlShader ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShaderHandles( ShaderDictionary::HandleType vtxShader, ShaderDictionary::HandleType pxlShader )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setShaderHandles( vtxShader, pxlShader ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxShader( const Shader * s )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setVtxShader( s ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxShaderHandle( ShaderDictionary::HandleType h )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setVtxShaderHandle( h ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setPxlShader( const Shader * s )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setPxlShader( s ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setPxlShaderHandle( ShaderDictionary::HandleType h )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setPxlShaderHandle( h ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderStateBlock( const RenderStateBlockDesc & rsb )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setRenderStateBlock( rsb ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderState( RenderState state, int32_t value )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setRenderState( state, value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setColorBuffer( size_t index, const Texture * texture, size_t face, size_t level, size_t slice )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setColorBuffer( index, texture, face, level, slice ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setDepthBuffer( const Texture * texture, size_t face, size_t level, size_t slice )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setDepthBuffer( texture, face, level, slice ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setMsaa( MsaaType type )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setMsaa( type ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setViewport( const Rectf & viewport )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setViewport( viewport ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setViewport( float left, float top, float width, float height )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setViewport( left, top, width, height ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureStateBlock( const TextureStateBlockDesc & value )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setTextureStateBlock( value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureState( size_t stage, TextureState state, TextureStateValue value )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setTextureState( stage, state, value ) );
    }

#undef _GNGFX_CONTEXT_UPDATE_STATE
#define _GNGFX_CONTEXT_UPDATE_DATA( contextMethod ) \
    _GNGFX_CONTEXT_UPDATE_BEGIN(); \
    mHelperContextData.contextMethod; \
    _GNGFX_CONTEXT_UPDATE_END();

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTexture( size_t stage, const Texture * tex )
    {
        _GNGFX_CONTEXT_UPDATE_DATA( setTexture( stage, tex ) )
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureHandle( size_t stage, TextureDictionary::HandleType tex )
    {
        _GNGFX_CONTEXT_UPDATE_DATA( setTextureHandle( stage, tex ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextures( const Texture * const texlist[], size_t start, size_t count )
    {
        _GNGFX_CONTEXT_UPDATE_DATA( setTextures( texlist, start, count ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureHandles( const TextureDictionary::HandleType texlist[], size_t start, size_t count )
    {
        _GNGFX_CONTEXT_UPDATE_DATA( setTextureHandles( texlist, start, count ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxFmt( VtxFmtHandle h )
    {
        _GNGFX_CONTEXT_UPDATE_DATA( setVtxFmt( h ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxBuf( size_t index, const VtxBuf * buffer, size_t stride )
    {
        _GNGFX_CONTEXT_UPDATE_DATA( setVtxBuf( index, buffer, stride ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setIdxBuf( const IdxBuf * idxBuf )
    {
        _GNGFX_CONTEXT_UPDATE_DATA( setIdxBuf( idxBuf ) );
    }

#undef _GNGFX_CONTEXT_UPDATE_DATA
#undef _GNGFX_CONTEXT_UPDATE_BEGIN
#undef _GNGFX_CONTEXT_UPDATE_END
}}

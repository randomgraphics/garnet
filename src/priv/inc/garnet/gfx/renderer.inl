namespace GN { namespace gfx
{
    // *************************************************************************
    // ContextState methods
    // *************************************************************************

#define _GNGFX_CONTEXT_STATE( FLAG, NAME, VALUE ) if( (NAME) != (VALUE) ) { flags.FLAG = 1; (NAME) = (VALUE); } else void(0)

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setShader( ShaderType type, const Shader * shader )
    {
        switch( type )
        {
            case VERTEX_SHADER : setVtxShader( shader ); break;
            case PIXEL_SHADER  : setPxlShader( shader ); break;
            default            : GN_ERROR( "invalid shader type : %d", type );
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setShaders( const Shader * const shaders[] )
    {
        setShaders( shaders[0], shaders[1] );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setShaders( const Shader * vtxShader, const Shader * pxlShader )
    {
        _GNGFX_CONTEXT_STATE( vtxShader, shaders[0], vtxShader );
        _GNGFX_CONTEXT_STATE( pxlShader, shaders[1], pxlShader );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setShaderHandles( ShaderDictionary::HandleType vtxShader, ShaderDictionary::HandleType pxlShader )
    {
        setVtxShaderHandle( vtxShader );
        setPxlShaderHandle( pxlShader );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setVtxShader( const Shader * s )
    {
        _GNGFX_CONTEXT_STATE( vtxShader, shaders[0], s );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setVtxShaderHandle( ShaderDictionary::HandleType h )
    {
        setVtxShader( gShaderDict.getResource(h) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setPxlShader( const Shader * s )
    {
        _GNGFX_CONTEXT_STATE( pxlShader, shaders[1], s );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setPxlShaderHandle( ShaderDictionary::HandleType h )
    {
        setShader( PIXEL_SHADER, gShaderDict.getResource(h) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setRenderStateBlock( const RenderStateBlockDesc & newRsb )
    {
        _GNGFX_CONTEXT_STATE( rsb, rsb, newRsb );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setRenderState( RenderState state, RenderStateValue value )
    {
        GN_ASSERT( 0 <= state && state < NUM_RENDER_STATES );
        GN_ASSERT( 0 <= value && value < NUM_RENDER_STATE_VALUES );
        _GNGFX_CONTEXT_STATE( rsb, rsb.rs[state], value );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setRenderStates( const int * statePairs, size_t count )
    {
        GN_ASSERT( statePairs || !count );

        int state;
        int value;

        for( size_t i = 0; i < count; ++i )
        {
            state = *statePairs; ++statePairs;
            value = *statePairs; ++statePairs;

            GN_ASSERT( 0 <= state && state < NUM_RENDER_STATES );
            GN_ASSERT( 0 <= value && value < NUM_RENDER_STATE_VALUES );

            _GNGFX_CONTEXT_STATE( rsb, rsb.rs[state], (RenderStateValue)value );
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setViewport( const Rectf & vp )
    {
        _GNGFX_CONTEXT_STATE( viewport, viewport, vp );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setViewport( float left, float top, float width, float height )
    {
        setViewport( Rectf( left, top, width, height ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setTextureState( size_t stage, TextureState state, TextureStateValue value )
    {
        GN_ASSERT(
            stage < (size_t)MAX_TEXTURE_STAGES &&
            0 <= state && state < NUM_TEXTURE_STATES &&
            0 <= value && value < NUM_TEXTURE_STATE_VALUES );
        _GNGFX_CONTEXT_STATE( textureStates, textureStates.ts[stage][state], value );
    }

#undef _GNGFX_CONTEXT_STATE

    // *************************************************************************
    // ContextData methods
    // *************************************************************************

#define _GNGFX_CONTEXT_DATA( FLAG, NAME, VALUE ) if( (NAME) != (VALUE) ) { flags.FLAG = 1; (NAME) = (VALUE); } else void(0)

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setTexture( size_t stage, const Texture * tex )
    {
        GN_ASSERT( stage < (size_t)MAX_TEXTURE_STAGES );
        flags.textures = 1;
        textures[stage] = tex;
        ++stage;
        numTextures = max( stage, numTextures );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setTextureHandle( size_t stage, TextureDictionary::HandleType tex )
    {
        setTexture( stage, gTexDict.getResource(tex) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setTextures( const Texture * const texlist[], size_t start, size_t count )
    {
        for( size_t i = 0; i < count; ++i, ++start )
        {
            setTexture( start, texlist[i] );
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setTextureHandles( const TextureDictionary::HandleType texlist[], size_t start, size_t count )
    {
        for( size_t i = 0; i < count; ++i, ++start ) setTextureHandle( start, texlist[i] );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setVtxFmt( VtxFmtHandle h )
    {
        _GNGFX_CONTEXT_DATA( vtxFmt, vtxFmt, h );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setVtxBuf( size_t index, const VtxBuf * buffer, size_t stride )
    {
        GN_ASSERT( index < (size_t)MAX_VERTEX_STREAMS );
        flags.vtxBufs = 1;
        vtxBufs[index].buffer = buffer;
        vtxBufs[index].stride = stride;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setIdxBuf( const IdxBuf * idxBuf )
    {
        _GNGFX_CONTEXT_DATA( idxBuf, idxBuf, idxBuf );
    }

#undef _GNGFX_CONTEXT_DATA

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
    inline void Renderer::setRenderState( RenderState state, RenderStateValue value )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setRenderState( state, value ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderStates( const int * statePairs, size_t count )
    {
        _GNGFX_CONTEXT_UPDATE_STATE( setRenderStates( statePairs, count ) );
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

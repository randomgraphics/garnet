namespace GN { namespace gfx
{
    // *************************************************************************
    // resource management
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

#define _GNGFX_CONTEXT_STATE( FLAG, NAME, VALUE ) \
    mHelperContextState.flags.FLAG = 1; mHelperContextState.NAME = VALUE;

#define _GNGFX_CONTEXT_DATA( FLAG, NAME, VALUE ) \
    mHelperContextData.flags.FLAG = 1; mHelperContextData.NAME = VALUE;

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShader( ShaderType type, const Shader * shader )
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
    inline void Renderer::setShaders( const Shader * const shaders[] )
    {
        setShaders( shaders[0], shaders[1] );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShaders( const Shader * vtxShader, const Shader * pxlShader )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();

        _GNGFX_CONTEXT_STATE( vtxShader, shaders[0], vtxShader );
        _GNGFX_CONTEXT_STATE( pxlShader, shaders[1], pxlShader );

        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setShaderHandles( ShaderDictionary::HandleType vtxShader, ShaderDictionary::HandleType pxlShader )
    {
        setVtxShaderHandle( vtxShader );
        setPxlShaderHandle( pxlShader );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxShader( const Shader * s )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();
        _GNGFX_CONTEXT_STATE( vtxShader, shaders[0], s );
        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxShaderHandle( ShaderDictionary::HandleType h )
    {
        setVtxShader( gShaderDict.getResource(h) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setPxlShader( const Shader * s )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();
        _GNGFX_CONTEXT_STATE( pxlShader, shaders[1], s );
        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setPxlShaderHandle( ShaderDictionary::HandleType h )
    {
        setShader( PIXEL_SHADER, gShaderDict.getResource(h) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderStateBlock( RsbHandle rsb )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();
        _GNGFX_CONTEXT_STATE( rsb, rsb, rsb );
        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderState( RenderState state, RenderStateValue value )
    {
        int pair[2] = { state, value };
        setRenderStates( pair, 1 );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderStates( const int * statePairs, size_t count )
    {
        GN_GUARD_SLOW;

        GN_ASSERT( statePairs || !count );

        RenderStateBlockDesc desc = getCurrentRenderStateBlock();

        int state;
        int value;

        for( size_t i = 0; i < count; ++i )
        {
            state = *statePairs; ++statePairs;
            value = *statePairs; ++statePairs;

            GN_ASSERT( 0 <= state && state < NUM_RENDER_STATES );
            GN_ASSERT( 0 <= value && value < NUM_RENDER_STATE_VALUES );

            desc.rs[state] = (RenderStateValue)value;
        }

        RsbHandle handle = createRenderStateBlock( desc );
        if( handle ) setRenderStateBlock( handle );

        GN_UNGUARD_SLOW;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setViewport( const Rectf & viewport )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();
        _GNGFX_CONTEXT_STATE( viewport, viewport, viewport );
        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setViewport( float left, float top, float width, float height )
    {
        setViewport( Rectf( left, top, width, height ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureState( size_t stage, TextureState state, TextureStateValue value )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();

        GN_ASSERT(
            stage < (size_t)MAX_TEXTURE_STAGES &&
            0 <= state && state < NUM_TEXTURE_STATES &&
            0 <= value && value < NUM_TEXTURE_STATE_VALUES );

        _GNGFX_CONTEXT_STATE( textureStates, textureStates.ts[stage][state], value );

        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTexture( size_t stage, const Texture * tex )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();

        GN_ASSERT( stage < (size_t)MAX_TEXTURE_STAGES );
        
        mHelperContextData.flags.textures = 1;
        mHelperContextData.textures[stage] = tex;
        ++stage;
        mHelperContextData.numTextures = max( stage, mHelperContextData.numTextures );

        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureHandle( size_t stage, TextureDictionary::HandleType tex )
    {
        setTexture( stage, gTexDict.getResource(tex) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextures( const Texture * const texlist[], size_t start, size_t count )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();
        for( size_t i = 0; i < count; ++i, ++start )
        {
            setTexture( start, texlist[i] );
        }
        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setTextureHandles( const TextureDictionary::HandleType texlist[], size_t start, size_t count )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();
        for( size_t i = 0; i < count; ++i, ++start ) setTextureHandle( start, texlist[i] );
        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxFmt( VtxFmtHandle h )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();
        _GNGFX_CONTEXT_DATA( vtxFmt, vtxFmt, h );
        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setVtxBuf( size_t index, const VtxBuf * buffer, size_t stride )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();

        GN_ASSERT( index < (size_t)MAX_VERTEX_STREAMS );

        mHelperContextData.flags.vtxBufs = 1;
        mHelperContextData.vtxBufs[index].buffer = buffer;
        mHelperContextData.vtxBufs[index].stride = stride;

        _GNGFX_CONTEXT_UPDATE_END();
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setIdxBuf( const IdxBuf * idxBuf )
    {
        _GNGFX_CONTEXT_UPDATE_BEGIN();
        _GNGFX_CONTEXT_DATA( idxBuf, idxBuf, idxBuf );
        _GNGFX_CONTEXT_UPDATE_END();
    }

#undef _GNGFX_CONTEXT_UPDATE_BEGIN
#undef _GNGFX_CONTEXT_UPDATE_END
#undef _GNGFX_CONTEXT_STATE
#undef _GNGFX_CONTEXT_DATA
}}

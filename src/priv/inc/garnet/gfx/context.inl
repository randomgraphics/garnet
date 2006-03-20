namespace GN { namespace gfx
{
    // *************************************************************************
    // ContextState methods
    // *************************************************************************

#define _GNGFX_CONTEXT_STATE( FLAG, NAME, VALUE ) \
    if( (!flags.FLAG) || (NAME)!=(VALUE) ) { flags.FLAG = 1; (NAME) = (VALUE); } else void(0)

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setShader( ShaderType type, const Shader * shader )
    {
        GN_ASSERT( 0 <= type && type < NUM_SHADER_TYPES );
        GN_ASSERT( !shader || type == shader->getType() );
        if( !flags.shaderBit(type) || shaders[type] != shader )
        {
            flags.shaders |= 1<<type;
            shaders[type] = shader;
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
        setShader( VERTEX_SHADER, vtxShader );
        setShader( PIXEL_SHADER, pxlShader );
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
        setShader( VERTEX_SHADER, s );
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
        setShader( PIXEL_SHADER, s );
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
    inline void ContextState::setRenderState( RenderState state, int32_t value )
    {
        if( !flags.rsb || !rsb.isSet(state) || rsb.get(state) != value )
        {
            flags.rsb = 1;
            rsb.set( state, value );
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setColorBuffer( size_t index, const Texture * texture, size_t face, size_t level, size_t slice )
    {
        GN_ASSERT( index < MAX_RENDER_TARGETS );

        RenderTargetDesc::SurfaceDesc desc = { texture, face, level, slice };

        if( !flags.renderTargets || index >= renderTargets.numColorBuffers )
        {
            if( index >= renderTargets.numColorBuffers && NULL == texture ) return;
            renderTargets.colorBuffers[index] = desc;
            renderTargets.numColorBuffers = index + 1;
            flags.renderTargets = 1;
        }
        else if( NULL == texture )
        {
            renderTargets.numColorBuffers = index;
        }
        else
        {
            renderTargets.colorBuffers[index] = desc;
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setDepthBuffer( const Texture * texture, size_t face, size_t level, size_t slice )
    {
        RenderTargetDesc::SurfaceDesc desc = { texture, face, level, slice };
        _GNGFX_CONTEXT_STATE( renderTargets, renderTargets.depthBuffer, desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextState::setMsaa( MsaaType type )
    {
        _GNGFX_CONTEXT_STATE( renderTargets, renderTargets.msaa, type );
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
    inline void ContextState::setTextureStateBlock( const TextureStateBlockDesc & value )
    {
        _GNGFX_CONTEXT_STATE( tsb, tsb, value );
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
        if( !flags.tsb || tsb.isSet( stage, state ) || value != tsb.get( stage, state ) )
        {
            flags.tsb = 1;
            tsb.set( stage, state, value );
        }
    }

#undef _GNGFX_CONTEXT_STATE

    // *************************************************************************
    // ContextData methods
    // *************************************************************************

#define _GNGFX_CONTEXT_DATA( FLAG, NAME, VALUE ) \
    if( !flags.FLAG || (NAME) != (VALUE) ) { flags.FLAG = 1; (NAME) = (VALUE); } else void(0)

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setTexture( size_t stage, const Texture * tex )
    {
        GN_ASSERT( stage < (size_t)MAX_TEXTURE_STAGES );
        if( !flags.textures || stage >= numTextures )
        {
            if( stage >= numTextures && NULL == tex ) return;
            textures[stage] = tex;
            numTextures = stage + 1;
            flags.textures = 1;
        }
        else if( NULL == tex )
        {
            numTextures = stage;
        }
        else
        {
            textures[stage] = tex;
        }
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
        if( !flags.vtxBufs || index >= numVtxBufs )
        {
            if( index >= numVtxBufs && NULL == buffer ) return;
            vtxBufs[index].buffer = buffer;
            vtxBufs[index].stride = stride;
            numVtxBufs = index + 1;
            flags.vtxBufs = 1;
        }
        else if( NULL == buffer )
        {
            numVtxBufs = index;
        }
        else
        {
            vtxBufs[index].buffer = buffer;
            vtxBufs[index].stride = stride;
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void ContextData::setIdxBuf( const IdxBuf * idxBuf )
    {
        _GNGFX_CONTEXT_DATA( idxBuf, idxBuf, idxBuf );
    }

#undef _GNGFX_CONTEXT_DATA
}}

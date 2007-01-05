namespace GN { namespace gfx
{
    // *************************************************************************
    // RendererContext methods
    // *************************************************************************

#define _GNGFX_CONTEXT_UPDATE( FLAG, NAME, VALUE ) \
    if( (!this->flags.FLAG) || (this->NAME)!=(VALUE) ) { this->flags.FLAG = 1; (this->NAME) = (VALUE); } else void(0)

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setShader( ShaderType type, const Shader * shader )
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
    inline void RendererContext::setShaders( const Shader * const shaders[] )
    {
        setShaders( shaders[0], shaders[1] );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setShaders( const Shader * vtxShader, const Shader * pxlShader )
    {
        setShader( SHADER_VS, vtxShader );
        setShader( SHADER_PS, pxlShader );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setVS( const Shader * s )
    {
        setShader( SHADER_VS, s );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setPS( const Shader * s )
    {
        setShader( SHADER_PS, s );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setRenderStateBlock( const RenderStateBlockDesc & newRsb )
    {
        _GNGFX_CONTEXT_UPDATE( rsb, rsb, newRsb );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setRenderState( RenderState state, SInt32 value )
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
    inline void RendererContext::setColorBuffer( size_t index, const Texture * texture, size_t face, size_t level, size_t slice )
    {
        GN_ASSERT( index < MAX_RENDER_TARGETS );

        SurfaceDesc desc = { texture, face, level, slice };

        if( !flags.colorBuffers || index >= numColorBuffers )
        {
            flags.colorBuffers = 1;
            if( index >= numColorBuffers && NULL == texture ) return;
            colorBuffers[index] = desc;
            numColorBuffers = index + 1;
        }
        else if( NULL == texture )
        {
            numColorBuffers = index;
        }
        else
        {
            colorBuffers[index] = desc;
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setDepthBuffer( const Texture * texture, size_t face, size_t level, size_t slice )
    {
        SurfaceDesc desc = { texture, face, level, slice };
        _GNGFX_CONTEXT_UPDATE( depthBuffer, depthBuffer, desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setMsaa( MsaaType type )
    {
        _GNGFX_CONTEXT_UPDATE( msaa, msaa, type );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setViewport( const Rectf & vp )
    {
        _GNGFX_CONTEXT_UPDATE( viewport, viewport, vp );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setViewport( float left, float top, float width, float height )
    {
        setViewport( Rectf( left, top, width, height ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setTextureStateBlock( const TextureStateBlockDesc & value )
    {
        _GNGFX_CONTEXT_UPDATE( tsb, tsb, value );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setTextureState( size_t stage, TextureState state, TextureStateValue value )
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

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setTexture( size_t stage, const Texture * tex )
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
    inline void RendererContext::setTextures( const Texture * const texlist[], size_t start, size_t count )
    {
        for( size_t i = 0; i < count; ++i, ++start )
        {
            setTexture( start, texlist[i] );
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setVtxFmt( VtxFmtHandle h )
    {
        _GNGFX_CONTEXT_UPDATE( vtxFmt, vtxFmt, h );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setVtxBuf( size_t index, const VtxBuf * buffer, size_t stride )
    {
        GN_ASSERT( index < (size_t)MAX_VERTEX_ATTRIBUTES );
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
    inline void RendererContext::setIdxBuf( const IdxBuf * idxBuf )
    {
        _GNGFX_CONTEXT_UPDATE( idxBuf, idxBuf, idxBuf );
    }

#undef _GNGFX_CONTEXT_UPDATE
}}

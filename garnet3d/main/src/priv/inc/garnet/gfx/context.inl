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
    inline void RendererContext::setShaders( const Shader * vs, const Shader * ps, const Shader * gs )
    {
        setShader( SHADER_VS, vs );
        setShader( SHADER_PS, ps );
        setShader( SHADER_GS, gs );
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
    inline void RendererContext::setGS( const Shader * s )
    {
        setShader( SHADER_GS, s );
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
    inline void RendererContext::setRenderTargets( const RenderTargetDesc & desc )
    {
        GN_ASSERT( desc.valid() );
        _GNGFX_CONTEXT_UPDATE( renderTargets, renderTargets, desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setDrawToBackBuf()
    {
        setRenderTargets( RenderTargetDesc::DRAW_TO_BACK_BUFFER );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void RendererContext::setDrawToTexture(
            UInt32 count,
            const Texture * rt0,
            const Texture * rt1,
            const Texture * rt2,
            const Texture * rt3,
            const Texture * z,
            MsaaType aa )

    {
        RenderTargetDesc desc;
        desc.drawToBaseMap( count, rt0, rt1, rt2, rt3, z, aa );
        setRenderTargets( desc );
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

        flags.textures = 1;

        textures[stage] = tex;

        if( stage >= numTextures )
        {
            numTextures = stage + 1;
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

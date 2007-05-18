namespace GN { namespace engine
{
    // *************************************************************************
    // DrawContext methods
    // *************************************************************************

#define _GN_ENGINE_DRAWCONTEXT_UPDATE( FLAG, NAME, VALUE ) \
    if( (!this->flags.FLAG) || (this->NAME)!=(VALUE) ) { this->flags.FLAG = 1; (this->NAME) = (VALUE); } else void(0)

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setShader( gfx::ShaderType type, const GraphicsResource * shader )
    {
        GN_ASSERT( 0 <= type && type < gfx::NUM_SHADER_TYPES );
        GN_ASSERT( 0 == shader || GRT_SHADER == shader->desc.type );
        //GN_ASSERT( !shader || type == shader->getType() );
        if( !flags.shaderBit(type) || shaders[type] != shader )
        {
            flags.shaders |= 1<<type;
            shaders[type] = shader;
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setShaders( const GraphicsResource * vs, const GraphicsResource * ps, const GraphicsResource * gs )
    {
        setShader( gfx::SHADER_VS, vs );
        setShader( gfx::SHADER_PS, ps );
        setShader( gfx::SHADER_GS, gs );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setVS( const GraphicsResource * s )
    {
        setShader( gfx::SHADER_VS, s );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setPS( const GraphicsResource * s )
    {
        setShader( gfx::SHADER_PS, s );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setGS( const GraphicsResource * s )
    {
        setShader( gfx::SHADER_GS, s );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setRenderStateBlock( const gfx::RenderStateBlockDesc & newRsb )
    {
        _GN_ENGINE_DRAWCONTEXT_UPDATE( rsb, rsb, newRsb );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setRenderState( gfx::RenderState state, SInt32 value )
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
    inline void DrawContext::setRenderTargets( const RenderTargetDesc & desc )
    {
        GN_ASSERT( desc.valid() );
        _GN_ENGINE_DRAWCONTEXT_UPDATE( renderTargets, renderTargets, desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setDrawToBackBuf()
    {
        RenderTargetDesc rtd;
        rtd.drawToBackbuffer();
        setRenderTargets( rtd );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setDrawToTextures(
            UInt32 count,
            const GraphicsResource * rt0,
            const GraphicsResource * rt1,
            const GraphicsResource * rt2,
            const GraphicsResource * rt3,
            const GraphicsResource * z,
            gfx::MsaaType aa )

    {
        RenderTargetDesc desc;
        desc.drawToBaseMap( count, rt0, rt1, rt2, rt3, z, aa );
        setRenderTargets( desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setDrawToTextureWithoutDepth(
            const GraphicsResource * tex,
            UInt32 level,
            UInt32 face,
            UInt32 slice,
            gfx::MsaaType aa_ )
    {
        RenderTargetDesc desc;
        desc.count = 1;
        desc.aa = aa_;
        desc.setcbuf( 0, tex, level, face, slice );
        desc.setzbuf( 0 );
        setRenderTargets( desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setViewport( const Rectf & vp )
    {
        _GN_ENGINE_DRAWCONTEXT_UPDATE( viewport, viewport, vp );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setViewport( float left, float top, float width, float height )
    {
        setViewport( Rectf( left, top, width, height ) );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setTextureStateBlock( const gfx::TextureStateBlockDesc & value )
    {
        _GN_ENGINE_DRAWCONTEXT_UPDATE( tsb, tsb, value );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setTextureState( size_t stage, gfx::TextureState state, gfx::TextureStateValue value )
    {
        GN_ASSERT(
            stage < (size_t)gfx::MAX_TEXTURE_STAGES &&
            0 <= state && state < gfx::NUM_TEXTURE_STATES &&
            0 <= value && value < gfx::NUM_TEXTURE_STATE_VALUES );
        if( !flags.tsb || tsb.isSet( stage, state ) || value != tsb.get( stage, state ) )
        {
            flags.tsb = 1;
            tsb.set( stage, state, value );
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setTexture( size_t stage, const GraphicsResource * tex )
    {
        GN_ASSERT( stage < (size_t)gfx::MAX_TEXTURE_STAGES );

        flags.textures = 1;

        textures[stage] = tex;

        if( stage >= numTextures )
        {
            numTextures = (UInt32)( stage + 1 );
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setTextures( const GraphicsResource * const texlist[], size_t start, size_t count )
    {
        for( size_t i = 0; i < count; ++i, ++start )
        {
            setTexture( start, texlist[i] );
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setVtxFmt( const GraphicsResource * vtxfmt )
    {
        GN_ASSERT( 0 == vtxfmt || GRT_VTXFMT == vtxfmt->desc.type );
        _GN_ENGINE_DRAWCONTEXT_UPDATE( vtxfmt, vtxfmt, vtxfmt );
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setVtxBuf( size_t index, const GraphicsResource * buffer, size_t offset, size_t stride )
    {
        GN_ASSERT( index < (size_t)gfx::MAX_VERTEX_ATTRIBUTES );
        GN_ASSERT( 0 == buffer || GRT_VTXBUF == buffer->desc.type );
        if( !flags.vtxbufs || index >= numVtxBufs )
        {
            if( index >= numVtxBufs && NULL == buffer ) return;
            vtxbufs[index].buffer = buffer;
            vtxbufs[index].offset = (UInt32)offset;
            vtxbufs[index].stride = (UInt32)stride;
            numVtxBufs = (UInt32)( index + 1 );
            flags.vtxbufs = 1;
        }
        else if( NULL == buffer )
        {
            numVtxBufs = (UInt32)index;
        }
        else
        {
            vtxbufs[index].buffer = buffer;
            vtxbufs[index].offset = (UInt32)offset;
            vtxbufs[index].stride = (UInt32)stride;
        }
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void DrawContext::setIdxBuf( const GraphicsResource * idxbuf )
    {
        GN_ASSERT( 0 == idxbuf || GRT_IDXBUF == idxbuf->desc.type );
        _GN_ENGINE_DRAWCONTEXT_UPDATE( idxbuf, idxbuf, idxbuf );
    }

#undef _GN_ENGINE_DRAWCONTEXT_UPDATE
}}

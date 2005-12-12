// *****************************************************************************
// caps functions
// *****************************************************************************

namespace GN { namespace gfx
{
    // *************************************************************************
    // CAPS functions
    // *************************************************************************

    //
    inline void Renderer::CapsDesc::set( uint32_t v )
    {
        value = v;
#if GN_DEBUG
        valid = true;
#endif
    }
    //
    inline void Renderer::CapsDesc::reset()
    {
#if GN_DEBUG
        valid = false;
#endif
    }
    //
    inline void Renderer::setCaps( RendererCaps c, uint32_t v )
    {
        GN_GUARD;
        if( 0 <= c && c < NUM_CAPS )
        {
            mCaps[c].set(v);
        }
        else
        {
            GN_ERROR( "invalid caps" );
        }
        GN_UNGUARD;
    }
    //
    inline void Renderer::resetAllCaps()
    {
        GN_GUARD;
        for( size_t i = 0; i < NUM_CAPS; ++i )
        {
            mCaps[i].reset();
        }
        GN_UNGUARD;
    }

    // *************************************************************************
    // shader functions
    // *************************************************************************

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createShader( ShaderType      type,
                            ShadingLanguage lang,
                            const StrA &    code,
                            const StrA &    entry )
    {
        GN_GUARD;
        switch( type )
        {
            case VERTEX_SHADER : return createVtxShader( lang, code, entry );
            case PIXEL_SHADER  : return createPxlShader( lang, code, entry );
            default :
                GN_ERROR( "invalid shader type!" );
                return 0;
        }
        GN_UNGUARD;
    }

    // *************************************************************************
    // render parameter functions
    // *************************************************************************

    //
    //
    // -------------------------------------------------------------------------
    inline bool rpt2Str( StrA & result, RenderParameterType type )
    {
        static const char * sTable[] =
        {
            "RPT_TRANSFORM_WORLD",
            "RPT_TRANSFORM_VIEW",
            "RPT_TRANSFORM_PROJ",
            "RPT_TRANSFORM_VIEWPORT",
            "RPT_LIGHT0_POSITION",
            "RPT_LIGHT0_DIFFUSE",
            "RPT_MATERIAL_DIFFUSE",
            "RPT_MATERIAL_SPECULAR",
        };

        if( type < 0 || type >= NUM_RENDER_PARAMETER_TYPES ) return false;

        GN_ASSERT( type < sizeof(sTable)/sizeof(sTable[0]) );

        result = sTable[type];

        return true;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline const char * rpt2Str( RenderParameterType type )
    {
        static const char * sTable[] =
        {
            "RPT_TRANSFORM_WORLD",
            "RPT_TRANSFORM_VIEW",
            "RPT_TRANSFORM_PROJ",
            "RPT_TRANSFORM_VIEWPORT",
            "RPT_LIGHT0_POSITION",
            "RPT_LIGHT0_DIFFUSE",
            "RPT_MATERIAL_DIFFUSE",
            "RPT_MATERIAL_SPECULAR",
        };

        if( type < 0 || type >= NUM_RENDER_PARAMETER_TYPES )
            return "BAD_RENDER_PARAMETER_TYPE";

        GN_ASSERT( type < sizeof(sTable)/sizeof(sTable[0]) );

        return sTable[type];
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setRenderParameter(
        RenderParameterType type, const float * value, size_t count )
    {
        GN_GUARD_SLOW;

        if( isParameterCheckEnabled() )
        {
            if( type < 0 || type >= NUM_RENDER_PARAMETER_TYPES )
            {
                GN_ERROR( "invalid render parameter type : %d", type );
                return;
            }

            if( count > 16 )
            {
                GN_ERROR( "number of float can't be larger then 16" );
                return;
            }

            switch( type )
            {
                case RPT_LIGHT0_POSITION :
                case RPT_LIGHT0_DIFFUSE :
                case RPT_MATERIAL_DIFFUSE :
                case RPT_MATERIAL_SPECULAR :
                case RPT_TRANSFORM_VIEWPORT :
                    if( 4 != count )
                    {
                        GN_ERROR( "parameter %s requires 4 floats.", rpt2Str(type), type );
                        return;
                    }
                    break;

                case RPT_TRANSFORM_WORLD :
                case RPT_TRANSFORM_VIEW :
                case RPT_TRANSFORM_PROJ :
                    if( 16 != count )
                    {
                        GN_ERROR( "parameter %d requires 16 floats.", rpt2Str(type), type );
                        return;
                    }
                    break;

                default:
                    GN_ERROR( "invalid render parameter type : %d", type );
                    return;
            }
        }

        GN_ASSERT( 0 <= type && type < NUM_RENDER_PARAMETER_TYPES );
        GN_ASSERT( count <= 16 );

        RenderParameterValue & rpv = mRenderParameters[type].top();
        rpv.type = RPVT_FLOAT;
        rpv.count = count;
        ::memcpy( rpv.valueFloats, value, sizeof(float)*count );

        // update dirty set
        mRenderParameterDirtySet.insert( type );

        GN_UNGUARD_SLOW;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::pushRenderParameter( RenderParameterType type )
    {
        GN_GUARD_SLOW;

        if( isParameterCheckEnabled() )
        {
            if( ( type < 0 || type >= NUM_RENDER_PARAMETER_TYPES ) && !(type&(int)_RPM_MASK) )
            {
                GN_ERROR( "invalid render parameter type : %d", type );
                return;
            }
        }

#define _GNGFX_PUSH_SINGLE_RP( t ) \
    GN_ASSERT( 0 <= (int)(t) && (t) < NUM_RENDER_PARAMETER_TYPES ); \
    mRenderParameters[t].push();
#define _GNGFX_PUSH_RP_MASK( mask, t ) \
        if( t & (int)RPM_##mask ) \
        { \
            for( int i = RPT_##mask##_FIRST; i <= (int)RPT_##mask##_LAST; ++i ) \
            { \
                _GNGFX_PUSH_SINGLE_RP( (RenderParameterType)i ); \
            } \
        }

        if( type & (int)_RPM_MASK )
        {
            _GNGFX_PUSH_RP_MASK( LIGHT, type );
            _GNGFX_PUSH_RP_MASK( MATERIAL, type );
            _GNGFX_PUSH_RP_MASK( TRANSFORM, type );
        }
        else
        {
            _GNGFX_PUSH_SINGLE_RP(type);
        }
#undef _GNGFX_PUSH_SINGLE_RP
#undef _GNGFX_PUSH_RP_MASK

        GN_UNGUARD_SLOW;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::popRenderParameter( RenderParameterType type )
    {
        GN_GUARD_SLOW;

        if( isParameterCheckEnabled() )
        {
            if( ( type < 0 || type >= NUM_RENDER_PARAMETER_TYPES ) && !(type&(int)_RPM_MASK) )
            {
                GN_ERROR( "invalid render parameter type : %d", type );
                return;
            }
        }

#define _GNGFX_POP_SINGLE_RP( t ) \
    GN_ASSERT( 0 <= (t) && (t) < NUM_RENDER_PARAMETER_TYPES ); \
    mRenderParameters[t].pop(); \
    mRenderParameterDirtySet.insert( t );

#define _GNGFX_POP_RP_MASK( mask, t ) \
        if( t & (int)RPM_##mask ) \
        { \
            for( int i = RPT_##mask##_FIRST; i <= (int)RPT_##mask##_LAST; ++i ) \
            { \
                _GNGFX_POP_SINGLE_RP( (RenderParameterType)i ); \
            } \
        }

        if( type & (int)_RPM_MASK )
        {
            _GNGFX_POP_RP_MASK( LIGHT, type );
            _GNGFX_POP_RP_MASK( MATERIAL, type );
            _GNGFX_POP_RP_MASK( TRANSFORM, type );
        }
        else
        {
            _GNGFX_POP_SINGLE_RP( type );
        }
#undef _GNGFX_POP_SINGLE_RP
#undef _GNGFX_POP_RP_MASK
        GN_UNGUARD_SLOW;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::setViewport( float left, float top, float width, float height )
    {
        float vp[] = { left, top, width, height };
        setRenderParameter( RPT_TRANSFORM_VIEWPORT, vp, 4 );
    }
}}

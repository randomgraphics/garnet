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
    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::bindShaders( const Shader * vtxShader, const Shader * pxlShader )
    {
        GN_GUARD_SLOW;
        const Shader * shaders[NUM_SHADER_TYPES];
        shaders[VERTEX_SHADER] = vtxShader;
        shaders[PIXEL_SHADER] = pxlShader;
        bindShaders( shaders );
        GN_UNGUARD_SLOW;
    }
    //
    //
    // -------------------------------------------------------------------------
    inline void Renderer::bindShaderHandles( ShaderDictionary::HandleType vtxShader, ShaderDictionary::HandleType pxlShader )
    {
        GN_GUARD_SLOW;
        const Shader * shaders[NUM_SHADER_TYPES];
        shaders[VERTEX_SHADER] = 0 == vtxShader ? 0 : gShaderDict.getResource( vtxShader );
        shaders[PIXEL_SHADER]  = 0 == pxlShader ? 0 : gShaderDict.getResource( pxlShader );;
        bindShaders( shaders );
        GN_UNGUARD_SLOW;
    }
}}

//! \cond
namespace GN { namespace gfx
{
    //
    //
    // -----------------------------------------------------------------------------
    inline const char *
    shaderType2Str( ShaderType type )
    {
        static const char * sTable [] = { "VERTEX", "PIXEL" };
        if( 0 <= type && type < NUM_SHADER_TYPES )
            return sTable[type];
        else
            return "BAD_SHADER_TYPE";
    }
    //
    inline bool
    shaderType2Str( StrA & str, ShaderType type )
    {
        str = shaderType2Str( type );
        return "BAD_SHADER_TYPE" != str;
    }
    //
    inline bool
    str2ShaderType( ShaderType & type, const char * str )
    {
        static const char * sTable [] = { "VERTEX", "PIXEL" };
        if( str )
        {
            for( int i = 0; i < 2; ++i )
            {
                if( 0 == ::strcmp(str,sTable[i]) )
                {
                    type = static_cast<ShaderType>(i);
                    return true;
                }
            }
        }
        return false;
    }

    //
    //
    // -----------------------------------------------------------------------------
    inline bool
    shadingLanguage2Str( StrA & str, ShadingLanguage lang )
    {
        static const char * sTable [] =
        {
            "OGL_ARB",
            "OGL_GLSL",
            "D3D_ASM",
            "D3D_HLSL",
            "CG"
        };
        if( 0 <= lang && lang < NUM_SHADING_LANGUAGES )
        {
            str = sTable[lang];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    shadingLanguage2Str( ShadingLanguage lang )
    {
        static const char * sTable [] =
        {
            "OGL_ARB",
            "OGL_GLSL",
            "D3D_ASM",
            "D3D_HLSL",
            "CG",
        };
        if( 0 <= lang && lang < NUM_SHADING_LANGUAGES )
        {
            return sTable[lang];
        }
        else return "BAD_SHADING_LANGUAGE";
    }

    //
    //
    // -----------------------------------------------------------------------------
    inline bool
    str2ShadingLanguage( ShadingLanguage & lang, const char * str )
    {
        static const char * sTable [] =
        {
            "OGL_ARB",
            "OGL_GLSL",
            "D3D_ASM",
            "D3D_HLSL",
            "CG"
        };

        if( str )
        {
            for( int i = 0; i < 5; ++i )
            {
                if( 0 == ::strcmp(str,sTable[i]) )
                {
                    lang = static_cast<ShadingLanguage>(i);
                    return true;
                }
            }
        }
        return false;
    }

    //
    //
    // -----------------------------------------------------------------------------
    inline bool
    texType2Str( StrA & str, TexType textype )
    {
        static const char * sTable [] =
        { "1D", "2D", "3D", "CUBE" };

        if( 0 <= textype && textype < NUM_TEXTYPES )
        {
            str = sTable[textype];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    texType2Str( TexType textype )
    {
        static const char * sTable [] =
        { "1D", "2D", "3D", "CUBE" };

        if( 0 <= textype && textype < NUM_TEXTYPES )
        {
            return sTable[textype];
        }
        else return "BAD_TEXTURE_TYPE";
    }
    //
    inline bool
    str2TexType( TexType & value, const char * name )
    {
        static const char * sTable [] =
        { "1D", "2D", "3D", "CUBE" };

        if( name )
        {
            for( int i = 0; i < 4; ++i )
            {
                if( 0 == ::strcmp(sTable[i],name) )
                {
                    value = static_cast<TexType>(i);
                    return true;
                }
            }
        }
        return false;
    }

    //
    //
    // -----------------------------------------------------------------------------
    inline bool
    texFilter2Str( StrA & str, TexFilter filter )
    {
        static const char * sTable [] =
        {
            "NEAREST", "LINEAR",
            "N_MIP_N", "N_MIP_L",
            "L_MIP_N", "L_MIP_L",
        };

        if( 0 <= filter && filter < NUM_TEXFILTERS )
        {
            str = sTable[filter];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    texFilter2Str( TexFilter filter )
    {
        static const char * sTable [] =
        {
            "NEAREST", "LINEAR",
            "N_MIP_N", "N_MIP_L",
            "L_MIP_N", "L_MIP_L",
        };

        if( 0 <= filter && filter < NUM_TEXFILTERS )
        {
            return sTable[filter];
        }
        else return "BAD_TEXTURE_FILTER";
    }
    //
    inline bool
    str2TexFilter( TexFilter & value, const char * name )
    {
        static const char * sTable [] =
        {
            "NEAREST", "LINEAR",
            "N_MIP_N", "N_MIP_L",
            "L_MIP_N", "L_MIP_L",
        };

        if( name )
        {
            for( size_t i = 0; i < NUM_TEXFILTERS; ++i )
            {
                if( 0 ==::strcmp(sTable[i],name) )
                {
                    value = static_cast<TexFilter>(i);
                    return true;
                }
            }
        }
        return false;
    }

    //
    //
    // -----------------------------------------------------------------------------
    inline bool
    texWrap2Str( StrA & str, TexWrap texwrap )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( 0 <= texwrap && texwrap < NUM_TEXWRAPS )
        {
            str = sTable[texwrap];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    texWrap2Str( TexWrap texwrap )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( 0 <= texwrap && texwrap < NUM_TEXWRAPS )
        {
            return sTable[texwrap];
        }
        else return "BAD_TEXTURE_WRAP";
    }
    //
    inline bool
    str2TexWrap( TexWrap & value, const char * name )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( name )
        {
            for( size_t i = 0; i < NUM_TEXWRAPS; ++i )
            {
                if( 0 ==::strcmp(sTable[i],name) )
                {
                    value = static_cast<TexWrap>(i);
                    return true;
                }
            }
        }
        return false;
    }
}}
//! \endcond

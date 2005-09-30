//! \cond
namespace GN
{
    namespace gfx
    {
        //
        //
        // ----------------------------------------------------------------------------
        inline const char *
        shaderUsage2Str( ShaderUsage usage )
        {
            static const char * sTable [] = { "VERTEX", "PIXEL" };
            if( 0 <= usage && usage < NUM_SUSAGES )
                return sTable[usage];
            else
                return "BAD_SHADER_USAGE";
        }
        //
        inline bool
        shaderUsage2Str( StrA & str, ShaderUsage usage )
        {
            str = shaderUsage2Str( usage );
            return "BAD_SHADER_USAGE" != str;
        }
        //
        inline bool
        str2ShaderUsage( ShaderUsage & usage, const char * str )
        {
            static const char * sTable [] = { "VERTEX", "PIXEL" };
            if( str )
            {
                for( int i = 0; i < 2; ++i )
                {
                    if( 0 == ::strcmp(str,sTable[i]) )
                    {
                        usage = static_cast<ShaderUsage>(i);
                        return true;
                    }
                }
            }
            return false;
        }

        //
        //
        // ----------------------------------------------------------------------------
        inline bool
        shadingLanguage2Str( StrA & str, ShadingLanguage slang )
        {
            static const char * sTable [] =
            {
                "FFP",
                "OGL_ARB",
                "OGL_GLSL",
                "D3D_ASM",
                "D3D_HLSL",
                "CG"
            };
            if( 0 <= slang && slang < NUM_SLANGS )
            {
                str = sTable[slang];
                return true;
            }
            else return false;
        }
        //
        inline const char *
        shadingLanguage2Str( ShadingLanguage slang )
        {
            static const char * sTable [] =
            {
                "FFP",
                "OGL_ARB",
                "OGL_GLSL",
                "D3D_ASM",
                "D3D_HLSL",
                "CG",
            };
            if( 0 <= slang && slang < NUM_SLANGS )
            {
                return sTable[slang];
            }
            else return "BAD_SHADING_LANGUAGE";
        }

        //
        //
        // ----------------------------------------------------------------------------
        inline bool
        str2ShadingLanguage( ShadingLanguage & slang, const char * str )
        {
            static const char * sTable [] =
            {
                "FFP",
                "OGL_ARB",
                "OGL_GLSL",
                "D3D_ASM",
                "D3D_HLSL",
                "CG"
            };

            if( str )
            {
                for( int i = 0; i < 6; ++i )
                {
                    if( 0 == ::strcmp(str,sTable[i]) )
                    {
                        slang = static_cast<ShadingLanguage>(i);
                        return true;
                    }
                }
            }
            return false;
        }

        //
        //
        // ----------------------------------------------------------------------------
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
        // ----------------------------------------------------------------------------
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
        // ----------------------------------------------------------------------------
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
    }
}
//! \endcond

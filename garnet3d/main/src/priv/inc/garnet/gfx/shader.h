#ifndef __GN_GFX_SHADER_H__
#define __GN_GFX_SHADER_H__
// *****************************************************************************
//! \file    shader.h
//! \brief   Vertex and pixel shader interface
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! shader type
    //!
    enum ShaderType
    {
        VERTEX_SHADER,      //!< vertex shader
        PIXEL_SHADER,       //!< pixel shader
        NUM_SHADER_TYPES    //!< number of available shader types
    };

    //!
    //! shader language type
    //!
    enum ShadingLanguage
    {
        LANG_OGL_ARB,          //!< OpenGL ARB shader
        LANG_OGL_GLSL,         //!< OpenGL shading language
        LANG_D3D_ASM,          //!< DirectX ASM shader
        LANG_D3D_HLSL,         //!< DirectX high level shading language
        LANG_CG,               //!< Nvidia Cg
        NUM_SHADING_LANGUAGES  //!< number of available languages
    };

    //!
    //! General shader interface
    //!
    struct Shader : public RefCounter
    {
        //!
        //! apply shader as well to render device
        //!
        virtual void apply() const = 0;

        //!
        //! get shader type
        //!
        ShaderType getType() const { return mType; }

        //!
        //! get shading language
        //!
        ShadingLanguage getLang() const { return mLang; }

    protected :

        //!
        //! protected constructor
        //!
        //! \param type     Shader type
        //! \param lang     Shading language
        //!
        Shader( ShaderType type, ShadingLanguage lang )
            : mType(type)
            , mLang(lang)
        {
            GN_ASSERT( 0 <= type && type < NUM_SHADER_TYPES );
            GN_ASSERT( 0 <= lang && lang < NUM_SHADING_LANGUAGES );
        }

    private:

        ShaderType      mType;
        ShadingLanguage mLang;
    };

    //!
    //! \name convert between shader usage tags to string
    //@{

    inline const char *
    shaderType2Str( ShaderType type )
    {
        static const char * sTable [] = { "VERTEX", "PIXEL" };
        if( 0 <= type && type < NUM_SHADER_TYPES )
            return sTable[type];
        else
            return "BAD_SHADER_TYPE";
    }

    inline bool
    shaderType2Str( StrA & str, ShaderType type )
    {
        str = shaderType2Str( type );
        return "BAD_SHADER_TYPE" != str;
    }

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

    //@}
}}

// *****************************************************************************
//                           End of shader.h
// *****************************************************************************
#endif // __GN_GFX_SHADER_H__

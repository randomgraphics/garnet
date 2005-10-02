#ifndef __GN_GFX_GFXSHADER_H__
#define __GN_GFX_GFXSHADER_H__
// *****************************************************************************
//! \file    gfxShader.h
//! \brief   Vertex and pixel shader interface
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
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
}}

// *****************************************************************************
//                           End of gfxShader.h
// *****************************************************************************
#endif // __GN_GFX_GFXSHADER_H__

#ifndef __GN_GFX_SHADER_H__
#define __GN_GFX_SHADER_H__
// *****************************************************************************
/// \file
/// \brief   graphics shader interface
/// \author  chenli@@REDMOND (2008.12.9)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// Shading language
    ///
    enum ShadingLanguage
    {
        SL_ASM,  ///< assembly
        SL_HLSL, ///< D3D HLSL
        SL_GLSL, ///< OpenGL Shading language
        SL_CG,   ///< Nvidia Cg
    };

    ///
    /// Shader profile.
    ///
    /// Shader profile defines the minimum requirements that the hardware has to support
    ///
    enum ShaderProfile
    {
        SP_1,                ///< D3D shader model 1.1
        SP_2,                ///< D3D shader model 2.0
        SP_3,                ///< D3D shader model 3.0
        SP_4,                ///< D3D shader model 4.0
        SP_ARB1,             ///< OpenGL ARB program
        SP_GLSL,             ///< OpenGL GLSL 1.0
        SP_CG,               ///< Nvidia Cg
        NUM_SHADER_PROFILES, ///< number of shader profiles
    };

    ///
    /// shader description
    ///
    struct ShaderDesc
    {
        ShadingLanguage vsLang;
        const char *    vsCode;
        const char *    vsEntry;
        ShadingLanguage gsLang;
        const char *    gsCode;
        const char *    gsEntry;
        ShadingLanguage psLang;
        const char *    psCode;
        const char *    psEntry;
    };

    ///
    /// Contains platform dependent shader data, which can be used for fast shader creation
    ///
    struct CompiledShaderBlob : public RefCounter
    {
        const void * data() const = 0;
        size_t       size() const = 0;
    };

    ///
    /// Graphics shader interface
    ///
    struct Shader : public RefCounter
    {
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SHADER_H__

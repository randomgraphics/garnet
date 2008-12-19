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
        SP_D3D_1_1,          ///< D3D shader model 1.1
        SP_D3D_2_0,          ///< D3D shader model 2.0
        SP_D3D_3_0,          ///< D3D shader model 3.0
        SP_D3D_4_0,          ///< D3D shader model 4.0
        SP_OGL_ARB1,         ///< OpenGL ARB program
        SP_OGL_GLSL,         ///< OpenGL GLSL 1.0
        SP_CG,               ///< Nvidia Cg
        NUM_SHADER_PROFILES, ///< number of shader profiles
    };

    ///
    /// shader parameter description
    ///
    struct ShaderParamDesc
    {
    };

    ///
    /// shader code
    ///
    struct ShaderCode
    {
        ShadingLanguage lang;
        const char *    code;
        const char *    entry;
    };

    ///
    /// shader description
    ///
    struct ShaderDesc
    {
        ShaderCode              vscode;
        ShaderCode              gscode;
        ShaderCode              pscode;
        UInt32                  numParameters;
        const ShaderParamDesc * parameters;
    };

    ///
    /// Contains platform dependent shader data, which can be used for fast shader creation
    ///
    struct CompiledShaderBlob : public RefCounter
    {
        virtual const void * data() const = 0;
        virtual size_t       size() const = 0;
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

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
    enum GpuProgramLanguage
    {
        GPL_ASM,  ///< assembly
        GPL_HLSL, ///< D3D HLSL
        GPL_GLSL, ///< OpenGL Shading language
        GPL_CG,   ///< Nvidia Cg
        NUM_GPU_PROGRAM_LANGUAGES,
    };

    ///
    /// Shader profile.
    ///
    /// Shader profile defines the minimum requirements that the hardware has to support
    ///
    enum GpuProgramProfile
    {
        GPP_D3D_1_1,  ///< D3D shader model 1.1
        GPP_D3D_2_0,  ///< D3D shader model 2.0
        GPP_D3D_3_0,  ///< D3D shader model 3.0
        GPP_D3D_4_0,  ///< D3D shader model 4.0
        GPP_OGL_ARB1, ///< OpenGL ARB program
        GPP_OGL_GLSL, ///< OpenGL GLSL 1.0
        GPP_CG,       ///< Nvidia Cg
        NUM_GPU_PROGRAM_PROFILES,
    };

    ///
    /// GPU program parameter description
    ///
    struct GpuProgramParamDesc
    {
        const char * externalName;   ///< external name that is visible to user
        const char * internalNameVS; ///< internal name that is visible to vertex shader. Null means VS does not use this parameter.
        const char * internalNameGS; ///< internal name that is visible to vertex shader. Null means GS does not use this parameter.
        const char * internalNamePS; ///< internal name that is visible to vertex shader. Null means PS does not use this parameter.
    };

    ///
    /// shader code
    ///
    struct ShaderCode
    {
        GpuProgramLanguage lang;
        const char *       code;
        const char *       entry;
    };

    ///
    /// GPU program description
    ///
    struct GpuProgramDesc
    {
        ShaderCode                  codeVS;
        ShaderCode                  codeGS;
        ShaderCode                  codePS;
        UInt32                      numParameters;
        const GpuProgramParamDesc * parameters;
    };

    ///
    /// Contains platform dependent program data, which can be used for fast progra saving, loading and creation
    ///
    struct CompiledGpuProgram : public RefCounter
    {
        virtual const void * data() const = 0;
        virtual size_t       size() const = 0;
    };

    ///
    /// Graphics shader interface
    ///
    struct GpuProgram : public RefCounter
    {
        /// set shader parameter
        virtual void setParameter( size_t index, const void * data ) = 0;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SHADER_H__

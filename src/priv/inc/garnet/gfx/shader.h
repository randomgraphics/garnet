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
    /// shader code
    ///
    struct ShaderCode
    {
        const char * code;  ///< NULL terminated shader code
        const char * entry; ///< NULL terminated shader entry function

        /// default ctor
        ShaderCode() : code(NULL), entry(NULL) {}
    };

    ///
    /// GPU program description
    ///
    struct GpuProgramDesc
    {
        GpuProgramLanguage lang;  ///< shading language. Ignored when shader code is NULL.
        ShaderCode         vs;
        ShaderCode         gs;
        ShaderCode         ps;
    };

    ///
    /// GPU program parameter type
    ///
    enum GpuProgramParameterType
    {
        GPPT_UNIFORM, ///< uniform type
        GPPT_TEXTURE, ///< texture type
        GPPT_VERTEX,  ///< vertex type
    };

    ///
    /// GPU program parameter descrption
    ///
    struct GpuProgramParameterDesc
    {
        //@{
        size_t               numUniforms;
        const char * const * uniformNames;
        const size_t       * uniformSizes;
        //@}

        //@{
        size_t               numAttributes;
        const char * const * attributeNames;
         //@}

        //@{
        size_t               numTextures;
        const char * const * textureNames;
        //@}
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
        static const size_t PARAMETER_NOT_FOUND = (size_t)-1;

        ///
        /// get number of uniform parameters
        ///
        virtual const GpuProgramParameterDesc & getParameterDesc() const = 0;

        ///
        /// get number of uniforms
        ///
        size_t getNumUniforms() const { return getParameterDesc().numUniforms; }

        ///
        /// get index of uniform with specific name. Return PARAMETER_NOT_FOUND if the name is invalid.
        ///
        size_t getUniformIndex( const char * name ) const
        {
            const GpuProgramParameterDesc  & pd = getParameterDesc();
            for( size_t i = 0; i < pd.numUniforms; ++i )
            {
                if( 0 == strCmp( name, pd.uniformNames[i] ) )
                {
                    return i;
                }
            }
            GN_ERROR(getLogger("GN.gfx.GpuProgram"))( "Invalid uniform name: %s", name?name:"<NULLPTR>" );
            return PARAMETER_NOT_FOUND;
        }

        ///
        /// get number of textures
        ///
        size_t getNumTextures() const { return getParameterDesc().numTextures; }

        ///
        /// get index of texture with specific name. Return PARAMETER_NOT_FOUND if the name is invalid.
        ///
        size_t getTextureIndex( const char * name ) const
        {
            const GpuProgramParameterDesc  & pd = getParameterDesc();
            for( size_t i = 0; i < pd.numTextures; ++i )
            {
                if( 0 == strCmp( name, pd.textureNames[i] ) )
                {
                    return i;
                }
            }
            GN_ERROR(getLogger("GN.gfx.GpuProgram"))( "Invalid texture name: %s", name?name:"<NULLPTR>" );
            return PARAMETER_NOT_FOUND;
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SHADER_H__

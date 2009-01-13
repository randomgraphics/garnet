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
    /// GPU program parameter descrption
    ///
    struct GpuProgramParameterDesc
    {
        const char * name;     ///< NULL terminated parameter name
        size_t       length;   ///< default parameter size
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
        /// get number of parameters
        ///
        virtual size_t getNumParameters() const = 0;

        ///
        /// get parameter array
        ///
        virtual const GpuProgramParameterDesc * getParameters() const = 0;

        ///
        /// set shader parameter
        ///
        /// \param index        Parameter index in parameter array
        /// \param value        Parameter value
        /// \param length       Length in bytes of parameter value. Set to 0 to use inherited length.
        ///
        virtual void setParameter( size_t index, const void * value, size_t length = 0 ) = 0;

        ///
        /// get parameter index by name. Return -1 if the name is not found.
        ///
        size_t getParameterIndex( const char * name ) const
        {
            const GpuProgramParameterDesc * params = getParameters();
            size_t n = getNumParameters();
            for( size_t i = 0; i < n; ++i )
            {
                if( 0 == strCmp( name, params[i].name ) )
                {
                    return i;
                }
            }
            return PARAMETER_NOT_FOUND;
        }

        ///
        /// set shader parameter by name
        ///
        /// \param namex        Parameter name.
        /// \param value        Parameter value
        /// \param length       Length in bytes of parameter value. Set to 0 to use inherited length.
        ///
        void setParameter( const char * name, const void * value, size_t length = 0 )
        {
            size_t idx = getParameterIndex( name );

            if( PARAMETER_NOT_FOUND == idx )
            {
                GN_ERROR(getLogger("GN.gfx.rndr.GpuProgram"))( "invalid parameter name: %s", name );
                return;
            }

            setParameter( idx, value, length );
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SHADER_H__

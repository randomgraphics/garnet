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
    struct GpuProgramLanguage
    {
        enum Enum
        {
            INVALID = 0,    ///< Indicate invalid language

            HLSL9,          ///< HLSL for D3D9

            HLSL10,         ///< HLSL for D3D10+

            GLSL,           ///< OpenGL Shading language

            COUNT,          ///< Number of GPU program languages
        };

        /// check for validity
        bool valid() const
        {
            return HLSL9 == *this
                || HLSL10 == *this
                || GLSL == *this;
        }

        /// convert to string
        const char * toString() const
        {
            switch( *this )
            {
                case HLSL9     : return "HLSL9";
                case HLSL10    : return "HLSL10";
                case GLSL      : return "GLSL";
                default        : return "INVALID_GPU_PROGRAM_LANGUAGE";
            };
        }

        /// convert string to from string
        static GpuProgramLanguage sFromString( const char * s )
        {
                 if( 0 == str::compareI( s, "HLSL9" ) )     return HLSL9;
            else if( 0 == str::compareI( s, "HLSL10" ) )    return HLSL10;
            else if( 0 == str::compareI( s, "GLSL" ) )      return GLSL;
            else                                             return INVALID;
        }

        GN_DEFINE_ENUM_CLASS_HELPERS( GpuProgramLanguage, Enum )
    };

    ///
    /// Shader Stage
    ///
    struct ShaderStage
    {
        enum Enum
        {
            VS = 0, //< Vertex shader
            PS,     //< Pixel shader
            GS,     //< Geoemtry shader
            HS,     //< Hull (tessellation control) shader
            DS,     //< Domain (tessellation evaluation) shader

            COUNT,  //< Number of shader stages.
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( ShaderStage, Enum )
    };

    ///
    /// Shader model flags
    ///
    struct ShaderModel
    {
        enum Enum
        {
            SM_2_0     = 1<<0, //< D3D Shader Model 2.0 (D3D9)
            SM_3_0     = 1<<1, //< D3D Shader Model 3.0 (D3D9)
            SM_3_X     = 1<<2, //< D3D Shader Model for Xbox 360
            SM_4_0     = 1<<3, //< D3D Shader Model 4.0 (D3D10)
            SM_5_0     = 1<<4, //< D3D Shader Model 5.0 (D3D11)

            GLSL_1_10  = 1<<5, //< OpenGL Shading Language 1.10 (OpenGL 2.0+)
            GLSL_3_30  = 1<<6, //< OpenGL Shading Language 3.30 (OpenGL 3.3+)
        };

        static uint32 sFromString( const char * str )
        {
            if( NULL == str ) return 0;

            uint32 flags = 0;

            while( *str )
            {
                const char * s = str;
                const char * e = str;

                while( ' ' == *s || '\t' == *s || '\n' == *s ) ++s;

                e = s;

                while( *e && '|' != *e ) ++e;

                const char * t = e-1;
                while( ' ' == *t || '\t' == *t || '\n' == *t ) --t;

                size_t len = t-s+1;

                     if( 0 == str::compareI( "SM_2_0",    s, len ) ) flags |= SM_2_0;
                else if( 0 == str::compareI( "SM_3_0",    s, len ) ) flags |= SM_3_0;
                else if( 0 == str::compareI( "SM_3_X"   , s, len ) ) flags |= SM_3_X;
                else if( 0 == str::compareI( "SM_4_0",    s, len ) ) flags |= SM_4_0;
                else if( 0 == str::compareI( "SM_5_0",    s, len ) ) flags |= SM_5_0;
                else if( 0 == str::compareI( "GLSL_1_10", s, len ) ) flags |= GLSL_1_10;
                else if( 0 == str::compareI( "GLSL_3_30", s, len ) ) flags |= GLSL_3_30;

                GN_ASSERT( '|' == *e || 0 == *e );
                str = e;
                if( '|' == *e ) ++str;
            }

            return flags;
        }

        static StrA sToString( uint32 flags )
        {
            StrA str;

            if( flags & SM_2_0    ) { flags &= ~SM_2_0;    str += str.empty() ? "SM_2_0"    : "|SM_2_0"; }
            if( flags & SM_3_0    ) { flags &= ~SM_3_0;    str += str.empty() ? "SM_3_0"    : "|SM_3_0"; }
            if( flags & SM_3_X    ) { flags &= ~SM_3_X;    str += str.empty() ? "SM_3_X"    : "|SM_3_X"; }
            if( flags & SM_4_0    ) { flags &= ~SM_4_0;    str += str.empty() ? "SM_4_0"    : "|SM_4_0"; }
            if( flags & SM_5_0    ) { flags &= ~SM_5_0;    str += str.empty() ? "SM_5_0"    : "|SM_5_0"; }
            if( flags & GLSL_1_10 ) { flags &= ~GLSL_1_10; str += str.empty() ? "GLSL_1_10" : "|GLSL_1_10"; }
            if( flags & GLSL_3_30 ) { flags &= ~GLSL_3_30; str += str.empty() ? "GLSL_3_30" : "|GLSL_3_30"; }

            if( flags )
            {
                if(!str.empty()) str += "|";
                str += str::format( "0x%X", flags );
            }

            return str;
        }

        GN_DEFINE_ENUM_CLASS_HELPERS( ShaderModel, Enum )
    };

    ///
    /// shader code
    ///
    struct ShaderCode
    {
        const char * source; ///< NULL terminated shader source. Set to NULL to use fixed functional pipeline.
        const char * entry;  ///< NULL terminated shader entry function (ignored for ASM shader code)
    };

    ///
    /// GPU program description
    ///
    struct GpuProgramDesc
    {
        GpuProgramLanguage lang;         //< shading language.
        uint32             shaderModels; //< shader models that the program is written against.

        union
        {

        ShaderCode         code[ShaderStage::COUNT]; //< shader code for each shader stage

        struct
        {

        ShaderCode         vs; //< Vertex shader source code
        ShaderCode         ps; //< Pixel shader source code
        ShaderCode         gs; //< Geometry shader source code
        ShaderCode         hs; //< Hull shader source code
        ShaderCode         ds; //< Domain shadser source code

        };
        };

        const char * name; // optional prgram name. for debugging/logging only.

        /// compile options
        //@{
        bool optimize; //< generate optimized shader. Default is on.
        bool debug;    //< generate debug symbol. Default is on.
        //@}

        ///
        /// default constructor
        ///
        GpuProgramDesc(const char * name_ = nullptr)
            : lang(GpuProgramLanguage::INVALID)
            , shaderModels(0)
            , name(name_)
            , optimize(true)
            , debug(true)
        {
            for( int i = 0; i < ShaderStage::COUNT; ++i )
            {
                code[i].source = NULL;
                code[i].entry = NULL;
            }
        }
    };

    ///
    /// GPU program parameter type
    ///
    struct GpuProgramParameterType
    {
        enum Enum
        {
            UNIFORM, ///< uniform type
            TEXTURE, ///< texture type
            VERTEX,  ///< vertex type
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( GpuProgramParameterType, Enum );
    };

    ///
    /// GPU program uniform description
    ///
    struct GpuProgramUniformParameterDesc
    {
        const char * name; ///< uniform name
        uint32       size; ///< uniform size in bytes
    };

    ///
    /// GPU program texture parameter desc
    struct GpuProgramTextureParameterDesc
    {
        const char * name; ///< texture name
    };

    ///
    /// GPU program attribute (input vertex) description
    ///
    struct GpuProgramAttributeParameterDesc
    {
        const char * name; ///< attribute name.
    };

    enum
    {
        /// indicate a invalid parameter index
        GPU_PROGRAM_PARAMETER_NOT_FOUND = (uint16)-1
    };

    ///
    /// GPU program parameter accessor template
    ///
    template<class PARAMETER_DESC_CLASS>
    class GpuProgramParameterAccessor
    {
        const uint8 * & mData;
        const uint32  & mCount;
        const uint32  & mStride;

    public:

        ///
        /// constructor
        ///
        GpuProgramParameterAccessor(
            const PARAMETER_DESC_CLASS * & data,
            const uint32 & count,
            const uint32 & stride )
            : mData((const uint8*&)data), mCount(count), mStride(stride)
        {
        }

        ///
        /// return number of parameters
        ///
        uint32 count() const { return mCount; }

        ///
        /// bracket operator. index must be valid
        ///
        const PARAMETER_DESC_CLASS & operator[]( size_t index ) const
        {
            // must be a valid index
            GN_ASSERT( index < mCount );

            // Note: stride must be larger than size of parameter class
            GN_ASSERT( mStride >= sizeof(PARAMETER_DESC_CLASS) );

            const PARAMETER_DESC_CLASS * p = (const PARAMETER_DESC_CLASS *)(mData + mStride * index);

            return *p;
        }

        ///
        /// Look up parameter with specific name, return GPU_PROGRAM_PARAMETER_NOT_FOUND for invalid name
        ///
        uint16 operator[]( const char * name ) const
        {
            // Note: stride must be larger than size of parameter class
            GN_ASSERT( mStride >= sizeof(PARAMETER_DESC_CLASS) );

            const uint8 * p = mData;
            for( uint32 i = 0; i < mCount; ++i, p+=mStride )
            {
                /// Assume that the first member of PARAMETER_DESC_CLASS is always parameter name
                const char * paramName = *(const char * const *)p;

                if( 0 == str::compare( name, paramName ) )
                {
                    // got you!
                    return (uint16)i;
                }
            }
            GN_ERROR(getLogger("GN.gfx.GpuProgram.GpuProgramParameterDesc"))(
                "Invalid GPU program parameter name: %s", name?name:"<NULLPTR>" );
            return (uint16)GPU_PROGRAM_PARAMETER_NOT_FOUND;
        }
    };

    ///
    /// GPU program parameter descrption
    ///
    class GpuProgramParameterDesc
    {
    public:

        /// parameter accessors
        ///@{
        GpuProgramParameterAccessor<GpuProgramUniformParameterDesc>   uniforms;
        GpuProgramParameterAccessor<GpuProgramTextureParameterDesc>   textures;
        GpuProgramParameterAccessor<GpuProgramAttributeParameterDesc> attributes;
        ///@}

        ///
        /// constructor
        ///
        GpuProgramParameterDesc()
            : uniforms( mUniformArray, mUniformCount, mUniformArrayStride )
            , textures( mTextureArray, mTextureCount, mTextureArrayStride )
            , attributes( mAttributeArray, mAttributeCount, mAttributeArrayStride )
            , mUniformCount(0)
            , mTextureCount(0)
            , mAttributeCount(0)
        {
        }

        ///
        /// destructor
        ///
        virtual ~GpuProgramParameterDesc() {}

    protected:

        // Note: it is subclass's responsibility to initialize these data members.

        /// uniform parameters
        //@{
        const GpuProgramUniformParameterDesc *   mUniformArray;
        uint32                                   mUniformCount;
        uint32                                   mUniformArrayStride;
        //@}

        /// texture parameters
        //@{
        const GpuProgramTextureParameterDesc   * mTextureArray;
        uint32                                   mTextureCount;
        uint32                                   mTextureArrayStride;
        //@}

        /// attribute parameters
        //@{
        const GpuProgramAttributeParameterDesc * mAttributeArray;
        uint32                                   mAttributeCount;
        uint32                                   mAttributeArrayStride;
        //@}
    };

    ///
    /// Graphics shader interface
    ///
    struct GpuProgram : public RefCounter
    {
        ///
        /// get GPU program parameters
        ///
        virtual const GpuProgramParameterDesc & getParameterDesc() const = 0;

    protected:

        /// protected ctor
        GpuProgram() {}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SHADER_H__

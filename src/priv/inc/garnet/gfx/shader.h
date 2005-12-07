#ifndef __GN_GFX_SHADER_H__
#define __GN_GFX_SHADER_H__
// *****************************************************************************
//! \file    shader.h
//! \brief   Vertex and pixel shader interface
//! \author  chenlee (2005.9.30)
// *****************************************************************************

#include <vector>
#include <map>
#include <set>

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
        //! get shader type
        //!
        ShaderType getType() const { return mType; }

        //!
        //! get shading language
        //!
        ShadingLanguage getLang() const { return mLang; }

        //!
        //! Get handle of uniform variable
        //!
        uint32_t getUniformHandle( const char * );

        //!
        //! \name Set value of uniform variable.
        //!
        //@{
        void setUniform( uint32_t, const bool *, size_t );
        void setUniform( uint32_t, const int32_t *, size_t );
        void setUniform( uint32_t, const float *, size_t );
        void setUniform( uint32_t, const Vector4f *, size_t );
        void setUniform( uint32_t, const Matrix44f *, size_t ); //!< \note Matrix should be row major

        void setUniform( uint32_t, bool );
        void setUniform( uint32_t, int32_t );
        void setUniform( uint32_t, float );
        void setUniform( uint32_t, const Vector4f & );
        void setUniform( uint32_t, const Matrix44f & ); //!< \note Matrix should be row major
        //@}

        //!
        //! \name Set uniform variable by name
        //!
        //@{
        void setUniformByName( const char *, const bool *, size_t );
        void setUniformByName( const char *, const int32_t *, size_t );
        void setUniformByName( const char *, const float *, size_t );
        void setUniformByName( const char *, const Vector4f *, size_t );
        void setUniformByName( const char *, const Matrix44f *, size_t );

        void setUniformByName( const char *, bool );
        void setUniformByName( const char *, int32_t );
        void setUniformByName( const char *, float );
        void setUniformByName( const char *, const Vector4f & );
        void setUniformByName( const char *, const Matrix44f & );
        //@}

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

        //!
        //! Uniform value type
        //!
        enum UniformValueType
        {
            UVT_BOOL,     //!< boolean
            UVT_INT,      //!< signed integer (32-bit)
            UVT_FLOAT,    //!< single precision floating point
            UVT_FLOAT4,   //!< 4 floats
            UVT_MATRIX44, //!< 4x4 row major matrix
            NUM_UNIFORM_VALUE_TYPES, //!< number of avaliable types.
        };

        //!
        //! Uniform structure
        //!
        struct Uniform
        {
            StrA                   name;          //!< uniform name
            UniformValueType       type;          //!< uniform type
            std::vector<uint8_t>   valueBool;     //!< Boolean value
            std::vector<int32_t>   valueInt;      //!< integer value
            std::vector<float>     valueFloat;    //!< float value
            std::vector<Vector4f>  valueVector4;  //!< vector value
            std::vector<Matrix44f> valueMatrix44; //!< matrix value
            HandleType             userData;      //!< User-defined data
        };

        //!
        //! Get first unform handle. Use to iterate all uniforms.
        //!
        uint32_t getFirstUniform() const { return mUniforms.first(); }

        //!
        //! Get next unform handle. Use to iterate all uniforms.
        //!
        uint32_t getNextUniform( uint32_t handle ) const { return mUniforms.next(handle); }

        //!
        //! Get the uniform data structure
        //!
        Uniform & getUniform( uint32_t handle )
        {
            GN_ASSERT( mUniforms.validHandle(handle) );
            return mUniforms[handle];
        }

        //!
        //! Get the uniform data structure
        //!
        const Uniform & getUniform( uint32_t handle ) const
        {
            GN_ASSERT( mUniforms.validHandle(handle) );
            return mUniforms[handle];
        }

        //!
        //! get list of dirty uniforms
        //!
        const std::set<uint32_t> & getDirtyUniforms() const { return mDirtySet; }

        //!
        //! clear dirty set
        //!
        void clearDirtySet() const { mDirtySet.clear(); }

    private:

        //!
        //! Query device-dependent uniform data
        //!
        //! \param name
        //!     uniform name
        //! \param userData
        //!     Return user-defined uniform data that will be stored in Uniform::userData.
        //!     Ignored if set to NULL.
        //!
        virtual bool queryDeviceUniform( const char * name, HandleType & userData ) const = 0;

        //!
        //! Add a new uniform to uniform list. Return handle of the uniform.
        //!
        uint32_t addUniform( const StrA & name )
        {
            GN_GUARD;

            if( name.empty() )
            {
                GN_ERROR( "uniform name can't be empty!" );
                return 0;
            }

            if( mUniformNames.end() != mUniformNames.find(name) )
            {
                GN_ERROR( "uniform named '%s' already exists.", name.cstr() );
                return 0;
            }

            Uniform u;
            u.name = name;

            uint32_t h = mUniforms.add( u );
            mUniformNames[name] = h;

            // success
            return h;

            GN_UNGUARD;
        }

        bool validateUniformValue( uint32_t handle, const void * values, size_t count )
        {
            if( !mUniforms.validHandle( handle ) )
            {
                GN_ERROR( "invalid uniform handle '%d'", handle );
                return false;
            }
            if( 0 == values && 0 != count )
            {
                GN_ERROR( "values is NULL, but count is not zero.'" );
                return false;
            }
            return true;
        }

        ShaderType      mType; //!< shader type
        ShadingLanguage mLang; //!< shading language

        HandleManager<Uniform,uint32_t> mUniforms;     //!< uniform handle manager
        std::map<StrA,uint32_t>         mUniformNames; //!< uniform name -> uniform handle
        mutable std::set<uint32_t>      mDirtySet;     //!< Store handle of dirty uniforms.
    };

    // *************************************************************************
    // inline methods
    // *************************************************************************

    // -------------------------------------------------------------------------
    inline uint32_t Shader::getUniformHandle( const char * name )
    {
        GN_GUARD_SLOW;

        // check parameter(s)
        if( strEmpty(name) ) { GN_ERROR( "Uniform name can't be empty!" ); return 0; }

        // check for existing uniform
        if( mUniformNames.end() != mUniformNames.find(name) )
        {
            GN_ASSERT( mUniforms.validHandle( mUniformNames[name] ) );
            return mUniformNames[name];
        }

        // query for device-dependent uniform
        HandleType userData;
        if( !queryDeviceUniform( name, userData ) )
        {
            GN_ERROR( "invalid uniform name: %s.", name );
            return 0;
        }

        // add new uniform
        uint32_t handle = addUniform( name );
        if( 0 == handle ) return 0;
        Uniform & u = getUniform(handle);
        GN_ASSERT( u.name == name );
        u.userData = userData;

        // success
        return handle;

        GN_UNGUARD_SLOW;
    }

    // -------------------------------------------------------------------------
    inline void Shader::setUniform( uint32_t handle, const bool * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !validateUniformValue( handle, values, count ) ) return;
        Uniform & u = mUniforms[handle];
        u.type = UVT_BOOL;
        u.valueBool.resize( count );
        if( count > 0 ) ::memcpy( &u.valueBool[0], values, count*sizeof(bool) );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniform( uint32_t handle, const int32_t * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !validateUniformValue( handle, values, count ) ) return;
        Uniform & u = mUniforms[handle];
        u.type = UVT_INT;
        u.valueInt.resize( count );
        if( count > 0 ) ::memcpy( &u.valueInt[0], values, count*sizeof(int32_t) );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniform( uint32_t handle, const float * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !validateUniformValue( handle, values, count ) ) return;
        Uniform & u = mUniforms[handle];
        u.type = UVT_FLOAT;
        u.valueFloat.resize( count );
        if( count > 0 ) ::memcpy( &u.valueFloat[0], values, count*sizeof(float) );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniform( uint32_t handle, const Vector4f * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !validateUniformValue( handle, values, count ) ) return;
        Uniform & u = mUniforms[handle];
        u.type = UVT_FLOAT4;
        u.valueVector4.resize( count );
        if( count > 0 ) ::memcpy( &u.valueVector4[0], values, count*sizeof(Vector4f) );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniform( uint32_t handle, const Matrix44f * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !validateUniformValue( handle, values, count ) ) return;
        Uniform & u = mUniforms[handle];
        u.type = UVT_MATRIX44;
        u.valueMatrix44.resize( count );
        if( count > 0 ) ::memcpy( &u.valueMatrix44[0], values, count*sizeof(Matrix44f) );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }

    // -------------------------------------------------------------------------
    inline void Shader::setUniform( uint32_t handle, bool value )
    {
        setUniform( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniform( uint32_t handle, int32_t value )
    {
        setUniform( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniform( uint32_t handle, float value )
    {
        setUniform( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniform( uint32_t handle, const Vector4f & value )
    {
        setUniform( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniform( uint32_t handle, const Matrix44f & value )
    {
        setUniform( handle, &value, 1 );
    }

    // -------------------------------------------------------------------------
    inline void  Shader::setUniformByName( const char * name, bool value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, value );
    }
    // --
    inline void  Shader::setUniformByName( const char * name, int32_t value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, value );
    }
    // --
    inline void  Shader::setUniformByName( const char * name, float value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, value );
    }
    // --
    inline void  Shader::setUniformByName( const char * name, const Vector4f & value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, value );
    }
    // --
    inline void  Shader::setUniformByName( const char * name, const Matrix44f & value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, value );
    }

    // -------------------------------------------------------------------------
    inline void  Shader::setUniformByName( const char * name, const bool * values, size_t count )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, values, count );
    }
    // --
    inline void  Shader::setUniformByName( const char * name, const int32_t * values, size_t count )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, values, count );
    }
    // --
    inline void  Shader::setUniformByName( const char * name, const float * values, size_t count )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, values, count );
    }
    // --
    inline void  Shader::setUniformByName( const char * name, const Vector4f * values, size_t count )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, values, count );
    }
    // --
    inline void  Shader::setUniformByName( const char * name, const Matrix44f * values, size_t count )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, values, count );
    }

    // *************************************************************************
    // Util functions
    // *************************************************************************

    //!
    //! \name convert between shader usage tags to string
    //@{

    // -------------------------------------------------------------------------
    inline const char *
    shaderType2Str( ShaderType type )
    {
        static const char * sTable [] = { "VERTEX", "PIXEL" };
        if( 0 <= type && type < NUM_SHADER_TYPES )
            return sTable[type];
        else
            return "BAD_SHADER_TYPE";
    }

    // -------------------------------------------------------------------------
    inline bool
    shaderType2Str( StrA & str, ShaderType type )
    {
        str = shaderType2Str( type );
        return "BAD_SHADER_TYPE" != str;
    }

    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
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

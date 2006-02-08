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
    //! Uniform value type
    //!
    enum UniformValueType
    {
        UVT_BOOL,     //!< boolean
        UVT_INT,      //!< signed integer (32-bit)
        UVT_FLOAT,    //!< single precision floating point
        UVT_VECTOR4,  //!< 4 floats
        UVT_MATRIX44, //!< 4x4 row major matrix
        NUM_UNIFORM_VALUE_TYPES, //!< number of avaliable types.
    };

    //!
    //! Uniform value structure
    //!
    struct UniformValue
    {
        UniformValueType       type;      //!< value type
        std::vector<int32_t>   bools;     //!< Boolean value
        std::vector<int32_t>   ints;      //!< integer value
        std::vector<float>     floats;    //!< float value
        std::vector<Vector4f>  vector4s;  //!< vector value
        std::vector<Matrix44f> matrix44s; //!< matrix value

        //! \name set uniform value
        //@{
        void setB( const int32_t *, size_t );
        void setI( const int32_t *, size_t );
        void setF( const float *, size_t );
        void setV( const Vector4f *, size_t );
        void setM( const Matrix44f *, size_t ); //!< \note Matrix should be row major
        //@}
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
        void setUniform( uint32_t, const UniformValue & );
        
        void setUniformB( uint32_t, const int32_t *, size_t );
        void setUniformI( uint32_t, const int32_t *, size_t );
        void setUniformF( uint32_t, const float *, size_t );
        void setUniformV( uint32_t, const Vector4f *, size_t );
        void setUniformM( uint32_t, const Matrix44f *, size_t ); //!< \note Matrix should be row major

        void setUniformB( uint32_t, bool );
        void setUniformI( uint32_t, int32_t );
        void setUniformF( uint32_t, float );
        void setUniformV( uint32_t, const Vector4f & );
        void setUniformM( uint32_t, const Matrix44f & ); //!< \note Matrix should be row major
        //@}

        //!
        //! \name Set uniform variable by name
        //!
        //@{
        void setUniformByName( const char *, const UniformValue & );

        void setUniformByNameB( const char *, const int32_t *, size_t );
        void setUniformByNameI( const char *, const int32_t *, size_t );
        void setUniformByNameF( const char *, const float *, size_t );
        void setUniformByNameV( const char *, const Vector4f *, size_t );
        void setUniformByNameM( const char *, const Matrix44f *, size_t );

        void setUniformByNameB( const char *, bool );
        void setUniformByNameI( const char *, int32_t );
        void setUniformByNameF( const char *, float );
        void setUniformByNameV( const char *, const Vector4f & );
        void setUniformByNameM( const char *, const Matrix44f & );
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
        //! Uniform structure
        //!
        struct Uniform
        {
            StrA         name;     //!< uniform name
            UniformValue value;    //!< uniform value
            HandleType   userData; //!< User-defined data
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

        bool checkUniformHandle( uint32_t handle )
        {
            if( !mUniforms.validHandle( handle ) )
            {
                GN_ERROR( "invalid uniform handle '%d'", handle );
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
    inline void UniformValue::setB( const int32_t * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_BOOL;
        bools.resize( count );
        if( count > 0 && values ) ::memcpy( &bools[0], values, count*sizeof(int32_t) );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void UniformValue::setI( const int32_t * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_INT;
        ints.resize( count );
        if( count > 0 && values ) ::memcpy( &ints[0], values, count*sizeof(int32_t) );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void UniformValue::setF( const float * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_FLOAT;
        floats.resize( count );
        if( count > 0 && values ) ::memcpy( &floats[0], values, count*sizeof(float) );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void UniformValue::setV( const Vector4f * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_VECTOR4;
        vector4s.resize( count );
        if( count > 0 && values ) ::memcpy( &vector4s[0], values, count*sizeof(Vector4f) );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void UniformValue::setM( const Matrix44f * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_MATRIX44;
        matrix44s.resize( count );
        if( count > 0 && values ) ::memcpy( &matrix44s[0], values, count*sizeof(Matrix44f) );
        GN_UNGUARD_SLOW;
    }

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
    inline void Shader::setUniform( uint32_t handle, const UniformValue & value )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value = value;
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformB( uint32_t handle, const int32_t * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setB( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformI( uint32_t handle, const int32_t * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setI( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformF( uint32_t handle, const float * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setF( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformV( uint32_t handle, const Vector4f * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setV( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformM( uint32_t handle, const Matrix44f * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setM( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }

    // -------------------------------------------------------------------------
    inline void Shader::setUniformB( uint32_t handle, bool value )
    {
        int32_t i = value;
        setUniformB( handle, &i, 1 );
    }
    // --
    inline void Shader::setUniformI( uint32_t handle, int32_t value )
    {
        setUniformI( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniformF( uint32_t handle, float value )
    {
        setUniformF( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniformV( uint32_t handle, const Vector4f & value )
    {
        setUniformV( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniformM( uint32_t handle, const Matrix44f & value )
    {
        setUniformM( handle, &value, 1 );
    }

    // -------------------------------------------------------------------------
    inline void  Shader::setUniformByNameB( const char * name, bool value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformB( handle, value );
    }
    // --
    inline void  Shader::setUniformByNameI( const char * name, int32_t value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformI( handle, value );
    }
    // --
    inline void  Shader::setUniformByNameF( const char * name, float value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformF( handle, value );
    }
    // --
    inline void  Shader::setUniformByNameV( const char * name, const Vector4f & value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformV( handle, value );
    }
    // --
    inline void  Shader::setUniformByNameM( const char * name, const Matrix44f & value )
    {
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformM( handle, value );
    }

    // -------------------------------------------------------------------------
    inline void Shader::setUniformByName( const char * name, const UniformValue & value )
    {
        GN_GUARD_SLOW;
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, value );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameB( const char * name, const int32_t * values, size_t count )
    {
        GN_GUARD_SLOW;
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformB( handle, values, count );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameI( const char * name, const int32_t * values, size_t count )
    {
        GN_GUARD_SLOW;
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformI( handle, values, count );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameF( const char * name, const float * values, size_t count )
    {
        GN_GUARD_SLOW;
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformF( handle, values, count );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameV( const char * name, const Vector4f * values, size_t count )
    {
        GN_GUARD_SLOW;
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformV( handle, values, count );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameM( const char * name, const Matrix44f * values, size_t count )
    {
        GN_GUARD_SLOW;
        uint32_t handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformM( handle, values, count );
        GN_UNGUARD_SLOW;
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

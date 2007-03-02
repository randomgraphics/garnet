#ifndef __GN_GFX_SHADER_H__
#define __GN_GFX_SHADER_H__
// *****************************************************************************
/// \file    shader.h
/// \brief   Vertex and pixel shader interface
/// \author  chenlee (2005.9.30)
// *****************************************************************************

#include <map>
#include <set>

namespace GN { namespace gfx
{
    ///
    /// shader type
    ///
    enum ShaderType
    {
        SHADER_VS,        ///< vertex shader
        SHADER_PS,        ///< pixel shader
        SHADER_GS,        ///< geometry shader (D3D10 only)
        NUM_SHADER_TYPES  ///< number of available shader types
    };

    ///
    /// shader language type
    ///
    enum ShadingLanguage
    {
        LANG_OGL_ARB,          ///< OpenGL ARB shader
        LANG_OGL_GLSL,         ///< OpenGL shading language
        LANG_D3D_ASM,          ///< DirectX ASM shader
        LANG_D3D_HLSL,         ///< DirectX high level shading language
        LANG_CG,               ///< Nvidia Cg
        NUM_SHADING_LANGUAGES  ///< number of available languages
    };

    ///
    /// General shader interface
    ///
    struct Shader : public RefCounter
    {
        ///
        /// get shader type
        ///
        ShaderType getType() const { return mType; }

        ///
        /// get shading language
        ///
        ShadingLanguage getLang() const { return mLang; }

        ///
        /// Get handle of uniform variable
        ///
        UInt32 getUniformHandle( const StrA & );

        ///
        /// \name Set value of uniform variable.
        ///
        //@{
        void setUniform( UInt32, const UniformValue & );
        
        void setUniformB( UInt32, const SInt32 *, size_t );
        void setUniformI( UInt32, const SInt32 *, size_t );
        void setUniformF( UInt32, const float *, size_t );
        void setUniformV( UInt32, const Vector4f *, size_t );
        void setUniformM( UInt32, const Matrix44f *, size_t ); ///< \note Matrix should be row major

        void setUniformB( UInt32, bool );
        void setUniformI( UInt32, SInt32 );
        void setUniformF( UInt32, float );
        void setUniformV( UInt32, const Vector4f & );
        void setUniformM( UInt32, const Matrix44f & ); ///< \note Matrix should be row major
        //@}

        ///
        /// \name Set uniform variable by name
        ///
        //@{
        void setUniformByName( const StrA &, const UniformValue & );

        void setUniformByNameB( const StrA &, const SInt32 *, size_t );
        void setUniformByNameI( const StrA &, const SInt32 *, size_t );
        void setUniformByNameF( const StrA &, const float *, size_t );
        void setUniformByNameV( const StrA &, const Vector4f *, size_t );
        void setUniformByNameM( const StrA &, const Matrix44f *, size_t );

        void setUniformByNameB( const StrA &, bool );
        void setUniformByNameI( const StrA &, SInt32 );
        void setUniformByNameF( const StrA &, float );
        void setUniformByNameV( const StrA &, const Vector4f & );
        void setUniformByNameM( const StrA &, const Matrix44f & );
        //@}

        /// \name get reference to shader name.
        ///
        /// Name field is for debug purpose only, it is not used by garnet library. Set it to any value you want.
        //@{
        const StrA & name() const { return mName; }
        StrA & name() { return mName; }
        //@}

    protected :

        ///
        /// protected constructor
        ///
        /// \param type     Shader type
        /// \param lang     Shading language
        ///
        Shader( ShaderType type, ShadingLanguage lang )
            : mType(type)
            , mLang(lang)
        {
            GN_ASSERT( 0 <= type && type < NUM_SHADER_TYPES );
            GN_ASSERT( 0 <= lang && lang < NUM_SHADING_LANGUAGES );
        }

        ///
        /// Uniform structure
        ///
        struct Uniform
        {
            StrA         name;     ///< uniform name
            UniformValue value;    ///< uniform value
            HandleType   userData; ///< User-defined data
        };

        ///
        /// Get first unform handle. Use to iterate all uniforms.
        ///
        size_t getNumUniforms() const { return mUniforms.size(); }

        ///
        /// Get first unform handle. Use to iterate all uniforms.
        ///
        UInt32 getFirstUniform() const { return mUniforms.first(); }

        ///
        /// Get next unform handle. Use to iterate all uniforms.
        ///
        UInt32 getNextUniform( UInt32 handle ) const { return mUniforms.next(handle); }

        ///
        /// Get the uniform data structure
        ///
        Uniform & getUniform( UInt32 handle )
        {
            GN_ASSERT( mUniforms.validHandle(handle) );
            return mUniforms[handle];
        }

        ///
        /// Get the uniform data structure
        ///
        const Uniform & getUniform( UInt32 handle ) const
        {
            GN_ASSERT( mUniforms.validHandle(handle) );
            return mUniforms[handle];
        }

        ///
        /// get list of dirty uniforms
        ///
        const std::set<UInt32> & getDirtyUniforms() const { return mDirtySet; }

        ///
        /// clear dirty set
        ///
        void clearDirtySet() const { mDirtySet.clear(); }

    private:

        ///
        /// Query device-dependent uniform data
        ///
        /// \param name
        ///     uniform name
        /// \param userData
        ///     Return user-defined uniform data that will be stored in Uniform::userData.
        ///     Ignored if set to NULL.
        ///
        virtual bool queryDeviceUniform( const char * name, HandleType & userData ) const = 0;

        ///
        /// Add a new uniform to uniform list. Return handle of the uniform.
        ///
        UInt32 addUniform( const StrA & name )
        {
            GN_GUARD;

            if( name.empty() )
            {
                static Logger * sLogger = getLogger("GN.gfx.Shader");
                GN_ERROR(sLogger)( "uniform name can't be empty!" );
                return 0;
            }

            if( mUniformNames.end() != mUniformNames.find(name) )
            {
                static Logger * sLogger = getLogger("GN.gfx.Shader");
                GN_ERROR(sLogger)( "uniform named '%s' already exists.", name.cptr() );
                return 0;
            }

            Uniform u;
            u.name = name;
#if GN_DEBUG_BUILD
            u.value.type = (UniformValueType)0xbadbeef;
            u.userData = (HandleType)0xbadbeef;
#endif
            UInt32 h = mUniforms.add( u );
            mUniformNames[name] = h;

            // success
            return h;

            GN_UNGUARD;
        }

        bool checkUniformHandle( UInt32 handle )
        {
            if( !mUniforms.validHandle( handle ) )
            {
                static Logger * sLogger = getLogger("GN.gfx.Shader");
                GN_ERROR(sLogger)( "invalid uniform handle '%d'", handle );
                return false;
            }
            return true;
        }

        ShaderType      mType; ///< shader type
        ShadingLanguage mLang; ///< shading language

        HandleManager<Uniform,UInt32> mUniforms;     ///< uniform handle manager
        std::map<StrA,UInt32>         mUniformNames; ///< uniform name -> uniform handle
        mutable std::set<UInt32>      mDirtySet;     ///< Store handle of dirty uniforms.

        StrA mName; ///< shader name ( debug purpose only )
    };

    // *************************************************************************
    // inline methods
    // *************************************************************************

    // -------------------------------------------------------------------------
    inline UInt32 Shader::getUniformHandle( const StrA & name )
    {
        GN_GUARD_SLOW;

        // check for existing uniform
        if( mUniformNames.end() != mUniformNames.find(name) )
        {
            GN_ASSERT( mUniforms.validHandle( mUniformNames[name] ) );
            return mUniformNames[name];
        }

        // query for device-dependent uniform
        HandleType userData;
        if( !queryDeviceUniform( name.cptr(), userData ) )
        {
            static Logger * sLogger = getLogger("GN.gfx.Shader");
            GN_ERROR(sLogger)( "invalid uniform name: %s.", name.cptr() );
            return 0;
        }

        // add new uniform
        UInt32 handle = addUniform( name );
        if( 0 == handle ) return 0;
        Uniform & u = getUniform(handle);
        GN_ASSERT( u.name == name );
        u.userData = userData;

        // success
        return handle;

        GN_UNGUARD_SLOW;
    }

    // -------------------------------------------------------------------------
    inline void Shader::setUniform( UInt32 handle, const UniformValue & value )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value = value;
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformB( UInt32 handle, const SInt32 * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setB( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformI( UInt32 handle, const SInt32 * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setI( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformF( UInt32 handle, const float * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setF( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformV( UInt32 handle, const Vector4f * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setV( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void Shader::setUniformM( UInt32 handle, const Matrix44f * values, size_t count )
    {
        GN_GUARD_SLOW;
        if( !checkUniformHandle( handle ) ) return;
        mUniforms[handle].value.setM( values, count );
        mDirtySet.insert( handle );
        GN_UNGUARD_SLOW;
    }

    // -------------------------------------------------------------------------
    inline void Shader::setUniformB( UInt32 handle, bool value )
    {
        SInt32 i = value;
        setUniformB( handle, &i, 1 );
    }
    // --
    inline void Shader::setUniformI( UInt32 handle, SInt32 value )
    {
        setUniformI( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniformF( UInt32 handle, float value )
    {
        setUniformF( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniformV( UInt32 handle, const Vector4f & value )
    {
        setUniformV( handle, &value, 1 );
    }
    // --
    inline void Shader::setUniformM( UInt32 handle, const Matrix44f & value )
    {
        setUniformM( handle, &value, 1 );
    }

    // -------------------------------------------------------------------------
    inline void  Shader::setUniformByNameB( const StrA & name, bool value )
    {
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformB( handle, value );
    }
    // --
    inline void  Shader::setUniformByNameI( const StrA & name, SInt32 value )
    {
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformI( handle, value );
    }
    // --
    inline void  Shader::setUniformByNameF( const StrA & name, float value )
    {
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformF( handle, value );
    }
    // --
    inline void  Shader::setUniformByNameV( const StrA & name, const Vector4f & value )
    {
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformV( handle, value );
    }
    // --
    inline void  Shader::setUniformByNameM( const StrA & name, const Matrix44f & value )
    {
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformM( handle, value );
    }

    // -------------------------------------------------------------------------
    inline void Shader::setUniformByName( const StrA & name, const UniformValue & value )
    {
        GN_GUARD_SLOW;
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniform( handle, value );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameB( const StrA & name, const SInt32 * values, size_t count )
    {
        GN_GUARD_SLOW;
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformB( handle, values, count );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameI( const StrA & name, const SInt32 * values, size_t count )
    {
        GN_GUARD_SLOW;
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformI( handle, values, count );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameF( const StrA & name, const float * values, size_t count )
    {
        GN_GUARD_SLOW;
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformF( handle, values, count );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameV( const StrA & name, const Vector4f * values, size_t count )
    {
        GN_GUARD_SLOW;
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformV( handle, values, count );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void  Shader::setUniformByNameM( const StrA & name, const Matrix44f * values, size_t count )
    {
        GN_GUARD_SLOW;
        UInt32 handle = getUniformHandle(name);
        if( 0 == handle ) return;
        setUniformM( handle, values, count );
        GN_UNGUARD_SLOW;
    }

    // *************************************************************************
    // Util functions
    // *************************************************************************

    ///
    /// \name convert between shader usage tags to string
    //@{

    // -------------------------------------------------------------------------
    inline const char *
    shaderType2Str( ShaderType type )
    {
        static const char * sTable [] = { "VS", "PS", "GS" };
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
        static const char * sTable [] = { "VS", "PS", "GS" };
        if( str )
        {
            for( int i = 0; i < 3; ++i )
            {
                if( 0 == strCmp( str, sTable[i] ) )
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
                if( 0 == strCmp(str,sTable[i]) )
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

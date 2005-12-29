#ifndef __GN_GFXOGL_OGLSHADER_H__
#define __GN_GFXOGL_OGLSHADER_H__
// *****************************************************************************
//! \file    ogl/oglShader.h
//! \brief   OGL shader classes
//! \author  chenlee (2005.12.6)
// *****************************************************************************

#include "oglTypes.h"

namespace GN { namespace gfx
{
    //!
    //! OGL basic shader class
    //!
    struct OGLBasicShader
    {
        //!
        //! Enable shader profile
        //!
        virtual void enable() const = 0;

        //!
        //! Disable shader profile
        //!
        virtual void disable() const = 0;

        //!
        //! Apply shader as well as shader constants to OpenGL
        //!
        virtual void apply() const = 0;

        //!
        //! Apply only dirty uniforms to OpenGL
        //!
        virtual void applyDirtyUniforms() const = 0;
    };

    // *************************************************************************
    // ARB shader
    // *************************************************************************

    //!
    //! OGL Basic ARB shader
    //!
    class OGLBasicShaderARB : public Shader, public OGLBasicShader, public OGLResource, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLBasicShaderARB, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLBasicShaderARB( OGLRenderer & r, ShaderType type )
            : Shader( type, LANG_OGL_ARB )
            , OGLResource( r )
            , mTarget( sSelectTarget( type ) )
        { clear(); }
        virtual ~OGLBasicShaderARB() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mProgram = 0;
            mCode.clear();
            mMaxLocalUniforms = 0;
            mMaxEnvUniforms = 0;
        }
        //@}

        // ********************************
        // from OGLResource
        // ********************************
    public:

        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from OGLBasicShader
        // ********************************
    public:

        virtual void enable() const;
        virtual void disable() const;
        virtual void apply() const;
        virtual void applyDirtyUniforms() const;

        // ********************************
        // from Shader
        // ********************************
    private:

        virtual bool queryDeviceUniform( const char * name, HandleType & userData ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        enum ARBParameterType { LOCAL_PARAMETER, ENV_PARAMETER, MATRIX_PARAMETER };

        union UniformDesc
        {
            uint32_t u32;
            struct
            {
                int type  : 2;  //one of ARBParameterType
                int index : 30; // ARB uniform index
            };
        };

        const GLenum mTarget; // ARB program target
        GLuint mProgram;      // ARB program handle
        StrA  mCode;          // shader code

        GLuint mMaxLocalUniforms;
        GLuint mMaxEnvUniforms;

        // ********************************
        // private functions
        // ********************************
    private:

        inline void applyUniform( const Uniform & ) const; // apply single uniform

        static GLenum sSelectTarget( ShaderType type )
        {
            switch( type )
            {
                case VERTEX_SHADER : return GL_VERTEX_PROGRAM_ARB;
                case PIXEL_SHADER : return GL_FRAGMENT_PROGRAM_ARB;
                default:
                    GN_UNEXPECTED();
                    return 0;
            }
        }
    };

    //!
    //! OGL ARB vertex shader.
    //!
    class OGLVtxShaderARB : public OGLBasicShaderARB
    {
    public:
        //!
        //! ctor
        //!
        OGLVtxShaderARB( OGLRenderer & r ) : OGLBasicShaderARB( r, VERTEX_SHADER ) {}
    };

    //!
    //! OGL ARB pixel shader.
    //!
    class OGLPxlShaderARB : public OGLBasicShaderARB
    {
    public:
        //!
        //! ctor
        //!
        OGLPxlShaderARB( OGLRenderer & r ) : OGLBasicShaderARB( r, PIXEL_SHADER ) {}
    };

    // *************************************************************************
    // GLSL shader
    // *************************************************************************

    //!
    //! Basic OGL GLSL shader class
    //!
    class OGLBasicShaderGLSL : public Shader, public OGLBasicShader, public OGLResource, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLBasicShaderGLSL, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLBasicShaderGLSL( OGLRenderer & r, ShaderType t )
            : Shader( t, LANG_OGL_GLSL )
            , OGLResource( r )
            , mUsage( sSelectUsage(t) ) { clear(); }
        virtual ~OGLBasicShaderGLSL() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mHandle = 0;
            mCode.clear();
        }
        //@}

        // ********************************
        // from OGLResource
        // ********************************
    public:

        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from OGLBasicShader
        // ********************************
    public:

        virtual void enable() const { GN_WARN( "this function should not be called!" ); }
        virtual void disable() const;
        virtual void apply() const { GN_WARN( "this function should not be called!" ); }
        virtual void applyDirtyUniforms() const { GN_WARN( "this function should not be called!" ); }

        // ********************************
        // from Shader
        // ********************************
    private:

        virtual bool queryDeviceUniform( const char * name, HandleType & userData ) const;

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Get OGL shader handle
        //!
        GLhandleARB getHandleARB() const { return mHandle; }

        //!
        //! Apply dirty uniforms to render context
        //!
        void applyDirtyUniforms( GLhandleARB program ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        const GLenum mUsage;
        GLhandleARB  mHandle;
        StrA         mCode;

        // ********************************
        // private functions
        // ********************************
    private:

        static GLenum sSelectUsage( ShaderType type )
        {
            switch( type )
            {
                case VERTEX_SHADER : return GL_VERTEX_SHADER_ARB;
                case PIXEL_SHADER : return GL_FRAGMENT_SHADER_ARB;
                default:
                    GN_UNEXPECTED();
                    return 0;
            }
        }

    };

    //!
    //! OGL GLSL vertex shader.
    //!
    class OGLVtxShaderGLSL : public OGLBasicShaderGLSL
    {
    public:
        //!
        //! ctor
        //!
        OGLVtxShaderGLSL( OGLRenderer & r ) : OGLBasicShaderGLSL( r, VERTEX_SHADER ) {}
    };

    //!
    //! OGL GLSL pixel shader.
    //!
    class OGLPxlShaderGLSL : public OGLBasicShaderGLSL
    {
    public:
        //!
        //! ctor
        //!
        OGLPxlShaderGLSL( OGLRenderer & r ) : OGLBasicShaderGLSL( r, PIXEL_SHADER ) {}
    };

    //!
    //! GLSL program class
    //!
    class OGLProgramGLSL : public StdClass
    {
         GN_DECLARE_STDCLASS( OGLProgramGLSL, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLProgramGLSL( GLEWContext * c ) : mContext(c) { GN_ASSERT(c); clear(); }
        virtual ~OGLProgramGLSL() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const OGLBasicShaderGLSL * vs, const OGLBasicShaderGLSL * ps );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mProgram = 0; mShaders.clear(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! apply GLSL program, as well as dirty uniforms, to rendering context.
        //!
        void apply() const
        {
            GN_GUARD_SLOW;

            GN_OGL_CHECK( glUseProgramObjectARB( mProgram ) );

            for( size_t i = 0; i < mShaders.size(); ++i )
            {
                GN_ASSERT( mShaders[i] );
                mShaders[i]->applyDirtyUniforms( mProgram );
            }

            GN_UNGUARD_SLOW;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        GLEWContext * mContext;
        GLhandleARB mProgram;
        std::vector<const OGLBasicShaderGLSL*> mShaders;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createProgram();
        StrA getProgramInfoLog( GLhandleARB program );

        // for GLEW multi-context support
        GLEWContext * glewGetContext() const { return mContext; }
    };
}}

// *****************************************************************************
//                           End of oglShader.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLSHADER_H__

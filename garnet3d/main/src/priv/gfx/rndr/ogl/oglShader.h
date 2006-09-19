#ifndef __GN_GFXOGL_OGLSHADER_H__
#define __GN_GFXOGL_OGLSHADER_H__
// *****************************************************************************
//! \file    ogl/oglShader.h
//! \brief   OGL shader classes
//! \author  chenlee (2005.12.6)
// *****************************************************************************

#include "oglResource.h"

namespace GN { namespace gfx
{
    //!
    //! OGL basic shader class
    //!
    struct OGLBasicShader : public Shader
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

    protected:

        //!
        //! protected ctor
        //!
        OGLBasicShader( ShaderType type, ShadingLanguage lang ) : Shader(type,lang) {}
    };

    // *************************************************************************
    // ARB shader
    // *************************************************************************

    //!
    //! OGL Basic ARB shader
    //!
    class OGLBasicShaderARB : public OGLBasicShader, public OGLResource, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLBasicShaderARB, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLBasicShaderARB( OGLRenderer & r, ShaderType type )
            : OGLBasicShader( type, LANG_OGL_ARB )
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
            mMaxEnvUniforms = 0;
            mMaxLocalUniforms = 0;
            mMaxMatrixUniforms = 0;
        }
        //@}

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
                unsigned int type  : 2;  //one of ARBParameterType
                unsigned int index : 30; // ARB uniform index
            };
        };
        GN_CASSERT( 4 == sizeof(UniformDesc) );

        const GLenum mTarget; // ARB program target
        GLuint mProgram;      // ARB program handle

        GLuint mMaxEnvUniforms;
        GLuint mMaxLocalUniforms;
        GLuint mMaxMatrixUniforms;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createShader( const StrA & );

        inline void applyUniform( const Uniform & ) const; // apply single uniform

        static GLenum sSelectTarget( ShaderType type )
        {
            switch( type )
            {
                case SHADER_VS : return GL_VERTEX_PROGRAM_ARB;
                case SHADER_PS : return GL_FRAGMENT_PROGRAM_ARB;
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
        OGLVtxShaderARB( OGLRenderer & r ) : OGLBasicShaderARB( r, SHADER_VS ) {}
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
        OGLPxlShaderARB( OGLRenderer & r ) : OGLBasicShaderARB( r, SHADER_PS ) {}
    };

    // *************************************************************************
    // GLSL shader
    // *************************************************************************

    //!
    //! Basic OGL GLSL shader class
    //!
    class OGLBasicShaderGLSL : public OGLBasicShader, public OGLResource, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLBasicShaderGLSL, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLBasicShaderGLSL( OGLRenderer & r, ShaderType t )
            : OGLBasicShader( t, LANG_OGL_GLSL )
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
        }
        //@}

        // ********************************
        // from OGLBasicShader
        // ********************************
    public:

        virtual void enable() const { GN_WARN(getLogger("GN.gfx.rndr.OGL"))( "this function should not be called!" ); }
        virtual void disable() const;
        virtual void apply() const { GN_WARN(getLogger("GN.gfx.rndr.OGL"))( "this function should not be called!" ); }
        virtual void applyDirtyUniforms() const { GN_WARN(getLogger("GN.gfx.rndr.OGL"))( "this function should not be called!" ); }

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

        // ********************************
        // private functions
        // ********************************
    private:

        bool createShader( const StrA & );

        static GLenum sSelectUsage( ShaderType type )
        {
            switch( type )
            {
                case SHADER_VS : return GL_VERTEX_SHADER_ARB;
                case SHADER_PS : return GL_FRAGMENT_SHADER_ARB;
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
        OGLVtxShaderGLSL( OGLRenderer & r ) : OGLBasicShaderGLSL( r, SHADER_VS ) {}
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
        OGLPxlShaderGLSL( OGLRenderer & r ) : OGLBasicShaderGLSL( r, SHADER_PS ) {}
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
        OGLProgramGLSL() { clear(); }
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

        GLhandleARB mProgram;
        std::vector<const OGLBasicShaderGLSL*> mShaders;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createProgram();
        StrA getProgramInfoLog( GLhandleARB program );
    };
}}

// *****************************************************************************
//                           End of oglShader.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLSHADER_H__

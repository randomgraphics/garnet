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

    //!
    //! OGL Basic ARB shader
    //!
    class OGLBasicShaderARB : public Shader, OGLBasicShader, public OGLResource, public StdClass
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
}}

// *****************************************************************************
//                           End of oglShader.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLSHADER_H__

#ifndef __GN_GFXOGL_OGLSHADER_H__
#define __GN_GFXOGL_OGLSHADER_H__
// *****************************************************************************
/// \file
/// \brief   OGL shader classes
/// \author  chenlee (2005.12.6)
// *****************************************************************************

#include "oglResource.h"
#include "../common/cgShader.h"

namespace GN { namespace gfx
{
    // *************************************************************************
    // Basic program object
    // *************************************************************************

    ///
    /// Basic OGL GPU program class
    ///
    struct OGLBasicGpuProgram : public GpuProgram, public OGLResource
    {
        ///
        /// Apply shader as well as program constants to OpenGL
        ///
        virtual void apply() const = 0;

        ///
        /// Disable the program
        ///
        virtual void disable() const = 0;

        ///
        /// Apply only dirty parameters to OpenGL
        ///
        virtual void applyDirtyParameters() const = 0;

    protected:

        ///
        /// protected ctor
        ///
        OGLBasicGpuProgram( OGLRenderer & r ) : OGLResource(r) {}
    };

    // *************************************************************************
    // GLSL program
    // *************************************************************************

    /*
    /// Basic OGL GLSL shader class
    ///
    class OGLBasicShaderGLSL : public OGLShader, public OGLResource, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLBasicShaderGLSL, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLBasicShaderGLSL( OGLRenderer & r, GLenum usage )
            : OGLShader( t, LANG_OGL_GLSL )
            , OGLResource( r )
            , mUsage( sSelectUsage(t) ) { clear(); }
        virtual ~OGLBasicShaderGLSL() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code, const StrA & hints );
        void quit();
    private:
        void clear()
        {
            mHandle = 0;
        }
        //@}

        // ********************************
        // from OGLShader
        // ********************************
    public:

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

        ///
        /// Get OGL shader handle
        ///
        GLhandleARB getHandleARB() const { return mHandle; }

        ///
        /// Apply dirty uniforms to render context
        ///
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
    };

    ///
    /// OGL GLSL vertex shader.
    ///
    class OGLVtxShaderGLSL : public OGLBasicShaderGLSL
    {
    public:
        ///
        /// ctor
        ///
        OGLVtxShaderGLSL( OGLRenderer & r ) : OGLBasicShaderGLSL( r, GL_VERTEX_SHADER_ARB ) {}
    };

    ///
    /// OGL GLSL pixel shader.
    ///
    class OGLPxlShaderGLSL : public OGLBasicShaderGLSL
    {
    public:
        ///
        /// ctor
        ///
        OGLPxlShaderGLSL( OGLRenderer & r ) : OGLBasicShaderGLSL( r, GL_FRAGMENT_SHADER_ARB ) {}
    };*/

    ///
    /// GLSL program class
    ///
    class OGLGpuProgramGLSL : public OGLBasicGpuProgram, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLGpuProgramGLSL, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLGpuProgramGLSL( OGLRenderer & r ) : OGLBasicGpuProgram( r ) { clear(); }
        virtual ~OGLGpuProgramGLSL() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GpuProgramDesc & desc );
        void quit();
    private:
        void clear() { mProgram = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// apply GLSL program, as well as dirty uniforms, to rendering context.
        ///
        virtual void apply() const
        {
            GN_GUARD_SLOW;

            GN_OGL_CHECK( glUseProgramObjectARB( mProgram ) );

            GN_UNGUARD_SLOW;
        }

        ///
        /// Disable the program
        ///
        virtual void disable() const { GN_UNIMPL(); }

        ///
        /// Apply only dirty parameters to OpenGL
        ///
        virtual void applyDirtyParameters() const { GN_UNIMPL(); }

        // ********************************
        // private variables
        // ********************************
    private:

        GLhandleARB mProgram;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createProgram();
    };

    // *************************************************************************
    // Cg shader
    // *************************************************************************

#if 0 // def HAS_CG_OGL

    ///
    /// Basic Cg Shader class
    ///
    class OGLBasicShaderCg : public OGLShader, public OGLResource, public StdClass
    {
        GN_DECLARE_STDCLASS( OGLBasicShaderCg, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLBasicShaderCg( OGLRenderer & r, ShaderType t, CGGLenum profileClass )
            : OGLShader( t, LANG_CG )
            , OGLResource( r )
            , mProfileClass( profileClass ) { clear(); }
        virtual ~OGLBasicShaderCg() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code, const StrA & hints );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // from OGLShader
        // ********************************
    public:

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

        const CGGLenum mProfileClass;
        CGprofile      mProfile;
        CgShader       mShader;

        // ********************************
        // private functions
        // ********************************
    private:
        inline void applyUniform( const Uniform & ) const;
    };

    ///
    /// OGL Cg vertex shader.
    ///
    class OGLVtxShaderCg : public OGLBasicShaderCg
    {
    public:
        ///
        /// ctor
        ///
        OGLVtxShaderCg( OGLRenderer & r ) : OGLBasicShaderCg( r, SHADER_VS, CG_GL_VERTEX ) {}
    };

    ///
    /// OGL Cg pixel shader.
    ///
    class OGLPxlShaderCg : public OGLBasicShaderCg
    {
    public:
        ///
        /// ctor
        ///
        OGLPxlShaderCg( OGLRenderer & r ) : OGLBasicShaderCg( r, SHADER_PS, CG_GL_FRAGMENT ) {}
    };

#endif

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLSHADER_H__

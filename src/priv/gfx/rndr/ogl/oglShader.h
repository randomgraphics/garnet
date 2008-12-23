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

    ///
    /// GLSL program uniform description
    ///
    struct GLSLUniformDesc
    {
        GLenum            type;      ///< uniform type
        GLsizei           count;     ///< uniform count
        GLint             location;  ///< uniform location
        StrA              name;      ///< uniform name
        DynaArray<UInt8>  value;     ///< uniform value
        GLSLUniformDesc * nextDirty; ///< pointer to next dirty uniform.
    };

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
        void clear() { mProgram = 0; mVS = 0; mPS = 0; mDirtyList = NULL; }
        //@}

        // ********************************
        // from GpuProgram
        // ********************************

    public:

        virtual size_t getNumParameters() const { return mParams.size(); }
        virtual const GpuProgramParameterDesc * getParameters() const { return mParams.cptr(); }
        virtual void setParameter( size_t index, const void * value, size_t length );

        // ********************************
        // from OGLBasicGpuProgram
        // ********************************
    public:

        ///
        /// apply GLSL program, as well as dirty uniforms, to rendering context.
        ///
        virtual void apply() const
        {
            GN_OGL_CHECK( glUseProgramObjectARB( mProgram ) );
            applyDirtyParameters();
        }

        ///
        /// Disable the program
        ///
        virtual void disable() const
        {
            GN_OGL_CHECK( glUseProgramObjectARB( 0 ) );
        }

        ///
        /// Apply only dirty parameters to OpenGL
        ///
        virtual void applyDirtyParameters() const;

        // ********************************
        // private variables
        // ********************************
    private:

        GLhandleARB                        mProgram;
        GLhandleARB                        mVS;
        GLhandleARB                        mPS;
        DynaArray<GLSLUniformDesc>         mUniforms;
        DynaArray<GpuProgramParameterDesc> mParams;
        mutable GLSLUniformDesc          * mDirtyList;

        // ********************************
        // private functions
        // ********************************
    private:
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

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
    // OGL GPU program parameter object
    // *************************************************************************

    ///
    /// interface of GPU proram parameter
    ///
    class OGLUniform : public Uniform, public OGLResource
    {
        const size_t mSize;
        void       * mData;
        SInt32       mTimeStamp;

    public:

        /// ctor
        OGLUniform( OGLRenderer & r, size_t sz )
            : OGLResource(r)
            , mSize(0==sz?1:sz)
            , mData( heapAlloc(mSize) )
            , mTimeStamp(0)
        {
        }

        /// dtor
        ~OGLUniform() { heapFree(mData); }

        /// get parameter size
        virtual size_t size() const { return mSize; }

        /// get current parameter value
        virtual const void * getval() const { return mData; }

        /// update parameter value
        virtual void update( size_t offset, size_t length, const void * data )
        {
            if( offset >= mSize || (offset+length) > mSize )
            {
                GN_ERROR(getLogger("GN.gfx.Uniform"))( "Out of range!" );
                return;
            }
            if( NULL == data )
            {
                GN_ERROR(getLogger("GN.gfx.Uniform"))( "Null pointer!" );
                return;
            }
            memcpy( (UInt8*)mData + offset, data, length );
            ++mTimeStamp;
        }

        /// update parameter value
        template<typename T>
        void update( const T & t ) { set( 0, sizeof(t), &t ); }

        /// get current update time stamp
        SInt32 getTimeStamp() const { return mTimeStamp; }
    };

    // *************************************************************************
    // Basic program object
    // *************************************************************************

    ///
    /// Basic OGL GPU program class
    ///
    struct OGLBasicGpuProgram : public GpuProgram, public OGLResource
    {
        ///
        /// Enable shader
        ///
        virtual void enable() const = 0;

        ///
        /// Disable the program
        ///
        virtual void disable() const = 0;

        ///
        /// Apply uniforms to OpenGL
        ///
        virtual void applyUniforms( const OGLUniform * const * gpps, size_t count ) const = 0;

        ///
        /// Apply texture to OpenGL
        ///
        virtual void applyTexture( const char * name, size_t stage ) const = 0;

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
        void clear() { mProgram = 0; mVS = 0; mPS = 0; }
        //@}

        // ********************************
        // from GpuProgram
        // ********************************

    public:

        virtual const GpuProgramParameterDesc & getParameterDesc() const { return mParamDesc; }

        // ********************************
        // from OGLBasicGpuProgram
        // ********************************
    public:

        virtual void enable() const
        {
            GN_OGL_CHECK( glUseProgramObjectARB( mProgram ) );
        }

        virtual void disable() const
        {
            GN_OGL_CHECK( glUseProgramObjectARB( 0 ) );
        }

        virtual void applyUniforms( const OGLUniform * const * gpps, size_t count ) const;

        virtual void applyTexture( const char * name, size_t stage ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        ///
        /// GLSL program uniform description
        ///
        struct GLSLParameterDesc
        {
            GLenum                         type;         ///< uniform type
            GLsizei                        count;        ///< uniform count
            GLint                          location;     ///< uniform location
            StrA                           name;         ///< uniform name
            size_t                         size;         ///< uniform size
            mutable WeakRef<const Uniform> lastUniform;  ///< pointer to last uniform parameter
            mutable SInt32                 lastStamp;    ///< update time stamp of the last uniform parameter
            mutable AutoInit<size_t,-1>    lastTexStage; ///< last texture stage associated to this parameter
        };


        // GLSL program and shader object handles
        GLhandleARB mProgram;
        GLhandleARB mVS;
        GLhandleARB mPS;

        // uniforms
        DynaArray<GLSLParameterDesc>       mUniforms;
        DynaArray<const char *>            mUniformNames;
        DynaArray<size_t>                  mUniformSizes;

        // textures
        DynaArray<GLSLParameterDesc>       mTextures;
        DynaArray<const char *>            mTextureNames;

        // parameter descriptor
        GpuProgramParameterDesc            mParamDesc;

        // ********************************
        // private functions
        // ********************************
    private:

        bool enumParameters();
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

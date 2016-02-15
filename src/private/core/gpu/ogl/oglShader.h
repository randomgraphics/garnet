#ifndef __GN_GFXOGL_OGLSHADER_H__
#define __GN_GFXOGL_OGLSHADER_H__
// *****************************************************************************
/// \file
/// \brief   OGL shader classes
/// \author  chenlee (2005.12.6)
// *****************************************************************************

#include "oglResource.h"
#include "../common/basicShader.h"
#include "../common/cgShader.h"

namespace GN { namespace gfx
{
    class OglGpuProgramParameterDesc : public GpuProgramParameterDesc
    {
    public:

        void setUniformArray(
            const GpuProgramUniformParameterDesc * array,
            uint32                                 count,
            uint32                                 stride )
        {
            mUniformArray       = array;
            mUniformCount       = count;
            mUniformArrayStride = stride;
        }

        void setTextureArray(
            const GpuProgramTextureParameterDesc * array,
            uint32                                 count,
            uint32                                 stride )
        {
            mTextureArray       = array;
            mTextureCount       = count;
            mTextureArrayStride = stride;
        }

        void setAttributeArray(
            const GpuProgramAttributeParameterDesc * array,
            uint32                                   count,
            uint32                                   stride )
        {
            mAttributeArray       = array;
            mAttributeCount       = count;
            mAttributeArrayStride = stride;
        }
    };


    // *************************************************************************
    // Basic program object
    // *************************************************************************

    enum OGLVertexSemantic
    {
        VERTEX_SEMANTIC_VERTEX,
        VERTEX_SEMANTIC_NORMAL,
        VERTEX_SEMANTIC_COLOR,
        VERTEX_SEMANTIC_FOG,
        VERTEX_SEMANTIC_TEXCOORD,
        VERTEX_SEMANTIC_ATTRIBUTE,
    };

    struct OGLVertexBindingDesc
    {
        OGLVertexSemantic semantic;
        uint8             index;
    };

    ///
    /// Basic OGL GPU program class
    ///
    class OGLBasicGpuProgram : public GpuProgram, public OGLResource
    {
    public:

        ///
        /// return non-zero unique shader ID.
        ///
        uint64 uniqueID() const { return mID; }

        ///
        /// Get vertex buffer binding description of specific attribute.
        /// Return false, if the binding name and index is not used byt the program.
        ///
        virtual bool getBindingDesc( OGLVertexBindingDesc & result, uint32 attributeIndex ) const = 0;

        ///
        /// Enable the program
        ///
        virtual void enable() const = 0;

        ///
        /// Disable the program
        ///
        virtual void disable() const = 0;

        ///
        /// Apply uniforms to OpenGL
        ///
        virtual void applyUniforms( const Uniform * const * uniforms, uint32 count ) const = 0;

        ///
        /// Apply textures to OpenGL
        ///
        virtual void applyTextures( const TextureBinding * textures, uint32 count ) const = 0;

    protected:

        ///
        /// protected ctor
        ///
        OGLBasicGpuProgram( OGLGpu & r ) : OGLResource(r)
        {
            static uint64 counter = 1;
            mID = counter++;
        }

    private:

        uint64 mID;
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
        OGLGpuProgramGLSL( OGLGpu & r ) : OGLBasicGpuProgram( r ) { clear(); }
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

        virtual bool getBindingDesc( OGLVertexBindingDesc & result, uint32 attributeIndex ) const;

        virtual void enable() const
        {
            GN_OGL_CHECK( glUseProgramObjectARB( mProgram ) );
        }

        virtual void disable() const
        {
            GN_OGL_CHECK( glUseProgramObjectARB( 0 ) );
        }

        virtual void applyUniforms( const Uniform * const * uniforms, uint32 count ) const;

        virtual void applyTextures( const TextureBinding * textures, uint32 count ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        ///
        /// GLSL uniform parameter description
        ///
        struct GLSLUniformOrTextureDesc
        {
            GpuProgramUniformParameterDesc     uniformDesc;  ///< uniform parameter description
            GpuProgramTextureParameterDesc     textureDesc;  ///< textureparameter description
            GLenum                             type;         ///< uniform type
            GLsizei                            count;        ///< uniform count
            GLint                              location;     ///< uniform location
            uint32                             size;         ///< uniform size in bytes
            StrA                               name;         ///< uniform name
            mutable WeakRef<const Uniform>     lastUniform;  ///< pointer to last uniform parameter
            mutable sint32                     lastStamp;    ///< update time stamp of the last uniform parameter
            mutable AutoInitializer<uint32,-1> lastTexStage; ///< last texture stage associated to this parameter
        };

        ///
        /// GLSL vertex attribute description
        ///
        struct GLSLAttributeDesc
        {
            GpuProgramAttributeParameterDesc desc;          ///< attribute parameter description
            StrA                             name;          ///< attribute name
            OGLVertexSemantic                semanticName;
            uint8                            semanticIndex;
        };

        // GLSL program and shader object handles
        GLhandleARB mProgram;
        GLhandleARB mVS;
        GLhandleARB mPS;

        // uniforms
        DynaArray<GLSLUniformOrTextureDesc,uint32>  mUniforms;

        // textures
        DynaArray<GLSLUniformOrTextureDesc,uint32>  mTextures;

        // attributes
        DynaArray<GLSLAttributeDesc,uint32>         mAttributes;

        // parameter descriptor
        OglGpuProgramParameterDesc                  mParamDesc;

        // ********************************
        // private functions
        // ********************************
    private:

        bool enumParameters();
        bool enumAttributes();
    };

    // *************************************************************************
    // Cg shader
    // *************************************************************************

#ifdef HAS_CG_OGL

    ///
    /// Cg shader
    ///
    class OGLGpuProgramCG : public OGLBasicGpuProgram, public StdClass
    {
        GN_DECLARE_STDCLASS( OGLGpuProgramCG, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLGpuProgramCG( OGLGpu & r ) : OGLBasicGpuProgram( r ) { clear(); }
        virtual ~OGLGpuProgramCG() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GpuProgramDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // from GpuProgram
        // ********************************

    public:

        virtual const GpuProgramParameterDesc & getParameterDesc() const { return mParam; }

        // ********************************
        // from OGLBasicGpuProgram
        // ********************************
    public:

        //@{

        virtual bool getBindingDesc( OGLVertexBindingDesc & result, uint32 attributeIndex ) const;

        virtual void enable() const
        {
            GN_CG_CHECK( cgGLEnableProfile( mVs.getProfile() ) );
            GN_CG_CHECK( cgGLBindProgram( mVs.getProgram() ) );

            GN_CG_CHECK( cgGLEnableProfile( mPs.getProfile() ) );
            GN_CG_CHECK( cgGLBindProgram( mPs.getProgram() ) );
        }

        virtual void disable() const
        {
            GN_CG_CHECK( cgGLDisableProfile( mVs.getProfile() ) );
            GN_CG_CHECK( cgGLDisableProfile( mPs.getProfile() ) );
        }

        virtual void applyUniforms( const Uniform * const * uniforms, uint32 count ) const;

        virtual void applyTextures( const TextureBinding * textures, uint32 count ) const;

        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private types
        // ********************************
    private:

        typedef void (CGENTRY *SetCgTypelessParameterValue)(CGparameter param, int n, const void * vals);

        struct OglCgParameter
        {
            DynaArray<CGparameter> handles; // in case that the parameter is used in more than one programs.
            StrA                   name;
        };

        struct OglCgUniform : public OglCgParameter
        {
            GpuProgramUniformParameterDesc desc;

            // Total number of elements. For example, float2x3 blah[4][5] contains 2x3x4x5=120 elements.
            uint32 count;

            // The function pointer that sets the uniform value.
            SetCgTypelessParameterValue setValueFuncPtr;
        };

        struct OglCgTexture : public OglCgParameter
        {
            GpuProgramTextureParameterDesc desc;
        };

        struct OglCgAttribute : public OglCgParameter
        {
            GpuProgramAttributeParameterDesc desc;
            OGLVertexBindingDesc             binding;
        };

        class FindCgParameterByName
        {
            const char * mName;

        public:

            FindCgParameterByName( const char * name ) : mName(name)
            {
            }

            bool operator()( const OglCgParameter & param )
            {
                return param.name == mName;
            }
        };

        // ********************************
        // private variables
        // ********************************
    private:

        CgShader                          mVs;
        CgShader                          mPs;
        DynaArray<OglCgUniform,uint32>    mUniforms;
        DynaArray<OglCgTexture,uint32>    mTextures;
        DynaArray<OglCgAttribute,uint32>  mAttributes;
        OglGpuProgramParameterDesc        mParam;

        // ********************************
        // private functions
        // ********************************
    private:

        void enumCgParameters( CGprogram prog, CGenum name_space );
        SetCgTypelessParameterValue GetCgSetParameterFuncPtr( CGparameter param );
    };

#endif

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLSHADER_H__

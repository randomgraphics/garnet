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
            size_t                                 count,
            size_t                                 stride )
        {
            mUniformArray       = array;
            mUniformCount       = count;
            mUniformArrayStride = stride;
        }

        void setTextureArray(
            const GpuProgramTextureParameterDesc * array,
            size_t                                 count,
            size_t                                 stride )
        {
            mTextureArray       = array;
            mTextureCount       = count;
            mTextureArrayStride = stride;
        }

        void setAttributeArray(
            const GpuProgramAttributeParameterDesc * array,
            size_t                                   count,
            size_t                                   stride )
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
        /// return program language used by this shader
        ///
        GpuProgramLanguage language() const { return mLanguage; }

        ///
        /// Get vertex buffer binding description of specific attribute.
        /// Return false, if the binding name and index is not used byt the program.
        ///
        virtual bool getBindingDesc( OGLVertexBindingDesc & result, size_t attributeIndex ) const = 0;

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
        virtual void applyUniforms( const Uniform * const * uniforms, size_t count ) const = 0;

        ///
        /// Apply textures to OpenGL
        ///
        virtual void applyTextures( const TextureBinding * textures, size_t count ) const = 0;

    protected:

        ///
        /// protected ctor
        ///
        OGLBasicGpuProgram( OGLGpu & r, GpuProgramLanguage lang ) : OGLResource(r), mLanguage(lang)
        {
            static uint64 counter = 1;
            mID = counter++;
        }

    private:

        const GpuProgramLanguage mLanguage;
        uint64                   mID;
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
        OGLGpuProgramGLSL( OGLGpu & r ) : OGLBasicGpuProgram( r, GpuProgramLanguage::GLSL ) { clear(); }
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

        virtual bool getBindingDesc( OGLVertexBindingDesc & result, size_t attributeIndex ) const;

        virtual void enable() const
        {
            GN_OGL_CHECK( glUseProgramObjectARB( mProgram ) );
        }

        virtual void disable() const
        {
            GN_OGL_CHECK( glUseProgramObjectARB( 0 ) );
        }

        virtual void applyUniforms( const Uniform * const * uniforms, size_t count ) const;

        virtual void applyTextures( const TextureBinding * textures, size_t count ) const;

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
            size_t                             size;         ///< uniform size in bytes
            StrA                               name;         ///< uniform name
            mutable WeakRef<const Uniform>     lastUniform;  ///< pointer to last uniform parameter
            mutable sint32                     lastStamp;    ///< update time stamp of the last uniform parameter
            mutable AutoInitializer<size_t,-1> lastTexStage; ///< last texture stage associated to this parameter
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
        DynaArray<GLSLUniformOrTextureDesc>  mUniforms;

        // textures
        DynaArray<GLSLUniformOrTextureDesc>  mTextures;

        // attributes
        DynaArray<GLSLAttributeDesc>         mAttributes;

        // parameter descriptor
        OglGpuProgramParameterDesc           mParamDesc;

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
        OGLGpuProgramCG( OGLGpu & r ) : OGLBasicGpuProgram( r, GpuProgramLanguage::CG ) { clear(); }
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

        virtual bool getBindingDesc( OGLVertexBindingDesc & result, size_t attributeIndex ) const;

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

        virtual void applyUniforms( const Uniform * const * uniforms, size_t count ) const;

        virtual void applyTextures( const TextureBinding * textures, size_t count ) const;

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
            size_t count;

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

        CgShader                   mVs;
        CgShader                   mPs;
        DynaArray<OglCgUniform>    mUniforms;
        DynaArray<OglCgTexture>    mTextures;
        DynaArray<OglCgAttribute>  mAttributes;
        OglGpuProgramParameterDesc mParam;

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

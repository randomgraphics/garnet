#ifndef __GN_GFXOGL_OGLSHADER_H__
#define __GN_GFXOGL_OGLSHADER_H__
// *****************************************************************************
/// \file
/// \brief   OGL shader classes
/// \author  chenlee (2005.12.6)
// *****************************************************************************

#include "oglResource.h"
#include "../common/basicShader.h"

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
            mutable AutoInitializer<uint32,(uint32)-1> lastTexStage; ///< last texture stage associated to this parameter
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
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLSHADER_H__

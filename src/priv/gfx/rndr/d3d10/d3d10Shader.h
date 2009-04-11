#ifndef __GN_GFXD3D10_D3D10SHADER_H__
#define __GN_GFXD3D10_D3D10SHADER_H__
// *****************************************************************************
/// \file
/// \brief   D3D shader classes
/// \author  chenlee (2005.11.26)
// *****************************************************************************

#include "d3d10Resource.h"
#include "../common/basicShader.h"

namespace GN { namespace gfx
{
    // *************************************************************************
    // HLSL shaders
    // *************************************************************************

    ///
    /// D3D10 shader compile flags
    ///
    struct D3D10ShaderCompileOptions
    {
        UInt32 compileFlags; ///< combination of D3D10_SHADER flags.
    };

    /// shader parameter classes
    //@{

    struct D3D10UniformParameterDesc : public GpuProgramUniformParameterDesc
    {
        ///
        /// shader specific properties
        ///
        struct ShaderSpecificProperties
        {
            AutoInitializer<bool,false> used;   ///< are these properties used
            UInt32                      cbidx;  ///< const buffer index
            UInt32                      offset; ///< uniform offset in bytes in the const buffer.
        };

        ShaderSpecificProperties ssp[3]; ///< shader specific properites for each shader type

        /// ctor
        D3D10UniformParameterDesc()
        {
            name = NULL;
        }
    };

    struct D3D10TextureParameterDesc : public GpuProgramTextureParameterDesc
    {
        ///
        /// shader specific properties
        ///
        struct ShaderSpecificProperties
        {
            AutoInitializer<bool,false> used;  ///< are these properties used
        };

        ShaderSpecificProperties ssp[3]; ///< shader specific properites for each shader type

        /// ctor
        D3D10TextureParameterDesc()
        {
            name = NULL;
        }
    };

    struct D3D10AttributeParameterDesc : public GpuProgramAttributeParameterDesc
    {
        ///
        /// shader specific properties
        ///
        struct ShaderSpecificProperties
        {
            AutoInitializer<bool,false> used;  ///< are these properties used
        };

        ShaderSpecificProperties ssp[3]; ///< shader specific properites for each shader type

        /// ctor
        D3D10AttributeParameterDesc()
        {
            name = NULL;
        }
    };

    class D3D10GpuProgramParameterDesc : public GpuProgramParameterDesc, public NoCopy
    {
        DynaArray<D3D10UniformParameterDesc>   mUniforms;
        DynaArray<D3D10TextureParameterDesc>   mTextures;
        DynaArray<D3D10AttributeParameterDesc> mAttributes;

    public:

        /// ctor
        D3D10GpuProgramParameterDesc();

        /// dtor
        ~D3D10GpuProgramParameterDesc();

        /// build parameter arrays
        void buildParameterArrays();

        /// find parameter with specific name
        //@{
        D3D10UniformParameterDesc   * findUniform( const char * name );
        D3D10TextureParameterDesc   * findTexture( const char * name );
        D3D10AttributeParameterDesc * findAttribute( const char * name );
        //@}

        /// add new parameters
        //@{
        void addUniform( const D3D10UniformParameterDesc & u ) { mUniforms.append( u ); }
        void addTexture( const D3D10TextureParameterDesc & );
        void addAttribute( const D3D10AttributeParameterDesc & );
        //@}
    };

    //@}

    ///
    /// D3D10 vertex shader
    ///
    struct D3D10VertexShaderHLSL
    {
        AutoComPtr<ID3D10VertexShader>          shader;    ///< shader pointer
        StackArray<ID3D10Buffer*,16>            constBufs; ///< constant buffers
        mutable StackArray<DynaArray<UInt8>,16> constData; ///< constant data

        /// initialize shader
        bool init(
            ID3D10Device                    & dev,
            const ShaderCode                & code,
            const D3D10ShaderCompileOptions & options,
            D3D10GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void clear() { shader.clear(); constBufs.clear(); constData.clear(); }
    };

    ///
    /// D3D10 geometry shader
    ///
    struct D3D10GeometryShaderHLSL
    {
        AutoComPtr<ID3D10GeometryShader>        shader;    ///< shader pointer
        StackArray<ID3D10Buffer*,16>            constBufs; ///< constant buffers
        mutable StackArray<DynaArray<UInt8>,16> constData; ///< constant data

        /// initialize shader
        bool init(
            ID3D10Device                    & dev,
            const ShaderCode                & code,
            const D3D10ShaderCompileOptions & options,
            D3D10GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void clear() { shader.clear(); constBufs.clear(); constData.clear(); }
    };

    ///
    /// D3D10 pixel shader
    ///
    struct D3D10PixelShaderHLSL
    {
        AutoComPtr<ID3D10PixelShader>           shader;    ///< shader pointer
        StackArray<ID3D10Buffer*,16>            constBufs; ///< constant buffers
        mutable StackArray<DynaArray<UInt8>,16> constData; ///< constant data

        /// initialize shader
        bool init(
            ID3D10Device                    & dev,
            const ShaderCode                & code,
            const D3D10ShaderCompileOptions & options,
            D3D10GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void clear() { shader.clear(); constBufs.clear(); constData.clear(); }
    };

    ///
    /// D3D10 HLSL GPU program
    ///
    class D3D10GpuProgram : public GpuProgram, public D3D10Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D10GpuProgram, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10GpuProgram( D3D10Renderer & r )
            : D3D10Resource(r)
        {
            clear();
        }
        virtual ~D3D10GpuProgram() { quit(); }
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
        // from Renderer
        // ********************************
    public:

        virtual const GpuProgramParameterDesc & getParameterDesc() const { return mParamDesc; }

        // ********************************
        // public methods
        // ********************************
    public:

        ///
        /// apply shader to D3D device
        ///
        void apply() const
        {
            ID3D10Device & dev = getDeviceRef();
            dev.VSSetShader( mVs.shader );
            dev.GSSetShader( mGs.shader );
            dev.PSSetShader( mPs.shader );
        }

        ///
        /// Apply uniforms to D3D device
        ///
        void applyUniforms( const SysMemUniform * const * uniforms, size_t count ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        D3D10GpuProgramParameterDesc mParamDesc;

        D3D10VertexShaderHLSL   mVs;
        D3D10GeometryShaderHLSL mGs;
        D3D10PixelShaderHLSL    mPs;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10SHADER_H__

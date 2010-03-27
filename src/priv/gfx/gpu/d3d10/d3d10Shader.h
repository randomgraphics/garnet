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

        ///
        /// shader specific properites for each shader type
        ///
        /// 0: VS
        /// 1: GS
        /// 2: PS
        ///
        ShaderSpecificProperties ssp[3];

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
            UINT                        stage; ///< texture stage that the parameter is binding to
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
        const D3D10UniformParameterDesc   * FindUniform( const char * name ) const;
        D3D10UniformParameterDesc         * FindUniform( const char * name );
        const D3D10TextureParameterDesc   * FindTexture( const char * name ) const;
        D3D10TextureParameterDesc         * FindTexture( const char * name );
        const D3D10AttributeParameterDesc * findAttribute( const char * name ) const;
        D3D10AttributeParameterDesc       * findAttribute( const char * name );
        //@}

        /// add new parameters
        //@{
        void addUniform( const D3D10UniformParameterDesc & u ) { mUniforms.Append( u ); }
        void addTexture( const D3D10TextureParameterDesc & t ) { mTextures.Append( t ); }
        void addAttribute( const D3D10AttributeParameterDesc & );
        //@}
    };

    //@}

    ///
    /// array of D3D10 constant buffer
    ///
    typedef StackArray<AutoComPtr<ID3D10Buffer>,16> D3D10ConstBufferArray;

    // We'll cast this auto-ptr array to raw pointer array. So
    // they must be the same size.
    GN_CASSERT( sizeof(AutoComPtr<ID3D10Buffer>) == sizeof(ID3D10Buffer*) );

    ///
    /// array of constant buffer in system memory
    ///
    typedef StackArray<DynaArray<UInt8>,16> SysMemConstBufferArray;

    ///
    /// D3D10 vertex shader
    ///
    struct D3D10VertexShaderHLSL
    {
        AutoComPtr<ID3D10VertexShader> shader;    ///< shader pointer
        D3D10ConstBufferArray          constBufs; ///< constant buffers
        mutable SysMemConstBufferArray constData; ///< constant data

        /// initialize shader
        bool Init(
            ID3D10Device                    & dev,
            const ShaderCode                & code,
            const D3D10ShaderCompileOptions & options,
            D3D10GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void Clear() { shader.Clear(); constBufs.Clear(); constData.Clear(); }
    };

    ///
    /// D3D10 geometry shader
    ///
    struct D3D10GeometryShaderHLSL
    {
        AutoComPtr<ID3D10GeometryShader> shader;    ///< shader pointer
        D3D10ConstBufferArray            constBufs; ///< constant buffers
        mutable SysMemConstBufferArray   constData; ///< constant data

        /// initialize shader
        bool Init(
            ID3D10Device                    & dev,
            const ShaderCode                & code,
            const D3D10ShaderCompileOptions & options,
            D3D10GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void Clear() { shader.Clear(); constBufs.Clear(); constData.Clear(); }
    };

    ///
    /// D3D10 pixel shader
    ///
    struct D3D10PixelShaderHLSL
    {
        AutoComPtr<ID3D10PixelShader>  shader;    ///< shader pointer
        D3D10ConstBufferArray          constBufs; ///< constant buffers
        mutable SysMemConstBufferArray constData; ///< constant data

        /// initialize shader
        bool Init(
            ID3D10Device                    & dev,
            const ShaderCode                & code,
            const D3D10ShaderCompileOptions & options,
            D3D10GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void Clear() { shader.Clear(); constBufs.Clear(); constData.Clear(); }
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
        D3D10GpuProgram( D3D10Gpu & r )
            : D3D10Resource(r)
        {
            Clear();
        }
        virtual ~D3D10GpuProgram() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const GpuProgramDesc & desc );
        void Quit();
    private:
        void Clear() {}
        //@}

        // ********************************
        // from Gpu
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

            // bind shader
            dev.VSSetShader( mVs.shader );
            dev.GSSetShader( mGs.shader );
            dev.PSSetShader( mPs.shader );

            // bind constant buffers
            if( mVs.constBufs.Size() )
            {
                dev.VSSetConstantBuffers(
                    0,
                    (UInt32)mVs.constBufs.Size(),
                    &mVs.constBufs[0] );
            }

            if( mGs.constBufs.Size() )
            {
                dev.GSSetConstantBuffers(
                    0,
                    (UInt32)mGs.constBufs.Size(),
                    &mGs.constBufs[0] );
            }

            if( mPs.constBufs.Size() )
            {
                dev.PSSetConstantBuffers(
                    0,
                    (UInt32)mPs.constBufs.Size(),
                    &mPs.constBufs[0] );
            }
        }

        ///
        /// Apply uniforms to D3D device
        ///
        void applyUniforms(
            const Uniform * const * uniforms,
            size_t                  count,
            bool                    skipDirtyCheck ) const;

        ///
        /// apply textures to D3D device
        ///
        void applyTextures(
            const TextureBinding * bindings,
            size_t                 count,
            bool                   skipDirtyCheck ) const;

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

#ifndef __GN_GFXD3D11_D3D11SHADER_H__
#define __GN_GFXD3D11_D3D11SHADER_H__
// *****************************************************************************
/// \file
/// \brief   D3D shader classes
/// \author  chenlee (2005.11.26)
// *****************************************************************************

#include "d3d11Resource.h"
#include "../common/basicShader.h"

namespace GN { namespace gfx
{
    // *************************************************************************
    // HLSL shaders
    // *************************************************************************

    ///
    /// D3D11 shader compile flags
    ///
    struct D3D11ShaderCompileOptions
    {
        UInt32 compileFlags; ///< combination of D3D11_SHADER flags.
    };

    /// shader parameter classes
    //@{

    struct D3D11UniformParameterDesc : public GpuProgramUniformParameterDesc
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
        D3D11UniformParameterDesc()
        {
            name = NULL;
        }
    };

    struct D3D11TextureParameterDesc : public GpuProgramTextureParameterDesc
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
        D3D11TextureParameterDesc()
        {
            name = NULL;
        }
    };

    struct D3D11AttributeParameterDesc : public GpuProgramAttributeParameterDesc
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
        D3D11AttributeParameterDesc()
        {
            name = NULL;
        }
    };

    class D3D11GpuProgramParameterDesc : public GpuProgramParameterDesc, public NoCopy
    {
        DynaArray<D3D11UniformParameterDesc>   mUniforms;
        DynaArray<D3D11TextureParameterDesc>   mTextures;
        DynaArray<D3D11AttributeParameterDesc> mAttributes;

    public:

        /// ctor
        D3D11GpuProgramParameterDesc();

        /// dtor
        ~D3D11GpuProgramParameterDesc();

        /// build parameter arrays
        void buildParameterArrays();

        /// find parameter with specific name
        //@{
        const D3D11UniformParameterDesc   * findUniform( const char * name ) const;
        D3D11UniformParameterDesc         * findUniform( const char * name );
        const D3D11TextureParameterDesc   * findTexture( const char * name ) const;
        D3D11TextureParameterDesc         * findTexture( const char * name );
        const D3D11AttributeParameterDesc * findAttribute( const char * name ) const;
        D3D11AttributeParameterDesc       * findAttribute( const char * name );
        //@}

        /// add new parameters
        //@{
        void addUniform( const D3D11UniformParameterDesc & u ) { mUniforms.append( u ); }
        void addTexture( const D3D11TextureParameterDesc & t ) { mTextures.append( t ); }
        void addAttribute( const D3D11AttributeParameterDesc & );
        //@}
    };

    //@}

    ///
    /// array of D3D11 constant buffer
    ///
    typedef StackArray<ID3D11Buffer*,16> D3D11ConstBufferArray;

    ///
    /// array of constant buffer in system memory
    ///
    typedef StackArray<DynaArray<UInt8>,16> SysMemConstBufferArray;

    ///
    /// D3D11 vertex shader
    ///
    struct D3D11VertexShaderHLSL
    {
        AutoComPtr<ID3D11VertexShader> shader;    ///< shader pointer
        D3D11ConstBufferArray          constBufs; ///< constant buffers
        mutable SysMemConstBufferArray constData; ///< constant data

        /// initialize shader
        bool init(
            ID3D11Device                    & dev,
            const ShaderCode                & code,
            const D3D11ShaderCompileOptions & options,
            D3D11GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void clear() { shader.clear(); constBufs.clear(); constData.clear(); }
    };

    ///
    /// D3D11 geometry shader
    ///
    struct D3D11GeometryShaderHLSL
    {
        AutoComPtr<ID3D11GeometryShader> shader;    ///< shader pointer
        D3D11ConstBufferArray            constBufs; ///< constant buffers
        mutable SysMemConstBufferArray   constData; ///< constant data

        /// initialize shader
        bool init(
            ID3D11Device                    & dev,
            const ShaderCode                & code,
            const D3D11ShaderCompileOptions & options,
            D3D11GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void clear() { shader.clear(); constBufs.clear(); constData.clear(); }
    };

    ///
    /// D3D11 pixel shader
    ///
    struct D3D11PixelShaderHLSL
    {
        AutoComPtr<ID3D11PixelShader>  shader;    ///< shader pointer
        D3D11ConstBufferArray          constBufs; ///< constant buffers
        mutable SysMemConstBufferArray constData; ///< constant data

        /// initialize shader
        bool init(
            ID3D11Device                    & dev,
            const ShaderCode                & code,
            const D3D11ShaderCompileOptions & options,
            D3D11GpuProgramParameterDesc    & paramDesc );

        /// clear shader
        void clear() { shader.clear(); constBufs.clear(); constData.clear(); }
    };

    ///
    /// D3D11 HLSL GPU program
    ///
    class D3D11GpuProgram : public GpuProgram, public D3D11Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D11GpuProgram, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D11GpuProgram( D3D11Renderer & r )
            : D3D11Resource(r)
        {
            clear();
        }
        virtual ~D3D11GpuProgram() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GpuProgramDesc & desc, bool hlsl9 );
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
            ID3D11DeviceContext & cxt = getDeviceContextRef();

            // bind shader
            cxt.VSSetShader( mVs.shader, NULL, 0 );
            cxt.GSSetShader( mGs.shader, NULL, 0 );
            cxt.PSSetShader( mPs.shader, NULL, 0 );

            // bind constant buffers
            if( mVs.constBufs.size() )
            {
                cxt.VSSetConstantBuffers(
                    0,
                    (UInt32)mVs.constBufs.size(),
                    mVs.constBufs.cptr() );
            }

            if( mGs.constBufs.size() )
            {
                cxt.GSSetConstantBuffers(
                    0,
                    (UInt32)mGs.constBufs.size(),
                    mGs.constBufs.cptr() );
            }

            if( mPs.constBufs.size() )
            {
                cxt.PSSetConstantBuffers(
                    0,
                    (UInt32)mPs.constBufs.size(),
                    mPs.constBufs.cptr() );
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

        D3D11GpuProgramParameterDesc mParamDesc;

        D3D11VertexShaderHLSL   mVs;
        D3D11GeometryShaderHLSL mGs;
        D3D11PixelShaderHLSL    mPs;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11SHADER_H__

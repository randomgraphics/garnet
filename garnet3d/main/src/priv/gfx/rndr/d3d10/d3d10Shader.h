#ifndef __GN_GFXD3D10_D3D10SHADER_H__
#define __GN_GFXD3D10_D3D10SHADER_H__
// *****************************************************************************
/// \file    d3d10/d3d10Shader.h
/// \brief   D3D shader classes
/// \author  chenlee (2005.11.26)
// *****************************************************************************

#include "d3d10Resource.h"
#include "../common/cgShader.h"

namespace GN { namespace gfx
{
    ///
    /// Basic D3D10 shader class
    ///
    struct D3D10BasicShader : public Shader
    {
        ///
        /// Apply shader as well as shader constants to D3D device
        ///
        virtual void apply() const = 0;

        ///
        /// Apply only dirty uniforms to D3D device
        ///
        virtual void applyDirtyUniforms() const = 0;

    protected:

        ///
        /// protected ctor
        ///
        D3D10BasicShader( ShaderType type, ShadingLanguage lang ) : Shader(type,lang) {}
    };

    // *************************************************************************
    // HLSL shaders
    // *************************************************************************

    ///
    /// shader creation hints structure
    ///
    struct D3D10ShaderCreationHints
    {
        StrA filename; ///< shader file name
        StrA entry;    ///< entry function
        bool optimize; ///< enable shader optimization, default is on.

        ///
        /// Setup hints from string.
        ///
        void fromStr( const StrA & s )
        {
            Registry r;
            r.importFromStr( s );
            filename = r.gets( "filename", "" );
            entry = r.gets( "entry", "main" );
            optimize = r.getb( "opt", true );
        }
    };

    ///
    /// Basic D3D10 HLSL shader class
    ///
    class D3D10ShaderHlsl : public D3D10Resource, public D3D10BasicShader, public StdClass
    {
        GN_DECLARE_STDCLASS( D3D10ShaderHlsl, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    protected:
        D3D10ShaderHlsl( D3D10Renderer & r, ShaderType type )
            : D3D10Resource( r )
            , D3D10BasicShader( type, LANG_D3D_HLSL )
        {
            switch( type )
            {
                case SHADER_VS :
                    mProfile = "vs_4_0";
                    mSetConstantBuffers = &ID3D10Device::VSSetConstantBuffers;
                    break;

                case SHADER_PS :
                    mProfile = "ps_4_0";
                    mSetConstantBuffers = &ID3D10Device::PSSetConstantBuffers;
                    break;

                case SHADER_GS :
                    mProfile = "gs_4_0";
                    mSetConstantBuffers = &ID3D10Device::GSSetConstantBuffers;
                    break;

                default :
                    GN_UNEXPECTED();
                    break;
            }

            clear();
        }
    public:
        virtual ~D3D10ShaderHlsl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code, const StrA & hints );
        void quit();
    private:
        void clear() { mBinary = 0; mReflection = 0; }
        //@}

        // ********************************
        // from D3D10BasicShader
        // ********************************
    public:

        void applyDirtyUniforms() const;

        // ********************************
        // from Shader
        // ********************************
    private:

        bool queryDeviceUniform( const char * name, HandleType & userData ) const;

        // ********************************
        // protected functions
        // ********************************
    protected:

        ///
        /// get shader binary
        ///
        ID3D10Blob * getBinary() const { return mBinary; }

        ///
        /// apply all uniforms to D3D device
        ///
        void applyAllUniforms() const
        {
            GN_GUARD_SLOW;

            // apply all uniform
            UInt32 handle = getFirstUniform();
            while( handle )
            {
                applyUniform( getUniform( handle ) );
                handle = getNextUniform( handle );
            }
            clearDirtySet();

            ID3D10Device * dev = getDevice();

            // bind constant buffers
            (dev->*mSetConstantBuffers)( 0, (UINT)mConstBufs.size(), mConstBufs.cptr() );

            GN_UNGUARD_SLOW;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        // shader type dependent function pointers
        typedef void (__stdcall ID3D10Device::*SetConstantBuffers)(
                UINT StartSlot,
                UINT NumBuffers,
                ID3D10Buffer *const * ppConstantBuffers );

        union UniformUserData
        {
            UInt32 u32;
            struct
            {
                unsigned int bufidx   :  4; ///< cbuffer index [0,15]
                unsigned int offsetdw : 14; ///< uniform offset in cbuffer, in DWORDs (4096*4=2^14)
                unsigned int sizedw   : 14; ///< uniform size in DWORDs
            };
        };
        GN_CASSERT( sizeof(UniformUserData) == 4 );

        const char *                 mProfile;
        ID3D10Blob *                 mBinary;      ///< shader binary
        ID3D10ShaderReflection *     mReflection;  ///< shader reflection
        StackArray<ID3D10Buffer*,16> mConstBufs;   ///< constant buffers

        SetConstantBuffers          mSetConstantBuffers;

        // ********************************
        // private functions
        // ********************************
    private:

        void applyUniform( const Uniform & ) const;
        bool compileShader( const StrA & code, const StrA & hints );
    };

    ///
    /// D3D10 HLSL vertex shader class
    ///
    class D3D10VtxShaderHlsl : public D3D10ShaderHlsl
    {
         GN_DECLARE_STDCLASS( D3D10VtxShaderHlsl, D3D10ShaderHlsl );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10VtxShaderHlsl( D3D10Renderer & r )
            : D3D10ShaderHlsl( r, SHADER_VS )
        { clear(); }
        virtual ~D3D10VtxShaderHlsl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code, const StrA & hints );
        void quit();
    private:
        void clear() { mD3DShader = 0; }
        //@}

        // ********************************
        // from D3D10BasicShader
        // ********************************
    public:

        void apply() const;

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10VertexShader * mD3DShader;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createShader();
    };

    ///
    /// D3D10 HLSL pixel shader class
    ///
    class D3D10PxlShaderHlsl : public D3D10ShaderHlsl
    {
         GN_DECLARE_STDCLASS( D3D10PxlShaderHlsl, D3D10ShaderHlsl );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10PxlShaderHlsl( D3D10Renderer & r )
            : D3D10ShaderHlsl( r, SHADER_PS )
        { clear(); }
        virtual ~D3D10PxlShaderHlsl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code, const StrA & hints );
        void quit();
    private:
        void clear() { mD3DShader = 0; }
        //@}

        // ********************************
        // from D3D10BasicShader
        // ********************************
    public:

        void apply() const;

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10PixelShader * mD3DShader;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createShader();
    };

    ///
    /// D3D10 HLSL geometry shader class
    ///
    class D3D10GeoShaderHlsl : public D3D10ShaderHlsl
    {
         GN_DECLARE_STDCLASS( D3D10GeoShaderHlsl, D3D10ShaderHlsl );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10GeoShaderHlsl( D3D10Renderer & r )
            : D3D10ShaderHlsl( r, SHADER_PS )
        { clear(); }
        virtual ~D3D10GeoShaderHlsl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code, const StrA & hints );
        void quit();
    private:
        void clear() { mD3DShader = 0; }
        //@}

        // ********************************
        // from D3D10BasicShader
        // ********************************
    public:

        void apply() const;

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10GeometryShader * mD3DShader;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createShader();
    };
}}

// *****************************************************************************
//                           End of d3d10Shader.h
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10SHADER_H__

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

    ///
    /// D3D10 HLSL shader class
    ///
    class D3D10ShaderHlsl : public D3D10Resource, public StdClass
    {
        GN_DECLARE_STDCLASS( D3D10ShaderHlsl, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10ShaderHlsl( D3D10Renderer & r, const char * profile );
        virtual ~D3D10ShaderHlsl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const ShaderCode & code, const D3D10ShaderCompileOptions & options );
        void quit();
    private:
        void clear() { mBinary = 0; mReflection = 0; }
        //@}

        // ********************************
        // protected functions
        // ********************************
    protected:

        ///
        /// get shader binary
        ///
        ID3D10Blob * getBinary() const { return mBinary; }

        // ********************************
        // private variables
        // ********************************
    private:

        // shader type dependent function pointers
        typedef void (__stdcall ID3D10Device::*SetConstantBuffers)(
                UINT StartSlot,
                UINT NumBuffers,
                ID3D10Buffer *const * ppConstantBuffers );

        const char *                            mProfile;
        ID3D10Blob *                            mBinary;      ///< shader binary
        ID3D10ShaderReflection *                mReflection;  ///< shader reflection
        StackArray<ID3D10Buffer*,16>            mConstBufs;   ///< constant buffers
        mutable StackArray<DynaArray<UInt8>,16> mConstCopies; ///< constant buffers

        SetConstantBuffers                      mSetConstantBuffers;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// D3D10 HLSL GPU program
    ///
    class D3D10GpuProgram : public GpuProgram, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D10GpuProgram, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10GpuProgram( D3D10Renderer & r ) : mVs(r, "vs_4_0"), mGs(r, "gs_4_0"), mPs(r, "ps_4_0") { clear(); }
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

        void apply() const;

        // ********************************
        // private variables
        // ********************************
    private:

        D3D10ShaderHlsl         mVs;
        D3D10ShaderHlsl         mGs;
        D3D10ShaderHlsl         mPs;
        GpuProgramParameterDesc mParamDesc;

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

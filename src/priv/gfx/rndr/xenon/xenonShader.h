#ifndef __GN_GFX_XENONSHADER_H__
#define __GN_GFX_XENONSHADER_H__
// *****************************************************************************
/// \file
/// \brief   D3D shader classes
/// \author  chenlee (2005.11.26)
// *****************************************************************************

#include "xenonResource.h"
#include "../common/basicShader.h"

namespace GN { namespace gfx
{
    ///
    /// Basic Xenon GPU program class
    ///
    struct XenonBasicGpuProgram : public GpuProgram
    {
        ///
        /// Apply program to D3D device
        ///
        virtual void apply() const = 0;

        ///
        /// Apply uniforms to D3D device
        ///
        virtual void applyUniforms( const SysMemUniform * const * uniforms, size_t count ) const = 0;
    };

    // *************************************************************************
    // ASM shaders
    // *************************************************************************

    ///
    /// Xenon ASM (microcodes) GPU program
    ///
    class XenonGpuProgramASM : public XenonBasicGpuProgram, public XenonResource, public StdClass
    {
        GN_DECLARE_STDCLASS( XenonGpuProgramASM, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XenonGpuProgramASM( XenonRenderer & r ) : XenonResource(r) { clear(); }
        virtual ~XenonGpuProgramASM() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GpuProgramDesc & desc );
        void quit();
    private:
        void clear() { mVs = 0; mPs = 0; }
        //@}

        // ********************************
        // from XenonBasicGpuProgram
        // ********************************
    public:

        virtual void apply() const;
        virtual void applyUniforms( const SysMemUniform * const * uniforms, size_t count ) const;

        // ********************************
        // from Shader
        // ********************************
    private:

        const GpuProgramParameterDesc & getParameterDesc() const { return mParamDesc; }

        // ********************************
        // private variables
        // ********************************
    private:

        IDirect3DVertexShader9 * mVs;
        IDirect3DPixelShader9  * mPs;
        GpuProgramParameterDesc  mParamDesc;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    // *************************************************************************
    // HLSL shaders
    // *************************************************************************

    ///
    /// Xenon HLSL GPU program class
    ///
    class XenonGpuProgramHLSL : public XenonBasicGpuProgram, public XenonResource, public StdClass
    {
         GN_DECLARE_STDCLASS( XenonGpuProgramHLSL, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XenonGpuProgramHLSL( XenonRenderer & r ) : XenonResource(r) { clear(); }
        virtual ~XenonGpuProgramHLSL() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GpuProgramDesc & desc );
        void quit();
    private:
        void clear() { mVs = 0; mVsConsts = 0; mPs = 0; mPsConsts = 0; }
        //@}

        // ********************************
        // from XenonBasicGpuProgram
        // ********************************
    public:

        virtual void apply() const;
        virtual void applyUniforms( const SysMemUniform * const * uniforms, size_t count ) const;

        // ********************************
        // from Shader
        // ********************************
    private:

        const GpuProgramParameterDesc & getParameterDesc() const { return mParamDesc; }

        // ********************************
        // private variables
        // ********************************
    private:

        class XenonGpuProgramParameterDesc : public GpuProgramParameterDesc
        {
            friend class XenonGpuProgramHLSL; // so that XenonGpuProgramHLSL can access private members of parameter descriptor
        };

        struct XenonUniformParamDesc : public GpuProgramUniformParameterDesc
        {
            StrA                 name;      ///< uniform name
            D3DXHANDLE           vshandle;  ///< VS constant handle. 0 means unused.
            D3DXHANDLE           pshandle;  ///< PS constant handle. 0 means unused.
            XenonUniformParamDesc() : vshandle(0), pshandle(0) {}
        };

        IDirect3DVertexShader9          * mVs;
        ID3DXConstantTable              * mVsConsts;
        IDirect3DPixelShader9           * mPs;
        ID3DXConstantTable              * mPsConsts;

        DynaArray<XenonUniformParamDesc>  mUniforms;

        XenonGpuProgramParameterDesc      mParamDesc;

        // ********************************
        // private functions
        // ********************************
    private:

        bool enumerateConsts( ID3DXConstantTable * constBuffer, bool vs );
        void buildUnformNameAndSizeArray();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONSHADER_H__

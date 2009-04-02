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
        /// Apply program as well as all uniforms to D3D device
        ///
        //virtual void apply() const = 0;

        ///
        /// Apply only dirty uniforms to D3D device
        ///
        //virtual void applyDirtyUniforms() const = 0;
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
        void clear() { mVs = 0; mPs = 0; }
        //@}

        // ********************************
        // from XenonResource
        // ********************************
    public:

        // ********************************
        // from XenonBasicGpuProgram
        // ********************************
    public:

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
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONSHADER_H__

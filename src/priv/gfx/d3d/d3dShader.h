#ifndef __GN_GFXD3D_D3DSHADER_H__
#define __GN_GFXD3D_D3DSHADER_H__
// *****************************************************************************
//! \file    d3d/d3dShader.h
//! \brief   D3D shader classes
//! \author  chenlee (2005.11.26)
// *****************************************************************************

#include "d3dResource.h"

namespace GN { namespace gfx
{
    //!
    //! Basic D3D shader class
    //!
    struct D3DBasicShader
    {
        //!
        //! Apply shader as well as shader constants to D3D device
        //!
        virtual void apply() const = 0;
    };
    
    //!
    //! D3D asm vertex shader class
    //!
    class D3DVtxShaderAsm : public Shader, public D3DBasicShader, public D3DResource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3DVtxShaderAsm, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3DVtxShaderAsm( D3DRenderer & r )
            : Shader(VERTEX_SHADER,LANG_D3D_ASM)
            , D3DResource(r)
        { clear(); }
        virtual ~D3DVtxShaderAsm() { quit(); }
        //@}

        // ********************************
        // standard init/quit
        // ********************************

        //@{
    public:
        bool init( const StrA & code );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mCode.clear(); mMachineCode = 0; mD3DShader = 0; }
        //@}

        // ********************************
        // from Shader
        // ********************************
    public:

        // ********************************
        // from D3DResource
        // ********************************
    public:
        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from D3DBasicShader
        // ********************************
    public:

        void apply() const;

        // ********************************
        // private variables
        // ********************************
    private:
        StrA                    mCode;
        LPD3DXBUFFER            mMachineCode;
        LPDIRECT3DVERTEXSHADER9 mD3DShader;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    //!
    //! D3D asm pixel shader class
    //!
    class D3DPxlShaderAsm : public Shader, public D3DBasicShader, public D3DResource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3DPxlShaderAsm, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3DPxlShaderAsm( D3DRenderer & r )
            : Shader(PIXEL_SHADER,LANG_D3D_ASM)
            , D3DResource(r)
        { clear(); }
        virtual ~D3DPxlShaderAsm() { quit(); }
        //@}

        // ********************************
        // standard init/quit
        // ********************************

        //@{
    public:
        bool init( const StrA & code );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mCode.clear(); mMachineCode = 0; mD3DShader = 0; }
        //@}

        // ********************************
        // from Shader
        // ********************************
    public:

        // ********************************
        // from D3DResource
        // ********************************
    public:
        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from D3DBasicShader
        // ********************************
    public:

        void apply() const;

        // ********************************
        // private variables
        // ********************************
    private:
        StrA                   mCode;
        LPD3DXBUFFER           mMachineCode;
        LPDIRECT3DPIXELSHADER9 mD3DShader;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of d3dShader.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DSHADER_H__

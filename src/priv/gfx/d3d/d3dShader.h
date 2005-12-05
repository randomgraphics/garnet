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

        //!
        //! Apply only dirty uniforms to D3D device
        //!
        virtual void applyDirtyUniforms() const = 0;
    };

    //!
    //! Basic D3D asm shader class
    //!
    struct D3DShaderAsm : public D3DBasicShader
    {
        enum
        {
            CONST_F = 1, //!< float const
            CONST_I = 2, //!< integer const
            CONST_B = 3, //!< bool const
        };

        //!
        //! Asm shader constant descriptor
        //!
        union D3DAsmShaderDesc
        {
            //!
            //! shader constant descriptor as uint32
            //!
            uint32_t u32;

            struct
            {
                uint16_t type; //!< should be one of CONST_F, CONST_I, CONST_B
                uint16_t index; //!< const index.
            };
        };
    };

    //!
    //! D3D asm vertex shader class
    //!
    class D3DVtxShaderAsm : public Shader, public D3DShaderAsm, public D3DResource, public StdClass
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
        void applyDirtyUniforms() const;

        // ********************************
        // private variables
        // ********************************
    private:
        StrA                    mCode;
        LPD3DXBUFFER            mMachineCode;
        LPDIRECT3DVERTEXSHADER9 mD3DShader;

        size_t mMaxConstF, mMaxConstI, mMaxConstB;

        // ********************************
        // private functions
        // ********************************
    private:
        bool compileShader();
        bool analyzeUniforms();
        void applyUniform( LPDIRECT3DDEVICE9, const Uniform & ) const;
        bool queryDeviceUniform( const char * name, HandleType * userData ) const;
    };

    //!
    //! D3D asm pixel shader class
    //!
    class D3DPxlShaderAsm : public Shader, public D3DShaderAsm, public D3DResource, public StdClass
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
        void applyDirtyUniforms() const;

        // ********************************
        // private variables
        // ********************************
    private:
        StrA                   mCode;
        LPD3DXBUFFER           mMachineCode;
        LPDIRECT3DPIXELSHADER9 mD3DShader;

        size_t mMaxConstF, mMaxConstI, mMaxConstB;

        // ********************************
        // private functions
        // ********************************
    private:
        bool compileShader();
        bool analyzeUniforms();
        void applyUniform( LPDIRECT3DDEVICE9, const Uniform & ) const;
        bool queryDeviceUniform( const char * name, HandleType * userData ) const;
    };
}}

#if GN_ENABLE_INLINE
#include "d3dVtxShaderAsm.inl"
#include "d3dPxlShaderAsm.inl"
#endif

// *****************************************************************************
//                           End of d3dShader.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DSHADER_H__

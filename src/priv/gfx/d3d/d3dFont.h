#ifndef __GN_GFXD3D_D3DFONT_H__
#define __GN_GFXD3D_D3DFONT_H__
// *****************************************************************************
//! \file    d3d/d3dFont.h
//! \brief   D3D Font class
//! \author  chenlee (2005.12.2)
// *****************************************************************************

#include "d3dResource.h"

namespace GN { namespace gfx {

#if GN_XENON
    //!
    //! Fake D3D font class for Xenon platform
    //!
    class D3DFont : public StdClass
    {
    public:

        //!
        //! Ctor (for compability to PC version)
        //!
        D3DFont( D3DRenderer & ) {}

        //!
        //! Draw unicode text
        //!
        void drawTextW( const wchar_t *, int, int, const Vector4f & ) {}
    };
#else
    //!
    //! D3D font class
    //!
    class D3DFont : public StdClass, public D3DResource
    {
         GN_DECLARE_STDCLASS( D3DFont, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3DFont( D3DRenderer & r ) : D3DResource(r) { clear(); }
        virtual ~D3DFont() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mFont = 0; }
        //@}

        // ********************************
        // from D3DResource
        // ********************************
    public:

        bool deviceCreate() { return createFont(); }
        bool deviceRestore() { GN_ASSERT( mFont ); GN_DX_CHECK_RV( mFont->OnResetDevice(), false ); return true; }
        void deviceDispose() { if( mFont ) GN_DX_CHECK( mFont->OnLostDevice() ); }
        void deviceDestroy() { safeRelease( mFont ); }

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Draw unicode text
        //!
        void drawTextW( const wchar_t * text, int x, int y, const Vector4f & color );

        // ********************************
        // private variables
        // ********************************
    private:

        LPD3DXFONT mFont;
        int        mFontHeight;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createFont();
    };
#endif
}}

// *****************************************************************************
//                           End of d3dFont.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DFONT_H__

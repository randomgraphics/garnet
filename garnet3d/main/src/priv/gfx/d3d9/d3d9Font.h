#ifndef __GN_GFXD3D9_D3D9FONT_H__
#define __GN_GFXD3D9_D3D9FONT_H__
// *****************************************************************************
//! \file    d3d9/d3d9Font.h
//! \brief   D3D9 Font class
//! \author  chenlee (2005.12.2)
// *****************************************************************************

#include "d3d9Resource.h"

namespace GN { namespace gfx {

#if GN_XENON
    //!
    //! Fake D3D font class for Xenon platform
    //!
    class D3D9Font : public StdClass
    {
    public:

        //!
        //! Ctor (for compability to PC version)
        //!
        D3D9Font( D3D9Renderer & ) {}

        //!
        //! Draw unicode text
        //!
        void drawTextW( const wchar_t *, int, int, const Vector4f & ) {}
    };
#else
    //!
    //! D3D font class
    //!
    class D3D9Font : public StdClass, public D3D9Resource
    {
         GN_DECLARE_STDCLASS( D3D9Font, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9Font( D3D9Renderer & r ) : D3D9Resource(r) { clear(); }
        virtual ~D3D9Font() { quit(); }
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
        // from D3D9Resource
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
//                           End of d3d9Font.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9FONT_H__

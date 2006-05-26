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
    //! D3D9 font class for Xenon platform
    //!
    class D3D9Font : public D3D9Resource, public StdClass
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
        void clear() {}
        //@}

        // ********************************
        // from D3D9Resource
        // ********************************
    public:

        bool deviceRestore();
        void deviceDispose() { mTexture.clear(); }

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

        struct QuadVert
        {
            float x, y, z; // position
            uint32_t c;    // color
            float u, v;    // texcoord
        };

        static const size_t MAX_CHARS = 1024;

        QuadVert         mBuffer[MAX_CHARS*4];
        AutoRef<Texture> mTexture;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createFont();

    };

#else

    //!
    //! D3D9 font class for PC platform.
    //!
    class D3D9Font : public D3D9Resourc, epublic StdClass
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

        bool deviceRestore() { GN_ASSERT( mFont ); GN_DX9_CHECK_RV( mFont->OnResetDevice(), false ); return true; }
        void deviceDispose() { if( mFont ) GN_DX9_CHECK( mFont->OnLostDevice() ); }

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

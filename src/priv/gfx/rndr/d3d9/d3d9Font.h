#ifndef __GN_GFXD3D9_D3D9FONT_H__
#define __GN_GFXD3D9_D3D9FONT_H__
// *****************************************************************************
//! \file    d3d9/d3d9Font.h
//! \brief   D3D9 Font class
//! \author  chenlee (2005.12.2)
// *****************************************************************************

#include "d3d9Resource.h"

namespace GN { namespace gfx {
    //!
    //! D3D9 font class
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
        void clear() { mTexture = 0; }
        //@}

        // ********************************
        // from D3D9Resource
        // ********************************
    public:

        bool deviceRestore();
        void deviceDispose() { safeRelease( mTexture ); }

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Draw unicode text
        //!
        void drawText( const char * text, int x, int y, const Vector4f & color );

        // ********************************
        // private variables
        // ********************************
    private:

        struct QuadVert
        {
            float x, y; // position
            float u, v; // texcoord
            uint32_t c; // color

            void set( float x_, float y_, float u_, float v_, uint32_t c_ )
            {
                x = x_; y = y_; u = u_; v = v_; c = c_;
            }
        };

        static const size_t MAX_CHARS = 1024;

        QuadVert           mBuffer[MAX_CHARS*4];
        LPDIRECT3DTEXTURE9 mTexture;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of d3d9Font.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9FONT_H__

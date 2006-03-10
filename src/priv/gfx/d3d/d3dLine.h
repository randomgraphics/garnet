#ifndef __GN_GFXD3D_D3DLINE_H__
#define __GN_GFXD3D_D3DLINE_H__
// *****************************************************************************
//! \file    d3d/d3dLine.h
//! \brief   D3D line renderer class
//! \author  chenlee (2005.12.2)
// *****************************************************************************

#include "d3dResource.h"

namespace GN { namespace gfx {
    //!
    //! D3D line renderer
    //!
    class D3DLine : public D3DResource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3DLine, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3DLine( D3DRenderer & r ) : D3DResource(r) { clear(); }
        virtual ~D3DLine() { quit(); }
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
        void clear()
        {
            mDecl = 0;
            mVtxShader = 0;
            mPxlShader = 0;
            mVtxBuf = 0;
            mNextLine = 0;
        }
        //@}

        // ********************************
        // from D3DResource
        // ********************************
    public:

        bool deviceCreate();
        bool deviceRestore();
        void deviceDispose();
        void deviceDestroy();

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Draw lines on screen
        //!
        //! \sa Renderer::drawLines
        //!
        void drawLines(
            BitField options,
            const float * positions, size_t stride,
            size_t count, uint32_t color,
            const Matrix44f & model,
            const Matrix44f & view,
            const Matrix44f & proj );

        // ********************************
        // private variables
        // ********************************
    private:

        enum
        {
            MAX_LINES = 4096 //!< maxinum number of lines vertex and index buffer can hold.
        };

        LPDIRECT3DVERTEXDECLARATION9 mDecl;
        LPDIRECT3DVERTEXSHADER9 mVtxShader;
        LPDIRECT3DPIXELSHADER9  mPxlShader;
        LPDIRECT3DVERTEXBUFFER9 mVtxBuf;

        size_t mNextLine; // cursor that indicates next avaiable line in vertex buffer.

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of d3dLine.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DLINE_H__

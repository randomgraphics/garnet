#ifndef __GN_GFXD3D_D3DQUAD_H__
#define __GN_GFXD3D_D3DQUAD_H__
// *****************************************************************************
//! \file    d3d/d3dQuad.h
//! \brief   D3D quad renderer class
//! \author  chenlee (2005.12.2)
// *****************************************************************************

#include "d3dResource.h"

namespace GN { namespace gfx {
    //!
    //! Class for rendering quad(s) on screen
    //!
    class D3DQuad : public D3DResource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3DQuad, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3DQuad( D3DRenderer & r ) : D3DResource(r) { clear(); }
        virtual ~D3DQuad() { quit(); }
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
            mVtxBuf = 0;
            mIdxBuf = 0;
            mVtxShader = 0;
            mPxlShader = 0;
            mFVF = 0;
            mNextQuad = 0;
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
        //! Draw quads on screen
        //!
        //! \sa Renderer::drawQuads
        //!
        void drawQuads(
            const float * positions, size_t posStride,
            const float * texCoords, size_t texStride,
            size_t count, uint32_t options );

        // ********************************
        // private variables
        // ********************************
    private:

        LPDIRECT3DVERTEXBUFFER9 mVtxBuf;
        LPDIRECT3DINDEXBUFFER9  mIdxBuf;
        LPDIRECT3DVERTEXSHADER9 mVtxShader;
        LPDIRECT3DPIXELSHADER9  mPxlShader;

        DWORD  mFVF;

        size_t mNextQuad; // cursor that indicates next avaiable quad in vertex buffer.

        enum
        {
            MAX_QUADS = 4096 //!< maxinum number of quads vertex and index buffer can hold.
        };

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of d3dQuad.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DQUAD_H__

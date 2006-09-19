#ifndef __GN_GFXD3D9_D3D9QUAD_H__
#define __GN_GFXD3D9_D3D9QUAD_H__
// *****************************************************************************
//! \file    d3d9/d3d9Quad.h
//! \brief   D3D9 quad renderer class
//! \author  chenlee (2005.12.2)
// *****************************************************************************

#include "d3d9Resource.h"

namespace GN { namespace gfx {
    //!
    //! Class for rendering quad(s) on screen
    //!
    class D3D9Quad : public D3D9Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D9Quad, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9Quad( D3D9Renderer & r ) : D3D9Resource(r) { clear(); }
        virtual ~D3D9Quad() { quit(); }
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
            mPxlShaderColorTextured = 0;
            mPxlShaderTextured = 0;
            mPxlShaderSolid = 0;
            mVtxBuf = 0;
            mIdxBuf = 0;
            mNextQuad = 0;
        }
        //@}

        // ********************************
        // from D3D9Resource
        // ********************************
    public:

        bool deviceRestore();
        void deviceDispose();

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
            BitField options,
            const float * positions, size_t posStride,
            const float * texCoords, size_t texStride,
            const uint32_t * colors, size_t clrStride,
            size_t count );

        // ********************************
        // private variables
        // ********************************
    private:

        LPDIRECT3DVERTEXDECLARATION9 mDecl;
        LPDIRECT3DVERTEXSHADER9 mVtxShader;
        LPDIRECT3DPIXELSHADER9  mPxlShaderColorTextured;
        LPDIRECT3DPIXELSHADER9  mPxlShaderTextured;
        LPDIRECT3DPIXELSHADER9  mPxlShaderSolid;
        LPDIRECT3DVERTEXBUFFER9 mVtxBuf;
        LPDIRECT3DINDEXBUFFER9  mIdxBuf;

        size_t mNextQuad; // cursor that indicates next avaiable quad in vertex buffer.

        enum
        {
            MAX_QUADS = 4096 //!< maxinum number of quads vertex and index buffer can hold.
        };

        // ********************************
        // private functions
        // ********************************
    private:
        bool createResources();
    };
}}

// *****************************************************************************
//                           End of d3d9Quad.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9QUAD_H__

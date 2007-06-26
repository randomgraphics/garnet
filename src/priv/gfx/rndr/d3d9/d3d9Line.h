#ifndef __GN_GFXD3D9_D3D9LINE_H__
#define __GN_GFXD3D9_D3D9LINE_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 line renderer class
/// \author  chenlee (2005.12.2)
// *****************************************************************************

#include "d3d9Resource.h"

namespace GN { namespace gfx {
    ///
    /// D3D line renderer
    ///
    class D3D9Line : public D3D9Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D9Line, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9Line( D3D9Renderer & r ) : D3D9Resource(r) { clear(); }
        virtual ~D3D9Line() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
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
        // from D3D9Resource
        // ********************************
    public:

        bool deviceRestore();
        void deviceDispose();

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Draw lines on screen
        ///
        /// \sa Renderer::drawLines
        ///
        void drawLines(
            BitFields options,
            const float * positions, size_t stride,
            size_t count, UInt32 rgba,
            const Matrix44f & model,
            const Matrix44f & view,
            const Matrix44f & proj );

        // ********************************
        // private variables
        // ********************************
    private:

        enum
        {
            MAX_LINES = 4096 ///< maxinum number of lines vertex and index buffer can hold.
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
        bool createDeclAndShaders();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9LINE_H__

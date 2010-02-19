#ifndef __GN_GFX_XENONLINE_H__
#define __GN_GFX_XENONLINE_H__
// *****************************************************************************
/// \file
/// \brief   Xenon line renderer
/// \author  chenlee (2005.12.2)
// *****************************************************************************

#include "xenonResource.h"

namespace GN { namespace gfx {

    ///
    /// Xenon line renderer
    ///
    class XenonLine : public XenonResource, public StdClass
    {
         GN_DECLARE_STDCLASS( XenonLine, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XenonLine( XenonGpu & r ) : XenonResource(r) { Clear(); }
        virtual ~XenonLine() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init();
        void Quit();
    private:
        void Clear()
        {
            mDecl = 0;
            mVtxShader = 0;
            mPxlShader = 0;
            mVtxBuf = 0;
            mNextLine = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Draw lines on screen
        ///
        /// \sa Gpu::drawLines
        ///
        void drawLines(
            BitFields         options,
            const float *     positions,
            size_t            stride,
            size_t            numpoints,
            UInt32            rgba,
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
#endif // __GN_GFX_XENONLINE_H__

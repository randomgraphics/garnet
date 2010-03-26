#ifndef __GN_GFX_XENONVTXBUF_H__
#define __GN_GFX_XENONVTXBUF_H__
// *****************************************************************************
/// \file
/// \brief   Xenon vertex buffer class
/// \author  chenlee (2005.11.20)
// *****************************************************************************

#include "xenonResource.h"
#include "../common/basicSurface.h"

namespace GN { namespace gfx
{
    ///
    /// Xenon vertex buffer class.
    ///
    class XenonVtxBuf : public BasicVtxBuf, public XenonResource, public StdClass
    {
         GN_DECLARE_STDCLASS( XenonVtxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XenonVtxBuf( XenonGpu & r ) : XenonResource(r) { Clear(); }
        virtual ~XenonVtxBuf() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const VtxBufDesc & desc );
        void Quit();
    private:
        void Clear()
        {
            mVb = 0;
        }
        //@}

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void Update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag );
        virtual void Readback( DynaArray<UInt8> & data );

        // ********************************
        // public functions
        // ********************************
    public:

        /// return pointer to D3D buffer
        IDirect3DVertexBuffer9 * GetD3DBuffer() const { return mVb; }

        // ********************************
        // private variables
        // ********************************
    private:

        IDirect3DVertexBuffer9 * mVb;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONVTXBUF_H__

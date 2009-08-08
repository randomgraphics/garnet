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
        XenonVtxBuf( XenonRenderer & r ) : XenonResource(r) { clear(); }
        virtual ~XenonVtxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const VtxBufDesc & desc );
        void quit();
    private:
        void clear()
        {
            mVb = 0;
        }
        //@}

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag );
        virtual void readback( std::vector<UInt8> & data );

        // ********************************
        // public functions
        // ********************************
    public:

        /// return pointer to D3D buffer
        IDirect3DVertexBuffer9 * getD3DBuffer() const { return mVb; }

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

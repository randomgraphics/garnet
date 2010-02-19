#ifndef __GN_GFX_XENONIDXBUF_H__
#define __GN_GFX_XENONIDXBUF_H__
// *****************************************************************************
/// \file
/// \brief   Xenon index buffer class
/// \author  chenlee (2005.11.20)
// *****************************************************************************

#include "xenonResource.h"
#include "../common/basicSurface.h"

namespace GN { namespace gfx
{
    ///
    /// Xenon index buffer class.
    ///
    class XenonIdxBuf : public BasicIdxBuf, public XenonResource, public StdClass
    {
         GN_DECLARE_STDCLASS( XenonIdxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XenonIdxBuf( XenonGpu & r ) : XenonResource(r) { Clear(); }
        virtual ~XenonIdxBuf() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const IdxBufDesc & desc );
        void Quit();
    private:
        void Clear()
        {
            mIb = 0;
        }
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual void update( size_t startidx, size_t numidx, const void * data, SurfaceUpdateFlag flag );
        virtual void readback( std::vector<UInt8> & data );

        // ********************************
        // public functions
        // ********************************
    public:

        /// return pointer to D3D buffer
        IDirect3DIndexBuffer9 * getD3DBuffer() const { return mIb; }

        // ********************************
        // private variables
        // ********************************
    private:

        IDirect3DIndexBuffer9 * mIb;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONIDXBUF_H__

#ifndef __GN_GFXD3D_D3DVTXBUF_H__
#define __GN_GFXD3D_D3DVTXBUF_H__
// *****************************************************************************
//! \file    d3d/d3dVtxBuf.h
//! \brief   D3D vertex buffer class
//! \author  chenlee (2005.11.20)
// *****************************************************************************

#include "d3dResource.h"
#include "../common/basicBuffer.h"

namespace GN { namespace gfx
{
    //!
    //! D3D vertex buffer class.
    //!
    class D3DVtxBuf : public BasicVtxBuf, public D3DResource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3DVtxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3DVtxBuf( D3DRenderer & r ) : D3DResource(r) { clear(); }
        virtual ~D3DVtxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t bytes, bool dynamic, bool hasSysCopy, const VtxBufLoader & loader );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mSysCopy.clear();
            mD3DVb = 0;
        }
        //@}

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void * lock( size_t offset, size_t bytes, LockFlag flag );
        virtual void unlock();

        // ********************************
        // from D3DResource
        // ********************************

    public:
        bool deviceCreate() { return true; }
        bool deviceRestore();
        void deviceDispose();
        void deviceDestroy() {}

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Return pointer of D3D vertex buffer
        //!
        LPDIRECT3DVERTEXBUFFER9 getD3DVb() const
        {
            GN_ASSERT( mD3DVb );
            return mD3DVb;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        std::vector<uint8_t>    mSysCopy;
        LPDIRECT3DVERTEXBUFFER9 mD3DVb;
        size_t                  mLockOffset;
        size_t                  mLockBytes;
        LockFlag                mLockFlag;
    };
}}

// *****************************************************************************
//                           End of d3dVtxBuf.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DVTXBUF_H__

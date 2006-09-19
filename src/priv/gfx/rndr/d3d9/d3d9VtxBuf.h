#ifndef __GN_GFXD3D9_D3D9VTXBUF_H__
#define __GN_GFXD3D9_D3D9VTXBUF_H__
// *****************************************************************************
//! \file    d3d9/d3d9VtxBuf.h
//! \brief   D3D vertex buffer class
//! \author  chenlee (2005.11.20)
// *****************************************************************************

#include "d3d9Resource.h"
#include "../common/basicBuffer.h"

namespace GN { namespace gfx
{
    //!
    //! D3D vertex buffer class.
    //!
    class D3D9VtxBuf : public BasicVtxBuf, public D3D9Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D9VtxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9VtxBuf( D3D9Renderer & r ) : D3D9Resource(r) { clear(); }
        virtual ~D3D9VtxBuf() { quit(); }
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

        static Logger * sLogger;
    };
}}

// *****************************************************************************
//                           End of d3d9VtxBuf.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9VTXBUF_H__

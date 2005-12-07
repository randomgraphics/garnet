#ifndef __GN_GFXD3D_D3DIDXBUF_H__
#define __GN_GFXD3D_D3DIDXBUF_H__
// *****************************************************************************
//! \file    d3d/d3dIdxBuf.h
//! \brief   D3D index buffer class
//! \author  chenlee (2005.11.21)
// *****************************************************************************

#include "d3dResource.h"

namespace GN { namespace gfx
{
    //!
    //! D3D index buffer class.
    //!
    class D3DIdxBuf : public IdxBuf, public D3DResource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3DIdxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3DIdxBuf( D3DRenderer & r ) : D3DResource(r) { clear(); }
        virtual ~D3DIdxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mSysCopy.clear();
            mD3DIb = 0;
            mLocked = false;
        }
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual uint16_t * lock( size_t startIdx, size_t numIdx, uint32_t flag );
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
        //! Return pointer of D3D index buffer
        //!
        LPDIRECT3DINDEXBUFFER9 getD3DIb() const
        {
            GN_ASSERT( mD3DIb );
            return mD3DIb;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        std::vector<uint16_t>  mSysCopy;
        LPDIRECT3DINDEXBUFFER9 mD3DIb;

        bool                   mLocked;
        size_t                 mLockStartIdx;
        size_t                 mLockNumIdx;
        uint32_t               mLockFlag;
    };
}}

// *****************************************************************************
//                           End of d3dIdxBuf.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DIDXBUF_H__

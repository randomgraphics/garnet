#ifndef __GN_GFXD3D11_D3D11BUFFER_H__
#define __GN_GFXD3D11_D3D11BUFFER_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 buffer classes
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "d3d11Resource.h"
#include "../common/basicSurface.h"

namespace GN { namespace gfx
{
    ///
    /// D3D buffer class
    ///
    class D3D11Buffer : public D3D11Resource, public StdClass
    {
        GN_DECLARE_STDCLASS( D3D11Buffer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D11Buffer( D3D11Gpu & r ) : D3D11Resource(r) { Clear(); }
        virtual ~D3D11Buffer() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( size_t bytes, bool fastCpuWrite, UInt32 bindFlags );
        void Quit();
    private:
        void Clear()
        {
            mD3DBuffer = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        // Get D3D11 buffer pointer (no reference couter change).
        ID3D11Buffer * GetD3DBuffer() const { GN_ASSERT(mD3DBuffer); return mD3DBuffer; }

        void           Update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag );
        void           Readback( DynaArray<UInt8> & data );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D11Buffer * mD3DBuffer;
        size_t         mBytes;
        bool           mFastCpuWrite;

        // ********************************
        // private function
        // ********************************
    private:
    };

    ///
    /// D3D vertex buffer class.
    ///
    class D3D11VtxBuf : public BasicVtxBuf, public D3D11Buffer
    {
         GN_DECLARE_STDCLASS( D3D11VtxBuf, D3D11Buffer );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D11VtxBuf( D3D11Gpu & r ) : D3D11Buffer(r) { Clear(); }
        virtual ~D3D11VtxBuf() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const VtxBufDesc & desc );
        void Quit();
    private:
        void Clear() {}
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

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private function
        // ********************************
    private:
    };

    ///
    /// D3D index buffer class.
    ///
    class D3D11IdxBuf : public BasicIdxBuf, public D3D11Buffer
    {
        GN_DECLARE_STDCLASS( D3D11IdxBuf, D3D11Buffer );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D11IdxBuf( D3D11Gpu & r ) : D3D11Buffer(r) { Clear(); }
        virtual ~D3D11IdxBuf() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const IdxBufDesc & desc );
        void Quit();
    private:
        void Clear() {}
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual void Update( size_t startidx, size_t numidx, const void * data, SurfaceUpdateFlag flag );
        virtual void Readback( DynaArray<UInt8> & data );

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private function
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11BUFFER_H__

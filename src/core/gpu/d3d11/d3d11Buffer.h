#ifndef __GN_GFXD3D11_D3D11BUFFER_H__
#define __GN_GFXD3D11_D3D11BUFFER_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 buffer classes
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "d3d11Resource.h"
#include "../common/basicSurface.h"

namespace GN {
namespace gfx {
///
/// D3D buffer class
///
class D3D11Buffer : public D3D11Resource, public StdClass {
    GN_DECLARE_STDCLASS(D3D11Buffer, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    D3D11Buffer(D3D11Gpu & r): D3D11Resource(r) { clear(); }
    virtual ~D3D11Buffer() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(uint32_t bytes, bool fastCpuWrite, uint32_t bindFlags);
    void quit();

private:
    void clear() { mD3DBuffer = 0; }
    //@}

    // ********************************
    // public functions
    // ********************************
public:
    //@{

    // Get D3D11 buffer pointer (no reference couter change).
    ID3D11Buffer * getD3DBuffer() const {
        GN_ASSERT(mD3DBuffer);
        return mD3DBuffer;
    }

    void update(uint32_t offset, uint32_t length, const void * data, SurfaceUpdateFlag flag);
    void readback(DynaArray<uint8_t> & data);

    //@}

    // ********************************
    // private variables
    // ********************************
private:
    ID3D11Buffer * mD3DBuffer;
    uint32_t       mBytes;
    bool           mFastCpuWrite;

    // ********************************
    // private function
    // ********************************
private:
};

///
/// D3D vertex buffer class.
///
class D3D11VtxBuf : public BasicVtxBuf, public D3D11Buffer {
    GN_DECLARE_STDCLASS(D3D11VtxBuf, D3D11Buffer);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    D3D11VtxBuf(D3D11Gpu & r): D3D11Buffer(r) { clear(); }
    virtual ~D3D11VtxBuf() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(const VtxBufDesc & desc);
    void quit();

private:
    void clear() {}
    //@}

    // ********************************
    // from VtxBuf
    // ********************************
public:
    virtual void update(uint32_t offset, uint32_t length, const void * data, SurfaceUpdateFlag flag);
    virtual void readback(DynaArray<uint8_t> & data);

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
class D3D11IdxBuf : public BasicIdxBuf, public D3D11Buffer {
    GN_DECLARE_STDCLASS(D3D11IdxBuf, D3D11Buffer);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    D3D11IdxBuf(D3D11Gpu & r): D3D11Buffer(r) { clear(); }
    virtual ~D3D11IdxBuf() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(const IdxBufDesc & desc);
    void quit();

private:
    void clear() {}
    //@}

    // ********************************
    // from IdxBuf
    // ********************************
public:
    virtual void update(uint32_t startidx, uint32_t numidx, const void * data, SurfaceUpdateFlag flag);
    virtual void readback(DynaArray<uint8_t> & data);

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
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11BUFFER_H__

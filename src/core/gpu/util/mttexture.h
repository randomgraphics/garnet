#ifndef __GN_GFX_UTIL_GPU_MTTEXTURE_H__
#define __GN_GFX_UTIL_GPU_MTTEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread texture wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtgpu.h"

namespace GN {
namespace gfx {
///
/// multi thread texture wrapper
///
class MultiThreadTexture : public Texture, public StdClass {
    GN_DECLARE_STDCLASS(MultiThreadTexture, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    MultiThreadTexture(MultiThreadGpu & r): mGpu(r) { clear(); }
    virtual ~MultiThreadTexture() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(Texture *);
    void quit();

private:
    void clear() { mTexture = NULL; }
    //@}

    // ********************************
    // public methods
    // ********************************
public:
    Texture * getRealTexture() const { return mTexture; }

    // ********************************
    // from Texture
    // ********************************
public:
    void     updateMipmap(uint32_t face, uint32_t level, const Box<uint32_t> * area, uint32_t rowPitch, uint32_t slicePitch, const void * data,
                          SurfaceUpdateFlag flag);
    void     readMipmap(uint32_t face, uint32_t level, MipmapData & data);
    void     blobWrite(const void * data, uint32_t length);
    uint32_t blobRead(void * data);
    void     generateMipmapPyramid();
    void *   getAPIDependentData() const;

    // ********************************
    // private variables
    // ********************************
private:
    MultiThreadGpu & mGpu;
    Texture *        mTexture;

    // ********************************
    // private functions
    // ********************************
private:
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTTEXTURE_H__

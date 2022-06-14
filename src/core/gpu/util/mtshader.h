#ifndef __GN_GFX_UTIL_GPU_MTSHADER_H__
#define __GN_GFX_UTIL_GPU_MTSHADER_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread shader wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtgpu.h"

namespace GN {
namespace gfx {
///
/// multithread GPU uniform
///
class MultiThreadUniform : public Uniform, public StdClass {
    GN_DECLARE_STDCLASS(MultiThreadUniform, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    MultiThreadUniform(MultiThreadGpu & r): mGpu(r) { clear(); }
    virtual ~MultiThreadUniform() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(Uniform *);
    void quit();

private:
    void clear() {
        mUniform      = NULL;
        mFrontEndData = NULL;
    }
    //@}

    // ********************************
    // public functions
    // ********************************
public:
    Uniform * getRealUniform() const { return mUniform; }

    // ********************************
    // from GpuProgram
    // ********************************
public:
    virtual uint32       size() const { return mSize; }
    virtual const void * getval() const { return mFrontEndData; }
    virtual void         update(uint32 offset, uint32 length, const void * data);

    // ********************************
    // private variables
    // ********************************
private:
    MultiThreadGpu & mGpu;
    Uniform *        mUniform;
    uint32           mSize;
    uint8 *          mFrontEndData;

    // ********************************
    // private functions
    // ********************************
private:
};

///
/// multithread GPU program wrapper
///
class MultiThreadGpuProgram : public GpuProgram, public StdClass {
    GN_DECLARE_STDCLASS(MultiThreadGpuProgram, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    MultiThreadGpuProgram(MultiThreadGpu & r): mGpu(r) { clear(); }
    virtual ~MultiThreadGpuProgram() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(GpuProgram *);
    void quit();

private:
    void clear() { mGpuProgram = NULL; }
    //@}

    // ********************************
    // public method
    // ********************************
public:
    GpuProgram * getRealGpuProgram() const { return mGpuProgram; }

    // ********************************
    // from GpuProgram
    // ********************************
public:
    virtual const GpuProgramParameterDesc & getParameterDesc() const { return *mParamDesc; }

    // ********************************
    // private variables
    // ********************************
private:
    MultiThreadGpu &                mGpu;
    GpuProgram *                    mGpuProgram;
    const GpuProgramParameterDesc * mParamDesc;

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
#endif // __GN_GFX_UTIL_GPU_MTSHADER_H__

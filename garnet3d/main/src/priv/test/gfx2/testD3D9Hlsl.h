#ifndef __GN_TESTGFX2_TESTD3D9HLSL_H__
#define __GN_TESTGFX2_TESTD3D9HLSL_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 hlsl kernel test application
/// \author  chenli@@FAREAST (2007.6.25)
// *****************************************************************************

#include "testapp.h"

using namespace GN::gfx;
using namespace GN::util;

///
/// D3D9 HLSL kernel test application
///
class TestD3D9Hlsl : public Gfx2TestApp
{
    Kernel             * mKernel;
    KernelParameterSet * mParam;
    Surface            * mVtxBuf;
    Surface            * mIdxBuf;
    Surface            * mTexture;
    KernelPortBinding  * mBinding;

    Matrix44f mProjView;
    ArcBall   mArcBall;

public:

    ///
    /// ctor
    ///
    TestD3D9Hlsl() : mKernel(0), mParam(0), mVtxBuf(0), mIdxBuf(0), mTexture(0), mBinding( 0 ) {}

    // from parent class
    //@{

    virtual bool init( GraphicsSystem & gs );
    virtual void quit( GraphicsSystem & gs );
    virtual void draw( GraphicsSystem & gs );

    //@}
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_TESTGFX2_TESTD3D9HLSL_H__

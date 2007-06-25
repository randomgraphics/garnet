#ifndef __GN_TESTGFX2_TESTD3D9HLSL_H__
#define __GN_TESTGFX2_TESTD3D9HLSL_H__
// *****************************************************************************
//! \file    gfx2/testD3D9Hlsl.h
//! \brief   D3D9 hlsl effect test application
//! \author  chenli@@FAREAST (2007.6.25)
// *****************************************************************************

#include "testapp.h"

using namespace GN::gfx2;

///
/// D3D9 HLSL effect test application
///
class TestD3D9Hlsl : public Gfx2TestApp
{
    Effect                       * mEffect;
    AutoObjPtr<EffectParameterSet> mParam;
    EffectBinding                  mBinding;

public:

    ///
    /// ctor
    ///
    TestD3D9Hlsl() : mBinding( 0 ) {}

    // from parent class
    //@{

    virtual bool init( GraphicsSystem & gs );
    virtual void quit( GraphicsSystem & gs );
    virtual void draw( GraphicsSystem & gs );

    //@}
};

// *****************************************************************************
//                           End of testD3D9Hlsl.h
// *****************************************************************************
#endif // __GN_TESTGFX2_TESTD3D9HLSL_H__

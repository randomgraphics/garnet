#include "pch.h"
#include "testD3D9Hlsl.h"

using namespace GN;
using namespace GN::gfx2;

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool TestD3D9Hlsl::init( GraphicsSystem & gs )
{
    mEffect = gs.getEffect( "D3D9_HLSL" );
    if( 0 == mEffect ) return false;

    mParam.attach( mEffect->createParameterSet() );
    if( 0 == mParam ) return false;

    EffectBindingDesc bd;
    mBinding = mEffect->createBinding( bd );
    if( 0 == mBinding ) return false;

    return true;
}

//
//
// -----------------------------------------------------------------------------
void TestD3D9Hlsl::quit( GraphicsSystem & )
{
    mParam.clear();
    if( mBinding )
    {
        GN_ASSERT( mEffect );
        mEffect->deleteBinding( mBinding );
        mBinding = 0;
    }
}

//
//
// -----------------------------------------------------------------------------
void TestD3D9Hlsl::draw( GraphicsSystem & )
{
    GN_ASSERT( mEffect );
    GN_ASSERT( mParam );
    mEffect->render( *mParam, mBinding );
}

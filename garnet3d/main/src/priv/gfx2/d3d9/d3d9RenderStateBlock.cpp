#include "pch.h"
#include <algorithm>

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9RenderStateBlock");

// *****************************************************************************
// private types
// *****************************************************************************

struct D3D9DeviceStateValue
{
    bool               valid;
    DWORD              value;
    D3D9DeviceStateValue() : valid(false) {}
};

struct D3D9DefaultDeviceStates
{
    GN::FixedArray<D3D9DeviceStateValue,210> rs;

    D3D9DefaultDeviceStates()
    {
        #define GN_D3D9_RENDER_STATE( x, y ) rs[x].valid = true; rs[x].value = y;
        #include "d3d9RenderStateMeta.h"
        #undef  GN_D3D9_RENDER_STATE
    }
};

static D3D9DefaultDeviceStates sDefaultD3D9DeviceStates;

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void sSetRs(
    IDirect3DDevice9 * dev,
    D3DRENDERSTATETYPE type,
    DWORD              value,
    bool               pure )
{
    if( pure )
    {
        dev->SetRenderState( type, value );
    }
    else
    {
        DWORD old;
        dev->GetRenderState( type, &old );
        if( old != value ) dev->SetRenderState( type, value );
    }
}

// *****************************************************************************
// public method
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::sSetupDefaultDeviceStates( D3D9GraphicsSystem & gs )
{
    const D3D9GraphicsSystemDesc & desc = gs.getD3D9Desc();

    bool pure = !!(desc.behavior & D3DCREATE_PUREDEVICE);

    PIXPERF_FUNCTION_EVENT();

    for( size_t i = 0; i < GN_ARRAY_COUNT(sDefaultD3D9DeviceStates.rs); ++i )
    {
        const D3D9DeviceStateValue & v = sDefaultD3D9DeviceStates.rs[i];
        if( v.valid )
        {
            sSetRs( desc.device, (D3DRENDERSTATETYPE)i, sDefaultD3D9DeviceStates.rs[i].value, pure );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9RenderStateBlock::D3D9RenderStateBlock( D3D9GraphicsSystem & gs )
{
    mDevice = gs.d3ddev();

    if( 0 == mDevice )
    {
        GN_UNEXPECTED();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::setRenderState( D3DRENDERSTATETYPE type, DWORD value )
{
    GN_ASSERT( sDefaultD3D9DeviceStates.rs[type].valid );

    if( value == sDefaultD3D9DeviceStates.rs[type].value )
    {
        if( mRsValues[type].valid )
        {
            mRsValues[type].valid = false;
            GN_ASSERT( 1 == std::count( mRsTypes.begin(), mRsTypes.end(), type ) );
            std::remove( mRsTypes.begin(), mRsTypes.end(), type );
            mRsTypes.popBack();
        }
        else
        {
            GN_ASSERT( 0 == std::count( mRsTypes.begin(), mRsTypes.end(), type ) );
        }
    }
    else if( mRsValues[type].valid )
    {
        GN_ASSERT( 1 == std::count( mRsTypes.begin(), mRsTypes.end(), type ) );
        D3DRENDERSTATETYPE * t = std::find( mRsTypes.begin(), mRsTypes.end(), type );
        GN_ASSERT( mRsTypes.begin() <= t && t < mRsTypes.end() );
        mRsValues[*t].value = value;
    }
    else
    {
        GN_ASSERT( 0 == std::count( mRsTypes.begin(), mRsTypes.end(), type ) );
        mRsValues[type].valid = true;
        mRsValues[type].value = value;
        mRsTypes.pushBack( type );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::unsetRenderState( D3DRENDERSTATETYPE type )
{
    GN_ASSERT( sDefaultD3D9DeviceStates.rs[type].valid );
    if( mRsValues[type].valid )
    {
        mRsValues[type].valid = false;
        GN_ASSERT( 1 == std::count( mRsTypes.begin(), mRsTypes.end(), type ) );
        std::remove( mRsTypes.begin(), mRsTypes.end(), type );
        mRsTypes.popBack();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::setSamplerState( size_t stage, D3DSAMPLERSTATETYPE type, DWORD value )
{
    GN_UNUSED_PARAM( stage );
    GN_UNUSED_PARAM( type );
    GN_UNUSED_PARAM( value );
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::setTextureState( size_t stage, D3DTEXTURESTAGESTATETYPE type, DWORD value )
{
    GN_UNUSED_PARAM( stage );
    GN_UNUSED_PARAM( type );
    GN_UNUSED_PARAM( value );
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RenderStateBlock::apply( const D3D9RenderStateBlock * last ) const
{
    // apply render states
    if( last )
    {
        D3DRENDERSTATETYPE t;
        for( size_t i = 0; i < mRsTypes.size(); ++i )
        {
            t = mRsTypes[i];
            const StateValue & v = mRsValues[t];
            GN_ASSERT( v.valid );
            if( !last->mRsValues[t].valid || last->mRsValues[t].value != v.value )
            {
                mDevice->SetRenderState( t, v.value );
            }
        }
    }
    else
    {
        D3DRENDERSTATETYPE t;
        for( size_t i = 0; i < mRsTypes.size(); ++i )
        {
            t = mRsTypes[i];
            GN_ASSERT( mRsValues[t].valid );
            mDevice->SetRenderState( t, mRsValues[t].value );
        }
    }
}

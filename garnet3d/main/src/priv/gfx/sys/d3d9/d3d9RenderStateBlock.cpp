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
    GN::FixedArray<D3D9DeviceStateValue,14>  ss;

    D3D9DefaultDeviceStates()
    {
        #define GN_D3D9_RENDER_STATE( x, y ) rs[x].valid = true; rs[x].value = y;
        #include "d3d9RenderStateMeta.h"
        #undef  GN_D3D9_RENDER_STATE

        #define GN_D3D9_SAMPLER_STATE( x, y ) ss[x].valid = true; ss[x].value = y;
        #include "d3d9SamplerStateMeta.h"
        #undef  GN_D3D9_SAMPLER_STATE
    }
};

static D3D9DefaultDeviceStates sDefaultD3D9DeviceStates;

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static UInt32 sGetNumTextureStages( const D3DCAPS9 & d3dcaps )
{
    UInt32 s = 0;

    //if( d3dcaps.VertexShaderVersion >= D3DVS_VERSION(3,0) ) s += 4;

    if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(2,0) ) s += 16;
    else if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,4) ) s += 6;
    else if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,1) ) s += 4;
    else s += d3dcaps.MaxTextureBlendStages;

    return s;
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
            if( pure )
            {
                desc.device->SetRenderState( (D3DRENDERSTATETYPE)i, v.value );
            }
            else
            {
                DWORD old;
                desc.device->GetRenderState( (D3DRENDERSTATETYPE)i, &old );
                if( old != v.value ) desc.device->SetRenderState( (D3DRENDERSTATETYPE)i, v.value );
            }
        }
    }

    UInt32 numStages = sGetNumTextureStages( desc.caps );
    for( UInt32 s = 0; s < numStages; ++s )
    for( size_t i = 0; i < GN_ARRAY_COUNT(sDefaultD3D9DeviceStates.ss); ++i )
    {
        const D3D9DeviceStateValue & v = sDefaultD3D9DeviceStates.ss[i];
        if( v.valid )
        {
            if( pure )
            {
                desc.device->SetSamplerState( s, (D3DSAMPLERSTATETYPE)i, v.value );
            }
            else
            {
                DWORD old;
                desc.device->GetSamplerState( s, (D3DSAMPLERSTATETYPE)i, &old );
                if( old != v.value ) desc.device->SetSamplerState( s, (D3DSAMPLERSTATETYPE)i, v.value );
            }
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9RenderStateBlock::D3D9RenderStateBlock( D3D9GraphicsSystem & gs )
    : mGfxSys( gs )
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
GN::gfx::D3D9RenderStateBlock::~D3D9RenderStateBlock()
{
    if( this == mGfxSys.getCurrentRenderStateBlock() )
    {
        mGfxSys.setRenderStateBlock( 0 );
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
    if( stage >= 16 )
    {
        GN_UNIMPL();
        GN_ASSERT( stage >= D3DDMAPSAMPLER );
        stage -= D3DDMAPSAMPLER;
        GN_ASSERT( stage < 21 );
    }

    GN_ASSERT( sDefaultD3D9DeviceStates.ss[type].valid );

    StageState newss = { (UINT32)stage, type };

    if( value == sDefaultD3D9DeviceStates.ss[type].value )
    {
        if( mSsValues[stage][type].valid )
        {
            // set to default value
            mSsValues[stage][type].valid = false;
            GN_ASSERT( 1 == std::count( mSsTypes.begin(), mSsTypes.end(), newss ) );
            std::remove( mSsTypes.begin(), mSsTypes.end(), newss );
            mSsTypes.popBack();
        }
        else
        {
            GN_ASSERT( 0 == std::count( mSsTypes.begin(), mSsTypes.end(), newss ) );
        }
    }
    else if( mSsValues[stage][type].valid )
    {
        // update to new (non-default) value
        GN_ASSERT( 1 == std::count( mSsTypes.begin(), mSsTypes.end(), newss ) );
        const StageState * t = std::find( mSsTypes.begin(), mSsTypes.end(), newss );
        GN_ASSERT( mSsTypes.begin() <= t && t < mSsTypes.end() );
        mSsValues[t->stage][t->type].value = value;
    }
    else
    {
        // set to non-default value
        GN_ASSERT( 0 == std::count( mSsTypes.begin(), mSsTypes.end(), newss ) );
        mSsValues[stage][type].valid = true;
        mSsValues[stage][type].value = value;
        mSsTypes.pushBack( newss );
    }
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
    if( last )
    {
        applyRenderStates( *last );
        applySamplerStates( *last );
    }
    else
    {
        applyRenderStates();
        applySamplerStates();
    }
}

// *****************************************************************************
// private method
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9RenderStateBlock::applyRenderStates() const
{
    for( size_t i = 0; i < mRsTypes.size(); ++i )
    {
        D3DRENDERSTATETYPE t = mRsTypes[i];
        GN_ASSERT( mRsValues[t].valid );
        mDevice->SetRenderState( t, mRsValues[t].value );
    }
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9RenderStateBlock::applyRenderStates( const D3D9RenderStateBlock & last ) const
{
    for( size_t i = 0; i < last.mRsTypes.size(); ++i )
    {
        D3DRENDERSTATETYPE t = last.mRsTypes[i];

        const StateValue & oldv = last.mRsValues[t];
        const StateValue & newv = mRsValues[t];

        GN_ASSERT( oldv.valid );

        if( !newv.valid )
        {
            mDevice->SetRenderState( t, sDefaultD3D9DeviceStates.rs[t].value );
        }
        else if( newv.value != oldv.value )
        {
            mDevice->SetRenderState( t, newv.value );
        }
    }

    // apply new rsb
    for( size_t i = 0; i < mRsTypes.size(); ++i )
    {
        D3DRENDERSTATETYPE t = mRsTypes[i];

        const StateValue & oldv = last.mRsValues[t];

        const StateValue & newv = mRsValues[t];

        GN_ASSERT( newv.valid );

        if( !oldv.valid )
        {
            mDevice->SetRenderState( t, newv.value );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9RenderStateBlock::applySamplerStates() const
{
    for( size_t i = 0; i < mSsTypes.size(); ++i )
    {
        const StageState & t = mSsTypes[i];
        GN_ASSERT( mSsValues[t.stage][t.type].valid );
        mDevice->SetSamplerState( t.stage, t.type, mSsValues[t.stage][t.type].value );
    }
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9RenderStateBlock::applySamplerStates( const D3D9RenderStateBlock & last ) const
{
    for( size_t i = 0; i < last.mSsTypes.size(); ++i )
    {
        const StageState &    t = last.mSsTypes[i];
        const StateValue & oldv = last.mSsValues[t.stage][t.type];
        const StateValue & newv = mSsValues[t.stage][t.type];

        GN_ASSERT( oldv.valid );

        if( !newv.valid )
        {
            mDevice->SetSamplerState( t.stage, t.type, sDefaultD3D9DeviceStates.ss[t.type].value );
        }
        else if( newv.value != oldv.value )
        {
            mDevice->SetSamplerState( t.stage, t.type, newv.value );
        }
    }

    // apply new rsb
    for( size_t i = 0; i < mSsTypes.size(); ++i )
    {
        const StageState &    t = mSsTypes[i];
        const StateValue & oldv = last.mSsValues[t.stage][t.type];
        const StateValue & newv = mSsValues[t.stage][t.type];

        GN_ASSERT( newv.valid );

        if( !oldv.valid )
        {
            mDevice->SetSamplerState( t.stage, t.type, newv.value );
        }
    }
}

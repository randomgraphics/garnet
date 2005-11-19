#include "pch.h"
#include "basicRenderer.h"

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// render state block management functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::rsbInit()
{
    GN_GUARD;

    // create an "invalid" render state block as current render state block
    mCurrentRsb = createRenderStateBlock( RenderStateBlockDesc::DEFAULT );
    if( 0 == mCurrentRsb ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::rsbDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mRsbHandles.empty() && mDevRsbMap.empty() );

    GN_ASSERT( mRsbHandles.validHandle(mCurrentRsb) );

    // rebind current render state block to rendering device
    AutoRef<DeviceRenderStateBlock> newDevRsb(
        createDeviceRenderStateBlock( RenderStateBlockDesc::INVALID, mRsbHandles[mCurrentRsb] ) );
    if( newDevRsb.empty() ) return false;
    newDevRsb->apply();

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::BasicRenderer::createRenderStateBlock( const RenderStateBlockDesc & desc )
{
    GN_GUARD;

    // find for existing render stage block
    uint32_t handle = mRsbHandles.find( desc );

    if( 0 == handle )
    {
        // not found. we have to create new one
        handle = mRsbHandles.add( desc );
        if( 0 == handle )
        {
            GNGFX_ERROR( "Fail to create new render state block!" );
        }
    }

    return handle;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::bindRenderStateBlock( uint32_t handle )
{
    GN_GUARD;

    // ignore redundant binding
    if( handle == mCurrentRsb ) return;

    // check handle
    if( !mRsbHandles.validHandle( handle ) )
    {
        GNGFX_ERROR( "Invalid render state handle!" );
        return;
    }

    // find device render state block
    GN_ASSERT( mRsbHandles.validHandle(mCurrentRsb) );
    DevRsbKey key;
    key.from = mCurrentRsb;
    key.to = handle;
    DeviceRenderStateBlockRefPtr & devRsb = mDevRsbMap[key];

    if( devRsb.empty() )
    {
        // create new device render state block
        devRsb.attach( createDeviceRenderStateBlock( mRsbHandles[mCurrentRsb], mRsbHandles[handle] ) );
        if( devRsb.empty() ) return;
    }

    // apply render state to device
    GN_ASSERT( !devRsb.empty() );
    devRsb->apply();

    // update current render state handle
    mCurrentRsb = handle;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::getCurrentRenderStateBlock( RenderStateBlockDesc & result ) const
{
    GN_GUARD;

    GN_ASSERT( mRsbHandles.validHandle( mCurrentRsb ) );

    result = mRsbHandles[mCurrentRsb];

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::BasicRenderer::setRenderState( RenderState state, RenderStateValue value )
{
    GN_GUARD;

    GN_ASSERT( mRsbHandles.validHandle( mCurrentRsb ) );

    // check for invalid parameters
    if( state < 0 || state >= NUM_RENDER_STATES )
    {
        GNGFX_ERROR( "invalid render state %d!", state );
        return 0;
    }
    if( value < 0 || value >= NUM_RENDER_STATE_VALUES )
    {
        GNGFX_ERROR( "invalid render state value %d!", value );
        return 0;
    }

    RenderStateBlockDesc desc = mRsbHandles.get( mCurrentRsb );
    desc.rs[state] = value;

    // create and apply the updated render state block
    uint32_t handle = createRenderStateBlock( desc );
    if( handle ) bindRenderStateBlock( handle );
    return handle;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::BasicRenderer::setTextureState( uint32_t stage, TextureState state, TextureStateValue value )
{
    GN_GUARD;

    GN_ASSERT( mRsbHandles.validHandle( mCurrentRsb ) );

    // check for invalid parameters
    if( stage >= MAX_TEXTURE_STAGES )
    {
        GNGFX_ERROR( "texture stage '%d' is too large. Maximum value is %d", stage, MAX_TEXTURE_STAGES-1 );
        return 0;
    }
    if( state < 0 || state >= NUM_TEXTURE_STATES )
    {
        GNGFX_ERROR( "invalid texture state %d!", state );
        return 0;
    }
    if( value < 0 || value >= NUM_TEXTURE_STATE_VALUES )
    {
        GNGFX_ERROR( "invalid texture state value %d!", value );
        return 0;
    }

    RenderStateBlockDesc desc = mRsbHandles.get( mCurrentRsb );
    desc.ts[stage][state] = value;

    // create and apply the updated render state block
    uint32_t handle = createRenderStateBlock( desc );
    if( handle ) bindRenderStateBlock( handle );
    return handle;

    GN_UNGUARD;
}

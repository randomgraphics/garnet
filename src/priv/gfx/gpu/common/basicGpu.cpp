#include "pch.h"
#include "basicGpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.common");

//
// RIP for GPU modules
// -----------------------------------------------------------------------------
void GN::gfx::rip( const char * msg, ... )
{
    GN_UNUSED_PARAM(msg);
}

// *****************************************************************************
//                         BasicGpu init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicGpu::init( const GpuOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicGpu, () );

    // check GPU options
    if( o.api < 0 || o.api >= GpuAPI::NUM_APIs )
    {
        if( GpuAPI::AUTO == o.api )
        {
            GN_ERROR(sLogger)( "GpuAPI::AUTO must be changed to actual API value before initializing GPU." );
        }
        else
        {
            GN_ERROR(sLogger)( "Invalid API: %d", o.api.ToRawEnum() );
        }

        return failure();
    }

    // sanity check: warning when render context size is larger than 2K bytes
    if( sizeof(GpuContext) > 2048 )
    {
        GN_WARN(sLogger)( "GN::gfx::GpuContext is huge! (%u bytes)", sizeof(GpuContext) );
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::bindContext( const GpuContext & newContext )
{
    // skip dirty check, if last context binding failed.
    bool skipDirtyCheck = !mContextOk;

    mContextOk = bindContextImpl( newContext, skipDirtyCheck );

    if( mContextOk )
    {
        mContext = newContext;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::rebindContext()
{
    mContextOk = bindContextImpl( mContext, true );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::getBackBufferContent( BackBufferContent & c )
{
    c.data.clear();
    c.format = ColorFormat::UNKNOWN;
    c.width = 0;
    c.height = 0;
    c.pitch = 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::setUserData( const Guid & id, const void * data, size_t length )
{
    UserDataMap::iterator iter = mUserData.find( id );

    if( NULL == data && 0 == length )
    {
        // delete existing data
        if( iter != mUserData.end() )
        {
            mUserData.erase( iter );
        }
        else
        {
            GN_ERROR(sLogger)( "Invalid user data GUID." );
        }
    }
    else if( iter != mUserData.end() )
    {
        // overwrite existing data

        DynaArray<UInt8> & userData = iter->second;

        if( NULL != data && length > 0 )
        {
            userData.resize( length );
            memcpy( userData.cptr(), data, length );
        }
        else
        {
            userData.clear();
        }
    }
    else
    {
        // adding new data
        DynaArray<UInt8> & userData = mUserData[id];

        if( NULL != data && length > 0 )
        {
            userData.resize( length );
            memcpy( userData.cptr(), data, length );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
const void * GN::gfx::BasicGpu::getUserData( const Guid & id, size_t * length ) const
{
    UserDataMap::const_iterator iter = mUserData.find( id );

    if( iter != mUserData.end() )
    {
        const DynaArray<UInt8> & userData = iter->second;

        if( length ) *length = userData.size();

        return userData.empty() ? NULL : userData.cptr();
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid user data GUID." );

        if( length ) *length = 0;

        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicGpu::hasUserData( const Guid & id ) const
{
    return mUserData.find( id ) != mUserData.end();
}

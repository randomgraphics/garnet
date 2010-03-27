#include "pch.h"
#include "basicGpu.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.common");

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
bool GN::gfx::BasicGpu::Init( const GpuOptions & o )
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

        return Failure();
    }

    // sanity check: warning when render context size is larger than 2K bytes
    if( sizeof(GpuContext) > 2048 )
    {
        GN_WARN(sLogger)( "GN::gfx::GpuContext is huge! (%u bytes)", sizeof(GpuContext) );
    }

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::Quit()
{
    GN_GUARD;

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::BindContext( const GpuContext & newContext )
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
void GN::gfx::BasicGpu::GetBackBufferContent( BackBufferContent & c )
{
    c.data.Clear();
    c.format = ColorFormat::UNKNOWN;
    c.width = 0;
    c.height = 0;
    c.pitch = 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::SetUserData( const Guid & id, const void * data, size_t length )
{
    UserData * currentUserData = mUserData.Find( id );

    if( NULL == data && 0 == length )
    {
        // delete existing data
        if( currentUserData )
        {
            mUserData.Remove( id );
        }
        else
        {
            GN_ERROR(sLogger)( "Invalid user data GUID." );
        }
    }
    else if( currentUserData )
    {
        // overwrite existing data

        if( NULL != data && length > 0 )
        {
            currentUserData->Resize( length );
            memcpy( currentUserData->ToRawPtr(), data, length );
        }
        else
        {
            currentUserData->Clear();
        }
    }
    else
    {
        // add new data
        DynaArray<UInt8> & newUserData = mUserData[id];

        if( NULL != data && length > 0 )
        {
            newUserData.Resize( length );
            memcpy( newUserData.ToRawPtr(), data, length );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
const void * GN::gfx::BasicGpu::GetUserData( const Guid & id, size_t * length ) const
{
    const UserData * currentUserData = mUserData.Find( id );

    if( NULL != currentUserData )
    {
        if( length ) *length = currentUserData->Size();

        return currentUserData->ToRawPtr();
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
bool GN::gfx::BasicGpu::HasUserData( const Guid & id ) const
{
    return NULL != mUserData.Find( id );
}

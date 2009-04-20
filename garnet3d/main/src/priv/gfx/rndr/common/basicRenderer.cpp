#include "pch.h"
#include "basicRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.common");

//
// RIP for renderer modules
// -----------------------------------------------------------------------------
void GN::gfx::rip( const char * msg, ... )
{
    GN_UNUSED_PARAM(msg);
}

// *****************************************************************************
//                         BasicRenderer init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::init( const RendererOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicRenderer, () );

    // check renderer options
    if( o.api < 0 || o.api >= RendererAPI::NUM_APIs )
    {
        if( RendererAPI::AUTO == o.api )
        {
            GN_ERROR(sLogger)( "RendererAPI::AUTO must be changed to actual API value before initializing renderer." );
        }
        else
        {
            GN_ERROR(sLogger)( "Invalid API: %d", o.api );
        }

        return failure();
    }

    // sanity check: warning when render context size is larger than 2K bytes
    if( sizeof(RendererContext) > 2048 )
    {
        GN_WARN(sLogger)( "GN::gfx::RendererContext is huge! (%u bytes)", sizeof(RendererContext) );
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::bindContext( const RendererContext & c )
{
    // skip dirty check, if last context binding failed.
    bool skipDirtyCheck = !mContextOk;

    mContextOk = bindContextImpl( c, skipDirtyCheck );

    if( mContextOk )
    {
        mContext = c;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::rebindContext()
{
    mContextOk = bindContextImpl( mContext, true );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::setUserData( const Guid & id, const void * data, size_t length )
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
const void * GN::gfx::BasicRenderer::getUserData( const Guid & id, size_t * length ) const
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
bool GN::gfx::BasicRenderer::hasUserData( const Guid & id ) const
{
    return mUserData.find( id ) != mUserData.end();
}

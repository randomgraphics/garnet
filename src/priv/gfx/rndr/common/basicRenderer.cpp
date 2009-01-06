#include "pch.h"
#include "basicRenderer.h"

GN::Logger * GN::gfx::BasicRenderer::sLogger = GN::getLogger("GN.gfx.rndr.common");

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

    size_t rcsize = sizeof(RendererContext);
    if( rcsize > 2048 )
    {
        GN_WARN(sLogger)( "GN::gfx::RendererContext is huge! (%u bytes)", rcsize );
    }

    // initialize sub-components one by one
    if( !dispInit(o) ) return failure();

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

    // shutdown sub-components in reverse sequence
    dispQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::bindContext( const RendererContext & c )
{
    if( bindContextImpl( c, false ) )
    {
        mContext = c;
    }
    else if( !bindContextImpl( mContext, true ) )
    {
        GN_RNDR_RIP( "fail to recover from context binding failure." );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::rebindContext()
{
    if( !bindContextImpl( mContext, true ) )
    {
        GN_RNDR_RIP( "fail to rebind current context." );
    }
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

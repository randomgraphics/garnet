#include "pch.h"
#include "rescache.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::init( const RenderEngineInitParameters & )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::engine::RenderEngine, () );

    // Do custom init here

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// draw request management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::frameBegin()
{
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::frameEnd()
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::DrawRequest & GN::engine::RenderEngine::newDrawRequest()
{
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::composeAndSubmitResourceRequest(
    DrawRequest &             dr,
    GraphicsResourceOperation op,
    GraphicsResourceId        resourceid,
    int                       lod,
    GraphicsResourceLoader  * loader,
    bool                      forceReload )
{
    const GraphicsResource * res = id2res( resourceid );
    if( 0 == res ) return;

    GraphicsResourceRequest * to_be_disposed = 0;

    if( GRS_DISPOSED == res->state )
    {
        to_be_disposed = mGfxResCache.makeRoomForResource( resourceid, dr.fence );
        /*
        while( there_is_no_enough_room_to_hold_resource( newres ) )
        {
            GraphicsResourceId oldres = get_the_appropriate_resource_to_be_disposed();

            mark_as_disposed( oldres );

            add_to_list( to_be_disposed, oldres );
        }
        */

        res->state = GRS_REALIZED;

        forceReload = true;
    }

    res->draw_fence = dr.fence;

    if( !to_be_disposed.empty() )
    {
        //submit_resource_request_to_rendering_thread(
        //    OP_DISPOSE,
        //    to_be_disposed );
    }

    if( forceReload )
    {
        dr.incPendingResource();
        //submit_resource_request_to_resource_thread(
        //    OP_LOAD,
        //    to_be_loaded );
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

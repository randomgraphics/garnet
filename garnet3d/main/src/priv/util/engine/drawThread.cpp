#include "pch.h"
#include "drawThread.h"
#include "garnet/GNinput.h"
#include "garnet/GNwin.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.DrawThread");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool sCreateDeviceData( GN::engine::GraphicsResource & res )
{
    using namespace GN::engine;
    using namespace GN::gfx;

    Renderer & r = gRenderer;

    switch( res.desc.type )
    {
        case GRT_SHADER :
            GN_ASSERT( 0 == res.shader );
            res.shader = r.createShader( res.desc.sd.type, res.desc.sd.lang, res.desc.sd.code, res.desc.sd.hints );
            if( 0 == res.shader ) return false;
            break;

        case GRT_TEXTURE:
            GN_ASSERT( 0 == res.texture );
            res.texture = r.createTexture( res.desc.td );
            if( 0 == res.texture ) return false;
            break;

        case GRT_VTXBUF :
            GN_ASSERT( 0 == res.vtxbuf );
            res.vtxbuf = r.createVtxBuf( res.desc.vd );
            if( 0 == res.vtxbuf ) return false;
            break;

        case GRT_IDXBUF :
            GN_ASSERT( 0 == res.idxbuf );
            res.idxbuf = r.createIdxBuf( res.desc.id );
            if( 0 == res.idxbuf ) return false;
            break;

        case GRT_CONSTBUF :
            GN_UNIMPL();
            return false;

        default:
            GN_UNEXPECTED();
            return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteDeviceData( GN::engine::GraphicsResource & res )
{
    using namespace GN::engine;

    switch( res.desc.type )
    {
        case GRT_SHADER :
            safeDecref( res.shader );
            break;

        case GRT_TEXTURE:
            safeDecref( res.texture );
            break;

        case GRT_VTXBUF :
            safeDecref( res.vtxbuf );
            break;

        case GRT_IDXBUF :
            safeDecref( res.idxbuf );
            break;

        case GRT_CONSTBUF :
            safeDecref( res.constbuf );
            break;

        default:
            GN_UNEXPECTED();
    }
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
static void sResolveResourceId(
    GN::engine::RenderEngine & engine,
    const T * & data )
{
    using namespace GN::engine;

    if( 0 == data ) return;

    GraphicsResourceId id = (GraphicsResourceId)data;

    GraphicsResourceItem * res = engine.resourceCache().id2ptr( id );
    GN_ASSERT( res );

    data = (const T*)res->data;
}
// *****************************************************************************
// draw command functions
// *****************************************************************************

namespace GN { namespace engine
{
    void DRAWFUNC_SET_CONTEXT( RenderEngine & engine, const void * param, size_t bytes )
    {
        using namespace GN::gfx;

        GN_ASSERT( param && bytes == align<size_t>( sizeof(DrawContext), 4 ) );

        DrawContext * context = (DrawContext*)param;

        // convert ID to resource pointer
        for( int i = 0; i < gfx::NUM_SHADER_TYPES; ++i )
        {
            if( context->flags.shaderBit( i ) ) sResolveResourceId( engine, context->shaders[i] );
        }
        if( context->flags.renderTargets )
        {
            for( int i = 0; i < gfx::MAX_RENDER_TARGETS; ++i )
            {
                sResolveResourceId( engine, context->renderTargets.cbuffers[i].texture );
            }
            sResolveResourceId( engine, context->renderTargets.zbuffer.texture );
        }
        if( context->flags.textures )
        {
            for( unsigned int i = 0; i < context->numTextures; ++i )
            {
                sResolveResourceId( engine, context->textures[i] );
            }
        }
        if( context->flags.vtxBufs )
        {
            for( unsigned int i = 0; i < context->numVtxBufs; ++i )
            {
                sResolveResourceId( engine, context->vtxBufs[i].buffer );
            }
        }
        if( context->flags.idxBuf )
        {
            sResolveResourceId( engine, context->idxBuf );
        }

        gRenderer.setContext( *context );
    }

    void DRAWFUNC_SET_UNIFORM( RenderEngine & engine, const void * param, size_t bytes )
    {
        using namespace gfx;

        struct ParamHeader
        {
            GraphicsResourceId shader;
            char               uniname[32];
            SInt32             unitype;
        };

        GN_ASSERT( param );

        const ParamHeader * header = (const ParamHeader*)param;

        const void * data = header + 1;

        bytes = bytes - sizeof(ParamHeader);

        // compose uniform value
        UniformValue unival;
        unival.type = (UniformValueType)header->unitype;
        switch( unival.type )
        {
            case UVT_BOOL:
            case UVT_FLOAT:
            case UVT_INT :
            case UVT_VECTOR4:
                GN_UNIMPL();
                break;

            case UVT_MATRIX44:
                unival.matrix44s.resize( bytes / sizeof(Matrix44f) );
                memcpy( &unival.matrix44s[0], data, bytes );
                break;

            default:
                GN_UNEXPECTED();
                break;
        }

        // get shader pointer
        GraphicsResource * res = engine.resourceCache().id2ptr(header->shader);
        GN_ASSERT( res && GRT_SHADER == res->desc.type );

        if( res->shader )
        {
            // set shader uniform
            res->shader->setUniformByName( header->uniname, unival );
        }
        else
        {
            GN_ERROR(sLogger)( "Null shader instance: id=%d.", header->shader );
        }
    }

    void DRAWFUNC_CLEAR( RenderEngine &, const void * param, size_t bytes )
    {
        #pragma pack(push,1)
        struct Param
        {
            Vector4f color;
            float    z;
            UInt8    s;
            UInt32   flags;
        };
        #pragma pack(pop)

        GN_ASSERT( param );
        GN_ASSERT( align<size_t>( sizeof(Param), 4 ) == bytes );
        const Param * p = (Param*)param;

        gfx::Renderer & r = gRenderer;

        r.clearScreen( p->color, p->z, p->s, p->flags );
    }

    void DRAWFUNC_DRAW( RenderEngine &, const void * param, size_t bytes )
    {
        struct Param
        {
            SInt32 prim;
            UInt32 numprim;
            UInt32 startvtx;
        };

        GN_ASSERT( param && sizeof(Param) == bytes );

        const Param * p = (const Param*)param;

        gfx::Renderer & r = gRenderer;

        r.draw( (gfx::PrimitiveType)p->prim, p->numprim, p->startvtx );
    }

    void DRAWFUNC_DRAW_INDEXED( RenderEngine &, const void * param, size_t bytes )
    {
        struct Param
        {
            SInt32 prim;
            UInt32 numprim;
            UInt32 startvtx;
            UInt32 minvtxidx;
            UInt32 numvtx;
            UInt32 startidx;
        };
        GN_ASSERT( param && sizeof(Param) == bytes );

        const Param * p = (const Param*)param;

        gfx::Renderer & r = gRenderer;

        r.drawIndexed(
            (gfx::PrimitiveType)p->prim,
            p->numprim,
            p->startvtx,
            p->minvtxidx,
            p->numvtx,
            p->startidx );
    }
}};

// *****************************************************************************
// draw command functions
// *****************************************************************************

namespace GN { namespace engine
{
    void RESFUNC_COPY( RenderEngine & engine,  ResourceCommand & cmd )
    {
        GN_GUARD;

        GraphicsResourceItem * res = engine.resourceCache().id2ptr( cmd.resourceId );
        GN_ASSERT( res );

        if( 0 == res->shader )
        {
            GN_INFO(sLogger)( "Create %s", res->desc.name.cptr() );
            if( !sCreateDeviceData( *res ) )
            {
                cmd.noerr = false;

                // free data buffer and loder
                cmd.loader->freebuf( cmd.data, cmd.bytes );
                cmd.loader.clear();
                return;
            }
        }

        GN_ASSERT( cmd.loader );
        GN_INFO(sLogger)( "Copy %s", res->desc.name.cptr() );
        cmd.noerr = cmd.loader->copy( *res, cmd.data, cmd.bytes, cmd.targetLod );

        // free data buffer and loder
        cmd.loader->freebuf( cmd.data, cmd.bytes );
        cmd.loader.clear();

        GN_UNGUARD;
    }

    void RESFUNC_DISPOSE( RenderEngine & engine,  ResourceCommand & cmd )
    {
        GN_GUARD;

        GraphicsResourceItem * res = engine.resourceCache().id2ptr( cmd.resourceId );
        GN_ASSERT( res );

        GN_INFO(sLogger)( "Dispose %s", res->desc.name.cptr() );

        sDeleteDeviceData( *res );

        GN_UNGUARD;
    }
}}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::DrawThread::init( UInt32 maxDrawCommandBufferBytes )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( DrawThread, () );

    // initialize draw function pointers
    memset( mDrawFunctions, 0, sizeof(mDrawFunctions) );
    mDrawFunctions[DCT_SET_CONTEXT]  = &DRAWFUNC_SET_CONTEXT;
    mDrawFunctions[DCT_SET_UNIFORM]  = &DRAWFUNC_SET_UNIFORM;
    mDrawFunctions[DCT_CLEAR]        = &DRAWFUNC_CLEAR;
    mDrawFunctions[DCT_DRAW]         = &DRAWFUNC_DRAW;
    mDrawFunctions[DCT_DRAW_INDEXED] = &DRAWFUNC_DRAW_INDEXED;

    // initialize draw buffers
    for( int i = 0; i < DRAW_BUFFER_COUNT; ++i )
    {
        mDrawBuffers[i].init( maxDrawCommandBufferBytes );
    }

    // create events and semaphores
    mDoSomething = createSyncEvent( false, true ); // initial unsignaled, auto reset
    mResetRendererComplete = createSyncEvent( false, true ); // initial unsignaled, auto reset
    mDrawBufferEmpty = createSyncEvent( true, false ); // initial signaled, manual reset
    mDrawBufferNotFull  = createSemaphore( DRAW_BUFFER_COUNT-1, DRAW_BUFFER_COUNT-1, "DrawThread.DrawBufferNotFull" );
    if( !mDoSomething || !mDrawBufferEmpty || !mDrawBufferNotFull ) return failure();

    // initial other data
    mActionQuit = false;
    mActionReset = false;
    mReadingIndex = 0;
    mWritingIndex = 0;
    mDrawFence = 0;

    // create thread
    mDrawThread = createThread(
        makeDelegate(this,&DrawThread::threadProc),
        0, // no parameter
        TP_NORMAL,
        false, // not suspended
        "RenderEngine.DrawThread" );
    if( 0 == mDrawThread ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::quit()
{
    GN_GUARD;

    // delete thread
    if( mDrawThread )
    {
        // tell draw thread to exit
        mActionQuit = true;
        mDoSomething->signal();

        // wait for end of draw thread
        mDrawThread->waitForTermination();

        safeDelete( mDrawThread );
    }

    // delete sync objects
    safeDelete( mDoSomething );
    safeDelete( mResetRendererComplete );
    safeDelete( mDrawBufferEmpty );
    safeDelete( mDrawBufferNotFull );

    // delete draw buffers
    for( int i = 0; i < DRAW_BUFFER_COUNT; ++i )
    {
        mDrawBuffers[i].quit();
    }

    // delete all pending resource commands
    if( !mResourceCommands.empty() )
    {
        GN_WARN(sLogger)( "DrawThread shut down: drop unhandled resource commands." );

        ResourceCommand * i1 = mResourceCommands.head(), * i2;
        while( i1 )
        {
            i2 = i1->next;

            mResourceCommands.remove( i1 );

            ResourceCommand::free( i1 );

            i1 = i2;
        }

        GN_ASSERT( mResourceCommands.empty() );
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::DrawThread::resetRenderer(
     gfx::RendererAPI api,
     const gfx::RendererOptions & ro )
{
    mActionReset = true;
    mRendererApi = api;
    mRendererOptions = ro;
    mDoSomething->signal();

    if( !mResetRendererComplete->wait() ) return false;

    return mResetSuccess;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::waitForIdle( float time ) const
{
    if(mDrawBufferEmpty) mDrawBufferEmpty->wait( time );

    while( !mResourceCommandEmpty )
        sleepCurrentThread( 0 );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::frameBegin()
{
    // do nothing
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::frameEnd()
{
    submitDrawBuffer();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::submitDrawBuffer()
{
    // must not be called in draw thread
    GN_ASSERT( !mDrawThread->isCurrentThread() );

    // switch to next command buffer
    // note: this is the only place to modify write pointer
    mDrawBufferNotFull->wait();
    mDrawBufferMutex.lock();
    GN_ASSERT( ( ( mWritingIndex + 1 ) & (DRAW_BUFFER_COUNT-1) ) != mReadingIndex ); // make sure command buffer pool is not full.
    mWritingIndex = ( mWritingIndex + 1 ) & (DRAW_BUFFER_COUNT-1);
    mDrawBufferEmpty->unsignal(); // block any thread that waits for idle.
    mDrawBufferMutex.unlock();

    // wake up draw thread
    mDoSomething->signal();

    // reset the new write buffer
    mDrawBuffers[mWritingIndex].reset();
}

//
//
// -----------------------------------------------------------------------------
UInt32 GN::engine::RenderEngine::DrawThread::threadProc( void * )
{
    while( !mActionQuit )
    {
        // wait for something to do
        mDoSomething->wait();

        if( mActionQuit )
        {
            mActionQuit = false;
            break;
        }

        if( mActionReset )
        {
            mActionReset = false;
            mResetSuccess = doDeviceReset();
            mResetRendererComplete->signal();
        }

        handleResourceCommands();

        mDrawBufferMutex.lock();
        bool empty = mReadingIndex == mWritingIndex;
        mDrawBufferMutex.unlock();

        while( !empty )
        {
            handleDrawCommands();

            // Note that this is the only place that updates reading pointer
            mDrawBufferMutex.lock();
    		mReadingIndex = ( mReadingIndex + 1 ) & (DRAW_BUFFER_COUNT-1);
            empty = mReadingIndex == mWritingIndex;
            if( empty ) mDrawBufferEmpty->signal(); // wake up thread waiting for idle.
            mDrawBufferMutex.unlock();
            mDrawBufferNotFull->wake(); // wake up threads that are waiting for draw command submission.
        }
    }

    // delete Renderer
    GN::gfx::deleteRenderer();

    // quit thread
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::handleDrawCommands()
{
    gfx::Renderer & r = gRenderer;

    // process windows messages
    GN::win::processWindowMessages( r.getDispDesc().windowHandle, true );

    if( r.drawBegin() )
    {
        DrawBuffer & db = mDrawBuffers[mReadingIndex];

        DrawCommandHeader * command = (DrawCommandHeader*)db.buffer;

        DrawCommandHeader * end = (DrawCommandHeader*)db.next;

        while( command < end && !mActionQuit )
        {
            // resource command has priority
            handleResourceCommands();

            // update command's resource waiting list
            int count = (int)command->resourceWaitingCount;
            GN_ASSERT( count >= 0 );
            for( int i = count - 1; i >= 0; --i )
            {
                DrawCommandHeader::ResourceWaitingItem & wi = command->resourceWaitingList[i];
                GraphicsResourceItem * res = mEngine.resourceCache().id2ptr( wi.id );
                if( res->lastCompletedFence >= wi.waitForUpdate )
                {
                    // remove from waiting list
                    if( (i+1) < count )
                    {
                        memcpy(
                            &command->resourceWaitingList[i],
                            &command->resourceWaitingList[i+1],
                            count - ( i + 1 ) );
                    }
                    GN_ASSERT( count > 0 );
                    --count;
                }
            }
            GN_ASSERT( count >= 0 );
            command->resourceWaitingCount = (UInt32)count;

            if( 0 == command->resourceWaitingCount )
            {
                // all resources are ready. do it!
                GN_ASSERT( command->func );
                command->func( mEngine, command->param(), command->bytes - sizeof(DrawCommandHeader) );

                // update draw fence
                mDrawFence = command->fence;

                // next command
                command = command->next();
                GN_ASSERT( command <= end );
            }
            else
            {
                // sleep for a while, then repeat current command
                sleepCurrentThread( 0 );
            }
        }

        r.drawEnd();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::handleResourceCommands()
{
    mResourceMutex.lock();
    ResourceCommand * cmd = mResourceCommands.head();
    mResourceMutex.unlock();

    ResourceCommand * prev;

    while( cmd && !mActionQuit )
    {
        // process the resource command
        if( cmd->mustAfterThisFence <= mDrawFence )
        {
            // remove it from resource command buffer
            mResourceMutex.lock();
            prev = cmd;
            cmd = cmd->next;
            mResourceCommands.remove( prev );
            if( mResourceCommands.empty() ) mResourceCommandEmpty = true;
            mResourceMutex.unlock();

            GraphicsResourceItem * res = mEngine.resourceCache().id2ptr( prev->resourceId );

            // update resource's complete fence
            res->lastCompletedFence = prev->submittedAtThisFence;

            if( prev->noerr )
            {
                switch( prev->op )
                {
                    case GROP_COPY :
                        RESFUNC_COPY( mEngine, *prev );
                        break;

                    case GROP_DISPOSE :
                        RESFUNC_DISPOSE( mEngine, *prev );
                        break;

                    default:
                        GN_UNEXPECTED();
                        break;
                }
            }

            // the resource command is done. Free it.
            ResourceCommand::free( prev );
        }
        else
        {
            // leave it in buffer, continue search.
            mResourceMutex.lock();
            cmd = cmd->next;
            mResourceMutex.unlock();
        }
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::DrawThread::doDeviceReset()
{
    GN_GUARD;

    // (re)create renderer
    GN::gfx::Renderer * r = gfx::createRenderer( mRendererApi );
    if( NULL == r ) return false;
    if( !r->changeOptions( mRendererOptions ) ) return false;

    mDispDesc = r->getDispDesc();

    // reattach input window
    if( gInputPtr && !gInput.attachToWindow( mDispDesc.displayHandle, mDispDesc.windowHandle ) )
    {
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

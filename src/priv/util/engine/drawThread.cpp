#include "pch.h"
#include "drawThread.h"
#include "garnet/GNinput.h"
#include "garnet/GNwin.h"
#include "dump.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.DrawThread");

// *****************************************************************************
// local functions
// *****************************************************************************

#define DUMP_COMMANDS 0

#define DRAW_THREAD_ACTION(X)

//
//
// -----------------------------------------------------------------------------
static void sDumpCommandText( const GN::StrA & s )
{
    GN_INFO(sLogger)( "%s", s.cptr() );
}

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

        case GRT_VTXFMT :
            GN_ASSERT( 0 == res.vtxfmt );
            res.vtxfmt = r.createVtxFmt( res.desc.fd );
            if( 0 == res.vtxfmt ) return false;
            break;

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

        case GRT_VTXFMT :
            res.vtxfmt = 0;
            break;

        default:
            GN_UNEXPECTED();
    }
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline void sResolveResourceId(
    GN::engine::RenderEngine & eng,
    T & result,
    const GN::engine::GraphicsResource * res )
{
    if( 0 == res )
    {
        result = 0;
    }
    else
    {
        GN_ASSERT( eng.checkResource( res ) );
        result = (T)res->data;
    }
}

//
//
// -----------------------------------------------------------------------------
static void sDrawContext2RendererContext(
    GN::engine::RenderEngine & eng,
    GN::gfx::RendererContext & rc,
    const GN::engine::DrawContext & dc )
{
    using namespace GN;
    using namespace GN::gfx;
    using namespace GN::engine;

    // flags
    rc.flags.u32 = dc.flags.u32;

    // shaders
    for( int i = 0; i < NUM_SHADER_TYPES; ++i )
    {
        if( dc.flags.shaderBit( i ) ) sResolveResourceId( eng, rc.shaders[i], dc.shaders[i] );
    }

    // rsb
    if( dc.flags.rsb ) rc.rsb = dc.rsb;

    // renderTargets
    if( dc.flags.renderTargets )
    {
        // color buffers
        for( int i = 0; i < gfx::MAX_RENDER_TARGETS; ++i )
        {
            const DrawContext::RenderTargetTexture& src = dc.renderTargets.cbuffers[i];
            gfx::RenderTargetTexture              & dst = rc.renderTargets.cbuffers[i];
            sResolveResourceId( eng, dst.texture, src.texture );
            dst.subidx = src.subidx;
        }

        // z buffer
        sResolveResourceId( eng, rc.renderTargets.zbuffer.texture, dc.renderTargets.zbuffer.texture );
        rc.renderTargets.zbuffer.subidx = dc.renderTargets.zbuffer.subidx;

        rc.renderTargets.count = dc.renderTargets.count;
        rc.renderTargets.aa    = dc.renderTargets.aa;
    }

    // viewport
    if( dc.flags.viewport ) rc.viewport = dc.viewport;

    // TODO: FFP parameters

    // textures
    if( dc.flags.textures )
    {
        for( unsigned int i = 0; i < dc.numTextures; ++i )
        {
            sResolveResourceId( eng, rc.textures[i], dc.textures[i] );
        }
        rc.numTextures = dc.numTextures;
    }

    // vtxfmt
    if( dc.flags.vtxfmt )
    {
        sResolveResourceId( eng, rc.vtxfmt, dc.vtxfmt );
    }

    // vtxbufs
    if( dc.flags.vtxbufs )
    {
        for( unsigned int i = 0; i < dc.numVtxBufs; ++i )
        {
            sResolveResourceId( eng, rc.vtxbufs[i].buffer, dc.vtxbufs[i].buffer );
            rc.vtxbufs[i].offset = dc.vtxbufs[i].offset;
            rc.vtxbufs[i].stride = dc.vtxbufs[i].stride;
        }
        rc.numVtxBufs = dc.numVtxBufs;
    }

    // idxbuf
    if( dc.flags.idxbuf )
    {
        sResolveResourceId( eng, rc.idxbuf, dc.idxbuf );
    }
}

// *****************************************************************************
// draw command functions
// *****************************************************************************

// *****************************************************************************
// draw command functions
// *****************************************************************************

namespace GN { namespace engine
{
    void DRAWFUNC_SET_CONTEXT( RenderEngine & engine, const void * param, size_t bytes )
    {
        using namespace GN::gfx;

        GN_ASSERT( param && bytes == align<size_t>( sizeof(DrawContext), 4 ) );

        DrawContext * dc = (DrawContext*)param;

        static gfx::RendererContext rc;

        sDrawContext2RendererContext( engine, rc, *dc );

        gRenderer.setContext( rc );

        if( DUMP_COMMANDS )
        {
            sDumpCommandText( GN_FUNCTION );
        }
    }

    void DRAWFUNC_SET_UNIFORM( RenderEngine & engine, const void * param, size_t bytes )
    {
        using namespace gfx;

        struct ParamHeader
        {
            GraphicsResourceItem * shader;
            char                   uniname[32];
            SInt32                 unitype;
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
        GN_ASSERT( engine.resourceCache().check( header->shader ) );
        GN_ASSERT( header->shader && GRT_SHADER == header->shader->desc.type );

        if( header->shader->shader )
        {
            // set shader uniform
            header->shader->shader->setUniformByName( header->uniname, unival );
        }
        else
        {
            GN_ERROR(sLogger)( "Set uniform for null shader is not allowed." );
        }

        if( DUMP_COMMANDS )
        {
            sDumpCommandText( GN_FUNCTION );
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

        if( DUMP_COMMANDS )
        {
            sDumpCommandText( GN_FUNCTION );
        }
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

        if( DUMP_COMMANDS )
        {
            sDumpCommandText( GN_FUNCTION );
        }
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

        if( DUMP_COMMANDS )
        {
            sDumpCommandText( GN_FUNCTION );
        }
    }
}};

// *****************************************************************************
// draw command functions
// *****************************************************************************

namespace GN { namespace engine
{
    void RESFUNC_COPY( RenderEngine & engine, ResourceCommand & cmd )
    {
        GN_GUARD;

        GN_ASSERT( engine.resourceCache().check( cmd.resource ) );

        if( 0 == cmd.resource->data )
        {
            if( DUMP_COMMANDS )
            {
                sDumpCommandText( strFormat( "Create resource: %s", cmd.resource->desc.name.cptr() ) );
            }
            if( !sCreateDeviceData( *cmd.resource ) )
            {
                cmd.noerr = false;

                // free data buffer and loder
                cmd.loader->freebuf( cmd.data, cmd.bytes );
                cmd.loader.clear();
                return;
            }
        }

        if( DUMP_COMMANDS )
        {
            sDumpCommandText( strFormat( "Load resource: %s", cmd.resource->desc.name.cptr() ) );
        }

        GN_ASSERT( cmd.loader );
        cmd.noerr = cmd.loader->copy( *cmd.resource, cmd.data, cmd.bytes, cmd.targetLod );

        // free data buffer and loder
        cmd.loader->freebuf( cmd.data, cmd.bytes );
        cmd.loader.clear();

        GN_UNGUARD;
    }

    void RESFUNC_DISPOSE( RenderEngine & engine, ResourceCommand & cmd )
    {
        GN_GUARD;

        GN_ASSERT( engine.resourceCache().check( cmd.resource ) );

        sDeleteDeviceData( *cmd.resource );

        if( DUMP_COMMANDS )
        {
            sDumpCommandText( strFormat( "Dispose resource: %s", cmd.resource->desc.name.cptr() ) );
        }

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

            // handle resource command again
            handleResourceCommands();

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
        if( DUMP_COMMANDS )
        {
            sDumpCommandText( "Frame BEGIN" );
        }

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
                GN_ASSERT( mEngine.resourceCache().check( wi.resource ) );
                if( wi.resource->lastCompletedFence >= wi.waitForUpdate )
                {
                    // remove from waiting list
                    if( (i+1) < count )
                    {
                        memcpy(
                            &command->resourceWaitingList[i],
                            &command->resourceWaitingList[i+1],
                            sizeof(wi) * (count - ( i + 1 )) );
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
                DRAW_THREAD_ACTION( strFormat( "run draw command %d\n", command->fence ) );
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
                DRAW_THREAD_ACTION( strFormat( "postpone draw command %d: wait for resources...\n", command->fence ) );
                sleepCurrentThread( 0 );
            }
        }

        r.drawEnd();

        if( DUMP_COMMANDS )
        {
            sDumpCommandText( "Frame END" );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::handleResourceCommands()
{
    bool loopAgain;
    do
    {
        loopAgain = false;

        mResourceMutex.lock();
        ResourceCommand * cmd = mResourceCommands.head();
        mResourceMutex.unlock();

        ResourceCommand * prev;

        while( cmd && !mActionQuit )
        {
            GN_ASSERT( mEngine.resourceCache().check( cmd->resource ) );

            // process the resource command
            if( cmd->mustAfterThisDrawFence <= mDrawFence &&
                cmd->mustAfterThisResourceFence <= cmd->resource->lastCompletedFence )
            {
                DRAW_THREAD_ACTION( strFormat( "run resource command %d\n", cmd->submittedAtThisFence ) );

                // remove it from resource command buffer
                mResourceMutex.lock();
                prev = cmd;
                cmd = cmd->next;
                mResourceCommands.remove( prev );
                mResourceMutex.unlock();

                // update resource's complete fence
                prev->resource->lastCompletedFence = prev->submittedAtThisFence;

                if( prev->noerr )
                {
                    if( GN_ENGINE_DUMP_ENABLED )
                    {
                        dumpResourceCommand( *prev );
                    }
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

                if( mResourceCommands.empty() ) mResourceCommandEmpty = true;

                loopAgain = true;
            }
            else
            {
                // leave it in buffer, continue search.
                DRAW_THREAD_ACTION( strFormat( "skip resource command %d : wait for draw fence %d and resource fence %d\n",
                    cmd->submittedAtThisFence,
                    cmd->mustAfterThisDrawFence,
                    cmd->mustAfterThisResourceFence ) );
                mResourceMutex.lock();
                cmd = cmd->next;
                mResourceMutex.unlock();
            }
        }
    }while( loopAgain );
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

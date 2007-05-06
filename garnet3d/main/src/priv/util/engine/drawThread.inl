//
//
// -----------------------------------------------------------------------------
inline GN::engine::DrawCommandHeader *
GN::engine::RenderEngine::DrawThread::submitDrawCommand(
    DrawCommandType type, size_t parameterBytes )
{
    GN_ASSERT( 0 <= type && type < NUM_DRAW_COMMAND_TYPES );
    GN_ASSERT( mDrawFunctions[type] );

    DrawBuffer & db = mDrawBuffers[mWritingIndex];

    size_t commandBytes = sizeof(DrawCommandHeader) + parameterBytes;
    commandBytes = ( commandBytes + 3 ) & ~3; // DWORD aligned.

    if( db.rooms() < commandBytes )
    {
        static Logger * sLogger = getLogger("GN.engine.RenderEngine.DrawThread");
        GN_ERROR(sLogger)( "No enough room in draw buffer to hold more drawing commands!" );

        return 0;
    }

    DrawCommandHeader * header = (DrawCommandHeader * )db.next;

    header->bytes = (UInt32)commandBytes;
    header->fence = mEngine.fenceManager().getAndIncFence();
    header->func  = mDrawFunctions[type];
    header->resourceWaitingCount = 0;

    db.last = (UInt8*)header;
    db.next += commandBytes;

    return header;
}

//
//
// -----------------------------------------------------------------------------
template<typename T1>
inline GN::engine::DrawCommandHeader *
GN::engine::RenderEngine::DrawThread::submitDrawCommand1(
    DrawCommandType type, const T1 & a1 )
{
    DrawCommandHeader * header = submitDrawCommand( type, sizeof(T1) );

    if( header )
    {
        T1 * p1 = (T1*)header->param();
        *p1 = a1;
    }

    return header;
}

//
//
// -----------------------------------------------------------------------------
template<typename T1, typename T2>
inline GN::engine::DrawCommandHeader *
GN::engine::RenderEngine::DrawThread::submitDrawCommand2(
    DrawCommandType type, const T1 & a1, const T2 & a2 )
{
    DrawCommandHeader * header = submitDrawCommand(
        type, sizeof(T1)+sizeof(T2) );

    if( header )
    {
        T1 * p1 = (T1*)header->param();
        *p1 = a1;
        T2 * p2 = (T2*)( p1 + 1 );
        *p2 = a2;
    }

    return header;
}

//
//
// -----------------------------------------------------------------------------
template<typename T1, typename T2, typename T3, typename T4>
inline GN::engine::DrawCommandHeader *
GN::engine::RenderEngine::DrawThread::submitDrawCommand4(
    DrawCommandType type, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4 )
{
    DrawCommandHeader * header = submitDrawCommand(
        type, sizeof(T1)+sizeof(T2)+sizeof(T3)+sizeof(T4) );

    if( header )
    {
        T1 * p1 = (T1*)header->param();
        *p1 = a1;
        T2 * p2 = (T2*)( p1 + 1 );
        *p2 = a2;
        T3 * p3 = (T3*)( p2 + 1 );
        *p3 = a3;
        T4 * p4 = (T4*)( p3 + 1 );
        *p4 = a4;
    }

    return header;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::DrawThread::submitResourceCommand(
    ResourceCommand * item  )
{
    GN_ASSERT( item );
    GN_ASSERT( GROP_DISPOSE == item->op || GROP_COPY == item->op );
    mResourceMutex.lock();
    mResourceCommands.append( item );
    mDoSomething->signal();
    mResourceCommandEmpty = false;
    mResourceMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::DrawThread::submitResourceDisposingCommand(
    GraphicsResourceId id )
{
    GraphicsResourceItem * res = mEngine.resourceCache().id2ptr( id );
    GN_ASSERT( res );
    GN_ASSERT( res->id == id );
    GN_ASSERT( GRS_DISPOSED == res->state );

    ResourceCommand * cmd = ResourceCommand::alloc();
    if( 0 == cmd ) return;

    FenceId fence = mEngine.fenceManager().getAndIncFence();

    cmd->noerr                = true;
    cmd->op                   = GROP_DISPOSE;
    cmd->resourceId           = id;
    cmd->mustAfterThisFence   = res->lastReferenceFence;
    cmd->submittedAtThisFence = fence;

    res->lastSubmissionFence  = fence;

    submitResourceCommand( cmd );
}

#include "pch.h"
#include "cmdbuf.h"

static GN::Logger * sLogger = GN::getLogger("GN.base.RingBuffer");

// *****************************************************************************
// GN::CommandBuffer - Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::CommandBuffer::init( size_t bufferSize )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::CommandBuffer, () );

    GN_ASSERT( NULL == m_Buffer );

    if( bufferSize < 16 )
    {
        GN_WARN(sLogger)( "The command buffer size is adjusted to 16 bytes." );
        bufferSize = 16;
    }

    // allocate ring buffer
    m_Buffer = (uint8*)HeapMemory::alloc( bufferSize * 2 ); // allocate double sized buffer, to handle rewind issue
    if( NULL == m_Buffer ) return failure();
    m_Size = bufferSize;

    // initialize read and write counters
    //   *   used bytes = (written - readen)
    //   *   free bytes = m_Size - (written - readen)
    m_ReadenCursor  = 0;
    m_WrittenCursor = 0;
    m_ReadingToken = NULL;
    m_WritingToken = NULL;
    memoryBarrier();

    // initialize internal events
    if( !m_ConsumptionEvent.create( SyncEvent::UNSIGNALED, SyncEvent::AUTO_RESET ) ||
        !m_NotEmpty.create(SyncEvent::UNSIGNALED, SyncEvent::MANUAL_RESET) )
    {
        return failure();
    }

#if GN_COMMAND_BUFFER_BUILT_IN_FENCE
    // initialize fence array
    size_t maxFenceCount = m_Size / 16;
    m_Fences = (FenceInternal*)HeapMemory::alloc( maxFenceCount * sizeof(FenceInternal) );
    if( NULL == m_Fences ) return failure();
    for( size_t i = 0; i < maxFenceCount; ++i )
    {
        m_Fences[i].event = NULL;
        m_Fences[i].prev = (i > 0) ? &m_Fences[i-1] : NULL;
        m_Fences[i].next = ((i+1) < maxFenceCount) ? &m_Fences[i+1] : NULL;
    }
    m_NextFreeFence = m_Fences;
#endif

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::CommandBuffer::quit()
{
    GN_GUARD;

    // Invalidate tokenID, to unblock any thread that are waiting for consumption tokenID.
    // Note that tokenID value is 16 byte aligned. so -1 would never be a valid tokenID.
    m_ReadenCursor = (uint32)-1;

    // delete events
    m_NotEmpty.destroy();
    m_ConsumptionEvent.destroy();

    // TODO:
    //  - Wait for current producer and consumer
    //  - Cancel incoming producers and consumers
    //  - Clear initialize flag

    // deallocate ring buffer
    if( NULL != m_Buffer ) ::free(m_Buffer), m_Buffer = NULL;

#if GN_COMMAND_BUFFER_BUILT_IN_FENCE
    // deallocate fence array
    if( NULL != m_Fences )
    {
        size_t maxFenceCount = m_Size / 16;
        for( size_t i = 0; i < maxFenceCount; ++i )
        {
            safeDelete( m_Fences[i].event );
        }
        HeapMemory::dealloc( m_Fences );
        m_Fences = NULL;
        m_NextFreeFence = NULL;
    }
#endif

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// GN::CommandBuffer - Public Methods
// *****************************************************************************

#if GN_COMMAND_BUFFER_BUILT_IN_FENCE

//
//
// -----------------------------------------------------------------------------
GN::CommandBuffer::Fence GN::CommandBuffer::insertFence()
{
}

//
//
// -----------------------------------------------------------------------------
GN::CommandBuffer::OperationResult
GN::CommandBuffer::waitForFence( Fence fence )
{
    /*for(;;)
    {
        uint32 rf = m_ReadenCursor;
        memoryBarrier();

        if( rf >= tokenID )
        {
            return (uint32)-1 == rf ? OPERATION_CANCELLED : OPERATION_SUCCEEDED;
        }

        // TODO: use a event to save some CPU.
        ::Sleep(1);
    }*/
}

#endif

//
//
// -----------------------------------------------------------------------------
GN::CommandBuffer::OperationResult
GN::CommandBuffer::beginProduce(
    uint16      command,
    uint16      parameterSize,
    Token *     token,
    SyncEvent * optionalCompletionEvent )
{
    // align command size to 16 bytes
    size_t cmdsize = (sizeof(TokenInternal) + parameterSize + 15) & ~15;
    if( cmdsize > m_Size )
    {
        GN_ERROR(sLogger)( "Command size is too large." );
        return OPERATION_FAILED;
    }
    parameterSize = (uint16)( cmdsize - sizeof(TokenInternal) );

    m_ProducerLock.lock();

    if( NULL != m_WritingToken )
    {
        m_ProducerLock.unlock();
        return OPERATION_FAILED;
    }

    for(;;)
    {
        // cache read/write tokenID to local variable
        size_t rc = m_ReadenCursor;
        size_t wc = m_WrittenCursor;
        memoryBarrier();

        size_t usedBytes = wc - rc;
        size_t freeBytes = m_Size - usedBytes;

        if( freeBytes >= cmdsize )
        {
            m_WritingToken = (TokenInternal*)(m_Buffer + ( wc % m_Size )); // TODO: align buffer size to power of 2 to save this mod operation.);
            m_WritingToken->commandId = command;
            m_WritingToken->parameterSize = (uint16)parameterSize;
            m_WritingToken->endOffset = (uint32)(wc + cmdsize);
            m_WritingToken->completionEvent = optionalCompletionEvent;

            if( token )
            {
                token->commandID = command;
                token->parameterSize = parameterSize;
                token->pParameterBuffer = (void*)(m_WritingToken+1);
            }

            // production succeeds.
            return OPERATION_SUCCEEDED;
        }

        // There's no enough space in ring buffer, wait for consumption
        else if( !m_ConsumptionEvent.wait() )
        {
            m_ProducerLock.unlock();
            return OPERATION_FAILED;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::CommandBuffer::endProduce()
{
    // Enter production lock again, which prevents all thread except the one
    // who calls beginProduce().
    ScopeMutex<Mutex> lock( m_ProducerLock );

    if( NULL == m_WritingToken )
    {
        // This means endProduce() is called without beginProduce().
        return;
    }

    // Updating written tokenID and m_NotEmpty event should be an
    // atomic operation.
    m_DataLock.lock();
    {
        GN_ASSERT( m_WrittenCursor + m_WritingToken->parameterSize + sizeof(TokenInternal) == m_WritingToken->endOffset );
        m_WrittenCursor = m_WritingToken->endOffset;
        m_WritingToken = NULL;
        m_NotEmpty.signal();
    }
    m_DataLock.unlock();

    // Leave the production lock which is entered in beginProduce()
    m_ProducerLock.unlock();
}

//
//
// -----------------------------------------------------------------------------
GN::CommandBuffer::OperationResult
GN::CommandBuffer::beginConsume( Token * token, TimeInNanoSecond timeoutTime )
{
    if( NULL == token )
    {
        return OPERATION_FAILED;
    }

    m_ConsumerLock.lock();

    GN::CommandBuffer::OperationResult hr = OPERATION_SUCCEEDED;

    if( NULL != m_ReadingToken )
    {
        // beginConsume is called more than once without endConsume.
        hr = OPERATION_FAILED;
    }

    // wait for production of next command
    if( OPERATION_SUCCEEDED == hr )
    {
        for(;;)
        {
            // cache production fences
            uint32 wc = m_WrittenCursor;
            memoryBarrier();

            size_t usedBytes = wc - m_ReadenCursor;

            if( usedBytes < sizeof(TokenInternal) )
            {
                WaitResult waitResult = m_NotEmpty.wait( timeoutTime );

                if( WaitResult::TIMEOUT == waitResult )
                {
                    hr = OPERATION_TIMEOUT;
                    break;
                }
                else if( WaitResult::KILLED == waitResult )
                {
                    // exit signal is received
                    hr = OPERATION_CANCELLED;
                    break;
                }
                else
                {
                    GN_ASSERT( WaitResult::COMPLETED == waitResult );
                }
            }
            else
            {
                break;
            }
        }
    }

    if( OPERATION_SUCCEEDED == hr )
    {
        // get command header and parameter
        m_ReadingToken = (TokenInternal*)( m_Buffer + (m_ReadenCursor % m_Size) );

#if GN_BUILD_ENABLE_ASSERT
        // full command including all parameters should have been written to command buffer.
        uint32 wc = m_WrittenCursor;
        memoryBarrier();
        size_t cmdsize = m_ReadingToken->parameterSize + sizeof(TokenInternal);
        GN_ASSERT( wc - m_ReadenCursor >= cmdsize );
#endif

        // update output token
        token->commandID = m_ReadingToken->commandId;
        token->parameterSize = m_ReadingToken->parameterSize;
        token->pParameterBuffer = (void**)(m_ReadingToken+1);
    }

    if( OPERATION_SUCCEEDED != hr )
    {
        m_ConsumerLock.unlock();
    }

    return hr;
}

//
//
// -----------------------------------------------------------------------------
void GN::CommandBuffer::endConsume()
{
    // Enter consumer lock again here, to prevent all thread other than the
    // one that calls beginConsume()
    ScopeMutex<Mutex>lock( m_ConsumerLock );

    if( NULL == m_ReadingToken )
    {
        // endConsume() is called without beginConsume()
        return;
    }

    SyncEvent * completionEvent = m_ReadingToken->completionEvent;
    m_ReadenCursor += m_ReadingToken->parameterSize + sizeof(TokenInternal);
    m_ReadingToken = NULL;
    m_ConsumptionEvent.signal();

    // Checking written tokenID and updating m_NotEmpty event should be an
    // atomic operation.
    m_DataLock.lock();
    {
        uint32 wf = m_WrittenCursor;
        memoryBarrier();
        if( wf == m_ReadenCursor )
        {
            m_NotEmpty.unsignal();
        }
    }
    m_DataLock.unlock();

    // trigger completion event
    if( completionEvent ) completionEvent->signal();

    // Leave the critical section that is entered in beginConsume()
    m_ConsumerLock.unlock();
}

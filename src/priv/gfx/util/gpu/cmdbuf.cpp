#include "pch.h"
#include "cmdbuf.h"

static GN::Logger * sLogger = GN::getLogger("GN.base.RingBuffer");

// *****************************************************************************
// GN::CommandBuffer - Initialize and shutdown
// *****************************************************************************

/*
//
// -----------------------------------------------------------------------------
bool GN::CommandBuffer::init( size_t bufferSize )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::CommandBuffer, () );

    GN_ASSERT( NULL == m_Buffer );

    // allocate ring buffer
    m_Buffer = (UInt8*)HeapMemory::alloc( bufferSize * 2 ); // allocate double sized buffer, to handle rewind issue
    if( NULL == m_Buffer ) return failure();
    m_Size = bufferSize;

    // initialize read and write counters
    //   *   used bytes = (write - read)
    //   *   free bytes = m_Size - (write - read)
    m_ReadenFence  = 0;
    m_WrittenFence = 0;
    m_ReadToken = NULL;
    m_WriteToken = NULL;
    memoryBarrier();

    // initialize internal events
    m_ConsumptionEvent = createSyncEvent( SyncEvent::UNSIGNALED, SyncEvent::AUTO_RESET );
    m_NotEmpty = createSyncEvent(SyncEvent::UNSIGNALED, SyncEvent::MANUAL_RESET);
    m_Exit = createSyncEvent(SyncEvent::UNSIGNALED, SyncEvent::MANUAL_RESET);
    if( NULL == m_ConsumptionEvent || NULL == m_NotEmpty || NULL == m_Exit) return failure();

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

    // Invalidate fence, to unblock any thread that are waiting for consumption fence.
    // Note that fence value is 16 byte aligned. so -1 would never be a valid fence.
    m_ReadenFence = (Fence)-1;

    // TODO:
    //  - Block producers
    //  - Cancel consumers
    //  - Clear initialize flag

    // deallocate ring buffer
    if( NULL != m_Buffer ) ::free(m_Buffer), m_Buffer = NULL;

    // delete events
    safeDelete( m_NotEmpty );
    safeDelete( m_ConsumptionEvent );
    safeDelete( m_Exit );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// GN::CommandBuffer - Public Methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::CommandBuffer::OperationResult
GN::CommandBuffer::waitForConsumptionFence( Fence fence )
{
    for(;;)
    {
        Fence rf = m_ReadenFence;
        memoryBarrier();

        if( rf >= fence )
        {
            return (Fence)-1 == rf ? OPERATION_CANCELLED : OPERATION_SUCCEEDED;
        }

        // TODO: use a event to save some CPU.
        ::Sleep(1);
    }
}

//
//
// -----------------------------------------------------------------------------
GN::CommandBuffer::OperationResult
GN::CommandBuffer::beginProduce(
    UInt16               command,
    UInt16               parameterSize,
    _Out_opt_ Token *    token,
    _In_opt_ SyncEvent * optionalCompletionEvent )
{
    // align command size to 16 bytes
    size_t cmdsize = (sizeof(TokenInternal) + parameterSize + 15) & ~15;
    if( cmdsize > m_Size )
    {
        GN_ERROR(sLogger)( "Command size is too large." );
        return OPERATION_FAILED;
    }
    parameterSize = (UInt16)( cmdsize - sizeof(TokenInternal) );

    m_ProducerLock.lock();

    if( NULL != m_WriteToken )
    {
        m_ProducerLock.unlock();
        return OPERATION_FAILED;
    }

    for(;;)
    {
        // cache read/write fence to local variable
        size_t rf = m_ReadenFence;
        size_t wf = m_WrittenFence;
        memoryBarrier();

        size_t usedBytes = wf - rf;
        size_t freeBytes = m_Size - usedBytes;

        if( freeBytes >= cmdsize )
        {
            m_WriteToken = (TokenInternal*)(m_Buffer + ( wf % m_Size )); // TODO: align buffer size to power of 2 to save this mod operation.);
            m_WriteToken->commandId = command;
            m_WriteToken->parameterSize = parameterSize;
            m_WriteToken->fence = wf + cmdsize;
            m_WriteToken->completionEvent = optionalCompletionEvent;

            if( token )
            {
                token->fence = m_WriteToken->fence;
                token->commandId = command;
                token->parameterSize = parameterSize;
                token->pParameterBuffer = (void*)(m_WriteToken+1);
            }

            // production succeeds.
            return OPERATION_SUCCEEDED;
        }

        // There's no enough space in ring buffer, wait for consumption
        else if( !m_ConsumptionEvent->wait() )
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
    CritSec::Helper lock( m_ProducerLock );

    if( NULL == m_WriteToken )
    {
        // This means endProduce() is called without beginProduce().
        return;
    }

    // Updating written fence and m_NotEmpty event should be an
    // atomic operation.
    EnterCriticalSection( &m_DataLock );
    {
        GN_ASSERT( m_WrittenFence + m_WriteToken->parameterSize + sizeof(TokenInternal) == m_WriteToken->fence );
        m_WrittenFence = m_WriteToken->fence;
        m_WriteToken = NULL;
        SetEvent( m_NotEmpty );
    }
    LeaveCriticalSection( &m_DataLock );

    // Leave the production lock which is entered in beginProduce()
    LeaveCriticalSection( &m_ProducerLock );
}

//
//
// -----------------------------------------------------------------------------
GN::CommandBuffer::OperationResult
GN::CommandBuffer::beginConsume( Token * token, int waitTime )
{
    if( NULL == token )
    {
        return OPERATION_FAILED;
    }

    EnterCriticalSection( &m_ConsumerLock );

    GN::CommandBuffer::OperationResult hr = OPERATION_SUCCEEDED;

    if( NULL != m_ReadToken )
    {
        // beginConsume is called more than once without endConsume.
        hr = OPERATION_FAILED;
    }

    // wait for production of next command
    if( SUCCEEDED(hr) )
    {
        Fence wf;
        for(;;)
        {
            // cache production fences
            wf = m_WrittenFence;
            MemoryBarrier();

            size_t usedBytes = wf - m_ReadenFence;

            if( usedBytes < sizeof(TokenInternal) )
            {
                HANDLE handles[2] = { m_NotEmpty, m_Exit };
                DWORD waitResult = WaitForMultipleObjects( 2, handles, FALSE, waitTime );

                if( WAIT_TIMEOUT == waitResult )
                {
                    hr = E_PENDING;
                    break;
                }
                else if( ( WAIT_OBJECT_0 + 1 ) == waitResult ||
                         ( WAIT_ABANDONED_0 + 0 ) == waitResult ||
                         ( WAIT_ABANDONED_0 + 1 ) == waitResult )
                {
                    // exit signal is received
                    hr = OPERATION_CANCELLED;
                    break;
                }
                else if( WAIT_OBJECT_0 != waitResult )
                {
                    hr = HRESULT_FROM_WIN32( GetLastError() );
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    if( SUCCEEDED(hr) )
    {
        // get command header and parameter
        m_ReadToken = (TokenInternal*)( m_Buffer + (m_ReadenFence % m_Size) );
        size_t cmdsize = m_ReadToken->parameterSize + sizeof(TokenInternal);

#if DBG
        // full command including all parameters should have been written to command buffer.
        Fence wf = m_WrittenFence;
        MemoryBarrier();
        GN_ASSERT( wf - m_ReadenFence >= cmdsize );
#endif

        // update output token
        token->fence = m_ReadenFence + cmdsize;
        token->commandId = m_ReadToken->commandId;
        token->parameterSize = m_ReadToken->parameterSize;
        token->pParameterBuffer = (void**)(m_ReadToken+1);
    }

    if( FAILED(hr) )
    {
        LeaveCriticalSection( &m_ConsumerLock );
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
    CritSec::Helper lock( m_ConsumerLock );

    if( NULL == m_ReadToken )
    {
        // endConsume() is called without beginConsume()
        return;
    }

    m_ReadenFence += m_ReadToken->parameterSize + sizeof(TokenInternal);
    m_ReadToken = NULL;
    SetEvent( m_ConsumptionEvent );

    // Checking written fence and updating m_NotEmpty event should be an
    // atomic operation.
    EnterCriticalSection( &m_DataLock );
    {
        Fence wf = m_WrittenFence;
        MemoryBarrier();
        if( wf == m_ReadenFence )
        {
            ResetEvent( m_NotEmpty );
        }
    }
    LeaveCriticalSection( &m_DataLock );

    // Leave the critical section that is entered in beginConsume()
    LeaveCriticalSection( &m_ConsumerLock );
}*/

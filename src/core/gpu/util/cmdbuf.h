#ifndef __GN_BASE_CMDBUF_H__
#define __GN_BASE_CMDBUF_H__
// *****************************************************************************
/// \file
/// \brief   Command buffer interface
/// \author  chenli@@REDMOND (2010.8.2)
// *****************************************************************************

// Note: built-in fence is not implemented yet.
#define GN_COMMAND_BUFFER_BUILT_IN_FENCE 0

namespace GN
{
    ///
    /// Command buffer class
    ///
    class CommandBuffer : public StdClass
    {
        GN_DECLARE_STDCLASS( CommandBuffer, StdClass );

        // *********************************
        // Public types
        // *********************************

    public:

#if GN_COMMAND_BUFFER_BUILT_IN_FENCE
        typedef uint32 Fence;
        static const Fence INVALID_FENCE = 0;
#endif

        enum OperationResult
        {
            OPERATION_SUCCEEDED,    // Operation is done successfully
            OPERATION_FAILED,       // Operation is failed.
            OPERATION_TIMEDOUT,     // Operation is timed out.
            OPERATION_CANCELLED,    // Operation is cancelled.
        };

        struct Token
        {
            uint16 commandID;        ///< command ID
            uint16 parameterSize;    ///< parameter buffer size.
            void * pParameterBuffer; ///< command
        };

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CommandBuffer()          { clear(); }
        virtual ~CommandBuffer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t bufferSize );
        void quit();
    private:
        void clear()
        {
            m_Buffer = NULL;
            m_ReadenCursor = (uint32)-1;
#if GN_COMMAND_BUFFER_BUILT_IN_FENCE
            m_Fences = NULL;
#endif
        }
        //@}

        // ********************************
        // Producer
        // ********************************
    public:

#if GN_COMMAND_BUFFER_BUILT_IN_FENCE

        typedef uint32 Fence;

        static const Fence INVALID_FENCE = 0;

        /// Insert a new fence to the command buffer.
        ///
        /// Return:
        ///     Fence ID, if succeeds.
        ///     INVALID_FENCE, if failed.
        Fence insertFence();

        // Wait for commands before specific fence are all consumed
        // Return:
        //      OPERATION_SUCCEEDED, if all commands before the token are consumed.
        //      OPERATION_CANCELLED, if the command buffer is shutting down.
        //
        // Note: calling this from consumer thread would dead lock the application.
        OperationResult waitForFence( Fence fence );

#endif

        // Return:
        //      OPERATION_SUCCEEDED, if production succeeds.
        //      OPERATION_CANCELLED if command buffer is shutting down.
        //      OPERATION_FAILED for other failures, like not paired with endProduce().
        OperationResult beginProduce( uint16 command, uint16 parameterSize, Token * token, SyncEvent * optionalCompletionEvent = NULL );
        void            endProduce();

        OperationResult postCommand0( uint16 command, SyncEvent * optionalCompletionEvent = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, 0, &token, optionalCompletionEvent );
            if( OPERATION_SUCCEEDED == hr )
            {
                endProduce();
            }
            return hr;
        }

        template<typename T1>
        OperationResult postCommand1( uint16 command, const T1 & p1, SyncEvent * optionalCompletionEvent = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(p1), &token, optionalCompletionEvent );
            if( OPERATION_SUCCEEDED == hr )
            {
                uint8 * buf = (uint8*)token.pParameterBuffer;
                memcpy( buf, &p1, sizeof(p1) );
                endProduce();
            }
            return hr;
        }

        template<typename T1, typename T2>
        OperationResult postCommand2( uint16 command, const T1 & p1, const T2 & p2, SyncEvent * optionalCompletionEvent = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(p1)+sizeof(p2), &token, optionalCompletionEvent );
            if( OPERATION_SUCCEEDED == hr )
            {
                uint8 * buf = (uint8*)token.pParameterBuffer;
                memcpy( buf, &p1, sizeof(p1) ); buf += sizeof(p1);
                memcpy( buf, &p2, sizeof(p2) );
                endProduce();
            }
            return hr;
        }

        template<typename T1, typename T2, typename T3>
        OperationResult postCommand3( uint16 command, const T1 & p1, const T2 & p2, const T3 & p3, SyncEvent * optionalCompletionEvent = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(p1)+sizeof(p2)+sizeof(p3), &token, optionalCompletionEvent );
            if( OPERATION_SUCCEEDED == hr )
            {
                uint8 * buf = (uint8*)token.pParameterBuffer;
                memcpy( buf, &p1, sizeof(p1) ); buf += sizeof(p1);
                memcpy( buf, &p2, sizeof(p2) ); buf += sizeof(p2);
                memcpy( buf, &p3, sizeof(p3) );
                endProduce();
            }
            return hr;
        }

        template<typename T1, typename T2, typename T3, typename T4>
        OperationResult postCommand4( uint16 command, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, SyncEvent * optionalCompletionEvent = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(p1)+sizeof(p2)+sizeof(p3)+sizeof(p4), &token, optionalCompletionEvent );
            if( OPERATION_SUCCEEDED == hr )
            {
                uint8 * buf = (uint8*)token.pParameterBuffer;
                memcpy( buf, &p1, sizeof(p1) ); buf += sizeof(p1);
                memcpy( buf, &p2, sizeof(p2) ); buf += sizeof(p2);
                memcpy( buf, &p3, sizeof(p3) ); buf += sizeof(p3);
                memcpy( buf, &p4, sizeof(p4) );
                endProduce();
            }
            return hr;
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5>
        OperationResult postCommand5( uint16 command, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, SyncEvent * optionalCompletionEvent = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(p1)+sizeof(p2)+sizeof(p3)+sizeof(p4)+sizeof(p5), &token, optionalCompletionEvent );
            if( OPERATION_SUCCEEDED == hr )
            {
                uint8 * buf = (uint8*)token.pParameterBuffer;
                memcpy( buf, &p1, sizeof(p1) ); buf += sizeof(p1);
                memcpy( buf, &p2, sizeof(p2) ); buf += sizeof(p2);
                memcpy( buf, &p3, sizeof(p3) ); buf += sizeof(p3);
                memcpy( buf, &p4, sizeof(p4) ); buf += sizeof(p4);
                memcpy( buf, &p5, sizeof(p5) );
                endProduce();
            }
            return hr;
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
        OperationResult postCommand6( uint16 command, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6, SyncEvent * optionalCompletionEvent = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(p1)+sizeof(p2)+sizeof(p3)+sizeof(p4)+sizeof(p5)+sizeof(p6), &token, optionalCompletionEvent );
            if( OPERATION_SUCCEEDED == hr )
            {
                uint8 * buf = (uint8*)token.pParameterBuffer;
                memcpy( buf, &p1, sizeof(p1) ); buf += sizeof(p1);
                memcpy( buf, &p2, sizeof(p2) ); buf += sizeof(p2);
                memcpy( buf, &p3, sizeof(p3) ); buf += sizeof(p3);
                memcpy( buf, &p4, sizeof(p4) ); buf += sizeof(p4);
                memcpy( buf, &p5, sizeof(p5) ); buf += sizeof(p5);
                memcpy( buf, &p6, sizeof(p6) );
                endProduce();
            }
            return hr;
        }

        // ********************************
        // Consumer
        // ********************************
    public:

        // Return:
        //      OPERATION_SUCCEEDED, if succeeds.
        //      OPERATION_TIMEOUT if timed out
        //      OPERATION_CANCELLED if command buffer is shutting down.
        //      OPERATION_FAILED for other failures, like not paired with endConsume().
        OperationResult beginConsume( Token * token, TimeInNanoSecond timeoutTime = INFINITE_TIME );
        void            endConsume();


        // The event is in signaled state, as long as the buffer is not empty.
        // Please DO NOT change state of this event manually, or it'll break command
        // buffer's internal logic.
        const SyncEvent & GetNotEmptyEvent() const { return m_NotEmpty; }

        // utilities to parse command parameter buffer

        template<typename T1>
        static void parseParameter( const void * commandParameter, size_t parameterSize, T1 * p1 )
        {
            GN_UNUSED_PARAM( parameterSize );
            GN_ASSERT( parameterSize >= ( ( sizeof(T1) + 15 ) & ~15 ) );

            *p1 = *(const T1*)commandParameter;
        }

        template<typename T1, typename T2>
        static void parseParameter( const void * commandParameter, size_t parameterSize, T1 * p1, T2 * p2 )
        {
            GN_UNUSED_PARAM( parameterSize );
            GN_ASSERT( parameterSize == ( ( sizeof(T1) + sizeof(T2) + 15 ) & ~15 ) );

            const uint8 * ptr = (const uint8*)commandParameter;

            *p1 = *(const T1*)ptr; ptr += sizeof(T1);
            *p2 = *(const T2*)ptr;
        }

        template<typename T1, typename T2, typename T3>
        static void parseParameter( const void * commandParameter, size_t parameterSize, T1 * p1, T2 * p2, T3 * p3 )
        {
            GN_UNUSED_PARAM( parameterSize );
            GN_ASSERT( parameterSize == ( ( sizeof(T1) + sizeof(T2) + sizeof(T3) + 15 ) & ~15 ) );

            const uint8 * ptr = (const uint8*)commandParameter;

            *p1 = *(const T1*)ptr; ptr += sizeof(T1);
            *p2 = *(const T2*)ptr; ptr += sizeof(T2);
            *p3 = *(const T3*)ptr;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        // Command token
        struct TokenInternal
        {
            uint16      commandId;           ///< command ID ( 2 bytes )
            uint16      parameterSize;       ///< command parameter size. this header is not included.
            uint32      endOffset;           ///< Ring buffer offset in bytes of the end of the command.
            SyncEvent * completionEvent;     ///< Optional event that gets signaled when the command is consumed.
#if !GN_X64
            uint32      _padding; // pad to 16 bytes to get a good cache alignment
#endif
        };
        GN_CASSERT( 16 == sizeof(TokenInternal) );

#if GN_COMMAND_BUFFER_BUILT_IN_FENCE
        union FenceInternal
        {
            uint32          offset; //< The offset of the command in the command buffer.
            SyncEvent     * event;  //< The completion event pointer.
            FenceInternal * prev;   //< Points to the previous free fence in the free list.
            FenceInternal * next;   //< Points to the next free fence in the free list.
        };
#endif

        // ring buffer
        uint8 *          m_Buffer;
        size_t           m_Size;
        uint8 *          m_End;
        volatile uint32  m_ReadenCursor;     // Ring buffer offset pointing to the next byte that will be used for consumption.
        volatile uint32  m_WrittenCursor;    // Ring buffer offset pointing to the next byte that will be used for production.
        TokenInternal *  m_ReadingToken;     // Pointer to the current consuming token. Should be NULL outside of beginConsume() and endConsume().
        TokenInternal *  m_WritingToken;     // Pointer to the current producing token. Should be NULL outside of beginProdue() and endProduce().
        SyncEvent        m_ConsumptionEvent; // auto-reset event that is signaled whenever a command is consumed, and unsignaled when buffer is full.
        SyncEvent        m_NotEmpty;         // manual-reset event that remains signaled when command buffer is not empty.

#if GN_COMMAND_BUFFER_BUILT_IN_FENCE
        // Fence
        FenceInternal *  m_Fences;
        FenceInternal *  m_NextFreeFence;
#endif

        Mutex m_ProducerLock; // To serialize multiple producers.
        Mutex m_ConsumerLock; // To serialize multiple consumers.
        Mutex m_DataLock;     // To protect internal shared data.

        // ********************************
        // private functions
        // ********************************
    private:

        // Internal version of produce function that support producing token command
        OperationResult beginProduceInternal( uint16 command, uint16 parameterSize, Token * token = NULL, SyncEvent * optionalCompletionEvent = NULL );
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CMDBUF_H__

#ifndef __GN_BASE_CMDBUF_H__
#define __GN_BASE_CMDBUF_H__
// *****************************************************************************
/// \file
/// \brief   Command buffer interface
/// \author  chenli@@REDMOND (2010.8.2)
// *****************************************************************************

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

        enum OperationResult
        {
            OPERATION_SUCCEEDED,    // Operation is done successfully
            OPERATION_FAILED,       // Operation is failed.
            OPERATION_TIMEOUT,      // Operation is timeout.
            OPERATION_CANCELLED,    // Operation is cancelled.
        };

        typedef UInt32 Fence;

        struct Token
        {
            Fence  fence;            ///< Token fence
            UInt16 commandId;        ///< command ID ( 2 bytes )
            UInt16 parameterSize;    ///< parameter buffer size.
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
            m_ReadenFence = (Fence)-1;
            m_ConsumptionEvent = NULL;
            m_NotEmpty = NULL;
            m_Exit = NULL;
        }
        //@}

        // ********************************
        // Control API
        // ********************************
    public:

        // Wait for all commands before specific fence are consumed
        // Return:
        //      OPERATION_SUCCEEDED, if the command is consumed.
        //      OPERATION_CANCELLED, if the command buffer is shutting down.
        OperationResult waitForConsumptionFence( Fence fence );

        // ********************************
        // Producer
        // ********************************
    public:

        // Return:
        //      OPERATION_SUCCEEDED, if production succeeds.
        //      OPERATION_CANCELLED if command buffer is shutting down.
        //      OPERATION_FAILED for other failures, like not paired with endProduce().
        OperationResult beginProduce( UInt16 command, UInt16 parameterSize, _Out_opt_ Token * token = NULL, _In_opt_ SyncEvent * optionalCompletionEvent = NULL );
        void            endProduce();

        OperationResult postCommand( UInt16 command, _Out_opt_ Fence * fence = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, 0, &token );
            if( OPERATION_SUCCEEDED == hr )
            {
                if( fence ) *fence = token.fence;
                endProduce();
            }
            return hr;
        }

        template<typename T1>
        OperationResult postCommand( UInt16 command, const T1 * p1, _Out_opt_ Fence * fence = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(*p1), &token );
            if( OPERATION_SUCCEEDED == hr )
            {
                UInt8 * buf = (UInt8*)token.pParameterBuffer;
                memcpy( buf, p1, sizeof(*p1) );
                if( fence ) *fence = token.fence;
                endProduce();
            }
            return hr;
        }

        template<typename T1, typename T2>
        OperationResult postCommand( UInt16 command, const T1 * p1, const T2 * p2, _Out_opt_ Fence * fence = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(*p1)+sizeof(*p2), &token );
            if( OPERATION_SUCCEEDED == hr )
            {
                UInt8 * buf = (UInt8*)token.pParameterBuffer;
                memcpy( buf, p1, sizeof(*p1) ); buf += sizeof(*p1);
                memcpy( buf, p2, sizeof(*p2) );
                if( fence ) *fence = token.fence;
                endProduce();
            }
            return hr;
        }

        template<typename T1, typename T2, typename T3>
        OperationResult postCommand( UInt16 command, const T1 * p1, const T2 * p2, const T3 * p3, _Out_opt_ Fence * fence = NULL )
        {
            Token token;
            OperationResult hr = beginProduce( command, sizeof(*p1)+sizeof(*p2)+sizeof(*p3), &token );
            if( OPERATION_SUCCEEDED == hr )
            {
                UInt8 * buf = (UInt8*)token.pParameterBuffer;
                memcpy( buf, p1, sizeof(*p1) ); buf += sizeof(*p1);
                memcpy( buf, p2, sizeof(*p2) ); buf += sizeof(*p2);
                memcpy( buf, p3, sizeof(*p3) );
                if( fence ) *fence = token.fence;
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
        OperationResult beginConsume( _Out_ Token * token, int waitTime );
        void            endConsume();


        // The event is in signaled state, as long as the buffer is not empty.
        // Please DO NOT change state of this event manually, or it'll break command
        // buffer's internal logic.
        SyncEvent * GetNotEmptyEvent() const { return m_NotEmpty; }

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

            const UInt8 * ptr = (const UInt8*)commandParameter;

            *p1 = *(const T1*)ptr; ptr += sizeof(T1);
            *p2 = *(const T2*)ptr;
        }

        template<typename T1, typename T2, typename T3>
        static void parseParameter( const void * commandParameter, size_t parameterSize, T1 * p1, T2 * p2, T3 * p3 )
        {
            GN_UNUSED_PARAM( parameterSize );
            GN_ASSERT( parameterSize == ( ( sizeof(T1) + sizeof(T2) + sizeof(T3) + 15 ) & ~15 ) );

            const UInt8 * ptr = (const UInt8*)commandParameter;

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
            UInt16      commandId;      ///< command ID ( 2 bytes )
            UInt16      parameterSize;  ///< command parameter size. this header is not included.
            Fence       fence;
            SyncEvent * completionEvent;
#if !GN_X64
            UInt32      _padding; // pad to 16 bytes to get a good cache alignment
#endif
        };
        GN_CASSERT( 16 == sizeof(TokenInternal) );

        // ring buffer
        UInt8 *          m_Buffer;
        size_t           m_Size;
        UInt8 *          m_End;
        volatile Fence   m_ReadenFence;
        volatile Fence   m_WrittenFence;
        TokenInternal *  m_ReadToken;
        TokenInternal *  m_WriteToken;
        SyncEvent *      m_ConsumptionEvent; // auto-reset event that is signaled whenever a command is consumed, and unsignaled when buffer is full.
        SyncEvent *      m_NotEmpty;         // manual-reset event that remains signaled when command buffer is not empty.
        SyncEvent *      m_Exit;             // manual-reset event that is signaled when command buffer is shutting down.

        Mutex m_ProducerLock; // To serialize multiple producers.
        Mutex m_ConsumerLock; // To serialize multiple consumers.
        Mutex m_DataLock;     // To protect internal shared data.

        // ********************************
        // private functions
        // ********************************
    private:
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CMDBUF_H__

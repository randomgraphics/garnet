#ifndef __GN_ENGINE2_DRAWTHREAD_H__
#define __GN_ENGINE2_DRAWTHREAD_H__
// *****************************************************************************
/// \file
/// \brief   async rendering thread interface
/// \author  chen@@CHENLI-HOMEPC (2007.4.27)
// *****************************************************************************

#include "fenceManager.h"
#include "resourceCache.h"
#include "resourceCommandBuffer.h"
#include "drawCommand.h"

namespace GN { namespace engine2
{
    class RenderEngine;

    ///
    /// Asyncronized rendering thread
    ///
    class RenderEngine::DrawThread : public StdClass
    {
        GN_DECLARE_STDCLASS( RenderEngine::DrawThread, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        DrawThread( RenderEngine & engine ) : mEngine(engine) { clear(); }
        virtual ~DrawThread() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t drawCommandBufferBytes );
        void quit();
    private:
        void clear()
        {
            mAction = 0;
            mResetRendererComplete = 0;
            mResourceCommandEmpty = true;
            mDrawBufferEmpty = 0;
            mDrawBufferNotFull = 0;
            mDrawThread = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{
        bool resetGraphicsSystem( const gfx::GraphicsSystemCreationParameter & );
        const gfx::GraphicsSystemCreationParameter & getGraphicsSystemCreationParameter() const { return mGraphicsSystemCreationParameter; }
        const gfx::GraphicsSystemDesc & getGraphicsSystemDesc() const { return mGraphicsSystemDesc; }
        //@}

        //@{

        ///
        /// check if current thread is draw thread
        ///
        bool isDrawThread() const { return mDrawThread->isCurrentThread(); }

        ///
        /// wait for draw thread idle: all submitted draw commands are executed
        ///
        void waitForIdle( float time = INFINITE_TIME );

        ///
        /// wait until specific resource is processed by both resource and draw thread
        ///
        void waitForResource( GraphicsResourceItem * item );

        //@}

        //@{

        ///
        /// do present: submit current draw buffer
        ///
        void present();

        // alloc new draw command, fill the header, but leave parameter array uninitialized.
        DrawCommandHeader * submitDrawCommand( DrawCommandType type, size_t parameterBytes );

        template<typename T1>
        DrawCommandHeader * submitDrawCommand1( DrawCommandType type, const T1 & a1 );

        template<typename T1, typename T2>
        DrawCommandHeader * submitDrawCommand2( DrawCommandType type, const T1 & a1, const T2 & a2 );

        template<typename T1, typename T2, typename T3>
        DrawCommandHeader * submitDrawCommand3( DrawCommandType type, const T1 & a1, const T2 & a2, const T3 & a3 );

        template<typename T1, typename T2, typename T3, typename T4>
        DrawCommandHeader * submitDrawCommand4( DrawCommandType type, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4 );

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
        DrawCommandHeader * submitDrawCommand6( DrawCommandType type, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4, const T5 & a5, const T6 & a6 );

        //@}

        /// resource requests. Might be called anytime, anywhere.
        ///
        //@{

        inline void submitResourceCommand( ResourceCommand * item );

        inline void submitResourceDisposingCommand( GraphicsResourceItem * item );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct FrameProfiler
        {
            ProfileTimer & timer;
            bool           start;

            FrameProfiler()
                : timer( ProfilerManager::sGetGlobalInstance().getTimer("RenderEngine_DrawThread_FrameTime") )
                , start( 0 )
            {
            }

            ~FrameProfiler()
            {
                if( start ) timer.stop();
            }

            void nextFrame()
            {
                if( start ) timer.stop();
                timer.start();
                start = true;
            }
        };

        struct DrawBuffer
        {
            UInt8 * buffer;
            UInt8 * last; ///< point to last command in buffer. NULL means buffer empty.
            UInt8 * next; ///< point to the next empty draw command in the buffer.
            UInt8 * end;  ///< point to the end of the buffer.

            DrawBuffer() : buffer(0), last(0), next(0), end(0) {}

            ~DrawBuffer() { delete[] buffer; }

            void init( size_t bytes )
            {
                GN_ASSERT(0 == buffer);
                buffer = new UInt8[bytes];
                last = 0;
                next = buffer;
                end = buffer + bytes;
            }

            void quit() { safeDelete( buffer ); }

            void reset() { last = 0; next = buffer; } ///< reset to empty buffer

            bool empty() const { return next == buffer; }

            size_t rooms() const { GN_ASSERT( next <= end ); return end - next; }
        };

        enum
        {
            QUIT_ACTION  = 0,
            RESET_ACTION,
            DRAW_ACTION,
            RESOURCE_ACTION,
            NUM_ACTIONS,

            DRAW_BUFFER_COUNT = 2, // must be 2^N, to avoid % operation.
        };

        RenderEngine & mEngine;

        // main thread action signal
        SyncEventGroup * mAction;

        // data to handle renderer device
        gfx::GraphicsSystemCreationParameter mGraphicsSystemCreationParameter;
        gfx::GraphicsSystemCreationParameter mNewGraphicsSystemCreationParameter;
        gfx::GraphicsSystemDesc              mGraphicsSystemDesc;
        gfx::GraphicsSystemCreator           mGraphicsSystemCreater;
        volatile bool                        mResetSuccess;
        SyncEvent *                          mResetRendererComplete;

        // data to handle resource commands
        DoubleLinkedList<ResourceCommand> mResourceCommands;
        Mutex                             mResourceMutex;
        volatile bool                     mResourceCommandEmpty;
        FenceId                           mCompletedResourceFence;

        // data to handle draw commands
        DrawFunction     mDrawFunctions[NUM_DRAW_COMMAND_TYPES];
        DrawBuffer       mDrawBuffers[DRAW_BUFFER_COUNT];
        volatile SInt32  mReadingIndex;
        volatile SInt32  mWritingIndex;
        FenceId          mCompletedDrawFence;   // means that draws before or equal this fence are done.
        Mutex            mDrawBufferMutex;
        SyncEvent      * mDrawBufferEmpty;
        Semaphore      * mDrawBufferNotFull;
        //Semaphore     * mDrawBufferNotEmpty;
        FrameProfiler    mFrameProfiler;

        Thread         * mDrawThread;

        // ********************************
        // private functions
        // ********************************
    private:

        void submitDrawBuffer(); // called by any threads other than draw thead.

        // methods runs in draw thread
        UInt32 threadProc( void * );
        void   handleDrawCommands();
        void   handleResourceCommands();
        bool   doGraphicsSystemReset();
    };
}}

#include "drawThread.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_DRAWTHREAD_H__

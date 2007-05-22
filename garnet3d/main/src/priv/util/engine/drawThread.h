#ifndef __GN_ENGINE_DRAWTHREAD_H__
#define __GN_ENGINE_DRAWTHREAD_H__
// *****************************************************************************
//! \file    engine/drawThread.h
//! \brief   async rendering thread interface
//! \author  chen@@CHENLI-HOMEPC (2007.4.27)
// *****************************************************************************

#include "resourceCache.h"
#include "resourceCommandBuffer.h"
#include "drawCommand.h"
#include "fenceManager.h"

namespace GN { namespace engine
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
        bool init( UInt32 maxDrawCommandCount );
        void quit();
    private:
        void clear()
        {
            mDoSomething = 0;
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
        bool resetRenderer( gfx::RendererAPI, const gfx::RendererOptions & );
        gfx::RendererAPI getRendererApi() const { return mRendererApi; }
        const gfx::RendererOptions & getRendererOptions() const { return mRendererOptions; }
        const gfx::DispDesc & getDispDesc() const { return mDispDesc; }
        //@}

        //@{

        ///
        /// wait for draw thread idle: all submitted draw commands are executed
        ///
        void waitForIdle( float time = INFINITE_TIME ) const;

        ///
        /// get current draw fence
        ///
        FenceId getCurrentDrawFence() const { return mDrawFence; }

        //@}

        //@{

        ///
        /// start of a frame
        ///
        void frameBegin();

        ///
        /// end of a frame: submit current draw buffer
        ///
        void frameEnd();

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

        struct DrawBuffer
        {
            UInt8 * buffer;
            UInt8 * last; ///< point to last command in buffer. NULL means buffer empty.
            UInt8 * next; ///< point to the next empty draw command in the buffer.
            UInt8 * end;  ///< point to the end of the buffer.

            DrawBuffer() : buffer(0), last(0), next(0), end(0) {}

            ~DrawBuffer() { delete[] buffer; }

            void init( UInt32 bytes )
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
            DRAW_BUFFER_COUNT = 2, // must be 2^N, to avoid % operation.
        };

        RenderEngine & mEngine;

        // actions
        volatile bool mActionQuit;
        volatile bool mActionReset;
        SyncEvent   * mDoSomething;

        // data to handle renderer device
        volatile gfx::RendererAPI     mRendererApi;
        gfx::RendererOptions          mRendererOptions;
        gfx::DispDesc                 mDispDesc;
        volatile bool                 mResetSuccess;
        SyncEvent *                   mResetRendererComplete;

        // data to handle resource commands
        DoubleLinkedList<ResourceCommand> mResourceCommands;
        Mutex                             mResourceMutex;
        volatile bool                     mResourceCommandEmpty;

        // data to handle draw commands
        DrawFunction    mDrawFunctions[NUM_DRAW_COMMAND_TYPES];
        DrawBuffer      mDrawBuffers[DRAW_BUFFER_COUNT];
        volatile SInt32 mReadingIndex;
        volatile SInt32 mWritingIndex;
        FenceId         mDrawFence;   // means that draws before or equal this fence are done.
        Mutex           mDrawBufferMutex;
        SyncEvent     * mDrawBufferEmpty;
        Semaphore     * mDrawBufferNotFull;
        //Semaphore     * mDrawBufferNotEmpty;

        Thread        * mDrawThread;

        // ********************************
        // private functions
        // ********************************
    private:

        void submitDrawBuffer(); // called by any threads other than draw thead.

        // methods runs in draw thread
        UInt32 threadProc( void * );
        void   handleDrawCommands();
        void   handleResourceCommands();
        bool   doDeviceReset();
    };
}}

#include "drawThread.inl"

// *****************************************************************************
//                           End of drawThread.h
// *****************************************************************************
#endif // __GN_ENGINE_DRAWTHREAD_H__

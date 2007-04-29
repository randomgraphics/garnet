#ifndef __GN_ENGINE_DRAWTHREAD_H__
#define __GN_ENGINE_DRAWTHREAD_H__
// *****************************************************************************
//! \file    engine/drawThread.h
//! \brief   async rendering thread interface
//! \author  chen@@CHENLI-HOMEPC (2007.4.27)
// *****************************************************************************

#include "resourceCommandBuffer.h"
#include "resourceCache.h"

namespace GN { namespace engine
{
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
        DrawThread()          { clear(); }
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

        ///
        /// wait for draw thread idle: all submitted draw commands are executed
        ///
        void waitForIdle( float time ) const { if(mDrawBufferEmpty) mDrawBufferEmpty->wait( time ); }

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

        ///
        /// alloc new draw command in draw buffer
        ///
        inline DrawCommand & newDrawCommand();

        //@}

        /// resource requests. Might be called anytime, anywhere.
        ///
        //@{

        inline void submitResourceCommand( const GraphicsResourceCommand & );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct DrawBuffer
        {
            UInt8 * buffer;
            UInt8 * next; ///< pointer to the next empty draw command in the buffer.
            UInt8 * end;  ///< pointer to the end of the buffer.

            DrawBuffer() : buffer(0), next(0), end(0) {}

            ~DrawBuffer() { delete[] buffer; }

            void init( UInt32 bytes ) { GN_ASSERT(0 == buffer); buffer = new UInt8[bytes]; next = buffer; end = buffer + bytes; }
            void quit() { safeDelete( buffer ); }

            void reset() { next = buffer; } ///< reset to empty buffer

            bool empty() const { return next == buffer; }

            size_t rooms() const { GN_ASSERT( next <= end ); return end - next; }
        };

        enum
        {
            DRAW_BUFFER_COUNT = 2, // must be 2^N, to avoid % operation.
        };

        // actions
        volatile bool mQuitDrawThread;
        volatile bool mResetRenderer;
        SyncEvent   * mDoSomething;

        // data to handle resource commands
        ResourceCommandBuffer mResourceCommands;

        // data to handle draw commands
        DrawBuffer      mDrawBuffers[DRAW_BUFFER_COUNT];
        volatile SInt32 mReadingIndex;
        volatile SInt32 mWritingIndex;
        FenceId         mDrawFence; // means that draws before or equal this fence is done.
        Mutex           mDrawBufferMutex;
        SyncEvent     * mDrawBufferEmpty;
        Semaphore     * mDrawBufferNotFull;
        //Semaphore     * mDrawBufferNotEmpty;

        Thread        * mDrawThread;

        // ********************************
        // private functions
        // ********************************
    private:

        bool drawBufferEmpty() const { return atomGet32(&mReadingIndex) == atomGet32(&mWritingIndex); }

        void   submitDrawBuffer(); // called by any threads other than draw thead.

        // methods runs in thread thread
        UInt32 threadProc( void * );
        void   handleDrawCommands();
        void   handleResourceCommands();
        void   doDraw( const DrawCommand & );
    };
}}

#include "drawThread.inl"

// *****************************************************************************
//                           End of drawThread.h
// *****************************************************************************
#endif // __GN_ENGINE_DRAWTHREAD_H__

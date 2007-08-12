#ifndef __GN_ENGINE2_RESOURCETHREAD_H__
#define __GN_ENGINE2_RESOURCETHREAD_H__
// *****************************************************************************
/// \file
/// \brief   Async resource loading threads
/// \author  chen@@CHENLI-HOMEPC (2007.4.27)
// *****************************************************************************

#include "fenceManager.h"
#include "resourceCache.h"
#include "resourceCommandBuffer.h"

namespace GN { namespace engine
{
    class RenderEngine;

    ///
    /// Asyncronized resource loader.
    ///
    class RenderEngine::ResourceThread : public StdClass
    {
        GN_DECLARE_STDCLASS( ResourceThread, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        ResourceThread( RenderEngine & engine ) : mEngine(engine) { clear(); }
        virtual ~ResourceThread() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        /// asyncronized commands
        // ********************************
    public:

        //@{

        void waitForIdle();

        inline void submitResourceLoadCommand( GraphicsResourceItem * item );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct SubThread
        {
            ResourceCommandBuffer commands;
            Thread *              thread;

            SubThread() : thread(0) {}

            ~SubThread() { quit(); }

            bool init( const ThreadProcedure & proc, const char * name );
            void quit();
        };

        RenderEngine & mEngine;

        SubThread mLoader;       // IO tread
        SubThread mDecompressor; // decompress thread

        SpinLoop  mMutex;

        // ********************************
        // private functions
        // ********************************
    private:

        inline void submitResourceCommand( ResourceCommand * item );

        // thread procedures
        UInt32 load( void * );
        UInt32 decompress( void * );
    };
}}

#include "resourceThread.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_RESOURCETHREAD_H__

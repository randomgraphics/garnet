#ifndef __GN_ENGINE_RESOURCETHREAD_H__
#define __GN_ENGINE_RESOURCETHREAD_H__
// *****************************************************************************
//! \file    engine/resourceThread.h
//! \brief   Async resource loading threads
//! \author  chen@@CHENLI-HOMEPC (2007.4.27)
// *****************************************************************************

#include "resourceCommandBuffer.h"

namespace GN { namespace engine
{
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
        ResourceThread()          { clear(); }
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

        void submitResourceCommand( const GraphicsResourceCommand & );

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

            bool init( const ThreadProcedure & proc );
            void quit();
        };

        SubThread mLoador;       // do IO
        SubThread mDecompressor; // do decompress
        SubThread mPopulator;    // do copy

        // ********************************
        // private functions
        // ********************************
    private:

        // thread procedures
        UInt32 load( void * );
        UInt32 decompress( void * );
        UInt32 populate( void * );

    };
}}

#include "resourceThread.inl"

// *****************************************************************************
//                           End of resourceThread.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCETHREAD_H__

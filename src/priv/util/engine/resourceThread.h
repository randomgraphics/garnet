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

        ResourceCommandBuffer mLoadList;
        ResourceCommandBuffer mDecompressList;
        ResourceCommandBuffer mCopyList;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of resourceThread.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCETHREAD_H__

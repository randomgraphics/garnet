#ifndef __GN_ENGINE_DRAWTHREAD_H__
#define __GN_ENGINE_DRAWTHREAD_H__
// *****************************************************************************
//! \file    engine/drawThread.h
//! \brief   async rendering thread interface
//! \author  chen@@CHENLI-HOMEPC (2007.4.27)
// *****************************************************************************

#include "resourceCache.h"

namespace GN { namespace engine
{
    enum
    {
        //@{

        DRAW_BUFFER_BYTES = 4 * 4096 * 4096, // 4M
        DRAW_BUFFER_COUNT = 2,

        //@}
    };

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

        void dispose( const GraphicsResourceItem * disposeList );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        UInt8 * mDrawBuffers[DRAW_BUFFER_COUNT];

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of drawThread.h
// *****************************************************************************
#endif // __GN_ENGINE_DRAWTHREAD_H__

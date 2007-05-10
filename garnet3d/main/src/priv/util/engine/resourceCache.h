#ifndef __GN_ENGINE_RESOURCECACHE_H__
#define __GN_ENGINE_RESOURCECACHE_H__
// *****************************************************************************
//! \file    engine/resourceCache.h
//! \brief   Graphics resource cache
//! \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

#include "resourceItem.h"

namespace GN { namespace engine
{
    class RenderEngine;

    ///
    /// Graphics resource reusable cache. This class is <b>_NOT_</b> thread safe, to ensure maxinum performance.
    ///
    class RenderEngine::GraphicsResourceCache : public StdClass
    {
        GN_DECLARE_STDCLASS( GraphicsResourceCache, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        GraphicsResourceCache( RenderEngine & engine ) : mEngine(engine) { clear(); }
        virtual ~GraphicsResourceCache() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear()
        {
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// these methods, except id2ptr(), are not thread safe, thus must be called in serialzed way.
        ///
        //@{
        GraphicsResourceItem * alloc( const GraphicsResourceDesc & );
        void                   free( GraphicsResourceItem * );
        bool                   check( GraphicsResourceItem * ) const;
        GraphicsResourceItem * first() const;
        GraphicsResourceItem * next( GraphicsResourceItem * ) const;
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private functions
        // ********************************
    private:

        typedef HandleManager<GraphicsResourceItem*,UInt32> ResourceHandleManager;

        RenderEngine        & mEngine;
        ResourceHandleManager mResources;
        mutable SpinLoop      mResourceMutex;
    };
}}

#include "resourceCache.inl"

// *****************************************************************************
//                           End of resourceCache.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCECACHE_H__

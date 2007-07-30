#ifndef __GN_ENGINE2_RESOURCECACHE_H__
#define __GN_ENGINE2_RESOURCECACHE_H__
// *****************************************************************************
/// \file
/// \brief   Graphics resource cache
/// \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

#include "resourceItem.h"

namespace GN { namespace engine2
{
    class RenderEngine;

    ///
    /// Graphics resource reusable cache. This class is <b>_NOT_</b> thread safe, to ensure maxinum performance.
    ///
    class RenderEngine::ResourceCache : public StdClass
    {
        GN_DECLARE_STDCLASS( ResourceCache, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        ResourceCache( RenderEngine & engine ) : mEngine(engine) { clear(); }
        virtual ~ResourceCache() { quit(); }
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
        GraphicsResourceItem * createResource( const GraphicsResourceDesc & );
        void                   deleteResource( GraphicsResourceItem * );
        bool                   checkResource( const GraphicsResource * ) const;
        bool                   checkResource( const GraphicsResource *, GraphicsResourceType mustBeThisType ) const;
        bool                   checkResource( const GraphicsResourceItem * ) const;
        GraphicsResourceItem * firstResource() const;
        GraphicsResourceItem * nextResource( const GraphicsResourceItem * ) const;
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
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_RESOURCECACHE_H__

#ifndef __GN_ENGINE_RESOURCELRU_H__
#define __GN_ENGINE_RESOURCELRU_H__
// *****************************************************************************
//! \file    engine/resourceLRU.h
//! \brief   resource LRU manager
//! \author  chen@@CHENLI-HP (2007.5.2)
// *****************************************************************************

#include "resourceItem.h"

namespace GN { namespace engine
{
    class RenderEngine;

    ///
    /// resoure LRU table
    ///
    class RenderEngine::ResourceLRU : public StdClass
    {
        GN_DECLARE_STDCLASS( ResourceLRU, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        ResourceLRU( RenderEngine & engine ) : mEngine(engine) { clear(); }
        virtual ~ResourceLRU() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t maxTexBytes, size_t maxMeshBytes );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{
        void insert( GraphicsResourceItem * );
        void remove( GraphicsResourceItem * );
        void realize( GraphicsResourceItem *, bool * needReload ); 
        void dispose( GraphicsResourceItem * );
        void disposeAll();
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        RenderEngine & mEngine;

        typedef DoubleLinkedList<GraphicsResourceItem> ResourceLRUList;

        ResourceLRUList mLRUList;

        // size management
        size_t mMaxTexBytes;
        size_t mMaxMeshBytes;
        size_t mRealizedTexBytes;
        size_t mRealizedMeshBytes;

        // ********************************
        // private functions
        // ********************************
    private:

        inline void markAsRecentlyUsed( GraphicsResourceItem * item );
    };
}}
// *****************************************************************************
//                           End of resourceLRU.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCELRU_H__

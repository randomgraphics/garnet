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
        bool init( UInt32 maxTexBytes, UInt32 maxMeshBytes );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        void insert( GraphicsResourceId );
        void remove( GraphicsResourceId );
        void realize( GraphicsResourceId, bool * needReload ); 
        void dispose( GraphicsResourceId );
        void disposeAll();

        // ********************************
        // private variables
        // ********************************
    private:

        RenderEngine & mEngine;

        typedef DoubleLinkedList<GraphicsResourceItem> ResourceLRUList;

        ResourceLRUList mLRUList;

        // size management
        UInt32 mMaxTexBytes;
        UInt32 mMaxMeshBytes;
        UInt32 mRealizedTexBytes;
        UInt32 mRealizedMeshBytes;

        // ********************************
        // private functions
        // ********************************
    private:

        void markAsRecentlyUsed( GraphicsResourceItem * item );
    };
}}
// *****************************************************************************
//                           End of resourceLRU.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCELRU_H__

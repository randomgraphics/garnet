#ifndef __GN_ENGINE2_RESOURCELRU_H__
#define __GN_ENGINE2_RESOURCELRU_H__
// *****************************************************************************
/// \file
/// \brief   resource LRU manager
/// \author  chen@@CHENLI-HP (2007.5.2)
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
        bool init( size_t capacity );
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
        void realize( GraphicsResourceItem * );
        void dispose( GraphicsResourceItem * );
        //@}

    private:

        // ********************************
        // private variables
        // ********************************
    private:

        RenderEngine & mEngine;

        typedef DoubleLinkedList<GraphicsResourceItem> ResourceLRUList;

        ResourceLRUList mLRUList;

        // size management
        size_t mCapacity;
        size_t mRealizedBytes;

        // ********************************
        // private functions
        // ********************************
    private:

        inline void markAsRecentlyUsed( GraphicsResourceItem * item );
    };
}}
// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_RESOURCELRU_H__

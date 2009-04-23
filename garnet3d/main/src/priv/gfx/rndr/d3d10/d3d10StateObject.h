#ifndef __GN_GFXD3D10_D3D10STATEOBJECT_H__
#define __GN_GFXD3D10_D3D10STATEOBJECT_H__
// *****************************************************************************
/// \file
/// \brief   D3D10 state object cache
/// \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

#include "d3d10Renderer.h"

namespace GN { namespace gfx
{
    ///
    /// general state template (should never be compiled)
    template<class DESC>
    struct D3D10StateObjectCreator
    {
        GN_CASSERT( 0 == sizeof(DESC) );
    };

    ///
    /// rasterize state template
    ///
    template<>
    struct D3D10StateObjectCreator<D3D10_RASTERIZER_DESC>
    {
        static ID3D10RasterizerState *
        create( ID3D10Device & dev, const D3D10_RASTERIZER_DESC & desc )
        {
            ID3D10RasterizerState  * obj;
            GN_DX10_CHECK_RV( dev.CreateRasterizerState( &desc, &obj ), NULL );
            return obj;
        }

        static inline size_t
        hash( const D3D10_RASTERIZER_DESC & )
        {
            GN_UNIMPL_WARNING();
            return 0;
        }

        ///
        /// rasterize state equality check
        ///
        static inline bool equal(
            const D3D10_RASTERIZER_DESC & a,
            const D3D10_RASTERIZER_DESC & b )
        {
            return 0 == ::memcmp( &a, &b, sizeof(a) );
        }
    };

    ///
    /// blend state template
    ///
    template<>
    struct D3D10StateObjectCreator<D3D10_BLEND_DESC>
    {
        static ID3D10BlendState *
        create( ID3D10Device & dev, const D3D10_BLEND_DESC & desc )
        {
            ID3D10BlendState * obj;
            GN_DX10_CHECK_RV( dev.CreateBlendState( &desc, &obj ), NULL );
            return obj;
        }

        static inline size_t
        hash( const D3D10_BLEND_DESC & )
        {
            GN_UNIMPL_WARNING();
            return 0;
        }

        ///
        /// blend state equality check
        ///
        static inline bool equal(
            const D3D10_BLEND_DESC & a,
            const D3D10_BLEND_DESC & b )
        {
            return 0 == memcmp( &a, &b, sizeof(a) );
        }
    };

    ///
    /// depth stencil state template
    ///
    template<>
    struct D3D10StateObjectCreator<D3D10_DEPTH_STENCIL_DESC>
    {
        static ID3D10DepthStencilState *
        create( ID3D10Device & dev, const D3D10_DEPTH_STENCIL_DESC & desc )
        {
            ID3D10DepthStencilState * obj;
            GN_DX10_CHECK_RV( dev.CreateDepthStencilState( &desc, &obj ), NULL );
            return obj;
        }

        static inline size_t
        hash( const D3D10_DEPTH_STENCIL_DESC & )
        {
            GN_UNIMPL_WARNING();
            return 0;
        }

        ///
        /// depth stencil state equality check
        ///
        static inline bool equal(
            const D3D10_DEPTH_STENCIL_DESC & a,
            const D3D10_DEPTH_STENCIL_DESC & b )
        {
            return 0 == memcmp( &a, &b, sizeof(a) );
        }
    };

    ///
    /// generic state object cache.
    ///
    template<

        ///< must be a D3D10 state object class.
        typename OBJECT_CLASS,

        ///< state object descriptor type
        typename OBJECT_DESC,

        /// cache size (maxinum items that the cache can hold)
        size_t   CACHE_SIZE = 4096

    >
    class D3D10StateObjectCache
    {
        // *************************************************
        // public methods
        // *************************************************
    public:

        //@{

        D3D10StateObjectCache( ID3D10Device & dev )
            : mDevice( dev )
        {
            mHead = &mTail;
            mTail.prev = NULL;
            mTail.next = NULL;
            mTail.object = 0;
            mCount = 0;
            mLogger = getLogger( "GN.gfx.rndr.D3D10.D3D10StateObjectCache" );

            // initialize free item list
            mNextFreeItem = &mPool[0];
            for( UInt32 i = 0; i < CACHE_SIZE - 1; ++i )
            {
                mPool[i].nextFree = &mPool[i+1];
            }
            mPool[CACHE_SIZE-1].nextFree = NULL;
        }

        ~D3D10StateObjectCache()
        {
            clear();
        }

        /// get the D3D10 device
        ID3D10Device & dev() const { return mDevice; }

        /// get number of objects in cache
        size_t size() const { return mCount; }

        ///
        /// clear cache. Delete all state objects.
        ///
        void clear()
        {
            // delete all state objects
            StateObjectItem * item = mHead;
            while( item != &mTail )
            {
                safeRelease( item->object );
                item = item->next;
            }

            // clear LRU list
            mHead = &mTail;
            mTail.prev = NULL;
            mTail.next = NULL;

            // rebuild free list
            mNextFreeItem = &mPool[0];
            for( UInt32 i = 0; i < CACHE_SIZE - 1; ++i )
            {
                mPool[i].nextFree = &mPool[i+1];
            }
            mPool[CACHE_SIZE-1].nextFree = NULL;

            mCount = 0;
        }

        ///
        /// get existing item, if there is one; or create new item
        ///
        OBJECT_CLASS * operator[]( const OBJECT_DESC & desc )
        {
            // look up existing item first
            StateObjectItem * * hashitem = mHashTable.find( desc );
            if( hashitem )
            {
                StateObjectItem * item = *hashitem;

                GN_ASSERT( item );

                // update LRU
                MoveToHead( item );

                // found
                return item->object;
            }

            // if cache is full, remove old items.
            if( mCount >= CACHE_SIZE )
            {
                // remove half items from cache
                size_t numToRemove = CACHE_SIZE / 2;
                for( size_t i = 0; i < numToRemove; ++i )
                {
                    // get last one in LRU list
                    StateObjectItem * item = mTail.prev;
                    GN_ASSERT( item && item != mHead );

                    // delete the state object object
                    safeRelease( item->object );

                    // remove from hash
                    mHashTable.remove( item->desc );

                    // remove from LRU list
                    StateObjectItem * prev = item->prev;
                    StateObjectItem * next = item->next;
                    prev->next = next;
                    next->prev = prev;

                    // add to free list
                    item->nextFree = mNextFreeItem;
                    mNextFreeItem = item;

                    // adjust item count
                    --mCount;
                }
            }

            // create new state object
            OBJECT_CLASS * newobj = D3D10StateObjectCreator<OBJECT_DESC>::create( mDevice, desc );
            if( NULL == newobj ) return NULL;

            // get a new item out of free list
            GN_ASSERT( mNextFreeItem );
            StateObjectItem * item = mNextFreeItem;
            mNextFreeItem = item->nextFree;
            item->object = newobj;
            item->desc   = desc;

            // add to hash
            mHashTable.insert( desc, item );

            // update LRU
            InsertToHead( item );

            // update item count
            ++mCount;
            GN_ASSERT( mCount <= CACHE_SIZE );

            // success
            return newobj;
        }

        //@}

        // *************************************************
        // private types
        // *************************************************
    private:

        ///
        /// state object item
        ///
        struct StateObjectItem
        {
            OBJECT_CLASS        * object; ///< state object instance
            OBJECT_DESC           desc;   ///< state object descriptor

            union
            {
                StateObjectItem * prev; ///< point to previous item in LRU list
                StateObjectItem * nextFree; ///< point next free item. valid only when the item is not used.
            };
            StateObjectItem     * next; ///< point to next item in LRU list

            ///
            /// ctor
            ///
            StateObjectItem() : object(0), prev(0), next(0) {}
        };

        ///
        /// Hash map type
        ///
        typedef HashMap<
            OBJECT_DESC,
            StateObjectItem*,
            &D3D10StateObjectCreator<OBJECT_DESC>::hash,
            &D3D10StateObjectCreator<OBJECT_DESC>::equal
            > ObjectHashMap;

        // *************************************************
        // private members
        // *************************************************
    private:

        ID3D10Device    & mDevice;

        // pool
        StateObjectItem   mPool[CACHE_SIZE]; ///< pre-allocated item pool, to avoid runtime memory allocation
        StateObjectItem * mNextFreeItem;

        // hash
        ObjectHashMap     mHashTable;

        // LRU
        StateObjectItem   mTail; ///< the end of LRU list.
        StateObjectItem * mHead; ///< point to the most recently used item
        size_t            mCount;

        // misc.
        Logger          * mLogger;

        // *************************************************
        // private functions
        // *************************************************
    private:

        void InsertToHead( StateObjectItem * p )
        {
            GN_ASSERT( p );
            GN_ASSERT( p != &mTail );
            GN_ASSERT( mHead );
            mHead->prev = p;
            p->next = mHead;
            p->prev = NULL;
            mHead   = p;
        }

        void MoveToHead( StateObjectItem * p )
        {
            GN_ASSERT( p );
            GN_ASSERT( p != &mTail );

            if( p == mHead ) return;

            // p is not head, so there's at least 2 items in LRU list.
            GN_ASSERT( p->prev && p->next );

            // remove from list
            StateObjectItem * prev = p->prev;
            StateObjectItem * next = p->next;
            prev->next = next;
            next->prev = prev;

            // insert to head
            InsertToHead( p );
        }

        void RemoveFromList( StateObjectItem * p )
        {
            GN_ASSERT( p );
            GN_ASSERT( p != &mTail );

            if( mHead == p )
            {
                mHead        = p->next;
                mHead->prev = NULL;
                p->prev     = NULL;
                p->next     = NULL;
            }
            else
            {
                StateObjectItem * prev = p->prev;
                StateObjectItem * next = p->next;

                prev->next = next;
                next->prev = prev;

                p->prev = NULL;
                p->next = NULL;
            }
        }
    };

    //@{
    typedef D3D10StateObjectCache<ID3D10RasterizerState,D3D10_RASTERIZER_DESC>      RasterStateCache;
    typedef D3D10StateObjectCache<ID3D10BlendState,D3D10_BLEND_DESC>                BlendStateCache;
    typedef D3D10StateObjectCache<ID3D10DepthStencilState,D3D10_DEPTH_STENCIL_DESC> DepthStencilStateCache;
    //@}

    ///
    /// state object manager
    ///
    class D3D10StateObjectManager
    {
        // specific state managers
        RasterStateCache          mRasterStates;
        ID3D10RasterizerState   * mCurrentRS;

        BlendStateCache           mBlendStates;
        ID3D10BlendState        * mCurrentBS;
        float                     mCurrentBlendFactors[4];
        UInt32                    mCurrentSampleMask;

        DepthStencilStateCache    mDepthStates;
        ID3D10DepthStencilState * mCurrentDS;
        UInt32                    mCurrentStencilRef;

    public:

        /// constructor
        D3D10StateObjectManager( ID3D10Device & dev );

        /// clear all
        void clear()
        {
            mRasterStates.clear();
            mCurrentRS = NULL;

            mBlendStates.clear();
            mCurrentBS = NULL;

            mDepthStates.clear();
            mCurrentDS = NULL;
        }

        /// set rasterization state
        bool setRS(
            const D3D10_RASTERIZER_DESC & desc,
            bool                          skipDirtyCheck );

        /// set blend state
        bool setBS(
            const D3D10_BLEND_DESC & desc,
            const float            * blendFactors,
            UInt32                   sampleMask,
            bool                     skipDirtyCheck );

        /// set depth stencil state
        bool setDS(
            const D3D10_DEPTH_STENCIL_DESC & desc,
            UInt32                           stencilRef,
            bool                             skipDirtyCheck );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10STATEOBJECT_H__

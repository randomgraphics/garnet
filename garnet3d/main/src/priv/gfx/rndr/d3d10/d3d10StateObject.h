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
    /// generic state object cache.
    ///
    template<
        typename OBJECT_TYPE,     ///< must be a state object type.
        typename OBJECT_DESC,     ///< state object descriptor type
        typename KEY_TYPE,        ///< key structure that can uniquely identify a state object item, which must has hash() method
        UInt32 HASH_COUNT = 4093, ///< range of hash key
        UInt32 CACHE_SIZE = 4096, ///< cache size
        bool CHECK_PARAM  = true  ///< debug flag
        >
    class StateObjectCache
    {
        // *************************************************
        // public types
        // *************************************************
    public:

        ///
        /// state object item
        ///
        struct StateObjectItem
        {
            OBJECT_TYPE         * _object; ///< state object instance
            OBJECT_DESC           _desc;   ///< state object descriptor

            union
            {
                StateObjectItem * _prev; ///< point to previous item in LRU list
                StateObjectItem * _free; ///< point next free item. valid only when the item is not used.
            };
            StateObjectItem     * _next; ///< point to next item in LRU list

            KEY_TYPE              _key;   ///< unique item key
            UInt32                _hash;  ///< hash value

            ///
            /// ctor
            ///
            StateObjectItem() : _object(0), _prev(0), _next(0) {}
        };

        // *************************************************
        // public methods
        // *************************************************
    public:

        //@{

        StateObjectCache()
        {
            _head = &_tail;
            _tail._prev = NULL;
            _tail._next = NULL;
            _tail._object = 0;
            _count = 0;
            _logger = getLogger( "GN.gfx.rndr.D3D10.StateObjectCache" );

            // initialize free item list
            _nextFreeItem = &_pool[0];
            for( UInt32 i = 0; i < CACHE_SIZE - 1; ++i )
            {
                _pool[i]._free = &_pool[i+1];
            }
            _pool[CACHE_SIZE-1]._free = NULL;
        }

        ~StateObjectCache()
        {
            clear();
        }

        size_t size() const { return _count; }

        ///
        /// clear cache. Call draw engine to delete all state object items as well.
        ///
        void clear()
        {
            // delete all state objects
            StateObjectItem * item = _head;
            while( item != &_tail )
            {
                safeRelease( item->_object );
            }

            // clear LRU list
            _head = &_tail;
            _tail._prev = NULL;
            _tail._next = NULL;

            // rebuild free list
            _nextFreeItem = &_pool[0];
            for( UInt32 i = 0; i < CACHE_SIZE - 1; ++i )
            {
                _pool[i]._free = &_pool[i+1];
            }
            _pool[CACHE_SIZE-1]._free = NULL;

            _count = 0;
        }

        ///
        /// check if specific key is in cache. This function does not modify cache states.
        ///
        bool exist( const KEY_TYPE & key ) const
        {
             UInt32 h = key.hash<HASH_COUNT>();

            GN_ASSERT( h < HASH_COUNT );
            const Bucket & b = _buckets[h];

            for( size_t i = 0; i < b.size(); ++i )
            {
                StateObjectItem * item = b[i];

                if( item->_key == key )
                {
                    // found
                    return true;
                }
            }

            // not found
             return false;
        }

        ///
        /// get block instance of the key. Return NULL, if not found. Also update LRU list, if found.
        ///
        StateObjectItem * get( const KEY_TYPE & key )
        {
             UInt32 h = key.hash<HASH_COUNT>();

            GN_ASSERT( h < HASH_COUNT );
            const Bucket & b = _buckets[h];

            for( size_t i = 0; i < b.size(); ++i )
            {
                StateObjectItem * item = b[i];

                if( item->_key == key )
                {
                    // update LRU
                    MoveToHead( item );

                    // found
                    return item;
                }
            }

            // not found
             return NULL;
        }

        ///
        /// create new item ( key must be unique)
        ///
        StateObjectItem * create( const KEY_TYPE & key, const OBJECT_DESC & desc )
        {
            if( CHECK_PARAM )
            {
                if( exist(key) )
                {
                    GN_ERROR(_logger)( "insert failure: key must be unique!" );
                }
            }
            else
            {
                GN_ASSERT( !exist(key) );
            }

            if( _count >= CACHE_SIZE )
            {
                // remove half items from cache
                size_t numToRemove = CACHE_SIZE / 2;
                for( size_t i = 0; i < numToRemove; ++i )
                {
                    // get last one in LRU list
                    StateObjectItem * item = _tail._prev;
                    GN_ASSERT( item && item != _head );

                    // delete the state object object
                    safeRelease( item->_object );

                    // remove from bucket
                    GN_ASSERT( item->_hash < HASH_COUNT );
                    Bucket & b = _buckets[item->_hash];
                    GN_ASSERT( b._items.end() != std::find( b._items.begin(), b._items.end(), item ) );
                    b._items.erase(
                        std::remove( b._items.begin(), b._items.end(), item ),
                        b._items.end() );

                    // remove from LRU list
                    StateObjectItem * prev = item->_prev;
                    StateObjectItem * next = item->_next;
                    prev->_next = next;
                    next->_prev = prev;

                    // add to free list
                    item->_free = _nextFreeItem;
                    _nextFreeItem = item;

                    // adjust item count
                    --_count;
                }
            }

            // get a new item from free list
            GN_ASSERT( _nextFreeItem );
            StateObjectItem * item = _nextFreeItem;
            _nextFreeItem = item->_free;
            item->_key  = key;
            item->_desc = desc;

            // add to bucket
            UInt32 h = key.hash<HASH_COUNT>();
            GN_ASSERT( h < HASH_COUNT );
            Bucket & b = _buckets[h];
            b._items.push_back( item );
            item->_hash = h;

            // update LRU
            InsertToHead( item );

            // update item count
            ++_count;
            GN_ASSERT( _count <= HASH_COUNT );

            // success
            return item;
        }

        //@}

        // *************************************************
        // private types
        // *************************************************
    private:

        struct Bucket
        {
            DynaArray<StateObjectItem*> _items;

            size_t size() const { return _items.size(); }

            StateObjectItem * operator[]( size_t index ) const
            {
                GN_ASSERT( index < _items.size() );
                GN_ASSERT( _items[index] );
                return _items[index];
            }
        };

        // *************************************************
        // private members
        // *************************************************
    private:

        // pool
        StateObjectItem   _pool[CACHE_SIZE]; ///< pre-allocated item pool, to avoid runtime memory allocation
        StateObjectItem * _nextFreeItem;

        // hash
        Bucket           _buckets[HASH_COUNT];

        // LRU
        StateObjectItem   _tail; ///< the end of LRU list.
        StateObjectItem * _head; ///< point to the most recently used item
        size_t            _count;

        // misc.
        Logger          * _logger;

        // *************************************************
        // private functions
        // *************************************************
    private:

        void InsertToHead( StateObjectItem * p )
        {
            GN_ASSERT( p );
            GN_ASSERT( p != &_tail );
            p->_next = _head;
            p->_prev = NULL;
            _head   = p;
        }

        void MoveToHead( StateObjectItem * p )
        {
            GN_ASSERT( p );
            GN_ASSERT( p != &_tail );

            if( p == _head ) return;

            // p is not head, so there's at least 2 items in LRU list.
            GN_ASSERT( p->_prev && p->_next );

            // remove from list
            StateObjectItem * prev = p->_prev;
            StateObjectItem * next = p->_next;
            prev->_next = next;
            next->_prev = prev;

            // insert to head
            p->_next = _head;
            p->_prev = NULL;
            _head = p;
        }

        void RemoveFromList( StateObjectItem * p )
        {
            GN_ASSERT( p );
            GN_ASSERT( p != &_tail );

            if( _head == p )
            {
                _head        = p->_next;
                _head->_prev = NULL;
                p->_prev     = NULL;
                p->_next     = NULL;
            }
            else
            {
                StateObjectItem * prev = p->_prev;
                StateObjectItem * next = p->_next;

                prev->_next = next;
                next->_prev = prev;

                p->_prev = NULL;
                p->_next = NULL;
            }
        }
    };

    ///
    /// raster state object key
    ///
    union RasterStateKey
    {
        struct
        {
            /// \name the whole key as (64+8) bits integer
            //@{
            UInt64 u64;
            UInt8  u8;
            //@}
        };
        struct
        {
            UInt32  bias;  ///< depth bias value

            float   slop;  ///< slope bias value

            // byte 8
            unsigned int fill    : 2; ///< fill mode. D3D10_FILL_MODE.
            unsigned int cull    : 2; ///< cull mode. D3D10_CULL_MODE.
            unsigned int msaa    : 1; ///< MSAA enabled or not
            unsigned int aaline  : 1; ///< anti-aliased line enabled or not.
            unsigned int scissor : 1; ///< scissor test enabled or not
            unsigned int _       : 1; ///< reserved.
        };

        ///
        /// equality operator
        ///
        bool operator == ( const RasterStateKey & rhs ) const
        {
            return u64 == rhs.u64 && u8 == rhs.u8;
        }

        ///
        /// return hash key in range of [0,range)
        ///
        template<UInt32 RANGE>
        UInt32 hash() const
        {
            // We add the last byte (byte 8) to the higher 8 bits of depth bias value, because we
            // estimate that depth bias will be small integers at most of time.
            return (UInt32)( ( u64 + ( ((UInt64)u8) << 24 ) ) % RANGE );
        }
    };

    ///
    /// blend state object key
    ///
    union BlendStateKey
    {
        UInt64 u64; ///< the whole key as 64-bit integer
        struct
        {
            // byte 0
            unsigned int blendsrc  : 5; ///< blend src
            unsigned int blendop   : 3; ///< blend op

            // byte 1
            unsigned int alphasrc  : 5; ///< blend src for alpha channel
            unsigned int alphaop   : 3; ///< blend op for alpha channel

            // byte 2-3
            unsigned int alphamask : 1; ///< Alpha-To-Coverage enable or not
            unsigned int blend     : 1; ///< blend enable flag
            unsigned int blenddst  : 5; ///< blend dest
            unsigned int alphadst  : 5; ///< blend dest for alpha channel
            unsigned int _         : 4; ///< reserved

            // byte 4-7
            union
            {
                unsigned int rtmask      : 32; ///< write masks for all render targets.
                struct
                {
                    unsigned int rtmask0 : 4; ///< render target write mask 0 (R,G,B,A)
                    unsigned int rtmask1 : 4; ///< render target write mask 1 (R,G,B,A)
                    unsigned int rtmask2 : 4; ///< render target write mask 2 (R,G,B,A)
                    unsigned int rtmask3 : 4; ///< render target write mask 3 (R,G,B,A)
                    unsigned int rtmask4 : 4; ///< render target write mask 4 (R,G,B,A)
                    unsigned int rtmask5 : 4; ///< render target write mask 5 (R,G,B,A)
                    unsigned int rtmask6 : 4; ///< render target write mask 6 (R,G,B,A)
                    unsigned int rtmask7 : 4; ///< render target write mask 7 (R,G,B,A)
                };
            };
        };

        ///
        /// equality operator
        ///
        bool operator == ( const BlendStateKey & rhs ) const
        {
            return u64 == rhs.u64;
        }

        ///
        /// return hash key in range of [0,range)
        ///
        template<UInt32 RANGE>
        UInt32 hash() const
        {
            return (UInt32)( u64 % RANGE );
        }
    };
    GN_CASSERT( sizeof(BlendStateKey) == 8 );

    ///
    /// depth stencil state key
    ///
    union DepthStencilStateKey
    {
        UInt64 u64; ///< the whole key as 64-bit integrater
        struct
        {
            // byte 0
            unsigned int stencil : 1; ///< stencil enabled
            unsigned int zenable : 1; ///< z enable
            unsigned int zwrite  : 1; ///< z write enable
            unsigned int zfunc   : 4; ///< z compare function ( D3D10_COMPARISON_FUNC )
            unsigned int _       : 1; ///< reserved

            // byte 1-2
            unsigned int frontfail  : 4; ///< front face operation, if stencil fails ( D3D10_STENCIL_OP )
            unsigned int frontzfail : 4; ///< front face operation, if stencil passes and depth failed. ( D3D10_STENCIL_OP )
            unsigned int frontpass  : 4; ///< front face operation, if both stencil and depth pass ( D3D10_STENCIL_OP )
            unsigned int frontfunc  : 4; ///< front face stencil function ( D3D10_COMPARISON_FUNC )

            // byte 3-4
            unsigned int backfail   : 4; ///< back face operation, if stencil fails ( D3D10_STENCIL_OP )
            unsigned int backzfail  : 4; ///< back face operation, if stencil passes and depth failed. ( D3D10_STENCIL_OP )
            unsigned int backpass   : 4; ///< back face operation, if both stencil and depth pass ( D3D10_STENCIL_OP )
            unsigned int backfunc   : 4; ///< back face stencil function ( D3D10_COMPARISON_FUNC )

            // byte 5-6
            unsigned int stencilreadmask  : 8; ///< stencil read mask
            unsigned int stencilwritemask : 8; ///< stencil write mask

            // byte 7
            unsigned int : 8; ///< reserved
        };

        ///
        /// equality operator
        ///
        bool operator == ( const DepthStencilStateKey & rhs ) const
        {
            return u64 == rhs.u64;
        }

        ///
        /// return hash key in range of [0,range)
        ///
        template<UInt32 RANGE>
        UInt32 hash() const
        {
            return (UInt32)( u64 % RANGE );
        }
    };

    //@{
    typedef StateObjectCache<ID3D10RasterizerState,D3D10_RASTERIZER_DESC,RasterStateKey> RasterStateCache;
    typedef StateObjectCache<ID3D10BlendState,D3D10_BLEND_DESC,BlendStateKey> BlendStateCache;
    typedef StateObjectCache<ID3D10DepthStencilState,D3D10_DEPTH_STENCIL_DESC,DepthStencilStateKey> DepthStencilStateCache;
    //@}

    ///
    /// state object manager
    ///
    class D3D10StateObjectManager
    {
    public:

        //@{
        D3D10StateObjectManager( D3D10Renderer & r );
        ~D3D10StateObjectManager();
        //@}

        ///
        /// clear to empty
        ///
        void clear();

        /// \name setup individual states
        //@{

        //@}

        ///
        /// apply state to device
        ///
        void apply();

    private:

        D3D10Renderer & mRenderer;

		// rasterization state cache
		bool                  mRasterDirty;
		RasterStateKey        mRasterKey;
		D3D10_RASTERIZER_DESC mRasterDesc;
		RasterStateCache      mRasterStates;

		// blend state cache
		bool                  mBlendDirty;
		BlendStateKey         mBlendKey;
		D3D10_BLEND_DESC      mBlendDesc;
		BlendStateCache       mBlendStates;
		UInt32                mBlendAaMask;
		float                 mBlendFactors[4]; // one for each RGBA channel.

		// depth stencil state cache
		bool                     mDepthDirty;
		DepthStencilStateKey     mDepthKey;
		D3D10_DEPTH_STENCIL_DESC mDepthDesc;
		DepthStencilStateCache   mDepthStates;
		UInt32                   mstencilRef;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10STATEOBJECT_H__

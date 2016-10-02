#ifndef __GN_GFXD3D11_D3D11STATEOBJECT_H__
#define __GN_GFXD3D11_D3D11STATEOBJECT_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 state object cache
/// \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

#include "d3d11Gpu.h"

namespace GN { namespace gfx
{
    ///
    /// general state template (should never be compiled)
    template<class DESC>
    struct D3D11StateObjectCreator
    {
        GN_CASSERT( 0 == sizeof(DESC) );
    };

    ///
    /// rasterize state template
    ///
    template<>
    struct D3D11StateObjectCreator<D3D11_RASTERIZER_DESC>
    {
        static ID3D11RasterizerState *
        create( ID3D11Device & dev, const D3D11_RASTERIZER_DESC & desc )
        {
            ID3D11RasterizerState  * obj;
            GN_DX_CHECK_RETURN( dev.CreateRasterizerState( &desc, &obj ), NULL );
            return obj;
        }

        union CompactDesc
        {
            uint64           u64;
            struct
            {
                unsigned int fillmode  :  2;
                unsigned int cullmode  :  2;
                unsigned int frontccw  :  1;
                unsigned int depthclip :  1;
                unsigned int scissor   :  1;
                unsigned int msaa      :  1;
                unsigned int antialias :  1;
                unsigned int depthbias : 23;
                float        misc;
            };
        };
        GN_CASSERT( 8 == sizeof(CompactDesc) );

        struct Hash
        {
            uint64 operator()( const D3D11_RASTERIZER_DESC & desc ) const
            {
                CompactDesc cd;

                cd.fillmode  = desc.FillMode;
                cd.cullmode  = desc.CullMode;
                cd.frontccw  = desc.FrontCounterClockwise;
                cd.depthclip = desc.DepthClipEnable;
                cd.scissor   = desc.ScissorEnable;
                cd.msaa      = desc.MultisampleEnable;
                cd.antialias = desc.AntialiasedLineEnable;
                cd.depthbias = desc.DepthBias;
                cd.misc      = desc.DepthBiasClamp + desc.SlopeScaledDepthBias;

                return cd.u64;
            }
        };

        ///
        /// rasterize state equality check
        ///
        struct Equal
        {
            bool operator()(
                const D3D11_RASTERIZER_DESC & a,
                const D3D11_RASTERIZER_DESC & b ) const
            {
                return 0 == ::memcmp( &a, &b, sizeof(a) );
            }
        };
    };

    ///
    /// blend state template
    ///
    template<>
    struct D3D11StateObjectCreator<D3D11_BLEND_DESC>
    {
        static ID3D11BlendState *
        create( ID3D11Device & dev, const D3D11_BLEND_DESC & desc )
        {
            ID3D11BlendState * obj;
            GN_DX_CHECK_RETURN( dev.CreateBlendState( &desc, &obj ), NULL );
            return obj;
        }

        union CompactDesc
        {
            uint64 u64;
            struct
            {
                uint64 a2c  :  1;
                uint64 ibe  :  1;
                uint64 be   :  8;
                uint64 sb   :  5;
                uint64 db   :  5;
                uint64 bo   :  3;
                uint64 sba  :  5;
                uint64 dba  :  5;
                uint64 boa  :  3;
                uint64 mask : 28;
            };
        };
        GN_CASSERT( 8 == sizeof(CompactDesc) );

        struct Hash
        {
            uint64 operator()( const D3D11_BLEND_DESC & desc ) const
            {
                CompactDesc cd;

                cd.a2c  = desc.AlphaToCoverageEnable;
                cd.ibe  = desc.IndependentBlendEnable;
                cd.be   = desc.RenderTarget[0].BlendEnable;
                cd.sb   = desc.RenderTarget[0].SrcBlend;
                cd.db   = desc.RenderTarget[0].DestBlend;
                cd.bo   = desc.RenderTarget[0].BlendOp;
                cd.sba  = desc.RenderTarget[0].SrcBlendAlpha;
                cd.dba  = desc.RenderTarget[0].DestBlendAlpha;
                cd.boa  = desc.RenderTarget[0].BlendOpAlpha;
                cd.mask = desc.RenderTarget[0].RenderTargetWriteMask;

                for( int i = 1; i < 8; ++i )
                {
                    cd.be |= desc.RenderTarget[i].BlendEnable << i;
                    cd.mask += desc.RenderTarget[i].RenderTargetWriteMask;
                }

                return cd.u64;
            }
        };

        ///
        /// blend state equality check
        ///
        struct Equal
        {
            bool operator()(
                const D3D11_BLEND_DESC & a,
                const D3D11_BLEND_DESC & b ) const
            {
                return 0 == memcmp( &a, &b, sizeof(a) );
            }
        };
    };

    ///
    /// depth stencil state template
    ///
    template<>
    struct D3D11StateObjectCreator<D3D11_DEPTH_STENCIL_DESC>
    {
        static ID3D11DepthStencilState *
        create( ID3D11Device & dev, const D3D11_DEPTH_STENCIL_DESC & desc )
        {
            ID3D11DepthStencilState * obj;
            GN_DX_CHECK_RETURN( dev.CreateDepthStencilState( &desc, &obj ), NULL );
            return obj;
        }

        union CompactDesc
        {
            uint64 u64;
            struct
            {
                unsigned int depth     : 1;
                unsigned int write     : 1;
                unsigned int zfunc     : 3;
                unsigned int stencil   : 1;
                unsigned int ff_fail   : 3;
                unsigned int ff_zfail  : 3;
                unsigned int ff_pass   : 3;
                unsigned int ff_func   : 3;
                unsigned int bf_fail   : 3;
                unsigned int bf_zfail  : 3;
                unsigned int bf_pass   : 3;
                unsigned int bf_func   : 3;
                unsigned int nouse     : 2;
                unsigned int srmask    : 16;
                unsigned int swmask    : 16;
            };
        };
        GN_CASSERT( 8 == sizeof(CompactDesc) );

        struct Hash
        {
            uint64 operator()( const D3D11_DEPTH_STENCIL_DESC & desc ) const
            {
                CompactDesc cd;

                cd.depth    = desc.DepthEnable;
                cd.write    = desc.DepthWriteMask;
                cd.zfunc    = desc.DepthFunc - 1;
                cd.stencil  = desc.StencilEnable;
                cd.ff_fail  = desc.FrontFace.StencilFailOp - 1;
                cd.ff_zfail = desc.FrontFace.StencilDepthFailOp - 1;
                cd.ff_pass  = desc.FrontFace.StencilPassOp - 1;
                cd.ff_func  = desc.FrontFace.StencilFunc - 1;
                cd.bf_fail  = desc.BackFace.StencilFailOp - 1;
                cd.bf_zfail = desc.BackFace.StencilDepthFailOp - 1;
                cd.bf_pass  = desc.BackFace.StencilPassOp - 1;
                cd.bf_func  = desc.BackFace.StencilFunc - 1;
                cd.nouse    = 0;
                cd.srmask   = desc.StencilReadMask;
                cd.swmask   = desc.StencilWriteMask;

                return cd.u64;
            }
        };

        ///
        /// depth stencil state equality check
        ///
        struct Equal
        {
            bool operator()(
                const D3D11_DEPTH_STENCIL_DESC & a,
                const D3D11_DEPTH_STENCIL_DESC & b ) const
            {
                return 0 == memcmp( &a, &b, sizeof(a) );
            }
        };
    };

    ///
    /// D3D11_SAMPLER_DESC creator
    template<>
    struct D3D11StateObjectCreator<D3D11_SAMPLER_DESC>
    {
        static ID3D11SamplerState *
        create( ID3D11Device & dev, const D3D11_SAMPLER_DESC & desc )
        {
            ID3D11SamplerState * obj;
            GN_DX_CHECK_RETURN( dev.CreateSamplerState( &desc, &obj ), NULL );
            return obj;
        }

        union CompactDesc
        {
            uint64           u64;
            struct
            {
            uint64 filter      :  8;
            uint64 addressU    :  3;
            uint64 addressV    :  3;
            uint64 addressW    :  3;
            uint64 lodbias     :  4;
            uint64 maxaniso    :  3;
            uint64 compare     :  3;
            uint64 bordercolor : 32;
            uint64 lod         :  5;
            };
        };
        GN_CASSERT( 8 == sizeof(CompactDesc) );

        struct Hash
        {
            uint64 operator()( const D3D11_SAMPLER_DESC & desc ) const
            {
                CompactDesc cd;

    ///
    /// compose BGRA32 color constant
    ///
#define GN_RGBA32_FROM_FLOAT4( r, g, b, a )   \
            ( ( (((uint32)(r*255.0f))&0xFF) <<  0 ) | \
              ( (((uint32)(g*255.0f))&0xFF) <<  8 ) | \
              ( (((uint32)(b*255.0f))&0xFF) << 16 ) | \
              ( (((uint32)(a*255.0f))&0xFF) << 24 ) )

                cd.filter      = desc.Filter;
                cd.addressU    = desc.AddressU;
                cd.addressV    = desc.AddressV;
                cd.addressW    = desc.AddressW;
                cd.lodbias     = (uint64)desc.MipLODBias;
                cd.maxaniso    = (uint64)desc.MaxAnisotropy;
                cd.compare     = desc.ComparisonFunc;
                cd.bordercolor = GN_RGBA32_FROM_FLOAT4( desc.BorderColor[0],
                                                        desc.BorderColor[1],
                                                        desc.BorderColor[2],
                                                        desc.BorderColor[3] );
                cd.lod         = (uint32)( desc.MinLOD + desc.MaxLOD );

                return cd.u64;
            }
        };

        ///
        /// rasterize state equality check
        ///
        struct Equal
        {
            bool operator()(
                const D3D11_SAMPLER_DESC & a,
                const D3D11_SAMPLER_DESC & b ) const
            {
                return 0 == ::memcmp( &a, &b, sizeof(a) );
            }
        };
    };

    ///
    /// generic state object cache.
    ///
    template<

        ///< must be a D3D11 state object class.
        typename OBJECT_CLASS,

        ///< state object descriptor type
        typename OBJECT_DESC,

        /// cache size (maxinum items that the cache can hold)
        size_t   CACHE_SIZE = 4096

    >
    class D3D11StateObjectCache
    {
        // *************************************************
        // public methods
        // *************************************************
    public:

        //@{

        D3D11StateObjectCache( ID3D11Device & dev, ID3D11DeviceContext & cxt )
            : mDevice( dev )
            , mDeviceContext( cxt )
        {
            mHead = &mTail;
            mTail.prev = NULL;
            mTail.next = NULL;
            mTail.object = 0;
            mCount = 0;
            mLogger = GetLogger( "GN.gfx.gpu.D3D11.D3D11StateObjectCache" );

            // initialize free item list
            mNextFreeItem = &mPool[0];
            for( uint32 i = 0; i < CACHE_SIZE - 1; ++i )
            {
                mPool[i].nextFree = &mPool[i+1];
            }
            mPool[CACHE_SIZE-1].nextFree = NULL;
        }

        ~D3D11StateObjectCache()
        {
            clear();
        }

        /// get the D3D11 device
        ID3D11DeviceContext & devcxt() const { return mDeviceContext; }

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
                SafeRelease( item->object );
                item = item->next;
            }

            // clear LRU list
            mHead = &mTail;
            mTail.prev = NULL;
            mTail.next = NULL;

            // rebuild free list
            mNextFreeItem = &mPool[0];
            for( uint32 i = 0; i < CACHE_SIZE - 1; ++i )
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
                    SafeRelease( item->object );

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
            OBJECT_CLASS * newobj = D3D11StateObjectCreator<OBJECT_DESC>::create( mDevice, desc );
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
            CACHE_SIZE,
            typename D3D11StateObjectCreator<OBJECT_DESC>::Hash,
            typename D3D11StateObjectCreator<OBJECT_DESC>::Equal
            > ObjectHashMap;

        // *************************************************
        // private members
        // *************************************************
    private:

        ID3D11Device        & mDevice;
        ID3D11DeviceContext & mDeviceContext;

        // pool
        StateObjectItem       mPool[CACHE_SIZE]; ///< pre-allocated item pool, to avoid runtime memory allocation
        StateObjectItem     * mNextFreeItem;

        // hash
        ObjectHashMap         mHashTable;

        // LRU
        StateObjectItem       mTail; ///< the end of LRU list.
        StateObjectItem     * mHead; ///< point to the most recently used item
        size_t                mCount;

        // misc.
        Logger              * mLogger;

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
    typedef D3D11StateObjectCache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>      RasterStateCache;
    typedef D3D11StateObjectCache<ID3D11BlendState,D3D11_BLEND_DESC>                BlendStateCache;
    typedef D3D11StateObjectCache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC> DepthStencilStateCache;
    typedef D3D11StateObjectCache<ID3D11SamplerState,D3D11_SAMPLER_DESC>            SamplerStateCache;
    //@}

    ///
    /// state object manager
    ///
    class D3D11StateObjectManager
    {
        // specific state managers
        RasterStateCache          mRasterStates;
        ID3D11RasterizerState   * mCurrentRS;

        BlendStateCache           mBlendStates;
        ID3D11BlendState        * mCurrentBS;
        Vector4f                  mCurrentBlendFactors;
        uint32                    mCurrentSampleMask;

        DepthStencilStateCache    mDepthStates;
        ID3D11DepthStencilState * mCurrentDS;
        uint32                    mCurrentStencilRef;

        SamplerStateCache         mSamplerStates;
        ID3D11SamplerState      * mCurrentVSSamplers[D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT];
        ID3D11SamplerState      * mCurrentGSSamplers[D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT];
        ID3D11SamplerState      * mCurrentPSSamplers[D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT];

    public:

        /// constructor
        D3D11StateObjectManager( ID3D11Device & dev, ID3D11DeviceContext & cxt );

        /// clear all
        void clear()
        {
            mRasterStates.clear();
            mCurrentRS = NULL;

            mBlendStates.clear();
            mCurrentBS = NULL;

            mDepthStates.clear();
            mCurrentDS = NULL;

            mSamplerStates.clear();
            memset( mCurrentVSSamplers, 0, sizeof(mCurrentVSSamplers) );
            memset( mCurrentGSSamplers, 0, sizeof(mCurrentGSSamplers) );
            memset( mCurrentPSSamplers, 0, sizeof(mCurrentPSSamplers) );
        }

        /// set rasterization state
        bool setRS(
            const D3D11_RASTERIZER_DESC & desc,
            bool                          skipDirtyCheck );

        /// set blend state
        bool setBS(
            const D3D11_BLEND_DESC & desc,
            const Vector4f         & blendFactors,
            uint32                   sampleMask,
            bool                     skipDirtyCheck );

        /// set depth stencil state
        bool setDS(
            const D3D11_DEPTH_STENCIL_DESC & desc,
            uint32                           stencilRef,
            bool                             skipDirtyCheck );

        /// set VS samplers
        bool setVSSampler(
            const D3D11_SAMPLER_DESC & desc,
            uint32                     stage,
            bool                       skipDirtyCheck );

        /// set VS samplers
        bool setGSSampler(
            const D3D11_SAMPLER_DESC & desc,
            uint32                     stage,
            bool                       skipDirtyCheck );

        /// set VS samplers
        bool setPSSampler(
            const D3D11_SAMPLER_DESC & desc,
            uint32                     stage,
            bool                       skipDirtyCheck );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11STATEOBJECT_H__

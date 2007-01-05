#include "../testCommon.h"

namespace GN
{
    //!
    //! Threading model
    //!
    struct SingleThreadingModel
    {
        struct SyncLock {};
        struct AutoLock {};
    };
    
    //!
    //! object pool
    //!
    template<
        class  T,
        size_t N         = 256,
        size_t ALIGNMENT = 4,
        class  A         = StandardAllocator<T>,
        class  TM        = SingleThreadingModel >
    class ObjectPool
    {
        GN_CASSERT( N > 0 );
        GN_CASSERT_EX( ALIGNMENT > 0 && 0==(ALIGNMENT&(ALIGNMENT-1)), alignment_must_be_power_of_two );

        typedef ObjectPool<T,N,ALIGNMENT,A> MyType;        
        typedef typename A::template Rebind<UInt8>::Other Allocator;
        typedef TM ThreadingModel;

        struct Pool;
        struct Item
        {
            union
            {
                Item * nextFree;
                UInt8 _reserved[sizeof(T)]; // make sure Item is large enough to hold one object.
            };
            Pool * pool;
            MyType * owner;
            bool used;

            T * tptr() { return (T*)this; }
        };

        static const size_t ALIGNED_BYTES = 1 << ALIGNMENT;
        static const size_t ITEM_SIZE = ( sizeof(Item) + (ALIGNED_BYTES-1) ) & ~(ALIGNED_BYTES-1);

        struct Pool : public DoubleLinkedItem<Pool>
        {
            size_t count; // used items in this pool
            UInt8 data[ITEM_SIZE*N];

            Item & getItem( size_t i )
            {
                GN_ASSERT( i < N );
                return (Item*)(data + ITEM_SIZE * i );
            }
        };

        
        DoubleLinkedList<Pool> mAllPools;
        Item * mFreeItems;
        typename ThreadingModel::SyncLock mLock;

    private:

        void constructObject( T * p )
        {
            GN_ASSERT( p );
            new (p) T;
        }

        void destructObject( T * p )
        {
            GN_ASSERT( p );
            p->T::~T();
        }

        T * doAlloc()
        {
            typename ThreadingModel::AutoLock locker(mLock);

            if( !mFreeItems )
            {
                // create new pool
                Pool * p = Allocator::sAlloc( 1 );
                if( 0 == p ) return 0;
                mAllPools.append( p );

                // format the pool
                p->count = 0;
                for( size_t i = 0; i < N; ++i )
                {
                    Item & item = p->getItem( i );
                    item.nextFree = ( (N-1) == i ) ? 0 : p->getItem( i+1 );
                    item.pool = p;
                    item.owner = this;
                    item.used = false;
                }
                mFreeItems = &p->getItem( 0 );
            }
            
            // get the first free item
            Item * p = mFreeItems;
            GN_ASSERT( !p->used );
            p->used = true;
            mFreeItems = mFreeItems->nextFree;

            // adjust item count of the pool
            GN_ASSERT( p->pool && p->pool.count < N );
            ++p->pool.count;

            // construct the object.
            constructObject( p );

            // success
            return p->tptr();
        }

        void doDealloc( T * p )
        {
            if( !p ) return;

            typename ThreadingModel::AutoLock locker(mLock);

            Item * item = (Item*)p;

            GN_ASSERT( this == item->owner );

            // destruct the object
            destructObject( p );

            // insert to free list
            GN_ASSERT( item->used );
            item->nextFree = mFreeItems;
            item->used = false;
            mFreeItems = item;

            // adjust pool's item count as well.
            GN_ASSERT( item->pool && item->pool->count > 0 );
            --item->pool->count;
        }

        void doFreeAll()
        {
            Pool * p = mAllPools;
            while( p )
            {
                Pool * next = p->next;

                for( size_t i = 0; i < N && p->count > 0; ++i )
                {
                    Item & item = p->getItem( i );
                    if( item.used )
                    {
                        destructObject( item.tptr() );
                        --p->count;
                    }
                }

                p = next;
            }
        }

        void doRecycle() // free empty pool
        {
            Pool * next;
            Pool * p = mAllPools.head();
            while( p )
            {
                next = p->next;

                if( 0 == p->count )
                {
                    mAllPools.remove( p );
                    Allocator::sDealloc( p, 1 );
                }

                p = next;
            }
        }

    public:

        //@{
        ObjectPool() : mAllPools(0), mFreeItems(0) {}
        ~ObjectPool() { freeAll(); }
        //@}

        //@{
        T * alloc() { return doAlloc(); }
        void dealloc( T * p ) { doDealloc( p ); }
        void freeAll() { doFreeAll(); }
        void recycle() { doRecycle(); } //!< Free unused memory as much as possible.
        size_t getItemCount() const;
        size_t getMemoryFootprint() const;
        //@}
    };
}

class MemPoolTest : public CxxTest::TestSuite
{
    struct Test
    {
        static GN::FixedSizedObjectAllocator<Test> sAllocator;
        inline void * operator new( size_t ) { return sAllocator.alloc(); }
        inline void operator delete( void* p ) { sAllocator.dealloc(p); }
        inline void * operator new( size_t, void * p ) { GN_INFO(sLogger)("placement new"); return p; }
        inline void operator delete( void*, void * ) { GN_INFO(sLogger)("placement delete"); }
    };

public:

    void testCustomAllocator()
	{
#if GN_GCC
        GN_TODO( "StdAllocator<> does not pass GCC build." );
#else
        typedef std::vector<char,GN::StlAllocator<char> > CharArray;
        typedef std::vector<Test,GN::StlAllocator<GN::StrA> > TestArray;
        CharArray a1(10);
        TestArray a2(10);
        TS_ASSERT_EQUALS( a1.size(), a2.size() );
#endif
    }

    void testPlacementNew()
    {
        UInt8 buf[sizeof(Test)*10];
        Test * a = new(buf) Test;
        TS_ASSERT_EQUALS( a, (Test*)buf );
        a->~Test();
    }

    void testObjectPoolWithPOD()
    {
        GN::FixedSizedObjectAllocator<char> a;
        a.alloc();
    }

    void testClassAllocator()
    {
        using namespace GN;

        for( int i = 0; i < 10000; ++i )
        {
            new Test;
        }
    }

    void testOneByteAllocator()
    {
        using namespace GN;
        FixedSizedMemoryAllocator a(1);

        for( int i = 0; i < 10000; ++i )
        {
            a.alloc();
        }
    }

    void testManyBytesAllocator()
    {
        using namespace GN;
        FixedSizedMemoryAllocator a(128);

        for( int i = 0; i < 10000; ++i )
        {
            a.alloc();
        }
    }
};
GN::FixedSizedObjectAllocator<MemPoolTest::Test> MemPoolTest::Test::sAllocator;

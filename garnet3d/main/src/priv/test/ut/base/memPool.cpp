#include "../testCommon.h"

class MemPoolTest : public CxxTest::TestSuite
{
    struct Test
    {
        inline void * operator new( size_t );
        inline void   operator delete( void * p );
        inline void * operator new( size_t, void * p ) { return p; }// GN_INFO(sLogger)("placement new"); return p; }
        inline void   operator delete( void *, void * ) {}// GN_INFO(sLogger)("placement delete"); }
    };

    static GN::ObjectPool<Test> sPool;

public:

    void testCustomAllocator()
	{
#if GN_GCC
        GN_TODO( "StlAllocator<> does not pass GCC build." );
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
        GN::ObjectPool<char> a;
        a.allocConstructed();
    }

    void testClassAllocator()
    {
        using namespace GN;

        for( int i = 0; i < 10000; ++i )
        {
            new Test;
        }
    }

    void testRawMemoryAllocator()
    {
        using namespace GN;

        FixSizedRawMemoryPool<12,8,2> a;

        void * p0 = a.alloc();
        void * p1 = a.alloc();
        void * p2 = a.alloc();
        void * p3 = a.alloc();

#if GN_X64
        TS_ASSERT_EQUALS( 40, (UInt8*)p0 - (UInt8*)p1 );
        TS_ASSERT_EQUALS( 40, (UInt8*)p2 - (UInt8*)p3 );
#else
        TS_ASSERT_EQUALS( 24, (UInt8*)p0 - (UInt8*)p1 );
        TS_ASSERT_EQUALS( 24, (UInt8*)p2 - (UInt8*)p3 );
#endif
    }

    void testOneByteAllocator()
    {
        using namespace GN;

        FixSizedRawMemoryPool<1> a;

        for( int i = 0; i < 100000; ++i )
        {
            a.alloc();
        }
    }

    void testManyBytesAllocator()
    {
        using namespace GN;
        FixSizedRawMemoryPool<128> a;

        for( int i = 0; i < 100000; ++i )
        {
            a.alloc();
        }
    }
};

inline void * MemPoolTest::Test::operator new( size_t ) { return MemPoolTest::sPool.allocUnconstructed(); }
inline void   MemPoolTest::Test::operator delete( void * p ) { MemPoolTest::sPool.freeWithoutDeconstruct(p); }
GN::ObjectPool<MemPoolTest::Test> MemPoolTest::sPool;

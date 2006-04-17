#include "../testCommon.h"

class MemPoolTest : public CxxTest::TestSuite
{
    struct Test
    {
        static GN::FixedSizedObjectAllocator<Test> sAllocator;
        inline void * operator new( size_t ) { return sAllocator.alloc(); }
        inline void operator delete( void* p ) { sAllocator.dealloc(p); }
        inline void * operator new( size_t, void * p ) { GN_INFO("placement new"); return p; }
        inline void operator delete( void*, void * ) { GN_INFO("placement delete"); }
    };

public:

    void testPlacementNew()
    {
        uint8_t buf[sizeof(Test)*10];
        Test * a = new(buf) Test;
        TS_ASSERT_EQUALS( a, (Test*)buf );
        a->~Test();
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

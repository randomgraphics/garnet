#include "../testCommon.h"

class MemPoolTest : public CxxTest::TestSuite
{
    struct Test
    {
        static GN::FixedSizedObjectAllocator<Test> sAllocator;
        inline void * operator new( size_t ) { return sAllocator.alloc(); }
        inline void operator delete( void* p ) { sAllocator.dealloc(p); }
    };

public:

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

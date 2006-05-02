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

    typedef std::vector<char,GN::MemAllocator<char> > CharArray;
    typedef std::vector<Test,GN::MemAllocator<GN::StrA> > TestArray;

public:

    void testCustomAllocator()
	{
        CharArray a1(10);
        TestArray a2(10);
        TS_ASSERT_EQUALS( a1.size(), a2.size() );
    }

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

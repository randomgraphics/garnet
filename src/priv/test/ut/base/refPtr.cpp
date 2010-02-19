#include "../testCommon.h"

class RefPtrTest : public CxxTest::TestSuite
{
    struct MyRefObj : public GN::RefCounter
    {
    };
    typedef GN::AutoRef<MyRefObj> MyRefObjPtr;

    struct MyRefObj2 : public MyRefObj
    {
    };
    typedef GN::AutoRef<MyRefObj2> MyRefObj2Ptr;

    struct MyStruct
    {
        MyRefObjPtr p;
    };

public:

    void testCopyCtor()
    {
        MyRefObj2Ptr p2( new MyRefObj2 );
        TS_ASSERT_EQUALS( p2->getref(), 1 );

        MyRefObjPtr p1( p2 );
        TS_ASSERT_EQUALS( p1->getref(), 2 );

        MyRefObjPtr p3( p1 );
        TS_ASSERT_EQUALS( p3->getref(), 3 );
   }

    void testAssignment()
    {
        MyRefObjPtr p2( new MyRefObj2 );
        TS_ASSERT_EQUALS( p2->getref(), 1 );

        MyRefObjPtr p1a, p1b;

        p1a = p2;
        TS_ASSERT_EQUALS( p1a->getref(), 2 );

        p1b = p1a;
        TS_ASSERT_EQUALS( p1b->getref(), 3 );
    }

    void testWeakRef()
    {
        using namespace GN;

        AutoRef<MyRefObj> o1( new MyRefObj );

        WeakRef<MyRefObj> w1( o1.get() );
        WeakRef<MyRefObj> w2;
        WeakRef<MyRefObj> w3( w1 );

        w2.set( o1 );

        TS_ASSERT( w1 );
        TS_ASSERT( w2 );
        TS_ASSERT( w3 );
        TS_ASSERT( w1 == w2 );
        TS_ASSERT( w1 == w3 );

        o1.Clear();

        TS_ASSERT( !w1 );
        TS_ASSERT( !w2 );
        TS_ASSERT( !w3 );
    }
};

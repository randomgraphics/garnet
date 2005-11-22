#include "../testCommon.h"

class RefPtrTest : public CxxTest::TestSuite
{
    struct MyRefObj : GN::RefCounter
    {
    };
    typedef GN::AutoRef<MyRefObj> MyRefObjPtr; 

    struct MyRefObj2 : MyRefObj
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
};

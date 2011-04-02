#include "../testCommon.h"

class AutoPtrTest : public CxxTest::TestSuite
{
    static int a;

    struct S1
    {
        int a;
        S1(int i) { AutoPtrTest::a = a = i; }
        ~S1() { AutoPtrTest::a = 0; }
    };

public:

    void testCtorDtor()
    {
        a = -1;
        {
            S1 * c1 = new S1(1);
            TS_ASSERT_EQUALS( 1, a );

            GN::AutoObjPtr<S1> p1( c1 );
            TS_ASSERT_EQUALS( 1, p1->a );
        }
        TS_ASSERT_EQUALS( 0, a );
    }

    void testAttach()
    {
        GN::AutoObjPtr<S1> p1;

        p1.attach( new S1(1) );
        TS_ASSERT_EQUALS( 1, p1->a );

        p1.attach( new S1(2) );
        TS_ASSERT_EQUALS( 2, p1->a );

        p1.attach( 0 );
        TS_ASSERT_EQUALS( (S1*)0, p1.rawptr() );
    }

    void testDetatch()
    {
        S1 * p1 = new S1(1);

        GN::AutoObjPtr<S1> p2( p1 );
        TS_ASSERT_EQUALS( 1, p2->a );

        S1 * p3 = p2.detach();
        delete p3;
        TS_ASSERT_EQUALS( p1, p3 );
        TS_ASSERT( !p2 );
    }
};
int AutoPtrTest::a;

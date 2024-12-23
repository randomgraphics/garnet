#include "../testCommon.h"

class AutoPtrTest : public CxxTest::TestSuite {
    inline static int a = 0;
    inline static int c = 0;

    struct S1 {
        int a;
        S1(int i) { AutoPtrTest::a = a = i; ++c; }
        ~S1() { AutoPtrTest::a = 0; --c; }
    };

public:
    void testCtorDtor() {
        a = -1;
        {
            S1 * c1 = new S1(1);
            TS_ASSERT_EQUALS(1, a);

            GN::AutoObjPtr<S1> p1(c1);
            TS_ASSERT_EQUALS(1, p1->a);
        }
        TS_ASSERT_EQUALS(0, a);
    }

    void testAttach() {
        GN::AutoObjPtr<S1> p1;

        p1.attach(new S1(1));
        TS_ASSERT_EQUALS(1, p1->a);

        p1.attach(new S1(2));
        TS_ASSERT_EQUALS(2, p1->a);

        p1.attach(0);
        TS_ASSERT_EQUALS((S1 *) 0, p1.rawptr());
    }

    void testDetatch() {
        S1 * p1 = new S1(1);

        GN::AutoObjPtr<S1> p2(p1);
        TS_ASSERT_EQUALS(1, p2->a);

        S1 * p3 = p2.detach();
        delete p3;
        TS_ASSERT_EQUALS(p1, p3);
        TS_ASSERT(!p2);
    }

    void testCopy() {
        GN::AutoObjPtr<S1> p1(new S1(1));
        GN::AutoObjPtr<S1> p2(p1);
        GN::AutoObjPtr<S1> p3 = p1;
        GN::AutoObjPtr<S1> p4;
        p4 = p1;

        TS_ASSERT_EQUALS(1, c); // there should be only one object

        TS_ASSERT_EQUALS(1, p1->a);
        TS_ASSERT_EQUALS(1, p2->a);
        TS_ASSERT_EQUALS(1, p3->a);
        TS_ASSERT_EQUALS(1, p4->a);

        p1.clear();
        p2.attach(nullptr);
        p3.clear();

        // p4 is the only one holding the object
        TS_ASSERT_EQUALS(1, p4->a);

        p4.clear();

        // object should be deleted
        TS_ASSERT_EQUALS(0, c);
    }

    struct FakeComClass {
        int ref;

        FakeComClass(): ref(0) {}

        ~FakeComClass() {}

        int AddRef() { return ++ref; }
        int Release() { return --ref; }
    };

    void testAutoComPtrAttach() {
        using namespace GN;

        FakeComClass c;
        c.ref                       = 1;
        AutoComPtr<FakeComClass> p1 = AutoComPtr<FakeComClass>::sAttach(&c);
        TS_ASSERT_EQUALS(c.ref, 1);
    }

    void testAutoComPtrAssignment() {
        using namespace GN;

        FakeComClass c;

        AutoComPtr<FakeComClass> p1;
        AutoComPtr<FakeComClass> p2;
        p1.set(&c);
        p2 = p1;
        TS_ASSERT_EQUALS(c.ref, 2);
        p2.clear();
        TS_ASSERT_EQUALS(c.ref, 1);
        p1.clear();
        TS_ASSERT_EQUALS(c.ref, 0);
    }
};

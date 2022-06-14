#include "../testCommon.h"
#ifdef HAS_BOOST
    #include <boost/bind.hpp>
#endif

GN::DynaArray<GN::StrA> g_callSequence;
GN::StrA                g_funcName;
int                     g_int1 = 0;
int                     g_int2 = 0;

void foo1(int i1, int i2) {
    g_funcName = "foo1()";
    g_callSequence.append("foo1()");
    g_int1 = i1;
    g_int2 = i2;
}

void foo2(int i1, int i2) {
    g_funcName = "foo2()";
    g_callSequence.append("foo2()");
    g_int1 = i1;
    g_int2 = i2;
}

void GN_FASTCALL foo3(int i1, int i2) {
    g_funcName = "foo3()";
    g_callSequence.append("foo3()");
    g_int1 = i1;
    g_int2 = i2;
}

void GN_STDCALL foo4(int i1, int i2) {
    g_funcName = "foo4()";
    g_callSequence.append("foo4()");
    g_int1 = i1;
    g_int2 = i2;
}

void GN_CDECL foo5(int i1, int i2) {
    g_funcName = "foo5()";
    g_callSequence.append("foo5()");
    g_int1 = i1;
    g_int2 = i2;
}

struct aaa {
    virtual ~aaa() {}

    void foo1(int a, int b) {
        g_funcName = "aaa::foo1()";
        g_callSequence.append("aaa::foo1()");
        g_int1 = a;
        g_int2 = b;
    }

    void foo1(int a, int b) const {
        g_funcName = "aaa::foo1() const";
        g_callSequence.append("aaa::foo1() const");
        g_int1 = a;
        g_int2 = b;
    }

    void foo2(int a, int b) {
        g_funcName = "aaa::foo2()";
        g_callSequence.append("aaa::foo2()");
        g_int1 = a;
        g_int2 = b;
    }

    void foo2(int a, int b) const {
        g_funcName = "aaa::foo2() const";
        g_callSequence.append("aaa::foo2() const");
        g_int1 = a;
        g_int2 = b;
    }

    int foo3(int a, int b) const {
        g_funcName = "aaa::foo3() const";
        g_callSequence.append("aaa::foo3()");
        g_int1 = a;
        g_int2 = b;
        return 0;
    }
};

struct bbb : public aaa, public GN::SlotBase {
    virtual ~bbb() {}

    void foo(int a, int b) const {
        g_funcName = "bbb::foo()";
        g_callSequence.append("bbb::foo()");
        g_int1 = a;
        g_int2 = b;
    }
};

template<class F1, class F2>
bool equal(const F1 & f1, const F2 & f2) {
    return f1 == f2 && f2 == f1 && !(f1 != f2) && !(f2 != f1) && !(f1 < f2) && !(f2 < f1) && !(f1 > f2) && !(f2 > f1);
}

template<class F1, class F2>
bool unequal(const F1 & f1, const F2 & f2) {
    return f1 != f2 && f2 != f1 && !(f1 == f2) && !(f2 == f1) && (f1 < f2 || f2 < f1) && (f1 > f2 || f2 > f1);
}

#define ASSERT_FUNC(NAME, INT1, INT2)   \
    TS_ASSERT_EQUALS(g_funcName, NAME); \
    TS_ASSERT_EQUALS(g_int1, INT1);     \
    TS_ASSERT_EQUALS(g_int2, INT2);

class FunctorTest : public CxxTest::TestSuite {
public:
    void testCallConvension() {
        GN::Delegate2<void, int, int> f3, f4, f5;
        f3 = GN::makeDelegate(&foo3);
        f4 = GN::makeDelegate(&foo4);
        f5 = GN::makeDelegate(&foo5);
        f3(1, 2);
        ASSERT_FUNC("foo3()", 1, 2);
        f4(3, 4);
        ASSERT_FUNC("foo4()", 3, 4);
        f5(5, 6);
        ASSERT_FUNC("foo5()", 5, 6);
    }

    void testMakeDeletate() {
        aaa                           a;
        const aaa &                   ca = a;
        GN::Delegate2<void, int, int> f1, f2, f3;
        f1 = GN::makeDelegate(&foo1);
        f2 = GN::makeDelegate<aaa, aaa, void, int, int>(&a, &aaa::foo1);
        f3 = GN::makeDelegate<aaa, aaa, void, int, int>(&ca, &aaa::foo1);

        f1(1, 2);
        ASSERT_FUNC("foo1()", 1, 2);
        f2(3, 4);
        ASSERT_FUNC("aaa::foo1()", 3, 4);
        f3(5, 6);
        ASSERT_FUNC("aaa::foo1() const", 5, 6);
    }

    void testDeleagte() {
        aaa                           a;
        bbb                           b;
        const aaa &                   ca = a;
        GN::Delegate2<void, int, int> f1, f2;

        // self comparision, free function
        f1.bind(&foo1);
        TS_ASSERT(equal(f1, f1));
        f1(1, 2);
        ASSERT_FUNC("foo1()", 1, 2);

        // same free functions
        f2.bind(&foo1);
        TS_ASSERT(equal(f1, f2));
        g_funcName = "";
        f2(3, 4);
        ASSERT_FUNC("foo1()", 3, 4);

        // different free functions
        f2.bind(&foo2);
        TS_ASSERT(unequal(f1, f2));
        f2(1, 2);
        ASSERT_FUNC("foo2()", 1, 2);

        // free function != member function
        f2.bind<aaa, aaa>(&a, &aaa::foo1);
        TS_ASSERT(unequal(f1, f2));
        f2(3, 4);
        ASSERT_FUNC("aaa::foo1()", 3, 4);

        // self compare, member function
        f1.bind<aaa, aaa>(&a, &aaa::foo1);
        TS_ASSERT(equal(f1, f1));
        g_funcName = "";
        f1(1, 2);
        ASSERT_FUNC("aaa::foo1()", 1, 2);

        // same member function
        f2.bind<aaa, aaa>(&a, &aaa::foo1);
        TS_ASSERT(equal(f1, f2));
        f2(1, 2);

        // different member function, same class, different constness
        f1.bind<aaa, aaa>(&ca, &aaa::foo1);
        TS_ASSERT(unequal(f1, f2));
        f1(3, 4);
        ASSERT_FUNC("aaa::foo1() const", 3, 4);

        // different member function, same class, different function
        f2.bind<aaa, aaa>(&ca, &aaa::foo2);
        TS_ASSERT(unequal(f1, f2));
        f2(1, 2);
        ASSERT_FUNC("aaa::foo2() const", 1, 2);

        f1.bind<aaa, aaa>(&a, &aaa::foo1);
        f2.bind<aaa, aaa>(&ca, &aaa::foo2);
        TS_ASSERT(unequal(f1, f2));
        f1(3, 4);
        ASSERT_FUNC("aaa::foo1()", 3, 4);
        f2(1, 2);
        ASSERT_FUNC("aaa::foo2() const", 1, 2);

        // different member function, different class
        f1.bind<aaa, aaa>(&a, &aaa::foo1);
        f2.bind<aaa, bbb>(&b, &aaa::foo1);
        TS_ASSERT(unequal(f1, f2));
        g_funcName = "";
        f1(1, 2);
        ASSERT_FUNC("aaa::foo1()", 1, 2);
        g_funcName = "";
        f2(3, 4);
        ASSERT_FUNC("aaa::foo1()", 3, 4);

#ifdef HAS_BOOST
        // working with boost::bind
        boost::bind<void>(f1, 1, 2)();
        int i      = 2;
        g_funcName = "";
        g_int1     = 0;
        g_int2     = 0;
        boost::bind<void>(f1, _1, _1)(i);
        ASSERT_FUNC("aaa::foo1()", 2, 2);
#endif
    }

    struct F1 {
        int & ii;
        F1(int & i): ii(i) {}
        void operator()() { ii = 1; }
    };
    struct F2 {
        int & ii;
        F2(int & i): ii(i) {}
        void operator()() { ii = 2; }
    };

    void testBindToFunctor() {
        int i = 0;
        F1  f1(i);
        F2  f2(i);

        GN::Delegate0<void> d1, d2;

        d1.bind(f1);
        d2.bind(&f2);

        d1(); // copy_of_f1.opeartor()
        TS_ASSERT_EQUALS(i, 1);

        d2(); // fp->operator()
        TS_ASSERT_EQUALS(i, 2);
    }

    void testConstAndNonConst() {
        aaa                         a;
        const aaa &                 b = a;
        GN::Signal2<void, int, int> s1;
        s1.connect(&a, &aaa::foo1);
        s1.connect<aaa, aaa>(&b, &aaa::foo1);
        TS_ASSERT_EQUALS(s1.getNumSlots(), 2);
        g_callSequence.clear();
        s1(0, 1);
        TS_ASSERT_EQUALS(g_callSequence[0], "aaa::foo1()");
        TS_ASSERT_EQUALS(g_callSequence[1], "aaa::foo1() const");
    }

    void testSigslotToFreeFunction() {
        GN::Signal2<void, int, int> s;

        s.connect(&foo1);
        TS_ASSERT_EQUALS(s.getNumSlots(), 1);
        s.disconnect(&foo1);
        TS_ASSERT_EQUALS(s.getNumSlots(), 0);
    }

    void testSigslotToMethod() {
        aaa                         a;
        GN::Signal2<void, int, int> s;

        s.connect(&a, &aaa::foo1);
        TS_ASSERT_EQUALS(s.getNumSlots(), 1);
        s.disconnect(&a);
        TS_ASSERT_EQUALS(s.getNumSlots(), 0);
    }

    void testSigslotToSlotClass() {
        bbb                         b;
        GN::Signal2<void, int, int> s;

        s.connect(&b, &bbb::foo1);
        TS_ASSERT_EQUALS(s.getNumSlots(), 1);
        s.disconnect(&b);
        TS_ASSERT_EQUALS(s.getNumSlots(), 0);
    }

    void testSigslotAutoDisconnection1() {
        GN::Signal2<void, int, int> s;
        {
            bbb b;
            s.connect(&b, &bbb::foo1);
            TS_ASSERT_EQUALS(s.getNumSlots(), 1);
            TS_ASSERT_EQUALS(b.getNumSignals(), 1);
        }
        TS_ASSERT_EQUALS(s.getNumSlots(), 0);
    }

    void testSigslotAutoDisconnection2() {
        bbb b;
        {
            GN::Signal2<void, int, int> s;
            s.connect(&b, &bbb::foo1);
            TS_ASSERT_EQUALS(s.getNumSlots(), 1);
            TS_ASSERT_EQUALS(b.getNumSignals(), 1);
        }
        TS_ASSERT_EQUALS(b.getNumSignals(), 0);
    }

    void testSigslotComplex() {
        aaa                         a;
        GN::Signal2<void, int, int> s1;
        s1.connect(&a, &aaa::foo1);
        s1.connect(&a, &aaa::foo2);
        s1.connect(&foo1);
        s1.connect(&a, &aaa::foo1); // duplicate member function
        s1.connect(&foo1);          // duplicate free function
        TS_ASSERT_EQUALS(s1.getNumSlots(), 3);
        g_callSequence.clear();
        s1.emit(0, 0);
        TS_ASSERT_EQUALS(g_callSequence.size(), 3);
        TS_ASSERT_EQUALS(g_callSequence[0], "aaa::foo1()");
        TS_ASSERT_EQUALS(g_callSequence[1], "aaa::foo2()");
        TS_ASSERT_EQUALS(g_callSequence[2], "foo1()");
        {
            bbb b;

            s1.connect(&b, &bbb::foo);
            s1.connect(&b, &aaa::foo1);

            TS_ASSERT_EQUALS(s1.getNumSlots(), 5);
            TS_ASSERT_EQUALS(b.getNumSignals(), 2);

            g_callSequence.clear();
            s1.emit(1, 1);
            TS_ASSERT_EQUALS(g_callSequence.size(), 5);
            TS_ASSERT_EQUALS(g_callSequence[0], "aaa::foo1()");
            TS_ASSERT_EQUALS(g_callSequence[1], "aaa::foo2()");
            TS_ASSERT_EQUALS(g_callSequence[4], "aaa::foo1()");
            TS_ASSERT_EQUALS(g_callSequence[2], "foo1()");
            TS_ASSERT_EQUALS(g_callSequence[3], "bbb::foo()");

            {
                GN::Signal2<void, int, int> s2;
                s2.connect(&b, &bbb::foo);
                s2.connect(&a, &aaa::foo1);

                TS_ASSERT_EQUALS(s2.getNumSlots(), 2);
                TS_ASSERT_EQUALS(b.getNumSignals(), 3);

                g_callSequence.clear();
                s2.emit(5, 5);
                TS_ASSERT_EQUALS(g_callSequence.size(), 2);
                TS_ASSERT_EQUALS(g_callSequence[0], "bbb::foo()");
                TS_ASSERT_EQUALS(g_callSequence[1], "aaa::foo1()");
            }

            TS_ASSERT_EQUALS(s1.getNumSlots(), 5);
            TS_ASSERT_EQUALS(b.getNumSignals(), 2);
        }

        // connection to b should be released automatically.
        TS_ASSERT_EQUALS(s1.getNumSlots(), 3);
        g_callSequence.clear();
        s1.emit(0, 0);
        TS_ASSERT_EQUALS(g_callSequence.size(), 3);
        TS_ASSERT_EQUALS(g_callSequence[0], "aaa::foo1()");
        TS_ASSERT_EQUALS(g_callSequence[1], "aaa::foo2()");
        TS_ASSERT_EQUALS(g_callSequence[2], "foo1()");
    }
};

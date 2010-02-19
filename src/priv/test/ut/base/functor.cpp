#include "../testCommon.h"
#include <vector>
#ifdef HAS_BOOST
#include <boost/Bind.hpp>
#endif

std::vector<GN::StrA>   g_callSequence;
GN::StrA                g_funcName;
int                     g_int1 = 0;
int                     g_int2 = 0;

void foo1(int i1, int i2)
{
    g_funcName = "foo1()";
    g_callSequence.push_back( "foo1()" );
    g_int1 = i1;
    g_int2 = i2;
}

void foo2(int i1, int i2)
{
    g_funcName = "foo2()";
    g_callSequence.push_back( "foo2()" );
    g_int1 = i1;
    g_int2 = i2;
}

void GN_FASTCALL foo3(int i1, int i2)
{
    g_funcName = "foo3()";
    g_callSequence.push_back( "foo3()" );
    g_int1 = i1;
    g_int2 = i2;
}

void GN_STDCALL foo4(int i1, int i2)
{
    g_funcName = "foo4()";
    g_callSequence.push_back( "foo4()" );
    g_int1 = i1;
    g_int2 = i2;
}

void GN_CDECL foo5(int i1, int i2)
{
    g_funcName = "foo5()";
    g_callSequence.push_back( "foo5()" );
    g_int1 = i1;
    g_int2 = i2;
}

struct aaa
{
    virtual ~aaa() {}

    void foo1(int a, int b)
    {
        g_funcName = "aaa::foo1()";
        g_callSequence.push_back( "aaa::foo1()" );
        g_int1 = a;
        g_int2 = b;
    }

    void foo1(int a, int b) const
    {
        g_funcName = "aaa::foo1() const";
        g_callSequence.push_back( "aaa::foo1() const" );
        g_int1 = a;
        g_int2 = b;
    }

    void foo2(int a, int b)
    {
        g_funcName = "aaa::foo2()";
        g_callSequence.push_back( "aaa::foo2()" );
        g_int1 = a;
        g_int2 = b;
    }

    void foo2(int a, int b) const
    {
        g_funcName = "aaa::foo2() const";
        g_callSequence.push_back( "aaa::foo2() const" );
        g_int1 = a;
        g_int2 = b;
    }

    int foo3(int a, int b) const
    {
        g_funcName = "aaa::foo3() const";
        g_callSequence.push_back( "aaa::foo3()" );
        g_int1 = a;
        g_int2 = b;
        return 0;
    }
};

struct bbb : public aaa, public GN::SlotBase
{
    virtual ~bbb() {}

    void foo( int a, int b ) const
    {
        g_funcName = "bbb::foo()";
        g_callSequence.push_back( "bbb::foo()" );
        g_int1 = a;
        g_int2 = b;
    }
};

template<class F1, class F2>
bool equal( const F1 & f1, const F2 & f2 )
{
    return
        f1 == f2 &&
        f2 == f1 &&
        !(f1!=f2) &&
        !(f2!=f1) &&
        !(f1<f2) && !(f2<f1) &&
        !(f1>f2) && !(f2>f1);
}

template<class F1, class F2>
bool unequal( const F1 & f1, const F2 & f2 )
{
    return
        f1 != f2 &&
        f2 != f1 &&
        !(f1==f2) &&
        !(f2==f1) &&
        (f1<f2||f2<f1) &&
        (f1>f2||f2>f1);
}

#define ASSERT_FUNC( NAME, INT1, INT2 ) \
    TS_ASSERT_EQUALS( g_funcName, NAME ); \
    TS_ASSERT_EQUALS( g_int1, INT1 ); \
    TS_ASSERT_EQUALS( g_int2, INT2 );

class FunctorTest : public CxxTest::TestSuite
{
public:

    void testCallConvension()
    {
        GN::Delegate2<void,int,int> f3, f4, f5;
        f3 = GN::makeDelegate( &foo3 );
        f4 = GN::makeDelegate( &foo4 );
        f5 = GN::makeDelegate( &foo5 );
        f3(1,2); ASSERT_FUNC( "foo3()", 1, 2 );
        f4(3,4); ASSERT_FUNC( "foo4()", 3, 4 );
        f5(5,6); ASSERT_FUNC( "foo5()", 5, 6 );
    }

    void testMakeDeletate()
    {
        aaa a;
        const aaa & ca = a;
        GN::Delegate2<void,int,int> f1, f2, f3;
        f1 = GN::makeDelegate( &foo1 );
        f2 = GN::makeDelegate<aaa,aaa,void,int,int>( &a, &aaa::foo1 );
        f3 = GN::makeDelegate<aaa,aaa,void,int,int>( &ca, &aaa::foo1 );

        f1(1,2); ASSERT_FUNC( "foo1()", 1, 2 );
        f2(3,4); ASSERT_FUNC( "aaa::foo1()", 3, 4 );
        f3(5,6); ASSERT_FUNC( "aaa::foo1() const", 5, 6 );
    }

    void testDeleagte()
    {
        aaa a;
        bbb b;
        const aaa & ca = a;
        GN::Delegate2<void,int,int> f1, f2;

        // self comparision, free function
        f1.Bind( &foo1 );
        TS_ASSERT( equal(f1,f1) );
        f1(1,2);
        ASSERT_FUNC( "foo1()", 1, 2 );

        // same free functions
        f2.Bind( &foo1 );
        TS_ASSERT( equal(f1,f2) );
        g_funcName = "";
        f2(3,4);
        ASSERT_FUNC( "foo1()", 3, 4 );

        // different free functions
        f2.Bind( &foo2 );
        TS_ASSERT( unequal(f1,f2) );
        f2(1,2);
        ASSERT_FUNC( "foo2()", 1, 2);

        // free function != member function
        f2.Bind<aaa,aaa>( &a,&aaa::foo1 );
        TS_ASSERT( unequal(f1,f2) );
        f2(3,4);
        ASSERT_FUNC( "aaa::foo1()", 3, 4 );

        // self compare, member function
        f1.Bind<aaa,aaa>( &a,&aaa::foo1 );
        TS_ASSERT( equal(f1,f1) );
        g_funcName = "";
        f1(1,2);
        ASSERT_FUNC( "aaa::foo1()", 1, 2 );

        // same member function
        f2.Bind<aaa,aaa>( &a,&aaa::foo1 );
        TS_ASSERT( equal(f1,f2) );
        f2(1,2);

        // different member function, same class, different constness
        f1.Bind<aaa,aaa>( &ca, &aaa::foo1 );
        TS_ASSERT( unequal(f1,f2) );
        f1(3,4);
        ASSERT_FUNC( "aaa::foo1() const", 3, 4 );

        // different member function, same class, different function
        f2.Bind<aaa,aaa>( &ca, &aaa::foo2 );
        TS_ASSERT( unequal(f1,f2) );
        f2(1,2);
        ASSERT_FUNC( "aaa::foo2() const", 1, 2 );

        f1.Bind<aaa,aaa>( &a, &aaa::foo1 );
        f2.Bind<aaa,aaa>( &ca, &aaa::foo2 );
        TS_ASSERT( unequal(f1,f2) );
        f1(3,4);
        ASSERT_FUNC( "aaa::foo1()", 3, 4 );
        f2(1,2);
        ASSERT_FUNC( "aaa::foo2() const", 1, 2 );

        // different member function, different class
        f1.Bind<aaa,aaa>( &a,&aaa::foo1 );
        f2.Bind<aaa,bbb>( &b,&aaa::foo1 );
        TS_ASSERT( unequal(f1,f2) );
        g_funcName = "";
        f1(1,2);
        ASSERT_FUNC( "aaa::foo1()", 1, 2 );
        g_funcName = "";
        f2(3,4);
        ASSERT_FUNC( "aaa::foo1()", 3, 4 );

#ifdef HAS_BOOST
        // working with boost::Bind
        boost::Bind<void>( f1, 1, 2 )();
        int i = 2;
        g_funcName = "";
        g_int1 = 0;
        g_int2 = 0;
        boost::Bind<void>( f1, _1, _1 )(i);
        ASSERT_FUNC( "aaa::foo1()", 2, 2 );
#endif
    }

    struct F1
    {
        int & ii;
        F1( int & i ) : ii(i) {}
        void operator()() { ii = 1; }
    };
    struct F2
    {
        int & ii;
        F2( int & i ) : ii(i) {}
        void operator()() { ii = 2; }
    };

    void testBindToFunctor()
    {
        int i = 0;
        F1 f1(i);
        F2 f2(i);

        GN::Delegate0<void> d1, d2;

        d1.Bind( f1 );
        d2.Bind( &f2 );

        d1(); // copy_of_f1.opeartor()
        TS_ASSERT_EQUALS( i, 1 );

        d2(); // fp->operator()
        TS_ASSERT_EQUALS( i, 2 );
    }

    void testConstAndNonConst()
    {
        aaa a;
        const aaa & b = a;
        GN::Signal2<void,int,int> s1;
        s1.Connect( &a, &aaa::foo1 );
        s1.Connect<aaa,aaa>( &b, &aaa::foo1 );
        TS_ASSERT_EQUALS( s1.GetNumSlots(), 2 );
        g_callSequence.clear();
        s1(0,1);
        TS_ASSERT_EQUALS( g_callSequence[0], "aaa::foo1()" );
        TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo1() const" );
    }

    void testSigslotToFreeFunction()
    {
        GN::Signal2<void,int,int> s;

        s.Connect( &foo1 );
        TS_ASSERT_EQUALS( s.GetNumSlots(), 1 );
        s.Disconnect( &foo1 );
        TS_ASSERT_EQUALS( s.GetNumSlots(), 0 );
    }

    void testSigslotToMethod()
    {
        aaa a;
        GN::Signal2<void,int,int> s;

        s.Connect( &a, &aaa::foo1 );
        TS_ASSERT_EQUALS( s.GetNumSlots(), 1 );
        s.Disconnect( &a );
        TS_ASSERT_EQUALS( s.GetNumSlots(), 0 );
    }

    void testSigslotToSlotClass()
    {
        bbb b;
        GN::Signal2<void,int,int> s;

        s.Connect( &b, &bbb::foo1 );
        TS_ASSERT_EQUALS( s.GetNumSlots(), 1 );
        s.Disconnect( &b );
        TS_ASSERT_EQUALS( s.GetNumSlots(), 0 );
    }

    void testSigslotAutoDisconnection1()
    {
        GN::Signal2<void,int,int> s;
        {
            bbb b;
            s.Connect( &b, &bbb::foo1 );
            TS_ASSERT_EQUALS( s.GetNumSlots(), 1 );
            TS_ASSERT_EQUALS( b.GetNumSignals(), 1 );
        }
        TS_ASSERT_EQUALS( s.GetNumSlots(), 0 );
    }

    void testSigslotAutoDisconnection2()
    {
        bbb b;
        {
            GN::Signal2<void,int,int> s;
            s.Connect( &b, &bbb::foo1 );
            TS_ASSERT_EQUALS( s.GetNumSlots(), 1 );
            TS_ASSERT_EQUALS( b.GetNumSignals(), 1 );
        }
        TS_ASSERT_EQUALS( b.GetNumSignals(), 0 );
    }

    void testSigslotComplex()
    {
        aaa a;
        GN::Signal2<void,int,int> s1;
        s1.Connect( &a, &aaa::foo1 );
        s1.Connect( &a, &aaa::foo2 );
        s1.Connect( &foo1 );
        s1.Connect( &a, &aaa::foo1 ); // duplicate member function
        s1.Connect( &foo1 );          // duplicate free function
        TS_ASSERT_EQUALS( s1.GetNumSlots(), 3 );
        g_callSequence.clear();
        s1.Emit(0,0);
        TS_ASSERT_EQUALS( g_callSequence.size(), 3 );
        TS_ASSERT_EQUALS( g_callSequence[0], "aaa::foo1()" );
        TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo2()" );
        TS_ASSERT_EQUALS( g_callSequence[2], "foo1()" );
        {
            bbb b;

			s1.Connect( &b, &bbb::foo );
            s1.Connect( &b, &aaa::foo1 );

            TS_ASSERT_EQUALS( s1.GetNumSlots(), 5 );
            TS_ASSERT_EQUALS( b.GetNumSignals(), 2 );

            g_callSequence.clear();
            s1.Emit(1,1);
            TS_ASSERT_EQUALS( g_callSequence.size(), 5 );
            TS_ASSERT_EQUALS( g_callSequence[0], "aaa::foo1()" );
            TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo2()" );
            TS_ASSERT_EQUALS( g_callSequence[4], "aaa::foo1()" );
            TS_ASSERT_EQUALS( g_callSequence[2], "foo1()" );
            TS_ASSERT_EQUALS( g_callSequence[3], "bbb::foo()" );

            {
                GN::Signal2<void,int,int> s2;
                s2.Connect( &b, &bbb::foo );
                s2.Connect( &a, &aaa::foo1 );

                TS_ASSERT_EQUALS( s2.GetNumSlots(), 2 );
                TS_ASSERT_EQUALS( b.GetNumSignals(), 3 );

                g_callSequence.clear();
                s2.Emit(5,5);
                TS_ASSERT_EQUALS( g_callSequence.size(), 2 );
                TS_ASSERT_EQUALS( g_callSequence[0], "bbb::foo()" );
                TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo1()" );
            }

            TS_ASSERT_EQUALS( s1.GetNumSlots(), 5 );
            TS_ASSERT_EQUALS( b.GetNumSignals(), 2 );
        }

        // connection to b should be released automatically.
        TS_ASSERT_EQUALS( s1.GetNumSlots(), 3 );
        g_callSequence.clear();
        s1.Emit(0,0);
        TS_ASSERT_EQUALS( g_callSequence.size(), 3 );
        TS_ASSERT_EQUALS( g_callSequence[0], "aaa::foo1()" );
        TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo2()" );
        TS_ASSERT_EQUALS( g_callSequence[2], "foo1()" );
    }
};

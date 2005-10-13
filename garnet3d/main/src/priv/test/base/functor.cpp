#include "../testCommon.h"
#include <vector>
#ifdef HAS_BOOST
#include <boost/bind.hpp>
#endif

using namespace GN;

std::vector<StrA>   g_callSequence;
StrA                g_funcName;
int                 g_int1 = 0;
int                 g_int2 = 0;

void foo1(int, int)
{
    g_funcName = "foo1()";
    g_callSequence.push_back( "foo1()" );
}

void foo2(int, int)
{
    g_funcName = "foo2()";
    g_callSequence.push_back( "foo2()" );
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

    void foo1(int, int) const
    {
        g_funcName = "aaa::foo1() const";
        g_callSequence.push_back( "aaa::foo1() const" );
    }

    void foo2(int, int)
    {
        g_funcName = "aaa::foo2()";
        g_callSequence.push_back( "aaa::foo2()" );
    }

    void foo2(int, int) const
    {
        g_funcName = "aaa::foo2() const";
        g_callSequence.push_back( "aaa::foo2() const" );
    }

    int foo3(int, int) const
    {
        g_funcName = "aaa::foo3() const";
        g_callSequence.push_back( "aaa::foo3()" );
        return 0;
    }
};

struct bbb : public aaa, public GN::SlotBase
{
    virtual ~bbb() {}

    void foo( int, int ) const
    {
        g_funcName = "bbb::foo()";
        g_callSequence.push_back( "bbb::foo()" );
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

class FunctorTest : public CxxTest::TestSuite
{
public:

    void testMakeFunctor()
    {
        aaa a;
        const aaa & ca = a;
        GN::Functor2<void,int,int> f1, f2, f3;
        f1 = GN::makeFunctor( &foo1 );
        f2 = GN::makeFunctor<aaa,aaa,void,int,int>( &a, &aaa::foo1 );
        f3 = GN::makeFunctor<aaa,aaa,void,int,int>( &ca, &aaa::foo1 );

        f1(0,0); TS_ASSERT_EQUALS( g_funcName, "foo1()" );
        f2(0,0); TS_ASSERT_EQUALS( g_funcName, "aaa::foo1()" );
        f3(0,0); TS_ASSERT_EQUALS( g_funcName, "aaa::foo1() const" );
    }

    void testFunctor()
    {
        aaa a;
        bbb b;
        const aaa & ca = a;
        GN::Functor2<void,int,int> f1, f2;

        // self comparision, free function
        f1.bind( &foo1 );
        TS_ASSERT( equal(f1,f1) );
        f1(1,2);
        TS_ASSERT_EQUALS( g_funcName, "foo1()" );

        // same free functions
        f2.bind( &foo1 );
        TS_ASSERT( equal(f1,f2) );
        g_funcName = "";
        f2(1,2);
        TS_ASSERT_EQUALS( g_funcName, "foo1()");

        // different free functions
        f2.bind( &foo2 );
        TS_ASSERT( unequal(f1,f2) );
        f2(1,2);
        TS_ASSERT_EQUALS( g_funcName, "foo2()");

        // free function != member function
        f2.bind<aaa,aaa>( &a,&aaa::foo1 );
        TS_ASSERT( unequal(f1,f2) );
        f2(1,2);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo1()" );

        // self compare, member function
        f1.bind<aaa,aaa>( &a,&aaa::foo1 );
        TS_ASSERT( equal(f1,f1) );
        g_funcName = "";
        f1(1,2);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo1()" );

        // same member function
        f2.bind<aaa,aaa>( &a,&aaa::foo1 );
        TS_ASSERT( equal(f1,f2) );
        f2(1,2);

        // different member function, same class, different constness
        f1.bind<aaa,aaa>( &ca, &aaa::foo1 );
        TS_ASSERT( unequal(f1,f2) );
        f1(1,2);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo1() const" );

        // different member function, same class, different function
        f2.bind<aaa,aaa>( &ca, &aaa::foo2 );
        TS_ASSERT( unequal(f1,f2) );
        f2(1,2);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo2() const" );

        f1.bind<aaa,aaa>( &a, &aaa::foo1 );
        f2.bind<aaa,aaa>( &ca, &aaa::foo2 );
        TS_ASSERT( unequal(f1,f2) );
        f1(1,2);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo1()" );
        f2(1,2);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo2() const" );

        // different member function, different class
        f1.bind<aaa,aaa>(&a,&aaa::foo1 );
        f2.bind<aaa,bbb>(&b,&aaa::foo1 );
        TS_ASSERT( unequal(f1,f2) );
        g_funcName = "";
        f1(1,2);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo1()" );
        g_funcName = "";
        f2(1,2);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo1()" );

#ifdef HAS_BOOST
        // working with boost::bind
        boost::bind<void>( f1, 1, 2 )();
        int i = 2;
        g_funcName = "";
        g_int1 = 0;
        g_int2 = 0;
        boost::bind<void>( f1, _1, _1 )(i);
        TS_ASSERT_EQUALS( g_funcName, "aaa::foo1()" );
        TS_ASSERT_EQUALS( g_int1, 2 );
        TS_ASSERT_EQUALS( g_int2, 2 );
#endif
    }

    void testConstAndNonConst()
    {
        aaa a;
        const aaa & b = a;
        GN::Signal2<void,int,int> s1;
        s1.connect( &a, &aaa::foo1 );
        s1.connect<aaa,aaa>( &b, &aaa::foo1 );
        TS_ASSERT_EQUALS( s1.getNumSlots(), 2 );
        g_callSequence.clear();
        s1(0,1);
        TS_ASSERT_EQUALS( g_callSequence[0], "aaa::foo1()" );
        TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo1() const" );
    }

    void testSigslotToFreeFunction()
    {
        GN::Signal2<void,int,int> s;

        s.connect( &foo1 );
        TS_ASSERT_EQUALS( s.getNumSlots(), 1 );
        s.disconnect( &foo1 );
        TS_ASSERT_EQUALS( s.getNumSlots(), 0 );
    }

    void testSigslotToMethod()
    {
        aaa a;
        GN::Signal2<void,int,int> s;

        s.connect( &a, &aaa::foo1 );
        TS_ASSERT_EQUALS( s.getNumSlots(), 1 );
        s.disconnect( &a );
        TS_ASSERT_EQUALS( s.getNumSlots(), 0 );
    }

    void testSigslotToSlotClass()
    {
        bbb b;
        GN::Signal2<void,int,int> s;

        s.connect( &b, &bbb::foo1 );
        TS_ASSERT_EQUALS( s.getNumSlots(), 1 );
        s.disconnect( &b );
        TS_ASSERT_EQUALS( s.getNumSlots(), 0 );
    }

    void testSigslotAutoDisconnection1()
    {
        GN::Signal2<void,int,int> s;
        {
            bbb b;
            s.connect( &b, &bbb::foo1 );
            TS_ASSERT_EQUALS( s.getNumSlots(), 1 );
            TS_ASSERT_EQUALS( b.getNumSignals(), 1 );
        }
        TS_ASSERT_EQUALS( s.getNumSlots(), 0 );
    }

    void testSigslotAutoDisconnection2()
    {
        bbb b;
        {
            GN::Signal2<void,int,int> s;
            s.connect( &b, &bbb::foo1 );
            TS_ASSERT_EQUALS( s.getNumSlots(), 1 );
            TS_ASSERT_EQUALS( b.getNumSignals(), 1 );
        }
        TS_ASSERT_EQUALS( b.getNumSignals(), 0 );
    }

    void testSigslotComplex()
    {
        aaa a;
        GN::Signal2<void,int,int> s1;
        s1.connect( &a, &aaa::foo1 );
        s1.connect( &a, &aaa::foo2 );
        s1.connect( &foo1 );
        s1.connect( &a, &aaa::foo1 ); // duplicate member function
        s1.connect( &foo1 );          // duplicate free function
        TS_ASSERT_EQUALS( s1.getNumSlots(), 3 );
        g_callSequence.clear();
        s1.emit(0,0);
        TS_ASSERT_EQUALS( g_callSequence.size(), 3 );
        TS_ASSERT_EQUALS( g_callSequence[0], "aaa::foo1()" );
        TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo2()" );
        TS_ASSERT_EQUALS( g_callSequence[2], "foo1()" );
        {
            bbb b;

			s1.connect( &b, &bbb::foo );
            s1.connect( &b, &aaa::foo1 );

            TS_ASSERT_EQUALS( s1.getNumSlots(), 5 );
            TS_ASSERT_EQUALS( b.getNumSignals(), 2 );

            g_callSequence.clear();
            s1.emit(1,1);
            TS_ASSERT_EQUALS( g_callSequence.size(), 5 );
            TS_ASSERT_EQUALS( g_callSequence[0], "aaa::foo1()" );
            TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo2()" );
            TS_ASSERT_EQUALS( g_callSequence[4], "aaa::foo1()" );
            TS_ASSERT_EQUALS( g_callSequence[2], "foo1()" );
            TS_ASSERT_EQUALS( g_callSequence[3], "bbb::foo()" );

            {
                GN::Signal2<void,int,int> s2;
                s2.connect( &b, &bbb::foo );
                s2.connect( &a, &aaa::foo1 );

                TS_ASSERT_EQUALS( s2.getNumSlots(), 2 );
                TS_ASSERT_EQUALS( b.getNumSignals(), 3 );

                g_callSequence.clear();
                s2.emit(5,5);
                TS_ASSERT_EQUALS( g_callSequence.size(), 2 );
                TS_ASSERT_EQUALS( g_callSequence[0], "bbb::foo()" );
                TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo1()" );
            }

            TS_ASSERT_EQUALS( s1.getNumSlots(), 5 );
            TS_ASSERT_EQUALS( b.getNumSignals(), 2 );
        }

        // connection to b should be released automatically.
        TS_ASSERT_EQUALS( s1.getNumSlots(), 3 );
        g_callSequence.clear();
        s1.emit(0,0);
        TS_ASSERT_EQUALS( g_callSequence.size(), 3 );
        TS_ASSERT_EQUALS( g_callSequence[0], "aaa::foo1()" );
        TS_ASSERT_EQUALS( g_callSequence[1], "aaa::foo2()" );
        TS_ASSERT_EQUALS( g_callSequence[2], "foo1()" );
    }
};

#include "../testCommon.h"

class StackArrayTest : public CxxTest::TestSuite
{
    struct Element
    {
        static int count;  ///< number of Element instances
        static int ctor;   ///< number of calls to ctor
        static int dtor;   ///< number of calls to dtor
        static int cctor;  ///< number of calls to copy constructor
        static int cop;    ///< number of calls to copy operator

        static void clear() { ctor = 0; dtor = 0; cctor = 0; cop = 0; }

        bool constructed;
        int  i;

        Element() : constructed(true)
        {
            ++ctor; ++count;
        }

        Element( int i_ ) : constructed(true), i(i_)
        {
            ++ctor; ++count;
        }

        Element( const Element & o )
            : constructed(true)
            , i(o.i)
        {
            if( !o.constructed )
            {
                GN_UNEXPECTED();
            }

            ++cctor;
            ++count;
        }

        ~Element()
        {
            if( !constructed )
            {
                GN_UNEXPECTED();
            }

            constructed = false;

            ++dtor;
            --count;
        }

        Element & operator=( const Element & o )
        {
            if( !constructed || !o.constructed )
            {
                GN_UNEXPECTED();
            }

            i = o.i;

            ++cop;
            return *this;
        }
    };

public:

    void testAppend()
    {
        using namespace GN;

        StackArray<int,2> a;
        a.append( 1 );
        a.append( 2 );
        a.append( 3 ); // should do nothing
        TS_ASSERT_EQUALS( 2, a.size() );
        TS_ASSERT_EQUALS( 1, a.front() );
        TS_ASSERT_EQUALS( 2, a.back() );

        Element::clear();
        StackArray<Element,2> b;
        TS_ASSERT_EQUALS( 0, Element::count );

        b.append( 1 );
        b.append( 2 );
        b.append( 3 ); // should do nothing
        TS_ASSERT_EQUALS( 2, Element::count );
    }

    void testInsert()
    {
        using namespace GN;
        StackArray<int,4> a;
        a.insert( 0, 1 ); // 1 (insert at head)
        a.insert( 1, 3 ); // 1, 3 (insert at tail)
        a.insert( 1, 2 ); // 1, 2, 3 (insert at middle)
        a.insert( 100, 0 ); // invalid position, should do nothing
        a.insert( 3, 4 ); // 1, 2, 3, 4 (insert at tail)
        a.insert( 4, 5 ); // already full, should do nothing
        TS_ASSERT_EQUALS( 4, a.size() );
        TS_ASSERT_EQUALS( 1, a[0] );
        TS_ASSERT_EQUALS( 2, a[1] );
        TS_ASSERT_EQUALS( 3, a[2] );
        TS_ASSERT_EQUALS( 4, a[3] );
    }

    void testInsert2()
    {
        using namespace GN;
        StackArray<Element,4> a;
        a.insert( 0, 1 ); // 1 (insert at head)
        a.insert( 1, 3 ); // 1, 3 (insert at tail)
        a.insert( 1, 2 ); // 1, 2, 3 (insert at middle)
        a.insert( 100, 0 ); // invalid position, should do nothing
        a.insert( 3, 4 ); // 1, 2, 3, 4 (insert at tail)
        a.insert( 4, 5 ); // already full, should do nothing
        TS_ASSERT_EQUALS( 4, a.size() );
    }

    void testErase()
    {
        using namespace GN;
        StackArray<int,4> a;
        a.append( 1 );
        a.append( 2 );
        a.append( 3 );
        a.append( 4 );

        // erase middle: 1, 2, 4
        a.eraseIdx( 2 );
        TS_ASSERT_EQUALS( 3, a.size() );
        TS_ASSERT_EQUALS( 4, a[2] );

        // erase head: 2, 4
        a.eraseIdx( 0 );
        TS_ASSERT_EQUALS( 2, a.size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // erase tail: 2
        a.eraseIdx( 1 );
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // invalid position
        a.eraseIdx( 1 );
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // erase to empty
        a.eraseIdx( 0 );
        TS_ASSERT_EQUALS( 0, a.size() );

        // already empty
        a.eraseIdx( 0 );
        TS_ASSERT_EQUALS( 0, a.size() );
    }

    void testResize()
    {
        using namespace GN;
        StackArray<int,4> a;

        a.resize( 2 );
        TS_ASSERT_EQUALS( 2, a.size() );

        a.resize( 4 );
        TS_ASSERT_EQUALS( 4, a.size() );

        a.resize( 5 );
        TS_ASSERT_EQUALS( 4, a.size() );

        a.resize( 2 );
        TS_ASSERT_EQUALS( 2, a.size() );
    }
};

int StackArrayTest::Element::count = 0;
int StackArrayTest::Element::ctor  = 0;
int StackArrayTest::Element::dtor  = 0;
int StackArrayTest::Element::cctor = 0;
int StackArrayTest::Element::cop = 0;

class DynaArrayTest : public CxxTest::TestSuite
{
    struct Element
    {
        static int count;  ///< number of Element instances
        static int ctor;   ///< number of calls to ctor
        static int dtor;   ///< number of calls to dtor
        static int cctor;  ///< number of calls to copy constructor
        static int cop;    ///< number of calls to copy operator
        static int mctor;
        static int mop;

        static void clear() { ctor = 0; dtor = 0; cctor = 0; cop = 0; mctor = 0; mop = 0; }

        bool constructed = true;
        int i = 0;

        Element()
        {
            ++ctor;
            ++count;
        }

        Element(int i_) : i(i_)
        {
            ++ctor; ++count;
        }

        Element( const Element & o )
        {
            GN_VERIFY(o.constructed);
            i = o.i;
            ++cctor;
            ++count;
        }

        Element(Element && o)
        {
            GN_VERIFY(o.constructed);
            i = o.i; o.i = 0;
            ++mctor;
            ++count;
        }

        ~Element()
        {
            GN_VERIFY(constructed);
            constructed = false;
            i = 0;
            ++dtor;
            --count;
        }

        Element & operator=( Element && o )
        {
            GN_VERIFY(constructed && o.constructed);
            i = o.i; o.i = 0;
            ++mop;
            return *this;
        }

        Element & operator=( const Element & o )
        {
            GN_VERIFY(constructed && o.constructed);
            i = o.i;
            ++cop;
            return *this;
        }
    };

public:

    void testDefautlCtor()
    {
        using namespace GN;
        DynaArray<Element> a;

        TS_ASSERT( a.empty() );
        TS_ASSERT_EQUALS( (Element*)NULL, a.rawptr() );
        TS_ASSERT_EQUALS( 0, a.size() );
    }

    void testCtorWithCount()
    {
        using namespace GN;

        {
            DynaArray<Element> a(1);
            TS_ASSERT_EQUALS( 1, a.size() );
            TS_ASSERT_EQUALS( 1, Element::count );
        }

        TS_ASSERT_EQUALS( 0, Element::count );
    }

    void testCtorWithPtrAndCount()
    {
        using namespace GN;

        Element::clear();

        Element e[2];
        TS_ASSERT_EQUALS( 2, Element::ctor );
        TS_ASSERT_EQUALS( 0, Element::cctor );

        {
            DynaArray<Element> a( e, 2 );
            TS_ASSERT_EQUALS( 2, Element::cctor );
            TS_ASSERT_EQUALS( 4, Element::count );
            TS_ASSERT_EQUALS( 0, Element::dtor );
        }

        TS_ASSERT_EQUALS( 2, Element::count );
        TS_ASSERT_EQUALS( 2, Element::dtor );
    }

    void testCopyCtor()
    {
        using namespace GN;

        Element::clear();

        DynaArray<Element> a( 2 );

        TS_ASSERT_EQUALS( 2, Element::count );
        TS_ASSERT_EQUALS( 2, Element::ctor );
        TS_ASSERT_EQUALS( 0, Element::cctor );
        TS_ASSERT_EQUALS( 0, Element::dtor );
        TS_ASSERT_EQUALS( 0, Element::cop );

        // 2 cctor for creating new array in b.
        // 2 dtor for deleteing old array in b.
        // 2 copy operators for copy c[0,1] to b[0,1]
        // 1 cctor for copy c[2] to b[2]
        DynaArray<Element> b( a );
        TS_ASSERT_EQUALS( 4, Element::count );
        TS_ASSERT_EQUALS( 2, Element::ctor );
        TS_ASSERT_EQUALS( 2, Element::cctor );
        TS_ASSERT_EQUALS( 0, Element::dtor );
        TS_ASSERT_EQUALS( 0, Element::cop );
    }

    void testCopyOperator()
    {
        using namespace GN;

        Element::clear();

        DynaArray<Element> a( 1 );
        DynaArray<Element> b( 2 );
        DynaArray<Element> c( 3 );
        TS_ASSERT_EQUALS( 6, Element::count );
        TS_ASSERT_EQUALS( 6, Element::ctor );
        TS_ASSERT_EQUALS( 0, Element::cctor );
        TS_ASSERT_EQUALS( 0, Element::dtor );
        TS_ASSERT_EQUALS( 0, Element::cop );

        // 2 cctor for creating new array in b.
        // 2 dtor for deleteing old array in b.
        // 2 copy operators for copy c[0,1] to b[0,1]
        // 1 cctor for copy c[2] to b[2]
        b = c;
        TS_ASSERT_EQUALS( 7, Element::count );
        TS_ASSERT_EQUALS( 6, Element::ctor );
        TS_ASSERT_EQUALS( 1, Element::cctor );
        TS_ASSERT_EQUALS( 2, Element::dtor );
        TS_ASSERT_EQUALS( 2, Element::cop );

        // 2 dtor for b[1] and b[2]
        // 1 copy operator for copy a[0] to b[0]
        b = a;
        TS_ASSERT_EQUALS( 5, Element::count );
        TS_ASSERT_EQUALS( 6, Element::ctor );
        TS_ASSERT_EQUALS( 1, Element::cctor );
        TS_ASSERT_EQUALS( 4, Element::dtor );
        TS_ASSERT_EQUALS( 3, Element::cop );
    }

    void testAppend()
    {
        using namespace GN;
        DynaArray<int> a;

        a.append( 1 );
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 1, a.front() );
        TS_ASSERT_EQUALS( 1, a.back() );

        a.append( 3 );
        TS_ASSERT_EQUALS( 2, a.size() );
        TS_ASSERT_EQUALS( 1, a.front() );
        TS_ASSERT_EQUALS( 3, a.back() );

        DynaArray<int> b;
        b.append( a );
        TS_ASSERT_EQUALS( 2, b.size() );
        TS_ASSERT_EQUALS( 1, b.front() );
        TS_ASSERT_EQUALS( 3, b.back() );
    }

    void testAppend2()
    {
        using namespace GN;
        DynaArray<Element> a;
        Element            b;
        Element            c[2];

        TS_ASSERT_EQUALS( 3, Element::count );

        a.append( b );
        TS_ASSERT_EQUALS( 4, Element::count );

        a.append( c, 2 );
        TS_ASSERT_EQUALS( 6, Element::count );
    }

    void testBeginEnd()
    {
        using namespace GN;
        DynaArray<int> a;

        a.append( 1 );
        a.append( 2 );

        int * b = a.begin();
        int * e = a.end();

        TS_ASSERT_EQUALS( 1, *b );
        TS_ASSERT_EQUALS( 2, e-b );
    }

    void testInsert()
    {
        using namespace GN;
        DynaArray<int> a;

        a.insert( 0, 3 ); // 3
        a.insert( 0, 1 ); // 1, 3
        a.insert( 1, 2 ); // 1, 2, 3
        a.insert( 100, 4 ); // should do nothing
        a.insert( 3, 4 ); // 1, 2, 3, 4
        a.insert( 4, 5 ); // 1, 2, 3, 4, 5

        TS_ASSERT_EQUALS( 5, a.size() );
        TS_ASSERT_EQUALS( 1, a[0] );
        TS_ASSERT_EQUALS( 2, a[1] );
        TS_ASSERT_EQUALS( 3, a[2] );
        TS_ASSERT_EQUALS( 4, a[3] );
        TS_ASSERT_EQUALS( 5, a[4] );
    }

    void testErase()
    {
        using namespace GN;
        DynaArray<int> a;

        a.append( 1 );
        a.append( 2 );
        a.append( 3 );
        a.append( 4 );
        a.append( 5 );

        a.eraseIdx( 100 ); // should do nothing
        TS_ASSERT_EQUALS( 5, a.size() );
        TS_ASSERT_EQUALS( 5, a[4] );

        a.eraseIdx( 4 ); // 1, 2, 3, 4
        TS_ASSERT_EQUALS( 4, a.size() );
        TS_ASSERT_EQUALS( 4, a[3] );

        a.eraseIdx( 3 ); // 1, 2, 3
        TS_ASSERT_EQUALS( 3, a.size() );
        TS_ASSERT_EQUALS( 3, a[2] );

        a.eraseIdx( 1 ); // 1, 3
        TS_ASSERT_EQUALS( 2, a.size() );
        TS_ASSERT_EQUALS( 1, a[0] );
        TS_ASSERT_EQUALS( 3, a[1] );

        a.eraseIdx( 0 ); // 3
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 3, a[0] );

        a.eraseIdx( 0 );
        TS_ASSERT_EQUALS( 0, a.size() );

        a.eraseIdx( 0 ); // this should do nothing
    }

    void testPopBack()
    {
        using namespace GN;
        DynaArray<int> a;
        a.append( 1 );
        a.append( 2 );
        a.popBack();
        TS_ASSERT_EQUALS( 1, a.size() );
        TS_ASSERT_EQUALS( 1, a.back() );

        a.popBack();
        TS_ASSERT( a.empty() );

        a.popBack(); // should do nothing
    }

    void testEraseAndPopback2()
    {
        // make sure erased item is destructed

        using namespace GN;

        Element::clear();

        DynaArray<Element> a( 3 );
        TS_ASSERT_EQUALS( 3, Element::count );
        TS_ASSERT_EQUALS( 3, Element::ctor );
        TS_ASSERT_EQUALS( 0, Element::cctor );
        TS_ASSERT_EQUALS( 0, Element::dtor );
        TS_ASSERT_EQUALS( 0, Element::cop );

        // 2 cop for moving a[1,2] to a[0,1]
        // 1 dtor for a[2]
        a.eraseIdx( 0 );
        TS_ASSERT_EQUALS( 2, Element::count );
        TS_ASSERT_EQUALS( 3, Element::ctor );
        TS_ASSERT_EQUALS( 0, Element::cctor );
        TS_ASSERT_EQUALS( 1, Element::dtor );
        TS_ASSERT_EQUALS( 2, Element::cop );

        // 1 dtor for a[1]
        a.popBack();
        TS_ASSERT_EQUALS( 1, Element::count );
        TS_ASSERT_EQUALS( 3, Element::ctor );
        TS_ASSERT_EQUALS( 0, Element::cctor );
        TS_ASSERT_EQUALS( 2, Element::dtor );
        TS_ASSERT_EQUALS( 2, Element::cop );
    }

    void testResize()
    {
        // make sure erased item is destructed
        using namespace GN;
        Element::clear();

        DynaArray<Element> a( 2 );
        TS_ASSERT_EQUALS( 2, Element::count );

        a.resize( 3 );
        TS_ASSERT_EQUALS( 3, Element::count );

        a.resize( 4 );
        TS_ASSERT_EQUALS( 4, Element::count );

        a.resize( 1 );
        TS_ASSERT_EQUALS( 1, Element::count );
    }

    void testMove()
    {
        // make sure erased item is destructed
        using namespace GN;
        Element::clear();

        DynaArray<Element> a;
        a.append({ 2 });
        TS_ASSERT_EQUALS(1, Element::count);
        TS_ASSERT_EQUALS(1, Element::mctor);

        DynaArray<Element> b = std::move(a);
        TS_ASSERT_EQUALS(0, a.size());
        TS_ASSERT_EQUALS(1, b.size());
        TS_ASSERT_EQUALS(1, Element::count);
        TS_ASSERT_EQUALS(2, Element::mop);
    }

    void testReserve()
    {
        using namespace GN;
        Element::clear();

        {

            DynaArray<Element> a;

            a.resize( 2 );
            TS_ASSERT_EQUALS( 2, a.size() );
            TS_ASSERT_EQUALS( 2, Element::count );
            TS_ASSERT_EQUALS( 2, Element::ctor );

            // reserve should not modify array size
            a.reserve( 1000 );
            TS_ASSERT_EQUALS( 2, a.size() );
            TS_ASSERT_EQUALS( 2, Element::count );
            TS_ASSERT_EQUALS( 2, Element::ctor );
            TS_ASSERT_EQUALS( 2, Element::cctor );
            TS_ASSERT_EQUALS( 2, Element::dtor );
        }

        TS_ASSERT_EQUALS( 0, Element::count );
        TS_ASSERT_EQUALS( 4, Element::dtor );
    }
};

int DynaArrayTest::Element::count = 0;
int DynaArrayTest::Element::ctor  = 0;
int DynaArrayTest::Element::dtor  = 0;
int DynaArrayTest::Element::cctor = 0;
int DynaArrayTest::Element::cop = 0;
int DynaArrayTest::Element::mctor = 0;
int DynaArrayTest::Element::mop = 0;

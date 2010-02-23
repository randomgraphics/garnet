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

        static void Clear() { ctor = 0; dtor = 0; cctor = 0; cop = 0; }

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
        a.Append( 1 );
        a.Append( 2 );
        a.Append( 3 ); // should do nothing
        TS_ASSERT_EQUALS( 2, a.Size() );
        TS_ASSERT_EQUALS( 1, a.Front() );
        TS_ASSERT_EQUALS( 2, a.Back() );

        Element::Clear();
        StackArray<Element,2> b;
        TS_ASSERT_EQUALS( 0, Element::count );

        b.Append( 1 );
        b.Append( 2 );
        b.Append( 3 ); // should do nothing
        TS_ASSERT_EQUALS( 2, Element::count );
    }

    void testInsert()
    {
        using namespace GN;
        StackArray<int,4> a;
        a.Insert( 0, 1 ); // 1 (insert at head)
        a.Insert( 1, 3 ); // 1, 3 (insert at tail)
        a.Insert( 1, 2 ); // 1, 2, 3 (insert at middle)
        a.Insert( 100, 0 ); // invalid position, should do nothing
        a.Insert( 3, 4 ); // 1, 2, 3, 4 (insert at tail)
        a.Insert( 4, 5 ); // already full, should do nothing
        TS_ASSERT_EQUALS( 4, a.Size() );
        TS_ASSERT_EQUALS( 1, a[0] );
        TS_ASSERT_EQUALS( 2, a[1] );
        TS_ASSERT_EQUALS( 3, a[2] );
        TS_ASSERT_EQUALS( 4, a[3] );
    }

    void testInsert2()
    {
        using namespace GN;
        StackArray<Element,4> a;
        a.Insert( 0, 1 ); // 1 (insert at head)
        a.Insert( 1, 3 ); // 1, 3 (insert at tail)
        a.Insert( 1, 2 ); // 1, 2, 3 (insert at middle)
        a.Insert( 100, 0 ); // invalid position, should do nothing
        a.Insert( 3, 4 ); // 1, 2, 3, 4 (insert at tail)
        a.Insert( 4, 5 ); // already full, should do nothing
        TS_ASSERT_EQUALS( 4, a.Size() );
    }

    void testErase()
    {
        using namespace GN;
        StackArray<int,4> a;
        a.Append( 1 );
        a.Append( 2 );
        a.Append( 3 );
        a.Append( 4 );

        // erase middle: 1, 2, 4
        a.Erase( 2 );
        TS_ASSERT_EQUALS( 3, a.Size() );
        TS_ASSERT_EQUALS( 4, a[2] );

        // erase head: 2, 4
        a.Erase( 0 );
        TS_ASSERT_EQUALS( 2, a.Size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // erase tail: 2
        a.Erase( 1 );
        TS_ASSERT_EQUALS( 1, a.Size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // invalid position
        a.Erase( 1 );
        TS_ASSERT_EQUALS( 1, a.Size() );
        TS_ASSERT_EQUALS( 2, a[0] );

        // erase to empty
        a.Erase( 0 );
        TS_ASSERT_EQUALS( 0, a.Size() );

        // already empty
        a.Erase( 0 );
        TS_ASSERT_EQUALS( 0, a.Size() );
    }

    void testResize()
    {
        using namespace GN;
        StackArray<int,4> a;

        a.Resize( 2 );
        TS_ASSERT_EQUALS( 2, a.Size() );

        a.Resize( 4 );
        TS_ASSERT_EQUALS( 4, a.Size() );

        a.Resize( 5 );
        TS_ASSERT_EQUALS( 4, a.Size() );

        a.Resize( 2 );
        TS_ASSERT_EQUALS( 2, a.Size() );
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

        static void Clear() { ctor = 0; dtor = 0; cctor = 0; cop = 0; }

        bool constructed;

        Element() : constructed(true)
        {
            ++ctor; ++count;
        }

        Element( const Element & o )
            :constructed(true)
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

        Element & operator=( Element & o )
        {
            if( !constructed || !o.constructed )
            {
                GN_UNEXPECTED();
            }

            ++cop;
            return *this;
        }
    };

public:

    void testDefautlCtor()
    {
        using namespace GN;
        DynaArray<Element> a;

        TS_ASSERT( a.Empty() );
        TS_ASSERT_EQUALS( (Element*)NULL, a.GetRawPtr() );
        TS_ASSERT_EQUALS( 0, a.Size() );
    }

    void testCtorWithCount()
    {
        using namespace GN;

        {
            DynaArray<Element> a(1);
            TS_ASSERT_EQUALS( 1, a.Size() );
            TS_ASSERT_EQUALS( 1, Element::count );
        }

        TS_ASSERT_EQUALS( 0, Element::count );
    }

    void testCtorWithPtrAndCount()
    {
        using namespace GN;

        Element::Clear();

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

        Element::Clear();

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

        Element::Clear();

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
        TS_ASSERT_EQUALS( 3, Element::cctor );
        TS_ASSERT_EQUALS( 2, Element::dtor );
        TS_ASSERT_EQUALS( 2, Element::cop );

        // 2 dtor for b[1] and b[2]
        // 1 copy operator for copy a[0] to b[0]
        b = a;
        TS_ASSERT_EQUALS( 5, Element::count );
        TS_ASSERT_EQUALS( 6, Element::ctor );
        TS_ASSERT_EQUALS( 3, Element::cctor );
        TS_ASSERT_EQUALS( 4, Element::dtor );
        TS_ASSERT_EQUALS( 3, Element::cop );
    }

    void testAppend()
    {
        using namespace GN;
        DynaArray<int> a;

        a.Append( 1 );
        TS_ASSERT_EQUALS( 1, a.Size() );
        TS_ASSERT_EQUALS( 1, a.Front() );
        TS_ASSERT_EQUALS( 1, a.Back() );

        a.Append( 3 );
        TS_ASSERT_EQUALS( 2, a.Size() );
        TS_ASSERT_EQUALS( 1, a.Front() );
        TS_ASSERT_EQUALS( 3, a.Back() );

        DynaArray<int> b;
        b.Append( a );
        TS_ASSERT_EQUALS( 2, b.Size() );
        TS_ASSERT_EQUALS( 1, b.Front() );
        TS_ASSERT_EQUALS( 3, b.Back() );
    }

    void testAppend2()
    {
        using namespace GN;
        DynaArray<Element> a;
        Element            b;
        Element            c[2];

        TS_ASSERT_EQUALS( 3, Element::count );

        a.Append( b );
        TS_ASSERT_EQUALS( 4, Element::count );

        a.Append( c, 2 );
        TS_ASSERT_EQUALS( 6, Element::count );
    }

    void testBeginEnd()
    {
        using namespace GN;
        DynaArray<int> a;

        a.Append( 1 );
        a.Append( 2 );

        int * b = a.Begin();
        int * e = a.End();

        TS_ASSERT_EQUALS( 1, *b );
        TS_ASSERT_EQUALS( 2, e-b );
    }

    void testInsert()
    {
        using namespace GN;
        DynaArray<int> a;

        a.Insert( 0, 3 ); // 3
        a.Insert( 0, 1 ); // 1, 3
        a.Insert( 1, 2 ); // 1, 2, 3
        a.Insert( 100, 4 ); // should do nothing
        a.Insert( 3, 4 ); // 1, 2, 3, 4
        a.Insert( 4, 5 ); // 1, 2, 3, 4, 5

        TS_ASSERT_EQUALS( 5, a.Size() );
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

        a.Append( 1 );
        a.Append( 2 );
        a.Append( 3 );
        a.Append( 4 );
        a.Append( 5 );

        a.Erase( 100 ); // should do nothing
        TS_ASSERT_EQUALS( 5, a.Size() );
        TS_ASSERT_EQUALS( 5, a[4] );

        a.Erase( 4 ); // 1, 2, 3, 4
        TS_ASSERT_EQUALS( 4, a.Size() );
        TS_ASSERT_EQUALS( 4, a[3] );

        a.Erase( 3 ); // 1, 2, 3
        TS_ASSERT_EQUALS( 3, a.Size() );
        TS_ASSERT_EQUALS( 3, a[2] );

        a.Erase( 1 ); // 1, 3
        TS_ASSERT_EQUALS( 2, a.Size() );
        TS_ASSERT_EQUALS( 1, a[0] );
        TS_ASSERT_EQUALS( 3, a[1] );

        a.Erase( 0 ); // 3
        TS_ASSERT_EQUALS( 1, a.Size() );
        TS_ASSERT_EQUALS( 3, a[0] );

        a.Erase( 0 );
        TS_ASSERT_EQUALS( 0, a.Size() );

        a.Erase( 0 ); // this should do nothing
    }

    void testPopBack()
    {
        using namespace GN;
        DynaArray<int> a;
        a.Append( 1 );
        a.Append( 2 );
        a.PopBack();
        TS_ASSERT_EQUALS( 1, a.Size() );
        TS_ASSERT_EQUALS( 1, a.Back() );

        a.PopBack();
        TS_ASSERT( a.Empty() );

        a.PopBack(); // should do nothing
    }

    void testEraseAndPopback2()
    {
        // make sure erased item is destructed

        using namespace GN;

        Element::Clear();

        DynaArray<Element> a( 3 );
        TS_ASSERT_EQUALS( 3, Element::count );
        TS_ASSERT_EQUALS( 3, Element::ctor );
        TS_ASSERT_EQUALS( 0, Element::cctor );
        TS_ASSERT_EQUALS( 0, Element::dtor );
        TS_ASSERT_EQUALS( 0, Element::cop );

        // 2 cop for moving a[1,2] to a[0,1]
        // 1 dtor for a[2]
        a.Erase( 0 );
        TS_ASSERT_EQUALS( 2, Element::count );
        TS_ASSERT_EQUALS( 3, Element::ctor );
        TS_ASSERT_EQUALS( 0, Element::cctor );
        TS_ASSERT_EQUALS( 1, Element::dtor );
        TS_ASSERT_EQUALS( 2, Element::cop );

        // 1 dtor for a[1]
        a.PopBack();
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
        Element::Clear();

        DynaArray<Element> a( 2 );
        TS_ASSERT_EQUALS( 2, Element::count );

        a.Resize( 3 );
        TS_ASSERT_EQUALS( 3, Element::count );

        a.Resize( 4 );
        TS_ASSERT_EQUALS( 4, Element::count );

        a.Resize( 1 );
        TS_ASSERT_EQUALS( 1, Element::count );
    }

    void testReserve()
    {
        using namespace GN;
        Element::Clear();

        {

            DynaArray<Element> a;

            a.Resize( 2 );
            TS_ASSERT_EQUALS( 2, a.Size() );
            TS_ASSERT_EQUALS( 2, Element::count );
            TS_ASSERT_EQUALS( 2, Element::ctor );

            // reserve should not modify array size
            a.Reserve( 1000 );
            TS_ASSERT_EQUALS( 2, a.Size() );
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

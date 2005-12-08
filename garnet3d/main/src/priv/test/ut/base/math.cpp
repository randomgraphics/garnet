#include "../testCommon.h"

template<class T>
bool equal( const T & a, const T & b )
{
    return
        a==b &&
        !(a!=b) &&
        !(a<b) &&
        !(b<a);
}

template<class T>
bool not_equal( const T & a, const T & b )
{
    return
        !(a==b) &&
        a!=b &&
        ( (a<b) || (b<a) );
}

template<class T>
void vector_tests( T a , T b )
{
    TS_ASSERT(a!=b);

    // ctor
    T c(a);
    TS_ASSERT( equal(c,a) );

    // assign
    c=b;
    TS_ASSERT( equal(c,b) );

    // add/dec
    TS_ASSERT( equal(a+b,b+a) );
    c = a+b;
    TS_ASSERT( equal(c-a,b) );

    c = a;
    c += b;
    TS_ASSERT( equal(c-a,b) );
    c = a + b;
    c -= a;
    TS_ASSERT( equal(c,b) );
}

class MathTest : public CxxTest::TestSuite
{
public:
    void testVector()
    {
        GN::Vector2<int> a2(rand(),rand()), b2(rand(),rand());
        GN::Vector3<int> a3(rand(),rand(),rand()), b3(rand(),rand(),rand());
        GN::Vector4<int> a4(rand(),rand(),rand(),rand()), b4(rand(),rand(),rand(),rand());

        vector_tests(a2,b2);
        vector_tests(a3,b3);
        vector_tests(a4,b4);
    }
};

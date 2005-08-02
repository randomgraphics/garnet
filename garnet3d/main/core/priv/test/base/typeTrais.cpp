#include "../testCommon.h"

class a : public GN::NoCopy { protected: virtual ~a() {}  };
class b : public a {};
class c {};
class d : public c, public b {};
typedef const d cd;

class TypeTraitsTest : public CxxTest::TestSuite
{
public:

    void testIsConst()
    {
        GN_CASSERT( !GN::IsConst<int>::value );
        GN_CASSERT( GN::IsConst<const int>::value );
        GN_CASSERT( !GN::IsConst<const int *>::value );
        GN_CASSERT( GN::IsConst<const int * const>::value );
        GN_CASSERT( !GN::IsConst<d>::value );
        GN_CASSERT( GN::IsConst<cd>::value );
    }

    void testIsClass()
    {
        GN_CASSERT( !GN::IsClass<int>::value );
        GN_CASSERT( GN::IsClass<a>::value );
    }

    void testIsConvertible()
    {
        GN_CASSERT( (GN::IsConvertible<d,a>::value) );
        GN_CASSERT( (GN::IsConvertible<const d,a>::value) );
        GN_CASSERT( (GN::IsConvertible<d&,a>::value) );
        GN_CASSERT( (GN::IsConvertible<const d&,a>::value) );

        GN_CASSERT( (GN::IsConvertible<d,a&>::value) );
        GN_CASSERT( !(GN::IsConvertible<const d,a&>::value) );
        GN_CASSERT( (GN::IsConvertible<d&,a&>::value) );
        GN_CASSERT( !(GN::IsConvertible<const d&,a&>::value) );

        GN_CASSERT( (GN::IsConvertible<d,const a>::value) );
        GN_CASSERT( (GN::IsConvertible<const d,const a>::value) );
        GN_CASSERT( (GN::IsConvertible<d&,const a>::value) );
        GN_CASSERT( (GN::IsConvertible<const d&,const a>::value) );

        GN_CASSERT( (GN::IsConvertible<d,const a&>::value) );
        GN_CASSERT( (GN::IsConvertible<const d,const a&>::value) );
        GN_CASSERT( (GN::IsConvertible<d&,const a&>::value) );
        GN_CASSERT( (GN::IsConvertible<const d&,const a&>::value) );
    }

    void testIsBaseAndDerived()
    {
        GN_CASSERT( (GN::IsBaseAndDerived<a,b>::value) );
        GN_CASSERT( (GN::IsBaseAndDerived<a,d>::value) );
        GN_CASSERT( !(GN::IsBaseAndDerived<a,c>::value) );
    }
};

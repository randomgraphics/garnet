#include "../testCommon.h"

#define GN_DEFINE_SMART_ENUM( ENUM_CLASS, ENUM_TYPE ) \
    private : \
        ENUM_TYPE mValue; \
    public: \
        ENUM_CLASS() \
        { \
        } \
        ENUM_CLASS( ENUM_TYPE e ) : mValue(e) {}  \
        operator ENUM_TYPE &() { return mValue; }

class SmartEnumTest : public CxxTest::TestSuite
{
    struct MyEnum
    {
        enum Enum
        {
            A = 1,
            B = 2,
            C = 4,
        };

        static const char * sToString( Enum )
        {
            return 0;
        }

        static bool sFromString( Enum &, const char * )
        {
            return false;
        }

        GN_DEFINE_SMART_ENUM( MyEnum, Enum )
    };

public:

    void test1()
    {
        MyEnum e;

        e = MyEnum::A;
        e = MyEnum::C;
        e = MyEnum::B;

        TS_ASSERT_DIFFERS( e, MyEnum::A );
        TS_ASSERT_DIFFERS( e, MyEnum::C );
        TS_ASSERT_EQUALS( e, MyEnum::B );

        TS_ASSERT( MyEnum::A <  e );
        TS_ASSERT( MyEnum::A <= e );
        TS_ASSERT( MyEnum::A != e );
        TS_ASSERT( MyEnum::B == e );
        TS_ASSERT( MyEnum::C >  e );
        TS_ASSERT( MyEnum::C >= e );

        TS_ASSERT( e >  MyEnum::A);
        TS_ASSERT( e >= MyEnum::A);
        TS_ASSERT( e != MyEnum::A);
        TS_ASSERT( e == MyEnum::B);
        TS_ASSERT( e <  MyEnum::C);
        TS_ASSERT( e <= MyEnum::C);

        //MyEnum::toStr(E1);
        //e.fromStr( "E1" );
        //e = MyEnum::sGetAt(0);
    }

    void testSwitchCase()
    {
        MyEnum e;

        e = MyEnum::A;

        switch( e )
        {
            case MyEnum::A:
                break;

            default :
                break;
        }
    }
};

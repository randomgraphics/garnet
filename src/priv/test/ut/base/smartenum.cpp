#include "../testCommon.h"

namespace GN
{
    template<typename ENUM>
    struct EnumItemDesc
    {
        ENUM value;
        const char * name;
        const char * desc;
    };

    template<typename ENUM>
    class SmartEnum
    {
        ENUM mValue;

    public:

        static EnumItemDesc<ENUM> mProp[];

        SmartEnum()
        {
        }

        SmartEnum & operator=( ENUM e ) { mValue = e; return *this; }

        static const char * toStr( ENUM, const char * = 0 )
        {
            return mProp[0].name;
        };

        ENUM fromStr( const char * ) const
        {
            return (ENUM)0;
        }
    };
}

GN::EnumItemDesc<MyEnumHaha> MyEnumDesc[] =
{
    { E1, "E1", "enum1" },
    { E2, "E2", "enum2" },
    { E3, "E3", "enum3" },
};

typedef GN::SmartEnum< MyEnumHaha > MyEnum;

class SmartEnumTest : public CxxTest::TestSuite
{
public:
    void test1()
    {
        MyEnum e;

        e = E1;
        //e = E2;
        //e = E3;

        MyEnum::toStr(E1);
        //e.fromStr( "E1" );
        //e = MyEnum::sGetAt(0);
    }
};

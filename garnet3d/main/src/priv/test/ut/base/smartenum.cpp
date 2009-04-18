#include "../testCommon.h"

namespace GN
{
    template<class ENUM_CLASS>
    struct SmartEnum : public ENUM_CLASS
    {
        // *********************************************************************
        // public types
        // *********************************************************************

        //@{

        typedef typename ENUM_CLASS::Enum ENUM_TYPE;

        //@}

        // *********************************************************************
        // public methods
        // *********************************************************************

        //@{

        /// constructor
        SmartEnum()
        {
        }

        /// construct from C enumeration
        SmartEnum( ENUM_TYPE e ) : mValue(e) {}

        //@}

        // *********************************************************************
        // Overloaded operators
        // *********************************************************************

        //@{

        SmartEnum operator=( ENUM_TYPE e )
        {
            mValue = e;
            return *this;
        }

        bool operator<( const SmartEnum & rhs ) const
        {
            return mValue < rhs.mValue;
        }

        bool operator<=( const SmartEnum & rhs ) const
        {
            return mValue <= rhs.mValue;
        }

        bool operator==( const SmartEnum & rhs ) const
        {
            return mValue == rhs.mValue;
        }

        bool operator!=( const SmartEnum & rhs ) const
        {
            return mValue != rhs.mValue;
        }

        bool operator>( const SmartEnum & rhs ) const
        {
            return mValue > rhs.mValue;
        }

        bool operator>=( const SmartEnum & rhs ) const
        {
            return mValue >= rhs.mValue;
        }

        friend bool operator<( const ENUM_TYPE & a, const SmartEnum & b )
        {
            return a < b.mValue;
        }

        friend bool operator<=( const ENUM_TYPE & a, const SmartEnum & b )
        {
            return a <= b.mValue;
        }

        friend bool operator==( const ENUM_TYPE & a, const SmartEnum & b )
        {
            return a == b.mValue;
        }

        friend bool operator!=( const ENUM_TYPE & a, const SmartEnum & b )
        {
            return a != b.mValue;
        }

        friend bool operator>( const ENUM_TYPE & a, const SmartEnum & b )
        {
            return a > b.mValue;
        }

        friend bool operator>=( const ENUM_TYPE & a, const SmartEnum & b )
        {
            return a >= b.mValue;
        }

        //@}

        // *********************************************************************
        // private data
        // *********************************************************************

    private:

        /// the enumeration value.
        ENUM_TYPE mValue;
    };

}

class SmartEnumTest : public CxxTest::TestSuite
{
    struct MyEnumDesc
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
    };

    typedef GN::SmartEnum<MyEnumDesc> MyEnum;

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
};

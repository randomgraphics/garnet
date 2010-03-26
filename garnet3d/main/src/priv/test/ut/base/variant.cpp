#include "../testCommon.h"

class VariantTest : public CxxTest::TestSuite
{
public:

    void testCtor()
    {
        {
            GN::Variant var(true);
            bool v = false;
            TS_ASSERT( var.GetB(v) ); TS_ASSERT_EQUALS( true, v );
        }
        {
            GN::Variant var(1);
            int v = 0;
            TS_ASSERT( var.GetI(v) ); TS_ASSERT_EQUALS( 1, v );
        }
        {
            GN::Variant var(1.0f);
            float v = 0.0f;
            TS_ASSERT( var.GetF(v) ); TS_ASSERT_EQUALS( 1.0f, v );
        }
        {
            GN::Variant var((void*)NULL);
            void * v = &var; // initialize to any non-NULL value.
            TS_ASSERT( var.GetP(v) ); TS_ASSERT_EQUALS( (void*)NULL, v );
        }
        {
            GN::Variant var(GN::StrA("a"));
            GN::StrA v = var.GetS();
            TS_ASSERT_EQUALS( "a", v );
        }
        {
            GN::Variant var(GN::Vector4f(1.0f,2.0f,3.0f,4.0f));
            GN::Vector4f v;
            TS_ASSERT( var.GetV(v) );
            TS_ASSERT_EQUALS( 1.0f, v.x );
            TS_ASSERT_EQUALS( 2.0f, v.y );
            TS_ASSERT_EQUALS( 3.0f, v.z );
            TS_ASSERT_EQUALS( 4.0f, v.w );
        }
    }

    void testFromInvalid()
    {
        GN::Variant var;
        bool          b;
        int           i;
        float         f;
        void *        p;
        GN::Vector4f  v;
        GN::Matrix44f m;

        TS_ASSERT( !var.GetB(b) );
        TS_ASSERT( !var.GetI(i) );
        TS_ASSERT( !var.GetF(f) );
        TS_ASSERT( !var.GetP(p) );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );
    }

    void testFromBool()
    {
        GN::Variant var;
        GN::StrA      s;
        bool          b = false;
        int           i = 0;
        float         f = 0.0f;
        void *        p = NULL;
        GN::Vector4f  v;
        GN::Matrix44f m;

        var.SetB(true);

        s = var.GetS(); TS_ASSERT_EQUALS( "1", s );
        TS_ASSERT( var.GetB(b) ); TS_ASSERT_EQUALS( true, b );
        TS_ASSERT( var.GetI(i) ); TS_ASSERT_EQUALS( 1, i );
        TS_ASSERT( var.GetF(f) ); TS_ASSERT_EQUALS( 1.0f, f );
        TS_ASSERT( var.GetP(p) ); TS_ASSERT_EQUALS( (void*)1, p );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );

        var.SetB(false);

        s = var.GetS(); TS_ASSERT_EQUALS( "0", s );
        TS_ASSERT( var.GetB(b) ); TS_ASSERT_EQUALS( false, b );
        TS_ASSERT( var.GetI(i) ); TS_ASSERT_EQUALS( 0, i );
        TS_ASSERT( var.GetF(f) ); TS_ASSERT_EQUALS( 0.0f, f );
        TS_ASSERT( var.GetP(p) ); TS_ASSERT_EQUALS( (void*)0, p );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );
    }

    void testFromInt()
    {
        GN::Variant var;
        GN::StrA      s;
        bool          b = false;
        int           i = 0;
        float         f = 0.0f;
        void *        p = NULL;
        GN::Vector4f  v;
        GN::Matrix44f m;

        var.SetI(1);

        s = var.GetS(); TS_ASSERT_EQUALS( "1", s );
        TS_ASSERT( var.GetB(b) ); TS_ASSERT_EQUALS( true, b );
        TS_ASSERT( var.GetI(i) ); TS_ASSERT_EQUALS( 1, i );
        TS_ASSERT( var.GetF(f) ); TS_ASSERT_EQUALS( 1.0f, f );
        TS_ASSERT( var.GetP(p) ); TS_ASSERT_EQUALS( (void*)1, p );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );

        var.SetI(0);

        s = var.GetS(); TS_ASSERT_EQUALS( "0", s );
        TS_ASSERT( var.GetB(b) ); TS_ASSERT_EQUALS( false, b );
        TS_ASSERT( var.GetI(i) ); TS_ASSERT_EQUALS( 0, i );
        TS_ASSERT( var.GetF(f) ); TS_ASSERT_EQUALS( 0.0f, f );
        TS_ASSERT( var.GetP(p) ); TS_ASSERT_EQUALS( (void*)0, p );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );
    }

    void testFromFloat()
    {
        GN::Variant var;
        GN::StrA      s;
        bool          b = false;
        int           i = 0;
        float         f = 0.0f;
        void *        p = NULL;
        GN::Vector4f  v;
        GN::Matrix44f m;

        var.SetF(1.123f);

        s = var.GetS(); TS_ASSERT_EQUALS( "1.123000", s );
        TS_ASSERT( var.GetB(b) ); TS_ASSERT_EQUALS( true, b );
        TS_ASSERT( var.GetI(i) ); TS_ASSERT_EQUALS( 1, i );
        TS_ASSERT( var.GetF(f) ); TS_ASSERT_EQUALS( 1.123f, f );
        TS_ASSERT( var.GetP(p) ); TS_ASSERT_EQUALS( (void*)1, p );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );

        var.SetF(0.0f);

        s = var.GetS(); TS_ASSERT_EQUALS( "0.000000", s );
        TS_ASSERT( var.GetB(b) ); TS_ASSERT_EQUALS( false, b );
        TS_ASSERT( var.GetI(i) ); TS_ASSERT_EQUALS( 0, i );
        TS_ASSERT( var.GetF(f) ); TS_ASSERT_EQUALS( 0.0f, f );
        TS_ASSERT( var.GetP(p) ); TS_ASSERT_EQUALS( (void*)0, p );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );
    }

    void testFromString()
    {
        GN::Variant var;
        GN::StrA      s;
        bool          b = false;
        int           i = 0;
        float         f = 0.0f;
        void *        p = NULL;
        GN::Vector4f  v;
        GN::Matrix44f m;

        var.SetS("1");

        s = var.GetS(); TS_ASSERT_EQUALS( "1", s );
        TS_ASSERT( var.GetB(b) ); TS_ASSERT_EQUALS( true, b );
        TS_ASSERT( var.GetI(i) ); TS_ASSERT_EQUALS( 1, i );
        TS_ASSERT( var.GetF(f) ); TS_ASSERT_EQUALS( 1.0f, f );
        TS_ASSERT( var.GetP(p) ); TS_ASSERT_EQUALS( (void*)1, p );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );

        var.SetS("0");

        s = var.GetS(); TS_ASSERT_EQUALS( "0", s );
        TS_ASSERT( var.GetB(b) ); TS_ASSERT_EQUALS( false, b );
        TS_ASSERT( var.GetI(i) ); TS_ASSERT_EQUALS( 0, i );
        TS_ASSERT( var.GetF(f) ); TS_ASSERT_EQUALS( 0.0f, f );
        TS_ASSERT( var.GetP(p) ); TS_ASSERT_EQUALS( (void*)0, p );
        TS_ASSERT( !var.GetV(v) );
        TS_ASSERT( !var.GetM(m) );

        var.SetS("1,2,3,4");
        TS_ASSERT( var.GetV(v) );
        TS_ASSERT_EQUALS( 1.0f, v.x );
        TS_ASSERT_EQUALS( 2.0f, v.y );
        TS_ASSERT_EQUALS( 3.0f, v.z );
        TS_ASSERT_EQUALS( 4.0f, v.w );
    }

};

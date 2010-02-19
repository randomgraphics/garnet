#include "../testCommon.h"

class EnvTest : public CxxTest::TestSuite
{
public:

    void testGetEnv()
    {
#if !GN_XENON
        TS_ASSERT( GN::GetEnv(0).Empty() );
        TS_ASSERT( GN::GetEnv("asdfhasdf aslfjoursw").Empty() );
        TS_ASSERT( !GN::GetEnv("PATH").Empty() );
#endif
    }

    void testPutEnv()
    {
#if !GN_XENON
        // invalid arguments
        GN::PutEnv( 0, 0 );
        GN::PutEnv( 0, "" );
        GN::PutEnv( "", 0 );
        GN::PutEnv( "", "" );

        const char * strangeEnv = "asdfhiarnsauraslascfh";

        // make sure strangeEnv do not exist
        TS_ASSERT( GN::GetEnv(strangeEnv).Empty() );

        // set it to "1"
        GN::PutEnv( strangeEnv, "1" );
        TS_ASSERT_EQUALS( GN::GetEnv(strangeEnv), "1" );

        // then clear it
        GN::PutEnv( strangeEnv, 0 );
        TS_ASSERT( GN::GetEnv(strangeEnv).Empty() );

        // set it to "2"
        GN::PutEnv( strangeEnv, "2" );
        TS_ASSERT_EQUALS( GN::GetEnv(strangeEnv), "2" );

        // then clear it in another way.
        GN::PutEnv( strangeEnv, "" );
        TS_ASSERT( GN::GetEnv(strangeEnv).Empty() );
#endif
    }

    void testGetEnvBoolean()
    {
#if !GN_XENON
        const char * strangeEnv = "testGetEnvBoolean_asdfyawnwauiynasiur";

        // make sure strangeEnv do not exist
        TS_ASSERT( GN::GetEnv(strangeEnv).Empty() );

        const char * yes[] = {
            "1",
            "true", "TRUE", "True", "tRue",
            "yes", "Yes", "YES", "yEs" };

        const char * no [] = { 0, "", "0", "2", "no", "NO", "No", "soiuran" };

        for( size_t j = 0; j < sizeof(no)/sizeof(no[0]); ++j )
        for( size_t i = 0; i < sizeof(yes)/sizeof(yes[0]); ++i )
        {
            TS_ASSERT( !GN::GetEnvBoolean(strangeEnv) );
            GN::PutEnv( strangeEnv, yes[i] );
            TS_ASSERT( GN::GetEnvBoolean(strangeEnv) );
            GN::PutEnv( strangeEnv, no[j] );
        }
#endif
    }
};

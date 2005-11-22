#include "../testCommon.h"

class EnvTest : public CxxTest::TestSuite
{
public:

    void testGetEnv()
    {
        TS_ASSERT( GN::getEnv(0).empty() );
        TS_ASSERT( GN::getEnv("asdfhasdf aslfjoursw").empty() );
        TS_ASSERT( !GN::getEnv("PATH").empty() );
    }

    void testPutEnv()
    {
        // invalid arguments
        GN::putEnv( 0, 0 );
        GN::putEnv( 0, "" );
        GN::putEnv( "", 0 );
        GN::putEnv( "", "" );

        const char * strangeEnv = "asdfhiarnsauraslascfh";

        // make sure strangeEnv do not exist
        TS_ASSERT( GN::getEnv(strangeEnv).empty() );

        // set it to "1"
        GN::putEnv( strangeEnv, "1" );
        TS_ASSERT_EQUALS( GN::getEnv(strangeEnv), "1" );

        // then clear it
        GN::putEnv( strangeEnv, 0 );
        TS_ASSERT( GN::getEnv(strangeEnv).empty() );

        // set it to "2"
        GN::putEnv( strangeEnv, "2" );
        TS_ASSERT_EQUALS( GN::getEnv(strangeEnv), "2" );

        // then clear it in another way.
        GN::putEnv( strangeEnv, "" );
        TS_ASSERT( GN::getEnv(strangeEnv).empty() );
    }

    void testGetEnvBoolean()
    {
        const char * strangeEnv = "testGetEnvBoolean_asdfyawnwauiynasiur";

        // make sure strangeEnv do not exist
        TS_ASSERT( GN::getEnv(strangeEnv).empty() );

        const char * yes[] = {
            "1",
            "true", "TRUE", "True", "tRue",
            "yes", "Yes", "YES", "yEs" };

        const char * no [] = { 0, "", "0", "2", "no", "NO", "No", "soiuran" };

        for( size_t j = 0; j < sizeof(no)/sizeof(no[0]); ++j )
        for( size_t i = 0; i < sizeof(yes)/sizeof(yes[0]); ++i )
        {
            TS_ASSERT( !GN::getEnvBoolean(strangeEnv) );
            GN::putEnv( strangeEnv, yes[i] );
            TS_ASSERT( GN::getEnvBoolean(strangeEnv) );
            GN::putEnv( strangeEnv, no[j] );
        }
    }
};
